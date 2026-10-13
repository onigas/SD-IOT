#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu;
constexpr uint32_t INTERVAL_MS = 500;
void setup() {
  Serial.begin(115200); delay(1000);
  Wire.begin(21,22);
  Serial.println("Task 4 - scanare I2C");
  for (uint8_t adr=1; adr<127; adr++) {
    Wire.beginTransmission(adr);
    if (Wire.endTransmission()==0) Serial.printf("I2C: 0x%02X\n",adr);
  }
  // AD0=0: 0x68; AD0=1: 0x69.
  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("MPU6050 absent. Verificati cablarea/adresa 0x69.");
    while (true) delay(100);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
void loop() {
  sensors_event_t a,g,t;
  mpu.getEvent(&a,&g,&t);
  Serial.printf("a[m/s^2] X=%.2f Y=%.2f Z=%.2f\n",
    a.acceleration.x,a.acceleration.y,a.acceleration.z);
  Serial.printf("w[rad/s] X=%.3f Y=%.3f Z=%.3f; T=%.1f C\n",
    g.gyro.x,g.gyro.y,g.gyro.z,t.temperature);
  delay(INTERVAL_MS);
}
