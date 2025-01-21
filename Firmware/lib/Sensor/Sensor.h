/*   Sensors are handled Here   */


// Definitions
#define SDAWire             18
#define SCLWire             16
#define SDAWire1            2
#define SCLWire1            15

#define AlphaLPF            0.3
#define AlphaCF             0.2
#define IMUDelay            100

typedef struct {
    float Pitch, Roll, Yaw;
} SensorData;


// Includes
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include "Wire.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "math.h"


// Sensors Setup
extern QueueHandle_t SensorQueue;
extern TaskHandle_t SensorTask;

void SetupIMU();
void SetupCompass();
void EstimateEulerAngles(float *Pitch, float *Roll, float *Yaw);
void SendSensorData(void* param);
void StartSensors();
void ReadBias(int Iteration);