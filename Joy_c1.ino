#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //スクリーンの大きさ（横）
#define SCREEN_HEIGHT 64 //スクリーンの大きさ（縦）

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int VRX = A0; //ジョイスティックとArduino Unoの接続先を示している
const int VRY = A1;

int x = 64;
int y = 32;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLEDが見つかりません");
    while (1);
  }

  display.clearDisplay();
}

void loop() {

  int joyX = analogRead(VRX);
  int joyY = analogRead(VRY);

  if (joyX < 400) { //ジョイスティックを左に動かす
    x--;
  }
  if (joyX > 600) { //右に動かす
    x++;
  }

  if (joyY < 400) { //ジョイスティックを下に動かす
    y--;
  }
  if (joyY > 600) { //上に動かす
    y++;
  }

  x = constrain(x, 0, 127); //画面外に出ないように
  y = constrain(y, 0, 63);

  // OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("x=");
  display.print(x);

  display.setCursor(0,10);
  display.print("y=");
  display.print(y);

  display.fillCircle(x, y, 2, SSD1306_WHITE);

  display.display();

  // シリアルモニタ
  Serial.print("x=");
  Serial.print(x);
  Serial.print("  y=");
  Serial.println(y);

  delay(50);
}