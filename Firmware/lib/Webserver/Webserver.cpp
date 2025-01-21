#include <Webserver.h>


// Objects
AsyncWebServer myServer(WebserverPort);
AsyncWebSocket mySocket(WebsocketURL);
QueueHandle_t InputQueue;

void StartWiFi()
{
    // WiFi.mode(WIFI_MODE_AP);
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin("Wifi", "LinkinPark");
    // WiFi.begin("Testing", "dontaskme");

    // Wait till Connection
    Serial.print("\nConnecting ");
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
    Serial.print("\tConnected!!\n");
    
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    vTaskDelay(100 /portTICK_RATE_MS);

    // Multicast DNS 
    MDNS.begin(MulticastDomainName);
}

void StartWebserver()
{
    myServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(CodeOK, "text/html", Webpage);});
    myServer.addHandler(&mySocket);
    myServer.begin();
    mySocket.onEvent(onWebSocketEvent);

    // Define Queue
    InputQueue = xQueueCreate(2, sizeof(InputData));
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("Connected\t: #%u %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DATA:
        SendInputData(data, len);
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("Disconnected\t: %s",  client->remoteIP().toString().c_str());
        break;
    }
}

void SendInputData(uint8_t *data, size_t len)
{
    JsonDocument myJSON;
    InputData newData;

    DeserializationError error = deserializeJson(myJSON, data, len);
    if (error) {
        // Serial.print("JSON Parsing Failed:");
        // Serial.println(error.c_str());
        return;
    }

    newData.Slider1 = int(myJSON["S1"]);
    newData.Slider2 = int(myJSON["S2"]);
    newData.Slider3 = int(myJSON["S3"]);
    newData.Slider4 = int(myJSON["S4"]);

    newData.Toggle1 = bool(myJSON["T1"]);
    newData.Toggle2 = bool(myJSON["T2"]);
    newData.Toggle3 = bool(myJSON["T3"]);
    newData.Toggle4 = bool(myJSON["T4"]);

    newData.Joystick1X = float(myJSON["J1"]["X"]);
    newData.Joystick1Y = float(myJSON["J1"]["Y"]);
    newData.Joystick2X = float(myJSON["J2"]["X"]);
    newData.Joystick2Y = float(myJSON["J2"]["Y"]);

    xQueueSend(InputQueue, &newData, 0);
}