package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

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
    ArrayList<TextView> eventTitles;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
    	final View rootView = inflater.inflate(R.layout.created_events, container, false);
    	eventTitles = new ArrayList<TextView>();
    	TextView e1 = (TextView) rootView.findViewById(R.id.e1Title);
    	eventTitles.add(e1);
    	TextView e2 = (TextView) rootView.findViewById(R.id.e2Title);
    	eventTitles.add(e2);
    	TextView e3 = (TextView) rootView.findViewById(R.id.e3Title);
    	eventTitles.add(e3);
    	TextView e4 = (TextView) rootView.findViewById(R.id.e4Title);
    	eventTitles.add(e4);
    	TextView e5 = (TextView) rootView.findViewById(R.id.e5Title);
    	eventTitles.add(e5);
    	
    	createdEventsList = Global.userDatabase.getCreatedEvents();
    	for (int i = 0; i < createdEventsList.size(); i++) {
    		eventTitles.get(i).setText(createdEventsList.get(i).getTitle());
    	}
    	
    	
    	
    	return rootView;
    	
    }
}
