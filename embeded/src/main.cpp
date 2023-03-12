#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" // remember to add this file to the .gitignore

#define Fan 22
#define Light 23



void setup() {
  pinMode(Light, OUTPUT);
  pinMode(Fan, OUTPUT);
  
  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    
    // Specify content-type header
   

      http.addHeader("Content-Type", "application/json");
      
      

      // Serialise JSON object into a string to be sent to the API
      StaticJsonDocument<1024> doc;
      String httpRequestData;

      float temp = random(21.0,33.0);


      doc["temperature"] = temp;

      serializeJson(doc, httpRequestData);


      // Send HTTP PUT request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check reuslt of POST request. negative response code means server wasn't reached
     
   
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      http_response = http.getString();
      Serial.println(http_response);
      
      // Free resources
      http.end();






    http.begin(endpoint2);
    

    int httpResponseCode = http.GET();

    if(httpResponseCode>0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("Response from server: ");
      http_response = http.getString();
      

    }
    else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();

    // Stream& input;

    StaticJsonDocument<192> doc;

    DeserializationError error = deserializeJson(doc, http_response);

    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
    }

    const char* id = doc["_id"]; // "63fa370b77ef4b58d465b0bd"
    const char* user = doc["user"]; // "Kingsley#9729"
    bool light_switch_1 = doc["light_switch_1"]; // false
    bool light_switch_2 = doc["light_switch_2"]; // true
    bool light_switch_3 = doc["light_switch_3"]; // true

    Serial.println("");

    Serial.print("light_switch_1: ");
    Serial.println(light_switch_1);

    Serial.print("light_switch_2: ");
    Serial.println(light_switch_2);

    Serial.print("light_switch_3: ");
    Serial.println(light_switch_3);

    Serial.println("");

    //switch 1
    if (Light==0)
    {
      digitalWrite(Light,LOW);
    }
    else{
      digitalWrite(Light,HIGH);
    }

    //switch 2
    if (Fan==0)
    {
      digitalWrite(Fan,LOW);
    }
    else{
      digitalWrite(Fan,HIGH);
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}