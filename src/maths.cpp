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
int TH = 0;
float ok = 0;
float ok1 = 0;
float finish;

float base_freq = 0;
float target_freq_l = base_freq;
float target_freq_r = base_freq;

float freq_r = 0;
float freq_l = 0;

int flag = 0;

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
int pulses_n_l = 0;
int pulses_n_r = 0;
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
    pulses_n_l = l*480/d;
    pulses_n_l = pulses_n_r;
}
void math_control1(){
    switch (TH)
    {
    case 1:
        Serial.print("da va case 1\n");
        speed_R_L(0.2, 0, target_freq_l, target_freq_r);
        straight-- ;
        if(straight < 1){
            TH = 2;
            straight = 700;
        }
        break;
    case 2:
        Serial.print("da va case 2\n");
        speed_R_L(0.1, 0.4, target_freq_l, target_freq_r);
        straight-- ;
        if(straight < 1){
            TH = 3;
            straight = 4000;
        }   
        break; 
    case 3 :
        Serial.print("da va case 3\n");
        speed_R_L(0.1, 0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 4;
        }
        break;
    
    default:
        Serial.print("chuyen sang control3\n");
        straight = 400;
        speed_R_L(0, 0, target_freq_l, target_freq_r);
        ok = 0;
        TH = 1;
        ok1 = 1;
        break;
    }
}
void math_control2(){
    switch (TH)
    {
    case 1:
        Serial.print("da vao tra bong case1 \n");
        speed_R_L(-0.2, 0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 2;
            straight = 1200;
        }
        break;
    case 2:
        Serial.print("da vao tra bong case2 \n");
        speed_R_L(0, 0.5, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 3;
            straight = 800;
        }   
        break; 
    case 3 :
        Serial.print("da vao tra bong case3 \n");
        speed_R_L(0.5, 0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 4;
            straight = 1000;
        }
        break;
    case 4 :
        Serial.print("da vao tra bong case4 \n");
        speed_R_L(0, 0, target_freq_l, target_freq_r);
        x = 100;
        straight -- ;
        if(straight < 1){
            TH = 5;
            straight = 700;
        }
        break;
    case 5 :
        speed_R_L(-0.5, 0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 6;
        }
        break;
    default:
        x = 0;
        speed_R_L(0, 0, target_freq_l, target_freq_r);
        break;
    }
}
void math_control3(){
    switch (TH)
    {
    case 1:
        Serial.print("da vao lay bong case 1\n");
        speed_R_L(0.0, -0.5, target_freq_l, target_freq_r);
        straight-- ;
        if(straight < 1){
            TH = 2;
            straight = 400;
        }   
        break; 
    case 2:
        Serial.print("da vao lay bong case 2\n");
        x = -200;
        speed_R_L(0.1,0.0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 3;
            straight = 1000;
        }   
        break; 
    case 3 :
        Serial.print("da vao tra bong case 3\n");
        speed_R_L(0, 0, target_freq_l, target_freq_r);
        straight -- ;
        if(straight < 1){
            TH = 4;
        }
        break;
    default:
        x = 0;
        ok1 = 0;
        kt = 1;
        TH =1;
        straight = 500;
        break;
    }
}
