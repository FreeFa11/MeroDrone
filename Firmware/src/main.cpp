#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <USB.h>

#include <LittleFS.h>
#include <Preferences.h>

#include <Webserver.h>
#include <Sensor.h>
#include <Motor.h>


// MAC: 80:65:99:FB:EB:D4
extern float AX, AY, AZ, GX, GY, GZ;


void Controller(void *param);
TaskHandle_t ControllerTask;
void Blinky(void *param);
TaskHandle_t BlinkyTask;


void setup()
{
    Serial.begin(115200);

    // Wifi & Server
    StartWiFi();
    StartWebserver();

    // Sensor
    // SetupIMU();
    // SetupCompass();
    // StartSensors();

    // Motor
    SetupMotor();
    StartMotor();

    xTaskCreatePinnedToCore(
        Controller,
        "Controller",
        4096,
        NULL,
        1,
        &ControllerTask,
        0
    );
    xTaskCreatePinnedToCore(
      Blinky,
      "Blinky",
      1000,
      NULL,
      1,
      &BlinkyTask,
      0
    );
}
void loop()
{   vTaskDelete(NULL);   }


void Controller(void *param)
{
    while(true)
    {
        // static SensorData Attitude;
        static MotorData Motor;
        static InputData Input;

        // if (xQueueReceive(SensorQueue, &Attitude, 0) == pdTRUE)
        // {
        //     Serial.print(Attitude.Pitch);
        //     Serial.print("\t");
        //     Serial.println(Attitude.Roll);
            // Serial.print(">Pitch:");
            // Serial.println(Attitude.Pitch);
            // Serial.print(">Roll:");
            // Serial.println(Attitude.Roll);
            // Serial.print(">AcceleroX:");
            // Serial.println(AX);
            // Serial.print(">AcceleroY:");
            // Serial.println(AY);
            // Serial.print(">AcceleroZ:");
            // Serial.println(AZ);
            // Serial.print(">GyroX:");
            // Serial.println(GX);
            // Serial.print(">GyroY:");
            // Serial.println(GY);
            // Serial.print(">GyroZ:");
            // Serial.println(GZ);
            
            // Serial.print(">Yaw:");
            // Serial.println(Attitude.Yaw);
        // }

        if(xQueueReceive(InputQueue, &Input, 0) == pdTRUE)
        {
            Motor.One = 1024 - 9 * Input.Slider1;
            Motor.Two = 1024 - 9 * Input.Slider1;
            Motor.Three = 1024 - 9 * Input.Slider1;
            Motor.Four = 1024 - 9 * Input.Slider1;

            xQueueSend(MotorQueue, &Motor, 1);
        }

        vTaskDelay(4 / portTICK_RATE_MS);
    }
}

void Blinky(void *param)
{
  static bool STATE;
  pinMode(BUILTIN_LED, OUTPUT);

  while(true)
  {
    digitalWrite(BUILTIN_LED, STATE);
    STATE = !STATE;

    vTaskDelay(500 / portTICK_RATE_MS);
  }
}