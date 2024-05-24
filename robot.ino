#include <PS2X_lib.h>
#include <Servo.h>

#define MOTORA_1 13  // IN3
#define MOTORA_2 12  // IN4
#define MOTORB_1 11  // IN1
#define MOTORB_2 10  // IN2

#define pin_Servo_1 5
#define pin_Servo_2 4
#define pin_Servo_3 3
#define pin_Servo_grip 2

#define laserPin A0

Servo Servo_1;
Servo Servo_2;
Servo Servo_3;
Servo Servo_grip;

PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;
int speed = 200;

bool isServo_grip_pressed = false;
bool isServo_3_pressed = false;
bool isServo_3 = false;
bool isServo_grip = false;
bool lastR2State = false;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 500;
int servoPosition_1 = 1400;
int step = 50;

int servoPosition_2 = 1400;

bool isLaserOn = false;
bool lastL2State = false;  // Trạng thái trước đó của nút L2

void setup() {
  Servo_1.attach(pin_Servo_1);
  Servo_1.writeMicroseconds(servoPosition_1);
  Servo_2.attach(pin_Servo_2);
  Servo_2.writeMicroseconds(servoPosition_2);
  Servo_3.attach(pin_Servo_3);
  Servo_3.write(180);
  Servo_grip.attach(pin_Servo_grip);
  Servo_grip.writeMicroseconds(500);

  pinMode(MOTORA_1, OUTPUT);
  pinMode(MOTORA_2, OUTPUT);
  pinMode(MOTORB_1, OUTPUT);
  pinMode(MOTORB_2, OUTPUT);

  pinMode(laserPin, OUTPUT);

  digitalWrite(MOTORA_1, LOW);
  digitalWrite(MOTORA_2, LOW);
  digitalWrite(MOTORB_1, LOW);
  digitalWrite(MOTORB_2, LOW);

  Serial.begin(57600);
  delay(300);
  error = ps2x.config_gamepad(6, 8, 7, 9, true, true);

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
  } else if (error == 1) {
    Serial.println("No controller found, check wiring or reset the Arduino");
  } else if (error == 2) {
    Serial.println("Controller found but not accepting commands");
  } else if (error == 3) {
    Serial.println("Controller refusing to enter Pressures mode, may not support it.");
  }

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
}


void ditrai() {
  digitalWrite(MOTORA_1, LOW);
  digitalWrite(MOTORA_2, speed);
  digitalWrite(MOTORB_1, speed);
  digitalWrite(MOTORB_2, LOW);
  Serial.println("ditrai");
}

void dithang() {
  digitalWrite(MOTORA_1, speed);
  digitalWrite(MOTORA_2, LOW);
  digitalWrite(MOTORB_1, speed);
  digitalWrite(MOTORB_2, LOW);
  Serial.println("di thang");
}

void dilui() {
  digitalWrite(MOTORA_1, LOW);
  digitalWrite(MOTORA_2, speed);
  digitalWrite(MOTORB_1, LOW);
  digitalWrite(MOTORB_2, speed);
  Serial.println("di lui");
}

void diphai() {
  digitalWrite(MOTORA_1, speed);
  digitalWrite(MOTORA_2, LOW);
  digitalWrite(MOTORB_1, LOW);
  digitalWrite(MOTORB_2, speed);
  Serial.println("diphai");
}

