package cs307.team7.playboiler;

import java.util.concurrent.ExecutionException;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.FragmentManager;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity
        implements NavigationDrawerFragment.NavigationDrawerCallbacks {

    /**
     * Fragment managing the behaviors, interactions and presentation of the navigation drawer.
     */
    private NavigationDrawerFragment mNavigationDrawerFragment;

    /**
     * Used to store the last screen title. For use in {@link #restoreActionBar()}.
     */
    private CharSequence mTitle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d("app", "Ping");

        mNavigationDrawerFragment = (NavigationDrawerFragment)
                getFragmentManager().findFragmentById(R.id.navigation_drawer);
        mTitle = getTitle();

        // Set up the drawer.
        mNavigationDrawerFragment.setUp(
                R.id.navigation_drawer,
                (DrawerLayout) findViewById(R.id.drawer_layout));
        //Setup database and check for user info
        Log.d("Ping", "Ping");
        Global.userDatabase = new MySqlLiteHelper(this);
        //Global.current_user = Global.userDatabase.getUser();
        
        final Dialog d = new Dialog(this);
    	d.setContentView(R.layout.login);
    	d.setCancelable(false);
    	
    	d.setTitle("Login");
    	Button login = (Button) d.findViewById(R.id.login);
    	Button create = (Button) d.findViewById(R.id.create);
    	create.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				final Dialog login = new Dialog(v.getContext());
				login.setContentView(R.layout.create_user);
				Button createUser = (Button) login.findViewById(R.id.createButton);
				final EditText alias = (EditText) login.findViewById(R.id.newAlias);
				final EditText name = (EditText) login.findViewById(R.id.newName);
				final EditText password = (EditText) login.findViewById(R.id.newPassword);
				final EditText age = (EditText) login.findViewById(R.id.newAge);
				final EditText gender = (EditText) login.findViewById(R.id.newGender);
				createUser.setOnClickListener(new View.OnClickListener() {
					
					@Override
					public void onClick(View v) {
						
						StringBuilder m = new StringBuilder();
						m.append("/crus/");
						m.append(password.getText());
						Global.addSpaces(m,20 - password.getText().length());
						m.append("/");
						m.append(name.getText());
						Global.addSpaces(m,50 - name.getText().length());
						m.append("/");
						m.append(alias.getText());
						Global.addSpaces(m,20 - alias.getText().length());
						m.append("/");
						m.append(age.getText());
						Global.addSpaces(m,2 - age.getText().length());
						m.append("/");
						m.append(gender.getText());
						Global.addSpaces(m,1 - gender.getText().length());
						m.append("/");
						m.append("HTTP/1.0\r\n");
						
						NetworkHandler nh = new NetworkHandler();
						//nh.execute(m.toString());
						String result = null;
						try {
							result = nh.execute(m.toString()).get();
						} catch (InterruptedException e1) {
							e1.printStackTrace();
						} catch (ExecutionException e1) {
							e1.printStackTrace();
						}
						result = result.replace("|", "");
						int res = Integer.parseInt(result.substring(7));
						if (res == -1) {
							Toast.makeText(v.getContext(), "Alias already exists. Please try again.", Toast.LENGTH_LONG).show();
						} else {
							User u = new User(res, name.getText().toString(), alias.getText().toString(), gender.getText().toString(), Integer.parseInt(age.getText().toString()), password.getText().toString());
							Global.userDatabase.addUser(u);
						
							login.cancel();
						}
					}
				});
				login.show();
			}
    		
    	});
    	final EditText enteredUser = (EditText) d.findViewById(R.id.userEdit);
    	final EditText enteredPassword = (EditText) d.findViewById(R.id.passEdit);
    	login.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				User u = Global.userDatabase.login(enteredUser.getText().toString(), enteredPassword.getText().toString());
				if (u.getKey() == -1) {
					//check global database
					
					//login failed
					Toast.makeText(v.getContext(), "Login Failed", Toast.LENGTH_LONG).show();
				} else {
					Global.userDatabase.setPastEvents();
					Log.d("USER INFO", u.getName() + " " + u.getPassword() + " " + u.getAlias());
					Global.current_user = u;
					d.cancel();
				}
			}
		});
    	d.show(); 
        
    }

    @Override
    public void onNavigationDrawerItemSelected(int position) {
        // update the main content by replacing fragments
    	
        FragmentManager fragmentManager = getFragmentManager();
        switch(position) {
        	case 0:
        			fragmentManager.beginTransaction()
        			.replace(R.id.container, FrontPageFragment.newInstance(position + 1))
        			.commit();
        			break;
        	case 1:
        			fragmentManager.beginTransaction()
        			.replace(R.id.container, ProfileFragment.newInstance(position + 1))
        			.commit();
        			break;
        	case 2:
    			fragmentManager.beginTransaction()
    			.replace(R.id.container, SearchFragment.newInstance(position + 1))
    			.commit();
    			break;
        	case 3:
        		fragmentManager.beginTransaction()
        		.replace(R.id.container, CreateEventFragment.newInstance(position+1))
        		.commit();
        		break;
        	case 4:
        		fragmentManager.beginTransaction()
        		.replace(R.id.container, CreatedEventsFragment.newInstance(position+1))
        		.commit();
        		break;
        	case 5:
        		fragmentManager.beginTransaction()
        		.replace(R.id.container, JoinedGamesFragment.newInstance(position+1))
        		.commit();
        		break;
        
        }
    	
    }
    
    

    public void onSectionAttached(int number) {
        switch (number) {
            case 1:
                mTitle = getString(R.string.title_section1);
                break;
            case 2:
                mTitle = getString(R.string.title_section2);
                break;
            case 3:
                mTitle = getString(R.string.title_section3);
                break;
            case 4:
            	mTitle = getString(R.string.title_section4);
            	break;
            case 5:
            	mTitle = getString(R.string.title_section5);
            	break;
        }
    }

    public void restoreActionBar() {
        ActionBar actionBar = getActionBar();
        actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_STANDARD);
        actionBar.setDisplayShowTitleEnabled(true);
        actionBar.setTitle(mTitle);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        if (!mNavigationDrawerFragment.isDrawerOpen()) {
            // Only show items in the action bar relevant to this screen
            // if the drawer is not showing. Otherwise, let the drawer
            // decide what to show in the action bar.
            getMenuInflater().inflate(R.menu.main, menu);
            restoreActionBar();
            return true;
        }
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public class LoginListener implements Dialog.OnClickListener {

		@Override
		public void onClick(DialogInterface dialog, int which) {
			
			
		}
    	
    }
    

    

}
