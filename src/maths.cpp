#include "maths.h"
#include <Arduino.h>
#include "motor.h"
float v = 0.163;
float w = 0;
float x = 0;
float R = 0.035; 
float L = 0.33;
float kt = 0;
float straight = 0;
float straight2 = 0;
float straight3 = 0;
float straight4 = 0;
float base_freq = 0;
float target_freq_l = base_freq;
float target_freq_r = base_freq;

float freq_r = 0;
float freq_l = 0;

float rpm_r;
float rpm_l;
int pwm_r = 0;
int pwm_l = 0;
int pwm_x = 0;
pid pid_r(0.2, 0.01, 0.005);
pid pid_l(0.2, 0.01, 0.005);
int dir_r = 0;
int dir_l = 0;
volatile bool control_flag = false;

float v_feedback = 0;
float w_feedback = 0;
int pulses_n = 0;
const int pulses_per_rev = 390;
float RPS_to_speed(float RPS){
    return (2*pi*RPS*R);
}
float speed_to_RPS(float v){
    return v/(R*2*pi);
}
void speed_R_L(float v,float w,float &freq1_l, float &freq1_r){
    float vr = v + (w*L)/2;
    float vl = v - (w*L)/2; 
    float RPS_r = speed_to_RPS(vr);
    float RPS_l = speed_to_RPS(vl);
    freq1_r = RPS_r*pulses_per_rev;
    freq1_l = RPS_l*pulses_per_rev;
}
void freq_to_speed(float freq1_l, float freq1_r) {
    float RPS_l = freq1_l / pulses_per_rev;
    float RPS_r = freq1_r / pulses_per_rev;

    float v_l = RPS_to_speed(RPS_l);
    float v_r = RPS_to_speed(RPS_r);

    v_feedback = (v_l + v_r) / 2.0f;
    w_feedback = (v_r - v_l) / L;
}
void math_cung(float n){
    float l = (pi*n*(L/2))/180;
    float d = 2*pi*R;
    pulses_n = l*480/d;
}
