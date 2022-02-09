package ir.alihatamitajik.saroyal;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Switch;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class SettingTabFragment extends Fragment {

    @SuppressLint("UseSwitchCompatOrMaterialCode")
    Switch isTime, isDate, isBattery, isTemperature, isRainbow, isPacman, isJalali;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view =  inflater.inflate(R.layout.setting_frag, container, false);
        isTime = view.findViewById(R.id.isTime);
        isDate = view.findViewById(R.id.isDate);
        isBattery = view.findViewById(R.id.isBattery);
        isTemperature = view.findViewById(R.id.isTemperature);
        isRainbow = view.findViewById(R.id.isRainbow);
        isPacman = view.findViewById(R.id.isPacman);
        isJalali = view.findViewById(R.id.isJalali);

        isTime.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isTime.setChecked(!isTime.isChecked());
                }
            }
        }.execute("isShowClock", String.valueOf(isTime.isChecked()?1:0)));

        isDate.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isDate.setChecked(!isDate.isChecked());
                }
            }
        }.execute("isShowDate", String.valueOf(isDate.isChecked()?1:0)));

        isBattery.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isBattery.setChecked(!isBattery.isChecked());
                }
            }
        }.execute("isShowBattery", String.valueOf(isBattery.isChecked()?1:0)));

        isTemperature.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isTemperature.setChecked(!isTemperature.isChecked());
                }
            }
        }.execute("isTemprature", String.valueOf(isTemperature.isChecked()?1:0)));

        isRainbow.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isRainbow.setChecked(!isRainbow.isChecked());
                }
            }
        }.execute("isShowRainbow", String.valueOf(isRainbow.isChecked()?1:0)));


        isPacman.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isPacman.setChecked(!isPacman.isChecked());
                }
            }
        }.execute("isShowPackman", String.valueOf(isPacman.isChecked()?1:0)));


        isJalali.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (status) {
                    if (isJalali.isChecked()) {
                        isJalali.setText(R.string.jalali);
                    } else {
                        isJalali.setText(R.string.georgian);
                    }

                } else {
                    isJalali.setChecked(!isJalali.isChecked());
                }
            }
        }.execute("isJalali", String.valueOf(isJalali.isChecked()?1:0)));

        return view;
    }

}
