#include <Arduino.h>

#define ARM1 26
#define ARM2 25

void initArm() {
  pinMode(ARM1, INPUT_PULLUP);
  pinMode(ARM2, INPUT_PULLUP);
}

boolean isArmPulled() {
  int arm1State = digitalRead(ARM1);
  int arm2State = digitalRead(ARM2);
  return arm1State == LOW && arm2State == HIGH;
}