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

#ifndef __WIFI_COMMUNICATOR_H__
#define __WIFI_COMMUNICATOR_H__

#include "my_wifi.h"
#include "config.h"

static SemaphoreHandle_t _send_tsk_mutex;
static SemaphoreHandle_t _recv_tsk_mutex;

static QueueHandle_t _send_q;
static QueueHandle_t _recv_q;
static TaskHandle_t _socket_reporter_task_h = NULL;

// The sockets client
static WiFiClient _client;

/*
  Attempt to connect the client
*/
void connect_client(){
  // We have to connect, no other options
  while(!_client.connect(SERVER_ADDRESS, SERVER_PORT)){ delay(1000); }
}

bool is_client_connected(){
  return _client.connected();
}


void send_message(char *msg){
  xQueueSend(_send_q, (void*)msg, 5);
}

bool get_message(char *msg){
  return xQueueReceive(_recv_q, (void*)msg, 5) == pdTRUE;
}

/*
  This task would wait for the send signal and send whatever in the sending queue
*/
static void sender_task(void*){
  // Unless conneted to the client, no need for this to run
  xSemaphoreTake(_send_tsk_mutex, portMAX_DELAY);

  char buff[MAX_BUFFER_LEN] = {0};
  while(1){
    // Wait for a notification to do anything
    if(xQueueReceive(_send_q, (void*)&buff, 5) == pdTRUE){
      _client.print(buff);
    }
  }
}

/*
  This task would wait for data to be present, and signal that something has been received
*/
static void receiver_task(void*){
  // Unless conneted to the client, no need for this to run
  xSemaphoreTake(_recv_tsk_mutex, portMAX_DELAY);

  char buf[MAX_BUFFER_LEN] = {0};
  while(1){
    // wait until data is available
    while(_client.available() <= 0){ delay(100); };

    // if here, then we have data to read
    for(uint8_t i = 0; i < _client.available(); i++){ buf[i] = (char)_client.read(); };

    // Add message to the received messages queue
    xQueueSend(_recv_q, (void*)&buf, 5);
  }
}

/*
  This function would initialize the communicator and setup everything
*/
void setup_wifi_communicator(){
  _send_q = xQueueCreate(QUEUE_LEN, MAX_BUFFER_LEN);
  _recv_q = xQueueCreate(QUEUE_LEN, MAX_BUFFER_LEN);
  _recv_tsk_mutex = xSemaphoreCreateMutex();
  _send_tsk_mutex = xSemaphoreCreateMutex();
    
  // block both tasks once created for them to wait for the client to connect
  xSemaphoreTake(_send_tsk_mutex, portMAX_DELAY);
  xSemaphoreTake(_recv_tsk_mutex, portMAX_DELAY);
  
  // Create tasks
  xTaskCreatePinnedToCore(sender_task, "sendTask", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(receiver_task, "receiveTask", 4096, NULL, 3, NULL, 1);

  connect_client();

  // release tasks
  xSemaphoreGive(_send_tsk_mutex);
  xSemaphoreGive(_recv_tsk_mutex);
}

#endif // __WIFI_COMMUNICATOR_H__
