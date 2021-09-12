#include <Arduino.h>
#include <fabgl.h>

#include "sounds/mario.h"

SoundGenerator soundGenerator;
// init all sounds here
SamplesGenerator mario = SamplesGenerator(marioSamples, sizeof(marioSamples));

enum class Track {
  Mario,
  None,
};
Track track = Track::None;

void initAudio() {
  soundGenerator.setVolume(127);
  // attach all sounds here
  soundGenerator.attach(&mario);
}

void play(Track track) {
  switch (track) {
    case Track::Mario:
      mario.enable(true);
      // TODO: disable others
      soundGenerator.play(true);
      break;
    case Track::None:
    default:
      // TODO: disable all
      mario.enable(false);
      soundGenerator.play(false);
      break;
  }
}
