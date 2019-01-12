// Temperature Webserver
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>    //DHT11 DHT22 
//#define DHTTYPE DHT11   // DHT 11
const char* ssid = "KRISHNA-HP";
const char* password = "zzzzzzzz";

ESP8266WebServer server(80);
//const int DHTPin = D4;  //DHT11 Data Pin
// Initialize DHT sensor.
DHT dht(D4, DHT11);   // intialize our DHT11

char temperatureString[6];
static char celsiusTemp[7];
const int buzzer = D0;

void setup(void){
  Serial.begin(115200);   //Serial Communication Begin
  delay(10);
  dht.begin();   // dht monitoring
  
  WiFi.begin(ssid, password); //Conecting it to WiFi
  Serial.println(""); // Message that device is Connected or Not

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  server.begin();      //ES8266 Webserver
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", []() {
    float temperature = dht.readHumidity();
    dtostrf(temperature, 2, 2, temperatureString);
  
    String title = "HUmidity";
    String cssClass = "mediumhot";
    if (temperature < 15)
      cssClass = "cold";
    else if (temperature > 25)
      cssClass = "hot";
   
    String message = "<!DOCTYPE html><html><head><title>" + title + "</title><meta charset=\"utf-8\" /><meta name=\"viewport\" content=\"width=device-width\" /><link href='https://fonts.googleapis.com/css?family=Advent+Pro' rel=\"stylesheet\" type=\"text/css\"><style>\n";
    message += "html {height: 100%;}";
    message += "div {color: #fff;font-family: 'Advent Pro';font-weight: 400;left: 50%;position: absolute;text-align: center;top: 50%;transform: translateX(-50%) translateY(-50%);}";
    message += "h2 {font-size: 90px;font-weight: 400; margin: 0}";
    message += "body {height: 100%;}";
    message += ".cold {background: linear-gradient(to bottom, #7abcff, #0665e0 );}";
    message += ".mediumhot {background: linear-gradient(to bottom, #81ef85,#057003);}";
    message += ".hot {background: linear-gradient(to bottom, #fcdb88,#d32106);}";
    message += "</style></head><body class=\"" + cssClass + "\"><div><h1>" + title +  "</h1><h2>" + temperatureString + "&nbsp;<small>&deg;C</small></h2></div></body></html>";
    
    server.send(200, "text/html", message);
  });

  server.begin();
  
  Serial.println("Humidity web server started!");
}

void loop(void){
  server.handleClient();
}
