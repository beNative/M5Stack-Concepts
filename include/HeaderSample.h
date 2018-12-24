#ifndef _HEADERSAMPLE_H_
#define _HEADERSAMPLE_H_

#include <M5Stack.h>
#include <Wifi.h>
#include <esp_wifi_types.h>
#include <esp_wifi.h>

class HeaderSample {
public:
  HeaderSample() {};

  uint16_t colorFill = 0x630C;
  uint16_t colorFont = 0xffff;
  uint8_t font = 1;

  int drawStr(const String& src, int x)
  {
    M5.Lcd.drawString(src,x,0,font);
    return x + M5.Lcd.textWidth(src,font);
  }

  String wifiStatus(wl_status_t src) {
    switch (src)
    {
    case WL_IDLE_STATUS    : return "IDLE_STATUS";
    case WL_NO_SSID_AVAIL  : return "NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED : return "SCAN_COMPLETED";
    case WL_CONNECTED      : return ""; // CONNECTED";
    case WL_CONNECT_FAILED : return "CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION_LOST";
    case WL_DISCONNECTED   : return "DISCONNECTED";
    }
    return "";
  }

  void draw()
  {
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(colorFont, colorFill);
    M5.Lcd.drawFastHLine(0, 8, TFT_HEIGHT, 0xC618);

    int x = 0;

    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    if (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA) {
      x = drawStr("AP:", x);
      x = drawStr(WiFi.localIP().toString(), x);
      x = drawStr(" ", x);
    }
    if (mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA) {
      wl_status_t s = wl_status_t(WiFi.status());
      x = drawStr(wifiStatus(s), x);
      x = drawStr(" ", x);
      if (s == WL_CONNECTED) {
        x = drawStr(WiFi.localIP().toString(), x);
        x = drawStr(" ", x);
      }
    }
    M5.Lcd.setTextFont(font);
    M5.Lcd.setCursor(TFT_HEIGHT - 96, 0);
    M5.Lcd.printf("Free%7d Byte", esp_get_free_heap_size());
    M5.Lcd.fillRect(x, 0, TFT_HEIGHT - 96-x, 8, colorFill);
  }
};

#endif
