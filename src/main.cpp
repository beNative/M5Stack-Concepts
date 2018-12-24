#include <M5ez.h>
#include <SPIFFS.h>
#include <ezTime.h>

#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "telenet-ap-9907265";
const char pass[] = "4kwxeZkheZ7k";

WiFiClient net;
MQTTClient client;

String exit_button = "Exit";

void powerOff() { m5.powerOFF(); }

void mainmenu_ota() {
  if (ez.msgBox("Get OTA_https demo", "This will replace the demo with a program that can then load the demo program again.", "Cancel#OK#") == "OK") {
    ezProgressBar progress_bar("OTA update in progress", "Downloading ...", "Abort");
    #include "raw_githubusercontent_com.h" // the root certificate is now in const char * root_cert
    if (ez.wifi.update("https://raw.githubusercontent.com/ropg/M5ez/master/compiled_binaries/OTA_https.bin", root_cert, &progress_bar)) {
      ez.msgBox("Over The Air updater", "OTA download successful. Reboot to new firmware", "Reboot");
      ESP.restart();
    } else {
      ez.msgBox("OTA error", ez.wifi.updateError(), "OK");
    }
  }
}

void sysInfoPage1() {
  const byte tab = 120;
  ez.screen.clear();
  ez.header.show("System Info  (1/2)");
  ez.buttons.show("#" + exit_button + "#down");
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);
  ez.canvas.println("");
  ez.canvas.print("CPU freq:");  ez.canvas.x(tab); ez.canvas.println(String(ESP.getCpuFreqMHz()) + " MHz");
  ez.canvas.print("CPU cores:");  ez.canvas.x(tab); ez.canvas.println("2");    //   :)
  ez.canvas.print("Chip rev.:");  ez.canvas.x(tab); ez.canvas.println(String(ESP.getChipRevision()));
  ez.canvas.print("Flash speed:");  ez.canvas.x(tab); ez.canvas.println(String(ESP.getFlashChipSpeed() / 1000000) + " MHz");
  ez.canvas.print("Flash size:");  ez.canvas.x(tab); ez.canvas.println(String(ESP.getFlashChipSize() / 1000000) + " MB");
  ez.canvas.print("ESP SDK:");  ez.canvas.x(tab); ez.canvas.println(String(ESP.getSdkVersion()));
  ez.canvas.print("M5ez:");  ez.canvas.x(tab); ez.canvas.println(String(ez.version()));
  ESP.
}

void sysInfoPage2() {
  const String SD_Type[5] = {"NONE", "MMC", "SD", "SDHC", "UNKNOWN"};
  const byte tab = 140;
  ez.screen.clear();
  ez.header.show("System Info  (2/2)");
  ez.buttons.show("up#" + exit_button + "#");
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);
  ez.canvas.println("");
  ez.canvas.print("Free RAM:");  ez.canvas.x(tab);  ez.canvas.println(String((long)ESP.getFreeHeap()) + " bytes");
  ez.canvas.print("Min. free seen:");  ez.canvas.x(tab); ez.canvas.println(String((long)esp_get_minimum_free_heap_size()) + " bytes");
  const int sd_type = SD.cardType();
  
  SPIFFS.begin();
  ez.canvas.print("SPIFFS size:"); ez.canvas.x(tab); ez.canvas.println(String((long)SPIFFS.totalBytes()) + " bytes");
  ez.canvas.print("SPIFFS used:"); ez.canvas.x(tab); ez.canvas.println(String((long)SPIFFS.usedBytes()) + " bytes");
  ez.canvas.print("SD type:"); ez.canvas.x(tab); ez.canvas.println(SD_Type[sd_type]);
  if (sd_type != 0) {
    ez.canvas.print("SD size:"); ez.canvas.x(tab); ez.canvas.println(String((long)SD.cardSize()  / 1000000) + " MB");
    ez.canvas.print("SD used:"); ez.canvas.x(tab); ez.canvas.println(String((long)SD.usedBytes()  / 1000000) + " MB");
  }
}

void sysInfo() {
  sysInfoPage1();
  while(true) {
    String btn = ez.buttons.poll();
    if (btn == "up") sysInfoPage1();
    if (btn == "down") sysInfoPage2();
    if (btn == "Exit") break;
  }
}

void sensors(){
  ez.header.show("Sensor readings");
  ez.buttons.show("#" + exit_button + "#");
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);
  ez.canvas.println("");
  ez.canvas.printf("%d", analogRead(ADC1));
  while(true) {
    String btn = ez.buttons.poll();
     if (btn == "Exit") break;
  }
}

void messageReceived(String &topic, String &payload) {  
  Serial.println("incoming: " + topic + " - " + payload);
  ez.canvas.println("incoming: " + topic + " - " + payload);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");


  client.subscribe("#");
  // client.unsubscribe("/hello");
  Serial.println("\nsubscribed to #!");
  client.subscribe("a");
  Serial.println("\nsubscribed to a!");
}

void MQTTdemo(){
  ez.screen.clear();
  ez.header.show("MQTT demo");
  ez.buttons.show("#" + exit_button + "#");

  
  ez.canvas.font(&FreeSans9pt7b);

  WiFi.begin(ssid, pass);
  
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("192.168.0.226", net);
  client.onMessage(messageReceived);  
  connect();

   while(true) {
    String btn = ez.buttons.poll();
    if (btn == "Exit") break;
    client.loop();    
  }
}

unsigned long lastMillis = 0;

uint16_t publish()
{
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  client.publish("/hello", "world");
  return 1000;
}

void setup() {
  #include <themes/default.h>
  #include <themes/dark.h>

  ezt::setDebug(DEBUG);
  ez.begin();
  ez.setFreeFont(&FreeSansBold9pt7b);
 // ez.addEvent(&publish);
}

void loop() {
   ezMenu mainmenu("Concepts ESP32");
  mainmenu.txtSmall();
  mainmenu.addItem("Built-in wifi & other settings", ez.settings.menu);
  mainmenu.addItem("Updates via https", mainmenu_ota);
  mainmenu.addItem("System info", sysInfo);
  mainmenu.addItem("Sensors", sensors);
  mainmenu.addItem("MQTT demo", MQTTdemo);

  mainmenu.upOnFirst("last|up");
  mainmenu.downOnLast("first|down");
  mainmenu.run();
}
