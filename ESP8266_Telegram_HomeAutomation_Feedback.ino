//Viral Science www.youtube.com/c/viralscience  www.viralsciencecreativity.com
// ESP8266 Home Automation controlled via Telegram application with Feedback


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define BOTtoken "5005939601:AAEooIdidd-1NkdIqYBPqPOMDyGtbcMpbT0"  // Bot token in the xxxxx format: xxxxxxxxxxxxxxx, start a conversation with the bot "BotFather" in Telegram and,
//           #define CHAT_ID "1959776138"                                 // follow the instructions to create a new bot. You will be given a secret token. Do not tell it to anyone.
char ssid[] = "";         // Wifi SSID 
char password[] = "";     // Wifi Password
String buttons[] = {"Bơm", "Đèn"};  // names of connected devices, DOES NOT WORK with Cyrillic!
int pin[] = {33, 25};                      // pin number to which the executing device is connected (relay, transistor, etc.)

bool protection = 1;                      //access rights: 0 - available to all users, 1 - access by Chat ID, if it is included in chatID_acces.
int chatID_acces[] = {1959776138}; // Chat IDs that are allowed access are ignored if protection = 0.
                             // Note: by the command / start in Telegram, if the user does not have permission to control the device, the bot will issue a Chat ID
                             // this user so that it can be added to chatID_acces. Chat ID is a unique chat number that is issued
                             // the user who started communicating with the bot.

String on_symbol="✅ ";  // ON Indicator
String off_symbol="☑ "; // OFF indicator.


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int quantity;
int Bot_mtbs = 3000;
long Bot_lasttime;   
bool Start = false;
const int ledPin = 2;
int ledStatus = 0;
String keyboardJson = "";

int messageID;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));


  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String m_id = String(bot.messages[i].message_id);
    if (bot.messages[i].type == "callback_query") {
      String statusMessage;
      for (int i1=0; i1<quantity; i1++) {
       if (bot.messages[i].text == buttons[i1]) {
       digitalWrite (pin[i1], !digitalRead(pin[i1]));
       }
      digitalRead(pin[i1])?statusMessage+=on_symbol:statusMessage+=off_symbol;
      statusMessage+=buttons[i1]; 
      statusMessage+='\n';
      }
      bot.deleteMessage(bot.messages[i].chat_id, bot.messages[i].message_id);
      bot.sendMessageWithInlineKeyboard(bot.messages[i].chat_id, statusMessage, "", keyboardJson);
      
    } else {
    String text = bot.messages[i].text;
    Serial.println(m_id);
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
    int i2=0;
    do{
    if (!protection || String(chatID_acces[i2]) == chat_id) {
      if (text == "/switch") {
      String statusMessage;
      for (int i=0; i<quantity; i++) {
      digitalRead(pin[i])?statusMessage+=on_symbol:statusMessage+=off_symbol;
      statusMessage+=buttons[i];  
      statusMessage+='\n';
      }
      bot.sendMessageWithInlineKeyboard(chat_id, statusMessage, "", keyboardJson);
      }

      if (text == "/start") {
      String welcome = "Welcome " + from_name + ".\n";
      welcome += "This is ESP8266 smart switch, controlled via Telegram.\n\n";
      welcome += "/switch : Click to open Switch Board.\n";
      String keyboardStart = "[[{ \"text\" : \"YouTube Channel\", \"url\" : \"https://www.youtube.com/c/viralscience\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, welcome, "", keyboardStart);
      }
      break;
    }else{
      if (i2==((sizeof(chatID_acces)/sizeof(int))-1)&&text == "/start"||(sizeof(chatID_acces)/sizeof(int))==0&&text == "/start") {
      bot.sendMessage(chat_id, "No access, Chat ID: "+chat_id, "");
      }
    }

    i2++;
    }while (i2<(sizeof(chatID_acces)/sizeof(int)));
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  quantity=sizeof(pin)/sizeof(int);
  for (int i=0; i<quantity; i++) {
  pinMode(pin[i], OUTPUT);
      }

  for (int i=0; i<quantity; i++) {
  if(i==0) keyboardJson += "[";
  keyboardJson += "[{ \"text\" : \"";
  keyboardJson += buttons[i];
  keyboardJson += "\", \"callback_data\" : \"";
  keyboardJson += buttons[i];
  keyboardJson += "\" }]";
  if(i==quantity-1){
    keyboardJson += "]";
    }else{
  keyboardJson += ",";  
  }
  }
  delay(10);
  client.setInsecure();
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
}
