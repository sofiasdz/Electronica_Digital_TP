/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *      File main.cpp
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"





int pinCS = 5; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
Max72xxPanel matrix = Max72xxPanel(pinCS, 1, 1);

const int pinRandom = A0;

const int wait = 100; // In milliseconds
const int length = 8;

void
setup(void)
{

  randomSeed(analogRead(pinRandom)); // Initialize random generator

    matrix.setIntensity(4); // Set brightness between 0 and 15
    matrix.fillScreen(0);
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


    matrix.fillScreen(1);
    if( verify_hw() == BUTTON )             //  Verify all input hardware
        do_publish("Hola de gonza","button");
    test_mqtt();                        //  Test news from broker
}
