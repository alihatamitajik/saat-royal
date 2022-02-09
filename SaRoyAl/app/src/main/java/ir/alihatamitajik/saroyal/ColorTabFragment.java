package ir.alihatamitajik.saroyal;

import android.app.AlertDialog;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.cardview.widget.CardView;
import androidx.fragment.app.Fragment;

import com.google.android.material.slider.Slider;


public class ColorTabFragment extends Fragment {

    Slider selectR, selectG, selectB, brightness;
    Button color60, color24, color12, color12N;
    View colorSelect;
    TextView clkMode;
    TextView FFMode;
    CardView FFModeCard;

    long RGB(int r, int g, int b) {
        return (long) r << 16 + g << 8 + b;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view  = inflater.inflate(R.layout.color_frag, container, false);
        selectR = view.findViewById(R.id.selectR);
        selectG = view.findViewById(R.id.selectG);
        selectB = view.findViewById(R.id.selectB);
        color60 = view.findViewById(R.id.color60);
        color12 = view.findViewById(R.id.color12);
        color24 = view.findViewById(R.id.color24);
        color12N = view.findViewById(R.id.color12N);
        colorSelect = view.findViewById(R.id.colorSelect);
        brightness = view.findViewById(R.id.brightness);
        clkMode = view.findViewById(R.id.clkMode);
        FFModeCard = view.findViewById(R.id.FFModeCard);
        FFMode = view.findViewById(R.id.FFMode);

        FFMode.setOnClickListener(view1 -> {
            AlertDialog.Builder alt_bld = new AlertDialog.Builder(view.getContext());
            //alt_bld.setIcon(R.drawable.icon);
            String[] grpname = new String[]{"Day","Night", "Evening"};
            alt_bld.setTitle("Select a Mode:");
            alt_bld.setSingleChoiceItems(grpname, -1, (dialog, item) -> {
                new HandlePost(){

                    @Override
                    public void onPostExecute(Boolean status) {
                        if (status) {
                            FFMode.setText(grpname[item]);
                        } else {
                            Toast.makeText(view.getContext(),"Try Again", Toast.LENGTH_SHORT).show();
                        }
                    }
                }.execute("ffMode", String.valueOf(item));
                dialog.dismiss();// dismiss the alertbox after chose option
            });
            AlertDialog alert = alt_bld.create();
            alert.show();
        });

        clkMode.setOnClickListener(view1 -> {
            AlertDialog.Builder alt_bld = new AlertDialog.Builder(view.getContext());
            //alt_bld.setIcon(R.drawable.icon);
            String[] grpname = new String[]{"Full Face Clock","Swing Clock", "Simple Clock"};
            alt_bld.setTitle("Select a Mode:");
            alt_bld.setSingleChoiceItems(grpname, -1, (dialog, item) -> {
                new HandlePost(){

                    @Override
                    public void onPostExecute(Boolean status) {
                        if (status) {
                            clkMode.setText(grpname[item]);
                            if (item == 0) {
                                FFModeCard.setVisibility(CardView.VISIBLE);
                            } else {
                                FFModeCard.setVisibility(CardView.INVISIBLE);
                            }
                        } else {
                            Toast.makeText(view.getContext(),"Try Again", Toast.LENGTH_SHORT).show();
                        }
                    }
                }.execute("modeClock", String.valueOf(item));
                dialog.dismiss();// dismiss the alertbox after chose option
            });
            AlertDialog alert = alt_bld.create();
            alert.show();
        });

        brightness.addOnSliderTouchListener(new Slider.OnSliderTouchListener() {
            private int preValue;

            @Override
            public void onStartTrackingTouch(@NonNull Slider slider) {
                preValue = (int) slider.getValue();
            }

            @Override
            public void onStopTrackingTouch(@NonNull Slider slider) {
                new HandlePost(){

                    @Override
                    public void onPostExecute(Boolean status) {
                        if (status){
                            brightness.setValue((int)slider.getValue());
                        } else {
                            brightness.setValue(preValue);
                        }
                    }
                }.execute("brightness",String.valueOf((int)brightness.getValue()));
            }
        });


        selectR.addOnChangeListener((slider, value, fromUser) -> updateColorSelect());
        selectG.addOnChangeListener((slider, value, fromUser) -> updateColorSelect());
        selectB.addOnChangeListener((slider, value, fromUser) -> updateColorSelect());


        color60.setOnClickListener(view1 -> {
            String hex = String.format("%02x%02x%02x", (int)selectR.getValue(),
                    (int)selectG.getValue(),
                    (int)selectB.getValue());
            new HandlePost(){
                @Override
                public void onPostExecute(Boolean status) {
                    if (status) {
                        color60.setTextColor(Color.rgb((int)selectR.getValue(),
                                (int)selectG.getValue(),
                                (int)selectB.getValue()));
                    }
                }
            }.execute("color60",String.valueOf(Long.parseLong(hex,16)));
        });

        color24.setOnClickListener(view1 -> {
            String hex = String.format("%02x%02x%02x", (int)selectR.getValue(),
                    (int)selectG.getValue(),
                    (int)selectB.getValue());
            new HandlePost(){
                @Override
                public void onPostExecute(Boolean status) {
                    if (status) {
                        color24.setTextColor(Color.rgb((int)selectR.getValue(),
                                (int)selectG.getValue(),
                                (int)selectB.getValue()));
                    }
                }
            }.execute("color24",String.valueOf(Long.parseLong(hex,16)));
        });

        color12.setOnClickListener(view1 -> {
            String hex = String.format("%02x%02x%02x", (int)selectR.getValue(),
                    (int)selectG.getValue(),
                    (int)selectB.getValue());
            new HandlePost(){
                @Override
                public void onPostExecute(Boolean status) {
                    if (status) {
                        color12.setTextColor(Color.rgb((int)selectR.getValue(),
                                (int)selectG.getValue(),
                                (int)selectB.getValue()));
                    }
                }
            }.execute("color12",String.valueOf(Long.parseLong(hex,16)));
        });

        color12N.setOnClickListener(view1 -> {
            String hex = String.format("%02x%02x%02x", (int)selectR.getValue(),
                    (int)selectG.getValue(),
                    (int)selectB.getValue());
            new HandlePost(){
                @Override
                public void onPostExecute(Boolean status) {
                    if (status) {
                        color12N.setTextColor(Color.rgb((int)selectR.getValue(),
                                (int)selectG.getValue(),
                                (int)selectB.getValue()));
                    }
                }
            }.execute("color12N",String.valueOf(Long.parseLong(hex,16)));
        });
        return view;
    }

    private void updateColorSelect() {
        colorSelect.getBackground()
                .setColorFilter(Color.rgb((int)selectR.getValue(),
                        (int)selectG.getValue(),
                        (int)selectB.getValue()),
                PorterDuff.Mode.SRC_ATOP);
    }


}
