# HomeAutomation

This is an Arduino based project- my first foray into IOT. The aim is to create a fully functional and viable solution to control all household appliances using the Google Assistant. 

**Hardware**

* NodeMCU v3 CH340S (https://www.aliexpress.com/item/Wireless-module-NodeMcu-v3-v2-ESP32-D1MINI-Lua-WIFI-development-board-ESP8266-with-pcb-Antenna-and/32656775273.html?spm=a2g0s.9042311.0.0.96e74c4dsPFrtl)
* 4 Channel Relay Octocoupler (https://www.aliexpress.com/item/32905958885.html?spm=a2g0s.9042311.0.0.2eca4c4dfOnvs3)
* AC-DC 5V 700mA 3.5W Precision Buck Converter Transformer module (https://www.aliexpress.com/item/32677330307.html?spm=a2g0s.9042311.0.0.96e74c4dsPFrtl)
* 5V-3.3V Voltage Regulator (https://www.aliexpress.com/snapshot/0.html?spm=a2g0s.9042311.0.0.96e74c4dsPFrtl&orderId=101806902870217&productId=1996291344)

**Software Implementation**
The project uses IFTTT and AdaFruit in order to interface between the Arduino and the Google Assistant. The WiFi Details and the AdaFruit Keys are redacted for obvious purposes.
While it seems straightforward, one of the guiding principles behind this project was to make sure it's viable in any household. This meant that powering the devices with a powerbank or a phone charger wasn't an option, since the entire apparatus has to be concealed behind the switches and thus, must be compact. For this purpose, I am experimenting with step down converters and voltage regulators in order to power the Arduino and the relays. 
