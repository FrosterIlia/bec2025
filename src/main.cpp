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

#define SERVO_LEFT_PIN
#define SERVO_RIGHT_PIN
#define SERVO_FRONT_PIN

#define RED_LED_PIN 3
#define GREEN_LED_PIN 4

#define MAX_SPEED_FORWARD 150
#define MAX_SPEED_BACKWARD -150

Motor motor_left(IN_1, IN_2, EN_A, AUTO);
Motor motor_right(IN_3, IN_4, EN_B, AUTO);

Sonic sonic_1(ECHO_1, TRIGGER_1);
Sonic sonic_2(ECHO_2, TRIGGER_2);
Sonic sonic_3(ECHO_3, TRIGGER_3);

Servo servo_left(SERVO_LEFT_PIN);
Servo servo_front(SERVO_RIGHT_PIN);
Servo servo_right(SERVO_FRONT_PIN);

void manual_mode();

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
}

void loop()
{

  manual_mode();

  // motor_left.setSpeed(MAX_SPEED_FORWARD);
  // motor_right.setSpeed(MAX_SPEED_FORWARD);

  static unsigned long last_control_ms = 0;
  static unsigned long last_plot_ms = 0;
  // if (millis() - last_control_ms >= CONTROL_PERIOD_MS)
  // {
  //   distance = sonic.get_distance();
  //   const int x = map(distance, MIN_SONIC_DISTANCE, MAX_SONIC_DISTANCE, -255, 255);

  //   int left = y + x;
  //   int right = y - x;

  //   // motor_left.setSpeed(left);
  //   // motor_right.setSpeed(right);

  //   last_control_ms = millis();
  // }

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

void manual_mode()
{
  if (IrReceiver.decode())
  {
    switch (IrReceiver.decodedIRData.command)
    {
    case CH: // Forward
      Serial.println("forward");
      motor_right.setSpeed(MAX_SPEED_FORWARD);
      motor_left.setSpeed(MAX_SPEED_FORWARD);
      break;

    case VOL_PLUS: // Backwards
      Serial.println("back");
      motor_right.setSpeed(MAX_SPEED_BACKWARD);
      motor_left.setSpeed(MAX_SPEED_BACKWARD);
      break;

    case PREV: // Left
      Serial.println("left");
      motor_right.setSpeed(MAX_SPEED_FORWARD);
      motor_left.setSpeed(MAX_SPEED_BACKWARD);
      break;

    case PLAY_PAUSE: // Right
      Serial.println("right");
      motor_right.setSpeed(MAX_SPEED_BACKWARD);
      motor_left.setSpeed(MAX_SPEED_FORWARD);
      break;

    case NEXT: // stop
      Serial.println("stop");
      motor_right.setSpeed(0);
      motor_left.setSpeed(0);
      break;
    }

    delay(50);
    IrReceiver.resume();
  }
}