#include <Arduino.h>

void blink(int count, int freq) {
  for (int i = 0; i < count; i++) {
    digitalWrite(GPIO_NUM_15, HIGH);
    delay(freq);
    digitalWrite(GPIO_NUM_15, LOW);
    delay(freq);
  }
}