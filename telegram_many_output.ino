//library
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;

//dht
#include "DHT.h"

//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//const char* ssid = "";
//const char* password = "";

#define BOTtoken ""  //from botfather

// Use @myidbot to find out the chat ID
#define CHAT_ID ""
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
WiFiClient client1;
UniversalTelegramBot bot(BOTtoken, client);
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

#define RelayBom 33 //1         36 39 33 35 27
#define RelayBom1 23
#define RelayDen 25//2
#define RelayQuat 26//3
#define RelayBonphan 27//4
#define Dongmai 18//5 IN1
#define Momai 19//6 IN2
#define DHTPIN 32
#define DHTTYPE DHT11
#define SOIL_MOIST_1_PIN 35
#define button 39
//#define button2 34
#define cambienmua 36
DHT dht(DHTPIN, DHTTYPE);
float humi; 
float temp;
float moist;

String RelayBomState = "HIGH";
String RelayDenState = "HIGH";
String RelayQuatState = "HIGH";
String RelayBonphanState = "HIGH";
String DongmaiState = "HIGH";
String MomaiState = "HIGH";

// ThingSpeak Settings
const int channelID = ; //
String writeAPIKey = ""; // write API key for your ThingSpeak Channel
const char* server = "";
const int postingInterval = 2 * 1000; // post data every 2 seconds

//FunctionDeclare
void    thingConnect();
void    readSensor(void);
float   getMoist();
void    printData(void);
void autoBom();
//telegram read sensor
String getReadings(){
  float temperature, humidity;
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  moist = getMoist();
  String message = "Nhiệt độ: " + String(temperature) + " ºC \n";
  message += "Độ ẩm: " + String (humidity) + " % \n";
  message += "Độ ẩm đất: " + String (moist) + " % \n";
  return message;
}
// Handle what happens when you receive new messages

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
//    text.toUpperCase();   
//    delay(10);

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Sử dụng các lệnh bên dưới để điều khiển.\n\n";
      welcome += "/batbom để bật bơm \n";
      welcome += "/tatbom để tắt bơm \n";
      welcome += "/batden để bật đèn \n";
      welcome += "/tatden để tắt đèn \n";
      welcome += "/batquat để bật quạt \n";
      welcome += "/tatquat để tắt quạt \n";
      welcome += "/batbonphan để bật bón phân \n";
      welcome += "/tatbonphan để tắt bón phân \n";
      welcome += "/dongmai để đóng mái che \n";
      welcome += "/momai để mở mái che \n";
      welcome += "/dung để dừng tất cả \n";
      welcome += "/docgiatri để đọc giá trị \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    
//    unsigned long time2 = 0;
//    if ( (unsigned long) (millis() - time2) > 20000)
//    {
//      if (moist > 2){
    if (text == "/batbom") {
      unsigned long time2 = 0;
      float moist = getMoist();
      bot.sendMessage(chat_id, "Bơm được bật", "");
      RelayBomState = "bật";
      //digitalWrite(RelayBom, LOW);
      while ( (unsigned long) (millis() - time2) > 10000)
    {
    
        // Thay đổi trạng thái đèn led
        if (digitalRead(RelayBom) == HIGH)
        {
            digitalWrite(RelayBom, LOW);
        } else {
            digitalWrite(RelayBom, LOW);
        }
        
        // cập nhật lại biến time
        time2 = millis();
    }
      }
     if (text == "/tatbom") {
      bot.sendMessage(chat_id, "Bơm đã tắt", "");
      RelayBomState = "tắt";
      digitalWrite(RelayBom, HIGH);
      
    }
      if (text == "/batden") {
      bot.sendMessage(chat_id, "Đèn được bật", "");
      RelayDenState =  "bật";
      digitalWrite(RelayDen, LOW);
   //   delay(15000);
    }
     if (text == "/tatden") {
      bot.sendMessage(chat_id, "Đèn đã tắt", "");
      RelayDenState ="tắt";
      digitalWrite(RelayDen, HIGH);
    }
      if (text == "/batquat") {
      bot.sendMessage(chat_id, "Quạt được bât", "");
      RelayQuatState =  "bật";
      digitalWrite(RelayQuat, LOW);
     // delay(15000);
      digitalWrite(RelayQuat, HIGH);
    }
     if (text == "/tatquat") {
      bot.sendMessage(chat_id, "Quạt đã tắt", "");
      RelayQuatState = "tắt";
      digitalWrite(RelayQuat, HIGH);
    }
      if (text == "/batbonphan") {
      bot.sendMessage(chat_id, "Bón phân được bật", "");
      RelayBonphanState =  "bật";
      digitalWrite(RelayBonphan, LOW);
    }
     if (text == "/tatbonphan") {
      bot.sendMessage(chat_id, "Bón phân đã tắt", "");
      RelayBonphanState = "tắt";
      digitalWrite(RelayBonphan, HIGH);
    }
      if (text == "/dongmai") {
      bot.sendMessage(chat_id, "Đóng mái che đang đóng", "");
      DongmaiState = "đóng";
      digitalWrite(Dongmai, HIGH);
      digitalWrite(Momai, LOW);
    }
     if (text == "/momai") {
      bot.sendMessage(chat_id, "Mở mái che được mở", "");
      MomaiState ="mở";
      digitalWrite(Momai, HIGH);
      digitalWrite(Dongmai, LOW);
    }
     if (text == "/dung") {
      bot.sendMessage(chat_id, "Dừng tất cả", "");
      MomaiState ="mở";
      digitalWrite(RelayBom, HIGH);
      digitalWrite(RelayDen, HIGH);
      digitalWrite(RelayQuat, HIGH);
      digitalWrite(RelayBonphan, HIGH);
      digitalWrite(Momai, LOW);
      digitalWrite(Dongmai, LOW);
    }
    if (text == "/docgiatri") {
      String readings = getReadings();
      bot.sendMessage(chat_id, readings, "");
    }  
