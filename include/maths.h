#ifndef __MATHS_H
#define __MATHS_H
#include "pid.h"
#define pi 3.1415926f
extern const int pulses_per_rev;

extern int pulses_n_l ;
extern int pulses_n_r ;

extern float R; 
extern float L;

extern float v;
extern float w;
extern float x;
extern float kt;

extern int flag;

extern float straight;
extern int TH;
extern int TH1;
extern float ok;
extern float ok1;
extern float finish;

extern float v_feedback;
extern float w_feedback;

extern float base_freq;
extern float target_freq_l;
extern float target_freq_r;

extern float rpm_r;
extern float rpm_l;
extern int pwm_r;
extern int pwm_l;
extern int pwm_x;

extern pid pid_r;
extern pid pid_l;
extern float freq_r;
extern float freq_l;
extern volatile bool control_flag;
extern int dir_r;
extern int dir_l;
float RPS_to_speed(float RPS);
float speed_to_RPS(float v);
void math_cung(float n);
void math_control1();
void math_control2();
void math_control3();
void speed_R_L(float v,float w,float &freq_l, float &freq_r);
void freq_to_speed(float freq_l, float freq_r); 

#endif