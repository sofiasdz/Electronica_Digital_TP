/*
 *  Project 91-vending 
 *  Austral - EAM
 *
 *  leds.h
 *      For driving independent leds
 */

#define NUM_LEDS 4

/*
 *  init_leds
 *      Called at power up
 */

void init_leds(void);

/*
 *  clear_leds
 *      clear all leds
 */

void clear_leds(void);

/*
 *  set_led
 *      led with index ix is lit on val <> 0  or shut off on val == 0
 *      
 */

void set_led(unsigned ix, unsigned val);


