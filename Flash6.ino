#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pins
#define UP_BTN   2
#define DOWN_BTN 3
#define OK_BTN   4
#define POT_PIN  A0

// Character sets
const char uppercaseSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lowercaseSet[] = "abcdefghijklmnopqrstuvwxyz";
const char numberSet[]    = "0123456789";

int mode = 0; // 0=uppercase, 1=lowercase, 2=numbers
String typedText = "";
int startIndex = 0;  // current letter index

// Parameters for wedge menu
const int visibleSlices = 6;
const int centerX = 0;
const int centerY = SCREEN_HEIGHT / 2;
const int innerR = 15;

// Text box scrolling
int scrollOffset = 0; // vertical pixel scroll offset

void setup() {
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(OK_BTN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Halt if display not found
  }
  display.clearDisplay();
  display.display();

  // Start typing interface
  startTyping();
}

void loop() {
  // Typing handled inside startTyping
}

// ==================== Typing Interface ====================

void startTyping() {
  while (true) {
    int maxChars = getCharCount();

    // Pot controls scrolling through characters
    int potVal = analogRead(POT_PIN);
    startIndex = map(potVal, 0, 1023, 0, maxChars - 1); // 0 = first char

    // Change input mode (upper/lower/number)
    if (digitalRead(UP_BTN) == LOW) {
      delay(200);
      mode = (mode + 1) % 3;
    }

    // Backspace
    if (digitalRead(DOWN_BTN) == LOW) {
      delay(200);
      if (typedText.length() > 0) {
        typedText.remove(typedText.length() - 1);
        if (scrollOffset > 0) scrollOffset--; // adjust scroll if needed
      }
    }

    // Select middle character
    if (digitalRead(OK_BTN) == LOW) {
      delay(200);
      typedText += getCharAt((startIndex + visibleSlices / 2) % maxChars);

      // Auto-scroll if text height exceeds box
      int lines = (typedText.length() * 6) / 84 + 1; // rough lines estimate
      int maxLines = 64 / 8; // max lines in text box
      if (lines > maxLines) {
        scrollOffset = (lines - maxLines) * 8;
      }
    }

    drawUI();
  }
}

// ==================== Helper Functions ====================

void drawUI() {
  display.clearDisplay();

  // Draw wedge menu
  drawWedgeMenu();

  // Draw text box
  int boxX = 40, boxY = 0, boxW = 88, boxH = 64;
  display.drawRect(boxX, boxY, boxW, boxH, WHITE);

  // Draw typed text with manual wrapping and scrolling
  display.setTextSize(1);
  display.setTextColor(WHITE);

  int cursorX = boxX + 4; // small padding
  int cursorY = boxY + 4 - scrollOffset; // apply scroll
  int charWidth = 6; // width of a character in pixels
  int charHeight = 8; // height of a character in pixels

  for (int i = 0; i < typedText.length(); i++) {
    char c = typedText[i];

    // Move to next line if exceeding box width
    if (cursorX + charWidth > boxX + boxW - 2) {
      cursorX = boxX + 4;
      cursorY += charHeight;
    }

    // Stop drawing if below the visible area
    if (cursorY + charHeight > boxY + boxH) break;

    if (cursorY + charHeight > boxY) { // only draw if inside box
      display.setCursor(cursorX, cursorY);
      display.print(c);
    }

    cursorX += charWidth;
  }

  display.display();
}

void drawWedgeMenu() {
  int charCount = getCharCount();
  float angleStep = PI / visibleSlices; // divide half circle

  for (int i = 0; i < visibleSlices; i++) {
    int idx = (startIndex + i) % charCount;
    char c = getCharAt(idx);

    float startA = -PI / 2 + i * angleStep;
    float endA   = startA + angleStep;

    // Highlight middle slice
    if (i == visibleSlices / 2) {
      display.fillCircle(centerX + cos((startA + endA) / 2) * (innerR + 10),
                         centerY + sin((startA + endA) / 2) * (innerR + 10),
                         9, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }

    // Draw letter inside slice
    int x = centerX + cos((startA + endA) / 2) * (innerR + 8);
    int y = centerY + sin((startA + endA) / 2) * (innerR + 8);

    display.setCursor(x - 3, y - 3);
    display.print(c);
  }
}

char getCharAt(int i) {
  if (mode == 0) return uppercaseSet[i % 26];
  if (mode == 1) return lowercaseSet[i % 26];
  return numberSet[i % 10];
}

int getCharCount() {
  if (mode == 0) return 26;
  if (mode == 1) return 26;
  return 10;
}
