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
  somo.playTrack(1, 2);
}

void playWin() {
  somo.playTrack(1, 3);
}

void playLose() {
  // somo.playTrack(4, random(1, LOSE_COUNT));
    somo.playTrack(1, 4);

}

void stopAudio() {
  somo.stop();
  delay(300);
}