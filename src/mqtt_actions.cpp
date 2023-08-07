/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt_actions.cpp
 *      Actions called as suscribed topics are received
 *      Low level actions called from here are in 'hw_actions.cpp'
 */

#include <Arduino.h>

#include "mqtt_actions.h"       //  Prototypes of functions whose code are here
#include "hw_actions.h"         //  Prototypes of functions called from here

void
clear_leds( char *msg )
{
    clear_all_leds();
}

void
toggle_red( char *msg )
{
    toggle_led_red();
}

void
toggle_yel( char *msg )
{
    toggle_led_yel();
}

void
toggle_grn( char *msg )
{
    toggle_led_grn();
}

void
toggle_int( char *msg )
{
    toggle_led_int();
}

void set_red(char *msg)
{
    set_led_red( msg[0] != '0' );
}

void set_yel(char *msg)
{
    set_led_yel( msg[0] != '0' );
}

void set_grn(char *msg)
{
    set_led_grn( msg[0] != '0' );
}

void set_int(char *msg)
{
    set_led_int( msg[0] != '0' );
}



