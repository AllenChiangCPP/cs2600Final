#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SDA 13
#define SCL 14

// WiFi
const char *ssid = "@_@ (2)"; // Enter your WiFi name
const char *password = "BudgetedBad";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "testConnect";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

//lcd screen, will display adata for the tic tac toe game such as what boxes available
WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27,16,2);

//add button jostick, cycles through numbers 0-9 for picking boxes in the tic tac toe game and picking options
int xyzPins[] = {33,35,34}; //xyz pins

int joystickInput = 0; //int that will be changed throughout the program. For publishing later. 

void setup() {
 //setup lcd screen
 Wire.begin(SDA, SCL);
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("START? (y=1/n=2)");
 //setup joystick
 pinMode(xyzPins[2], INPUT_PULLUP);
 // Set software serial baud to 115200;
 Serial.begin(115200);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish(topic, "Hi EMQX I'm ESP32");
 client.subscribe(topic);


}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() 
{
  client.loop();
  int xVal = analogRead(xyzPins[0]);
  int yVal = analogRead(xyzPins[1]);
  int zVal = digitalRead(xyzPins[2]);
  Serial.printf("X,Y,Z: %d,\t%d,\t%d\n", xVal, yVal, zVal);
  lcd.setCursor(0,1);
  lcd.print("Input: ");
  lcd.print(joystickInput);
  delay(250);  
  //92-113, use joystick to pick numbers between 0-8, makes sure joystock counter is between 0-8, all possible values for the board
  if (xVal == 0) 
  {
    if (joystickInput <= 0)
    {
      joystickInput = 8;
    }
    else
    {
      joystickInput--;
    }
  }
  if (xVal == 4095) 
  {
    if (joystickInput >= 8)
    {
      joystickInput = 0;
    }
    else
    {
      joystickInput++;
    }
  }
  //if joystick is pointing upward, publish joystickInput int to mqttx
  if (yVal == 0) 
  {
    char pub_out[1];
    sprintf(pub_out,"%d",joystickInput);
    client.publish("testConnect", pub_out);
  }
}