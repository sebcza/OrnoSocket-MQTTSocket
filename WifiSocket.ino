
#include "orgb417pb.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

uint8_t GPIO_Pin = D2;

const char* ssid     = "iPhone (Sebastian)";
const char* password = "komputer1";

const char* mqttServer = "mqtt.hackcube.org";
const int mqttPort = 11883;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

ORGB417PB transciver(digitalPinToInterrupt(GPIO_Pin), D0);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char raw_message[30];
  for (int i = 0; i < length; i++) {
    raw_message[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }

  long ornoCode = atol(raw_message);
  Serial.println(ornoCode);
  transciver.send(ornoCode, 4);

}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
  Serial.println("Connected to the WiFi network");
   
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  connectToMqtt();

}

void connectToMqtt(){
    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
      client.subscribe("gniazdka/salon");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}



void loop() {
  if(!client.connected()){
    connectToMqtt();
  }  
  client.loop();
  delay(1000);
}

