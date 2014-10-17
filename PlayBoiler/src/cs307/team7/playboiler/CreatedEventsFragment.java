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
    ArrayList<TextView> eventTitles, eventDates;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
    	final View rootView = inflater.inflate(R.layout.created_events, container, false);
    	eventTitles = new ArrayList<TextView>();
    	eventDates = new ArrayList<TextView>();
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
    	
    	TextView e11 = (TextView) rootView.findViewById(R.id.e1Date);
    	eventDates.add(e11);
    	TextView e21 = (TextView) rootView.findViewById(R.id.e2Date);
    	eventDates.add(e21);
    	TextView e31 = (TextView) rootView.findViewById(R.id.e3Date);
    	eventDates.add(e31);
    	TextView e41 = (TextView) rootView.findViewById(R.id.e4Date);
    	eventDates.add(e41);
    	TextView e51 = (TextView) rootView.findViewById(R.id.e5Date);
    	eventDates.add(e51);
    	
    	createdEventsList = Global.userDatabase.getCreatedEvents();
    	for (int i = 0; i < createdEventsList.size(); i++) {
    		eventTitles.get(i).setText(createdEventsList.get(i).getTitle());
    	}
    	for (int i = 0; i < createdEventsList.size(); i++) {
    		eventDates.get(i).setText(createdEventsList.get(i).getDate());
    	}
    	
    	
    	
    	return rootView;
    	
    }
}
