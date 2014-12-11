package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;
import java.util.concurrent.ExecutionException;

import android.app.AlertDialog;
import android.app.Dialog;
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
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class FriendsListFragment extends Fragment {
	
	public static FriendsListFragment newInstance(int sectionNumber) {
        FriendsListFragment fragment = new FriendsListFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public FriendsListFragment() {
    	
    }
    
    List<Integer> friends;
    ArrayList<String> friend_aliases;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	final View rootView = inflater.inflate(R.layout.friends_list, container, false);
    	friends = Global.userDatabase.getFriends(Global.current_user.getKey());
    	StringBuilder message = new StringBuilder();
    	if (friends != null) {
    	message.append("/gail/");
    	for (int i = 0; i < friends.size(); i++) {
    		message.append(friends.get(i));
    		if (i != friends.size()-1) {
    			message.append(",");
    		}
    	}
    	Global.addSpaces(message, 1006-message.length());
    	message.append("/\r\n");
    	Log.d("Message", message.toString());
		String result = null;
		NetworkHandler nh = new NetworkHandler();
		try {
			result = nh.execute(message.toString()).get();
		} catch (InterruptedException e1) {
			e1.printStackTrace();
		} catch (ExecutionException e1) {
			e1.printStackTrace();
		}
		StringTokenizer st = new StringTokenizer(result, "|");
		friend_aliases = new ArrayList<String>();
		int x = 0;
		while(st.hasMoreElements()) {
			friend_aliases.add(x, st.nextElement().toString());
			Log.d("Friends", friend_aliases.get(x));
			x++;
		}
    	
    	//Get all aliases from server
    	}		
    	if (friends != null) {
    		LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.friendsListContainer);
    		for (int i = 0; i < friend_aliases.size(); i++) {
    			View entry = inflater.inflate(R.layout.friend_view, container, false);
    			TextView tv = (TextView) entry.findViewById(R.id.friend_name);
    			tv.setText(""+friend_aliases.get(i));
    			entry.setOnClickListener(new friendClickListener(tv.getText().toString(), entry));
    		
    			contain.addView(entry);
    		}
    	}
    	
    	Button add = (Button) rootView.findViewById(R.id.addFriend);
    	add.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				final Dialog d = new Dialog(v.getContext());
				d.setContentView(R.layout.add_friend);
				d.setTitle("Add a Friend");
				Button search = (Button) d.findViewById(R.id.searchFriend);
				Button cancel = (Button) d.findViewById(R.id.cancelFriend);
				cancel.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						d.cancel();
					}
					
				});
				final EditText name = (EditText) d.findViewById(R.id.addFriendInput);
				search.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						//Ping server
						int keyLen = 4;
						int pasLen = 20;
						int userLen = 20;
						String user = name.getText().toString();
						StringBuilder sb = new StringBuilder();
						//your id : your pass : alias of friend
						sb.append("/addf/");
						sb.append(Global.current_user.getKey());
						String kyString = "" + Global.current_user.getKey();
						Global.addSpaces(sb, keyLen - kyString.length());
						sb.append("/");
						sb.append(Global.current_user.getPassword());
						Global.addSpaces(sb, pasLen - Global.current_user.getPassword().length());
						sb.append("/");
						sb.append(user);
						Global.addSpaces(sb, userLen - user.length());
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
						Log.d("Friend Requested", result);
						d.cancel();
						//Search is either successful or failed
					}
				});
				d.show();
			}
		});
    	
    	return rootView;
    }
    
    public class friendClickListener implements OnClickListener {
    	private String alias;
    	private View page;
    	
    	public friendClickListener(String alias, View page) {
    		super();
    		this.alias = alias;
    		this.page = page;
    	}

		@Override
		public void onClick(View v) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			//builder.setView(page);
			final TextView et = new TextView(getActivity());
			
			et.setText(alias);
			et.setGravity(Gravity.CENTER);
			et.setTextSize(35);
			builder.setCustomTitle(et);
			builder.setNegativeButton("Remove Friend", new Dialog.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					//remove friend from friends list
					int keyLen = 4;
					int pasLen = 20;
					int userLen = 20;
					StringBuilder sb = new StringBuilder();
					sb.append("/delf/");
					//your id : your pass : alias
					sb.append(Global.current_user.getKey());
					String kyString = "" + Global.current_user.getKey();
					Global.addSpaces(sb, keyLen - kyString.length());
					sb.append("/");
					sb.append(Global.current_user.getPassword());
					Global.addSpaces(sb, pasLen - Global.current_user.getPassword().length());
					sb.append("/");
					int z = friend_aliases.indexOf(alias);
					//sb.append(friends.get(z));
					//kyString = "" + friends.get(z);
					sb.append(alias);
					//Global.addSpaces(sb, keyLen - kyString.length());
					Global.addSpaces(sb, userLen-alias.length());
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
					if (!result.equals("INVALID")) {
						Global.userDatabase.removeFriend(Global.current_user.getKey(), friends.get(z));
						Toast.makeText(page.getContext(), "Friend Removed", Toast.LENGTH_SHORT).show();
						Log.d("Friend Removed", result);
						((ViewGroup)page.getParent()).removeView(page);
					} else {
						Toast.makeText(page.getContext(), "An Error Occurred", Toast.LENGTH_SHORT).show();
					}
					
				}
			});
			builder.setPositiveButton("Ok", new Dialog.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					
				}
			});
			builder.setOnCancelListener(new OnCancelListener() {
				
				@Override
				public void onCancel(DialogInterface arg0) {
					Global.userDatabase.removeFriend(Global.current_user.getKey(), Integer.parseInt(alias));
					((ViewGroup)page.getParent()).removeView(page);
					
				}
			});
			builder.show();
		}
    	
    	
    }
}
