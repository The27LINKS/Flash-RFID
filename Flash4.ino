#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Helper function to draw an arc (startAngle & endAngle in degrees)
void drawArc(int x0, int y0, int radius, int startAngle, int endAngle) {
  for (int a = startAngle; a <= endAngle; a++) {
    float rad = a * 3.14159 / 180.0;
    int x = x0 + radius * cos(rad);
    int y = y0 + radius * sin(rad);
    display.drawPixel(x, y, SSD1306_WHITE);
  }
}

void animatePn532(String Condition){
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  // Radii for arcs (distance from center)
  int arcRadii[] = {30, 40, 50, 60};
  int totalArcs = 4;

  // Sequentially display arcs
  for (int i = 0; i < totalArcs; i++) {
    display.clearDisplay();

    // Draw central "Scanning" text
    display.setTextSize(1); // small size
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(Condition, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(centerX - w / 2, centerY - h / 2);
    display.print(Condition);

       // Draw right arcs up to current index (shorter arc length: 40°)
    for (int r = 0; r <= i; r++) {
      drawArc(centerX, centerY, arcRadii[r], -20, 20);  // Right side
    }

    // Draw left arcs up to current index (shorter arc length: 40°)
    for (int r = 0; r <= i; r++) {
      drawArc(centerX, centerY, arcRadii[r], 160, 200); // Left side
    }

    display.display();
    delay(100);
  }
}
void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) ; // hang if OLED not found
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
 animatePn532("Emulating");

  delay(100);
}
