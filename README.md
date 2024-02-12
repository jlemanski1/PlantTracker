# Plant Tracker
> Your stomach tells you when you're hungry, so why can't your plants do the same?

The Plant Tracker project is comprised of an ESP32 microcontroller, a capacitive soil moisture sensor, and leverages Firebase Realtime Database & Cloud Functions to alert you when your plant(s) need watering. The idea came to me for this project after I kept over/under-watering one of my plants that is hung high up on the ceiling. It's hard to tell how moist the soil is with it being so high, and so the obvious answer is networked plants! While there are a million low-tech ways to resolve this problem of mine, creating this serves as a fun weekend exercise in ESP32s, soldering, and 3D printing.


![Overview diagram of the data flow](../Assets/Plant%20Tracker.svg)
<img src="https://github.com/jlemanski1/PlantTracker/blob/main/Assets/Plant%20Tracker.svg">

---
## Hardware
- [ESP32 IOT Microcontroller](https://ca.robotshop.com/products/firebeetle-esp32-iot-microcontroller?variant=42413189267607)
- [Capacative Soil Moisture Sensor](https://ca.robotshop.com/products/gravity-moisture-sensor-corrosion-resistant?variant=42411303370903)
- Li-on battery




## Set up & Configuration
Due to the Wifi & Firebase connections, there is some configuration required before you can build and flash the image.

 1. Create a Firebase project & create a Realtime Database
	 a. From Project > Settings, note the webApiKey
	 b. From the realtime database section, note the database url
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
	#endif
	```
4. Build the code & Flash it to your ESP32 (You can either use the Arduino IDE or PlatformIO in VSCode for this step)



## Future Improvements

- Multiplant support


## Contributing

1. [Fork it](<https://github.com/jlemanski1/PlantTracker/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
