/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *      File main.cpp
 */

#include <Arduino.h>

#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"

void
setup(void)
{
    int board;

    Serial.begin(BAUD);

    connect_wifi();

    init_hw();
    board = get_board_num();
    printf("Board = %d\n", board);
    init_mqtt(board);
}

void
loop(void)
{
    if( verify_hw() == BUTTON )             //  Verify all input hardware
        do_publish("button","button");
    test_mqtt();                            //  Test news from broker
}