void stopMotors() {
  digitalWrite(MOTORA_1, LOW);
  digitalWrite(MOTORA_2, LOW);
  digitalWrite(MOTORB_1, LOW);
  digitalWrite(MOTORB_2, LOW);
}
void loop() {
  unsigned long currentMillis = millis();

  // Kiểm tra xem controller đã được cấu hình chưa
  if (error == 0) {
    ps2x.read_gamepad(false, vibrate);

    if (ps2x.Button(PSB_PAD_UP)) {
      dithang();
    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      diphai();
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {
      ditrai();
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      dilui();
    }

    //servo 1
    if (ps2x.Button(PSB_PINK)) {  // Nút PSB_PINK nhấn
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        servoPosition_1 += step;
        Servo_1.writeMicroseconds(servoPosition_1);
        Serial.println("trai");
        if (servoPosition_1 >= 2300 || servoPosition_1 <= 500) {
          step = -step;
        }
      }
    }
    if (ps2x.Button(PSB_RED)) {  // Nút PSB_RED nhấn
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        servoPosition_1 -= step;
        Servo_1.writeMicroseconds(servoPosition_1);
        Serial.println("phai");
        if (servoPosition_1 >= 2300 || servoPosition_1 <= 500) {
          step = -step;
        }
      }
    }

    //servo 2
    if (ps2x.Button(PSB_GREEN)) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        servoPosition_2 += step;
        Servo_2.writeMicroseconds(servoPosition_2);
        Serial.println("len");
        if (servoPosition_2 >= 2300 || servoPosition_2 <= 500) {
          step = -step;
        }
      }
    }
    if (ps2x.Button(PSB_BLUE)) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        servoPosition_2 -= step;
        Servo_2.writeMicroseconds(servoPosition_2);
        Serial.println("xuong");
        if (servoPosition_2 >= 2300 || servoPosition_2 <= 500) {
          step = -step;
        }
      }
    }
    //servo 3

    if (ps2x.Button(PSB_R1)) {
      if (!isServo_3_pressed) {
        if (!isServo_3) {
          Servo_3.write(95);
          isServo_3 = true;
          Serial.println("gap");
        } else {
          Servo_3.write(180);
          isServo_3 = false;
          Serial.println("nha");
        }
        isServo_3_pressed = true;
        delay(1000);
      }
    } else {
      isServo_3_pressed = false;
    }


    //gắp Servo_grip
    bool currentR2State = ps2x.Button(PSB_R2);

    // Kiểm tra xem nút đã được nhấn hay không
    if (currentR2State && !lastR2State) {
      // Đảo trạng thái của biến isServo_grip
      isServo_grip = !isServo_grip;

      // Gửi tín hiệu tới Servo tùy thuộc vào trạng thái của biến isServo_grip
      if (isServo_grip) {
        Servo_grip.writeMicroseconds(1400);
        Serial.println("nâng cơ");
      } else {
        Servo_grip.writeMicroseconds(500);
        Serial.println("hạ cơ");
      }
    }

    // Lưu trạng thái hiện tại của nút để so sánh trong lần lặp tiếp theo
    lastR2State = currentR2State;


    // if (ps2x.Button(PSB_R2)) {
    //   if (!isServo_grip_pressed) {
    //     if (!isServo_grip) {
    //       Servo_grip.write(90);
    //       isServo_grip = true;
    //       Serial.println("nang co");
    //     } else {
    //       Servo_grip.write(0);
    //       isServo_grip = false;
    //       Serial.println("ha co");
    //     }
    //     isServo_grip_pressed = true;
    //     delay(1000);
    //   }
    // } else {
    //   isServo_grip_pressed = false;
    // }
    // Control laser
    bool currentL2State = ps2x.Button(PSB_L2);
    if (currentL2State != lastL2State) {
      lastL2State = currentL2State;
      if (currentL2State) {
        isLaserOn = !isLaserOn;
        digitalWrite(laserPin, isLaserOn ? HIGH : LOW);
        Serial.println(isLaserOn ? "đèn bật" : "đèn tắt");
      }
    }

    if (!ps2x.Button(PSB_PAD_DOWN) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_RIGHT) && !ps2x.Button(PSB_PAD_LEFT)) {
      stopMotors();
    }

  } else {
    // Controller chưa được cấu hình, giữ chương trình ở trong vòng lặp setup
    setup();
  }

  delay(50);  // Thêm thời gian chờ giữa các lần đọc từ bộ điều khiển
}


// void loop() {
//   unsigned long currentMillis = millis();
//   if (error == 1)
//     return;
//   else {
//     ps2x.read_gamepad(false, vibrate);

//     if (ps2x.Button(PSB_PAD_UP)) {
//       dithang();
//     }
//     if (ps2x.Button(PSB_PAD_RIGHT)) {
//       diphai();
//     }
//     if (ps2x.Button(PSB_PAD_LEFT)) {
//       ditrai();
//     }
//     if (ps2x.Button(PSB_PAD_DOWN)) {
//       dilui();
//     }

