/*************************************************************/
/*
/*   Hotspot is goldie
/*
/*   pasward is    gramgram
/*
/*   website is    goldieiii.local
/*
/*************************************************************/
// Load Wi-Fi library
#include <ESPmDNS.h>
#include <WiFi.h>

const char* YOUR_NEW_HOSTNAME = "GOLDIE_iii";
// Replace with your network credentials
const char* ssid     = "goldie";
const char* password = "gramgram";

IPAddress staticIP(192, 168, 1, 59);

IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress dns(192, 168, 1, 1);

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
int count = 0;
bool ledState = false;

// Auxiliar variables to store the current output state
String frontSpkrState = "on";
String towerSpkrState = "on";
String backSpkrState = "on";
String bassSpkrState = "on";

// Assign output variables to GPIO pins
/*const int outputFront = 13;
const int outputTower = 26;
const int outputBack = 12;
const int outputBass = 33;*/
const int flashingLed = 19;

const int frontL = 18;
const int frontR = 17;

const int towerL = 16;
const int towerR = 15;

const int backR = 21;
const int backL = 23;

const int sub = 22;


void setup() {
   Serial.begin(9600);
  // Initialize the output variables as outputs
  /*pinMode(outputFront, OUTPUT);
  pinMode(outputTower, OUTPUT);
  pinMode(outputBack, OUTPUT);
  pinMode(outputBass, OUTPUT);*/
  pinMode(flashingLed, OUTPUT);
  pinMode(frontL, OUTPUT);
  pinMode(frontR, OUTPUT);
  pinMode(towerL, OUTPUT);
  pinMode(towerR, OUTPUT);
  pinMode(backL, OUTPUT);
  pinMode(backR, OUTPUT);
  pinMode(sub, OUTPUT);



  // Set outputs to LOW
  digitalWrite(frontL, HIGH);
  digitalWrite(frontR, HIGH);
  digitalWrite(towerL, HIGH);
  digitalWrite(towerL, HIGH);
  digitalWrite(backL, HIGH);
  digitalWrite(backR, HIGH);
  digitalWrite(sub, HIGH);

digitalWrite(flashingLed, LOW);

  WiFi.setAutoConnect(false);
 

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  WiFi.setHostname(YOUR_NEW_HOSTNAME);
 //WiFi.config(ip, dns, gateway, subnet);
  //IPAddress IP = WiFi.softAPIP();
   if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
   Serial.println("Configuration failed.");
 }
    if(!MDNS.begin("goldieIII")) {
     Serial.println("Error starting mDNS");
     return;
  }
  Serial.print("AP IP address: ");
  Serial.println(staticIP);

  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  count++;
  if(count == 10000){
    if(ledState == false){
      digitalWrite(flashingLed, HIGH);
      ledState = true;
    }else{
      digitalWrite(flashingLed, LOW);
      ledState = false;
    }
    count = 0;
  }

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) { 
   if(count == 100000){
    if(ledState == false){
      digitalWrite(flashingLed, HIGH);
      ledState = true;
    }else{
      digitalWrite(flashingLed, LOW);
      ledState = false;
    }
    count = 0;
  }      
  
                // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /front/off") >= 0) {
              Serial.println("Front off");
              frontSpkrState = "off";
              digitalWrite(frontL, HIGH);
              digitalWrite(frontR, HIGH);
              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("LOW");
            } else if (header.indexOf("GET /front/on") >= 0) {
              Serial.println("Front on");
              frontSpkrState = "on";
              digitalWrite(frontL, LOW);
              digitalWrite(frontR, LOW);
              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("HIGH");
            } else if (header.indexOf("GET /tower/on") >= 0) {
              Serial.println("Tower on");
              towerSpkrState = "on";
              digitalWrite(towerL, LOW);

              digitalWrite(towerR, LOW); 
                           Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("HIGH");
             } else if (header.indexOf("GET /tower/off") >= 0) {
              Serial.println("Tower off");
              towerSpkrState = "off";
              digitalWrite(towerL, HIGH);
              digitalWrite(towerR, HIGH);
              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("LOW");
             }else if (header.indexOf("GET /back/on") >= 0) {
              Serial.println("Back on");
              backSpkrState = "on";
              digitalWrite(backL, LOW);
              digitalWrite(backR, LOW);
              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("HIGH");
             } else if (header.indexOf("GET /back/off") >= 0) {
              Serial.println("Back off");
              backSpkrState = "off";
              digitalWrite(backL, HIGH);
              digitalWrite(backR, HIGH);              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("LOW");
             }else if (header.indexOf("GET /bass/on") >= 0) {
              Serial.println("Bass on");
              bassSpkrState = "on";
              digitalWrite(sub, LOW);
              Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("HIGH");
             } else if (header.indexOf("GET /bass/off") >= 0) {
              Serial.println("Bass off");
              bassSpkrState = "off";
              digitalWrite(sub, HIGH);
               Serial.println("the Pin is !!!!!!!!!!!!!!!!!!");
              Serial.println("LOW");
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #1ffa07; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #fa1b07;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1 style='color: gold'>GOLDIE III</h1>");
            
            // Display current state, and ON/OFF buttons for Front  
            client.println("<p>Front - Speaker </p>");
            // If the frontSpkrState is off, it displays the ON button       
            if (frontSpkrState=="off") {
              client.println("<p><a href=\"/front/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/front/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Tower  
            client.println("<p>Tower - Speaker </p>");
            // If the towerSpkrState is off, it displays the ON button       
            if (towerSpkrState=="off") {
              client.println("<p><a href=\"/tower/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/tower/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // Display current state, and ON/OFF buttons for Back  
            client.println("<p>Back - Speaker </p>");
            // If the towerSpkrState is off, it displays the ON button       
            if (backSpkrState=="off") {
              client.println("<p><a href=\"/back/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/back/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Bass  
            client.println("<p>Bass - Speaker </p>");
            // If the towerSpkrState is off, it displays the ON button       
            if (bassSpkrState=="off") {
              client.println("<p><a href=\"/bass/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/bass/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
         
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
