/*
 *  Project 91-vending 
 *  Austral - EAM
 *
 *  button.cpp
 *      For driving push button
 */

#include <Arduino.h>

#include "button.h"

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
                result = 1;
        }
    }
    lastButtonState = reading;                          // Save the reading. Next time through the loop, it'll be the lastButtonState
    return result;
}

/*
 *  Public functions
 */



/*
 *  init_button
 *      For initializing push button
 */

void
init_button(void)
{
    pinMode(PUSH, INPUT_PULLUP);
}

/*
 *  verify_button
 *      Verifies closing button
 *      if true, button push
 */

int
verify_button(void)
{
    return verify_push();
}

