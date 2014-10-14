package cs307.team7.playboiler;

import java.util.Calendar;

import android.app.Dialog;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebView.FindListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TextView;
import android.widget.TimePicker;

public class CreateEventFragment extends Fragment {
	
	private int year;
	private int month;
	private int day;
	Button dateSelect;

	public static CreateEventFragment newInstance(int sectionNumber) {
        CreateEventFragment fragment = new CreateEventFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public CreateEventFragment() {
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	final View rootView = inflater.inflate(R.layout.create_event, container, false);
    	final TextView tvDate = (TextView) rootView.findViewById(R.id.eventDate);
    	dateSelect = (Button) rootView.findViewById(R.id.selectDate);
    	dateSelect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				final Dialog d = new Dialog(v.getContext());
				d.setContentView(R.layout.pick_date);
				d.setTitle("Pick a Date");
				
				final DatePicker dp = (DatePicker) d.findViewById(R.id.datePicker);
				TimePicker tp = (TimePicker) d.findViewById(R.id.timePicker);
				Button done = (Button) d.findViewById(R.id.dateDone);
				done.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v1) {
						StringBuilder date = new StringBuilder().append((dp.getMonth() + 1)).append("-").append(dp.getDayOfMonth()).append("-").append(dp.getYear());
						tvDate.setText(date);
						d.cancel();
					}
				});
				d.show();
			}
    		
    	});
    	/*
    	DatePicker dp = (DatePicker) rootView.findViewById(R.id.datePicker);
    	final Calendar c = Calendar.getInstance();
    	year = c.get(Calendar.YEAR);
    	month = c.get(Calendar.MONTH);
    	day = c.get(Calendar.DAY_OF_MONTH);
    	
    	dp.init(year, month, day, null);
		*/		
    	
    	return rootView;
    	
    
    }
}
