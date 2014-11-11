package cs307.team7.playboiler;

import java.util.LinkedList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;

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
				"proficiencies TEXT, " + "password TEXT)";
		String CREATE_GAMES_TABLE = "CREATE TABLE past_games (" + "id INTEGER PRIMARY KEY, " + "sport TEXT, " + "location TEXT, " + 
				"date TEXT, " + "time TEXT, " + "title TEXT, " + "summary TEXT, " + "creating_user TEXT, " + "attending_ind INTEGER, " + "max_attending INTEGER)";
		String CREATE_GAMES_TYPE_TABLE = "CREATE TABLE types_of_games (" + "sport_type TEXT PRIMARY KEY)";
		db.execSQL(CREATE_GAMES_TABLE);
		db.execSQL(CREATE_USER_PROFILE_TABLE);
		db.execSQL(CREATE_GAMES_TYPE_TABLE);
	}
	
	@Override
	public void onUpgrade(SQLiteDatabase db, int olderVersion, int newVersion)
	{
		db.execSQL("DROP TABLE IF EXISTS user_profile");
		db.execSQL("DROP TABLE IF EXISTS past_games");
		db.execSQL("DROP TABLE IF EXISTS types_of_games");
		
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
	
	public static final String[] USER_COLUMNS = {USER_KEY, USER_NAME, USER_ALIAS, USER_GENDER, USER_AGE, USER_DESCRIPTION
		, USER_PROFICIENCIES, USER_PASSWORD};
	
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
	
	public static final String[] GAMES_COLUMNS = {GAMES_KEY, GAMES_SPORT, GAMES_LOCATION, GAMES_DATE, GAMES_TIME,  GAMES_TITLE, GAMES_SUMMARY,
		GAMES_CREATING_USER, GAMES_ATTENDING_IND, GAMES_MAX_ATTENDING};
	
	public static final String GAMES_TYPE_TABLE = "types_of_games";
	public static final String GAMES_TYPE_KEY = "id";
	public static final String GAMES_TYPE = "sport_type";
	public static final String[] TYPE_COLUMN = {GAMES_TYPE_KEY, GAMES_TYPE};
	
	public List<String> topThreeSports()
	{
	/*	SELECT	GAMES_SPORT
		FROM	GAMES_TABLE
		WHERE	GAMES_ATTENDING_IND = '3'
		ORDER BY COUNT(GAMES_SPORT) limit 3 */
		
		List<String> top_sports = new LinkedList<String>();

		SQLiteDatabase db = this.getReadableDatabase();
		String query = 	"SELECT " + GAMES_SPORT + 
						" FROM " + GAMES_TABLE + 
						" WHERE " + GAMES_ATTENDING_IND + "='" + 3 + "'" + 
						" GROUP BY " + GAMES_SPORT +
						" ORDER BY COUNT(" + GAMES_SPORT + ") desc limit 3";
		
		Cursor cursor = db.rawQuery(query, null);
		int x = 0;
		
		while(cursor.moveToNext())
		{
			top_sports.add(cursor.getString(0));
			x++;
		}
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
		}
		db.close();
		return return_user;
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
		Log.d("Database", "Name : " + return_user.getName());
		db.close();
		return return_user;
	}
	public User updateUser(User user)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		String query = "UPDATE " + USER_TABLE + " SET " + USER_NAME + "='" + user.getName() + "'," + USER_ALIAS + "='" + user.getAlias() + "'," + USER_GENDER + "='" + user.getGender() +"'," + 
		USER_AGE + "='" + user.getAge() + "'," + USER_DESCRIPTION + "='" + user.getDescription() +"'," + USER_PROFICIENCIES + "='" + user.getProficiencies() + "' " +
				"WHERE " + USER_KEY + "=" + user.getKey();
		Cursor cursor = db.rawQuery(query, null);
		Log.d("Database", query);
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
		}
		db.close();
	/*	ContentValues values = new ContentValues();
		values.put(USER_KEY, user.getKey());
		values.put(USER_NAME, user.getName());
		values.put(USER_ALIAS, user.getAlias());
		values.put(USER_GENDER, user.getGender());
		values.put(USER_AGE, user.getAge());
		values.put(USER_DESCRIPTION, user.getDescription());
		values.put(USER_PROFICIENCIES, user.getProficiencies());

		
		db.update(USER_TABLE, values, "key =", new String[]{String.valueOf(user.getKey())});*/
		
		return user;
	}
	public void setPastEvents()
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		// get the current date and time
		DateFormat dateFormat = new SimpleDateFormat("yyyyMMDD HHmm");
		Calendar cal = Calendar.getInstance();
		String date_time = dateFormat.format(cal.getTime());
		String date = date_time.substring(0, 8);
		String time = date_time.substring(9);
		
		// query that parses through the whole entire list of events
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_DATE + "<'" + date + "' OR (" + GAMES_DATE + "='" + date + "' AND " + GAMES_TIME + "<'" + time +"')";
		Cursor date_cursor = db.rawQuery(query, null);
		
		// have a cursor that moves through the selected values and changes their indicator values and updates the table
		while(date_cursor.moveToNext())
		{
			String update_query = "UPDATE " + GAMES_TABLE +
								" SET " + GAMES_ATTENDING_IND + "='" + 3 + "'" +
								" WHERE " + GAMES_KEY + "=" + date_cursor.getString(0);
			Cursor return_value = db.rawQuery(update_query, null);
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
		
		db.insert(GAMES_TABLE, null, values);
		db.close();
	}
	public Event deleteEvent(Event event)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		String query = "DELETE FROM " + GAMES_TABLE + "WHERE " + GAMES_KEY + "=" + event.getKey();
		
		db.rawQuery(query, null);
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
		
		db.update(GAMES_TABLE, values, "key = ", new String[]{String.valueOf(event.getKey())});
		db.close();
		return event;
	}
	public List<Event> getPreviousEvents()
	{
		List<Event> events = new LinkedList<Event>();
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_ATTENDING_IND + "=0";
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
				events.add(event);
				x++;
			}while(x < 5 && cursor.moveToNext());
		}
		db.close();
		return events;
	}
	public List<Event> getJoinedGames()
	{
		List<Event> events = new LinkedList<Event>();
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_ATTENDING_IND + "=1";
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
		
		String query = "SELECT * FROM " + GAMES_TABLE + " WHERE " + GAMES_ATTENDING_IND + "=2";
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
				events.add(event);
				x++;
			}while(x < 5 && cursor.moveToNext());
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
