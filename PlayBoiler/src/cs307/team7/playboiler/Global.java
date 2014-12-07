package cs307.team7.playboiler;

import java.util.List;
import java.util.concurrent.ExecutionException;

import cs307.team7.playboiler.FriendsListFragment.friendClickListener;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.DatePicker;
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
	public static final int OK = 2;
	public static boolean loggedIn = false;
	
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
		String alias;
		int keyLen = 4;
		int pasLen = 20;
		int userLen = 20;
		String result;
		NetworkHandler nh;
		int id;
		StringBuilder sb;
		String kyString;
		
		public RequestClickListener(int action, View message, String alias, int id) {
			this.action = action;
			this.message = message;
			this.alias = alias;
			this.id = id;
		}
		
		@Override
		public void onClick(View v) {
			switch (action) {
			case DECLINE:
				Toast.makeText(v.getContext(), "Friend Request Declined.", Toast.LENGTH_SHORT).show();
				sb = new StringBuilder();
				sb.append("/delf/");
				//your id : your pass : alias
				sb.append(Global.current_user.getKey());
				kyString = "" + Global.current_user.getKey();
				Global.addSpaces(sb, keyLen - kyString.length());
				sb.append("/");
				sb.append(Global.current_user.getPassword());
				Global.addSpaces(sb, pasLen - Global.current_user.getPassword().length());
				sb.append("/");
				sb.append(alias);
				Global.addSpaces(sb, userLen - alias.length());
				sb.append("/");
				sb.append("\r\n");
				
				Log.d("Message", sb.toString());
				result = null;
				nh = new NetworkHandler();
				try {
					result = nh.execute(sb.toString()).get();
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} catch (ExecutionException e1) {
					e1.printStackTrace();
				}
				Log.d("Friend Denied", result);
				break;
			case ACCEPT:
				Toast.makeText(v.getContext(), "Friend Request Accepted!", Toast.LENGTH_SHORT).show();
				
				sb = new StringBuilder();
				//your id : your pass : alias of friend
				sb.append("/addf/");
				sb.append(Global.current_user.getKey());
				kyString = "" + Global.current_user.getKey();
				Global.addSpaces(sb, keyLen - kyString.length());
				sb.append("/");
				sb.append(Global.current_user.getPassword());
				Global.addSpaces(sb, pasLen - Global.current_user.getPassword().length());
				sb.append("/");
				sb.append(alias);
				Global.addSpaces(sb, userLen - alias.length());
				sb.append("/");
				sb.append("\r\n");
				
				Log.d("Message", sb.toString());
				result = null;
				nh = new NetworkHandler();
				try {
					result = nh.execute(sb.toString()).get();
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} catch (ExecutionException e1) {
					e1.printStackTrace();
				}
				Log.d("Friend Requested", result);
				Global.userDatabase.addFriend(Global.current_user.getKey(), id);
				break;
			case OK:
				Toast.makeText(v.getContext(), "Message Removed", Toast.LENGTH_SHORT).show();
				sb = new StringBuilder();
				sb.append("/dlfq/");
				//your id : your pass : id
				sb.append(Global.current_user.getKey());
				String kyString2 = "" + Global.current_user.getKey();
				Global.addSpaces(sb, keyLen - kyString2.length());
				sb.append("/");
				sb.append(Global.current_user.getPassword());
				Global.addSpaces(sb, pasLen - Global.current_user.getPassword().length());
				sb.append("/");
				String idString = "" + id;
				sb.append(id);
				Global.addSpaces(sb, keyLen - idString.length());
				sb.append("/");
				sb.append("\r\n");
				Log.d("Message", sb.toString());
				result = null;
				nh = new NetworkHandler();
				try {
					result = nh.execute(sb.toString()).get();
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				} catch (ExecutionException e1) {
					e1.printStackTrace();
				}
				Log.d("Message Removed", result);
				
				break;
			}
			((ViewGroup)message.getParent()).removeView(message);
		}
		
	}
	
	public static class eventClickListener implements OnClickListener {

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
			AlertDialog.Builder builder = new AlertDialog.Builder(v.getContext());
			builder.setView(page);
			final TextView et = new TextView(v.getContext());
			et.setText(t);
			et.setGravity(Gravity.CENTER);
			et.setTextSize(35);
			builder.setCustomTitle(et);
			builder.setPositiveButton("Join Event", new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					//Code to join event
					int keyLen = 4;
					int pasLen = 20;
					StringBuilder str = new StringBuilder();
					str.append("/join/");
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
					if (result.equals("INVALID")) {
						Toast.makeText(et.getContext(), "Event Not Joined : Full", Toast.LENGTH_LONG).show();
					} else {
						Toast.makeText(et.getContext(), "Event Joined", Toast.LENGTH_LONG).show();
						//Add event to local database of joined events
						e.setSpecificUser(Global.current_user.getAlias());
						Global.userDatabase.joinEvent(e);
					}
					
					
					
					
					
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
	
	public static class dateClickListener implements OnClickListener {
		TextView tvDate;
		public dateClickListener(TextView tvDate) {
			this.tvDate = tvDate;
		}
		@Override
		public void onClick(View v) {
			final Dialog d = new Dialog(v.getContext());
			d.setContentView(R.layout.pick_date);
			d.setTitle("Pick a Date");
			
			final DatePicker dp = (DatePicker) d.findViewById(R.id.datePicker);
			Button done = (Button) d.findViewById(R.id.dateDone);
			done.setOnClickListener(new OnClickListener() {
				
				@Override
				public void onClick(View v1) {
					int month = dp.getMonth() + 1;
					String strMon;
					if (month < 10) {
						strMon = "0"+month;
					} else {
						strMon = ""+month;
					}
					int day = dp.getDayOfMonth();
					String strDay;
					if (day < 10) {
						strDay = "0"+day;
					} else {
						strDay = ""+day;
					}
					StringBuilder date = new StringBuilder().append((strMon)).append("-").append(strDay).append("-").append(dp.getYear());
					//StringBuilder date = new StringBuilder().append((dp.getMonth() + 1)).append("-").append(dp.getDayOfMonth()).append("-").append(dp.getYear());
					tvDate.setText(date);
					d.cancel();
				}
			});
			d.show();
		}
	}
}


