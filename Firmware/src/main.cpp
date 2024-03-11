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

bool bootup = true;

void setup()
{
    Wire.begin(I2C_SDA, I2C_SCL);

    Serial.begin(115200);
    Serial.println("Tester started");

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

    display.setRotation(2);

    display.clearDisplay();
    display.drawBitmap(0, 0, image_data_boot_screen, 128, 64, 1);
    display.display();
}

void loop()
{
    if (bootup) {
        bootup = false;
        delay(3000);
    }

    display.clearDisplay();
    display.drawBitmap(0, 0, image_data_USB_Breakout, 128, 64, 1);
    display.display();

    bool anyPinConnected = false;
    for (PinPair &pair : pinPairs)
    {
        if (checkConnection(pair.pinA, pair.pinB))
        {
            anyPinConnected = true;
        }
    }

    if (!anyPinConnected)
    {
        return;
    }

    bool noConnection = false;
    for (PinPair &pair : pinPairs)
    {
        if (!checkConnection(pair.pinA, pair.pinB))
        {
            display.drawBitmap(0, 0, pair.imageData, 128, 64, 1);
            noConnection = true;

            Serial.println("Pin not connected: " + pair.pinA);
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

                Serial.println("Short detected: " + check.pin);
            }
        }
    }

    display.display();

    // if any pin is connected, start the real check
    if (anyPinConnected)
    {

        // If all pins are connected and no shorts detected, display a OK message
        if (!noConnection && !shortDetected)
        {
            playSuccessMelody();
            display.drawBitmap(0, 0, image_data_OK, 128, 64, 1);
            display.display();

            Serial.println("OK");
        }
        else
        {
            playErrorMelody();
            Serial.println("NOK");
        }
    }

    delay(1000);
}
