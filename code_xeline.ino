#include <digitalWriteFast.h>

int enbA = 9;
int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;
int enbB = 10;

int s1_previous = 0;
int s2_previous = 0;
int s3_previous = 0;
int s4_previous = 0;
int s5_previous = 0;
bool lost_line = false;
bool found_line = false;

// PID Constants
float Kp = 1.0;
float Ki = 0.0;
float Kd = 0.0;

// PID Variables
float P = 0.0;
float I = 0.0;
float D = 0.0;
float PID_value = 0.0;
float previous_error = 0.0;
float initial_motor_speed = 180; 

int error = 0;



void read_sensor_values();
void calculate_pid();
void motor_control();
void dithang();
void disangtrai();
void disangphai();
void dunglai();
void quaytron();
void dilui();

void setup() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enbA, LOW);
  digitalWrite(enbB, LOW);

  Serial.begin(9600);
}


void loop() {
  int s1 = digitalRead(A1);
  int s2 = digitalRead(A2);
  int s3 = digitalRead(A3);
  int s4 = digitalRead(A4);
  int s5 = digitalRead(A5);
  if (!found_line) {
    quaytron();
  } else {
    if ((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1)) {
      dunglai();
      return;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 1)) {
      error = 4;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1)) {
      error = 5;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1)) {
      error = 3;
      lost_line = false;
    } else if ((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0)) {
      error = -4;
      lost_line = false;
    } else if ((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0)) {
      error = -3;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1)) {
      error = 3;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 0)) {
      error = 2;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 0)) {
      error = 1;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 0) && (s5 == 0)) {
      error = 0;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0)) {
      error = -1;
      lost_line = false;
    } else if ((s1 == 0) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
      error = -2;
      lost_line = false;
    } else if ((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
      error = -3;
      lost_line = false;
    } else if ((s1 == 1) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
      error = -4;
      lost_line = false;
    } else {
      if (lost_line) {
        if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 0) && (s4_previous == 0) && (s5_previous == 1))
          disangtrai();
        else if ((s1_previous == 0) && (s2_previous == 1) && (s3_previous == 1) && (s4_previous == 1) && (s5_previous == 1))
          disangtrai();
        else if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 1) && (s4_previous == 1) && (s5_previous == 1))
          disangtrai();
        else if ((s1_previous == 1) && (s2_previous == 1) && (s3_previous == 1) && (s4_previous == 1) && (s5_previous == 0))
          disangphai();
        else if ((s1_previous == 1) && (s2_previous == 1) && (s3_previous == 1) && (s4_previous == 0) && (s5_previous == 0))
          disangphai();
        else if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 0) && (s4_previous == 1) && (s5_previous == 1))
          disangtrai();
        else if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 0) && (s4_previous == 1) && (s5_previous == 0))
          disangtrai();
        else if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 1) && (s4_previous == 1) && (s5_previous == 0))
          disangtrai();
        else if ((s1_previous == 0) && (s2_previous == 0) && (s3_previous == 1) && (s4_previous == 0) && (s5_previous == 0))
          dilui();
        else if ((s1_previous == 0) && (s2_previous == 1) && (s3_previous == 1) && (s4_previous == 0) && (s5_previous == 0))
          disangphai();
        else if ((s1_previous == 0) && (s2_previous == 1) && (s3_previous == 0) && (s4_previous == 0) && (s5_previous == 0))
          disangphai();
        else if ((s1_previous == 1) && (s2_previous == 1) && (s3_previous == 0) && (s4_previous == 0) && (s5_previous == 0))
          disangphai();
        else if ((s1_previous == 1) && (s2_previous == 0) && (s3_previous == 0) && (s4_previous == 0) && (s5_previous == 0))
          disangphai();
      }
    }
    s1_previous = s1;
    s2_previous = s2;
    s3_previous = s3;
    s4_previous = s4;
    s5_previous = s5;
    lost_line = true;

    //tính
    P = error;
    I += error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;

    int left_motor_speed = initial_motor_speed - PID_value;
    int right_motor_speed = initial_motor_speed + PID_value;

    left_motor_speed = constrain(left_motor_speed, 0, 200);
    right_motor_speed = constrain(right_motor_speed, 0, 200);

    analogWrite(enbB, right_motor_speed);
    analogWrite(enbA, left_motor_speed);

    if (PID_value > 0) {  // Turn right
      digitalWriteFast(in1, LOW);
      digitalWriteFast(in2, LOW);
      digitalWriteFast(in3, HIGH);
      digitalWriteFast(in4, LOW);
    } else if (PID_value < 0) {  // Turn left
      digitalWriteFast(in1, HIGH);
      digitalWriteFast(in2, LOW);
      digitalWriteFast(in3, LOW);
      digitalWriteFast(in4, LOW);
    } else {  // Go straight
      digitalWriteFast(in1, HIGH);
      digitalWriteFast(in2, LOW);
      digitalWriteFast(in3, HIGH);
      digitalWriteFast(in4, LOW);
    }
  }
}


void dithang() {
  digitalWriteFast(in1, HIGH);
  digitalWriteFast(in2, LOW);
  digitalWriteFast(in3, HIGH);
  digitalWriteFast(in4, LOW);
  analogWrite(enbA, 110);
  analogWrite(enbB, 110);
}
void dunglai() {
  digitalWriteFast(in1, LOW);
  digitalWriteFast(in2, HIGH);
  digitalWriteFast(in3, LOW);
  digitalWriteFast(in4, HIGH);
  delay(100); 
  digitalWrite(enbA, LOW);
  digitalWrite(enbB, LOW);
}
void disangtrai() {
  digitalWriteFast(in1, HIGH);
  digitalWriteFast(in2, LOW);
  digitalWriteFast(in3, LOW);
  digitalWriteFast(in4, LOW);
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
}

void disangphai() {
  digitalWriteFast(in1, LOW);
  digitalWriteFast(in2, LOW);
  digitalWriteFast(in3, HIGH);
  digitalWriteFast(in4, LOW);
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
}
void dilui() {
  digitalWriteFast(in1, LOW);
  digitalWriteFast(in2, HIGH);
  digitalWriteFast(in3, LOW);
  digitalWriteFast(in4, HIGH);
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
}

void quaytron() {
  digitalWriteFast(in1, HIGH);
  digitalWriteFast(in2, LOW);
  digitalWriteFast(in3, HIGH);
  digitalWriteFast(in4, LOW);
  analogWrite(enbA, 200);
  analogWrite(enbB, 200);

  int s1 = digitalRead(A1);
  int s2 = digitalRead(A2);
  int s3 = digitalRead(A3);
  int s4 = digitalRead(A4);
  int s5 = digitalRead(A5);

  if (s1 == 1 || s2 == 1 || s3 == 1 || s4 == 1 || s5 == 1) {
    found_line = true;
    dunglai();
  }
}