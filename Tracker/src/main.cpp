#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <esp_sleep.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

#include "secrets.h"

#define HOUR 3600e6 // Hour in microseconds
#define MINUTE 60e6 // Minute in microseconds

#define MOISTURE_SENSOR 36

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

  // Sign up new anonymous account
  if (Firebase.signUp(&fbConfig, &fbAuth, "", "")) {
    Serial.println("Sign up successful");
    signupOK = true;
  } else {
    Serial.printf("%s\n", fbConfig.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task
   * from "addons/TokenHelper"*/
  fbConfig.token_status_callback = tokenStatusCallback;

  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);
}

/**
 * Runs once on initial start or when hardware reset.
 * Sets the deep sleep timer, connects to wifi and Firebase
 */
void setup() {
  Serial.begin(115200);

  esp_sleep_enable_timer_wakeup(MINUTE);

  setupWifi();
  setupFirebase();
}

/**
 * Main loop: Initiates deep sleep for the configured time, then wakes
 * and uploads the sensor value to Firebase Real Time Database
 */
void loop() {
  // Enter deep sleep
  esp_deep_sleep_start();

  // Read sensor and update in firebase if signed up
  if (Firebase.ready() && signupOK) {

    // Read the moisture sensor
    int moisture = analogRead(MOISTURE_SENSOR);

    // Build the data payload with a server timestamp
    char payloadBuffer[100];
    sprintf(payloadBuffer,
            "{\"timestamp\": {\".sv\": \"timestamp\"}, \"value\": %d}",
            moisture);

    // Upload to Firebase
    if (Firebase.RTDB.push(&fbData, "sensor_data", String(payloadBuffer))) {
      Serial.println("Data uploaded successfully");
    } else {
      Serial.println("Error uploading data");
      Serial.println(fbData.errorReason());
    }

    // Free the buffer from memory
    free(payloadBuffer);
  } else {
    Serial.println("Error with firebase and/or signup");
  }
}