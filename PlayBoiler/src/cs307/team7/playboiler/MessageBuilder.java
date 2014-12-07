package cs307.team7.playboiler;

import cs307.team7.playboiler.Global.RequestClickListener;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class MessageBuilder {
	
	private static final int REQUEST = 0;
	private static final int ACCEPTED = 1;
	private static final int INVITE = 2;
	private int id;
	private String alias;
	private String inviter;
	private Event e;
	private int u_id;
	
	public MessageBuilder(int id, String alias, int u_id) {
		this.id = id;
		this.alias = alias;
		this.u_id = u_id;
	}
	
	public MessageBuilder(int id, String inviter, Event e) {
		this.id = id;
		this.inviter = inviter;
		this.e = e;
	}
	
	public View generateMessage(LayoutInflater inflater, ViewGroup container) {
		View view = null;
		TextView tv;
		Button yes,no,ok;
		switch(id) {
		case REQUEST: 
			view = inflater.inflate(R.layout.friend_request, container, false);
			tv = (TextView) view.findViewById(R.id.requestString);
			tv.setText(alias + " sent \nyou a friend request");
	        yes = (Button) view.findViewById(R.id.acceptRequest);
	        no = (Button) view.findViewById(R.id.declineRequest);
	        yes.setOnClickListener(new RequestClickListener(Global.ACCEPT, view, alias, u_id));
	        no.setOnClickListener(new RequestClickListener(Global.DECLINE, view, alias, u_id));
	        break;
		case ACCEPTED:
			view = inflater.inflate(R.layout.accepted_request, container, false);
			tv = (TextView) view.findViewById(R.id.acceptedString);
			tv.setText(alias + " has accepted\n your friend request");
			ok = (Button) view.findViewById(R.id.removeMessage);
			ok.setOnClickListener(new RequestClickListener(Global.OK, view, alias, u_id));
			break;
		case INVITE:
			view = inflater.inflate(R.layout.accepted_request, container, false);
			tv = (TextView) view.findViewById(R.id.acceptedString);
			tv.setText(inviter + " has invited you\n to the event, " + e.getTitle());
			ok = (Button) view.findViewById(R.id.removeMessage);
			ok.setOnClickListener(new RequestClickListener(Global.OK, view, e.getTitle(), u_id));
			View page = Global.fillEventPage(e, inflater, container); 
			view.setOnClickListener(new Global.eventClickListener(e.getTitle(), page, e));
			break;
			
		}
		
		return view;
	}

}
