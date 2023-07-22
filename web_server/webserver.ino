#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "oled_u8g2.h" // oled display
#include <ArduinoJson.h> // ArduinoJSON library for JSON manipulation

const char* ssid = "SmartFactory";
const char* password = "inha4885";

OLED_U8G2 oled; // create oled object
WebServer server(80);  // create WebServer object, port

int tempSensor = A2; // temperature sensor
// formula
int Vo;
double R1 = 10000;
double logR2, R2, T, Tc;
double c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
double Tf = 0;

void setup(void) {
  Serial.begin(115200);  // ESP32 baud rate
  oled.setup();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) { // wait for connection
    delay(500);
    Serial.print("-> ");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP addr: ");
  Serial.println(WiFi.localIP()); // print assigned ip address

  server.on("/", handleRootEvent);  // root, event handling function

  server.begin();
  Serial.println("Web server started!");
}

void loop(void) {
  oled.setLine(1, "Inha Factory");
  oled.setLine(2, "Web Server");
  oled.display();

  server.handleClient();  // clients process
  delay(5); // 5/1000 sec
}

void handleRootEvent() {
  Serial.print("main page from ");

  String clientIP = server.client().remoteIP().toString();  // client's ip addr.

  // 192.168.15.245 clientIP
  int octet1, octet2, octet3, octet4;
  sscanf(clientIP.c_str(), "%d.%d.%d.%d", &octet1, &octet2, &octet3, &octet4);
  String maskedIP = String(octet1) + ".XXX.XXX." + String(octet4); // 2nd, 3rd masking

  Vo = analogRead(tempSensor); // read from temperature sensing value
  R2 = R1 * (4095.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;  // celsius
  Tf = (Tc * 9.0/5.0) + 32.0;  // fahrenheit

  StaticJsonDocument<200> jsonDoc;  // create JSON object
  jsonDoc["message"] = "Welcome Inha SmartFactory WebServer!";
  jsonDoc["ip_address"] = maskedIP;
  jsonDoc["temperature_celsius"] = Tc;
  jsonDoc["temperature_fahrenheit"] = Tf;

  String jsonResponse;  // JSON to string
  serializeJson(jsonDoc, jsonResponse);

  server.send(200, "application/json", jsonResponse); // status code 200(OK), format, JSON response

  Serial.println(clientIP);
  Serial.print(Tc);
  Serial.print("C (");
  Serial.print(Tf);
  Serial.println("F)");
}
