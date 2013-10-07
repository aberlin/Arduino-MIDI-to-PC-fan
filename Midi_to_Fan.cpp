// Do not remove the include below, I use it in Eclipse(http://www.baeyens.it/eclipse/)
#include "Midi_to_Fan.h" // you dont need this in Arduino Sketchbook IDE
#include <MIDI.h>  // Add Midi Library (http://sourceforge.net/projects/arduinomidilib/)
#define LED 13    // Arduino Board LED is on Pin 13
int speakerPin1 = 8;
int speakerPin2 = 4;
int muteFrquenzy = 30000;//The Fan should move even when there is no Sound you can hear

volatile byte lastTone1 = 0;
volatile byte lastTone2 = 0;

//Prototypesn
int getTone(int noteByte);
void playTone(int noteByte);

void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
	if (pitch == lastTone1) {
		tone(speakerPin1, muteFrquenzy);
		lastTone1 = 0;
	}
	if (pitch == lastTone2) {
		tone(speakerPin2, muteFrquenzy);
		lastTone2 = 0;
	}

	digitalWrite(LED, LOW); //Turn LED off
}

// Below is my function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
	digitalWrite(LED, HIGH);  //Turn LED on
	if (velocity == 0) { //A NOTE ON message with a velocity = Zero is actually a NOTE OFF
		MyHandleNoteOff(channel, pitch, velocity);
	}

	if (pitch > 64) {
		playTone(pitch);
	}
}

void setup() {
	pinMode(LED, OUTPUT); // Set Arduino board pin 13 to output
	MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
// OMNI sets it to listen to all channels.. MIDI.begin(2) would set it
// to respond to channel﻿﻿ 2 notes only.
	MIDI.setHandleNoteOn(MyHandleNoteOn); // This is important!! This command
	// tells the Midi L﻿﻿ibrary which function I want called when a Note ON command
	// is received. in this case it's "MyHandleNoteOn".
	MIDI.setHandleNoteOff(MyHandleNoteOff);
}

void loop() { // Main loop

	MIDI.read(); // Continually check what Midi Commands have been received.
	//MakeSample();
	//PlaySample();
}

//MIDI
void playTone(int noteByte) {
	if (lastTone1 == 0) {
		lastTone1 = noteByte;
		tone(speakerPin1, getTone(noteByte));
		return;
	}

	if (lastTone2 == 0) {
		lastTone2 = noteByte;
		tone(speakerPin2, getTone(noteByte));
	}
	return;
}

int getTone(int noteByte) {


	//an Octave
	static const int tones[] = {
			8372 / 2,//c
			8870 / 2,//c#
			9397 / 2,//d
			9956 / 2, //d#
			10548/ 2,//e
			11175 / 2,//f
			11840 / 2,//f#
			12544 / 2,//g
			13290 / 2,//g#
			14080 / 2, //a
			14917/ 2,//a#
			15804 / 2 //h
			};

	uint16_t rightshift = 10 - noteByte/12; // find the right octave for the tone
	return tones[noteByte%12] >> rightshift; // take the frequency and transpose it into the right octave
	// to transpose an octave deeper the frequency is divided by 2, this is done with a >> in this case


}

