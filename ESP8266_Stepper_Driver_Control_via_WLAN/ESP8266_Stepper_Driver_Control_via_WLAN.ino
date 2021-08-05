//----------------------------------------Include the NodeMCU ESP8266 Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AccelStepper.h>

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router


//----------------------------------------SSID and Password of your WiFi router
const char *ssid = "Tenda_0B8E30";
const char *password = "t1nd2wifi2.4";
//----------------------------------------

int pos;

AccelStepper stepper(AccelStepper :: HALF4WIRE, 14, 12, 13, 15);

ESP8266WebServer server(80);  //--> Server on port 80

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//----------------------------------------

//----------------------------------------Procedure for handling servo control
void handleStepper(){
  String POS = server.arg("stepperPOS");
  int p = POS.toInt();
  pos = map(p, 0, 360, 0, 360*4);
  stepper.moveTo(pos);
  //Serial.print("Stepper Position:");
  //Serial.println(pos);
  server.send(200, "text/plane","");
}
//----------------------------------------
//----------------------------------------Setup----------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board

  stepper.setMaxSpeed(800);
  stepper.setAcceleration(2000);
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------

  //----------------------------------------Initialize Webserver
  server.on("/",handleRoot);  //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS",handleStepper); //--> Sets servo position from Web request
  server.begin();  
  Serial.println("HTTP server started");
}
//------------------------------------------------------------------------------------
//----------------------------------------Loop----------------------------------------
void loop() {
 server.handleClient();

 stepper.run();
}
//------------------------------------------------------------------------------------
