#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <iostream>
#include <string>

TFT_eSPI tft = TFT_eSPI();

// const char *ssid = "desenvolvimento";
// const char *password = "11223344";

const char *ssid = "TC-CORPORATIVO";
const char *password = "";

AsyncWebServer server(80);

String mac;
String parametro;
String id_number;

int status = 11;
int batery = 100;
int rele = 27;
int aviso = 15;
int feedback = 15;

void setup()
{
  tft.setRotation(4);
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(4); // Landscape
  tft.fillScreen(TFT_BLACK);

  //Define pin
  pinMode(rele, OUTPUT);
  pinMode(feedback, OUTPUT);

  digitalWrite(rele, 1);
  digitalWrite(feedback, 1);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  mac = WiFi.macAddress();
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  server.on("/checagem", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", mac.c_str()); });
  server.on("/setpoint", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int params = request->params();
    for(int i = 0; i < params; i++){
      AsyncWebParameter* p = request->getParam(i);
      id_number = p->name().c_str();
      parametro = p->value().c_str();

      if(id_number == "status"){
        status = atoi(parametro.c_str());
         Serial.printf("Status atual: %\n", status);
      }
      if(id_number == "batery"){
        batery = atoi(parametro.c_str());
        Serial.printf("Nivel bateria: %i\n", batery);
      }
    }request->send(200, "text/plain", "Success, setpoint"); });
 
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop()
{

if(status == 10){
  digitalWrite(rele, 1);
}
else if(status == 11) {
  digitalWrite(rele, 0);
}
else if(status == 32){
    digitalWrite(rele, 1);
} 
else if(status == 33) {
  digitalWrite(rele, 0);
}
else {
  digitalWrite(rele, 1);
}
if(batery < 80 && (status == 10 || status == 32) ) {
  digitalWrite(aviso, 1);
} else {digitalWrite(aviso, 0);}
  delay(500);
}