///////////////////////////////////////////////////////////////////////////////////// ////////////////////////////////////

 if (text == "/trangthaibom") {
     
  bot.sendMessage(chat_id, "Bơm đã " + RelayBomState + "");
    } 
     if (text == "/trangthaiden") {
     
  bot.sendMessage(chat_id, "Đèn đã " + RelayDenState + "");
    } 
     if (text == "/trangthaiquat") {
     
  bot.sendMessage(chat_id, "Quạt đã " + RelayQuatState + "");
    } 
     if (text == "/trangthaibonphan") {
     
  bot.sendMessage(chat_id, "Bón phân đã " + RelayBonphanState + "");
    } 
//     if (text == "/trangthaidongmai") {
//     
//  bot.sendMessage(chat_id, "DONG MAI is " + DongmaiState + "");
//    } 
//     if (text == "/trangthaimomai") {
//     
//  bot.sendMessage(chat_id, "MO MAI is " + MomaiState + "");
//    } 
        
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
  }
} 

void setup() {
  Serial.begin(115200);
  dht.begin();
  #ifdef ESP8266
    client.setInsecure();
  #endif
  pinMode(button, INPUT);
//  pinMode(button2, INPUT);
  pinMode(RelayBom, OUTPUT);
  pinMode(RelayBom1, OUTPUT);
  pinMode(RelayDen, OUTPUT);
  pinMode(RelayQuat, OUTPUT);
  pinMode(RelayBonphan, OUTPUT);
  pinMode(Dongmai, OUTPUT);
  pinMode(Momai, OUTPUT);
  digitalWrite(RelayBom, HIGH);
  digitalWrite(RelayBom1, HIGH);
  digitalWrite(RelayDen, HIGH);
  digitalWrite(RelayQuat, HIGH);
  digitalWrite(RelayBonphan, HIGH);
  digitalWrite(Dongmai, LOW);
  digitalWrite(Momai, LOW);
  
  // Connect to Wi-Fi
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);

    if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
}
    //Nếu kết nối wifi thành công, in thông báo ra màn hình
    Serial.println("connected...yeey :)");
    lcd.begin(16, 2);
    lcd.init();
    lcd.backlight();
    lcd.print("Xin chao");
    lcd.setCursor(0, 1);
    lcd.print("Vuon rau tu dong");
    delay(5000);
    lcd.clear();
    lcd.print("IP Address");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());  
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  Serial.println(WiFi.localIP());
 // autoBom();
}

void loop() {

  readSensor();
//  delay(10000);
  printData();
  thingConnect();
  
 // autoQuat();
//  autoDen();
  // autoMai();
  abc();
//  ctht1();
  autoBom();
//ctht2();
 
 
}

//auto Mai che
//void autoMai(){
////TESTING
//float temperature;
//temperature = dht.readTemperature();
//int cambienmuaStatus = analogRead(cambienmua);
//Serial.print("Giá trị mưa: ");
//Serial.println(cambienmuaStatus);
//if (temperature <15 || temperature > 27 || cambienmuaStatus > 20){
//  digitalWrite(Dongmai, HIGH);
//  digitalWrite(Momai, LOW);
//  int buttonStatus = digitalRead(button);
//      Serial.println(buttonStatus);
//  if (buttonStatus == 0){
//    digitalWrite(Dongmai, LOW);
//    digitalWrite(Momai, LOW);
//      }
//}
////button -> button2, buttonStatus -> buttonStatus2
//if (temperature > 15 && temperature <27 || cambienmuaStatus < 20){
//  digitalWrite(Momai, HIGH);
//  digitalWrite(Dongmai, LOW);
//  int buttonStatus = digitalRead(button);
//  if (buttonStatus == 0){
//    digitalWrite(Dongmai, LOW);
//    digitalWrite(Momai, LOW);
//  }
//}
//}

