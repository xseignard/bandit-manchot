# 1 "/var/folders/l0/n6_pmns12kx3653h57b65f800000gn/T/tmpiutnd5d8"
#include <Arduino.h>
# 1 "/Users/xavier.seignard/repos/bandit-manchot/hardware/src/main.ino"
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


#define PICK_COUNT 4
int8_t count = 0;


#define PRIVATE_KEY_LENGTH 66
#define PUBLIC_KEY_LENGTH 132
#define ADDRESS_LENGTH 42
char privateKey[PRIVATE_KEY_LENGTH + 1];
char publicKey[PUBLIC_KEY_LENGTH + 1];
char address[ADDRESS_LENGTH + 1];
char balance[256];
bool bingo;


enum class State {
  Init,
  Idle,
  Arm,
  Picking,
  Picked,
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
void setup();
void loop();
void handleIdle();
void handleArm();
void handlePicking();
void handlePicked();
void handleLose();
void handleWin();
#line 48 "/Users/xavier.seignard/repos/bandit-manchot/hardware/src/main.ino"
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
  count = 0;
  delay(8000);
  updateState(State::Idle);
}

void handleWin() {
  if (prevState != State::Win) {
    updateState(State::Win);
    playWin();
    printWin(privateKey, publicKey, address, balance);
  }
  count = 0;
  delay(20000);
  updateState(State::Idle);
}