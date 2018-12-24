#include <vector>
#include <SPIFFS.h>
#include <Wifi.h>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <M5OnScreenKeyboard.h>
#include <M5StackUpdater.h>
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>
#include <MenuItemWiFiClient.h>
#include <Preferences.h>
#include <esp_deep_sleep.h>
#include <M5ButtonDrawer.h>
//#include <MQTT.h>


#include "MPU9250Demo.h"
//#include "MenuItemSDUpdater.h"
#include "HeaderSample.h"
#include "SystemInfo.h"
#include "I2CScanner.h"
//#include "WiFiWPS.h"
// #include "CBFTPserver.h"
// #include "CBFTPserverSPIFFS.h"

/*
Remarks:
  Both M5OnScreenKeyboard and M5TreeView are included as local libraries as the official version does
  not work with platformio yet.
*/

//MQTTClient client;
M5TreeView treeView;
M5OnScreenKeyboard osk;
HeaderSample header;

typedef std::vector<MenuItem*> vmi;

template <class T>
void CallBackExec(MenuItem* sender)
{
  T menucallback;
  menucallback(sender);
}

void MQTTdemo(){
  // ez.screen.clear();
  // ez.header.show("MQTT demo");
  // ez.buttons.show("#" + exit_button + "#");

  // ez.canvas.font(&FreeSans9pt7b);

  // WiFi.begin(ssid, pass);

  // // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // // You need to set the IP address directly.
  // client.begin("192.168.0.226", net);
  // client.onMessage(messageReceived);
  // connect();

  //  while(true) {
  //   String btn = ez.buttons.poll();
  //   if (btn == "Exit") break;
  //   client.loop();
  // }
}

void UARTdemo(){
  // String s(Serial.baudRate());
  // ez.screen.clear();
  // ez.header.show("UART demo");
  // ez.buttons.show("#" + exit_button + "#" + s.c_str());
  // ez.canvas.font(&FreeSans9pt7b);
  //  while(true) {
  //   String btn = ez.buttons.poll();
  //   if (btn == "Exit") break;
  //   s = Serial.readString();
  //   if (s != "")
  //   {
  //     ez.canvas.println(s);
  //     ez.header.show("UART demo");
  //   }
  //   client.loop();
  // }
}

uint16_t publish()
{
  // client.loop();
  // delay(10);  // <- fixes some issues with WiFi stability

  // if (!client.connected()) {
  //   connect();
  // }

  // client.publish("/hello", "world");
  // return 1000;
}

void drawFrame() {
  Rect16 r = treeView.clientRect;
  r.inflate(1);
  M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
  M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
}

void FileView(File ff){
  M5.Lcd.setTextColor(0xffff,0);
  M5.Lcd.setCursor(0,0);
  size_t len = ff.size();
  uint8_t buf[256];

  if (ff.read(buf, len)) {
    for(int i=0; i<len; ++i){
      M5.Lcd.write(buf[i]);
    }
  }
}

void CallBackWiFiClient(MenuItem* sender)
{
  // MenuItemWiFiClient* mi = static_cast<MenuItemWiFiClient*>(sender);
  // if (!mi) return;

  // if (mi->ssid == "") return;

  // Preferences preferences;
  // preferences.begin("wifi-config");
  // preferences.putString("WIFI_SSID", mi->ssid);
  // String wifi_passwd = preferences.getString("WIFI_PASSWD");

  // if (mi->auth != WIFI_AUTH_OPEN) {
  //   osk.setup(wifi_passwd);
  //   while (osk.loop()) { delay(1); }
  //   wifi_passwd = osk.getString();
  //   osk.close();
  //   WiFi.disconnect();
  //   WiFi.begin(mi->ssid.c_str(), wifi_passwd.c_str());
  //   preferences.putString("WIFI_PASSWD", wifi_passwd);
  // } else {
  //   WiFi.disconnect();
  //   WiFi.begin(mi->ssid.c_str(), "");
  //   preferences.putString("WIFI_PASSWD", "");
  // }
  // preferences.end();
  // while (M5.BtnA.isPressed()) M5.update();
}

void CallBackWiFiDisconnect(MenuItem* sender)
{
 // WiFi.disconnect(true);
}

void CallBackDeepSleep(MenuItem* sender)
{
   esp_deep_sleep_start();
}

void CallBackBrightness(MenuItem* sender)
{
  MenuItemNumeric* mi = static_cast<MenuItemNumeric*>(sender);
  if (!mi) return;
  M5.Lcd.setBrightness(mi->value);
}

void CallBackDACtest(MenuItem* sender)
{
  MenuItemNumeric* mi = static_cast<MenuItemNumeric*>(sender);
  if (!mi) return;

  pinMode(mi->tag, OUTPUT);
  dacWrite(mi->tag, mi->value);
}

