#include <Arduino.h>
#include <Somo2.h>

Somo2 somo = Somo2();

void initAudio() {
  somo.begin();
  somo.reset();
  delay(1000);
  somo.setVolume(30);
}

void playAudio() {
  somo.playTrack(1, 1);
}

void stopAudio() {
  somo.stop();
}