package ir.alihatamitajik.saroyal;

import android.annotation.SuppressLint;
import android.app.TimePickerDialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.TimePicker;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import java.util.Calendar;

public class AlarmTabFragment extends Fragment {

    TextView alarm;
    Switch isAlarm;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view =  inflater.inflate(R.layout.alarm_frag, container, false);
        alarm = view.findViewById(R.id.alarmTime);
        isAlarm = view.findViewById(R.id.isAlarm);

        alarm.setOnClickListener(v -> {
            final Calendar cldr = Calendar.getInstance();
            int hour = cldr.get(Calendar.HOUR_OF_DAY);
            int minutes = cldr.get(Calendar.MINUTE);
            // time picker dialog
            TimePickerDialog picker = new TimePickerDialog(view.getContext(),
                    (tp, sHour, sMinute) -> new HandlePost(){
                        @Override
                        public void onPostExecute(Boolean status) {
                            if (status) {
                                alarm.setText(String.format("%d:%d", sHour, sMinute));
                            }
                        }
                    }.execute("alarmH", String.valueOf(sHour),
                            "alarmM", String.valueOf(sMinute))
                    , hour, minutes, true);
            picker.show();
        });

        isAlarm.setOnClickListener(view1 -> new HandlePost(){
            @Override
            public void onPostExecute(Boolean status) {
                if (!status) {
                    isAlarm.setChecked(!isAlarm.isChecked());
                }
            }
        }.execute("isAlarm", String.valueOf(isAlarm.isChecked()?1:0)));


        return view;
    }
}
