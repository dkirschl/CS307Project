package cs307.team7.playboiler;

public class Event {
	
	private int key;
	private String sport;
	private String location;
	private String date;
	private String time;
	private String title;
	private String creating_user;
	private int attending_ind;
	private String summary;
	private int maxPlayers;
	private String created;
	private int currentNumberAttending;
	private int competitivness;
	private String specificUser;
	private int attending;
	
	public int getCompetitivness() {
		return competitivness;
	}
	public void setCompetitivness(int competitivness) {
		this.competitivness = competitivness;
	}
	public int getCurrentNumberAttending() {
		return currentNumberAttending;
	}
	public void setCurrentNumberAttending(int currentNumberAttending) {
		this.currentNumberAttending = currentNumberAttending;
	}
	//comment
	public Event()
	{
		key = -1;
		sport = "";
		location = "";
		date = "";
		time = "";
		title = "";
		creating_user = "";
		attending_ind = 0;
		currentNumberAttending = 0;
		summary = "";
		created = "";
		competitivness = 1;
		specificUser = "";
		attending = 0;
	}
	public int getAttending() {
		return attending;
	}
	public void setAttending(int attending) {
		this.attending = attending;
	}
	public Event(int k, String s, String l, String d, String t1, String t2, String cu, String su, int max, int a, String c)
	{
		key = k;
		sport = s;
		location = l;
		date = d;
		time = t1;
		title = t2;
		creating_user = cu;
		summary = su;
		attending_ind = a;
		maxPlayers = max;
		created = c;
	}
	public String getSpecificUser() {
		return specificUser;
	}
	public void setSpecificUser(String specificUser) {
		this.specificUser = specificUser;
	}
	public String getCreated() {
		return created;
	}
	public void setCreated(String created) {
		this.created = created;
	}
	public int getMaxPlayers() {
		return maxPlayers;
	}
	public void setMaxPlayers(int maxPlayers) {
		this.maxPlayers = maxPlayers;
	}
	public String getSummary() {
		return summary;
	}
	public void setSummary(String summary) {
		this.summary = summary;
	}
	public int getKey() {
		return key;
	}
	public void setKey(int key) {
		this.key = key;
	}
	public int getAttending_ind() {
		return attending_ind;
	}
	public void setAttending_ind(int attending_ind) {
		this.attending_ind = attending_ind;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getSport() {
		return sport;
	}
	public void setSport(String sport) {
		this.sport = sport;
	}
	public String getLocation() {
		return location;
	}
	public void setLocation(String location) {
		this.location = location;
	}
	public String getDate() {
		return date;
	}
	public void setDate(String date) {
		this.date = date;
	}
	public String getTime() {
		return time;
	}
	public void setTime(String time) {
		this.time = time;
	}
	public String getCreating_user() {
		return creating_user;
	}
	public void setCreating_user(String creating_user) {
		this.creating_user = creating_user;
	}
	public int getAttendingInd() {
		return attending_ind;
	}
	public void setAttendingInd(int attending_ind) {
		this.attending_ind = attending_ind;
	}
	
}
