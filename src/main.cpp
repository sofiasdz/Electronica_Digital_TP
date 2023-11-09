

#include <Arduino.h>

#include "cutils.h"
#include "leds.h"
#include "matrix.h"
#include "stock.h"
#include "vdisplay.h"
#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"

#define FILL    99

#if LED_TEST == 1

static
void
scan_leds(unsigned val)
{
    int i;

    Serial.printf("%s: %d\n",__FUNCTION__,val);
    for(i = 0; i < NUM_LEDS; ++i)
    {
        set_led(i,val);
        delay(500);
    }
}

static void
led_test(void)
{
    forever
    {
        scan_leds(1);
        scan_leds(0);
    }
}
#endif

void
setup(void)
{
    Serial.begin(BAUD);

    init_leds();
    init_matrix();
    init_stock();
    init_vdisplay();
    int board;
    connect_wifi();
    init_hw();
    board = get_board_num();
    printf("Board = %d\n", board);
    init_mqtt(board);

#if LED_TEST == 1
    led_test();
#elif MATRIX_TEST == 1
    forever
        drawShape();
#endif
}

void
loop(void)
{

       
    test_mqtt();                       //  Test news from broker
    String in;
    int product_no,status;

    status = 0;
    do
    {
        Serial.printf("=========================\n");
        refresh_vdisplay();
        Serial.printf("Input product number (%d to %d)....", MIN_PRODUCT, MAX_PRODUCT);
        Serial.flush();
        while(!Serial.available())
        ;
        in = Serial.readStringUntil('\n');
        product_no = atoi(in.c_str()); 
        Serial.printf("Product selected = %d\n", product_no);
        if( product_no == FILL )
            Serial.printf("Refilling stock\n");
        else if( status = (product_no < MIN_PRODUCT || product_no > MAX_PRODUCT) )
            Serial.printf("\nBad product number %d Redo...\n", product_no );
    } while(status);

    if( product_no == FILL )
    {
        fill_stock();
        Serial.printf("stock updated\n");
        refresh_vdisplay();
    } else
    {
        printf("\n\tSelected product_no = %d\n", product_no);
        if( stock_state(product_no) > 0 )
        {

            Serial.printf("Stock remaining after delibering = %d\n", change_stock(product_no,-1) );
            Serial.printf( "Product number %d delivered\n", product_no );
            do_publish("Hola de gonza","button");
        }
        else
            Serial.printf("No product %d remain in stock\n", product_no);
        refresh_vdisplay();
    }
}


