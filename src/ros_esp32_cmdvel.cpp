#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>
#include <WiFi.h>
#include "maths.h"
#include <nav_msgs/msg/odometry.h>
#include <rosidl_runtime_c/string_functions.h>

rcl_publisher_t publisher_feedback;
nav_msgs__msg__Odometry odom_msg;
rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg_sub;
rcl_node_t node;
rcl_allocator_t allocator;
rclc_support_t support;
rclc_executor_t executor;

#define LED_PIN 4
#define RCCHECK(x) if((x) != RCL_RET_OK){Serial.println(" ❌RCCHECK lỗi"); error_loop();}
#define RCSOFTCHECK(x) if((x) != RCL_RET_OK){}

void error_loop() {
  while (1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}
const char * ssid = "PTIT-RD-01";
const char * password = "11246879";
const char * ip_address = "192.168.0.131";
const uint16_t port = 8887;
// const char * ssid = "IEC lab";
// const char * password = "roboticsptit";
// const char * ip_address = "192.168.2.130";
// const uint16_t port = 8888;
void Odometry_Init() {
  rosidl_runtime_c__String__assign(&odom_msg.header.frame_id, "odom");
  rosidl_runtime_c__String__assign(&odom_msg.child_frame_id, "base_footprint");
}
void ROS_Send_Feedback_Odom() {
  unsigned long now = millis();
  odom_msg.header.stamp.sec = now / 1000;
  odom_msg.header.stamp.nanosec = (now % 1000) * 1000000;

  freq_to_speed(freq_l, freq_r);
  
  odom_msg.twist.twist.linear.x = v_feedback; 
  odom_msg.twist.twist.linear.y = finish;
  odom_msg.twist.twist.angular.z = w_feedback;

  RCSOFTCHECK(rcl_publish(&publisher_feedback, &odom_msg, NULL));

}

void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;
  v = msg->linear.x;
  x = msg->linear.y;
  w = msg->angular.z;
  kt = msg->angular.x;
  if(kt > 0){
    flag ++;
  }
  if(flag == 1){
    TH =1;
    straight = 500;
    Serial.print("da vao 1 lan \n");
  }
  if(flag == 5 ){
    flag =2;
  }
  // if(k>0){
  //   Serial.print("da nhan n");
  //   math_cung(n);
  // }
  speed_R_L(v, w, target_freq_l, target_freq_r);

}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi connected!");
}

void ROS_Init() {
  // Serial.begin(115200);
  // delay(1000);
  connectToWiFi(); 
  set_microros_wifi_transports((char *)ssid, (char *)password, (char *)ip_address, port);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "esp32_node", "", &support));

  RCCHECK(rclc_subscription_init_best_effort(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"
  ));
  RCCHECK(rclc_publisher_init_default(
    &publisher_feedback,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
    "esp32_feedback"
  ));
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg_sub, &subscription_callback, ON_NEW_DATA));
  Odometry_Init();
  Serial.println("✅ ROS2 initialized");
}

void ROS_Spin() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(20));
}
