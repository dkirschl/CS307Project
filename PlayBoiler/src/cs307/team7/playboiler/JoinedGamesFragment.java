package cs307.team7.playboiler;

import android.app.Fragment;
import android.os.Bundle;

public class JoinedGamesFragment extends Fragment {
	
	public static JoinedGamesFragment newInstance(int sectionNumber) {
		JoinedGamesFragment fragment = new JoinedGamesFragment();
		Bundle args = new Bundle();
		fragment.setArguments(args);
		return fragment;
	}
	
	public JoinedGamesFragment() {
		
	}

}
