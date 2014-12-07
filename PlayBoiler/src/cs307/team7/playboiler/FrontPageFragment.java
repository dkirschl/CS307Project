package cs307.team7.playboiler;

import java.util.concurrent.ExecutionException;

import cs307.team7.playboiler.Global.RequestClickListener;
import android.app.Activity;
import android.app.Fragment;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FrontPageFragment extends Fragment {
    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
	
	LinearLayout messagesContainer;
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
        
        
        messagesContainer = (LinearLayout) rootView.findViewById(R.id.messagesContainer);
        final getMessages gm = new getMessages(inflater, container, getActivity(), messagesContainer);
        Button refresh = (Button) rootView.findViewById(R.id.refresh);
        refresh.setOnClickListener(new msgRefreshListener(inflater, container));
        
        //Get messages
        
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
    
    public class getMessages extends AsyncTask<String, Void, String> {
    	
    	LayoutInflater inflater;
    	ViewGroup container;
    	Activity a;
    	LinearLayout mContainer;
    	
    	public getMessages(LayoutInflater inflater, ViewGroup container, Activity a, LinearLayout mContainer) {
    		this.inflater = inflater;
    		this.container = container;
    		this.a = a;
    		this.mContainer = mContainer;
    	}

		@Override
		protected String doInBackground(String... arg0) {
			Log.d("ASYNC TASK", "RUnning");
			
			
			return null;
		}
		
		@Override
		protected void onPostExecute(String thing) {
			a.runOnUiThread(new Runnable() {

				@Override
				public void run() {
					if (Global.current_user.getPreferences() == 1) {
						
						StringBuilder sb = new StringBuilder();
				        sb.append("/gtal/");
				        sb.append(Global.current_user.getKey());
				        Global.addSpaces(sb, 4-(String.valueOf(Global.current_user.getKey()).length()));
						sb.append("/");
						sb.append(Global.current_user.getPassword());
						Global.addSpaces(sb, 20 - Global.current_user.getPassword().length());
						sb.append("/");
						sb.append("\r\n");
						Log.d("Message", sb.toString());
						NetworkHandler nh = new NetworkHandler();
						String result = null;
						try {
							result = nh.execute(sb.toString()).get();
						} catch (InterruptedException e1) {
							e1.printStackTrace();
						} catch (ExecutionException e1) {
							e1.printStackTrace();
						}
						Log.d("Get Messages", result);
						
						/*
				        MessageBuilder mb = new MessageBuilder(1, "cbrentz");
				        View m1 = mb.generateMessage(inflater, container);
				        mContainer.addView(m1);
				        MessageBuilder mb2 = new MessageBuilder(2, "cbrentz");
				        View m2 = mb2.generateMessage(inflater, container);
				        mContainer.addView(m2);
				        Event e = new Event();
				        e.setTitle("Event Name");
				        MessageBuilder mb3 = new MessageBuilder(3, "cbrentz", e);
				        View m3 = mb3.generateMessage(inflater, container);
				        mContainer.addView(m3);
				        */
			        }
				}
				
			});
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
    
    private class msgRefreshListener implements OnClickListener {

    	LayoutInflater inflater;
    	ViewGroup container;
    	MessageBuilder mb;
    	View v;
    	
    	public msgRefreshListener(LayoutInflater inflater, ViewGroup container) {
    		this.inflater = inflater;
    		this.container = container;
    	}
		@Override
		public void onClick(View v) {
			StringBuilder sb = new StringBuilder();
	        sb.append("/gtal/");
	        sb.append(Global.current_user.getKey());
	        Global.addSpaces(sb, 4-(String.valueOf(Global.current_user.getKey()).length()));
			sb.append("/");
			sb.append(Global.current_user.getPassword());
			Global.addSpaces(sb, 20 - Global.current_user.getPassword().length());
			sb.append("/");
			sb.append("\r\n");
			Log.d("Message", sb.toString());
			NetworkHandler nh = new NetworkHandler();
			String result = null;
			try {
				result = nh.execute(sb.toString()).get();
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			} catch (ExecutionException e1) {
				e1.printStackTrace();
			}
			Log.d("Get Messages", result);
			if (!result.equals("||")) {
				String[][] msgs = split(result);
				for (int i = 0; i < msgs.length; i++) {
					int type = Integer.parseInt(msgs[i][0]);
					Log.d("Type", ""+type);
					if (type == 0) {
						//pending friend request
						mb = new MessageBuilder(0, msgs[i][2], Integer.parseInt(msgs[i][1]));
			        	v = mb.generateMessage(inflater, container);
			        	messagesContainer.addView(v);
					
					} else if (type == 1) {
						//a request was accepted
						Global.userDatabase.addFriend(Global.current_user.getKey(), Integer.parseInt(msgs[i][1]));
						mb = new MessageBuilder(1, msgs[i][2], Integer.parseInt(msgs[i][1]));
						v = mb.generateMessage(inflater, container);
						messagesContainer.addView(v);
					} else if (type == 2) {
						//game invite
					}
					for (int j = 0; j < msgs[i].length; j++) {
						Log.d("PART", "" + i + "," + j + ":" +msgs[i][j] +":");
					
					}
				}
			}
		}
    	
    }
    
    
}
