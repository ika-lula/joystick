class InstantGame{

private:
  int direction;

  unsigned long startTime;
  unsigned long p1Time;
  unsigned long p2Time;

  bool p1Win;
  bool p2Win;

public:

  InstantGame(){
    p1Win = false;
    p2Win = false;
  }

  void readyScreen(){
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Ready...");   // 前の画面を消す

  display.display();  //メモリの内容をOLEDに送信

  delay(1000);
  }

  void waitRandom(){
  int wait_time;
  wait_time = random(2000, 5001); //2～5秒の間でランダムに
  delay(wait_time);
  }

  void showDirection(){
    direction = random(4);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(40, 20);

    switch(direction){
      case 0:
        display.println("UP");
        break;

      case 1:
        display.println("RIGHT");
        break;
      
      case 2:
        display.println("DOWN");
        break;
      
      case 3:
        display.println("LEFT");
        break;
    }

  display.display();
  startTime = millis();
  
  }

  bool checkDirection(){
    p1Win = false;
    p2Win = false;

    while(millis() - startTime < 5000){
      int p1 = getDirection(J1_X, J1_Y);
      int p2 = getDirection(J2_X, J2_Y);

      if(p1 == direction){
        p1Time = millis() - startTime;
        p1Win = true;

        return true;
      }

      if(p2 == direction){
        p2Time = millis() - startTime;
        p2Win = true;

        return true;
      }
    }
    return false;
  }   
  
  void resultScreen(){
    display.clearDisplay();
    display.setCursor(20,20);

    if(p1Win){
      display.println("P1 WIN");

      display.setCursor(20,40);
      display.print(p1Time);
      display.println(" ms");
    }

    else if(p2Win){
      display.println("P2 WIN");

      display.setCursor(20,40);
      display.print(p2Time);
      display.println(" ms");
    }

    else{
      display.println("TIME UP");
    }

    display.display();
    delay(2000);
  }

};