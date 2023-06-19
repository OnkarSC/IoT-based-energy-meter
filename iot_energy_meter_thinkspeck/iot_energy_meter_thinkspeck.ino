#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20,4);

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"      // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int a = 0;  int j = 0; int g = 0; int l = 0; float bill = 0; float un =0 ;
String myStatus = "";

void setup() {
  Serial.begin(115200); 
    lcd.begin();
    lcd.print("  IoT Energy Meter");
    delay(2000);
    lcd.clear();
    lcd.print("Connecting to ");
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    lcd.setCursor(10,1);
    lcd.print(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(10000);     
    } 
    Serial.println("\nConnected.");
    lcd.setCursor(0,2);
    lcd.print("     Connected.");
    lcd.clear();
  }
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  
 a = analogRead(A0);        // pulse read from energy meter

 Serial.println(a);
 if(a > 200){
  j++;
 
 }
//  lcd.clear();
  data();

  g++;
//  Serial.print(g);
  if(g == 12000)
  {
    upload();
  }
//  delay(1000); // Wait 20 seconds to update the channel again
}
void upload(){
  g = 0;
//    lcd.clear();
    lcd.setCursor(0,3);
    lcd.print("Uploading successful...");
    ThingSpeak.setField(1, "228");
    ThingSpeak.setField(2, un);
    ThingSpeak.setField(3, bill);
    ThingSpeak.setField(4, l);
    ThingSpeak.setField(5, j);
    lcd.clear();
  ThingSpeak.setStatus(myStatus);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
    delay(1000);
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
void data(){
  lcd.setCursor(0,0);
  lcd.print("Cal :");
//  lcd.setCursor(5,0);
  lcd.print(j);
  lcd.setCursor(0,1);
   lcd.print("Unit:");
//  lcd.setCursor(5,1);
  un=j/3200.0;
  lcd.print(un,5);
  lcd.print(" kWH");
  lcd.setCursor(0,2);
  lcd.print("Bill:");
//  lcd.setCursor(5,2);
  bill = un*8;
  lcd.print(bill,5);
  lcd.print(" Rs");
}