//OK
//void autoQuat(){
//  float moist;
//  float humidity;
//  humidity = dht.readHumidity();
//  moist = getMoist();
// if ( humidity > 60 ){
//  digitalWrite(RelayQuat, LOW);
// }
// else {
//  digitalWrite(RelayQuat, HIGH);
// }
//  }
void autoBom(){
  float moist;
  moist = getMoist(); 
if (moist < 2){
  digitalWrite(RelayBom, LOW);
}
else{
   if ( (unsigned long) (millis() - time1) > 10000)
    {
    
        // Thay đổi trạng thái đèn led
        if (digitalRead(RelayBom) == LOW)
        {
            digitalWrite(RelayBom, HIGH);
        } else {
            digitalWrite(RelayBom, HIGH);
        }
        
        // cập nhật lại biến time
        time1 = millis();
    }
}
}

//void millis123(){
//  int numNewMessages;
//  for (int i=0; i<numNewMessages; i++) {
//    // Chat id of the requester
//    String chat_id = String(bot.messages[i].chat_id);
//    if (chat_id != CHAT_ID){
//      bot.sendMessage(chat_id, "Unauthorized user", "");
//      continue;
//    }
//  String text = bot.messages[i].text;
//  if (text == "/batbom") {
//      bot.sendMessage(chat_id, "Bơm được bật", "");
//      RelayBomState = "bật";
//      digitalWrite(RelayBom, LOW);
//        if ( (unsigned long) (millis() - time1) > 10000)
//    {
//        // Thay đổi trạng thái đèn led
////        if (digitalRead(RelayBom) == LOW)
////        {
//            digitalWrite(RelayBom, HIGH);
////        } else {
////            digitalWrite(RelayBom, HIGH);
//        }
//        // cập nhật lại biến time
//        time1 = millis();
//    }
//   }} 
//void thucong(){
//     int numNewMessages;
//  for (int i=0; i<numNewMessages; i++) {
//    // Chat id of the requester
//    String chat_id = String(bot.messages[i].chat_id);
//    if (chat_id != CHAT_ID){
//      bot.sendMessage(chat_id, "Unauthorized user", "");
//      continue;
//    }
//  String text = bot.messages[i].text;
//  if (text == "/batbom") {
//      bot.sendMessage(chat_id, "Bơm được bật", "");
//      RelayBomState = "bật";
//      digitalWrite(RelayBom, LOW);
//    //  delay(10000);
//   }
//}}
//void autoDen(){
//  int cambienmuaStatus = analogRead(cambienmua);
//  float temperature;
//  temperature = dht.readTemperature();
//  if (cambienmuaStatus > 20 && temperature > 23)
//  {
//    digitalWrite(RelayDen, LOW); 
//  }
//  if (temperature > 23 && temperature < 25){
//    digitalWrite(RelayDen, LOW);
//  }
//  if (temperature < 23){
//    digitalWrite(RelayDen, HIGH);
//  }
//}
//void ctht1(){
//    int buttonStatus = digitalRead(button);
//      Serial.println(buttonStatus);
//      if (buttonStatus == 0){
//        digitalWrite(Dongmai, LOW);
//        digitalWrite(Momai, LOW);
//      }
//}
//void ctht2(){
//  int buttonStatus2 = digitalRead(button2);
//     Serial.println(buttonStatus2);
//      if (buttonStatus2 == 0){
//        digitalWrite(Dongmai, LOW);
//        digitalWrite(Momai, LOW);
//      }
//}
void printData(void) {
  Serial.printf("Temp: %s°C - Hum: %s% - Moist: %s%\r\n", String(temp, 1).c_str(), String(humi, 1).c_str(), String(moist, 1).c_str());
}
void readSensor(void) {
  temp  = dht.readTemperature();
  humi  = dht.readHumidity();
  moist = getMoist();
}
void thingConnect() {
  if (client1.connect(server, 80)) {
    String body = "field1=" + String(temp, 1) + "&field2=" + String(humi, 1) + "&field3=" + String(moist, 1);
    client1.print("POST /update HTTP/1.1\n");
    client1.print("Host: api.thingspeak.com\n");
    client1.print("Connection: close\n");
    client1.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client1.print("Content-Type: application/x-www-form-urlencoded\n");
    client1.print("Content-Length: ");
    client1.print(body.length());
    client1.print("\n\n");
    client1.print(body);
    client1.print("\n\n");
  }
}
void abc(){
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
float getMoist(){
float i = 0;
float anaValue = 0;
  for (i = 0; i < 10; i++)  
  {
    anaValue += analogRead(SOIL_MOIST_1_PIN);
    delay(50);
  }
  anaValue = anaValue / (i);
  anaValue = map(anaValue, 0, 4095, 0, 100);
  anaValue = 100 - anaValue;
  return anaValue;
}
