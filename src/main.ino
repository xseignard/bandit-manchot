#include <Arduino.h>
#include "keys.h"
#include "etherscan.h"
#include "wifiUtils.h"
#include "ui.h"
#include "audio.h"

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
  Idle, // waiting for a coin
  Picking, // coin inserted start picking priv/pub/address
  Picked, // priv/pub/address picked
  Win
};
State state = State::Idle;



void setup() {
  Serial.begin(115200);
  startWifi();
  initKeys();
  initUI();
  initAudio();
}

void loop() {
  switch (state) {
    case State::Idle:
      handleIdle();
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
      Serial.println("UHO, shouldn't be there...");
      state = State::Idle;
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
  play(Track::Mario);
  printIdle();
  delay(10000);
  resetUI();
  play(Track::None);
  state = State::Picking;
}

void handlePicking() {
  pick(privateKey, publicKey, address);
  getBalance(address, balance);
  printPick(privateKey, publicKey, address, balance);
  count++;
  delay(1000);

  if (balance != "0") state = State::Win;
  else state = State::Picked;
}

void handlePicked() {
  if (count < PICK_COUNT) state = State::Picking;
  else state = State::Idle;
}

void handleWin() {
  delay(5000);
  state = State::Idle;
}