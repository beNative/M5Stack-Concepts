#ifndef _CBFTPSERVER_H_
#define _CBFTPSERVER_H_

#include <MenuCallBack.h>
#include "ESP32FtpServer.h"

class CBFTPserver : public MenuCallBack
{
public:
  FtpServer ftpSrv;

  bool setup(){
    ftpSrv.begin("esp32","esp32");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
    M5.Lcd.fillScreen(0);
    M5.Lcd.setTextColor(0xFFFF);
    M5.Lcd.setCursor(0,20);
    M5.Lcd.fillRect(0,M5.Lcd.getCursorY()-2, TFT_HEIGHT, 12, 0x00F8);
    M5.Lcd.print("  FTP Server (SDcard)\r\n\r\n");
    M5.Lcd.println("host : " + WiFi.localIP().toString());
    M5.Lcd.print("user : esp32\r\npass : esp32\r\n\r\n");
    M5.Lcd.print("ftp://esp32:esp32@" + WiFi.localIP().toString() + "/");
    return true;
  }

  bool loop()
  {
    ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   

    return true;
  }

private:
};
#endif
