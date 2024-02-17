#include <SoftwareSerial.h>  //Software Serial library
#include <Arduino.h>
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "Raptor";       // WiFi SSID
String myPWD = "9921763244"; // WiFi Password
String myAPI = "NKWLHQ8KEN2DTPYV";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 

int IN1 = 5;//relay
int Pin1 = A0;
float sensor1Value = 0;
void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(Pin1, INPUT);
  digitalWrite(IN1, HIGH);
  delay(100);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
  delay(1000);
  
}

  void loop()
  {
   
   Serial.print("Plant 1 - Moisture Level:");
   sensor1Value = analogRead(Pin1);
   Serial.println(sensor1Value);

   if (sensor1Value > 450) {
       digitalWrite(IN1, LOW);
       delay(5000);
       digitalWrite(IN1, HIGH);
   } else {
       digitalWrite(IN1, HIGH);
   }
   
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sensor1Value);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sensor1Value);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(20000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
  
