//10秒ピッタシに押せた方の勝ちのミニゲーム
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int P1_BUTTON = 2;
const int P2_BUTTON = 3;

bool p1Pressed = false;
bool p2Pressed = false;

unsigned long p1Time = 0;
unsigned long p2Time = 0;

//開始段階・10seconds minigame!と表示
void StartScreen(){
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("10seconds minigame!");   // 前の画面を消す

  display.display();  //メモリの内容をOLEDに送信
  delay(1000);
  

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Ready...");   // 前の画面を消す

  display.display();  //メモリの内容をOLEDに送信
  delay(500);


  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("START!!");   // 前の画面を消す

  display.display();  //メモリの内容をOLEDに送信
  delay(1000);
}

//10秒から減っていってくのを表示
void WaitTime(){
  p1Pressed = false;
  p2Pressed = false;
  long p1Time = 0;
  long p2Time = 0; 
  unsigned long startTime = millis();  //10秒のカウントダウン用

  while (true) { 
    unsigned long elapsedTime = millis() - startTime;  //秒数計測。elapsedTimeは経過時間
    int remain = 10 - elapsedTime / 1000;  //「millis()」は1000分の1秒

    display.clearDisplay();

    if (remain >= 7){ //7秒以上だけ表示させる
      display.setTextSize(3);
      display.setCursor(45,20);
      display.print(remain);
    }

    display.display();

    if (digitalRead(P1_BUTTON) == LOW && !p1Pressed) {   //1人目のボタンが押された＆P1がボタンをまだ押していない
        p1Pressed = true;
        p1Time = elapsedTime;
    }

    if (digitalRead(P2_BUTTON) == LOW && !p2Pressed) {   //2人目のボタンが押された&感知
        p2Pressed = true;
        p2Time = elapsedTime;
    }

    if (elapsedTime >= 15000) {   // 15秒で強制終了
      break;
    }    

    if (p1Pressed && p2Pressed) {    // 両者押したら終了
      break;
    }
  }
}


//勝ち負けの判定
long p1Diff = abs((long)p1Time - 10000); //10秒からどのくらい離れているか
long p2Diff = abs((long)p2Time - 10000);

void resultScreen(){
  display.clearDisplay();
  display.setCursor(20,20);

  if(p1Diff < p2Diff){
    display.println("P1 WIN");

    display.print("P1:");
    display.print(p1Time);
    display.println("ms");
    
    display.print("P2:");
    display.print(p2Time);
    display.println(" ms");
  }

  else if(p1Diff > p2Diff){
    display.println("P2 WIN");

    display.print("P2:");
    display.print(p2Time);
    display.println(" ms");

    display.print("P1");
    display.print(p1Time);
    display.println(" ms");
  }

  else{
    display.println("DRAW");
  }

  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLEDが見つかりません");
    while (1);
  }

  display.clearDisplay();
}

void loop(){
  StartScreen();
  
  WaitTime();

  resultScreen();
}
