package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.app.TimePickerDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Path.FillType;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

public class SearchFragment extends Fragment {
	public static SearchFragment newInstance(int sectionNumber) {
        SearchFragment fragment = new SearchFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }
	
	Button dateSelect, timeSelect, dateSelectEnd, timeSelectEnd;
	int m,h;
	ArrayList<String> titles, dates;

    public SearchFragment() {
    }

    @Override
    public View onCreateView(final LayoutInflater inflater, final ViewGroup container,
            Bundle savedInstanceState) {
    	
    	
        View rootView = inflater.inflate(R.layout.search, container, false);
        final Context c = rootView.getContext();
        final LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.searchContainer);
        final TextView searchSport = (TextView) rootView.findViewById(R.id.searchSport);
        final EditText searchLocation = (EditText) rootView.findViewById(R.id.searchLocation);
        final TextView tvDate = (TextView) rootView.findViewById(R.id.searchEventDate);
        tvDate.setClickable(true);
        final TextView tvTime = (TextView) rootView.findViewById(R.id.searchEventTime);
        tvTime.setClickable(true);
        final EditText compete = (EditText) rootView.findViewById(R.id.compEdit);
        compete.setFilters(new InputFilterMinMax[]{ new InputFilterMinMax("1", "3")});
        final TextView tvDateEnd = (TextView) rootView.findViewById(R.id.searchEventDateEnd);
        tvDateEnd.setClickable(true);
        final TextView tvTimeEnd = (TextView) rootView.findViewById(R.id.endTimeTextField);
        tvTimeEnd.setClickable(true);
        searchSport.setOnClickListener(new OnClickListener() {

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
						searchSport.setText(String.valueOf(spinner.getSelectedItem()));
					}
				});
				//display as clickable text in a dialog
				//return with selected event
			}
    		
    	});
        titles = new ArrayList<String>();
    	dates  = new ArrayList<String>();
        
        
        tvDate.setOnClickListener(new Global.dateClickListener(tvDate));
        
        final TimePickerDialog.OnTimeSetListener timePickerListener = new TimePickerDialog.OnTimeSetListener() {
			
			@Override
			public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
				h = hourOfDay;
				m = minute;
				tvTime.setText(new StringBuilder().append(timeFix(h)).append(":").append(timeFix(m)));
			}
		};
		final TimePickerDialog.OnTimeSetListener timePickerListener2 = new TimePickerDialog.OnTimeSetListener() {
			
			@Override
			public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
				h = hourOfDay;
				m = minute;
				tvTimeEnd.setText(new StringBuilder().append(timeFix(h)).append(":").append(timeFix(m)));
			}
		};
    	
    	tvTime.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				TimePickerDialog tpd = new TimePickerDialog(v.getContext(), timePickerListener, h, m, false);
				tpd.show();
			}
		});
    	
    	tvDateEnd.setOnClickListener(new Global.dateClickListener(tvDateEnd));
    	
       	tvTimeEnd.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				TimePickerDialog tpd2 = new TimePickerDialog(v.getContext(), timePickerListener2, h, m, false);
				tpd2.show();
				
			}
		});
    	
        Button search = (Button) rootView.findViewById(R.id.searchButton);
        search.setClickable(true);
        search.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Log.d("Search Button", "CLICKED");
				String sport = searchSport.getText().toString();
				String location = searchLocation.getText().toString();
				String date = tvDate.getText().toString();
				String dateEnd = tvDateEnd.getText().toString();
				String time = tvTime.getText().toString();
				String timeEnd = tvTimeEnd.getText().toString();
				String comp = compete.getText().toString();
				
				StringBuilder str = new StringBuilder();
				str.append("/gtev/");
				str.append(sport);
				Global.addSpaces(str, 30-sport.length());
				str.append("/");
				str.append(location);
				Global.addSpaces(str, 30-location.length());
				str.append("/");
				date = date.replace("-", "");
				str.append(date);
				Global.addSpaces(str, 8-date.length());
				str.append("/");
				time = time.replace(":", "");
				str.append(time);
				Global.addSpaces(str, 4-time.length());
				str.append("/");
				str.append(comp);
				Global.addSpaces(str, 1-comp.length()); //comp level
				str.append("/");
				str.append("                         ");
				str.append("/");
				dateEnd = dateEnd.replace("-", "");
				str.append(dateEnd);
				Global.addSpaces(str, 8-date.length());
				str.append("/");
				timeEnd=timeEnd.replace(":", "");
				str.append(timeEnd);
				Global.addSpaces(str, 4-time.length());
				str.append("/");
				str.append("\r\n");
				Log.d("Message", str.toString());
				NetworkHandler nh = new NetworkHandler();
				String result = null;
				try {
					result = nh.execute(str.toString()).get();
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} catch (ExecutionException e1) {
					e1.printStackTrace();
				}
				Log.d("Search Event", result);
				String[][] s = split(result);
				for (int i = 0; i < s.length; i++) {
					for (int j = 0; j < s[i].length; j++) {
						Log.d("PART", "" + i + "," + j + ":" +s[i][j] +":");
					}
				}
				Log.d("Len", ""+s.length);
				contain.removeAllViews();
				for (int i = 0; i < s.length; i++) {
					View ll = inflater.inflate(R.layout.event_view, container, false);
		        	TextView tv = (TextView) ll.findViewById(R.id.searchEventTitle);
		        	TextView tv2 = (TextView) ll.findViewById(R.id.searchEventDate);
		        	Event e = new Event();
		        	//Log.d("Info", s[i][0] + s[i][1] + s[i][2] + s[i][3] + s[i][4]);
		        	//e.setTitle(s[i][4]); e.setDate(s[i][2]); e.setTime(s[i][3]); e.setLocation(s[i][0]); e.setSport(s[i][1]);
		        	//e.setTitle(s[i][0]); e.setDate(s[i][1]);
		        	e.setKey(Integer.parseInt(s[i][0]));
		        	e.setSport(s[i][1]);
		        	e.setLocation(s[i][2]);
		        	e.setDate(s[i][3]);
		        	e.setTime(s[i][4]);
		        	e.setCreating_user(s[i][5]);
		        	e.setTitle(s[i][9]);
		        	e.setSummary(s[i][6]);
		        	e.setCurrentNumberAttending(Integer.parseInt(s[i][10]));
		        	e.setMaxPlayers(Integer.parseInt(s[i][11]));
		        	e.setCompetitivness(Integer.parseInt(s[i][7]));
		        	//s[i][8] all attending users
		        	//s[i][12] owning user
		        	e.setAttending_ind(1);
		        	View page = Global.fillEventPage(e, inflater, container);
		        	ll.setOnClickListener(new Global.eventClickListener(e.getTitle(), page, e));
		        	tv.setText("Title : " + e.getTitle());
		        	tv2.setText("Date : " + e.getDate());
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
    
    
    private static String timeFix(int c) {
    	if (c >= 10) {
    		return String.valueOf(c);
    	} else {
    		return "0" + String.valueOf(c);
    	}
    }
    
    public String[][] split(String result) {
    	//int pipe = result.indexOf('|');
    	//int tilde = result.indexOf('~');
    	String[] parts = result.split("\\|");
    	String[][] allParts = new String[parts.length-1][];
    	for (int i = 1; i < parts.length; i++) {
    		allParts[i-1] = parts[i].split("~");
    	}
    	
    	String[] tempString;
    	String tempString2;
    	for (int i = 0; i < allParts.length; i++) {
    		for (int j = 0; j < allParts[i].length; j++) {
    			tempString = allParts[i][j].split(" ");
    			tempString2 = "";
    			for (int k = 0; k < tempString.length; k++){
    				if(k != 0){
    					tempString2 = tempString2 + " ";
    				}
    				tempString2 = tempString2 + tempString[k];
    			}
    			allParts[i][j] = tempString2;
    		}
    	}
    	
    	
    	
    	return allParts;
    	
    	
    }
    
    
    public int checkAll(int overall_counter, int c, String total_message)
	{
	        int checker = c;
	        while(total_message.indexOf(checker) == ' ')
	        {
	                checker++;
	        }
	        if(total_message.indexOf(checker) == '~')
	        {
	                titles.add(total_message.substring(overall_counter, c));
	                return checker;
	        }
	        if(total_message.indexOf(checker) == '|')
	        {
	                dates.add(total_message.substring(overall_counter, c));
	                return checker;
	        }
	        return checker;
	}
    
    
}
