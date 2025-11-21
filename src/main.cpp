#include <Arduino.h>
#include "motor.h"
#include "sonic.h"

#define RECEIVER_PIN A2

#define EN_A 5
#define EN_B 6

#define IN_1 12
#define IN_2 A0
#define IN_3 8
#define IN_4 7

#define TRIGGER A4
#define ECHO A5

Motor motor_left(IN_2, IN_1, EN_A, AUTO);
Motor motor_right(IN_4, IN_3, EN_B, AUTO);

Sonic sonic(ECHO, TRIGGER);

int y = 255;
int x;

float distance = MAX_SONIC_DISTANCE;

const uint32_t CONTROL_PERIOD_MS = 60;
const uint32_t PLOT_PERIOD_MS = 100;

void setup()
{
  Serial.begin(115200);
  // pinMode(RECEIVER_PIN, INPUT);
}

void loop()
{

  static unsigned long last_control_ms = 0;
  if (millis() - last_control_ms >= CONTROL_PERIOD_MS)
  {
    distance = sonic.get_distance();
    const int x = map(distance, MIN_SONIC_DISTANCE, MAX_SONIC_DISTANCE, -255, 255);

    int left = y + x;
    int right = y - x;

    motor_left.setSpeed(left);
    motor_right.setSpeed(right);

    last_control_ms = millis();
  }

  static unsigned long last_plot_ms = 0;
  if (millis() - last_plot_ms >= PLOT_PERIOD_MS)
  {
    Serial.print("{p(d:");
    Serial.print(static_cast<int>(distance));
    Serial.print(",s:");
    Serial.print(2);
    Serial.print(")}");
    last_plot_ms = millis();
  }
}
