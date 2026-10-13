// Implementare didactica: MPU6050, LittleFS, SSE si integrare omega*dt.
// Interfata JS adaptata separat din Random Nerd Tutorials.
#include "lab_config.h"
#include <Wire.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include <LittleFS.h>
Adafruit_MPU6050 mpu;
AsyncWebServer server(80);
AsyncEventSource events("/events");
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
uint8_t cerereReset = 0;
float unghi[3] = {0,0,0}, bias[3] = {0,0,0};
uint32_t anteriorUs = 0, ultimaTrimitere = 0;
bool gata = false;
void rutaReset(const char* ruta, uint8_t masca) {
  server.on(ruta, HTTP_POST, [masca](AsyncWebServerRequest* r) {
    portENTER_CRITICAL(&mux); cerereReset |= masca;
    portEXIT_CRITICAL(&mux);
    r->send(200,"text/plain","OK");
  });
}
void setup() {
  Serial.begin(115200); delay(1000);
  Serial.println("Task 5 - MPU6050 web");
  Wire.begin(21,22);
  if (!mpu.begin(0x68,&Wire)) {
    Serial.println("MPU6050 absent: verificati Task 4."); return;
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("Tineti senzorul nemiscat pentru calibrare (2 s).");
  delay(1000);
  for (int i=0;i<200;i++) {
    sensors_event_t a,g,t; mpu.getEvent(&a,&g,&t);
    bias[0]+=g.gyro.x; bias[1]+=g.gyro.y; bias[2]+=g.gyro.z;
    delay(10);
  }
  for (int k=0;k<3;k++) bias[k]/=200.0f;
  if (!LittleFS.begin(false)) {
    Serial.println("LittleFS absent: incarcati data/ si reporniti."); return;
  }
  const char* fisiere[] = {"/index.html","/script.js","/style.css",
                         "/three.min.js"};
  for (const char* f : fisiere) {
    if (!LittleFS.exists(f)) {
      Serial.printf("Lipseste %s; incarcati data/ complet.\n",f); return;
    }
  }
  if (!conectareWiFi()) return;
  server.on("/",HTTP_GET,[](AsyncWebServerRequest* r) {
    r->send(LittleFS,"/index.html","text/html; charset=utf-8");
  });
  server.serveStatic("/",LittleFS,"/");
  rutaReset("/reset",7); rutaReset("/resetX",1);
  rutaReset("/resetY",2); rutaReset("/resetZ",4);
  server.addHandler(&events); server.begin();
  anteriorUs=micros(); gata=true;
  Serial.println("Server pornit. Date SSE la aproximativ 100 ms.");
}
void loop() {
  if (!gata) { delay(100); return; }
  uint32_t acum=micros();
  if (acum-anteriorUs < 10000) { delay(1); return; }
  float dt=(acum-anteriorUs)*1e-6f; anteriorUs=acum;
  sensors_event_t a,g,t; mpu.getEvent(&a,&g,&t);
  float w[3]={g.gyro.x,g.gyro.y,g.gyro.z};
  portENTER_CRITICAL(&mux);
  uint8_t reset=cerereReset; cerereReset=0;
  portEXIT_CRITICAL(&mux);
  for (int k=0;k<3;k++) {
    if (reset & (1<<k)) unghi[k]=0;
    else if (dt<0.1f) unghi[k]+=(w[k]-bias[k])*dt;
  }
  if (millis()-ultimaTrimitere < 100) return;
  ultimaTrimitere=millis();
  JSONVar rot,acc;
  rot["gyroX"]=unghi[0]; rot["gyroY"]=unghi[1]; rot["gyroZ"]=unghi[2];
  acc["accX"]=a.acceleration.x; acc["accY"]=a.acceleration.y;
  acc["accZ"]=a.acceleration.z;
  String j=JSON.stringify(rot), q=JSON.stringify(acc);
  events.send(j.c_str(),"gyro_readings",ultimaTrimitere);
  events.send(q.c_str(),"accelerometer_readings",ultimaTrimitere);
  events.send(String(t.temperature,1).c_str(),"temperature_reading",
              ultimaTrimitere);
}
