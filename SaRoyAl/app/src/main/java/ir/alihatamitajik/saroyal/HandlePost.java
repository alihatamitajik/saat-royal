package ir.alihatamitajik.saroyal;

import android.os.AsyncTask;

import java.io.IOException;

import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;


public abstract class HandlePost extends AsyncTask<String, String, Boolean> {
    @Override
    protected Boolean doInBackground(String[] objects) {
        OkHttpClient client = new OkHttpClient().newBuilder()
                .build();
        MediaType mediaType = MediaType.parse("text/plain");
        MultipartBody.Builder bodyBuilder = new MultipartBody.Builder().setType(MultipartBody.FORM);
        for (int i = 0; i < objects.length/2; i+=1) {
            bodyBuilder.addFormDataPart(objects[2*i],objects[2*i+1]);
        }
        RequestBody body = bodyBuilder.build();
        Request request = new Request.Builder()
                .url("http://192.168.4.1/post")
                .method("POST", body)
                .build();
        try {
            Response response = client.newCall(request).execute();
            return response.code() == 200;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    @Override
    abstract public void onPostExecute(Boolean status);
}