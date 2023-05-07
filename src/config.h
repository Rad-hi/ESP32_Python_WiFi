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

#ifndef __CONFG_H___
#define __CONFG_H___

/* Pins definitions */
#define LED_PIN                     33
#define BTN_PIN                     25

/* Communication params */
#define ACK                         "A" // acknowledgment packet
#define QUEUE_LEN                   5
#define MAX_BUFFER_LEN              128

/* WiFi params */
#define WIFI_SSID                   "test"
#define WIFI_PASSWORD               "123456789"

/* Socket */
#define SERVER_ADDRESS              "PYTHON.IP.GOES.HERE"
#define SERVER_PORT                 11111

#endif // __CONFG_H___
