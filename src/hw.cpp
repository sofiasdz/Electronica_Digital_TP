/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  hw.cpp
 *      Hardware dependent code
 */

#include <Arduino.h>

#include "hw.h"

/*
 *  Private variables
 */

static int buttonState;                     // current reading from the input pin (pushbutton)
static int lastButtonState = HIGH;          // previous reading from the input pin (pushbutton)

static unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
static unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

/*
 *  Private functions
 */

/*
 *  verify_push:
 *      verifies if tact pushbotton has closed
 *      returns news BUTTON on closure
 */

static int
verify_push(void)
{
    int reading, result;

    result = 0;
    reading = digitalRead(PUSH);                        // Now read the button state

    if (reading != lastButtonState)                     // If the pushbutton state changed (due to noise or pressing it), then ....
        lastDebounceTime = millis();                    // ... reset the debouncing timer
    else
    if ((millis() - lastDebounceTime) > debounceDelay)  // If the pin_button state has changed, after the debounce time
    {
        if (reading != buttonState)                     // And if the current reading is different than the current buttonState
        {
            buttonState = reading;
            if (buttonState == LOW)
                result = BUTTON;
        }
    }
    lastButtonState = reading;                          // Save the reading. Next time through the loop, it'll be the lastButtonState
    return result;
}

/*
 *  Public functions
 */

/*
 *  get_board_num
 *      Return board number read from IB1,IB0
 */


int
get_board_num(void)
{
    return digitalRead(IB0) | (digitalRead(IB1) << 1); 
}

/*
 *  init_hw
 *      Called at power up
 */

void
init_hw(void)
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YEL, OUTPUT);
    pinMode(LED_GRN, OUTPUT);
    pinMode(LED_INT, OUTPUT);

    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_YEL,LOW);
    digitalWrite(LED_GRN,LOW);
    digitalWrite(LED_INT,LOW);

    pinMode(PUSH, INPUT_PULLUP);
    pinMode(IB0,INPUT_PULLUP);
    pinMode(IB1,INPUT_PULLUP);
}

/*
 *  verify_hw
 *      Verifies all input hardware
 */

int
verify_hw(void)
{
    return verify_push();
}

