package cs307.team7.playboiler;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class Global {
	public static MySqlLiteHelper userDatabase;
	public static User current_user;
	public static int temp_key;
	public static int USER_CREATED_EVENT_CODE = 2;
	public static String USER_CREATED_YES = "yes";
	public static String USER_CREATED_NO = "no";
	
	public static View fillEventPage(Event e, LayoutInflater inflater, ViewGroup container) {
		View v = inflater.inflate(R.layout.event_page_no_join, container, false);
		//TextView title = (TextView) v.findViewById(R.id.epnjTitle);
		TextView date = (TextView) v.findViewById(R.id.epnjDate);
		TextView time = (TextView) v.findViewById(R.id.epnjTime);
		TextView loc = (TextView) v.findViewById(R.id.epnjLoc);
		TextView sport = (TextView) v.findViewById(R.id.epnjSport);
		//title.setText(e.getTitle());
		date.setText(e.getDate());
		time.setText(e.getTime());
		loc.setText(e.getLocation());
		sport.setText(e.getSport());
		
    	return v;
    }
	
	public static StringBuilder addSpaces(StringBuilder sb, int numSpaces) {
		for(int i=0; i < numSpaces; i++) {
			sb.append(" ");
		}
    	return sb;
    	
    }
}
