// ������� ����� ���� SoftwareSerial ���̺귯��
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Relay(D2���� ���)
int relay = 2;
//servo motor(������ �������� - D3��)
Servo windowservo;
// SoftwareSerial(RX, TX) - D4,5��
SoftwareSerial BTSerial(4, 5);
//servo motor(���� �������� - D6��)
Servo waterservo;
// DC Motor(D7���� ���)
int motor = 7;
// Set the LCD address to 0xF for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C LCD ��ü ����
//�����
int moodred = 8;
int moodgreen = 9;
int moodblue = 10;
//������ ��
int airred = 11;
int airgreen = 12;
int airblue = 13;


//Water sensor(�Ƴ��α� A0 �� ���)
int analogPin = 0;
int val = 0; //���ͼ����� �����ϴ� ���� �Էµ�

//â���� ���������� �����ִ����� ���� ���� 0�� �������� , 1�� ��������
int window = 0;
int water = 0;

void setup()
{
  //�ø���� ������� ��Ÿ� ����ȭ
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Welcome. Ready to Pair!");

  //lcd ��밡�� 16ĭ, 2���� �ǹ�
  lcd.init();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.noBacklight();
  
  //2������ ����ϴ� ������, 7������ ����ϴ� ����  �ƿ�ǲ ����
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); //ó���� ���������Ƿ� ���̷� ����
  
  //9������ ����ϴ� ���� ���Ϳ� 10������ ����ϴ� ������ ���� ����
  windowservo.attach(3);
  waterservo.attach(6);
  //�ʱ� ���� ���� ������ ��� ����ǵ� �ȿ�����
  windowservo.write(120);
  waterservo.write(20);

  //7������ ����ϴ� ����  �ƿ�ǲ ����
  pinMode(motor, OUTPUT);

  //������ ������ �� ���
  pinMode(moodblue,OUTPUT);
  pinMode(moodgreen,OUTPUT);
  pinMode(moodred,OUTPUT);
  
  pinMode(airblue,OUTPUT);
  pinMode(airgreen,OUTPUT);
  pinMode(airred,OUTPUT);
  //�ʱ⿣ �����ִ� ����
  digitalWrite(moodblue,HIGH);
  digitalWrite(moodgreen,HIGH);
  digitalWrite(moodred,HIGH);
  digitalWrite(airblue,HIGH);
  digitalWrite(airgreen,HIGH);
  digitalWrite(airred,HIGH);
}

void loop()
{
  // ��������� ���� ���ŵ� �����͸� �д´�.
  if (BTSerial.available()) {
    long y = BTSerial.parseInt();
    byte buf[20];
    Serial.print("recv: ");
    // ��������κ��� �����͸� �����Ѵ�.
    byte len = BTSerial.readBytes(buf, 20);
    // ���ŵ� �����͸� �ø��� ����Ϳ� ����Ѵ�.
    Serial.write(buf, len);
    Serial.println(y);

    if (y == 1) { //tv Ŵ
      lcd.setCursor(0, 0);
      lcd.print(" HEY! DISS!  ");
      lcd.backlight();
      Serial.println("TV turn on");
    } else if (y == 2) { //tv ��
      Serial.println("TV turn off");
      lcd.clear();
      lcd.noBacklight();
    } else if (y == 3) { //������ Ŵ
      Serial.println("Air conditioner turn on");
      digitalWrite(motor,HIGH);
      //�Ķ��� ����
      digitalWrite(airblue,LOW);
      digitalWrite(airgreen,LOW);
      digitalWrite(airred,HIGH);   
    } else if (y == 4) { //������ ����
      Serial.println("Air conditioner turn off");
      digitalWrite(motor,LOW);
      //�Ķ��� �ҵ�
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
    } else if (y == 5) { //�� ����
      digitalWrite(relay, HIGH);
      Serial.println("Light turn on");
    } else if (y == 6) { //�� ����
      digitalWrite(relay, LOW);
      Serial.println("Light turn off");
    } else if (y == 7 && window == 0) { //����ε� �÷���
       for (int i = 120; i > 55; i--) 
       {
        windowservo.write(i);
        delay(20);
      }
      window = 1;
      Serial.println("Blind turn on");
    } else if (y == 8 && window == 1) { //����ε� ������
      Serial.println("Blind turn off");
      for (int i = 55; i < 120; ++i) {
        windowservo.write(i);
        delay(20);
      }
      window = 0;
    } else if (y == 9 && water == 0) { //�� Ʋ����
      for (int i = 20; i < 70; i++) {
        waterservo.write(i);
        delay(30);
      }
      water = 1; //���� Ʋ���� ���·� ��ȯ
      Serial.println("Water turn on");
      Serial.println("Water Sensor On");
    } else if (y == 10 && water == 1) { //�� ����
      for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
      }
      water = 0; //���� ���� ���·� ��ȯ
      Serial.println("Water turn off");
    }else if (y == 11) { //�� ����
      //Ƽ�� ����
      lcd.clear();
      lcd.noBacklight();
      Serial.println("TV turn off");
      //������ ����
      digitalWrite(motor,LOW);
       //�Ķ��� �ҵ�
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
      Serial.println("Air conditioner turn off");
      //�� ����
      digitalWrite(relay, HIGH);
      Serial.println("Light turn off");
      //�� ����
      if(water == 1){ 
        for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
        }
        water = 0; //���� ���� ���·� ��ȯ
        Serial.println("Water turn off");
      }  
      //����� ����
      digitalWrite(moodblue,HIGH);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,HIGH);
      Serial.println("Mood Light off");
    }
    else if (y == 12){ //������
      //Ƽ�� ����
      lcd.clear();
      lcd.noBacklight();
      Serial.println("TV turn off");
      //������ ����
      digitalWrite(motor,LOW);
       //�Ķ��� �ҵ�
      digitalWrite(airblue,HIGH);
      digitalWrite(airgreen,HIGH);
      digitalWrite(airred,HIGH);
      Serial.println("Air conditioner turn off");
      //�� ����
      digitalWrite(relay, LOW);
      Serial.println("Light turn off");
      //�� ����
      if(water == 1){ 
        for (int i = 70; i > 20; --i) {
        waterservo.write(i);
        delay(30);
        }
        water = 0; //���� ���� ���·� ��ȯ
        Serial.println("Water turn off");
      }
      //����ε� ������
      Serial.println("Blind turn off");
      if(window == 1){
        for (int i = 55; i < 120; ++i) {
          windowservo.write(i);
          delay(20);
       }
       window = 0;
      }
      //����� ����
      digitalWrite(moodblue,LOW);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,LOW);   
      Serial.println("Mood Light on");
    }
    else if (y == 13){ //����� ����
      digitalWrite(moodblue,LOW);
      digitalWrite(moodgreen,HIGH);
      digitalWrite(moodred,LOW);
      Serial.println("Mood Light on");
    }
    else if (y == 14){ //����� ����
     digitalWrite(moodblue,HIGH);
     digitalWrite(moodgreen,HIGH);
     digitalWrite(moodred,HIGH);
     Serial.println("Mood Light off");
    }
    else {
    Serial.println("WRONG NUMBER");
    }
  }
  
    val = analogRead(analogPin); //���ͼ����� ���� �޾ƿ��� ����
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