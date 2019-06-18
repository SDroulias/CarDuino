#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
// #include <NewPing.h>
#include <Servo.h>

int bluetoothTx = 13;
int bluetoothRx = 12;

int btData;

SoftwareSerial bluetooth(bluetoothRx, bluetoothTx);

int rightMotorPin1 = 7;
int rightMotorPin2 = 8;

int leftMotorPin1 = 9;
int leftMotorPin2 = 10;

Servo servo;

const int TrigPinF = A5;
const int EchoPinF = A4;

const int TrigPinR = 5;
const int EchoPinR = 4;

const int TrigPinL = 3;
const int EchoPinL = 2;

// #define SAFE 15

int distanceR;
int distanceF;
int distanceL;

Ultrasonic sensorF(TrigPinF, EchoPinF, 40000UL), sensorR(TrigPinR, EchoPinR, 40000UL), sensorL(TrigPinL, EchoPinL, 40000UL);
// NewPing sonar(TrigPinF,EchoPinF, 100);
bool forwardMoveAllowed;
int direction;

void MoveForward(){
  // if (forwardMoveAllowed == false){
  //   return;
  // }
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  // direction = 'F';
}

void MoveBackward(){
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
}

void servoRotateRight(){
  servo.write(155);
}

void servoRotateLeft(){
  servo.write(35);
}

void servoGoToCenter(){
  servo.write(90);
}

void MoveFR(){
  MoveForward();
  servoRotateRight();
}

void MoveFL(){
  MoveForward();
  servoRotateLeft();
}

void MoveBR(){
  MoveBackward();
  servoRotateRight();
}

void MoveBL(){
  MoveBackward();
  servoRotateLeft();
}

// int sScan(Ultrasonic sensor){
//   sensor.read();
//   delay(50);
//   Serial.println("Reads ");
//   Serial.print(sensor.read());
//   Serial.println();
//   return sensor.read();
// }

void stop() {
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  servoGoToCenter();
}

void autoRight() {
  MoveForward();
  servoRotateRight();
  delay(1000);
  stop();
}

void autoLeft() {
  MoveForward();
  servoRotateLeft();
  delay(1000);
  stop();
}

void setup() {

  servo.attach(11);
  bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  delay(100);

}

void loop() {
  Serial.println(sensorF.read());
  if (bluetooth.available()){
    btData = bluetooth.read();
  }
  if (btData=='0'){
    stop();
  }
  else if (btData=='1') {
    delay(100);
    if (sensorF.read() <=15)
    {
      delay(100);
      Serial.println(sensorF.read());
      stop();
      if (sensorR.read() >= sensorL.read()) {
        delay(100);
        autoRight();
        delay(100);
      }else
      {
        delay(100);
        autoLeft();
        delay(100);
      }    
    }else
    {
      MoveForward();
    }
  }
  else if(btData=='4') {
    MoveBackward();
  }
  else if(btData=='7') {
    servoRotateRight();
  }
  else if (btData=='8') {
    servoRotateLeft();
  }
  else if (btData=='2') {
    servoRotateRight();
    MoveForward();
  }
  else if (btData=='3') {
    servoRotateLeft();
    MoveForward();
  }
  else if (btData=='5') {
    servoRotateRight();
    MoveBackward();
  }
  else if (btData=='6') {
    servoRotateLeft();
    MoveBackward();
  }
}