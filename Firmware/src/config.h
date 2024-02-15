#ifndef CONFIG_H
#define CONFIG_H

#include <images.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define DAC1 25 // (GPIO25)
#define DAC2 26 // (GPIO26)

#define BUZZER_PIN 25 // Connect your speaker or buzzer to GPIO 25
#define LEDC_CHANNEL 0
#define LEDC_TIMER 13
#define LEDC_RESOLUTION 8 // Resolution of 8 bits
#define LEDC_BASE_FREQ 5000 // Base frequency for LEDC

#define TONE_PWM_CHANNEL 0

#define A_SHIELD_PIN 16
#define A_GND_PIN 18
#define A_5V_PIN 23
#define A_Dminus_PIN 14
#define A_Dplus_PIN 32
#define A_ID_PIN 4

#define B_SHIELD_PIN 17
#define B_GND_PIN 19
#define B_5V_PIN 13
#define B_Dminus_PIN 27
#define B_Dplus_PIN 33
#define B_ID_PIN 5

// Define an array of pin checks and corresponding images
struct PinCheck
{
    int pin;
    const uint8_t *imageData;
} pinChecks[] = {
    {A_SHIELD_PIN, image_data_Short_Shield},
    {A_GND_PIN, image_data_Short_GND},
    {A_5V_PIN, image_data_Short_5V},
    {A_Dminus_PIN, image_data_Short_Dminus},
    {A_Dplus_PIN, image_data_Short_Dplus},
    {A_ID_PIN, image_data_Short_ID}};

// Define an array of pin pairs for connection checks
struct PinPair
{
    int pinA;
    int pinB;
    const uint8_t *imageData; // Image to display if connected
} pinPairs[] = {
    {A_SHIELD_PIN, B_SHIELD_PIN, image_data_NC_Shield},
    {A_GND_PIN, B_GND_PIN, image_data_NC_GND},
    {A_5V_PIN, B_5V_PIN, image_data_NC_5V},
    {A_Dminus_PIN, B_Dminus_PIN, image_data_NC_Dminus},
    {A_Dplus_PIN, B_Dplus_PIN, image_data_NC_Dplus},
    {A_ID_PIN, B_ID_PIN, image_data_NC_ID}};

#endif // CONFIG_H