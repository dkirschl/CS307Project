package cs307.team7.playboiler;

import java.util.List;

import cs307.team7.playboiler.FriendsListFragment.friendClickListener;
import android.app.Dialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class Global {
	public static MySqlLiteHelper userDatabase;
	public static User current_user;
	public static int temp_key;
	public static int USER_CREATED_EVENT_CODE = 2;
	public static String USER_CREATED_YES = "yes";
	public static String USER_CREATED_NO = "no";
	public static final int DECLINE = 0;
	public static final int ACCEPT = 1;
	
	public static View fillEventPage(Event e, LayoutInflater inflater, ViewGroup container) {
		View v = inflater.inflate(R.layout.event_page_no_join, container, false);
		Button invite = (Button) v.findViewById(R.id.inviteFriend);
		invite.setOnClickListener(new inviteFriendListener(inflater, container));
		//TextView title = (TextView) v.findViewById(R.id.epnjTitle);
		TextView date = (TextView) v.findViewById(R.id.epnjDate);
		TextView time = (TextView) v.findViewById(R.id.epnjTime);
		TextView loc = (TextView) v.findViewById(R.id.epnjLoc);
		TextView sport = (TextView) v.findViewById(R.id.epnjSport);
		TextView currAttending = (TextView) v.findViewById(R.id.currAttending9);
		TextView maxPlayers = (TextView) v.findViewById(R.id.maxAttending9);
		//title.setText(e.getTitle());
		date.setText(e.getDate());
		time.setText(e.getTime());
		loc.setText(e.getLocation());
		sport.setText(e.getSport());
		currAttending.setText(String.valueOf(e.getCurrentNumberAttending()));
		maxPlayers.setText(String.valueOf(e.getMaxPlayers()));
		
    	return v;
    }
	
	public static StringBuilder addSpaces(StringBuilder sb, int numSpaces) {
		for(int i=0; i < numSpaces; i++) {
			sb.append(" ");
		}
    	return sb;
    	
    }
	
	public static class inviteFriendListener implements OnClickListener {
		LayoutInflater inflater;
		ViewGroup container;
		
		public inviteFriendListener(LayoutInflater li, ViewGroup container) {
			inflater = li;
			this.container = container;
		}

		@Override
		public void onClick(View v) {
			Dialog d = new Dialog(v.getContext());
			List<Integer> friends = Global.userDatabase.getFriends(Global.current_user.getKey());
			d.setContentView(R.layout.friends_list);
			
			if (friends != null) {
	    		LinearLayout contain = (LinearLayout) d.findViewById(R.id.friendsListContainer);
	    		for (int i = 0; i < friends.size(); i++) {
	    			View entry = inflater.inflate(R.layout.select_friend_view, container, false);
	    			CheckBox cb = (CheckBox) entry.findViewById(R.id.inviteBox);
	    			cb.setText(""+friends.get(i));
	    		
	    			contain.addView(entry);
	    		}
	    	}
			d.show();
		}
		
	}
	
	public static class RequestClickListener implements OnClickListener {

		int action;
		View message;
		
		public RequestClickListener(int action, View message) {
			this.action = action;
			this.message = message;
		}
		
		@Override
		public void onClick(View v) {
			switch (action) {
			case DECLINE:
				Toast.makeText(v.getContext(), "Friend Request Declined.", Toast.LENGTH_SHORT).show();
				//Global.userDatabase.removeFriend(Global.current_user.getKey(),20);
				break;
			case ACCEPT:
				Toast.makeText(v.getContext(), "Friend Request Accepted!", Toast.LENGTH_SHORT).show();
				//Global.userDatabase.addFriend(Global.current_user.getKey(), 21);
				break;
			}
			((ViewGroup)message.getParent()).removeView(message);
		}
		
	}
}
