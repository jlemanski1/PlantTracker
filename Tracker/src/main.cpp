#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <esp_sleep.h>

#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

#include "secrets.h"

#define HOUR 3600e6 // Hour in microseconds
#define MINUTE 60e6 // Minute in microseconds, used for testing

#define MOISTURE_SENSOR 36 // GPIO pin of the sensor

/**
 * Plant Tracker
 *
 * This program is designed to run on an ESP32-WROOM-32D based microcontroller,
 * and uses a capacitive soil moisture sensor to read soil moisture content,
 * pushing the values to a Firebase Realtime Database.
 *
 * Soil Sensor:
 * https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193
 */
FirebaseData fbData;
FirebaseAuth fbAuth;
FirebaseConfig fbConfig;

bool signupOK = false;

/**
 * Connect to wifi
 */
void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting:");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

/**
 * Handles setting up the connection to firebase & authenticating
 */
void setupFirebase() {
  fbConfig.api_key = FIREBASE_KEY;
  fbConfig.database_url = FIREBASE_DB_URL;

  fbAuth.user.email = FIREBASE_EMAIL;
  fbAuth.user.password = FIREBASE_PASSWORD;

  /* Assign the callback function for the long running token generation task
   * from "addons/TokenHelper"*/
  fbConfig.token_status_callback = tokenStatusCallback;

  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectNetwork(true);
}

/**
 * Runs once on initial start or when hardware reset.
 * Sets the deep sleep timer, connects to wifi and Firebase
 */
void setup() {
  Serial.begin(115200);

  esp_sleep_enable_timer_wakeup(MINUTE); // TODO: Set to HOUR after testing

  setupWifi();
  setupFirebase();
}

/**
 * Main loop: Uploads the sensor value to Firebase Real Time Database then
 * initiates deep sleep for the configured time.
 */
void loop() {
  if (Firebase.ready()) {

    // Read the moisture sensor
    int moisture = analogRead(MOISTURE_SENSOR);

    FirebaseJson json;

    json.set("moisture", moisture);

    // Push data with timestamp
    Serial.printf("Push data with timestamp... %s\n",
                  Firebase.RTDB.pushJSON(&fbData, "/", &json)
                      ? "Success"
                      : fbData.errorReason().c_str());

  } else {
    Serial.println("Error with firebase and/or signup");
  }

  // Enter deep sleep
  esp_deep_sleep_start();
}