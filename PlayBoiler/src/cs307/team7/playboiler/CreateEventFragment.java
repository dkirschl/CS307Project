package cs307.team7.playboiler;

import java.util.Calendar;

import android.app.Dialog;
import android.app.Fragment;
import android.app.TimePickerDialog;
import android.app.TimePickerDialog.OnTimeSetListener;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebView.FindListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;
import android.widget.TimePicker.OnTimeChangedListener;

public class CreateEventFragment extends Fragment {
	
	
	Button dateSelect, timeSelect;
	Event event;
	Button createEvent;
	int h;
	int m;

	public static CreateEventFragment newInstance(int sectionNumber) {
        CreateEventFragment fragment = new CreateEventFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public CreateEventFragment() {
    	h=0;
    	m=0;
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	final View rootView = inflater.inflate(R.layout.create_event, container, false);
    	final EditText titleEdit = (EditText) rootView.findViewById(R.id.setEventTitle);
    	final TextView tvDate = (TextView) rootView.findViewById(R.id.eventDate);
    	final TextView tvTime = (TextView) rootView.findViewById(R.id.eventTime);
    	final EditText sportEdit = (EditText) rootView.findViewById(R.id.selectSport);
    	final EditText locEdit = (EditText) rootView.findViewById(R.id.selectLocation);
    	dateSelect = (Button) rootView.findViewById(R.id.selectDate);
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
    	timeSelect = (Button) rootView.findViewById(R.id.selectTime);
    	timeSelect.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				TimePickerDialog tpd = new TimePickerDialog(v.getContext(), timePickerListener, h, m, false);
				tpd.show();
			}
		});
    		
    	createEvent = (Button) rootView.findViewById(R.id.createEvent);
    	createEvent.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				event = new Event(1, sportEdit.getText().toString(), locEdit.getText().toString(), tvDate.getText().toString(), tvTime.getText().toString(), titleEdit.getText().toString(), ""+Global.current_user.getKey(), 0);
				Global.userDatabase.addEvent(event);
				sportEdit.setText("");
				locEdit.setText("");
				tvDate.setText("");
				tvTime.setText("");
				titleEdit.setText("");
				Toast.makeText(v.getContext(), "Event successfully created. Check the \"View Events\" page to see the event.",Toast.LENGTH_LONG).show();
			}
    		
    	});
    	
    	return rootView;
    	
    
    }
    private static String timeFix(int c) {
    	if (c >= 10) {
    		return String.valueOf(c);
    	} else {
    		return "0" + String.valueOf(c);
    	}
    }
    
}
