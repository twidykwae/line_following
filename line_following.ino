//www.elegoo.com
//2016.09.23
// COS 280 starter kit for Elegoo Smart Car V4, modified 2024.02.05 by Joey Gorski, 2025.03.04 by Stefan Brandle

#include <Servo.h>

//----------------------------------------------
// Ultrasonic sensor stuff
//----------------------------------------------
Servo myservo;
const int PIN_Trig = 13;
const int PIN_Echo = 12;
const int PIN_Servo = 10;

//----------------------------------------------
// Motor pins
//----------------------------------------------
const int PIN_Motor_PWMA = 5;
const int PIN_Motor_PWMB = 6;
const int PIN_Motor_BIN_1 = 8;
const int PIN_Motor_AIN_1 = 7;
const int PIN_Motor_STBY = 3;

//----------------------------------------------
// Line sensor pins
//----------------------------------------------
const int LINE_SENSE_L = A2;
const int LINE_SENSE_M = A1;
const int LINE_SENSE_R = A0;

//----------------------------------------------
// Variables to hold the line sensor values.
//----------------------------------------------
int line_L, line_M, line_R;

//----------------------------------------------

//----------------------------------------------
const int default_speed = 60;

bool motor_on = true;

const int LINE_THRESHOLD = 200;

int last_ping = 0;

void setup() {
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);

  pinMode(LINE_SENSE_L, INPUT);
  pinMode(LINE_SENSE_M, INPUT);
  pinMode(LINE_SENSE_R, INPUT);

  pinMode(PIN_Echo, INPUT);
  pinMode(PIN_Trig, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setup done");
}

void loop() {

  line_L = analogRead(LINE_SENSE_L);
  line_M = analogRead(LINE_SENSE_M);
  line_R = analogRead(LINE_SENSE_R);

  bool L = isonLine(line_L);
  bool M = isonLine(line_M);
  bool R = isonLine(line_R);

  // Useful for debugging line sensor input
  Serial.print("L: ");
  Serial.print(line_L); 
  Serial.print(", M: ");
  Serial.print(line_M);
  Serial.print(", R: ");
  Serial.print(line_R);
  Serial.println();
  
  int ping_dist = ultrasonic_ping();
  
  Serial.print("Ultrasound response: ");
  Serial.println(ping_dist);
  delay(10);

  if (L && M && R) {        
    try_forward(ping_dist, last_ping);
  }
  else if (L && !M && R) {  
    try_forward(ping_dist, last_ping);
  }
  else if (M && !L && !R) {
    try_forward(ping_dist, last_ping);
  }
  else if (L && !M && !R) {
    hardLeft();
  }
  else if (L && M && !R) {
    slightLeft();
  }
  else if (R && !M && !L) {
    hardRight();
  }
  else if (R && M && !L) {
    slightRight();
  }
  else if (!L && !M && !R) {
    left(); // search
  }

  last_ping = ping_dist;
}
bool isonLine(int val) {
  return val > LINE_THRESHOLD;
}

void try_forward(int ping_dist, int last_ping) {
  if(ping_dist < 20 && last_ping < 20) {
    halt();
  } else if (ping_dist > 50 && last_ping > 50) {
    forward();
  }
}

void forward() {
  Serial.println("go forward!");
  if (!motor_on) return;

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, default_speed);
  analogWrite(PIN_Motor_PWMB, default_speed);
}

void back() {
  Serial.println("go back!");
  if (!motor_on) return;

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, default_speed);
  analogWrite(PIN_Motor_PWMB, default_speed);
}

void left() {
  Serial.println("go left!");
  if (!motor_on) return;

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, default_speed);
  analogWrite(PIN_Motor_PWMB, default_speed);
}

void right() {
  Serial.println("go right!");
  if (!motor_on) return;

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, default_speed);
  analogWrite(PIN_Motor_PWMB, default_speed);
}

void halt() {
  Serial.println("Stop!");
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
  digitalWrite(PIN_Motor_STBY, LOW);
}
void slightLeft() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 40);
  analogWrite(PIN_Motor_PWMB, 60);
}

void slightRight() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 60);
  analogWrite(PIN_Motor_PWMB, 40);
}

void hardLeft() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 60);
  analogWrite(PIN_Motor_PWMB, 0);
}

void hardRight() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 60);
}

unsigned int ultrasonic_ping() {
  digitalWrite(PIN_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_Trig, LOW);
  return ((unsigned int)pulseIn(PIN_Echo, HIGH) / 58);
}
