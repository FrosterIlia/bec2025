#include <Arduino.h>
#include "motor.h"
#include "sonic.h"
#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h"

#define CH_MINUS 69
#define CH 70
#define CH_PLUS 71
#define PREV 68
#define NEXT 64
#define PLAY_PAUSE 67
#define VOL_MINUS 7
#define VOL_PLUS 21 
#define EQ 9
#define ZERO 22
#define HUNDRED_PLUS 25
#define TWO_HUNDRED_PLUS 13
#define ONE 12
#define TWO 24
#define THREE 94
#define FOUR 8
#define FIVE 28 
#define SIX 90
#define SEVEN 66 
#define EIGHT 82
#define NINE 74

#define IR_RECEIVE_PIN 2

#define EN_A 5
#define EN_B 6

#define IN_1 12
#define IN_2 A0
#define IN_3 8
#define IN_4 7

#define TRIGGER A4
#define ECHO A5

// Motor motor_left(IN_2, IN_1, EN_A, AUTO);
// Motor motor_right(IN_4, IN_3, EN_B, AUTO);

// Sonic sonic(ECHO, TRIGGER);

int y = 255;
int x;

float distance = MAX_SONIC_DISTANCE;

const uint32_t CONTROL_PERIOD_MS = 60;
const uint32_t PLOT_PERIOD_MS = 100;

void setup()
{
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, 1);
}

void loop()
{

  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.command);

    delay(50);
    IrReceiver.resume();
  }

  // static unsigned long last_control_ms = 0;
  // if (millis() - last_control_ms >= CONTROL_PERIOD_MS)
  // {
  //   distance = sonic.get_distance();
  //   const int x = map(distance, MIN_SONIC_DISTANCE, MAX_SONIC_DISTANCE, -255, 255);

  //   int left = y + x;
  //   int right = y - x;

  //   motor_left.setSpeed(left);
  //   motor_right.setSpeed(right);

  //   last_control_ms = millis();
  // }

  // static unsigned long last_plot_ms = 0;
  // if (millis() - last_plot_ms >= PLOT_PERIOD_MS)
  // {
  //   Serial.print("{p(d:");
  //   Serial.print(static_cast<int>(distance));
  //   Serial.print(",s:");
  //   Serial.print(2);
  //   Serial.print(")}");
  //   last_plot_ms = millis();
  // }
}
