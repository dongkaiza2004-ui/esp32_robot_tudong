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

TaskHandle_t ROSHandle;
TaskHandle_t MotorHandle;
void ROS_Task(void *parameter) {
  esp_task_wdt_add(NULL); 
  while (1) {
    ROS_Spin(); 
    unsigned long now = millis();
    if (now - last_odom >= 100) {  
      ROS_Send_Feedback_Odom();
      last_odom = now;
    }
    esp_task_wdt_reset(); 
    vTaskDelay(pdMS_TO_TICKS(10)); 
  }
}
void Motor_Task(void *parameter) {
  esp_task_wdt_add(NULL); 
  for (;;) {
    if(kt > 0){
      if(straight > 1){
        straight --;
        speed_R_L(-0.2, 0, target_freq_l, target_freq_r);
      }
      else{
        if(pulses_n > 0){
          speed_R_L(0, 0.5, target_freq_l, target_freq_r);
        }
        else{
          if(straight2 > 0){
            straight2--;
            speed_R_L(0.5, 0, target_freq_l, target_freq_r);
          }
          else{
            x = 100;
            if(straight3 > 0){
              straight3--;
              speed_R_L(0, 0, target_freq_l, target_freq_r);
            }
            else{
              if(straight4 > 0){
                straight4--;
                speed_R_L(-0.5, 0, target_freq_l, target_freq_r);
              }
              else{
                kt = 0;
                x= 0;
                speed_R_L(0, 0, target_freq_l, target_freq_r);
              }
            }
          }

        }
      }

    }
    Control_Task(); 
    esp_task_wdt_reset(); 
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void setup() {
  Serial.begin(115200);
  Motor_Init();
  pinMode(R_MOTOR_PWM_A, INPUT_PULLUP);
  pinMode(L_MOTOR_PWM_A, INPUT_PULLUP);
  pid_r.reset();
  pid_l.reset();
  last_time = millis();
  ROS_Init();
  xTaskCreatePinnedToCore(ROS_Task, "ROS_Task",8192, NULL, 1, &ROSHandle, 0);
  xTaskCreatePinnedToCore(Motor_Task, "Motor_Task",4096, NULL, 2, &MotorHandle, 1);
  esp_task_wdt_init(2, true); 

}

void loop() {
  // Serial.printf("v = %.2f, w = %.2f, x = %.2f\n", v,w,x);
  // Serial.print(pulses_n);
  // Serial.print("\n");
  if(straight == 1){
    if(pulses_n > 0){
      if(flag_motor_l_a){
          pulses_n--;
          flag_motor_l_a = false;
      }
    }
  }
  pwm_x = int(x);
  control_x();
}
