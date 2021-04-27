#include "Enes100.h"
using namespace std;
#include "NewPing.h"
#include "HX711.h"  

//const int Plastic = 1;
const int LOADCELL_DOUT_PIN =  A4;
const int LOADCELL_SCK_PIN =  A5;
const int motPinR1 = 4;
const int motPinR2 = 5;
const int motPinL1 = 6;
const int motPinL2 = 7;
const int motPinP1 = 2;
const int motPinP2 = 3;
const int osvSpeed = 11;
const int basSpeed = 10;
#define TPinR A1
#define EPinR A0
#define TPinL 12
#define EPinL 13
#define MAX_DIST 400

HX711 scale;
NewPing leftUS(TPinL, EPinL, MAX_DIST);
NewPing rightUS(TPinR, EPinR, MAX_DIST);
float distanceL, distanceR, durationL, durationR;
float dL, dR;

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Enes100.begin("Materials", DEBRIS, 6, 8, 9);
  pinMode(motPinL1, OUTPUT);
  pinMode(motPinL2, OUTPUT);
  pinMode(motPinR1, OUTPUT);
  pinMode(motPinR2, OUTPUT);
  pinMode(osvSpeed, OUTPUT);
  pinMode(basSpeed, OUTPUT);
  Serial.begin(9600);
  durationL = leftUS.ping();
  distanceL = (durationL / 2) * 0.0343;
  dL = distanceL;
  durationR = rightUS.ping();
  distanceR = (durationR / 2) * 0.0343;
  dR = distanceR;
  Enes100.updateLocation();
  boolean x = false;
  boolean y = false;
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Press T to tare");
  scale.set_scale(852120.00);
  scale.tare();
  pinMode(A2, OUTPUT);
  pinMode(A3, INPUT_PULLUP);
}

void moveForward() {
  digitalWrite(motPinL1, HIGH);
  digitalWrite(motPinL2, LOW);
  digitalWrite(motPinR1, LOW);
  digitalWrite(motPinR2, HIGH);
  digitalWrite(osvSpeed, 255);
}

void moveBackward() {
  digitalWrite(motPinL1, LOW);
  digitalWrite(motPinL2, HIGH);
  digitalWrite(motPinR1, HIGH);
  digitalWrite(motPinR2, LOW);
  digitalWrite(osvSpeed, 128);
}

void turnLeft(){
  digitalWrite(motPinL1, LOW);
  digitalWrite(motPinL2, HIGH);
  digitalWrite(motPinR1, LOW);
  digitalWrite(motPinR2, HIGH);
  digitalWrite(osvSpeed, 50);  
}

void turnRight(){
  digitalWrite(motPinL1, HIGH);
  digitalWrite(motPinL2, LOW);
  digitalWrite(motPinR1, HIGH);
  digitalWrite(motPinR2, LOW);
  digitalWrite(osvSpeed, 50);
}

void stopOSV() {
  digitalWrite(motPinL1, LOW);
  digitalWrite(motPinL2, LOW);
  digitalWrite(motPinR1, LOW);
  digitalWrite(motPinR2, LOW);
}

void rpUp(){
  digitalWrite(motPinP1, LOW);
  digitalWrite(motPinP2, HIGH);
  digitalWrite(basSpeed, 128);
}

void rpDown(){
  digitalWrite(motPinP1, HIGH);
  analogWrite(motPinP2, LOW);
  digitalWrite(basSpeed, 128);
}

void rpStop() {  
  digitalWrite(motPinP2, LOW);
  digitalWrite(motPinP1, LOW);
}

void readSensor() {
  durationL = leftUS.ping();
  distanceL = (durationL / 2) * 0.0343;
  dL = distanceL;
  durationR = rightUS.ping();
  distanceR = (durationR / 2) * 0.0343;
  dR = distanceR;
}

void passRockOnLeft() {
  while(Enes100.location.theta < 1.57*.5  /*60 degrees*/ ) {
    turnLeft();
    //Enes100.updateLocation();
    printStatement();
  }
  float yInit = Enes100.location.y;
  while(Enes100.location.y < yInit + 0.5) {
    moveForward();
    printStatement();
  }
  align();
  stopOSV();
}

void passRockOnRight() {
  while(Enes100.location.theta > -1.57*.5) {
    turnRight();
    //Enes100.updateLocation();
    printStatement();
  }
  float yInit = Enes100.location.y;
  while(Enes100.location.y > yInit - 0.5) {
    moveForward();
    printStatement();
  }
  align();
  stopOSV();
}

void passRockOnRightAlt() {
  turnRight();
  delay(500);
  moveForward();
  delay(2500);
  turnLeft();
  delay(450);
  stopOSV();
}

void passRockOnLeftAlt() {
  turnLeft();
  delay(500);
  moveForward();
  delay(2500);
  turnRight();
  delay(450);
  stopOSV();
}

void dig() {
  rpDown();
  delay(500);
  rpStop();
  shimmy();
  delay(500);
  stopOSV();
}

void shimmy() {
  for(int i = 0; i < 10; i++) {
    digitalWrite(motPinL1, HIGH);
    digitalWrite(motPinL2, LOW);
    digitalWrite(motPinR1, HIGH);
    digitalWrite(motPinR2, LOW);
    delay(100);
    digitalWrite(motPinL1, LOW);
    digitalWrite(motPinL2, HIGH);
    digitalWrite(motPinR1, LOW);
    digitalWrite(motPinR2, HIGH);
    delay(100);
  }
  stopOSV();
}

void liftBall() {
  
  dig();
  stopOSV();
  rpStop();
  delay(1000);
  shimmy();
  stopOSV();
  delay(3000);
  moveForward();
  delay(500);
  stopOSV();
  delay(2000);
  rpUp();
  delay(600);
  rpStop();
}