void CallBackFS(MenuItem* sender)
{
  MenuItemFS* mi = static_cast<MenuItemFS*>(sender);
  if (!mi) return;

  if (mi->isDir) return;

  M5.Lcd.clear(0);
  int idx = mi->path.lastIndexOf('.') + 1;
  String ext = mi->path.substring(idx);
  if (ext == "jpg") {
    M5.Lcd.drawJpgFile(mi->getFS(), mi->path.c_str());
  } else {
    File file = mi->getFS().open(mi->path, FILE_READ);
    if (!file.isDirectory()) {
      FileView(file);
    }
    file.close();
  }
  M5ButtonDrawer btnDrawer;
  btnDrawer.setText("Back","","");
  btnDrawer.draw(true);
  while (!M5.BtnA.wasReleased()) M5.update();
}

void setup() {
  M5.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
     Serial.println("Will Load menu binary");
     updateFromFS(SD);
     ESP.restart();
  }
  M5.Lcd.setBrightness(200);

  treeView.clientRect.x = 2;
  treeView.clientRect.y = 16;
  treeView.clientRect.w = 316;
  treeView.clientRect.h = 200;
  treeView.itemWidth    = 220;

  treeView.useFACES       = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;

  treeView.fontColor[0]  = 0xFFFF;
  treeView.backColor[0]  = 0x0000;
  treeView.frameColor[0] = 0xA514;
  treeView.fontColor[1]  = 0x0000;
  treeView.backColor[1]  = 0xFFF8;
  treeView.frameColor[1] = 0xFFFF;
  treeView.backgroundColor = 0x4208;
  treeView.font            = 2;
  treeView.itemHeight      = 20;
  M5ButtonDrawer::height   = 20;
  M5ButtonDrawer::width    = 80;

  osk.useTextbox     = true;
  osk.useFACES       = true;
  osk.useCardKB      = true;
  osk.useJoyStick    = true;
  osk.usePLUSEncoder = true;
  osk.msecHold   = treeView.msecHold;
  osk.msecRepeat = treeView.msecRepeat;

  treeView.setItems(vmi
  { new MenuItem("Devices", vmi
      { new MenuItem("MPU9250", CallBackExec<MPU9250Demo>) }
    )
    , new MenuItem("Numeric Sample ", vmi
    { new MenuItemNumeric("Brightness", 0, 255, 80, CallBackBrightness)
    , new MenuItemNumeric("GPIO25 DAC", 0, 255, 0, 25, CallBackDACtest)
    , new MenuItemNumeric("GPIO26 DAC", 0, 255, 0, 26, CallBackDACtest)
    } )
  , new MenuItem("WiFi ", vmi
    {
      //new MenuItemWiFiClient("WiFi Client", CallBackWiFiClient)
    //,
     new MenuItem("WiFi mode", vmi
      { new MenuItem("WiFi disconnect(true)", 2000)
      , new MenuItem("WiFi mode OFF", 2001)
      , new MenuItem("WiFi mode STA", 2002)
      , new MenuItem("WiFi mode AP" , 2003)
      , new MenuItem("WiFi mode AP STA", 2004)
      ,new MenuItem("WiFi ", vmi
                 {
                   //new MenuItemWiFiClient("WiFi Client", CallBackWiFiClient)
                 //, new MenuItem("WiFi WPS", WiFiWPS())
                 //,
                 new MenuItem("WiFi disconnect", CallBackWiFiDisconnect)
                 } )
               , new MenuItem("Tools", vmi
                 { new MenuItem("System Info", SystemInfo())
                 , new MenuItem("I2C Scanner", I2CScanner())
                //  , new MenuItem("FTP Server (SDcard)", CBFTPserver())
                //  , new MenuItem("FTP Server (SPIFFS)", CBFTPserverSPIFFS())
                 } )
               //, new MenuItemSDUpdater("SD Updater")
               , new MenuItemSD("SD card")
               , new MenuItemSPIFFS("SPIFFS")
               , new MenuItem("DeepSleep", CallBackDeepSleep)
    } )
  } )
  , new MenuItemSD("SD card", CallBackFS)
  , new MenuItemSPIFFS("SPIFFS", CallBackFS)
}
  );

  treeView.begin();
  drawFrame();
}

void loop() {
  MenuItem* mi = treeView.update();
   if (treeView.isRedraw()) {
     drawFrame();
   }

  //if (!(loopcounter % 10))  header.draw();

  if (mi != NULL) {
    switch (mi->tag) {
    default: return;
    // case 2000:
    //   WiFi.disconnect(true);
    //   return;

    // case 2001:
    //   WiFi.mode(WIFI_OFF);
    //   return;

    // case 2002:
    //   WiFi.mode(WIFI_STA);
    //   return;

    // case 2003:
    //   WiFi.mode(WIFI_AP);
    //   return;

    // case 2004:
    //   WiFi.mode(WIFI_AP_STA);
    //   return;
    //
     }
    for (int i = 0; i < 2; ++i) {
      M5ButtonDrawer::fontColor[i] = treeView.fontColor[i];
      M5ButtonDrawer::backColor[i] = treeView.backColor[i];
      M5ButtonDrawer::frameColor[i] = treeView.frameColor[i];
      osk.fontColor[i] = treeView.fontColor[i];
      osk.backColor[i] = treeView.backColor[i];
      osk.frameColor[i]= treeView.frameColor[i];
    }
    M5ButtonDrawer::font = treeView.font;
    M5.Lcd.fillRect(0, 218, M5.Lcd.width(), 22, 0);
  }
}
