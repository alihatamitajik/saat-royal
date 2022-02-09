package ir.alihatamitajik.saroyal;

import static android.graphics.Color.HSVToColor;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.lifecycle.MutableLiveData;
import androidx.viewpager.widget.ViewPager;

import android.content.res.ColorStateList;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.transition.Slide;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.progressindicator.CircularProgressIndicator;
import com.google.android.material.slider.Slider;
import com.google.android.material.tabs.TabLayout;
import com.google.gson.Gson;

import org.w3c.dom.Text;

import java.io.IOException;
import java.util.Calendar;
import java.util.Locale;

import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {

    TabLayout tabLayout;
    ViewPager viewPager;

    FloatingActionButton sync;
    FloatingActionButton adjust;
    FloatingActionButton operate;

    CircularProgressIndicator batteryPercentage;
    TextView batteryText;

    CircularProgressIndicator temperaturePercentage;
    TextView temperatureText;

    public static double mapOneRangeToAnother(double sourceNumber, double fromA, double fromB, double toA, double toB, int decimalPrecision ) {
        double deltaA = fromB - fromA;
        double deltaB = toB - toA;
        double scale  = deltaB / deltaA;
        double negA   = -1 * fromA;
        double offset = (negA * scale) + toA;
        double finalNumber = (sourceNumber * scale) + offset;
        int calcScale = (int) Math.pow(10, decimalPrecision);
        return (double) Math.round(finalNumber * calcScale) / calcScale;
    }

    TextView status;

    boolean isConnected;

    Switch isTime;
    Switch isDate;
    Switch isBattery;
    Switch isTemp;
    Switch isRain;
    Switch isPac;
    Switch isJalali;

    float v = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tabLayout = findViewById(R.id.tab_layout);
        viewPager = findViewById(R.id.view_pager);
        batteryPercentage = findViewById(R.id.batteryPercentage);
        batteryText = findViewById(R.id.batteryText);
        temperaturePercentage = findViewById(R.id.temperaturePercentage);
        temperatureText = findViewById(R.id.temperatureText);
        status = findViewById(R.id.status);
        sync = findViewById(R.id.sync);
        adjust = findViewById(R.id.adjust);
        operate = findViewById(R.id.operate);

        tabLayout.addTab(tabLayout.newTab().setText(R.string.setting));
        tabLayout.addTab(tabLayout.newTab().setText(R.string.colors));
        tabLayout.addTab(tabLayout.newTab().setText(R.string.alarm));

        tabLayout.setTabGravity(TabLayout.GRAVITY_FILL);

        final TabAdapter adapter = new TabAdapter(getSupportFragmentManager(),
                this,
                tabLayout.getTabCount());

        viewPager.setAdapter(adapter);

        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));


        sync.setOnClickListener(view -> new SyncHandler().execute());
        operate.setOnClickListener(view -> new OperateHandler().execute());

        adjust.setOnClickListener(view -> {
            final Calendar cldr = Calendar.getInstance();
            int hh = cldr.get(Calendar.HOUR_OF_DAY);
            int mm = cldr.get(Calendar.SECOND);
            int ss = cldr.get(Calendar.MINUTE);
            int YYYY = cldr.get(Calendar.YEAR);
            int MM = cldr.get(Calendar.MONTH);
            int DD = cldr.get(Calendar.DAY_OF_MONTH);

            new AdjustHandler().execute("YYYY",String.valueOf(YYYY-48),
                    "MM",String.valueOf(MM+1),
                    "DD",String.valueOf(DD),
                    "hh",String.valueOf(hh),
                    "mm",String.valueOf(mm),
                    "ss",String.valueOf(ss));
        });
    }

    public class AdjustHandler extends AsyncTask<String, Void, Boolean> {
        @Override
        protected Boolean doInBackground(String[] objects) {
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            MediaType mediaType = MediaType.parse("text/plain");
            MultipartBody.Builder bodyBuilder = new MultipartBody.Builder().setType(MultipartBody.FORM);
            for (int i = 0; i < objects.length/2; i+=1) {
                bodyBuilder.addFormDataPart(objects[2*i],objects[2*i+1]);
            }
            RequestBody body = bodyBuilder.build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/adjust")
                    .method("POST", body)
                    .build();
            try {
                Response response = client.newCall(request).execute();
                return response.code() == 200;
            } catch (IOException e) {
                e.printStackTrace();
                return false;
            }
        }
    }

    public class OperateHandler extends AsyncTask {

        @Override
        protected Object doInBackground(Object[] objects) {
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/operate")
                    .method("GET", null)
                    .build();
            try {
                Response response = client.newCall(request).execute();
                String s = response.body().string();
                isConnected = response.code() == 200;
                return s;
            } catch (Exception e) {
                e.printStackTrace();
                isConnected = false;
            }
            return null;
        }
    }

    public class SyncHandler extends AsyncTask {

        boolean sendTest() throws IOException {
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            return response.code() == 200;
        }

        boolean sendReady() throws IOException {
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/ready")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            return  response.code() == 200;
        }

        void updateStatusFields(StatusEntity entity) {
            runOnUiThread(() -> {
                batteryText.setText(entity.getBattery() + "%");
                batteryPercentage.setProgress((entity.getBattery()*75/100));
                batteryPercentage.setIndicatorColor(HSVToColor (new float[]{(entity.getBattery()*120/100), 1.0F, 1.0F}));
                status.setText(entity.Current);
                temperaturePercentage.setProgress((entity.temprature + 40) * 100 / 80);
                temperatureText.setText(entity.temprature + "°");
                batteryPercentage.setIndicatorColor(HSVToColor (
                        new float[]{(float) (120 - mapOneRangeToAnother(entity.temprature, -80,
                                80,0,120,
                                2))
                                , 1.0F, 1.0F}
                        ));
            });
        }

        boolean updateStatus() throws IOException{
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/status")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            Gson gson = new Gson();
            StatusEntity entity = gson.fromJson(response.body().string(), StatusEntity.class);
            updateStatusFields(entity);
            return response.code() == 200;
        }

        void updateSettingFields(SettingEntity entity) {
            runOnUiThread(() -> {
                isTime = findViewById(R.id.isTime);
                isTime.setChecked(entity.isShowClock==1);

                isDate = findViewById(R.id.isDate);
                isDate.setChecked(entity.isShowDate==1);

                isTemp = findViewById(R.id.isTemperature);
                isTemp.setChecked(entity.isTemprature==1);

                isBattery = findViewById(R.id.isBattery);
                isBattery.setChecked(entity.isShowBattery==1);

                isRain = findViewById(R.id.isRainbow);
                isRain.setChecked(entity.isShowRainbow==1);

                isPac = findViewById(R.id.isPacman);
                isPac.setChecked(entity.isShowPackman==1);

                isJalali = findViewById(R.id.isJalali);
                if (entity.isJalali==1) {
                    isJalali.setText(R.string.jalali);
                    isJalali.setChecked(true);
                } else {
                    isJalali.setText(R.string.georgian);
                    isJalali.setChecked(false);
                }
            });
        }

        boolean updateSetting() throws IOException {
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/setting")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            Gson gson = new Gson();
            SettingEntity entity = gson.fromJson(response.body().string(), SettingEntity.class);
            updateSettingFields(entity);
            return response.code() == 200;
        }

        String getFaceMode(int mode) {
            switch (mode) {
                case 0: return "Full Face Clock";
                case 1: return "Swing Clock";
                case 2: return "Simple Clock";
                default:
                    return "Sync Again";
            }
        }

        String getFFMode(int mode) {
            switch (mode) {
                case 0: return "Day";
                case 1: return "Night";
                case 2: return "Evening";
                default:
                    return "Sync Again";
            }
        }

        void updateAlarmFields(AlarmEntity entity) {
            runOnUiThread(() -> {
                Switch isAlarm = findViewById(R.id.isAlarm);
                if (entity.isAlarm == 1) {
                    isAlarm.setChecked(true);
                    isAlarm.setText(R.string.on);
                } else {
                    isAlarm.setChecked(false);
                    isAlarm.setText(R.string.off);
                }

                TextView alarm = findViewById(R.id.alarmTime);
                alarm.setText(String.format("%02d:%02d",entity.alarmH ,entity.alarmM));
            });
        }


        private boolean updateAlarm() throws IOException{
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/alarm")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            Gson gson = new Gson();
            AlarmEntity entity = gson.fromJson(response.body().string(), AlarmEntity.class);
            updateAlarmFields(entity);
            return response.code() == 200;
        }


        void updateColorFields(ColorEntity colorEntity) {
            runOnUiThread(() -> {
                if (colorEntity.modeClock == 0){
                    CardView cardView = findViewById(R.id.FFModeCard);
                    cardView.setVisibility(CardView.VISIBLE);
                } else {
                    CardView cardView = findViewById(R.id.FFModeCard);
                    cardView.setVisibility(CardView.INVISIBLE);
                }
                TextView mode = findViewById(R.id.clkMode);
                mode.setText(getFaceMode(colorEntity.modeClock));
                TextView ffmode = findViewById(R.id.FFMode);
                ffmode.setText(getFFMode(colorEntity.ffMode));

                Button color60 = findViewById(R.id.color60);
                color60.setTextColor(0xff000000 +(int) colorEntity.color60);

                Button color24 = findViewById(R.id.color24);
                color24.setTextColor(0xff000000 +(int) colorEntity.color24);


                Button color12 = findViewById(R.id.color12);
                color12.setTextColor(0xff000000 +(int) colorEntity.color12);;


                Button color12N = findViewById(R.id.color12N);
                color12N.setTextColor(0xff000000 +(int) colorEntity.color12N);


                Slider br = findViewById(R.id.brightness);
                br.setValue(colorEntity.brightness);

            });
        }

        private boolean updateColors() throws IOException{
            OkHttpClient client = new OkHttpClient().newBuilder()
                    .build();
            Request request = new Request.Builder()
                    .url("http://192.168.4.1/colors")
                    .method("GET", null)
                    .build();
            Response response = client.newCall(request).execute();
            Gson gson = new Gson();
            ColorEntity entity = gson.fromJson(response.body().string(), ColorEntity.class);
            updateColorFields(entity);
            return response.code() == 200;


        }

        @Override
        protected Object doInBackground(Object[] objects) {
            try {
                if(sendTest()) {
                    if(sendReady()) {
                        Thread.sleep(500);
                        updateStatus();
                        switch (tabLayout.getSelectedTabPosition()){
                            case 0: updateSetting(); break;
                            case 1: updateColors(); break;
                            case 2: updateAlarm(); break;
                        }
                    }
                }
            } catch (Exception e) {
//                status.setText(R.string.error_wifi);
                e.printStackTrace();
            }
            return null;
        }

    }
}