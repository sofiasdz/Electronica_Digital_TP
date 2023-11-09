/*
 *  Project 91-vending 
 *  Austral - EAM
 *
 *  leds.cpp
 *      For driving independent leds
 */

#include <Arduino.h>

#include "cutils.h"
#include "leds.h"

static const unsigned led_pins[] =
{
    LED_RED, LED_YEL, LED_GRN, LED_INT
};


/*
 *  Private functions
 */

/*
 *  Public functions
 */

/*
 *  clear_leds
 *      clear all leds
 */

void
clear_leds(void)
{
    const unsigned *p;
    int i;

    for(p = led_pins, i = 0; i < NUM_LEDS; ++i, ++p )
        digitalWrite(*p,LOW);
}


/*
 *  init_leds
 *      Called at power up
 */

void
init_leds(void)
{
    const unsigned *p;
    int i;

    for(p = led_pins, i = 0; i < NUM_LEDS; ++i, ++p )
        pinMode(*p,OUTPUT);

}

/*
 *  set_led
 *      led with index ix is lit on val <> 0  or shut off on val == 0
 *      
 */

void
set_led(unsigned ix, unsigned val)
{
    if( ix >= NUM_LEDS )
        return;
    digitalWrite(led_pins[ix],val);
}

