package cs307.team7.playboiler;

public class ReoccuringGame extends Event 
{
	private String start_date;
	private String end_date;
	private String sport;
	private String location;
	private String time;
	private String title;
	private String summary;
	private String creating_user;
	private int max_attending;
	
	public ReoccuringGame(String sport, String location, String time, String title, String sum, String c_user, int max_a,String s_date, String e_date)
	{
		start_date = s_date;
		end_date = e_date;
		this.sport = sport;
		this.location = location;
		this.time = time;
		this.title = title;
		this.summary = sum;
		creating_user = c_user;
		max_attending = max_a;
	}
}
