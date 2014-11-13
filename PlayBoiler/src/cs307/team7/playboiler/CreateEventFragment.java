package cs307.team7.playboiler;

import java.util.List;
import java.util.concurrent.ExecutionException;

import android.app.Dialog;
import android.app.Fragment;
import android.app.TimePickerDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

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
    	final TextView sportEdit = (TextView) rootView.findViewById(R.id.selectSport);
    	final EditText locEdit = (EditText) rootView.findViewById(R.id.selectLocation);
    	final EditText sumEdit = (EditText) rootView.findViewById(R.id.setSummary);
    	final EditText maxPlay = (EditText) rootView.findViewById(R.id.setCapacity);
    	final Button chooseSport = (Button) rootView.findViewById(R.id.chooseSport);
    	final EditText compete = (EditText) rootView.findViewById(R.id.compete);
    	compete.setFilters(new InputFilterMinMax[]{ new InputFilterMinMax("1", "3")});
    	chooseSport.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				Dialog d = new Dialog(arg0.getContext());
				d.setContentView(R.layout.sport_spinner);
				d.setTitle("Select Sport");
				//get list of all sports
				List<String> allSports = (List<String>) Global.userDatabase.getSports();
				Log.d("Test", "Ping");
				for (int i = 0; i < allSports.size(); i++) {
					Log.d("Sports", allSports.get(i));
				}
				final Spinner spinner = (Spinner) d.findViewById(R.id.sportsSpinner);
				ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(arg0.getContext(), android.R.layout.simple_spinner_dropdown_item, allSports);
				dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				spinner.setAdapter(dataAdapter);
				d.show();
				//display as clickable text in a dialog
				d.setOnCancelListener(new OnCancelListener() {
					
					@Override
					public void onCancel(DialogInterface dialog) {
						sportEdit.setText(String.valueOf(spinner.getSelectedItem()));
					}
				});
				//display as clickable text in a dialog
				//return with selected event
			}
    		
    	});
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
						//StringBuilder date = new StringBuilder().append((strMon)).append("-").append(strDay).append("-").append(dp.getYear());
						StringBuilder date = new StringBuilder().append(dp.getYear()).append(strMon).append(strDay);
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
				String edittedDate = tvDate.getText().toString();
				String edittedTime = tvTime.getText().toString();
				edittedTime = edittedTime.replace(":", "");
				edittedDate = edittedDate.replace("-", "");
				
				int sportLen = 30;
				int locLen = 30;
				int dateLen = 8; //was 10
				int timeLen = 4; //was 5
				int titleLen = 25;
				int sumLen = 100;
				int passLen = 20;
				int maxPlayerLen = 3;
				StringBuilder str = new StringBuilder();
				str.append("/crev/");
				str.append(Global.current_user.getKey());
				Global.addSpaces(str, 4-(String.valueOf(Global.current_user.getKey()).length()));
				str.append("/");
				String password = Global.current_user.getPassword();
				str.append(password);
				Global.addSpaces(str, passLen - password.length());
				str.append("/");
				str.append(sportEdit.getText().toString());
				Global.addSpaces(str, sportLen - sportEdit.getText().toString().length());
				str.append("/");
				str.append(locEdit.getText().toString());
				Global.addSpaces(str, locLen - locEdit.getText().toString().length());
				str.append("/");
				//String date = event.getDate();
				//date = date.replace("-", "");
				str.append(edittedDate);
				Global.addSpaces(str, dateLen - edittedDate.length());
				str.append("/");
				//String time = event.getTime();
				//time = time.replace(":", "");
				str.append(edittedTime);
				Global.addSpaces(str, timeLen-edittedTime.length());
				str.append("/");
				//summary
				String summary = sumEdit.getText().toString();
				str.append(summary);
				Global.addSpaces(str, sumLen-summary.length());
				str.append("/");
				//comp
				str.append(compete.getText());
				str.append("/");
				str.append(titleEdit.getText().toString());
				Global.addSpaces(str, titleLen - titleEdit.getText().toString().length());
				str.append("/");
				str.append(maxPlay.getText().toString());
				//String maxPlayersString = ""+event.getMaxPlayers();
				Global.addSpaces(str, maxPlayerLen - maxPlay.getText().toString().length());
				str.append("/");
				str.append("\r\n");
				Log.d("The Message", str.toString());
				
				NetworkHandler nh = new NetworkHandler();
				
				
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
					result = result.replace("|", "");
					int res = Integer.parseInt(result.substring(7));
					Log.d("Create", ""+res);
					//event.setKey(res);
					event = new Event(res,sportEdit.getText().toString(), locEdit.getText().toString(), edittedDate, edittedTime, titleEdit.getText().toString(), ""+Global.current_user.getKey(), sumEdit.getText().toString(),Integer.parseInt(maxPlay.getText().toString()), Global.USER_CREATED_EVENT_CODE, Global.USER_CREATED_YES);
					event.setSpecificUser(Global.current_user.getAlias());
					sportEdit.setText("");
					locEdit.setText("");
					tvDate.setText("");
					tvTime.setText("");
					titleEdit.setText("");
					sumEdit.setText("");
					maxPlay.setText("");
					compete.setText("");
					Global.userDatabase.addEvent(event);
					Toast.makeText(v.getContext(), "Event successfully created. Check the \"View Events\" page to see the event.",Toast.LENGTH_LONG).show();
				}
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
