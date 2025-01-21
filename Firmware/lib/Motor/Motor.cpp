#include <Motor.h>



// Objects
QueueHandle_t MotorQueue;
TaskHandle_t MotorTask;

void SetupMotor()
{
    ledcSetup(MotorOneChannel, FREQUENCY, RESOLUTION);
    ledcSetup(MotorTwoChannel, FREQUENCY, RESOLUTION);
    ledcSetup(MotorThreeChannel, FREQUENCY, RESOLUTION);
    ledcSetup(MotorFourChannel, FREQUENCY, RESOLUTION);

    ledcAttachPin(MotorOnePin, MotorOneChannel);
    ledcAttachPin(MotorTwoPin, MotorTwoChannel);
    ledcAttachPin(MotorThreePin, MotorThreeChannel);
    ledcAttachPin(MotorFourPin, MotorFourChannel);
    
}

void StartMotor()
{
    MotorQueue = xQueueCreate(2, sizeof(MotorData));

    xTaskCreatePinnedToCore(
        SendMotorPWM,
        "MotorTask",
        2048,
        NULL,
        1,
        &MotorTask,
        0
    );
}

void SendMotorPWM(void *param)
{   
    MotorData newData;

    while (true)
    {
        if (xQueueReceive(MotorQueue, &newData, 0) == pdTRUE)
        {
            ledcWrite(MotorOneChannel, newData.One);
            ledcWrite(MotorTwoChannel, newData.Two);
            ledcWrite(MotorThreeChannel, newData.Three);
            ledcWrite(MotorFourChannel, newData.Four);
        }

        vTaskDelay(5 / portTICK_RATE_MS);
    }
}