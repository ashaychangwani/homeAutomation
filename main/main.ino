#include <ESP8266WiFi.h>
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

Adafruit_MQTT_Subscribe Fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Fan");
Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Light");
Adafruit_MQTT_Subscribe YellowLight = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/YellowLight");
Adafruit_MQTT_Subscribe BlueLight = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/BlueLight");

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
  mqtt.subscribe(&Fan);
  mqtt.subscribe(&Light);
  mqtt.subscribe(&YellowLight);
  mqtt.subscribe(&BlueLight);

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
    if (subscription == &Fan) {
      Serial.print(F("Fan: "));
      Serial.println((char *)Fan.lastread);
      int Fan_State = atoi((char *)Fan.lastread);
      digitalWrite(Relay1, !(Fan_State));
      
    }
    if (subscription == &Light){
      Serial.print(F("Light: "));
      Serial.println((char *)Light.lastread);
      int Light_State = atoi((char *)Light.lastread);
      digitalWrite(Relay2, !(Light_State));
    }
    if (subscription == &YellowLight){
      Serial.print(F("Yellow Light: "));
      Serial.println((char *)YellowLight.lastread);
      int YellowLight_State = atoi((char *)YellowLight.lastread);
      digitalWrite(Relay3, !(YellowLight_State));
    }
    if (subscription == &BlueLight){
      Serial.print(F("Blue Light: "));
      Serial.println((char *)BlueLight.lastread);
      int BlueLight_State = atoi((char *)BlueLight.lastread);
      digitalWrite(Relay4, !(BlueLight_State));
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
