

#include <Arduino.h>

#include "cutils.h"
#include "leds.h"
#include "matrix.h"
#include "stock.h"
#include "vdisplay.h"
#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"

#include <iostream>

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

static const char *lista[] = {"Oreo", "Chocolinas", "Traviatta", "Rumba", "Mellizas", "Amor", "Tentacion", "Criollitas"};
static const std::string machine_no = "1";

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
    // cuando hace el setup la maquina hago un publish a machine/conected/ message:”machineId:1”
    std::string topic = "machine/connected/";
    const char *topic_cstr = topic.c_str();
    const char *message_cstr = machine_no.c_str();
    do_publish(topic_cstr,message_cstr);

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
         // Lista de productos
        const char *lista[] = {"Oreo", "Chocolinas", "Traviatta", "Rumba", "Mellizas", "Amor", "Tentacion", "Criollitas"};

         // Tamaño de la lista
        const int tamanoLista = sizeof(lista) / sizeof(lista[0]);

        // Mostrar la lista de productos con sus números
        std::cout << "Lista de productos:" << std::endl;
        for (int i = 0; i < tamanoLista; ++i) {
            std::cout << i + 1 << ". " << lista[i] << std::endl;
         }

        Serial.printf("Input product number (%d to %d)....", MIN_PRODUCT, MAX_PRODUCT);
        Serial.flush();
        while(!Serial.available())
        ;
        in = Serial.readStringUntil('\n');
        product_no = atoi(in.c_str()); 
        Serial.printf("Product selected = %d\n",  lista[product_no-1]);
        if( product_no == FILL ){
            Serial.printf("Refilling stock\n");
            
            // tengo que hacer algo cuando hago un refill de productos??
            std::string topic = "stock/machineId:" + machine_no+",ProductId: "+ std::string(lista[product_no - 1]);
            int stock =8;
            const char *topic_cstr = topic.c_str();
            std::string str_stock = std::to_string(stock);
            const char *cstr_stock = str_stock.c_str();
            do_publish(topic_cstr,cstr_stock);
        }

        else if( status = (product_no < MIN_PRODUCT || product_no > MAX_PRODUCT) )
            Serial.printf("\nBad product number %d Redo...\n", product_no );
    } while(status);

    if( product_no == FILL )
    {
        fill_stock();
        Serial.printf("stock updated\n");
         refresh_vdisplay();
         //relleno todos los productos
    for (int i = 1; i <= 8; ++i) {
            std::cout << "Número: " << i << std::endl;

            std::string topic = "stock/" + std::string( lista[product_no-1]);
            int stock =8;
            const char *topic_cstr = topic.c_str();
            std::string str_stock = std::to_string(stock);
            const char *cstr_stock = str_stock.c_str();
            do_publish(topic_cstr,cstr_stock);
        }

       
    } else
    {
        printf("\n\tSelected product_no = %s\n", lista[product_no-1]);
        if( stock_state(product_no) > 0 )
        {
            //cuando compra alguien algo hago un publish 
            //que dice soldProducts/ message:” machieneId:1, productId:1
            Serial.printf("Stock remaining after delibering = %d\n", change_stock(product_no,-1) );
            Serial.printf( "Product number %d delivered\n",  std::string(lista[product_no-1] ));
            std::string topic = "soldProducts/";
            std::string message = "machineId:" + machine_no +",ProductId: "+ std::to_string((product_no));
            int stock =stock_state(product_no);
            const char *topic_cstr = topic.c_str();
            const char *message_cstr = message.c_str();
            do_publish(topic_cstr,message_cstr);
        }
        else
            Serial.printf("No product %d remain in stock\n",  lista[product_no-1]);

          
        refresh_vdisplay();
    }
}


