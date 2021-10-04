#include <Arduino.h>
#include <Wifi.h>
#include "blink.h"
#include "prefs.h"
#include "ui.h"
#include "coin.h"
#include "arm.h"
#include "account.h"
#include "audio.h"
#include "wifiUtils.h"
#include "serverUtils.h"

bool helpDisplayed = false;

// how many addresses do we pick for a coin
#define PICK_COUNT 4
int8_t count = 0;

// keep track of data
#define PRIVATE_KEY_LENGTH 66
#define PUBLIC_KEY_LENGTH 132
#define ADDRESS_LENGTH 42
char privateKey[PRIVATE_KEY_LENGTH + 1];
char publicKey[PUBLIC_KEY_LENGTH + 1];
char address[ADDRESS_LENGTH + 1];
char balance[256]; // TODO: max size of balance ??
bool bingo;

// handle state transitions
enum class State {
  Init, // init state, only used for prev state
  Idle, // waiting for a coin
  Arm, // waiting for the arm to be pulled
  Picking, // arm pulled, start picking priv/pub/address
  Picked, // priv/pub/address picked
  Lose,
  Win
};
State state = State::Idle;
State prevState = State::Init;

void updateState(State newState, bool reset = true) {
  if (reset) resetUI();
  prevState = state;
  state = newState;
}

void setup() {
  readNVS();
  delay(1000);
  pinMode(GPIO_NUM_15, OUTPUT);
  digitalWrite(GPIO_NUM_15, LOW);
  startWifi();
  startServer();
  initUI();
  initCoin();
  initArm();
  initAudio();
}

void loop() {
  if (WiFi.getMode() == WIFI_STA) {
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

      case State::Lose:
        handleLose();
        break;

      case State::Win:
        handleWin();
        break;

      default:
        updateState(State::Idle);
        break;
    }
  } else {
    if (!helpDisplayed) {
      slowPrintf("Please connect to bandit-manchot wifi and go to 192.168.1.1");
      helpDisplayed = true;
    }
    dnsServer.processNextRequest();
  }
}


void handleIdle() {
  // wait for a coin
  if (prevState != State::Idle) {
    updateState(State::Idle);
    resetCoin();
    printIdle();
    stopAudio();
    playIdle();
  }
  if (hasCoin()) {
    updateState(State::Arm);
    resetCoin();
  }
}

void handleArm() {
  if (prevState != State::Arm) {
    updateState(State::Arm);
    printArm();
  }
  if (isArmPulled()) updateState(State::Picking);
}

void handlePicking() {
  playPicking();
  bool error = getAccount(privateKey, publicKey, address, balance, &bingo);
  // call failed, retry pulling the arm
  if (error) updateState(State::Arm);
  else {
    printPick(privateKey, publicKey, address, balance, count == 0);
  }
  count++;
  stopAudio();
  delay(1000);

  if (bingo) updateState(State::Win);
  else updateState(State::Picked, false);
}

void handlePicked() {
  if (count < PICK_COUNT) updateState(State::Picking, false);
  else {
    count = 0;
    delay(3000);
    updateState(State::Lose);
  }
}

void handleLose() {
  if (prevState != State::Lose) {
    updateState(State::Lose);
    printLose();
    playLose();
  }
  delay(8000);
  updateState(State::Idle);
}

void handleWin() {
  if (prevState != State::Win) {
    updateState(State::Win);
    playWin();
    printWin();
  }
  delay(20000);
  updateState(State::Idle);
}