package cs307.team7.playboiler;

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
				"proficiencies TEXT)";
		String CREATE_GAMES_TABLE = "CREATE TABLE past_games (" + "id INTEGER PRIMARY KEY AUTOINCREMENT, " + "sport TEXT, " + "location TEXT, " + 
				"date TEXT, " + "time TEXT, " + "title TEXT, " + "creating_user TEXT, " + "attending_ind INTEGER)";
		db.execSQL(CREATE_GAMES_TABLE);
		db.execSQL(CREATE_USER_PROFILE_TABLE);
	}
	
	@Override
	public void onUpgrade(SQLiteDatabase db, int olderVersion, int newVersion)
	{
		db.execSQL("DROP TABLE IF EXISTS user_profile");
		db.execSQL("DROP TABLE IF EXISTS past_games");
		
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
	
	public static final String[] USER_COLUMNS = {USER_KEY, USER_NAME, USER_ALIAS, USER_GENDER, USER_AGE, USER_DESCRIPTION
		, USER_PROFICIENCIES};
	
	public static final String GAMES_TABLE = "past_games";
	public static final String GAMES_KEY = "id";
	public static final String GAMES_SPORT = "sport";
	public static final String GAMES_LOCATION = "location";
	public static final String GAMES_DATE = "date";
	public static final String GAMES_TIME = "time";
	public static final String GAMES_TITLE = "title";
	public static final String GAMES_CREATING_USER = "creating_user";
	public static final String GAMES_ATTENDING_IND = "attending_ind";
	
	public static final String[] GAMES_COLUMNS = {GAMES_KEY, GAMES_SPORT, GAMES_LOCATION, GAMES_DATE, GAMES_TIME,  GAMES_TITLE, 
		GAMES_CREATING_USER, GAMES_ATTENDING_IND};
	
	public void addEvent(Event event)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		ContentValues values = new ContentValues();
		
		values.put(GAMES_KEY, event.getKey());
		values.put(GAMES_SPORT, event.getSport());
		values.put(GAMES_DATE, event.getDate());
		values.put(GAMES_LOCATION, event.getLocation());
		values.put(GAMES_TIME, event.getTime());
		values.put(GAMES_TITLE, event.getTitle());
		values.put(GAMES_CREATING_USER, event.getCreating_user());
		values.put(GAMES_ATTENDING_IND, event.getAttendingInd());
		
		db.insert(GAMES_TABLE, null, values);
		db.close();
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
		return return_user;
	}
	public User updateUser(User user)
	{
		SQLiteDatabase db = this.getWritableDatabase();
		
		ContentValues values = new ContentValues();
		values.put(USER_KEY, user.getKey());
		values.put(USER_NAME, user.getName());
		values.put(USER_ALIAS, user.getAlias());
		values.put(USER_GENDER, user.getGender());
		values.put(USER_AGE, user.getAge());
		values.put(USER_DESCRIPTION, user.getDescription());
		values.put(USER_PROFICIENCIES, user.getProficiencies());
		
		db.update(USER_TABLE, values, "key = ?", new String[]{String.valueOf(user.getKey())});
		db.close();
		return user;
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
		values.put(GAMES_CREATING_USER, event.getCreating_user());
		values.put(GAMES_ATTENDING_IND, event.getAttending_ind());
		
		db.update(GAMES_TABLE, values, "key = ?", new String[]{String.valueOf(event.getKey())});
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
				event.setCreating_user(cursor.getString(6));
				event.setAttending_ind(Integer.parseInt(cursor.getString(7)));
				events.add(event);
				x++;
			}while(x < 5 && cursor.moveToNext());
		}
		return events;
	}
	public List<Event> getFutureEvents()
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
				event.setCreating_user(cursor.getString(6));
				event.setAttending_ind(Integer.parseInt(cursor.getString(7)));
				events.add(event);
				x++;
			}while(x < 5 && cursor.moveToNext());
		}
		return events;
	}

}
