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
#include <WiFiClientSecure.h>


/*
 *  Project includes
 */

#include "mqtt.h"               //  Prototypes of this module public functions
#include "mqtt_actions.h"       //  Actions to be called in case of subscription topics news
#include "mqtt_def.h"           //  User configuration file

/*
 *  Objects instantiations
 */

WiFiClientSecure espClient;
PubSubClient client(espClient);

/*
 *  Private variables
 */

static char client_id[100];
static char tp_string[100];
static int board_num;
static char sub_header[50];


// Certificado de la CA
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n
MIIDHzCCAgcCFEoiQ99ulAqBFaCVpSRilkqpB5B1MA0GCSqGSIb3DQEBCwUAMEwx
CzAJBgNVBAYTAkFSMRUwEwYDVQQIDAxCdWVub3MgQWlyZXMxFTATBgNVBAoMDEV4
cGVuZGV4IExMQzEPMA0GA1UEAwwGU2VydmVyMB4XDTIzMTEyNjAxNTg0NFoXDTI0
MTEyNTAxNTg0NFowTDELMAkGA1UEBhMCQVIxFTATBgNVBAgMDEJ1ZW5vcyBBaXJl
czEVMBMGA1UECgwMRXhwZW5kZXggTExDMQ8wDQYDVQQDDAZTZXJ2ZXIwggEiMA0G
CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC/b8a9NayHApQzBx3rz6D8EJUcJyo5
jXlR526a4wWFyO7DjtLHEKJ9j+WrFJqcCHs3zxDkaCeTflgxaE1Rloc7tJNRJrCk
wji55dEmPKNwv7gmq3zeDjnvs8N0MDddYA+XXOvWRYLFeGV/40CkrP89nfi+pZyO
kPVZ8YfIhNRYx1Y9Hb32bT8wPbpumBFkFWt5mk1L1WiGn5kdZn3b0NPOxUXKlxhe
A8kPw9OrfWSVD1aaWDowzjbRsUf9Z/ejF8559ZlpG5lgt3qqA5v85OT8zoMaYUAy
jnE2nEkQurWnoOVJT1JmjY6yztfKZwPf5jDMU12xMjfM8Gxtmy4s2mcZAgMBAAEw
DQYJKoZIhvcNAQELBQADggEBAIYcMShsAvkw5+ZxfAWOSo3XBScrZbuYLvTyEJjf
xABjuRIbmXsuK0cFIBO9E+oHPmJ/YhdvPlMcAvGYtWzUwK5VBxgIaJK308FMz0XO
hGhrZbB2Zq+xa4/dTzDDYLu7jyEGODG9QZxxgKy2l7HX3S8gXAFMurY52j2zWv8e
t3fCZXhXpBGf3DC7/n26RP0OotQM5kbFlSQILmDPA/MdYiQJ7xDCbbStd2W8KtDv
Qww6lJIDI8IyY85cGCmXE44Fadk3F+AWxGLayHNboH5QGLxvfrO8r32SNEZbnGTv
tcGBW45OfgivnFP3c4MF3Jmzzp4Pd8sK1a2H14FviRinSWA=\n
-----END CERTIFICATE-----";

// Configura el certificado de la CA
espClient.setCACert(ca_cert);


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

static void
callback(char *topic, byte *payload, unsigned int length)
{
    char *pl;

    payload[length] = 0;
    Serial.printf( "For topic: \"%s\", message \"%s\"\n", topic, payload );
    if( ( pl = strrchr(topic,'/') ) == NULL )
        return;
    Serial.printf("main topic = %s\n", ++pl );

    for( int i = 0; topics[i].sub_topic != NULL; ++i )
        if( strcmp( pl, topics[i].sub_topic ) == 0 )
        {
            (*topics[i].action)( (char *)payload );
            break;
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

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    while (!client.connected())
    {
        if (client.connect(client_id, mqttUser, mqttPassword))
            Serial.printf( "%s: client %s connected\n", __FUNCTION__, client_id );
        else
        {
            Serial.printf("%s: failed with state = %d\n", __FUNCTION__, client.state());
            delay(2000);
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


