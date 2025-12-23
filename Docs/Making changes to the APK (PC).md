# 1. Know your game's main activity

Now we are looking for main activity

Decompile the game's APK file. Open `AndroidManifest.xml` and search after `<action android:name="android.intent.action.MAIN"/>`.

Example the game's main activity was `com.unity3d.player.UnityPlayerActivity`. Be sure to enable Word wrap so it is easier to read

![](https://i.imgur.com/FfOtc1K.png)

APK Easy Tool since it can read out location of main activity without decompiling APK

![](https://i.imgur.com/JQdPjyZ.png)

Note it somewhere to remember it. You can use any tools in your choice

# 2. Making corresponding changes

Decompile the game's APK

Now we need to launch your mod menu. There are 2 ways to launch your mod menu. First Method is recommended. 
Do NOT use both methods at the same time

## Method 1 - Launch from smali file

Locate to the game's path of main activity and open the **smali** file. If the game have multi dexes, it may be located in smali_classes2... please check all

With the path of the target game’s main activity which we determined earlier `com.unity3d.player.UnityPlayerActivity`. Think of it like a path `/com/unity3d/player/UnityPlayerActivity.smali`

Search for OnCreate method and paste this code inside

```
invoke-static {p0}, Lcom/android/support/MainActivity;->onCreate(Landroid/content/Context;)V
```

example:
```
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 9

    .line 594
	invoke-static {p0}, Lcom/android/support/MainActivity;->onCreate(Landroid/content/Context;)V
	
    invoke-super {p0, p1}, Lcom/unity3d/player/UnityPlayerActivity;->onCreate(Landroid/os/Bundle;)V
```

## Method 2 - Launch from AndroidManifest.xml

This requires overlay permission. Only use if you really want to launch your own activity

On yout `MainActivity.java`, put the game's main activity to `public String GameActivity`

![](https://i.imgur.com/jdacwvH.png)

Uncomment this code

```
Log.e("Mod", "Error. Game's main activity does not exist");
```

On `AndroidManifest.xml`, remove `<action android:name="android.intent.action.MAIN"/>` from the game's activity, like this:

![](https://i.imgur.com/z1RxPjc.png)

If you don't remove `<action android:name="android.intent.action.MAIN"/>` from the game's activity, your menu will not launch. `AndroidManifest.xml` can ONLY contain one `<action android:name="android.intent.action.MAIN"/>`

Add your activity tag. `com.android.support.MainActivity` is your main activity

```xml
<activity android:configChanges="keyboardHidden|orientation|screenSize"
    android:name="com.android.support.MainActivity" android:exported="true">
    <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>
</activity>
```

![](https://i.imgur.com/ZgbpTm8.png)

# 3. Building your project and copying files

Build the project to the APK file.
**Build** -> **Build Bundle(s)/APK(s)** -> **Build APK(s)**

If no errors occured, you did everything right and build will succeded. You will be notified that it has built successfully

![](https://i.imgur.com/WpSKV1L.png)

Click on **locate** to show you the location of **build.apk**. It is stored at `(your-project)\app\build\outputs\apk\app-debug.apk`

![](https://i.imgur.com/wBTPSLi.png)

Decompile your **app-debug.apk**.

Copy your mod menu from decompiled app-debug.apk smali to the game's smali folder. Example ours is com.android.support.Menu, we copy the `com` folder from **app-debug** `(decompiled app-debug\smali\com)` to the game's decompiled directory `(decompiled game)\(any smali folder)`

![](https://i.imgur.com/lU3Ne9l.png)
 
If the game have multidexes, just add your smali to the last `smali_classes` if possible to prevent compilation smali limit error `Unsigned short value out of range: xxxxx`

Copy the library file (.so) from **app-debug.apk** to the target game. Watch out the folder names, we don't want to mix them up, so make sure to copy our lib file to the correct architecture. Like our armeabi-v7a lib goes to the target games armeabi-v7a folder, arm64-v8a lib goes to the arm64-v8a folder...

PUTTING THE LIB FILE ON A WRONG ARCHITECTURE LIKE PUTTING ARM64 LIB TO ARMV7 WILL RESULT A CRASH!
 
![](https://i.imgur.com/PXA7dUt.png)
 
# 4. Compiling game apk
 
Now compile and sign the apk, and install it on your device

Congrats. You have successfully implemented a mod menu.
