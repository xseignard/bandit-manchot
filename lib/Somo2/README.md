# SOMO-II library

[SOMO-II](https://www.4dsystems.com.au/product/SOMO_II/) is a cheap mp3 player that can be controlled over a serial port.

This library implements the serial control in order to use it along with an Arduino/Teensy/Any Arduino capable board.

Please check the source code/examples to see how to use it.

## Simple example

```Arduino
#include <Somo2.h>

// Rx, Tx to the mp3 player
Somo2 somo(11, 12);

void setup () {
	Serial.begin(9600);
	// init mp3 player
	somo.begin();
	// reset it
	somo.reset();
	// wait a bit for the reset
	delay(1000);
	// set the volume at 20 out of 30
	somo.setVolume(20);
	// play the first track of the first folder
	somo.playTrack(1, 1);
}

void loop () {
}
```
