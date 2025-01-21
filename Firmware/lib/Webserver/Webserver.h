/*   Webserver is handled Here   */


// Definitions
#define WebserverPort           80
#define WebsocketURL            "/CommunicationSocket"
#define MulticastDomainName     "dronecontrol"

#define CodeOK                  200               
#define CodeCreated             201
#define CodeNoContent           204
#define CodeBadRequest          400
#define CodeUnauthorized        401
#define CodeForbidden           403
#define CodeNotFound            404
#define CodeMethodNotAllowed    405
#define CodeInternalServerError 500
#define CodeBadGateway          502
#define CodeServiceUnavailable  503
#define CodeGatewayTimeout      504
#define CodeMovedPermanently    301
#define CodeFound               302
#define CodeNotModified         304

typedef struct {
    int Slider1, Slider2, Slider3, Slider4;
    bool Toggle1, Toggle2, Toggle3, Toggle4;
    float Joystick1X, Joystick2X, Joystick1Y, Joystick2Y;
} InputData;


// Includes
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <AsyncUDP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>


// Webserver Setup
#include <Webpage.h>
extern QueueHandle_t InputQueue;

void StartWiFi();
void StartWebserver();
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void SendInputData(uint8_t *data, size_t len);