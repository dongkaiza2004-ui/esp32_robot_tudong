#include <Arduino.h>
#include "motor.h"
#include "pid.h"
#include "pinout.h"
#include "maths.h"
#include "ros_esp32_cmdvel.h"
#include "esp_task_wdt.h"

unsigned long last_time = 0;
unsigned long last_odom = 0;
const unsigned long interval = 500; 

TaskHandle_t MotorHandle;
void Motor_Task(void *parameter) {
  esp_task_wdt_add(NULL); 
  for (;;) {
    if(ok){
      math_control1();
    }
    if(ok1 > 0){
      math_control3();
    }
    if(kt > 0){
      math_control2();
    }
    Control_Task(); 
    esp_task_wdt_reset(); 
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void setup() {
  esp_task_wdt_add(NULL); 
  esp_task_wdt_init(5, true); 
  Serial.begin(115200);
  Serial.println("motor starting...");
  Motor_Init();
  pinMode(R_MOTOR_PWM_A, INPUT_PULLUP);
  pinMode(L_MOTOR_PWM_A, INPUT_PULLUP);
  pid_r.reset();
  pid_l.reset();
  last_time = millis();
  esp_task_wdt_reset();
  xTaskCreatePinnedToCore(Motor_Task, "Motor_Task",4096, NULL, 2, &MotorHandle, 1);
  ok = 1;
  TH = 1;
  straight = 1900;
}

void loop() {
  esp_task_wdt_reset();
  pwm_x = int(x);
  control_x();
}
