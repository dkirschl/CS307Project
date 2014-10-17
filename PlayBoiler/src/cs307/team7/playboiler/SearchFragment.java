package cs307.team7.playboiler;

import android.app.Activity;
import android.app.Dialog;
import android.app.Fragment;
import android.app.TimePickerDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.TimePicker;

public class SearchFragment extends Fragment {
	public static SearchFragment newInstance(int sectionNumber) {
        SearchFragment fragment = new SearchFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }
	
	Button dateSelect, timeSelect;
	int m,h;

    public SearchFragment() {
    }

    @Override
    public View onCreateView(final LayoutInflater inflater, final ViewGroup container,
            Bundle savedInstanceState) {
    	
    	
        View rootView = inflater.inflate(R.layout.search, container, false);
        final LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.searchContainer);
        final TextView tvDate = (TextView) rootView.findViewById(R.id.searchEventDate);
        final TextView tvTime = (TextView) rootView.findViewById(R.id.searchEventTime);
        
        dateSelect = (Button) rootView.findViewById(R.id.searchSelectDate);
    	dateSelect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				final Dialog d = new Dialog(v.getContext());
				d.setContentView(R.layout.pick_date);
				d.setTitle("Pick a Date");
				
				final DatePicker dp = (DatePicker) d.findViewById(R.id.datePicker);
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
    	
final TimePickerDialog.OnTimeSetListener timePickerListener = new TimePickerDialog.OnTimeSetListener() {
			
			@Override
			public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
				h = hourOfDay;
				m = minute;
				tvTime.setText(new StringBuilder().append(timeFix(h)).append(":").append(timeFix(m)));
			}
		};
    	timeSelect = (Button) rootView.findViewById(R.id.searchSelectTime);
    	timeSelect.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				TimePickerDialog tpd = new TimePickerDialog(v.getContext(), timePickerListener, h, m, false);
				tpd.show();
			}
		});
        Button search = (Button) rootView.findViewById(R.id.searchButton);
        search.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				StringBuilder str = new StringBuilder();
				str.append("/gtev/");
				addSpaces(str, 30);
				str.append("/");
				addSpaces(str, 30);
				str.append("/");
				addSpaces(str, 10);
				str.append("/");
				addSpaces(str, 5);
				str.append("/");
				addSpaces(str, 1);
				str.append("/");
				str.append("\r\n");
				Log.d("Message", str.toString());
				NetworkHandler nh = new NetworkHandler();
				nh.execute(str.toString());
				
				/*
				try {
					container.getContext().wait(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				*/
				int j = 20;
		        for (int i= 0; i < j; i++) {
		        	View ll = inflater.inflate(R.layout.event_view, container, false);
		        	TextView tv = (TextView) ll.findViewById(R.id.searchEventTitle);
		        	TextView tv2 = (TextView) ll.findViewById(R.id.searchEventDate);
		        	tv.setText("Title : " + i);
		        	tv2.setText("Date : " + i);
		        	contain.addView(ll);
		        	
		        }
			}
		});
        
        
        
        
        //TextView textView = (TextView) rootView.findViewById(R.id.section_label);
        //textView.setText(Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER)));
        return rootView;
    	
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        //((MainActivity) activity).onSectionAttached(
                //getArguments().getInt(ARG_SECTION_NUMBER));
    }
    
    public static StringBuilder addSpaces(StringBuilder sb, int numSpaces) {
		for(int i=0; i < numSpaces; i++) {
			sb.append(" ");
		}
    	return sb;
    	
    }
    
    private static String timeFix(int c) {
    	if (c >= 10) {
    		return String.valueOf(c);
    	} else {
    		return "0" + String.valueOf(c);
    	}
    }
}
