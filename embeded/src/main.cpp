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
  

  float temp = random(21.0,33.0);
    // Establish a connection to the server
    http.begin(endpoint);
    
    // Specify content-type header
   

      http.addHeader("Content-Type", "application/json");
      http.addHeader("Content-length", "23");
      

      // Serialise JSON object into a string to be sent to the API
      StaticJsonDocument<1024> doc;
      String httpRequestData;

      


      doc["temperature"] =temp;

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






    http.begin(endpoint);
    

    int httpResponseCode2 = http.GET();

    if(httpResponseCode2>0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode2);

      Serial.print("Response from server: ");
      http_response = http.getString();
      

    }
    else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode2);
    }

    

    // Stream& input;

   

    StaticJsonDocument<192> doc2;

    DeserializationError error = deserializeJson(doc2, http_response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* id = doc["_id"]; // "640e3d68bf11b4131596c840"
    float temperature = doc["temperature"]; // 3.2
    bool light = doc["light"]; // true
    bool fan = doc["fan"]; // false
    const char* poke = doc["poke"]; // "mon"

    Serial.println("");

    Serial.print("Fan: ");
    Serial.println(fan);

    Serial.print("Light: ");
    Serial.println(light);

    Serial.println("");

    //switch 1
      digitalWrite(Light,light);

      digitalWrite(Fan,fan);

    http.end();
    
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}