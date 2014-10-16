package cs307.team7.playboiler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

import android.os.AsyncTask;
import android.util.Log;

public class NetworkHandler extends AsyncTask<String, Void, String> {

	public static StringBuffer response;
	
	@Override
	protected String doInBackground(String... params) {
		String message = params[0];
		String message1 = "/simple.html";
		String url="http://____.cs.purdue.edu:_____"+message;
		URL obj = null;
		try {
			obj = new URL(url);
		} catch (MalformedURLException mue) {
			mue.printStackTrace();
		}
		HttpURLConnection con = null;
		try {
			con = (HttpURLConnection) obj.openConnection();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
		try {
			con.setRequestMethod("GET");
		} catch (ProtocolException pe) {
			pe.printStackTrace();
		}
		con.setRequestProperty("User-Agent", "CS307Lab");
		int responseCode = 0;
		try {
			responseCode = con.getResponseCode();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		Log.d("HTTP", "\nSending 'Get' request to URL : " + url);
		Log.d("HTTP", "Response Code : " + responseCode);
		
		BufferedReader in = null;
		try {
			in = new BufferedReader( new InputStreamReader(con.getInputStream()));
		} catch (IOException e) {
			e.printStackTrace();
		}
		String inputLine;
		response = new StringBuffer();
		
		try {
			while((inputLine = in.readLine()) != null) {
				response.append(inputLine);
			}
		} catch(IOException e) {
			e.printStackTrace();
		}
		try {
			in.close();
		} catch(IOException e) {
			e.printStackTrace();
		}
		//Print result
		Log.d("HTTP", response.toString());
		
		return null;
	}

}
