#include <Arduino.h>
#include <ArduinoJson.h>
#include "cutils.h"
#include "leds.h"
#include "matrix.h"
#include "stock.h"
#include "vdisplay.h"
#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"
#include "machine.h"  // Include the machine header file
#include <iostream>
#include <ArduinoJson.h>

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
// Create a mutable Machine object
Machine vendingMachine(true, false, 0.0, {}, 1);  // Initialize with default values; adjust as needed

void
setup(void)
{
    Serial.begin(BAUD);
    init_leds();
    init_matrix();
    //init_stock();
    init_vdisplay();
    int board;
    connect_wifi();
    init_hw();
    board = get_board_num();
    printf("Board = %d\n", board);
    init_mqtt(board);
    // cuando hace el setup la maquina hago un publish a machine/conected/ message:”machineId:1”
// Assuming vendingMachine is an instance of the Machine class
    std::string topic = "machine/connected";
    const char* topic_cstr = topic.c_str();

    // Create a JSON object
    DynamicJsonDocument doc(256); // Adjust the size based on your needs

    // Add data to the JSON object
    doc["machineId"] = vendingMachine.customId;

    // Serialize the JSON object to a string
    std::string message;
    serializeJson(doc, message);

    // Convert the message to const char
    const char *message_cstr = message.c_str();

    do_publish(topic_cstr, message_cstr);


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
         //Traerlas desde la machine
         //getProductNames(vendingMachine); creado en machine.cpp
        std::vector<std::string> productNames = vendingMachine.getProductNames();

        const char* lista[productNames.size()];

         // Tamaño de la lista
        const int tamanoLista = sizeof(lista) / sizeof(lista[0]);

        // Mostrar la lista de productos con sus números
        std::cout << "Vending Machine Simulation" << std::endl;
        Serial.printf("=========================\n");
        std::cout << "Cookies Available:" << std::endl;
        for (size_t i = 0; i < productNames.size(); ++i) {
            std::cout << i + 1 << ". " << productNames[i] << std::endl;
        }

        Serial.printf("Input product number to select (%d to %d)....", MIN_PRODUCT, MAX_PRODUCT);
        Serial.flush();
        while(!Serial.available())
        ;
        in = Serial.readStringUntil('\n');
        product_no = atoi(in.c_str()); 
        Serial.printf("Product selected = %d\n", product_no);
        if( product_no == FILL ){
            
        }

        else if( status = (product_no < MIN_PRODUCT || product_no > MAX_PRODUCT) )
            Serial.printf("\nBad product number %d Redo...\n", product_no );
    } while(status);

    if( product_no == FILL ) //Esto lo deberia triggerear mister mantenimiento
    {
        //fill_stock();
        Serial.printf("stock updated\n");
         refresh_vdisplay();
    } else {
        printf("\n\tSelected cookie = %s\n", lista[product_no-1]);
    }


    
        printf("\n\tSelected cookie = %s\n", lista[product_no-1]);
        //if( stock_state(product_no) > 0 )
        if(true)
        {
            //cuando compra alguien algo hago un publish 
          //que dice soldProducts/ message:” machieneId:1, productId:1
            //Serial.printf("Stock remaining after delivering = %d\n", change_stock(product_no,-1) );
            Serial.printf( "Product number %d delivered\n",  product_no );
            std::string topic = "soldProducts";

            // Create a JSON object
            DynamicJsonDocument doc(256); // Adjust the size based on your needs

            // Add data to the JSON object
            doc["machineId"] = std::to_string(vendingMachine.customId);
            doc["productId"] = product_no;

            // Serialize the JSON object to a string
            std::string message;
            serializeJson(doc, message);

            // Convert the topic and message to const char*
            const char *topic_cstr = topic.c_str();
            const char *message_cstr = message.c_str();

            // Publish the message
            do_publish(topic_cstr, message_cstr);
        }
        else
           Serial.printf("No product %d remain in stock\n",  product_no);

          
        refresh_vdisplay();
}




