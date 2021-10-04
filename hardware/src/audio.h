#include <Arduino.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define RX_PIN 16
#define TX_PIN 17

HardwareSerial audioBus(1);
DFRobotDFPlayerMini player;

void initAudio() {
  randomSeed(analogRead(GPIO_NUM_13));
  audioBus.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(1000);
  player.begin(audioBus);
  player.reset();
  delay(1000);
  player.volume(30);
}

void playIdle() {
  player.playFolder(1, 1);
}

void playPicking() {
  player.playFolder(2, 1);
}

void playWin() {
  player.playFolder(4, 1);
}

void playLose() {
  player.playFolder(3, random(1, 23));
}

void stopAudio() {
  player.stop();
  delay(300);
}