//     //servo 1
//     if (ps2x.Button(PSB_PINK)) {  // Nút PSB_PINK nhấn
//       if (currentMillis - previousMillis >= interval) {
//         previousMillis = currentMillis;
//         servoPosition_1 += step;
//         Servo_1.writeMicroseconds(servoPosition_1);
//         Serial.println("trai");
//         if (servoPosition_1 >= 2300 || servoPosition_1 <= 500) {
//           step = -step;
//         }
//       }
//     }
//     if (ps2x.Button(PSB_RED)) {  // Nút PSB_RED nhấn
//       if (currentMillis - previousMillis >= interval) {
//         previousMillis = currentMillis;
//         servoPosition_1 -= step;
//         Servo_1.writeMicroseconds(servoPosition_1);
//         Serial.println("phai");
//         if (servoPosition_1 >= 2300 || servoPosition_1 <= 500) {
//           step = -step;
//         }
//       }
//     }

//     //servo 2
//     if (ps2x.Button(PSB_GREEN)) {
//       if (currentMillis - previousMillis >= interval) {
//         previousMillis = currentMillis;
//         servoPosition_2 += step;
//         Servo_2.writeMicroseconds(servoPosition_2);
//         Serial.println("len");
//         if (servoPosition_2 >= 2300 || servoPosition_2 <= 500) {
//           step = -step;
//         }
//       }
//     }
//     if (ps2x.Button(PSB_BLUE)) {
//       if (currentMillis - previousMillis >= interval) {
//         previousMillis = currentMillis;
//         servoPosition_2 -= step;
//         Servo_2.writeMicroseconds(servoPosition_2);
//         Serial.println("xuong");
//         if (servoPosition_2 >= 2300 || servoPosition_2 <= 500) {
//           step = -step;
//         }
//       }
//     }
//     //servo 3

//     if (ps2x.Button(PSB_R1)) {
//       if (!isServo_3_pressed) {
//         if (!isServo_3) {
//           Servo_3.write(95);
//           isServo_3 = true;
//           Serial.println("gap");
//         } else {
//           Servo_3.write(180);
//           isServo_3 = false;
//           Serial.println("nha");
//         }
//         isServo_3_pressed = true;
//         delay(1000);
//       }
//     } else {
//       isServo_3_pressed = false;
//     }


//     //gắp Servo_grip
//     bool currentR2State = ps2x.Button(PSB_R2);
//     if (currentR2State != lastR2State) {
//       lastR2State = currentR2State;
//       if (currentR2State) {
//         if (!isServo_grip_pressed) {
//           isServo_grip_pressed = true;  // Đánh dấu rằng nút đang được nhấn
//           if (!isServo_grip) {
//             Servo_grip.writeMicroseconds(1400);  // Chỉ cần gửi tín hiệu để servo giữ vị trí
//             isServo_grip = true;
//             Serial.println("nâng cơ");
//           } else {
//             Servo_grip.writeMicroseconds(1400);  // Chỉ cần gửi tín hiệu để servo giữ vị trí
//             // Không cần cập nhật biến isServo_grip
//             Serial.println("hạ cơ");
//           }
//         }
//       } else {
//         isServo_grip_pressed = false;  // Đánh dấu rằng nút không còn được nhấn
//       }
//     }


//     // if (ps2x.Button(PSB_R2)) {
//     //   if (!isServo_grip_pressed) {
//     //     if (!isServo_grip) {
//     //       Servo_grip.write(90);
//     //       isServo_grip = true;
//     //       Serial.println("nang co");
//     //     } else {
//     //       Servo_grip.write(0);
//     //       isServo_grip = false;
//     //       Serial.println("ha co");
//     //     }
//     //     isServo_grip_pressed = true;
//     //     delay(1000);
//     //   }
//     // } else {
//     //   isServo_grip_pressed = false;
//     // }
//     // Control laser
//     bool currentL2State = ps2x.Button(PSB_L2);
//     if (currentL2State != lastL2State) {
//       lastL2State = currentL2State;
//       if (currentL2State) {
//         isLaserOn = !isLaserOn;
//         digitalWrite(laserPin, isLaserOn ? HIGH : LOW);
//         Serial.println(isLaserOn ? "đèn bật" : "đèn tắt");
//       }
//     }

//     if (!ps2x.Button(PSB_PAD_DOWN) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_RIGHT) && !ps2x.Button(PSB_PAD_LEFT)) {
//       stopMotors();
//     }
//   }
//   delay(50);
// }
