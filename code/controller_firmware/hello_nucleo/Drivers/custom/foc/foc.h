/**
 * foc.h
 * 
 * 3/14/20
 * Cameron Tressler
 * */


#ifndef _FOC_H_
#define _FOC_H_

// 2 * pi * 50 (50 is the nominal frequency)
#define NOM_FLUX_SPD 314.159

// Sampling time
#define PWM_PERIOD (1 / 2000000)

// Define and tune PI constants
#define k_p 0
#define k_i 0

#include "../pid_reg/pid_reg.h"

/**
 * 
 * input parameters for compute_FOC()
 * 
 * */
typedef struct FOC_in {

  // The values of each current
  float i_a;
  float i_b;
  float i_c;

  // Necessary to find the flux angle
  
  // The previous magnetizing current
  float magCurrent;

  // The previous flux angle
  float fluxAngle;

  // The current mechanical speed
  float mechanicalSpeed;

  float prev_i_d;
  float prev_i_q;

  float target_i_d;
  float target_i_q;

  PID_Handle_t pid_handle;

} FOC_in;

void FOC_in_init(FOC_in* foc);

void set_i_a(FOC_in* foc, float i_a_in);
void set_i_b(FOC_in* foc, float i_b_in);
void set_i_c(FOC_in* foc, float i_c_in);

void setMechSpeed(FOC_in* foc, float mechSpeed_in);


/**
 * 
 * Physical properties of our motors
 * General to all of our motors
 * 
 * */
typedef struct Motor_Properties {
  float inductance;
  float resistance;
} Motor_Properties;

void setInductance(Motor_Properties* props, float inductance_in);
void setResistance(Motor_Properties* props, float resistance_in);


/**
 * 
 * Output parameters for compute_FOC()
 * 
 * */
typedef struct FOC_out {
  float v_a;
  float v_b;
  float v_c;
} FOC_out;

FOC_out* compute_FOC(FOC_in* in, Motor_Properties* props);


#endif
