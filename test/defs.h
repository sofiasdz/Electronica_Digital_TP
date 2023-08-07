/*
 *  defs.h
 *      MQTT must be organized without collisions.
 *
 *      In this example, we have the following order
 *      MAIN_NAME (defined in platformio.ini): is the same for all projects
 *      SUB_NAME (also defined in platformio.ini)
 *      GROUP: group number form 0 to 8 inclusive as two digits:
 *          first digit is group number, second digit is user in each group 
 *          it is convrenient use group with second digit in 0 when testing integrated project and using
 *          second digit from 1 to 9 for each member which tests independently
 *      BOARD: obtained from BIT0 and BIT1 in execution
 */

/*
 *  MQTT client_id structure
 *
 *  MAIN_NAME+SUB_NAME+GROUP+BOARD as an string
 *  Example
 *  "Austral_bio_12_02"
 */

/*
 *  MQTT topic definitions
 */

/*
 *  topics for publishing are as follows
 *      MAIN_NAME/SUB_NAME/GROUP/BOARD/topic
 *  Example for topic "button"
 *      "Austral/ind/00/03/button"
 */

const char* client_name = "mqtt_test";


