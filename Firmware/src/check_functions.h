#ifndef CHECK_FUNCTIONS_H
#define CHECK_FUNCTIONS_H

#include <Arduino.h>
#include <config.h>

/**
 * Returns true if pinToCheck is shorted to another pin
 */
bool checkShort(int pinToCheck)
{
    int pins[] = {A_SHIELD_PIN, A_GND_PIN, A_5V_PIN, A_Dminus_PIN, A_Dplus_PIN, A_ID_PIN};

    // Set the checked pin as INPUT_PULLUP and all others as OUTPUT and LOW
    for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++)
    {
        if (pins[i] == pinToCheck)
        {
            pinMode(pins[i], INPUT_PULLUP);
        }
        else
        {
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], LOW);
        }
    }

    delay(10); // Short delay to allow the signal to stabilize

    if (digitalRead(pinToCheck) == LOW)
    {
        return true;
    }
    return false;
}

/**
 * Returns true if pinA is connected to pinB
 */
bool checkConnection(int pinA, int pinB)
{
    pinMode(pinA, OUTPUT);
    pinMode(pinB, INPUT_PULLUP); // Enable pull-up to ensure a stable read when not connected

    digitalWrite(pinA, LOW); // Send a LOW signal from pinA
    delay(10);               // Short delay to allow the signal to stabilize

    bool isConnected = (digitalRead(pinB) == LOW); // Check if pinB reads the HIGH signal

    // Reset pin modes to default (INPUT) to minimize interference with other checks
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);

    return isConnected;
}

#endif // CHECK_FUNCTIONS_H