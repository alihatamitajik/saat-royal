package ir.alihatamitajik.saroyal;

import android.content.Context;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;

public class TabAdapter extends FragmentPagerAdapter {
    private Context context;
    int totalTab;

    public TabAdapter(FragmentManager fm, Context context, int totalTabs) {
        super(fm);
        this.context = context;
        this.totalTab = totalTabs;
    }

    @Override
    public int getCount() {
        return totalTab;
    }

    public Fragment getItem(int position) {
        switch (position) {
            case 0:
                return new SettingTabFragment();
            case 1:
                return new ColorTabFragment();
            case 2:
                return new AlarmTabFragment();
            default:
                return null;
        }
    }

}
