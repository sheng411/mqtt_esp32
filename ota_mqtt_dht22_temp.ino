#include <WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#define DHTPIN 32                 //開發板腳位
#define DHTTYPE DHT22             //Sensors used(using DHT22)
#define MQTT_USER "name"           //MQTT username
#define MQTT_PASS "pwd"  //MQTT password
#define OTA_id "ESP32 MQTT_DHT22" //ESP32 OTA connect id
#define OTA_pwd "ESP-32"          //ESP32 OTA connect password


//Router settings
const char* ssid = "your ssid";   //  your network SSID (name)
const char* pwd = "your password";//  your network Password (pwd)

//MQTT server
char server[] = "ip or domain name"; //MQTT server(ip or domain name)
int port = 18000;                    //MQTT server port

//MQTT settings
char client_id[] = "your id";                               //MQTT client id
#define TOPIC_INFO  "isu11003058/school/info"               //test
#define TOPIC_TEMP  "isu11003058/school/sensor/temp"        //temperature
#define TOPIC_HUM   "isu11003058/school/sensor/hum"         //humidity
#define TOPIC_LED_CONTROL "isu11003058/school/control/led1" //LED
#define ELED_PIN 27 //Error LED
#define LED_PIN 33  //generaiiy LED
//LED_BUILTIN

DHT dht(DHTPIN, DHTTYPE);

// Clients for MQTT
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// LED Control info
typedef enum {
  LED_OFF = 0,
  LED_ON,
  LED_FLASH
} LEDStatus;
LEDStatus led_status = LED_OFF;

// Timer info
#define TEMP_PERIOD 5000
#define LED_FLASH_PERIOD 200
unsigned long temp_last_time, led_last_time;

void led_on(){
  digitalWrite(LED_PIN, HIGH);
}

void led_off(){
  digitalWrite(LED_PIN, LOW);
}

void led_toggle() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

//Error LED
void eled_on(){
  digitalWrite(ELED_PIN, HIGH);
}

void eled_off(){
  digitalWrite(ELED_PIN, LOW);
}

void eled_toggle() {
  digitalWrite(ELED_PIN, !digitalRead(ELED_PIN));
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Output incoming message to serial terminal
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // If LED Control command is incoming, change LED status
  if(!strcmp(topic, TOPIC_LED_CONTROL)) {
    switch (payload[0]) {
      case '0':
        led_status = LED_OFF;
        break;
      case '1':
        led_status = LED_ON;
        break;
      case '2':
        led_status = LED_FLASH;
        break;
      default: {}
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  /*if(WiFi.status() != WL_CONNECTED) { //reconnect
      eled_toggle();
  }*/
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC_INFO,"sensor node ready ...");
      // ... and resubscribe
      client.subscribe(TOPIC_LED_CONTROL);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  // setup LED/Button pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(ELED_PIN, OUTPUT);
  eled_on();
  led_on();
  delay(500);
  Serial.begin(115200);
  client.setServer(server, port);
  client.setCallback(callback);
  dht.begin();
  led_off();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {     //reconnect
    delay(500);
    eled_toggle();
    Serial.print(".");
  }

  timeClient.begin();
  ntp();
  
  Serial.println("WiFi connected !!");
  printWifiStatus();
  led_last_time = millis();
  temp_last_time = millis();
  Serial.println(WiFi.localIP());
  ArduinoOTA.setHostname(OTA_id);
  ArduinoOTA.setPassword(OTA_pwd);
  ArduinoOTA.begin();
  /*
  char cntp[1]={'1'};
  client.publish(TOPIC_NTP,cntp);
  */
}

void loop() {
  ArduinoOTA.handle();
  unsigned long current_time;
  
  /*
  //check restart
  char cntp[1]={'0'};
  if (client.subscribe(TOPIC_NTP)==cntp) {
    ESP.restart();
  }
  */
  // Check MQTT broker connection status
  // If it is disconnected, reconnect to the broker
  if (!client.connected()) {
    reconnect();
  }

  // Get temperature & humidity and publish them
  current_time = millis();
  if( TEMP_PERIOD < (current_time - temp_last_time) ) {
    // Read Humidity & Temperature (Celsius)
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // Output to serial terminal
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
    // pubilsh to MQTT broker
    char buf[10];
    sprintf(buf,"%s", String((float)t, 2).c_str());
    client.publish(TOPIC_TEMP, buf);
    sprintf(buf,"%s", String((float)h, 2).c_str());
    client.publish(TOPIC_HUM, buf);
    // update last time value
    temp_last_time = current_time;
  }
  
  // Control LED according to led_status
  switch (led_status) {
    case LED_OFF:
      led_off();
      break;
    case LED_ON:
      led_on();
      break;
    case LED_FLASH:
      current_time = millis();
      if (LED_FLASH_PERIOD < (current_time - led_last_time)) {
        led_toggle();
        led_last_time = current_time;
      }
      break;
    default: {}
  }
  
  // Keep MQTT process on going
  client.loop();
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    eled_off();
}
