#include <M5ez.h>
#include <SPIFFS.h>
#include <ezTime.h>

#include "images.h"

String exit_button = "Exit";

void submenu_more() {
  ez.header.show("A simple menu in code...");
  ez.canvas.lmargin(10);
  ez.canvas.println("");
  ez.canvas.println("ezMenu menu(\"Main menu\");");
  ez.canvas.println("menu.addItem(\"Option 1\");");
  ez.canvas.println("menu.addItem(\"Option 2\");");
  ez.canvas.println("menu.addItem(\"Option 3\");");
  ez.canvas.println("while ( menu.run() ) {");
  ez.canvas.println("  if (menu.pick == 1) {");
  ez.canvas.println("    ez.msgBox (\"One!\");");
  ez.canvas.println("  }");
  ez.canvas.println("}");
  ez.buttons.wait("OK");
  
  ezMenu fontmenu("Menus can change looks");
  fontmenu.txtFont(&Satisfy_24);
  fontmenu.addItem("Menus can use");
  fontmenu.addItem("Various Fonts");
  fontmenu.runOnce();
  
  ezMenu delmenu("Menus are dynamic");
  delmenu.txtSmall();
  delmenu.addItem("You can delete items");
  delmenu.addItem("While the menu runs");
  delmenu.addItem("Delete me!");
  delmenu.addItem("Delete me!");
  delmenu.addItem("Delete me!");
  delmenu.addItem("Exit | Go back" );
  while (delmenu.runOnce()) {
    if (delmenu.pickName() == "Delete me!") {
      delmenu.deleteItem(delmenu.pick());
    }
  }
}

void powerOff() { m5.powerOFF(); }

void aboutM5ez() {
  ez.msgBox("About M5ez", "M5ez was written by | Rop Gonggrijp | | https://github.com/ropg/M5ez");
}

void mainmenu_entry() {
  if (ez.msgBox("We're gonna enter text ... !", "Have you learned to use the buttons? Go there first if you haven't been there. Or hit 'Go' to see if you can enter your name.", "Back # # Go") == "Go") {
    String your_name = ez.textInput();
    ez.msgBox("Pfew...", "Hi " + your_name + "! | | Now that was a pain! But it is good enough for entering, say, a WPA key, or don't you think?");
    ez.msgBox("Don't worry", "(You do get better with practice...)");
  }
}

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

void printButton(){
  while (true) {
  String btnpressed = ez.buttons.poll();
  if (btnpressed == "Done") break;
  if (btnpressed != "") {
    m5.lcd.fillRect (0, ez.canvas.bottom() - 45, TFT_W, 40, ez.theme->background); 
    ez.canvas.pos(20, ez.canvas.bottom() - 45);
    ez.canvas.color(TFT_RED);
    ez.canvas.font(&FreeSansBold18pt7b);
    ez.canvas.print(btnpressed);
    ez.canvas.font(&FreeSans12pt7b);
    ez.canvas.color(TFT_BLACK);
  }
  }
}

void mainmenu_menus() { 
  ezMenu submenu("This is a sub menu");
  submenu.txtSmall();
  submenu.buttons("up#Back#select##down#");
  submenu.addItem("You can make small menus");
  submenu.addItem("Or big ones");
  submenu.addItem("(Like the Main menu)");
  submenu.addItem("In this menu most options");
  submenu.addItem("Do absolutely nothing");
  submenu.addItem("They are only here");
  submenu.addItem("To demonstrate that menus");
  submenu.addItem("Can run off the screen");
  submenu.addItem("And will just scroll");
  submenu.addItem("And scroll");
  submenu.addItem("And Scroll");
  submenu.addItem("And Scroll even more");   
  submenu.addItem("more | Learn more about menus", submenu_more);
  submenu.addItem("Exit | Go back to main menu");
  submenu.run();
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

void mainmenu_image() {
  ezMenu images;
  images.imgBackground(TFT_BLACK);
  images.imgFromTop(40);
  images.imgCaptionColor(TFT_WHITE);
  images.addItem(sysinfo_jpg, "System Information", sysInfo);
  images.addItem(wifi_jpg, "WiFi Settings", ez.wifi.menu);
  images.addItem(about_jpg, "About M5ez", aboutM5ez);
  images.addItem(sleep_jpg, "Power Off", powerOff);
  images.addItem(return_jpg, "Back");
  images.run();
}
    
void mainmenu_msgs() {
  String cr = (String)char(13);
  ez.msgBox("You can show messages", "ez.msgBox shows text");
  ez.msgBox("Looking the way you want", "In any font !", "OK", true, &FreeSerifBold24pt7b, TFT_RED);
  ez.msgBox("More ez.msgBox", "Even multi-line messages where everything lines up and is kept in the middle of the screen");
  ez.msgBox("Questions, questions...", "But can it also show any buttons you want?", "No # # Yes"); 
  ez.textBox("And there's ez.textBox", "To present or compose longer word-wrapped texts, you can use the ez.textBox function." + cr + cr + "M5ez (pronounced \"M5 easy\") is a complete interface builder library for the M5Stack ESP32 system. It allows even novice programmers to create good looking interfaces. It comes with menus as text or as images, message boxes, very flexible button setup (including different length presses and multi-button functions), 3-button text input (you have to see it to believe it) and built-in Wifi support. Now you can concentrate on what your program does, and let M5ez worry about everything else.", true);
}
    
void mainmenu_buttons() {
  ez.header.show("Simple buttons...");
  ez.canvas.font(&FreeSans12pt7b);
  ez.canvas.lmargin(20);
  ez.canvas.println("");      
  ez.canvas.println("You can have three buttons");
  ez.canvas.println("with defined funtions.");
  ez.buttons.show("One # Two # Done");
  printButton();
  ez.canvas.clear();
  ez.header.show("More functions...");
  ez.canvas.println("");      
  ez.canvas.println("But why stop there?");
  ez.canvas.println("If you press a little longer");
  ez.canvas.println("You access the functions");
  ez.canvas.println("printed in cyan.");
  ez.buttons.show("One # Two # Three # Four # Done #");
  printButton();
  ez.canvas.clear();
  ez.header.show("Two keys ...");
  ez.canvas.y(ez.canvas.top() + 10);
  ez.canvas.println("It gets even better...");
  ez.canvas.println("The purple bar shows the");
  ez.canvas.println("functions for key combis.");
  ez.canvas.println("See if you can work it out...");
  ez.buttons.show("One # Two # Three # Four # Five # Six # Seven # Eight # Done");
  printButton();
}

void setup() {
  #include <themes/default.h>
  #include <themes/dark.h>
  ezt::setDebug(INFO);
  ez.begin();
}

void loop() {
  ezMenu mainmenu("Welcome to M5ez");
  mainmenu.txtSmall();
  mainmenu.addItem("Flexible text menus", mainmenu_menus);
  mainmenu.addItem("Image menus", mainmenu_image);
  mainmenu.addItem("Neat messages", mainmenu_msgs);
  mainmenu.addItem("Multi-function buttons", mainmenu_buttons);
  mainmenu.addItem("3-button text entry", mainmenu_entry);
  mainmenu.addItem("Built-in wifi & other settings", ez.settings.menu);
  mainmenu.addItem("Updates via https", mainmenu_ota);
  mainmenu.upOnFirst("last|up");
  mainmenu.downOnLast("first|down");
  mainmenu.run();
}