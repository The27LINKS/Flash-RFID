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

//------------------- Menu Configuration -------------------
const char* menuItems[] = {
  "Option 1", "Option 2", "Option 3", "Option 4", "Option 5",
  "Option 6", "Option 7", "Option 8", "Option 9", "Option 10"
};
const int MENU_ITEMS = sizeof(menuItems) / sizeof(menuItems[0]);
const int VISIBLE_ITEMS = 3;

//------------------- Menu State -------------------
int selectedIndex = 0;
bool inMenu = true;

//------------------- Setup -------------------
void setup() {
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(OK_BTN, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

//------------------- Main Loop -------------------
void loop() {
  if (inMenu) {
    showMenu();
    handleMenuInput();
  } else {
    showSelectedOption();
    if (!digitalRead(OK_BTN)) {
      delay(200);
      inMenu = true;
    }
  }
}

//=================== User Defined Functions ===================

//------------------- Draw Menu -------------------
void showMenu() {
  display.clearDisplay();
  display.setTextSize(2);

  int startIdx = selectedIndex - VISIBLE_ITEMS / 2;
  if (startIdx < 0) startIdx = 0;
  if (startIdx > MENU_ITEMS - VISIBLE_ITEMS) startIdx = MENU_ITEMS - VISIBLE_ITEMS;

  for (int i = 0; i < VISIBLE_ITEMS; i++) {
    int idx = startIdx + i;
    if (idx >= MENU_ITEMS) break;

    drawMenuItem(idx, i);
  }

  drawScrollBar();
  display.display();
}

//------------------- Draw Single Menu Item -------------------
void drawMenuItem(int menuIdx, int posIdx) {
  bool isSelected = (menuIdx == selectedIndex);

  // Highlight background if selected
  if (isSelected) {
    display.fillRect(0, posIdx * 21, 128, 21, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  } else {
    display.setTextColor(SSD1306_WHITE);
  }

  // Draw menu text
  display.setCursor(2, posIdx * 21);
  display.print(menuItems[menuIdx]);
}

//------------------- Draw Scroll Bar -------------------
void drawScrollBar() {
  int trackX = 120, trackWidth = 2;
  display.drawRect(trackX, 0, trackWidth, SCREEN_HEIGHT, SSD1306_WHITE);

  int thumbHeight = (VISIBLE_ITEMS * SCREEN_HEIGHT) / MENU_ITEMS;
  int thumbY = (selectedIndex * (SCREEN_HEIGHT - thumbHeight)) / (MENU_ITEMS - 1);

  // Shadow behind thumb for 3D effect
  display.fillRoundRect(trackX - 1, thumbY, trackWidth + 2, thumbHeight, 1, SSD1306_WHITE);
  // Thumb itself
  display.fillRoundRect(trackX, thumbY, trackWidth, thumbHeight, 1, SSD1306_BLACK);
}

//------------------- Handle Menu Input -------------------
void handleMenuInput() {
  if (!digitalRead(UP_BTN)) { selectedIndex = (selectedIndex - 1 + MENU_ITEMS) % MENU_ITEMS; delay(150); }
  if (!digitalRead(DOWN_BTN)) { selectedIndex = (selectedIndex + 1) % MENU_ITEMS; delay(150); }
  if (!digitalRead(OK_BTN)) { delay(150); inMenu = false; }
}

//------------------- Display Selected Option -------------------
void showSelectedOption() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 20);
  display.println("Selected:");
  display.println(menuItems[selectedIndex]);

  display.setCursor(0, 40);
  display.println("Press OK to go back");

  display.display();
}
