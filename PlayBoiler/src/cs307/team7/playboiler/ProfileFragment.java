package cs307.team7.playboiler;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.Dialog;
import android.app.Fragment;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebView.FindListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

public class ProfileFragment extends Fragment {
	 /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static ProfileFragment newInstance(int sectionNumber) {
        ProfileFragment fragment = new ProfileFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    public ProfileFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
    	
    	
        final View rootView = inflater.inflate(R.layout.profile, container, false);
        
        final TextView bio = (TextView) rootView.findViewById(R.id.bio);
        bio.setMovementMethod(new ScrollingMovementMethod());
        final TextView currName = (TextView) rootView.findViewById(R.id.name);
        final TextView currAlias = (TextView) rootView.findViewById(R.id.alias);
        final TextView currAge = (TextView) rootView.findViewById(R.id.ageCurrent);
        final TextView currGender = (TextView) rootView.findViewById(R.id.genderCurrent);
        final TextView topOne = (TextView) rootView.findViewById(R.id.sportOne);
        final TextView topTwo = (TextView) rootView.findViewById(R.id.sportTwo);
        final TextView topThree = (TextView) rootView.findViewById(R.id.sportThree);
        List<TextView> listOfBullshit = new ArrayList<TextView>();
        listOfBullshit.add(topOne);
        listOfBullshit.add(topTwo);
        listOfBullshit.add(topThree);
        
        List<String> topThreeSports = Global.userDatabase.topThreeSports();
        for (int i = 0; i < topThreeSports.size(); i++) {
        	listOfBullshit.get(i).setText(topThreeSports.get(i));
        }
        User user = Global.current_user;
        if (user.getKey() != -1) {
        	currName.setText(user.getName());
        	currAlias.setText(user.getAlias());
        	currAge.setText("" +user.getAge());
        	currGender.setText(user.getGender());
        	bio.setText(user.getDescription());
        }
        Button edit = (Button) rootView.findViewById(R.id.edit);
        edit.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				final Dialog d = new Dialog(rootView.getContext());
				
				d.setContentView(R.layout.editdialog);
				d.setTitle("Edit Your Profile");
				
				final EditText etName = (EditText) d.findViewById(R.id.nameEditBox);
				String n = (String) currName.getText();
				etName.setText(n);
				final EditText etAlias = (EditText) d.findViewById(R.id.aliasEditBox);
				n = (String) currAlias.getText();
				etAlias.setText(n);
				final EditText etAge = (EditText) d.findViewById(R.id.ageEditBox);
				n = (String) currAge.getText();
				etAge.setText(n);
				
				final RadioButton rbMale = (RadioButton) d.findViewById(R.id.male);
				final RadioButton rbFemale = (RadioButton) d.findViewById(R.id.female);
				if (currGender.getText().equals("Male")) {
					rbMale.setChecked(true);
					rbFemale.setChecked(false);
				} else {
					rbMale.setChecked(false);
					rbFemale.setChecked(true);
				}
				
				final EditText etBio = (EditText) d.findViewById(R.id.editAbout);
				etBio.setText(bio.getText());
				
				Button exit = (Button) d.findViewById(R.id.editExit);
				exit.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						d.cancel();
					}
				});
				
				
				Button done = (Button) d.findViewById(R.id.editDone);
				done.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						bio.setText(etBio.getText());
						currName.setText(etName.getText());
						currAlias.setText(etAlias.getText());
						currAge.setText(etAge.getText());
						if (rbMale.isChecked()) {
							currGender.setText("Male");
						} else {
							currGender.setText("Female");
						}
						User temp = new User(Global.current_user.getKey(), etName.getText().toString(), etAlias.getText().toString(), currGender.getText().toString(), Integer.parseInt(etAge.getText().toString()), etBio.getText().toString(), "", "");
						if (Global.current_user.getKey() != -1) {
							Global.userDatabase.updateUser(temp);
							//Global.userDatabase.addUser(temp);
							String m = "/crus/"+temp.getName()+"/"+temp.getAlias()+"/"+temp.getAge()+"/"+temp.getGender()+"/"+ temp.getPassword()+"password/ HTTP/1.0\r\n";
							NetworkHandler nh = new NetworkHandler();
							//nh.execute(m);
						} else {
							//error
							Toast.makeText(v.getContext(), "Error", Toast.LENGTH_LONG).show();
						}
						Global.current_user = temp;
						d.cancel();
					}
				});
				
				d.show();
			}
        	
        });
        //TextView textView = (TextView) rootView.findViewById(R.id.section_label);
        //textView.setText(Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER)));
        return rootView;
    	
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        //((MainActivity) activity).onSectionAttached(
                //getArguments().getInt(ARG_SECTION_NUMBER));
    }
}
