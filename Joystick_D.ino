#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //スクリーンの大きさ（横）
#define SCREEN_HEIGHT 64 //スクリーンの大きさ（縦）

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int VRX = A0; //ジョイスティックとArduino Unoの接続先を示している
const int VRY = A1;
const int SW = 2;

int x = 64;
int y = 32;
int speed = 1;

bool lastpressed = false;

class Joystick { 
  private: 
    int xPin, yPin; 
    int paintX[10];
    int paintY[10];
    int paintindex;
  public: 
    Joystick(int x, int y){
      xPin = x;
      yPin = y;
      paintindex = 0;
      for (int & p : paintX){
        p = -1;
      }
      for (int & p : paintY){
        p = -1;
      }      
    }

    int getX(){
      int joyX = analogRead(xPin);
      if (joyX < 400)
        return -1;
      if (joyX > 600)
        return 1;
      return 0;
      }; 
    int getY(){
      int joyY = analogRead(yPin);
      if (joyY < 400)
        return -1;
      if (joyY > 600)
        return 1;
      return 0;
    };

    void paint_save(int X_paint,int Y_paint){
      paintX[paintindex] = X_paint;
      paintY[paintindex] = Y_paint;
      paintindex += 1;
    } 

    void paint_display(){
      for (int i = 0; i < paintindex; i++){
        display.fillCircle(paintX[i], paintY[i], 2, SSD1306_WHITE);
      }
    }

    void speed_change(){
      speed += 1;
    }
}; 

class Cursor { 
  private: 
    int x, y; 
    int minX, maxX, minY, maxY; 
  public: 
    Cursor(int startX, int startY, int minX_C, int maxX_C, int minY_C, int maxY_C){
      x = startX;
      y = startY;
      minX = minX_C;
      maxX = maxX_C;
      minY = minY_C;
      maxY = maxY_C;
    }
    void move(int* x, int* y, int dx, int dy){
      *x += dx;
      *y += dy;
      *x = constrain(*x, minX, maxX); //画面外にいかないように
      *y = constrain(*y, minY, maxY); 
      display.drawLine(*x-2, *y, *x+2, *y ,SSD1306_WHITE);
      display.drawLine(*x, *y-2, *x, *y+2 ,SSD1306_WHITE);        
      // display.fillCircle(*x, *y, 2, SSD1306_WHITE);
    }   
}; 

void setup() {
  Serial.begin(9600);

  pinMode(SW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLEDが見つかりません");
    while (1);
  }

  display.clearDisplay();
}

Joystick _Joystick(VRX,VRY);
Cursor _Cursor(x,y,0,127,0,63);

void loop() {

  // int joyX = analogRead(VRX);
  // int joyY = analogRead(VRY);いりませんでした。

  // x += _Joystick.getX();
  // y += _Joystick.getY();

  display.clearDisplay();

  _Cursor.move(&x,&y,_Joystick.getX()*speed,_Joystick.getY()*speed);

  bool ispressed = (digitalRead(SW) == LOW);
  if (ispressed == true && lastpressed == false){
    _Joystick.paint_save(x,y);
    _Joystick.speed_change();
  }

  lastpressed = ispressed;
  _Joystick.paint_display();

  // OLED
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("x=");
  display.print(x);

  display.setCursor(0,10);
  display.print("y=");
  display.print(y);

  display.display();

  // シリアルモニタ
  Serial.print("x=");
  Serial.print(x);
  Serial.print("  y=");
  Serial.println(y);

  delay(50);
}
