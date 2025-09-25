#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//------------------- OLED Setup -------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//------------------- Button Pins -------------------
#define UP_BTN 2
#define DOWN_BTN 3
#define OK_BTN 4

//------------------- Function Prototypes -------------------
void drawCenteredText(const char* text, int y, int textSize = 1);
void showScreen(const char* title);
void showMenu();
void handleMenuInput();
void runSelectedFunction(int index);
void handleSubMenu(const char* title, const char* items[], int itemCount);

//=================== Main Menu ====================
const char* menuItems[] = {
  "Contacts", "Call Logs", "Messages", "Alarms", "Radio", "Tools", "Settings", "About"
};
const int MENU_ITEMS = sizeof(menuItems) / sizeof(menuItems[0]);
const int VISIBLE_ITEMS = 3;

int selectedIndex = 0;
bool inMenu = true;

//=================== Tools Menu ====================
const char* toolsItems[] = { "NFC/RFID", "Bluetooth", "WiFi", "IR", "FlashLight", "Back" };
const int TOOLS_ITEMS = sizeof(toolsItems) / sizeof(toolsItems[0]);
const int TOOLS_VISIBLE_ITEMS = 3;

int toolsIndex = 0;
bool inToolsMenu = false;

//=================== Submenu Items ====================
const char* contactsSub[] = { "Add Contact", "Search", "Back" };
const char* callLogsSub[] = { "View Logs", "Clear Logs", "Back" };
const char* messagesSub[] = { "New Message", "Inbox", "Sent", "Back" };
const char* alarmsSub[] = { "Set Alarm", "Alarm List", "Back" };
const char* radioSub[] = { "FM", "AM", "Back" };
const char* settingsSub[] = { "Display", "Sound", "Network", "Back" };

//=================== About Page ====================
const char* aboutItems[] = {
  "--About--",
  "Device : Flash",
  "Version : 1.0.0",
  "Display : OLED 128x64",
  "Card Compiler : MO20",
  "CHIP : ATmega328P",
  "GSM : SIM800L",
  "RTC : DS1307",
  "NFC : PN532",
  "",
  "Creator : The27LINKS"
};
const int ABOUT_ITEMS = sizeof(aboutItems) / sizeof(aboutItems[0]);
const int ABOUT_VISIBLE_LINES = 6;

