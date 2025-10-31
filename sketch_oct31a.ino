#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === AYARLAR ===
const int ANGLE_MIN = 0;
const int ANGLE_MAX = 180;
const int ANGLE_STEP = 2;
const int STEP_DELAY_MS = 40;

const int DANGER_CM = 30;
const int MAX_DISPLAY_CM = 150;

// === PINLER ===
const int PIN_TRIG  = 9;
const int PIN_ECHO  = 10;
const int PIN_SERVO = 6;
const int PIN_RED   = 3;
const int PIN_GREEN = 4;
const int PIN_BUZZ  = 5;

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo myServo;

/////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BUZZ, OUTPUT);

  myServo.attach(PIN_SERVO);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 bulunamadi"));
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("RADAR SISTEMI ACILIYOR");
  display.display();
  delay(1000);
  display.clearDisplay();

  myServo.write(0);
  digitalWrite(PIN_GREEN, HIGH);
}

/////////////////////////////////////////////////
// Mesafe Ölçümü
int getDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  if (duration == 0) return -1;
  int distance = duration * 0.034 / 2;
  if (distance > 300) return -1;
  return distance;
}

/////////////////////////////////////////////////
// RADAR TASARIMI
void drawRadar(int angle, int distance) {
  display.clearDisplay();
  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT - 1;
  int maxR = 60;

  // Radar halkaları
  for (int r = 15; r <= maxR; r += 15) {
    display.drawCircle(cx, cy, r, SSD1306_WHITE);
  }

  // Çizgi
  float rad = angle * (PI / 180.0);
  int lx = cx + cos(rad) * maxR;
  int ly = cy - sin(rad) * maxR;
  display.drawLine(cx, cy, lx, ly, SSD1306_WHITE);

  // Algılanan nesne
  if (distance > 0 && distance <= MAX_DISPLAY_CM) {
    float scale = (float)maxR / (float)MAX_DISPLAY_CM;
    int px = cx + cos(rad) * distance * scale;
    int py = cy - sin(rad) * distance * scale;
    display.fillCircle(px, py, 3, SSD1306_WHITE);
  }

  // Bilgi
  display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Aci:");
  display.print(angle);
  display.print("°  Mesafe:");
  if (distance > 0) {
    display.print(distance);
    display.print("cm");
  } else {
    display.print("---");
  }

  display.display();
}

/////////////////////////////////////////////////
// LED + Buzzer
void updateAlert(int distance) {
  if (distance > 0 && distance <= DANGER_CM) {
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, LOW);
    tone(PIN_BUZZ, 1000);
  } else {
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, HIGH);
    noTone(PIN_BUZZ);
  }
}

/////////////////////////////////////////////////
void loop() {
  // 0 → 180 derece
  for (int a = ANGLE_MIN; a <= ANGLE_MAX; a += ANGLE_STEP) {
    myServo.write(a);
    int d = getDistanceCm();
    updateAlert(d);
    drawRadar(a, d);
    delay(STEP_DELAY_MS);
  }

  // 180 → 0 derece
  for (int a = ANGLE_MAX; a >= ANGLE_MIN; a -= ANGLE_STEP) {
    myServo.write(a);
    int d = getDistanceCm();
    updateAlert(d);
    drawRadar(a, d);
    delay(STEP_DELAY_MS);
  }
}
