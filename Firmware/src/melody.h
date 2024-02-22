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


void playSuccessMelody()
{
    playNote(NOTE_C4, 50);
}

void playErrorMelody()
{
    playNote(NOTE_C5, 250); 
    delay(50);
    playNote(NOTE_C5, 500);
}

#endif // MELODY_H