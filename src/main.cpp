#include <Arduino.h>
#include "motor.h"
#include "sonic.h"
#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h"
#include <Servo.h>
#include "irRemote.h"

#define IR_RECEIVE_PIN 2

#define EN_A 6
#define EN_B 5

#define IN_1 12
#define IN_2 11
#define IN_3 10
#define IN_4 9

#define TRIGGER_1 A0
#define ECHO_1 A1
#define TRIGGER_2 A2
#define ECHO_2 A3
#define TRIGGER_3 A4
#define ECHO_3 A5

#define SERVO_BACK_PIN 7
#define SERVO_FRONT_PIN 8

#define RED_LED_PIN 3
#define GREEN_LED_PIN 4

#define MAX_SPEED_FORWARD 150
#define MAX_SPEED_BACKWARD -150

#define MANUAL_MODE 0
#define AUTO_MODE 1

#define SERVO_FRONT_RIGHT_ANGLE 85
#define SERVO_FRONT_FORWARD_ANGLE 70
#define SERVO_FRONT_LEFT_ANGLE 40

#define SERVO_BACK_RIGHT_ANGLE 160
#define SERVO_BACK_FORWARD_ANGLE 140
#define SERVO_BACK_LEFT_ANGLE 120

#define SONIC_THRESHOLD 15 // cm

Motor motor_left(IN_1, IN_2, EN_A, AUTO);
Motor motor_right(IN_3, IN_4, EN_B, AUTO);

Sonic sonic_1(ECHO_1, TRIGGER_1);
Sonic sonic_2(ECHO_2, TRIGGER_2);
Sonic sonic_3(ECHO_3, TRIGGER_3);

Servo servo_front;
Servo servo_back;

int servo_front_pos = SERVO_FRONT_FORWARD_ANGLE;
int servo_back_pos = SERVO_BACK_FORWARD_ANGLE;

void manual_mode(uint8_t ir_command);
void auto_mode(uint8_t ir_command);

int y = 255;
int x;

float distance = MAX_SONIC_DISTANCE;

const uint32_t CONTROL_PERIOD_MS = 60;
const uint32_t PLOT_PERIOD_MS = 100;

void setup()
{
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, 1);

  motor_left.setMode(AUTO);
  motor_right.setMode(AUTO);

  motor_left.setDirection(1);
  motor_right.setDirection(1);

  servo_back.attach(SERVO_BACK_PIN);
  servo_front.attach(SERVO_FRONT_PIN);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

}

uint8_t mode = MANUAL_MODE;

uint8_t is_obstacle = false;

void loop()
{

  if (IrReceiver.decode())
  {
    switch (IrReceiver.decodedIRData.command)
    {
    case ONE:
      mode = MANUAL_MODE;
      break;

    case TWO:
      mode = AUTO_MODE;
      break;

    default:
    {
      switch (mode)
      {
      case MANUAL_MODE:
        manual_mode(IrReceiver.decodedIRData.command);
        break;

      case AUTO_MODE:
        auto_mode(IrReceiver.decodedIRData.command);

        break;
      }
    }
    }
  }

  servo_front.write(servo_front_pos);
  servo_back.write(servo_back_pos);
  // Serial.println(servo_front_pos);

  float distance_1 = sonic_1.get_distance();
  float distance_2 = sonic_2.get_distance();
  float distance_3 = sonic_3.get_distance();

  if (distance_1 < SONIC_THRESHOLD || distance_2 < SONIC_THRESHOLD || distance_3 < SONIC_THRESHOLD)
  {
    is_obstacle = true;
  }
  else
  {
    is_obstacle = false;
  }

  digitalWrite(RED_LED_PIN, is_obstacle);


  // static unsigned long last_plot_ms = 0;
  // if (millis() - last_plot_ms >= PLOT_PERIOD_MS)
  // {
  //   Serial.print("{p(d1:");
  //   Serial.print(sonic_1.get_distance());
  //   Serial.print(",d2:");
  //   Serial.print(sonic_2.get_distance());
  //   Serial.print(",d3:");
  //   Serial.print(sonic_3.get_distance());
  //   Serial.print(")}");
  //   last_plot_ms = millis();
  // }
}

void manual_mode(uint8_t ir_command)
{

  switch (ir_command)
  {
  case CH: // Forward
    Serial.println("forward");
    motor_right.setSpeed(MAX_SPEED_FORWARD);
    motor_left.setSpeed(MAX_SPEED_FORWARD);
    digitalWrite(GREEN_LED_PIN, 1);
    break;

  case VOL_PLUS: // Backwards
    Serial.println("back");
    motor_right.setSpeed(MAX_SPEED_BACKWARD);
    motor_left.setSpeed(MAX_SPEED_BACKWARD);
    digitalWrite(GREEN_LED_PIN, 1);
    break;

  case PREV: // Left
    Serial.println("left");
    servo_front_pos = SERVO_FRONT_LEFT_ANGLE;
    servo_back_pos = SERVO_BACK_RIGHT_ANGLE;
    break;

  case PLAY_PAUSE: // Right
    Serial.println("right");
    servo_front_pos = SERVO_FRONT_RIGHT_ANGLE;
    servo_back_pos = SERVO_BACK_LEFT_ANGLE;
    break;

  case HUNDRED_PLUS: // Left
    Serial.println("straight");
    servo_front_pos = SERVO_FRONT_FORWARD_ANGLE;
    servo_back_pos = SERVO_BACK_FORWARD_ANGLE;
    break;

  case NEXT: // stop
    Serial.println("stop");
    motor_right.setSpeed(0);
    motor_left.setSpeed(0);
    digitalWrite(GREEN_LED_PIN, 0);
    break;
  }

  delay(50);
  IrReceiver.resume();
}

void auto_mode(uint8_t ir_command)
{
}