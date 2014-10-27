package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
    	final View rootView = inflater.inflate(R.layout.created_events, container, false);
    	
    	LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.createdEventsContainer);
    	
    	
    	createdEventsList = Global.userDatabase.getCreatedEvents();
    	
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
    
}
