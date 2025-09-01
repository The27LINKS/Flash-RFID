#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Caller info
String callerName = "The 27LINKS";
String callerNumber = "+91 91033 98177";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
}

void loop() {
  drawCallingScreen();
  delay(400); // controls animation speed
}

void drawCallingScreen() {
  display.clearDisplay();

  // === Profile Placeholder (Top Center) ===
  int profileX = SCREEN_WIDTH / 2;
  int profileY = 10;       // top margin
  int profileRadius = 10;  // circle radius

  // Outer circle
  display.drawCircle(profileX, profileY, profileRadius, SSD1306_WHITE);
  display.fillCircle(profileX, profileY, profileRadius - 2, SSD1306_BLACK);

  // === Avatar Inside Profile Circle ===
  // Head
  display.drawCircle(profileX, profileY - 2, 3, SSD1306_WHITE);
  display.fillCircle(profileX, profileY - 2, 2, SSD1306_WHITE);

  // Bigger Shoulders / Torso
  display.drawLine(profileX - 6, profileY + 4, profileX + 6, profileY + 4, SSD1306_WHITE); // wider shoulders
  display.drawLine(profileX - 6, profileY + 4, profileX - 3, profileY + 8, SSD1306_WHITE); // left torso
  display.drawLine(profileX + 6, profileY + 4, profileX + 3, profileY + 8, SSD1306_WHITE); // right torso
  display.drawLine(profileX - 3, profileY + 8, profileX + 3, profileY + 8, SSD1306_WHITE); // bottom torso

  // === Caller Name ===
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int nameX = (SCREEN_WIDTH - callerName.length() * 6) / 2; // center
  display.setCursor(nameX, 24);
  display.print(callerName);

  // === Caller Number ===
  int numX = (SCREEN_WIDTH - callerNumber.length() * 6) / 2;
  display.setCursor(numX, 35);
  display.print(callerNumber);

  // === Animated "Calling..." Text ===
  static int dotCount = 0;
  dotCount = (dotCount + 1) % 4; // 0..3 dots
  String callingText = "Calling";
  for (int i = 0; i < dotCount; i++) callingText += ".";
  int textX = (SCREEN_WIDTH - callingText.length() * 6) / 2;
  display.setCursor(textX, 45);
  display.print(callingText);

  // === Accept / Reject Buttons with Text ===
  display.setCursor(3, SCREEN_HEIGHT - 8); // left
  display.print("<Reject");
  display.setCursor(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 8); // right
  display.print("Pick>");

  display.display();
}
             
