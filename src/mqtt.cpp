/*
 *
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt.cpp
 *      Manage conversation with MQTT broker
 */

/*
 *  System includes
 */

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
/*#include <WiFiClientSecure.h>*/
#include "secrets.h"
/*
 *  Project includes
 */

#include "mqtt.h"               //  Prototypes of this module public functions
#include "mqtt_actions.h"       //  Actions to be called in case of subscription topics news
#include "mqtt_def.h"           //  User configuration file

/*
 *  Objects instantiations
 */

WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);

/*
 *  Private variables
 */

static char client_id[100];
static char tp_string[100];
static int board_num;
static char sub_header[50];

/*
 *  Private constants
 */

static const char *mqttServer =     BROKER_NAME;
static const int  mqttPort =        BROKER_PORT;
static const char *mqttUser =       BROKER_USER;
static const char *mqttPassword =   BROKER_PASS;

/*
 *  Private functions
 */

/*
 *  callback:
 *      Function to asynchronously call when someone
 *      publish in broker a topic that we are subscribed
 */

// static void
// callback(char *topic, byte *payload, unsigned int length)
// {
//     char *pl;

//     payload[length] = 0;
//     Serial.printf( "For topic: \"%s\", message \"%s\"\n", topic, payload );
//     if( ( pl = strrchr(topic,'/') ) == NULL )
//         return;
//     Serial.printf("main topic = %s\n", ++pl );

//     for( int i = 0; topics[i].sub_topic != NULL; ++i )
//         if( strcmp( pl, topics[i].sub_topic ) == 0 )
//         {
//             (*topics[i].action)( (char *)payload );
//             break;
//         }
//     Serial.println("-----------------------");
// }
static int
get_origin(char *st, char *slast )
{
    char *sfirst;

    *slast = '\0';
    if( ( sfirst = strrchr(st,'/') ) == NULL )
        return -1;
    return atoi(++sfirst);
}

static void callback(char *topic, byte *payload, unsigned int length) {
    char payloadStr[length + 1]; // Crear un buffer para el payload
    memcpy(payloadStr, payload, length); // Copiar los datos del payload
    payloadStr[length] = '\0'; // Asegurarse de que la cadena esté terminada en cero

    Serial.printf("Received message on topic: \"%s\"\n", topic);
    Serial.printf("Message: \"%s\"\n", payloadStr);

    // Extraer el subtópico de la parte final del tópico
    char *subTopic = strrchr(topic, '/');
    if (subTopic == NULL) {
        Serial.println("Subtopic not found in the topic.");
        return;
    }
    subTopic++; // Moverse más allá del carácter '/'

    Serial.printf("Subtopic: \"%s\"\n", subTopic);

    // Buscar y ejecutar la acción correspondiente al subtópico
    bool actionFound = false;
    for (int i = 0; topics[i].sub_topic != NULL; ++i) {
        if (strcmp(subTopic, topics[i].sub_topic) == 0) {
            Serial.printf("Executing action for subtopic: \"%s\"\n", subTopic);
            (*topics[i].action)(payloadStr);
            actionFound = true;
            break;
        }
    }

    if (!actionFound) {
        Serial.printf("No action found for subtopic: \"%s\"\n", subTopic);
    }

    Serial.println("-----------------------");
}
/*
 *  client_connect:
 *      Connection to broker
 *      Tries until connected
 */

static void
client_connect(void)
{
    Serial.printf("Connecting to MQTT...\n");
    Serial.printf("server = %s\n",mqttServer);

    //Configura el certificado de la CA
    //espClient.setCACert(CERT_CA);

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    while (!client.connected())
    {
        if (client.connect(client_id, mqttUser, mqttPassword))
            Serial.printf( "%s: client %s connected\n", __FUNCTION__, client_id );
        else
        {
            Serial.printf("%s: failed with state = %d\n", __FUNCTION__, client.state());
            //delay(2000);
        }
    }
    Serial.printf("Connected to %s\n", mqttServer);
}

/*
 *  subscribe_to:
 *      Function to subscribe to one topic
 */

static void
subscribe_to( const char *ptopic )
{
    int status;
 
    status = client.subscribe(ptopic);
    Serial.printf("%s -> topic: %s [status = %d]\n", __FUNCTION__, ptopic, status);
}

/*
 *  init_subscriptions:
 *      Function to subscribe to all topics
 *      In this case, all topics that are in a topic table
 *      outside this file (obtained in from 'mqtt_def.h'
 */
    

static void
init_subscriptions(void)
{
    const char **p;
    char subs_string[100];

    for( p = subs; *p != NULL; ++p )
    {
        sprintf(subs_string,"%s/%s", sub_header, *p );
        subscribe_to(subs_string);
    }
}

/*
 *  Public functions
 */

/*
 *  init_mqtt:
 *      All iniializations needed to set variables
 *      connect to broker and initialize subscriptions
 */


void
init_mqtt(int num)
{
    subs_string(sub_header);

    id_string(client_id,num);
    Serial.printf("client_id: %s\n", client_id );

    topic_string(tp_string,num);
    Serial.printf("tp_string: %s\n", tp_string );

    client_connect();
    init_subscriptions();
    
}

/*
 *  do_publish:
 *      Function to make a publication in broker
 *      receives pointer to topic and message string
 */

void
do_publish(const char *ptopic, const char *msg)
{
    static char tp[100];

    tp[sizeof(tp)-1] = '\0';
    snprintf( tp, sizeof(tp)-1, "%s/%s", tp_string, ptopic );
    Serial.printf( "topic: %s\n", tp );
    client.publish( tp, msg );
    Serial.printf( "%s: %s %s\n\r", __FUNCTION__, tp, msg );
}

/*
 *  test_mqtt:
 *      update to see broker news
 */

void
test_mqtt(void)
{
    client.loop();
}


