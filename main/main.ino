#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define Relay1            D1

#define WLAN_SSID "kukuluc102"
#define WLAN_PASS "wifiplease"
#define LED_BUILTIN 2

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "ashayc"
#define AIO_KEY "a39bf47606ec4b01a45284238d878a2f"


WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Fan1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Fan");

void MQTT_connect();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Relay1,OUTPUT);
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
  mqtt.subscribe(&Fan1);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //Serial.println("Currently off");        // wait for a second

  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //Serial.println("Turned on");

  
  MQTT_connect();

  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Fan1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Fan1.lastread);
      int Light1_State = atoi((char *)Fan1.lastread);
      digitalWrite(Relay1, !(Light1_State));
      
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
