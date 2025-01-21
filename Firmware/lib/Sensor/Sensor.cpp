#include <Sensor.h>



// Objects
MPU6050 IMU;
HMC5883L Compass;
QueueHandle_t SensorQueue;
TaskHandle_t SensorTask;

float AX, AY, AZ, GX, GY, GZ, MX, MY, MZ;
float AXB, AYB, AZB, GXB, GYB, GZB;
float AccPitch, AccRoll, GyroPitch, GyroRoll, GyroRollRate, GyroPitchRate;
float EulerRoll, EulerPitch;
int CurrentTick, Ticks;



void SetupIMU()
{
  Wire.begin(SDAWire, SCLWire);
  IMU.initialize(ACCEL_FS::A2G, GYRO_FS::G250DPS);
}

void SetupCompass()
{
  // This is for Powering the Compass Module (1mA)
  // pinMode(32, OUTPUT); pinMode(33, OUTPUT);
  // digitalWrite(32, HIGH); digitalWrite(33, LOW);

  // Wire.begin(SDAWire, SCLWire);
  // Compass.initialize(); 
  // Compass.setDataRate(HMC5883L_RATE_75);
  // Compass.setSampleAveraging(HMC5883L_AVERAGING_8);

}

void ReadBias(int Iteration)
{
  for (int i = 0; i < Iteration; i++)
    {
      // AXB += IMU.getAccelerationX();
      // AYB += IMU.getAccelerationY();
      // AZB += IMU.getAccelerationZ();
      GXB += IMU.getRotationX();
      GYB += IMU.getRotationY();
      GZB += IMU.getRotationZ();

      vTaskDelay(40 * portTICK_RATE_MS);
    }
    // AXB = AXB / Iteration;
    // AYB = AYB / Iteration;
    // AZB = AZB / Iteration;
    GXB = GXB / Iteration;
    GYB = GYB / Iteration;
    GZB = GZB / Iteration;
}

void EstimateEulerAngles(float *Pitch, float *Roll, float *Yaw)
{

// Pitch & Roll
  // Filtered Data
  AX = AlphaLPF * (IMU.getAccelerationX() - AXB) + (1 - AlphaLPF) * AX;
  AY = AlphaLPF * (IMU.getAccelerationY() - AYB) + (1 - AlphaLPF) * AY;
  AZ = AlphaLPF * (IMU.getAccelerationZ() - AZB) + (1 - AlphaLPF) * AZ;
  GX = AlphaLPF * (IMU.getRotationX() - GXB) + (1 - AlphaLPF) * GX;
  GY = AlphaLPF * (IMU.getRotationY() - GYB) + (1 - AlphaLPF) * GY;
  GZ = AlphaLPF * (IMU.getRotationZ() - GZB) + (1 - AlphaLPF) * GZ;

  // **Time for Integral** //
  CurrentTick = xTaskGetTickCount();
  Ticks = CurrentTick - Ticks;

  // Acce Attitude
  AccPitch = atanf(-AX/AZ);
  AccRoll = atanf(AY/AZ);

  // Gyro Attitude
  GyroRollRate  = GX +  GY * sinf(AccRoll) * tanf(AccPitch) + GZ * cosf(AccRoll) * tanf(AccPitch);     // Careful about the Angle Units
  GyroPitchRate = GY * cosf(AccRoll) - GZ * sinf(AccRoll);

  // Feedback
  GyroPitch = EulerPitch + 0.0001f * GyroPitchRate * Ticks;
  GyroRoll = EulerRoll + 0.0001f * GyroRollRate * Ticks;

  // Complementary Filter
  EulerPitch = AlphaCF * AccPitch * 90.0 + (1 - AlphaCF) * GyroPitch;       // Scaled by 90 in this case
  EulerRoll = AlphaCF * AccRoll * 90.0 + (1 - AlphaCF) * GyroRoll;

  *Pitch = EulerPitch;
  *Roll = EulerRoll;
  *Yaw = float(Ticks);

  // **Time for Integral** //
  Ticks = CurrentTick;
}

void SendSensorData(void* param)
{
  SensorData newData;

  while(true)
  {
    EstimateEulerAngles(&newData.Pitch, &newData.Roll, &newData.Yaw);
    xQueueSend(SensorQueue, &newData, 0);

    vTaskDelay(IMUDelay / portTICK_RATE_MS);
  }
}

void StartSensors()
{
  SensorQueue = xQueueCreate(2, sizeof(SensorData));

  ReadBias(100);

  xTaskCreatePinnedToCore(
    SendSensorData,
    "SensorTask",
    2048,
    NULL,
    1,
    &SensorTask,
    0
  );
}
