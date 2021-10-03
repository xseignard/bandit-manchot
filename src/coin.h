#include <Arduino.h>

#define COIN 23

boolean coinInserted = false;

void coinInterrupt() {
  coinInserted = true;
}

void initCoin() {
  attachInterrupt(COIN, coinInterrupt, FALLING);
}

boolean hasCoin() {
  return coinInserted;
}

void resetCoin() {
  coinInserted = false;
}