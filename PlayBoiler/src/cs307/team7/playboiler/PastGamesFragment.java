package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;

import cs307.team7.playboiler.CreatedEventsFragment.eventClickListener;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.TextView;

public class PastGamesFragment extends Fragment{
	
	public static PastGamesFragment newInstance(int sectionNumber) {
        PastGamesFragment fragment = new PastGamesFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public PastGamesFragment() {
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
    	final View rootView = inflater.inflate(R.layout.past_events, container, false);
		Global.userDatabase.setPastEvents();
    	
    	LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.pastEventsContainer);
    	
    	List<Event> pastEventsList = Global.userDatabase.getPreviousEvents();
    	for (int i = 0; i < pastEventsList.size(); i++) {
			View ll = inflater.inflate(R.layout.event_view, container, false);
        	TextView tv = (TextView) ll.findViewById(R.id.searchEventTitle);
        	TextView tv2 = (TextView) ll.findViewById(R.id.searchEventDate);
        	View page = Global.fillEventPage(pastEventsList.get(i), inflater, container);
        	ll.setOnClickListener(new eventClickListener(pastEventsList.get(i).getTitle(), page));
        	tv.setText("Title : " + pastEventsList.get(i).getTitle());
        	tv2.setText("Date : " + pastEventsList.get(i).getDate());
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
}