//=================== Setup ====================
void setup() {
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(OK_BTN, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

//=================== Main Loop ====================
void loop() {
  if (inMenu) {
    showMenu();
    handleMenuInput();
  } else if (inToolsMenu) {
    handleToolsMenu();
  } else {
    runSelectedFunction(selectedIndex);
  }
}

//=================== Main Menu Functions ====================
void showMenu() {
  display.clearDisplay();
  display.setTextSize(2);

  int startIdx = selectedIndex - VISIBLE_ITEMS / 2;
  if (startIdx < 0) startIdx = 0;
  if (startIdx > MENU_ITEMS - VISIBLE_ITEMS) startIdx = MENU_ITEMS - VISIBLE_ITEMS;

  for (int i = 0; i < VISIBLE_ITEMS; i++) {
    int idx = startIdx + i;
    if (idx >= MENU_ITEMS) break;

    bool isSelected = (idx == selectedIndex);
    if (isSelected) {
      display.fillRect(0, i * 21, 128, 21, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(2, i * 21);
    display.print(menuItems[idx]);
  }

  // Draw scroll bar
  int trackX = 120, trackWidth = 2;
  display.drawRect(trackX, 0, trackWidth, SCREEN_HEIGHT, SSD1306_WHITE);
  int thumbHeight = (VISIBLE_ITEMS * SCREEN_HEIGHT) / MENU_ITEMS;
  int thumbY = (selectedIndex * (SCREEN_HEIGHT - thumbHeight)) / (MENU_ITEMS - 1);
  display.fillRoundRect(trackX - 1, thumbY, trackWidth + 2, thumbHeight, 1, SSD1306_WHITE);
  display.fillRoundRect(trackX, thumbY, trackWidth, thumbHeight, 1, SSD1306_BLACK);

  display.display();
}

void handleMenuInput() {
  if (!digitalRead(UP_BTN)) { selectedIndex = (selectedIndex - 1 + MENU_ITEMS) % MENU_ITEMS; delay(150); }
  if (!digitalRead(DOWN_BTN)) { selectedIndex = (selectedIndex + 1) % MENU_ITEMS; delay(150); }
  if (!digitalRead(OK_BTN)) { delay(150); inMenu = false; }
}

//=================== Run Selected Function ====================
void runSelectedFunction(int index) {
  switch (index) {
    case 0: handleSubMenu("Contacts", contactsSub, sizeof(contactsSub)/sizeof(contactsSub[0])); break;
    case 1: handleSubMenu("Call Logs", callLogsSub, sizeof(callLogsSub)/sizeof(callLogsSub[0])); break;
    case 2: handleSubMenu("Messages", messagesSub, sizeof(messagesSub)/sizeof(messagesSub[0])); break;
    case 3: handleSubMenu("Alarms", alarmsSub, sizeof(alarmsSub)/sizeof(alarmsSub[0])); break;
    case 4: handleSubMenu("Radio", radioSub, sizeof(radioSub)/sizeof(radioSub[0])); break;
    case 5: openTools(); break;
    case 6: handleSubMenu("Settings", settingsSub, sizeof(settingsSub)/sizeof(settingsSub[0])); break;
    case 7: openAbout(); break;
  }
}

//=================== Tools Menu Functions ====================
void openTools() {
  inToolsMenu = true;
  toolsIndex = 0;
}

void handleToolsMenu() {
  display.clearDisplay();
  display.setTextSize(2);

  int startIdx = toolsIndex - TOOLS_VISIBLE_ITEMS / 2;
  if (startIdx < 0) startIdx = 0;
  if (startIdx > TOOLS_ITEMS - TOOLS_VISIBLE_ITEMS) startIdx = TOOLS_ITEMS - TOOLS_VISIBLE_ITEMS;

  for (int i = 0; i < TOOLS_VISIBLE_ITEMS; i++) {
    int idx = startIdx + i;
    if (idx >= TOOLS_ITEMS) break;

    bool isSelected = (idx == toolsIndex);
    if (isSelected) {
      display.fillRect(0, i * 21, 128, 21, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.setCursor(2, i * 21);
    display.print(toolsItems[idx]);
  }

  // Scroll bar for Tools
  int trackX = 120, trackWidth = 2;
  display.drawRect(trackX, 0, trackWidth, SCREEN_HEIGHT, SSD1306_WHITE);
  int thumbHeight = (TOOLS_VISIBLE_ITEMS * SCREEN_HEIGHT) / TOOLS_ITEMS;
  int thumbY = (toolsIndex * (SCREEN_HEIGHT - thumbHeight)) / (TOOLS_ITEMS - 1);
  display.fillRoundRect(trackX - 1, thumbY, trackWidth + 2, thumbHeight, 1, SSD1306_WHITE);
  display.fillRoundRect(trackX, thumbY, trackWidth, thumbHeight, 1, SSD1306_BLACK);

  display.display();

  // Handle input
  if (!digitalRead(UP_BTN)) { toolsIndex = (toolsIndex - 1 + TOOLS_ITEMS) % TOOLS_ITEMS; delay(150); }
  if (!digitalRead(DOWN_BTN)) { toolsIndex = (toolsIndex + 1) % TOOLS_ITEMS; delay(150); }
  if (!digitalRead(OK_BTN)) {
    delay(150);
    if (toolsIndex == TOOLS_ITEMS - 1) { // Back
      inToolsMenu = false;
      inMenu = true;
    } else {
      showScreen(toolsItems[toolsIndex]);
      while (!digitalRead(OK_BTN)); delay(150);
    }
  }
}

//=================== About Page ====================
void openAbout() {
  int aboutIndex = 0;
  bool inAbout = true;

  while (inAbout) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    for (int i = 0; i < ABOUT_VISIBLE_LINES; i++) {
      int idx = aboutIndex + i;
      if (idx >= ABOUT_ITEMS) break;
      drawCenteredText(aboutItems[idx], i * 10, 1);
    }
    display.display();

    if (!digitalRead(UP_BTN)) { if (aboutIndex > 0) aboutIndex--; delay(150); }
    if (!digitalRead(DOWN_BTN)) { if (aboutIndex < ABOUT_ITEMS - ABOUT_VISIBLE_LINES) aboutIndex++; delay(150); }
    if (!digitalRead(OK_BTN)) { while (!digitalRead(OK_BTN)); delay(200); inAbout = false; }
  }
  inMenu = true;
}

//=================== Helper Functions ====================
void handleSubMenu(const char* title, const char* items[], int itemCount) {
  int subIndex = 0;
  bool inSubMenu = true;

  while (inSubMenu) {
    display.clearDisplay();
    display.setTextSize(2);

    int startIdx = subIndex - 1;
    if (startIdx < 0) startIdx = 0;
    if (startIdx > itemCount - 3) startIdx = itemCount - 3;

    for (int i = 0; i < 3; i++) {
      int idx = startIdx + i;
      if (idx >= itemCount) break;

      bool isSelected = (idx == subIndex);
      if (isSelected) {
        display.fillRect(0, i * 21, 128, 21, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }

      display.setCursor(2, i * 21);
      display.print(items[idx]);
    }

    display.display();

    if (!digitalRead(UP_BTN)) { subIndex = (subIndex - 1 + itemCount) % itemCount; delay(150); }
    if (!digitalRead(DOWN_BTN)) { subIndex = (subIndex + 1) % itemCount; delay(150); }
    if (!digitalRead(OK_BTN)) {
      delay(150);
      if (strcmp(items[subIndex], "Back") == 0) { inSubMenu = false; inMenu = true; }
      else { showScreen(items[subIndex]); while (!digitalRead(OK_BTN)); delay(150); }
    }
  }
}

void showScreen(const char* title) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 20);
  display.println(title);

  display.setCursor(0, 40);
  display.println("Press OK to go back");
  display.display();

  while (digitalRead(OK_BTN)); // Wait until OK released
  while (!digitalRead(OK_BTN)); // Wait until OK pressed
  delay(200);
}

void drawCenteredText(const char* text, int y, int textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x, y);
  display.print(text);
}
