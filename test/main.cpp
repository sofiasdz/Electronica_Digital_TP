/*
 *  Project 40-mqtt - main.cpp
 */

#include <Arduino.h>

#include <WiFi.h>

#include <PubSubClient.h>

#include "wifi_ruts.h"

#include "defs.h"

#if MQTT == 1

const char *mqttServer =    R_NAME;
const int mqttPort =        R_PORT;
const char *mqttUser =      R_USER;
const char *mqttPassword =  R_PASS;

#else

const char *mqttServer =    L_NAME;
const int mqttPort =        L_PORT;
const char *mqttUser =      L_USER;
const char *mqttPassword =  L_PASS;

#endif

/*
 *  Hardware independent definitions
 */


typedef struct
{
    const char *sub_topic;
    void (*action)( char *message );
} topic_t;

WiFiClient espClient;
PubSubClient client(espClient);

char client_id[100];

#include "hard.h"   //  Hardware dependent code

/*
 *  Hardware independent code
 */

void
callback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = 0;
    Serial.printf( "For topic: \"%s\", message \"%s\"\n\r", topic, payload );
    for( int i = 0; topics[i].sub_topic != NULL; ++i )
        if( strstr( topic, topics[i].sub_topic ) != NULL )
        {
            (*topics[i].action)( (char *)payload );
            break;
        }
    Serial.println("-----------------------");
}

void
init_mqtt(void)
{
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect( client_id, mqttUser, mqttPassword))
            Serial.printf( "client %s connected\n\r", client_id );
        else
        {
            Serial.print("MQTT connection: failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    Serial.printf( "Connected to %s\n\r", mqttServer );
}

void
setup(void)
{
    get_client_id( client_id );
    Serial.begin(BAUD);
    printf("Hello\n");

    connect_wifi();

#if 0
    for(;;)
    ;
#endif

    init_hardware();
    init_mqtt();
    init_suscriptions();
}

void
loop()
{
    verify_changes();
    client.loop();
}
