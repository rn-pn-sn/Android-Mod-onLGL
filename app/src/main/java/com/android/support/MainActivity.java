package com.android.support;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {
    static {
        // When you change the lib name, change also on Android.mk file... Both must have same name
        System.loadLibrary("MyLibName");
    }

    static native void Init(Context context);

    // for smali option call method
    public static void onCreate(Context context) {
        CrashHandler.init(context);
        if (context instanceof Activity) {
            Init(context);
        } else {
            Log.e("Mod", "Failed to launch the mod");
        }
    }

    //Only if you have changed MainActivity to yours and you wanna call game's activity.
    public String GameActivity = "com.unity3d.player.UnityPlayerActivity";
    public boolean hasLaunched = false;

    //for activity call method, please refer to README.md
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //To launch game activity
        if (!hasLaunched) {
            try {
                //Start service
                hasLaunched = true;
                //Launch mod.
                MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(MainActivity.this.GameActivity)));
                CrashHandler.init(this);
                Init(this);
                return;
            } catch (ClassNotFoundException e) {
                Log.e("Mod", "Error. Game's main activity does not exist");
            }
        }
        CrashHandler.init(this);
    }
}