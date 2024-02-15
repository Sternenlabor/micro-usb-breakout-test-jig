#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#include <config.h>
#include <images.h>
#include <check_functions.h>
#include <melody.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // Initialize LEDC
    ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_RESOLUTION);
    ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Infinite loop
    }

    // Clear the buffer
    display.clearDisplay();

    // display.setTextSize(1); // Normal 1:1 pixel scale
    // display.setFont(&FreeMono9pt7b);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner

    // display.print("booting...");

    delay(10);
}

bool runChecks = true;
bool noConnection = false;

void loop()
{
    display.clearDisplay();

    // Draw bitmap on the screen
    display.drawBitmap(0, 0, image_data_USB_Breakout, 128, 64, 1);

    // display.drawBitmap(0, 0, image_data_OK, 128, 64, 1);

    display.display();

    bool anyPinConnected = false;
    for (PinPair &pair : pinPairs)
    {
        if (checkConnection(pair.pinA, pair.pinB))
        {
            display.drawBitmap(0, 0, pair.imageData, 128, 64, 1);
            anyPinConnected = true;
        }
    }

    delay(1000);

    for (PinPair &pair : pinPairs)
    {
        if (!checkConnection(pair.pinA, pair.pinB))
        {
            display.drawBitmap(0, 0, pair.imageData, 128, 64, 1);
            noConnection = true;
        }
    }

    bool shortDetected = false;
    // Only check for shorts if we already have a connection to every pin
    if (!noConnection)
    {
        for (PinCheck &check : pinChecks)
        {
            if (checkShort(check.pin))
            {
                display.drawBitmap(0, 0, check.imageData, 128, 64, 1);
                shortDetected = true;
            }
        }
    }

    display.display();

    // if any pin is connected, start the real check
    if (anyPinConnected && runChecks)
    {
        runChecks = false;

        // If all pins are connected and no shorts detected, display a OK message
        if (!noConnection && !shortDetected)
        {
            playSuccessMelody();
        }
        else
        {
            playErrorMelody();
        }
    }
    else
    {
        runChecks = true;
    }

    delay(1000);

    // If all pins are connected and no shorts detected, display a OK message
    if (!noConnection && !shortDetected)
    {
        display.drawBitmap(0, 0, image_data_OK, 128, 64, 1);
    }

    delay(1000);
}
