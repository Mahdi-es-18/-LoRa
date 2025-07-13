#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_DOT    2
#define BUTTON_DASH   3
#define BUTTON_SPACE  4

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String sentMessage = "";
String receivedMessage = "";

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_DOT, INPUT_PULLUP);
  pinMode(BUTTON_DASH, INPUT_PULLUP);
  pinMode(BUTTON_SPACE, INPUT_PULLUP);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("LoRa Morse Ready");
  display.display();

  // LoRa
  if (!LoRa.begin(433E6)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LoRa Failed");
    display.display();
    while (1);
  }
}

void loop() {
  // دریافت
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      char c = (char)LoRa.read();
      receivedMessage += c;
      updateDisplay();
    }
  }

  // ارسال نقطه
  if (digitalRead(BUTTON_DOT) == LOW) {
    sendChar('.');
  }

  // ارسال خط
  if (digitalRead(BUTTON_DASH) == LOW) {
    sendChar('-');
  }

  // ارسال فاصله
  if (digitalRead(BUTTON_SPACE) == LOW) {
    sendChar(' ');
  }

  delay(150); // جلوگیری از تکرار
}

void sendChar(char c) {
  sentMessage += c;
  LoRa.beginPacket();
  LoRa.print(c);
  LoRa.endPacket();
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sent:");
  display.println(sentMessage);
  display.println("");
  display.println("Recv:");
  display.println(receivedMessage);
  display.display();
}
