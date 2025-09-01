#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Simulated radio values
float currentFrequency = 99.7;  // Example FM station
int volumeLevel = 10;             // Volume 0–10
float minFreq = 88.0;
float maxFreq = 108.0;
String channelName = "Big Fm";  // Station name

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();

}

void loop() {
      drawRadioUI();
  delay(500);  // refresh
}

void drawRadioUI() {
  display.clearDisplay();
  
  // === Frequency in Big Numbers ===
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 0);
  display.print(currentFrequency, 1);  // One decimal place
  display.print("MHz");

  // === Channel Name ===
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - (channelName.length() * 6)) / 2, 20);
  display.println(channelName);
  
  // === Volume Indicator ===
  display.setCursor(1, 30);
  display.setTextSize(1);
  display.print("VOL:");
  display.print(volumeLevel);
  display.drawRect(40, 30, 80, 6, SSD1306_WHITE);
  display.fillRect(40, 30, map(volumeLevel, 0, 10, 0, 80), 6, SSD1306_WHITE);

  // === Menu Bar ===
  display.setTextSize(1);
  display.setCursor(5, 54);
  display.print("<Pre.");
  display.setCursor(43, 54);
  display.print("[Scan]");
  display.setCursor(90, 54);
  display.print("Next>");
  radioScan();
  display.display();
}
void radioScan(){
// === Scanning Text Animation ===
 static int dotCount = 0;  // Keeps track of dots
 dotCount = (dotCount + 1) % 4;  // Cycle through 0,1,2,3
 String scanningText = "Scanning";
 for (int i = 0; i < dotCount; i++) {
   scanningText += ".";
}
// Draw the text centered
 display.setTextSize(1);
 int textX = (SCREEN_WIDTH - (scanningText.length() * 6)) / 2;
 display.setCursor(textX, 42);  // Adjust Y as needed
 display.print(scanningText);
}
