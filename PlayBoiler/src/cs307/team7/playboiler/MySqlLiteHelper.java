package cs307.team7.playboiler;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class MySqlLiteHelper extends SQLiteOpenHelper
{
	public static final int DATBASE_VERSION = 1;
	public static final String DATABASE_NAME = "PlayBoilerDB";
	
	public MySqlLiteHelper(Context context)
	{
		super(context, DATABASE_NAME, null, DATBASE_VERSION);
	}
	
	@Override
	public void onCreate(SQLiteDatabase db)
	{
		Log.d("Database", "Database being initialized in onCreate()");
		String CREATE_USER_PROFILE_TABLE = "CREATE TABLE user_profile (" + "id INTEGER PRIMARY KEY, " +
				"name TEXT, " + "alias TEXT, " + "age INTEGER, " + "gender TEXT, " + "description TEXT, " +
				"proficiencies TEXT, " + "password TEXT, " + "friends TEXT, " + "preferences INTEGER)";
		
		String CREATE_GAMES_TABLE = "CREATE TABLE past_games (" + "id, " + "sport TEXT, " + "location TEXT, " + 
				"date TEXT, " + "time TEXT, " + "title TEXT, " + "summary TEXT, " + "creating_user TEXT, " + "attending_ind INTEGER, " + "max_attending INTEGER, " + "created TEXT, "+
				"specific_user TEXT, " + "number_attending INTEGER, PRIMARY KEY (id, specific_user))";
		
		String CREATE_GAMES_TYPE_TABLE = "CREATE TABLE types_of_games (" + "sport_type TEXT PRIMARY KEY)";
		
		String CREATE_REOCURRING_GAMES_TABLE = "CREATE TABLE reoccuring_games (" + "id INTEGER PRIMARY KEY, " + "start_date TEXT, " + "end_date TEXT, " +"sport TEXT, " + "location TEXT, " + 
				"time TEXT, " + "title TEXT, " + "summary TEXT, " + "creating_user TEXT, " + "max_attending INTEGER, " + "created TEXT, "+
				"specific_user TEXT, "+ "monday TEXT, " + "tuesday TEXT, " + "wednesday TEXT, " + "thursday TEXT, " + "friday TEXT, " + "saturday TEXT, " + "sunday TEXT)";
		
		db.execSQL(CREATE_GAMES_TABLE);
		db.execSQL(CREATE_USER_PROFILE_TABLE);
		db.execSQL(CREATE_GAMES_TYPE_TABLE);
		db.execSQL(CREATE_REOCURRING_GAMES_TABLE);
	}
	
	@Override
	public void onUpgrade(SQLiteDatabase db, int olderVersion, int newVersion)
	{
		db.execSQL("DROP TABLE IF EXISTS user_profile");
		db.execSQL("DROP TABLE IF EXISTS past_games");
		db.execSQL("DROP TABLE IF EXISTS types_of_games");
		db.execSQL("DROP TABLE IF EXISTS reoccuring_games");
		
		this.onCreate(db);
	}

	public static final String USER_TABLE = "user_profile";
	public static final String USER_KEY = "id";
	public static final String USER_NAME = "name";
	public static final String USER_ALIAS = "alias";
	public static final String USER_AGE = "age";
	public static final String USER_GENDER = "gender";
	public static final String USER_DESCRIPTION = "description";
	public static final String USER_PROFICIENCIES = "proficiencies";
	public static final String USER_PASSWORD = "password";
	public static final String USER_FRIENDS = "friends";
	public static final String USER_PREFERENCES = "preferences";
	
	public static final String[] USER_COLUMNS = {USER_KEY, USER_NAME, USER_ALIAS, USER_GENDER, USER_AGE, USER_DESCRIPTION
		, USER_PROFICIENCIES, USER_PASSWORD, USER_FRIENDS, USER_PREFERENCES};
	
	public static final String GAMES_TABLE = "past_games";
	public static final String GAMES_KEY = "id";
	public static final String GAMES_SPORT = "sport";
	public static final String GAMES_LOCATION = "location";
	public static final String GAMES_DATE = "date";
	public static final String GAMES_TIME = "time";
	public static final String GAMES_TITLE = "title";
	public static final String GAMES_SUMMARY = "summary";
	public static final String GAMES_CREATING_USER = "creating_user";
	public static final String GAMES_ATTENDING_IND = "attending_ind";
	public static final String GAMES_MAX_ATTENDING = "max_attending";
	public static final String GAMES_CREATED = "created";
	public static final String GAMES_SPECIFIC_USER = "specific_user";
	public static final String GAMES_ATTENDING = "number_attending";
	
	public static final String[] GAMES_COLUMNS = {GAMES_KEY, GAMES_SPORT, GAMES_LOCATION, GAMES_DATE, GAMES_TIME,  GAMES_TITLE, GAMES_SUMMARY,
		GAMES_CREATING_USER, GAMES_ATTENDING_IND, GAMES_MAX_ATTENDING, GAMES_CREATED, GAMES_SPECIFIC_USER, GAMES_ATTENDING};
	
	public static final String GAMES_TYPE_TABLE = "types_of_games";
	public static final String GAMES_TYPE_KEY = "id";
	public static final String GAMES_TYPE = "sport_type";
	public static final String[] TYPE_COLUMN = {GAMES_TYPE_KEY, GAMES_TYPE};
	
	public static final String R_GAMES_TABLE = "reoccuring_games";
	public static final String R_GAMES_START_DATE = "start_date";
	public static final String R_GAMES_END_DATE = "end_date";
	public static final String R_GAMES_KEY ="id";
	public static final String R_MONDAY = "monday";
	public static final String R_TUESDAY = "tuesday";
	public static final String R_WEDNESDAY = "wednesday";
	public static final String R_THURSDAY = "thursday";
	public static final String R_FRIDAY = "friday";
	public static final String R_SATURDAY = "saturday";
	public static final String R_SUNDAY = "sunday";

	public static final String[] REOCCURING_GAMES_COLUMNS = {R_GAMES_KEY, R_GAMES_START_DATE, R_GAMES_END_DATE, GAMES_SPORT, GAMES_LOCATION, GAMES_TIME, GAMES_TITLE, GAMES_SUMMARY, 
		GAMES_CREATING_USER, GAMES_MAX_ATTENDING, GAMES_CREATED, GAMES_SPECIFIC_USER, R_MONDAY, R_TUESDAY, R_WEDNESDAY, R_THURSDAY, R_FRIDAY, R_SATURDAY, R_SUNDAY};
	
	public void addReoccuringGame(ReoccuringGame g)
	{
		
	}
	public List<String> topThreeSports()
	{
	/*	SELECT	GAMES_SPORT
		FROM	GAMES_TABLE
		WHERE	GAMES_ATTENDING_IND = '3'
		ORDER BY COUNT(GAMES_SPORT) limit 3 */
		
		List<String> top_sports = new LinkedList<String>();

		SQLiteDatabase db = this.getReadableDatabase();
		String query = 	"SELECT " + GAMES_SPORT + ", COUNT(" + GAMES_SPORT + ")" +
						" FROM " + GAMES_TABLE + 
						" WHERE " + GAMES_ATTENDING_IND + "=" + 3 + " AND " + GAMES_SPECIFIC_USER + "='" + Global.current_user.getAlias() + "'" +
						" GROUP BY " + GAMES_SPORT + 
						" ORDER BY COUNT(" + GAMES_SPORT + ") desc limit 3";
		
		//String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_ATTENDING_IND + "=" + 3;
		
		Log.d("Query", query);
		Cursor cursor = db.rawQuery(query, null);
		int x = 0;
		
		while(cursor.moveToNext())
		{
			top_sports.add(cursor.getString(0));
			x++;
		}
		Log.d("Database", "" + x);
		db.close();
		return top_sports;
	}
	public User login(String alias, String password)
	{
		SQLiteDatabase db = this.getReadableDatabase();
		
		String query = "SELECT * FROM " + USER_TABLE + " where " + USER_ALIAS + "='" + alias + "' AND "
				+ USER_PASSWORD + "='" + password + "'";
		Cursor cursor = db.rawQuery(query, null);
		
		User return_user = new User();
		
		if(!cursor.moveToFirst())
		{
			Log.d("Database", "User information isn't stored locally");
			return return_user;
		}
		else //means that the information is stored locally
		{
			return_user.setKey(Integer.parseInt(cursor.getString(0)));
			return_user.setName(cursor.getString(1));
			return_user.setAlias(cursor.getString(2));
			return_user.setGender(cursor.getString(4));
			return_user.setAge(Integer.parseInt(cursor.getString(3))); //switched age and gender. May have to switch back
			return_user.setDescription(cursor.getString(5));
			return_user.setProficiencies(cursor.getString(6));
			return_user.setPassword(cursor.getString(7));
			return_user.setFriends(cursor.getString(8));
			return_user.setPreferences(cursor.getInt(9));
		}
		db.close();
		return return_user;
	}
	public boolean addFriend(int user_key, int friend_key)
	{
		String friends_list = "";
		
		SQLiteDatabase db = this.getWritableDatabase();
		
		String get_friends_query = "SELECT friends FROM " + USER_TABLE + " WHERE " + USER_KEY + " = " + user_key;
		Cursor cursor = db.rawQuery(get_friends_query, null);
		
		if(!cursor.moveToFirst())
		{
			Log.d("Database", "Something really isn't correct");
			return false;
		}
		else
		{
			friends_list = cursor.getString(0);
			Log.d("Database", "Frieds List contains: " + friends_list);
		}
		
		String[] remover = friends_list.split("~");
		StringBuilder s = new StringBuilder();
		
		Log.d("Database", "Friends List size: " + remover.length);
		int x = 1;
		if(!friends_list.equals(""))
		{
			while(x<remover.length)
			{
				Log.d("Database", "Friend: " + x);
				Log.d("Database", "ID: " + remover[x]);
				if(Integer.parseInt(remover[x]) == friend_key)
				{
					Log.d("Database", "Error friend already in the list");
					return false;
				}
				x++;
			}
		}
		s.append(friends_list);
		s.append("~"+friend_key);
		
		friends_list = s.toString();
		
		ContentValues values = new ContentValues();
		values.put(USER_FRIENDS, friends_list);
		db.update(USER_TABLE, values, USER_KEY + "=" + user_key, null);
		db.close();
		
		return true;
	}
	public boolean removeFriend(int user_key, int friend_key)
	{
		String friends_list = "";
		
		SQLiteDatabase db = this.getWritableDatabase();
		
		String get_friends_query = "SELECT " + USER_FRIENDS + " FROM " + USER_TABLE + " WHERE " + USER_KEY + " = " + user_key;
		Cursor cursor = db.rawQuery(get_friends_query, null);
		
		if(!cursor.moveToFirst())
		{
			Log.d("Database", "Something really isn't correct");
			return false;
		}
		else
		{
			friends_list = cursor.getString(0);
		}
		String[] remover = friends_list.split("~");
		StringBuilder s = new StringBuilder();
		
		int x = 1;
		while(x<remover.length)
		{
			if(!(Integer.parseInt(remover[x]) == friend_key))
			{
				s.append("~" + remover[x]);
			}
			x++;
		}
		friends_list = s.toString();
		ContentValues values = new ContentValues();
		values.put(USER_FRIENDS, friends_list);
		db.update(USER_TABLE, values, USER_KEY + "=" + user_key, null);
		db.close();
		
		return true;
	}
	public ArrayList<Integer> getFriends(int user_key)
	{
		ArrayList<Integer> friends_list = new ArrayList<Integer>();
		String friends = "";
		
		SQLiteDatabase db = this.getWritableDatabase();
		
		String get_friends_query = "SELECT friends FROM " + USER_TABLE + " WHERE " + USER_KEY + " = " + user_key;
		Cursor cursor = db.rawQuery(get_friends_query, null);
		
		if(!cursor.moveToFirst())
		{
			Log.d("Database", "Something really isn't correct");
			return friends_list;
		}
		else
		{
			friends = cursor.getString(0);
			Log.d("Database", "Friends list is: " + friends);
			if(friends.equals(""))
			{
				return null;
			}
			String[] f = friends.split("~");
			Log.d("Database", "Friends list size is: " + f.length);
			int x = 0;
			while(x < f.length)
			{
				if(x == 0)
				{
					x++;
					continue;
				}
				friends_list.add(Integer.parseInt(f[x]));
				x++;
			}
		}
		return friends_list;
	}
	public void addUser(User user)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		
		values.put(USER_KEY, user.getKey());
		values.put(USER_NAME, user.getName());
		values.put(USER_ALIAS, user.getAlias());
		values.put(USER_DESCRIPTION, user.getDescription());
		values.put(USER_PROFICIENCIES, user.getProficiencies());
		values.put(USER_AGE, user.getAge());
		values.put(USER_GENDER, user.getGender());
		values.put(USER_PASSWORD, user.getPassword());
		values.put(USER_FRIENDS, user.getFriends());
		values.put(USER_PREFERENCES, user.getPreferences());
		
		db.insert(USER_TABLE, null, values);
		db.close();
	}
	
	public User getUser()
	{
		Log.d("Database", "Getting User Info");
		SQLiteDatabase db = this.getReadableDatabase();
		
		String query = "SELECT * FROM " + USER_TABLE;
		Cursor cursor = db.rawQuery(query, null);
		
		User return_user = new User();
		if (!cursor.moveToFirst()) {
			Log.d("Database", "User info does not exist currently");
			return return_user;	
		}
		/*
		if(cursor != null)
		{
			cursor.moveToFirst();
		}
		else
		{
			Log.d("Database", "User info does not exist currently");
			return return_user;
		}
		*/
		return_user.setKey(Integer.parseInt(cursor.getString(0)));
		return_user.setName(cursor.getString(1));
		return_user.setAlias(cursor.getString(2));
		return_user.setGender(cursor.getString(4));
		return_user.setAge(Integer.parseInt(cursor.getString(3))); //switched age and gender. May have to switch back
		return_user.setDescription(cursor.getString(5));
		return_user.setProficiencies(cursor.getString(6));
		return_user.setFriends(cursor.getString(8));
		return_user.setPreferences(cursor.getInt(9));
		Log.d("Database", "Name : " + return_user.getName());
		db.close();
		return return_user;
	}
	public User updateUser(User user)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		/*String query = "UPDATE " + USER_TABLE + " SET " + USER_NAME + "='" + user.getName() + "'," + USER_ALIAS + "='" + user.getAlias() + "'," + USER_GENDER + "='" + user.getGender() +"'," + 
		USER_AGE + "='" + user.getAge() + "'," + USER_DESCRIPTION + "='" + user.getDescription() +"'," + USER_PROFICIENCIES + "='" + user.getProficiencies() + "' " +
				"WHERE " + USER_KEY + "=" + user.getKey();*/
		ContentValues values = new ContentValues();
		values.put(USER_NAME, user.getName());
		values.put(USER_ALIAS, user.getAlias());
		values.put(USER_GENDER, user.getGender());
		values.put(USER_AGE, user.getAge());
		values.put(USER_DESCRIPTION, user.getDescription());
		values.put(USER_PROFICIENCIES, user.getProficiencies());
		
		db.update(USER_TABLE, values, USER_KEY + "=" + user.getKey(), null);
		db.close();
		
		return user;
	}
	public void updatePreferences(User user, int pref)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		ContentValues values = new ContentValues();
		values.put(USER_PREFERENCES, user.getPreferences());
		
		db.update(USER_TABLE, values, USER_KEY + "=" + user.getKey(), null);
		db.close();
	}
	public void setPastEvents()
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		// get the current date and time
		DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd HHmm");
		Calendar cal = Calendar.getInstance();
		String date_time = dateFormat.format(cal.getTime());
		String date = date_time.substring(0, 8);
		String time = date_time.substring(9);
		Log.d("DB", date);
		Log.d("DB", time);
		// query that parses through the whole entire list of events
		String query = "SELECT " + GAMES_KEY + " FROM " + GAMES_TABLE + " WHERE (" + GAMES_DATE + "<'" + date + "' OR (" + GAMES_DATE + "='" + date + "' AND " + GAMES_TIME + "<'" + time +"'))"
				+ " AND " + GAMES_SPECIFIC_USER + "='" + Global.current_user.getAlias() +"'";
		Cursor date_cursor = db.rawQuery(query, null);
		
		// have a cursor that moves through the selected values and changes their indicator values and updates the table
		while(date_cursor.moveToNext())
		{
			ContentValues values = new ContentValues();
			values.put(GAMES_ATTENDING_IND, 3);
			db.update(GAMES_TABLE, values, GAMES_KEY + "=" + date_cursor.getString(0), null);
		/*	String update_query = "UPDATE " + GAMES_TABLE +
								" SET " + GAMES_ATTENDING_IND + "=" + 3 +
								" WHERE " + GAMES_KEY + "=" + date_cursor.getString(0);
			Cursor return_value = db.rawQuery(update_query, null);*/
			Log.d("Database", "updating stuffer");
			
			Log.d("Database", "Updated a value given the current date");
		}
		db.close();
	}
	public void addEvent(Event event)
	{
		Log.d("Add Event", "Adding event : " + event.getTitle());
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		
		/* Uncomment out when we get the key back from the server */
		values.put(GAMES_KEY, event.getKey());
		values.put(GAMES_SPORT, event.getSport());
		values.put(GAMES_DATE, event.getDate());
		values.put(GAMES_LOCATION, event.getLocation());
		values.put(GAMES_TIME, event.getTime());
		values.put(GAMES_TITLE, event.getTitle());
		values.put(GAMES_SUMMARY, event.getSummary());
		values.put(GAMES_CREATING_USER, event.getCreating_user());
		values.put(GAMES_ATTENDING_IND, event.getAttendingInd());
		values.put(GAMES_MAX_ATTENDING, event.getMaxPlayers());
		values.put(GAMES_CREATED, event.getCreated());
		values.put(GAMES_SPECIFIC_USER, event.getSpecificUser());
		values.put(GAMES_ATTENDING, event.getAttending());
		
		
		db.insert(GAMES_TABLE, null, values);
		db.close();
		Log.d("Add Event", "Event Add Complete");
	}
	public void joinEvent(Event event)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		
		values.put(GAMES_KEY, event.getKey());
		values.put(GAMES_SPORT, event.getSport());
		values.put(GAMES_DATE, event.getDate());
		values.put(GAMES_LOCATION, event.getLocation());
		values.put(GAMES_TIME, event.getTime());
		values.put(GAMES_TITLE, event.getTitle());
		values.put(GAMES_SUMMARY, event.getSummary());
		values.put(GAMES_CREATING_USER, event.getCreating_user());
		values.put(GAMES_ATTENDING_IND, event.getAttendingInd());
		values.put(GAMES_MAX_ATTENDING, event.getMaxPlayers());
		values.put(GAMES_CREATED, event.getCreated());
		values.put(GAMES_SPECIFIC_USER, event.getSpecificUser());
		Log.d("DB", event.getSpecificUser());
		values.put(GAMES_ATTENDING, event.getAttending());

		Log.d("DB", "Joining "+event.getKey());
		db.insert(GAMES_TABLE, null, values);
		db.close();
	}
	public void updateAttending(int key, int at)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		values.put(GAMES_ATTENDING, at);
		db.update(GAMES_TABLE, values, GAMES_KEY + "=" + key, null);
		db.close();
	}
	public Event deleteEvent(Event event)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		db.delete(GAMES_TABLE, GAMES_KEY + "=" + event.getKey(), null);
		db.close();
		return event;
	}
	public Event updateEvent(Event event)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		//String query = "SELECT * FROM "+ GAMES_TABLE + "WHERE " + GAMES_KEY + "=" + event.getKey();
		//Cursor cursor = db.rawQuery(query, null);
		
		ContentValues values = new ContentValues();
		values.put(GAMES_KEY, event.getKey());
		values.put(GAMES_SPORT, event.getSport());
		values.put(GAMES_LOCATION, event.getLocation());
		values.put(GAMES_DATE, event.getDate());
		values.put(GAMES_TIME, event.getTime());
		values.put(GAMES_TITLE, event.getTitle());
		values.put(GAMES_SUMMARY, event.getSummary());
		values.put(GAMES_CREATING_USER, event.getCreating_user());
		values.put(GAMES_ATTENDING_IND, event.getAttending_ind());
		values.put(GAMES_MAX_ATTENDING, event.getMaxPlayers());
		values.put(GAMES_CREATED, event.getCreated());
		values.put(GAMES_SPECIFIC_USER, event.getSpecificUser());
		values.put(GAMES_ATTENDING, event.getAttending());

		
		db.update(GAMES_TABLE, values, "key = ", new String[]{String.valueOf(event.getKey())});
		db.close();
		return event;
	}
	public List<Event> getPreviousEvents()
	{
		List<Event> events = new LinkedList<Event>();
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_ATTENDING_IND + "=3" + " AND " + GAMES_SPECIFIC_USER + "='" + Global.current_user.getAlias() + "'";
		SQLiteDatabase db = this.getWritableDatabase();
		
		Cursor cursor = db.rawQuery(query, null);
		Event event = null;
		
		if(cursor.moveToFirst())
		{
			int x = 0;
			do
			{
				event = new Event();
				event.setKey(Integer.parseInt(cursor.getString(0)));
				event.setSport(cursor.getString(1));
				event.setLocation(cursor.getString(2));
				event.setDate(cursor.getString(3));
				event.setTime(cursor.getString(4));
				event.setTitle(cursor.getString(5));
				event.setSummary(cursor.getString(6));
				event.setCreating_user(cursor.getString(7));
				event.setAttending_ind(Integer.parseInt(cursor.getString(8)));
				event.setMaxPlayers(cursor.getInt(9));
				event.setSpecificUser(cursor.getString(10));
				event.setAttending(cursor.getInt(11));
				events.add(event);
				x++;
			}while(cursor.moveToNext());
		}
		db.close();
		return events;
	}
	public List<Event> getJoinedGames()
	{
		List<Event> events = new LinkedList<Event>();
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE (" + GAMES_ATTENDING_IND + "=1 OR " + GAMES_ATTENDING_IND + "=2) AND " + GAMES_SPECIFIC_USER + "='" + Global.current_user.getAlias() +"'";
		Log.d("DB", query);
		SQLiteDatabase db = this.getWritableDatabase();
		Cursor cursor = db.rawQuery(query, null);
		Event event = null;
		
		if(cursor.moveToFirst())
		{
			int x = 0;
			do
			{
				event = new Event();
				event.setKey(Integer.parseInt(cursor.getString(0)));
				event.setSport(cursor.getString(1));
				event.setLocation(cursor.getString(2));
				event.setDate(cursor.getString(3));
				event.setTime(cursor.getString(4));
				event.setTitle(cursor.getString(5));
				event.setSummary(cursor.getString(6));
				event.setCreating_user(cursor.getString(7));
				event.setAttending_ind(Integer.parseInt(cursor.getString(8)));
				event.setMaxPlayers(cursor.getInt(9));
				event.setSpecificUser(cursor.getString(10));
				Log.d("DB", cursor.getString(10));
				event.setAttending(cursor.getInt(11));
				events.add(event);
				x++;
			}while(x < 5 && cursor.moveToNext());
		}
		db.close();
		return events;
	}
	public List<Event> getCreatedEvents()
	{
		// crazy changes
		// double crazy changes
		List<Event> events = new LinkedList<Event>();
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_CREATED + "='yes' AND " + GAMES_SPECIFIC_USER + "='" + Global.current_user.getAlias() + "'";
		SQLiteDatabase db = this.getWritableDatabase();
		
		Cursor cursor = db.rawQuery(query, null);
		Event event = null;
		
		if(cursor.moveToFirst())
		{
			int x = 0;
			do
			{
				event = new Event();
				event.setKey(Integer.parseInt(cursor.getString(0)));
				event.setSport(cursor.getString(1));
				event.setLocation(cursor.getString(2));
				event.setDate(cursor.getString(3));
				event.setTime(cursor.getString(4));
				event.setTitle(cursor.getString(5));
				event.setSummary(cursor.getString(6));
				event.setCreating_user(cursor.getString(7));
				event.setAttending_ind(Integer.parseInt(cursor.getString(8)));
				event.setMaxPlayers(cursor.getInt(9));
				event.setSpecificUser(cursor.getString(10));
				event.setAttending(cursor.getInt(11));
				events.add(event);
				x++;
			}while(cursor.moveToNext());
		}
		db.close();
		return events;
	}
	public List<String> getSports()
	{
		List<String> sport_types = new LinkedList<String>();
		
		String query = "SELECT * FROM " + GAMES_TYPE_TABLE;
		SQLiteDatabase db = this.getReadableDatabase();
		
		Cursor cursor = db.rawQuery(query, null);
		int x = 0;
		
		while(cursor.moveToNext())
		{
			sport_types.add(cursor.getString(0));
			x++;
		}
		
		db.close();
		return sport_types;
	}
	public void addSports()
	{
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		
		String query = "SELECT * FROM " + GAMES_TYPE_TABLE;
		Cursor cursor = db.rawQuery(query, null);
		if(!cursor.moveToNext())
		{
			values.put(GAMES_TYPE, "Football");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Soccer");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Baseball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Basketball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Cricket");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Disc Golf");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Golf");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Wallyball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Bowling");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Volleyball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Sand Voleyball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Ultimate Frisbee");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Ping-Pong");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Floor Hockey");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Dodgeball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Racquetball");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Squash");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Badminton");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Tennis");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Nerf Wars");
			db.insert(GAMES_TYPE_TABLE, null, values);
			values.put(GAMES_TYPE, "Other");
			db.insert(GAMES_TYPE_TABLE, null, values);
		}
		db.close();
	}

}
