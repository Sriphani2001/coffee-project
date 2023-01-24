#include <SPI.h>
#include <WiFi101.h>
//#include<TFT.h>

char ssid[] = "Prajna";        // your network SSID (name)
char pass[] = "123456789";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
float lightLevel;
int tempPin = A1;
const int relaySignal =8;
const float currentval = A3;
const int buttonPin =4;
float pwr =0;
int state =0;
int timer=0;
int seconds ;
int minutes ;
int hours ;
float sensorValue;
float temp;
float tempC;
float result;
float currentSens;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  pinMode(tempPin, INPUT);
  pinMode(buttonPin , INPUT ) ;
  pinMode(currentval , INPUT ) ;
  pinMode(relaySignal , OUTPUT ) ;

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWiFiStatus();
}

void loop() {



  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      printMacAddress(remoteMac);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c); 


   if (currentLine.endsWith("GET /ON")) {
          digitalWrite(relaySignal , HIGH);

        }
   if (currentLine.endsWith("GET /ON")) {
        state=1;
        if(state=1){
          timer=timer+1;
                    }
        }

    if (currentLine.endsWith("GET /OFF")) {
          digitalWrite(relaySignal , LOW);
       }

    if (currentLine.endsWith("GET /OFF")) {
          state=0;
          timer=0;
        }
    if (currentLine.endsWith("GET /ON")) {
              state=1;
              if(state=1){
              result = analogRead(currentSens/1024.00*3.3);
                    }
                    }
     if (currentLine.endsWith("GET /OFF")) {
              result = 0;
            }



        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Refresh: 1");
            client.println();
            client.println("<!DOCTYPE HTML>");
            //client.println("<meta name="viewport" content="width=device-width", initial-scale=1.0">);

            client.println("<html>");
            
            // the content of the HTTP response follows the header:

            client.println("<style>");
            
            client.println("body {background-color: #e68a00;}");
            client.println("h1 {background-color: #b36b00;}");
            client.println("linkA {text-align: center;}");
            
            client.println("h1 {text-align: center;}");
            client.println("h2 {text-align: center;}");
            client.println("h3 {text-align: center;}");
            client.println("h3 {text-align: center;}"); 
            
            client.println("</style>");
            
            client.print("<body><h1>Smart Coffee Maker</h1>"); //To create title on new page
            client.print("<h2>Coffee Maker <a href=\"ON\">ON</a></h2>");
           
            client.print("<h2>Coffee Maker <a href=\"OFF\">OFF</a></h2>");
            

            client.println("-->The coffee maker has been ON for :   ");
            server.print(timer);
            client.println("Seconds");
            client.print("<br>");
            
            client.println("-->The power consumed by the coffee maker is:   ");
            server.print(result);
            client.print(" W");
            client.print("<br>"); 

            // Photo Transistor
            client.print("-->The position of coffee pot: ");
            int readlight = A2;
            lightLevel = analogRead(readlight);
              if(lightLevel<15){
              
              client.print("Coffee pot available");
               }
              else{
                  
                  client.print("Coffee pot is taken");
                    }
            client.print("<br>"); 
// temperature
            sensorValue = analogRead(A1);
            temp = (sensorValue/1024) * 5 ;
            temp = (temp - 0.5) * 100;
            temp = temp - 32;
            tempC = temp * 5/9;
            client.print("-->The Temperature of the coffee maker: ");
            server.print(tempC);
            client.print("C");
            client.println("<br />");
            // The HTTP response ends with another blank line:
            client.println();

            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    
          currentLine += c;      
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}