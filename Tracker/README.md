# Plant Tracker

## Set up & Configuration
Due to the Wifi & Firebase connections, there is some configuration required before you can build and flash the image.

 1. Create a Firebase project & create a Realtime Database
	 a. From Project > Settings, note the webApiKey
	 b. From the realtime database section, note the database url
	 c. From the authentication tab, create a user with an email and password (Does not have to be a real email)
 2. Clone the repo
 3. In the `src` directory, create a file named `secrets.h`
	 a. In this file, paste the following code:
	```
	#ifndef SECRETS_H
	#define  SECRETS_H
	#define  WIFI_SSID  "YOUR_WIFI_SSID"
	#define  WIFI_PASS  "YOUR_WIFI_PASSWORD"

	#define  FIREBASE_KEY  "YOUR_FIREBASE_API_KEY"
	#define  FIREBASE_DB_URL  "YOUR_FIREBASE_RTDB_URL"

	#define FIREBASE_EMAIL "YOUR_FIREBASE_USER_EMAIL"
	#define FIREBASE_PASSWORD "YOUR_FIREBASE_USER_PASSWORD"
	#endif
	```
	Note: cloud function secrets (ie. api keys) are managed with [Secret Manager](https://firebase.google.com/docs/functions/config-env?gen=2nd#create-secret)
4. Build the code & Flash it to your ESP32 using PlatformIO


## Contributing
1. [Fork it](<https://github.com/jlemanski1/PlantTracker/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
