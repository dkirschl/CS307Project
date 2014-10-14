package cs307.team7.playboiler;

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
