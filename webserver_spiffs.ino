/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_html_js.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use JavaScript in HTML weppages
  that are served in a web server's response.
  https://youtu.be/ZJoBy2c1dPk

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;
uint8_t pin_led = BUILTIN_LED;
char* ssid = "YOUR_WIFI_SSID";
char* password = "YOUR_WIFI_PASSWORD";


void setup()
{
  SPIFFS.begin();
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", serveIndexFile);
  server.on("/bundle.min.js", serveBundleFile);
  server.on("/styles.css", serveStyleFile);
  server.on("/ledstate",getLEDState);
  server.onNotFound(onError);
  server.begin();
}

void loop() {
  server.handleClient();
}

void serveIndexFile() {
  Serial.println(server.arg("plain"));
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}


void serveBundleFile() {
  Serial.println(server.arg("plain"));
  File file = SPIFFS.open("/bundle.min.js","r");
  server.streamFile(file, "application/javascript");
  file.close();
}

void serveStyleFile() {
  Serial.println(server.arg("plain"));
  File file = SPIFFS.open("/styles.css","r");
  server.streamFile(file, "text/css");
  file.close();
}

void onError() {
  Serial.println(server.arg("plain"));
  server.send_P(404,"text/html", "Error 404");
}

