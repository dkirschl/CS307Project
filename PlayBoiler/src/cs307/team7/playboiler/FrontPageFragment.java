package cs307.team7.playboiler;

import cs307.team7.playboiler.Global.RequestClickListener;
import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FrontPageFragment extends Fragment {
    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static FrontPageFragment newInstance(int sectionNumber) {
        FrontPageFragment fragment = new FrontPageFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public FrontPageFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	
        View rootView = inflater.inflate(R.layout.front_page, container, false);
        LinearLayout messagesContainer = (LinearLayout) rootView.findViewById(R.id.messagesContainer);
        //Get messages
        
        MessageBuilder mb = new MessageBuilder(1, "cbrentz");
        View m1 = mb.generateMessage(inflater, container);
        messagesContainer.addView(m1);
        MessageBuilder mb2 = new MessageBuilder(2, "cbrentz");
        View m2 = mb2.generateMessage(inflater, container);
        messagesContainer.addView(m2);
        Event e = new Event();
        e.setTime("Event Name");
        MessageBuilder mb3 = new MessageBuilder(3, "cbrentz", e);
        View m3 = mb3.generateMessage(inflater, container);
        messagesContainer.addView(m3);
        
        /*
        //PLACEHOLDER MESSAGE FOR TESTING
        View testMessage = inflater.inflate(R.layout.friend_request, container, false);
        Button yes = (Button) testMessage.findViewById(R.id.acceptRequest);
        Button no = (Button) testMessage.findViewById(R.id.declineRequest);
        yes.setOnClickListener(new RequestClickListener(Global.ACCEPT, testMessage));
        no.setOnClickListener(new RequestClickListener(Global.DECLINE, testMessage));
        messagesContainer.addView(testMessage);
        //////////////////////////////////////
        */
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
    
    
}
