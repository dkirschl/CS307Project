package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

import cs307.team7.playboiler.SearchFragment.eventClickListener;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class JoinedGamesFragment extends Fragment {
	
	public static JoinedGamesFragment newInstance(int sectionNumber) {
		JoinedGamesFragment fragment = new JoinedGamesFragment();
		Bundle args = new Bundle();
		fragment.setArguments(args);
		return fragment;
	}
	
	public JoinedGamesFragment() {
		
	}
	
	@Override
    public View onCreateView(final LayoutInflater inflater, final ViewGroup container,
            Bundle savedInstanceState) {
		View rootView = inflater.inflate(R.layout.joined_events, container, false);
		Log.d("JOINED", "Ping");
		final LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.joinedContainer);
		List<Event> joinedEvents = Global.userDatabase.getJoinedGames();
		Log.d("Joined Games", ""+joinedEvents.size());
		for (int i = 0; i < joinedEvents.size(); i++) {
			View ll = inflater.inflate(R.layout.event_view, container, false);
			TextView tv = (TextView) ll.findViewById(R.id.searchEventTitle);
        	TextView tv2 = (TextView) ll.findViewById(R.id.searchEventDate);
        	
        	View page = Global.fillEventPage(joinedEvents.get(i), inflater, container);
        	ll.setOnClickListener(new eventClickListener(joinedEvents.get(i).getTitle(), page, joinedEvents.get(i)));
        	tv.setText("Title : " + joinedEvents.get(i).getTitle());
        	tv2.setText("Date : " + joinedEvents.get(i).getDate());
        	//aaa
        	contain.addView(ll);
		}
				
		
		
		
		return rootView;
	}
	
	public class eventClickListener implements OnClickListener {
    	private String t;
    	private View page;
    	private Event e;
    	
    	public eventClickListener(String title, View page, Event event) {
    		super();
    		t = title;
    		this.page = page;
    		e = event;
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
			builder.setPositiveButton("Leave Event", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					//Code to join event
					int keyLen = 4;
					int pasLen = 20;
					StringBuilder str = new StringBuilder();
					str.append("/unjn/");
					str.append(Global.current_user.getKey());
					String kyString = "" + Global.current_user.getKey();
					Global.addSpaces(str, keyLen - kyString.length());
					str.append("/");
					str.append(Global.current_user.getPassword());
					Global.addSpaces(str, pasLen - Global.current_user.getPassword().length());
					str.append("/");
					str.append(e.getKey());
					String keyString = "" + e.getKey();
					Global.addSpaces(str, keyLen - keyString.length());
					str.append("/");
					str.append("\r\n");
					//Add user to event on external server
					Log.d("Message", str.toString());
					String result = null;
					NetworkHandler nh = new NetworkHandler();
					try {
						result = nh.execute(str.toString()).get();
					} catch (InterruptedException e1) {
						e1.printStackTrace();
					} catch (ExecutionException e1) {
						e1.printStackTrace();
					}
					Log.d("Join Result", result);
					Toast.makeText(getActivity(), "Event Left", Toast.LENGTH_LONG).show();
					
					
					
					//Add event to local database of joined events
					Global.userDatabase.deleteEvent(e);
					//Global.userDatabase.joinEvent(e);
					
				}
			});
			builder.setOnCancelListener(new OnCancelListener() {
				
				@Override
				public void onCancel(DialogInterface arg0) {
					((ViewGroup)page.getParent()).removeView(page);
					
				}
			});
			builder.show();
		}
    	
    	
    }

}
