#include <Arduino.h>
#include <Somo2.h>

Somo2 somo = Somo2();

void initAudio() {
  randomSeed(analogRead(GPIO_NUM_13));
  somo.begin();
  somo.reset();
  delay(1000);
  somo.setVolume(30);
}

void playIdle() {
  somo.playTrack(1, 1);
}

void playPicking() {
  somo.playTrack(2, 1);
}

void playWin() {
  somo.playTrack(4, 1);
}

void playLose() {
  somo.playTrack(3, random(1, 23));
}

void stopAudio() {
  somo.stop();
  delay(300);
}