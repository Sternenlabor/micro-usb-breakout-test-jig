#ifndef MELODY_H
#define MELODY_H

#include <Arduino.h>
#include "config.h"

// Note frequencies for a simple melody (C scale)
#define NOTE_C4 261
#define NOTE_D4 294
#define NOTE_E4 329
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 493
#define NOTE_C5 523

void playNote(int note, int duration)
{
    ledcWriteTone(LEDC_CHANNEL, note); // Play the note
    delay(duration);                   // Wait for the duration of the note
    ledcWriteTone(LEDC_CHANNEL, 0);    // Stop the note
}

/*
void playSuccessMelody()
{
    playNote(NOTE_C4, 500);
    playNote(NOTE_E4, 500);
    playNote(NOTE_G4, 500);
    playNote(NOTE_C5, 1000);
}

// Success Melody: A rising sequence with a harmonious end
void playSuccessMelody()
{
    playNote(523, 250); // C5
    playNote(587, 250); // D5
    playNote(659, 250); // E5
    playNote(698, 250); // F5
    playNote(784, 500); // G5
}

// Error Melody: A descending sequence with a minor feel
void playErrorMelody()
{
    playNote(784, 250); // G5
    playNote(698, 250); // F5
    playNote(659, 250); // E5
    playNote(587, 250); // D5
    playNote(523, 500); // C5
}
*/

void playSuccessMelody()
{
    playNote(784, 250); // G5
}

void playErrorMelody()
{
    playNote(523, 250); 
    playNote(523, 500);
}

#endif // MELODY_H