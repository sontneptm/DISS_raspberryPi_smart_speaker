// 블루투스 통신을 위한 SoftwareSerial 라이브러리
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Relay(D2번핀 사용)
int relay = 2;
//servo motor(윈도우 서보모터 - D3핀)
Servo windowservo;
// SoftwareSerial(RX, TX) - D4,5핀
SoftwareSerial BTSerial(4, 5);
//servo motor(워터 서보모터 - D6핀)
Servo waterservo;
// DC Motor(D7번핀 사용)
int motor = 7;
// Set the LCD address to 0xF for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C LCD 객체 선언
//무드등
int moodred = 8;
int moodgreen = 9;
int moodblue = 10;
//에어컨 등
int airred = 11;
int airgreen = 12;
int airblue = 13;


//Water sensor(아날로그 A0 핀 사용)
int analogPin = 0;
int val = 0; //워터센서가 감지하는 값이 입력됨

//창문과 수도꼭지가 열려있는지에 대한 변수 0이 닫힌상태 , 1이 열린상태
int window = 0;
int water = 0;

void setup()
{
  //시리얼과 블루투스 통신망 동기화
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Welcome. Ready to Pair!");

  //lcd 사용가능 16칸, 2줄을 의미
  lcd.init();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.noBacklight();
  
  //2번핀을 사용하는 릴레이, 7번핀을 사용하는 모터  아웃풋 정의
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); //처음엔 켜져있으므로 하이로 설정
  
  //9번핀을 사용하는 워터 모터와 10번핀을 사용하는 윈도우 모터 정의
  windowservo.attach(3);
  waterservo.attach(6);
  //초기 각도 값을 설정해 줘야 연결되도 안움직임
  windowservo.write(120);
  waterservo.write(20);

  //7번핀을 사용하는 모터  아웃풋 정의
  pinMode(motor, OUTPUT);

  //무드등과 에어컨 등 출력
  pinMode(moodblue,OUTPUT);
  pinMode(moodgreen,OUTPUT);
  pinMode(moodred,OUTPUT);
  
  pinMode(airblue,OUTPUT);
  pinMode(airgreen,OUTPUT);
  pinMode(airred,OUTPUT);
  //초기엔 꺼져있는 상태
  digitalWrite(moodblue,HIGH);
  digitalWrite(moodgreen,HIGH);
  digitalWrite(moodred,HIGH);
  digitalWrite(airblue,HIGH);
  digitalWrite(airgreen,HIGH);
  digitalWrite(airred,HIGH);
}

void loop()
{
  // 블루투스로 부터 수신된 데이터를 읽는다.
  if (BTSerial.available()) {
    long y = BTSerial.parseInt();
    byte buf[20];
    Serial.print("recv: ");
    // 블루투스로부터 데이터를 수신한다.
    byte len = BTSerial.readBytes(buf, 20);
    // 수신된 데이터를 시리얼 모니터에 출력한다.
    Serial.write(buf, len);
    Serial.println(y);

    if (y == 1) { //tv 킴
      lcd.setCursor(0, 0);
      lcd.print(" HEY! DISS!  ");
      lcd.backlight();
      Serial.println("TV turn on");
    } else if (y == 2) { //tv 끔
      Serial.println("TV turn off");
      lcd.clear();
      lcd.noBacklight();
    } else if (y == 3) { //에어컨 킴
      Serial.println("Air conditioner turn on");
      digitalWrite(motor,HIGH);
      //파랑불 점등
      digitalWrite(airblue,LOW);
      digitalWrite(airgreen,LOW);
      digitalWrite(airred,HIGH);   
    } else if (y == 4) { //에어컨 꺼줘
      Serial.println("Air conditioner turn off");
      digitalWrite(motor,LOW);
      //파랑불 소등
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
    } else if (y == 5) { //불 켜줘
      digitalWrite(relay, HIGH);
      Serial.println("Light turn on");
    } else if (y == 6) { //불 꺼줘
      digitalWrite(relay, LOW);
      Serial.println("Light turn off");
    } else if (y == 7 && window == 0) { //블라인드 올려줘
       for (int i = 120; i > 55; i--) 
       {
        windowservo.write(i);
        delay(20);
      }
      window = 1;
      Serial.println("Blind turn on");
    } else if (y == 8 && window == 1) { //블라인드 내려줘
      Serial.println("Blind turn off");
      for (int i = 55; i < 120; ++i) {
        windowservo.write(i);
        delay(20);
      }
      window = 0;
    } else if (y == 9 && water == 0) { //물 틀어줘
      for (int i = 20; i < 70; i++) {
        waterservo.write(i);
        delay(30);
      }
      water = 1; //물이 틀어진 상태로 전환
      Serial.println("Water turn on");
      Serial.println("Water Sensor On");
    } else if (y == 10 && water == 1) { //물 꺼줘
      for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
      }
      water = 0; //물이 꺼진 상태로 전환
      Serial.println("Water turn off");
    }else if (y == 11) { //다 꺼줘
      //티비 꺼줘
      lcd.clear();
      lcd.noBacklight();
      Serial.println("TV turn off");
      //에어컨 꺼줘
      digitalWrite(motor,LOW);
       //파랑불 소등
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
      Serial.println("Air conditioner turn off");
      //불 꺼줘
      digitalWrite(relay, HIGH);
      Serial.println("Light turn off");
      //물 꺼줘
      if(water == 1){ 
        for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
        }
        water = 0; //물이 꺼진 상태로 전환
        Serial.println("Water turn off");
      }  
      //무드등 꺼줘
      digitalWrite(moodblue,HIGH);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,HIGH);
      Serial.println("Mood Light off");
    }
    else if (y == 12){ //수면모드
      //티비 꺼줘
      lcd.clear();
      lcd.noBacklight();
      Serial.println("TV turn off");
      //에어컨 꺼줘
      digitalWrite(motor,LOW);
       //파랑불 소등
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
      Serial.println("Air conditioner turn off");
      //불 꺼줘
      digitalWrite(relay, LOW);
      Serial.println("Light turn off");
      //물 꺼줘
      if(water == 1){ 
        for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
        }
        water = 0; //물이 꺼진 상태로 전환
        Serial.println("Water turn off");
      }
      //블라인드 내려줘
      Serial.println("Blind turn off");
      if(window == 1){
        for (int i = 55; i < 120; ++i) {
          windowservo.write(i);
          delay(20);
       }
       window = 0;
      }
      //무드등 켜줘
      digitalWrite(moodblue,LOW);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,LOW);   
      Serial.println("Mood Light on");
    }
    else if (y == 13){ //무드등 켜줘
      digitalWrite(moodblue,LOW);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,LOW);
      Serial.println("Mood Light on");
    }
    else if (y == 14){ //무드등 꺼줘
     digitalWrite(moodblue,HIGH);
     digitalWrite(moodgreen,HIGH);
     digitalWrite(moodred,HIGH);
     Serial.println("Mood Light off");
    }
    else {
    Serial.println("WRONG NUMBER");
    }
  }
  
    val = analogRead(analogPin); //워터센서의 값을 받아오는 변수
    if(water == 1)
       Serial.println(val);
    if (val > 400 && water == 1) { 
       Serial.println("TURN OFF");
       for (int i = 70; i >20; i--) {
         waterservo.write(i);
         delay(30);
       }
       Serial.println("Water full");
       water = 0;
    }
    delay(300);   
}