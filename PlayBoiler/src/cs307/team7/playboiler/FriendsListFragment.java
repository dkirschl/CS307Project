package cs307.team7.playboiler;

import java.util.List;

import android.app.AlertDialog;
import android.app.Dialog;
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

public class FriendsListFragment extends Fragment {
	
	public static FriendsListFragment newInstance(int sectionNumber) {
        FriendsListFragment fragment = new FriendsListFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public FriendsListFragment() {
    	
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	final View rootView = inflater.inflate(R.layout.friends_list, container, false);
    	List<Integer> friends = Global.userDatabase.getFriends(Global.current_user.getKey());
    	StringBuilder message = new StringBuilder();
    			
    	
    	LinearLayout contain = (LinearLayout) rootView.findViewById(R.id.friendsListContainer);
    	for (int i = 0; i < friends.size(); i++) {
    		View entry = inflater.inflate(R.layout.friend_view, container, false);
    		TextView tv = (TextView) entry.findViewById(R.id.friend_name);
    		tv.setText(friends.get(i));
    		
    		contain.addView(entry);
    	}
    	
    	
    	return rootView;
    }
    
    public class eventClickListener implements OnClickListener {
    	private String alias;
    	private View page;
    	
    	public eventClickListener(String alias, View page) {
    		super();
    		this.alias = alias;
    		this.page = page;
    	}

		@Override
		public void onClick(View v) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			builder.setView(page);
			final TextView et = new TextView(getActivity());
			et.setText(alias);
			et.setGravity(Gravity.CENTER);
			et.setTextSize(35);
			builder.setCustomTitle(et);
			builder.setNegativeButton("Remove Friend", new Dialog.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					//remove friend from friends list
					((ViewGroup)page.getParent()).removeView(page);
					
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
					((ViewGroup)page.getParent()).removeView(page);
					
				}
			});
			builder.show();
		}
    	
    	
    }
}
