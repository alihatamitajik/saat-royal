package ir.alihatamitajik.saroyal;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.google.android.material.slider.Slider;
import com.google.android.material.textfield.TextInputEditText;


public class ColorTabFragment extends Fragment {

    TextView color60, color12, color24, color12N;
    Slider brightness;
    TextInputEditText color;


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view  = inflater.inflate(R.layout.color_frag, container, false);

        color60 = view.findViewById(R.id.color60);
        color60.setOnClickListener(view1 -> {
            color = view.findViewById(R.id.color);
            new HandlePost().execute("color60", color.getText().toString());
        });

        color24 = view.findViewById(R.id.color24);
        color24.setOnClickListener(view1 -> {
            color = view.findViewById(R.id.color);
            new HandlePost().execute("color24", color.getText().toString());
        });

        color12 = view.findViewById(R.id.color12);
        color24.setOnClickListener(view1 -> {
            color = view.findViewById(R.id.color);
            new HandlePost().execute("color12",color.getText().toString());
        });

        color12N = view.findViewById(R.id.color12N);
        color12N.setOnClickListener(view1 -> {
            color = view.findViewById(R.id.color);
            new HandlePost().execute("color12N", color.getText().toString());
        });

        return view;
    }
}
