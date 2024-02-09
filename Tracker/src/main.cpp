#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

#define WIFI_SSID "***REMOVED***"
#define WIFI_PASS "***REMOVED***" // Read from .env or something to omit from code

#define FIREBASE_KEY "***REMOVED***"
#define FIREBASE_DB_URL "***REMOVED***"

FirebaseData fbDataDTO;
FirebaseAuth fbAuth;
FirebaseConfig fbConfig;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setupWifi();
void setupFirebase();

void setup() {
  Serial.begin(115200);

  setupWifi();
  setupFirebase();
}

void loop() {
  // TODO: Login or Keep auth instead of signing up everytime
  if (Firebase.ready() && signupOK &&
      (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbDataDTO, "test/int", count)) {
      Serial.printf("PASSED\n");
      Serial.printf("PATH: %s\n", fbDataDTO.dataPath());
      Serial.printf("TYPE: %s\n", fbDataDTO.dataType());
    } else {
      Serial.printf("FAILED\n");
      Serial.printf("REASON: %s\n", fbDataDTO.errorReason());
    }
    count++;

    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbDataDTO, "test/float",
                               0.01 + random(0, 100))) {
      Serial.printf("PASSED\n");
      Serial.printf("PATH: %s\n", fbDataDTO.dataPath());
      Serial.printf("TYPE: %s\n", fbDataDTO.dataType());
    } else {
      Serial.printf("FAILED\n");
      Serial.printf("REASON: %s\n", fbDataDTO.errorReason());
    }
  }
}

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