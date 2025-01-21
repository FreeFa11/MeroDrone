/*   Motor Output are handled Here   */


// Definition
#define FREQUENCY               30000
#define RESOLUTION              10
#define MotorOnePin             2
#define MotorTwoPin             3
#define MotorThreePin           4
#define MotorFourPin            5
#define MotorOneChannel         0
#define MotorTwoChannel         1
#define MotorThreeChannel       2
#define MotorFourChannel        3

typedef struct {
    int One, Two, Three, Four;
} MotorData;


// Includes
#include <Arduino.h>
#include <freertos/FreeRTOS.h>


// Ouput Setup
extern QueueHandle_t MotorQueue;
extern TaskHandle_t MotorTask;

void SetupMotor();
void StartMotor();
void SendMotorPWM(void *param);