/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt_actions.h
 *      Actions called as suscribed topics are received
 */

void clear_leds( char *msg );

void toggle_red( char *msg );
void toggle_yel( char *msg );
void toggle_grn( char *msg );
void toggle_int( char *msg );

void set_red(char *msg);
void set_yel(char *msg);
void set_grn(char *msg);
void set_int(char *msg);


