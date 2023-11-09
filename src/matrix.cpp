/*
 *  matrix.cpp
 */

#include <MD_MAX72xx.h>
#include <SPI.h>

#include "cutils.h"
#include "matrix.h"

#define delay_t 1000

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1
#define CS_PIN 5


// Hardware SPI connection
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE,CS_PIN,MAX_DEVICES);

#define NUM_SHAPE   4
#define NUM_ROWS    8

static const uchar symbols[NUM_SHAPE][NUM_ROWS] =
{
    {   0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18  },  //  heart
    {   0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C  },  //  face
    {   0x00, 0x24, 0x24, 0x24, 0x00, 0x42, 0x3C, 0x00  },  //  face2
    {   0x18, 0x0C, 0x06, 0xFF, 0xFF, 0x06, 0x0C, 0x18  }   //  arrow
};

static const char * shape_name[NUM_SHAPE] =
{
    "heart", "face1", "face2", "arrow"
};

void
init_matrix(void)
{
    mx.begin();
    mx.control(MD_MAX72XX::INTENSITY, 0);
    mx.clear();
}

void
set_row(int row_no, int content )
{
    mx.setRow(0, 0, row_no, content );
}

void
drawShape()
{
    int shape, i;


    for( shape = 0; shape < NUM_SHAPE; ++shape )
    {
        Serial.printf("%s\n", shape_name[shape]);
        for(i = 0; i < NUM_ROWS; ++i )
            mx.setRow(0, 0, i, symbols[shape][i]);
        delay(delay_t);
    }
}
