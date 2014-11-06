package cs307.team7.playboiler;

import java.util.Calendar;
import java.util.concurrent.ExecutionException;

import android.app.Dialog;
import android.app.Fragment;
import android.app.TimePickerDialog;
import android.app.TimePickerDialog.OnTimeSetListener;
import android.os.Bundle;
import android.util.Log;
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
    	final EditText sumEdit = (EditText) rootView.findViewById(R.id.setSummary);
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
						int month = dp.getMonth() + 1;
						String strMon;
						if (month < 10) {
							strMon = "0"+month;
						} else {
							strMon = ""+month;
						}
						int day = dp.getDayOfMonth();
						String strDay;
						if (day < 10) {
							strDay = "0"+day;
						} else {
							strDay = ""+day;
						}
						StringBuilder date = new StringBuilder().append((strMon)).append("-").append(strDay).append("-").append(dp.getYear());
						//StringBuilder date = new StringBuilder().append((dp.getMonth() + 1)).append("-").append(dp.getDayOfMonth()).append("-").append(dp.getYear());
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
				event = new Event(sportEdit.getText().toString(), locEdit.getText().toString(), tvDate.getText().toString(), tvTime.getText().toString(), titleEdit.getText().toString(), ""+Global.current_user.getKey(), sumEdit.getText().toString(), 2);
				
				int sportLen = 30;
				int locLen = 30;
				int dateLen = 8; //was 10
				int timeLen = 4; //was 5
				int titleLen = 25;
				int sumLen = 100;
				int passLen = 20;
				StringBuilder str = new StringBuilder();
				str.append("/crev/");
				str.append(Global.current_user.getKey());
				addSpaces(str, 4-(String.valueOf(Global.current_user.getKey()).length()));
				str.append("/");
				String password = Global.current_user.getPassword();
				str.append(password);
				addSpaces(str, passLen - password.length());
				str.append("/");
				str.append(event.getSport());
				addSpaces(str, sportLen - event.getSport().length());
				str.append("/");
				str.append(event.getLocation());
				addSpaces(str, locLen - event.getLocation().length());
				str.append("/");
				String date = event.getDate();
				date = date.replace("-", "");
				str.append(date);
				addSpaces(str, dateLen - date.length());
				str.append("/");
				String time = event.getTime();
				time = time.replace(":", "");
				str.append(time);
				addSpaces(str, timeLen-time.length());
				str.append("/");
				//summary
				String summary = event.getSummary();
				str.append(summary);
				addSpaces(str, sumLen-summary.length());
				str.append("/");
				//comp
				str.append(1);
				str.append("/");
				str.append(event.getTitle());
				addSpaces(str, titleLen - event.getTitle().length());
				str.append("/");
				str.append("\r\n");
				Log.d("The Message", str.toString());
				//String m = "/crev/"+event.getSport()+"/"+event.getLocation()+"/"+event.getDate()+"/"+event.getTime()+"/"+event.getTitle()+"/"+event.getCreating_user()+"/0/ HTTP/1.0\n\r";
				//new NetworkHandler().execute(m);
				NetworkHandler nh = new NetworkHandler();
				//new NetworkHandler().execute(str.toString());
				sportEdit.setText("");
				locEdit.setText("");
				tvDate.setText("");
				tvTime.setText("");
				titleEdit.setText("");
				Toast.makeText(v.getContext(), "Event successfully created. Check the \"View Events\" page to see the event.",Toast.LENGTH_LONG).show();
				String result = null;
				try {
					result = nh.execute(str.toString()).get();
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} catch (ExecutionException e1) {
					e1.printStackTrace();
				}
				if (result.equals("INVALID")) {
					Toast.makeText(v.getContext(), "Event Not Created", Toast.LENGTH_LONG).show();
					
				} else {
					int res = Integer.parseInt(result.substring(7));
					event.setKey(res);
					Global.userDatabase.addEvent(event);
				}
			}
    		
    	});
    	
    	return rootView;
    	
    
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