void celebrate() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(motPinL1, HIGH);
    digitalWrite(motPinL2, LOW);
    digitalWrite(motPinR1, HIGH);
    digitalWrite(motPinR2, LOW);
    digitalWrite(motPinP1, LOW);
    digitalWrite(motPinP2, HIGH);
    delay(100);
    digitalWrite(motPinL1, LOW);
    digitalWrite(motPinL2, HIGH);
    digitalWrite(motPinR1, LOW);
    digitalWrite(motPinR2, HIGH);
    digitalWrite(motPinP1, HIGH);
    digitalWrite(motPinP2, LOW);
    delay(100);
  }
}

void faceForward() {
  while(Enes100.location.theta < -0.2) {
    turnLeft();
    Enes100.updateLocation();
  }
  stopOSV();
  while(Enes100.location.theta > 0.2) {
    turnRight();
    Enes100.updateLocation();
  }
  stopOSV();
}

void printStatement() {
  readSensor();
  Enes100.updateLocation();
  Enes100.print(Enes100.location.x);
  Enes100.print(" ");
  Enes100.print(Enes100.location.y);
  Enes100.print(" ");
  Enes100.print(Enes100.location.theta);
  Enes100.print(" ");
  Enes100.print(dR);
  Enes100.print(" ");
  Enes100.println(dL);
}

void align() {
  if(Enes100.location.theta < -0.15) {
    while(Enes100.location.theta < -0.15) {
      turnLeft();
      printStatement();
    }
    stopOSV();
  }
  if(Enes100.location.theta > 0.15) {
    while(Enes100.location.theta > 0.15) {
      turnRight();
      printStatement();
      stopOSV();
      delay(50);
    }
    stopOSV();
  }
  stopOSV();
}

void cleanUpdate() {
  while((Enes100.location.x == 0 && Enes100.location.y == 0 && Enes100.location.theta == 0) || 
  Enes100.location.x > 4 || Enes100.location.x < 0 || Enes100.location.y > 2 || 
  Enes100.location.y < 0 || Enes100.location.theta > 3.14 || Enes100.location.theta < -3.14) {
    //Enes100.updateLocation();
    printStatement();
    stopOSV();
  }
}

void mass() {
  float weight = scale.get_units() * 1000;
  Serial.println(weight);
  Enes100.println(weight);
  Enes100.mission(weight);
}

void material() {
  boolean Copper = false;
  boolean Plastic = false;
  if(digitalRead(A2)==HIGH){
    digitalWrite(A3, HIGH);
    Copper = true;
    Plastic = false;
  }else{
    digitalWrite(A2, LOW);
    Copper = false;
    Plastic = true;
  }
  reader (digitalRead(A3));
}

void reader(int number) {
  if(number > 0) {
    Serial.println("Plastic");}
    else { Serial.println("Copper");}
  }

void loop() {
  // put your main code here, to run repeatedly:
  /*printStatement();
  cleanUpdate();
  align();
  delay(1000);
  readSensor();
  while(Enes100.location.x < 3){
    while(dL > 30 && dR > 30) {
      moveForward();
      //readSensor();
      //Enes100.updateLocation();
      printStatement();
      if(Enes100.location.x >= 3) {
        stopOSV();
        break;
      }
    }
    stopOSV();
    delay(500);
    if(Enes100.location.y > 1) {
      passRockOnRight();
      //Enes100.updateLocation();
      printStatement();
    } else {
      passRockOnLeft();
      //Enes100.updateLocation();
      printStatement();
    }
    stopOSV();
    printStatement();
  }
  stopOSV();
  align();
  */
  
  if(Enes100.destination.y >= 1 && Enes100.location.y > 1) {
    while(Enes100.location.theta < 1.57) {
      turnLeft();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y < 1.7) {
      moveForward();
      printStatement();
    }
    stopOSV();
    align();
    while(Enes100.location.x < Enes100.destination.x - 0.03) {
      moveForward(); 
      printStatement();
    }
    stopOSV();
    while(Enes100.location.theta > -1.57) {
      turnRight();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y > 0.1) {
      moveForward();
      printStatement();
    }
    stopOSV();
  }

  else if(Enes100.destination.y >= 1 && Enes100.location.y < 1) {
    while(Enes100.location.x < Enes100.destination.x - 0.03) {
      moveForward(); 
      printStatement();
    }
    stopOSV();
    while(Enes100.location.theta < 1.57) {
      turnLeft();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y < 1.9) {
      moveForward();
      printStatement();
    }
    stopOSV();
  }

  else if(Enes100.destination.y < 1 && Enes100.location.y > 1) {
    while(Enes100.location.x < Enes100.destination.x - 0.03) {
      moveForward(); 
      printStatement();
    }
    stopOSV();
    while(Enes100.location.theta > -1.57) {
      turnRight();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y > 0.1) {
      moveForward();
      printStatement();
    }
    stopOSV();
  }

  else if(Enes100.destination.y < 1 && Enes100.location.y < 1) {
    while(Enes100.location.theta > -1.57) {
      turnRight();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y > 0.3) {
      moveForward();
      printStatement();
    }
    stopOSV();
    align();
    while(Enes100.location.x < Enes100.destination.x - 0.03) {
      moveForward(); 
      printStatement();
    }
    stopOSV();
    while(Enes100.location.theta < 1.57) {
      turnLeft();
      printStatement();
    }
    stopOSV();
    while(Enes100.location.y < 1.9) {
      moveForward();
      printStatement();
    }
    stopOSV();
  }

  rpUp();
  delay(400);
  mass();
  material();
  
  delay(300000);
}
