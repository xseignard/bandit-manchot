#include <Arduino.h>
#include "keys.h"
#include "etherscan.h"
#include "wifiUtils.h"
#include "ui.h"
#include "coin.h"

// how many addresses do we pick for a coin
#define PICK_COUNT 5
int8_t count = 0;

// keep track of data
// 2 chars for each hex + termination char
char privateKey[PRIVATE_KEY_LENGTH * 2 + 1];
char publicKey[PUBLIC_KEY_LENGTH * 2 + 1];
// '0x' + 2 chars for each hex + termination char
char address[2 + ADDRESS_LENGTH * 2 + 1];
char balance[64]; // TODO: max size of balance ??

// handle state transitions
enum class State {
  Init, // init state, only used for prev state
  Idle, // waiting for a coin
  Arm, // waiting for the arm to be pulled
  Picking, // arm pulled, start picking priv/pub/address
  Picked, // priv/pub/address picked
  Win
};
State state = State::Idle;
State prevState = State::Init;

void updateState(State newState) {
  prevState = state;
  state = newState;
}

void setup() {
  pinMode(GPIO_NUM_15, OUTPUT);
  digitalWrite(GPIO_NUM_15, LOW);
  Serial.begin(115200);
  startWifi();
  initKeys();
  initUI();
  initCoin();
}

void loop() {
  switch (state) {
    case State::Idle:
      handleIdle();
      break;
    
    case State::Arm:
      handleArm();
      break;

    case State::Picking:
      handlePicking();
      break;

    case State::Picked:
      handlePicked();
      break;

    case State::Win:
      handleWin();
      break;

    default:
      Serial.println("UHO, you shouldn't be there...");
      updateState(State::Idle);
      break;
  }
}


void handleIdle() {
  // a serie of picking just ended
  if (count > 0) {
    // reset what needs to be
    count = 0;
    resetUI();
  }
  // wait for a coin
  if (prevState != State::Idle) {
    updateState(State::Idle);
    printIdle();
  }
  if (hasCoin()) {
    digitalWrite(GPIO_NUM_15, HIGH);
    updateState(State::Arm);
    resetCoin();
  }
}

void handleArm() {
  printArm();
  // TODO handle arm
  delay(5000);
  updateState(State::Picking);
}

void handlePicking() {
  pick(privateKey, publicKey, address);
  getBalance(address, balance);
  printPick(privateKey, publicKey, address, balance);
  count++;
  delay(1000);

  if (balance != "0") updateState(State::Win);
  else updateState(State::Picked);
}

void handlePicked() {
  if (count < PICK_COUNT) updateState(State::Picking);
  else updateState(State::Idle);
}

void handleWin() {
  delay(5000);
  updateState(State::Idle);
}