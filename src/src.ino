/* 
 * -----------------------------------------------------------------------------
 * Example: Two way communication between ESP32 and Python using WIFI
 * -----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 07-05-2023 (7th of May, 2023)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#include "config.h"
#include "my_wifi.h"
#include "wifi_communicator.h"

#include <MyButton.h>

#define ENABLE_DEBUG /* <-- Commenting this line will remove any trace of debug printing */
#include <MacroDebugger.h>

// Button object to simulate input events
MyButton my_btn(BTN_PIN, NORMAL_DOWN, 50);

// Communication messages
char incoming_msg[MAX_BUFFER_LEN] = {0};
char response[MAX_BUFFER_LEN] = {0};

/* A collection of random responses to send when the button is clicked */
#define NUM_RANDOM_RESPONSES    10
char *responses[NUM_RANDOM_RESPONSES] = {
  "hola!",
  "hiii",
  "potato",
  "arduino",
  "esp32",
  "okay so we get it it's a random message!",
  "so what?",
  "running out of messages here",
  "okay two more to go",
  "finally ..."
};

void setup(){
  DEBUG_BEGIN();
  
  setup_wifi();
  
  setup_wifi_communicator();

  pinMode(LED_PIN, OUTPUT);

  DEBUG_I("Done setting up!");
}

void loop(){

  // if we lost connection, we attempt to reconnect (blocking)
  if(!is_client_connected()){ connect_client(); }
  
  bool received = get_message(incoming_msg);

  if(received){
    DEBUG_I("Received: %s", incoming_msg);
    uint8_t start = 0;

    if(incoming_msg[0] == 'A'){
      sprintf(response, "%s", ACK);
      start++;
    }

    //switch the number and do the appropriate action
    switch(incoming_msg[start]){
      case 'n':
        digitalWrite(LED_PIN, HIGH);
        break;

      default:
      case 'f':
        digitalWrite(LED_PIN, LOW);
        break;
    }

    // If start is bigger than 0, then we have to acknowledge the reception
    if(start > 0){
      send_message(response);
      // Clear the response buffer
      memset(response, 0, MAX_BUFFER_LEN);
    }
  }

  if(my_btn.readRisingClick()){
    // Choose a random response to send back
    uint8_t idx = random(NUM_RANDOM_RESPONSES);
    strncpy(response, responses[idx], MAX_BUFFER_LEN);
    send_message(response);
    memset(response, 0, MAX_BUFFER_LEN);
    DEBUG_I("Sent: %s", responses[idx]);
  }
}
