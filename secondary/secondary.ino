
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4

#define WLAN_SSID "kukuluc102"
#define WLAN_PASS "wifiplease"
#define LED_BUILTIN 2

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "ashayc"
#define AIO_KEY "a39bf47606ec4b01a45284238d878a2f"


WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Underglow = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Underglow");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Light2");
Adafruit_MQTT_Subscribe YellowLight2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/YellowLight2");
Adafruit_MQTT_Subscribe Overhead = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Overhead");

void MQTT_connect();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(WLAN_SSID);

  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi Connected");
  Serial.print("IP Address");
  Serial.print(WiFi.localIP());
  ArduinoOTA.onStart([](){
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([](){
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %u%%\r",(progress/(total/100)));
  });
  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  mqtt.subscribe(&Underglow);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&YellowLight2);
  mqtt.subscribe(&Overhead);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //Serial.println("Currently off");        // wait for a second

  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //Serial.println("Turned on");

  ArduinoOTA.handle();
  
  MQTT_connect();

  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Underglow) {
      Serial.print(F("Underglow: "));
      Serial.println((char *)Underglow.lastread);
      int Underglow_State = atoi((char *)Underglow.lastread);
      digitalWrite(Relay1, !(Underglow_State));
      
    }
    if (subscription == &Light2){
      Serial.print(F("Light2: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Relay2, !(Light2_State));
    }
    if (subscription == &YellowLight2){
      Serial.print(F("Yellow Light: "));
      Serial.println((char *)YellowLight2.lastread);
      int YellowLight2_State = atoi((char *)YellowLight2.lastread);
      digitalWrite(Relay3, !(YellowLight2_State));
    }
    if (subscription == &Overhead){
      Serial.print(F("Blue Light: "));
      Serial.println((char *)Overhead.lastread);
      int Overhead_State = atoi((char *)Overhead.lastread);
      digitalWrite(Relay4, !(Overhead_State));
    }
  }
}

void MQTT_connect(){
  int8_t ret;
  if(mqtt.connected()){
    return;
  }

  Serial.println("Connecting to MQTT");

  uint8_t retries=0;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries++;
    Serial.print("retrying counter: ");
    Serial.println(retries);
  }
  Serial.println("MQTT Connected!");
  
}
