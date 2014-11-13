package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebView.FindListener;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

public class CreatedEventsFragment extends Fragment {
	
	public static CreatedEventsFragment newInstance(int sectionNumber) {
        CreatedEventsFragment fragment = new CreatedEventsFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public CreatedEventsFragment() {
    }
    
    List<Event> createdEventsList;
    ArrayList<TextView> eventTitles, eventDates, eventSports;
    ArrayList<Integer> eventIds, attendingSizes;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
    	final View rootView = inflater.inflate(R.layout.created_events, container, false);
    	
    	LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.createdEventsContainer);
    	eventIds = new ArrayList<Integer>();
		attendingSizes = new ArrayList<Integer>();
    	
    	createdEventsList = Global.userDatabase.getCreatedEvents();
    	
    	
    	
    	//query server
    	StringBuilder sb = new StringBuilder();
    	int keyLen = 4;
    	sb.append("/crup/");
    	sb.append(Global.current_user.getKey());
    	String keyString = ""+Global.current_user.getKey();
    	Global.addSpaces(sb, keyLen-keyString.length());
    	sb.append("/");
    	sb.append("\r\n");
    	
    	Log.d("Message", sb.toString());
		String result = null;
		NetworkHandler nh = new NetworkHandler();
		try {
			result = nh.execute(sb.toString()).get();
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		} catch (ExecutionException e1) {
			e1.printStackTrace();
		}
		Log.d("Result", result);
		//result = result.substring(7);
		String[][] s = split(result);
		
		for (int i = 0; i < s.length; i++) {
			//eventIds.add(Integer.parseInt(s[i][0]));
			//attendingSizes.add(Integer.parseInt(s[i][1]));
			createdEventsList.get(i).setCurrentNumberAttending(Integer.parseInt(s[i][1]));
			
			for (int j = 0; j < s[i].length; j++) {
				Log.d("PART", "" + i + "," + j + ":" +s[i][j] +":");
			}
		}
		
		for (int i = 0; i < createdEventsList.size(); i++) {
			View ll = inflater.inflate(R.layout.event_view, container, false);
        	TextView tv = (TextView) ll.findViewById(R.id.searchEventTitle);
        	TextView tv2 = (TextView) ll.findViewById(R.id.searchEventDate);
        	View page = Global.fillEventPage(createdEventsList.get(i), inflater, container);
        	ll.setOnClickListener(new eventClickListener(createdEventsList.get(i).getTitle(), page));
        	tv.setText("Title : " + createdEventsList.get(i).getTitle());
        	tv2.setText("Date : " + createdEventsList.get(i).getDate());
        	contain.addView(ll);
		}
		
		
    	
    	return rootView;
    	
    }
    
    public class eventClickListener implements OnClickListener {
    	private String t;
    	private View page;
    	
    	public eventClickListener(String title, View page) {
    		super();
    		t = title;
    		this.page = page;
    	}

		@Override
		public void onClick(View v) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setView(page);
			final TextView et = new TextView(getActivity());
			et.setText(t);
			et.setGravity(Gravity.CENTER);
			et.setTextSize(35);
			builder.setCustomTitle(et);
			builder.setOnCancelListener(new OnCancelListener() {
				
				@Override
				public void onCancel(DialogInterface arg0) {
					((ViewGroup)page.getParent()).removeView(page);
					
				}
			});
			builder.show();
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
    
}
