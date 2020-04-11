#include "foc.h"

// Private:
// (Implementation below)

// Two intermediate structs to handle data inbetween transformations
struct Post_Clarke {
    float alpha;
    float beta;
};
struct Post_Park {
    float d;
    float q;
};

// Transformation functions
void forwardClarke(struct Post_Clarke* out_i, const struct FOC_in* in);

void forwardPark(struct Post_Park* out_i, const struct Post_Clarke* in_i,
                  float flux_angle);

void reversePark(struct Post_Clarke* out_v, const struct Post_Park* in_v,
                  float flux_angle);

void reverseClarke(struct FOC_out* out, const struct Post_Clarke* in_v);

// Finds the rotor time constant based on the motor properties
float getRotorTimeConstant(Motor_Properties* props);

// Function and helper functions to find the flux angle
float getMagCurrent(struct FOC_in* in, struct Motor_Properties* props);
float getFluxSpeed(struct FOC_in* in, struct Motor_Properties* props);
float getFluxAngle(struct FOC_in* in, struct Motor_Properties* props);

// PI functions
float calc_PI(PID_Handle_t* pid_handle, float error);


// Public functions:

void FOC_in_init(FOC_in* foc) {
  foc->i_a = 0;
  foc->i_b = 0;
  foc->i_c = 0;

  foc->magCurrent = 0;
  foc->fluxAngle = 0;
  foc->mechanicalSpeed = 0;

  foc->prev_i_d = 0;
  foc->prev_i_q = 0;

  foc->target_i_d = 0;
  foc->target_i_q = 0;

  PID_HandleInit(&(foc->pid_handle));
  PID_SetKP(&(foc->pid_handle), k_p);
  PID_SetKI(&(foc->pid_handle), k_i);
}

/**
 * Call these functions every time to set the three currents
 * before calling compute_FOC()
 * */
void set_i_a(FOC_in* foc, float i_a_in) {
  foc->i_a = i_a_in;
}
void set_i_b(FOC_in* foc, float i_b_in) {
  foc->i_b = i_b_in;
}
void set_i_c(FOC_in* foc, float i_c_in) {
  foc->i_c = i_c_in;
}
void setMechSpeed(FOC_in* foc, float mechSpeed_in) {
  foc->mechanicalSpeed = mechSpeed_in;
}


/**
 * These two functions should only need to be called once to set motor properties
 * */
void setInductance(Motor_Properties* props, float inductance_in) {
  props->inductance = inductance_in;
}
void setResistance(Motor_Properties* props, float resistance_in) {
  props->resistance = resistance_in;
}


/**
 * 
 * The primary function that should be called for clarke/park transformations.
 * 
 * */
FOC_out* compute_FOC(FOC_in* in, Motor_Properties* props) {
  
  // Run the forward Clarke transformation
  struct Post_Clarke forwardClarke_out;
  forwardClarke(&forwardClarke_out, in);

  // Compute the current flux angle
  in->fluxAngle = get_flux_angle(in, props);

  // Run the forward Park transformation
  struct Post_Park forwardPark_out;
  forwardPark(&forwardPark_out, &forwardClarke_out, in->fluxAngle);

  // Store the park transformation output in the FOC_in object for next time
  in->prev_i_d = forwardPark_out.d;
  in->prev_i_q = forwardPark_out.q;

  //////////////////////////////////////////////

  struct Post_Park pid_out;

  pid_out.d = PI_Controller(&(in->pid_handle), in->prev_i_d - in->target_i_d);
  pid_out.q = PI_Controller(&(in->pid_handle), in->prev_i_d - in->target_i_d);

  //////////////////////////////////////////////

  // Run the reverse Park transformation
  struct Post_Clarke reversePark_out;
  reversePark(&reversePark_out, &pid_out, in->fluxAngle);

  // Run the reverse Clarke transformation
  struct FOC_out out;
  reverseClarke(&out, &reversePark_out);

  return &out;

}



/**
 * Implementation of private functions
 * */

void forwardClarke(struct Post_Clarke* out_i, const struct FOC_in* in) {
    out_i->alpha = 1.5 * in->i_a;
    out_i->beta = sqrt(3) * (in->i_b + in->i_c) / 2;
}

void forwardPark(struct Post_Park* out_i, const struct Post_Clarke* in_i,
                  float flux_angle) {
    out_i->d = in_i->alpha * cos(flux_angle) + in_i->beta * sin(flux_angle);
    out_i->q = -in_i->alpha * sin(flux_angle) + in_i->beta * cos(flux_angle);
}

void reversePark(struct Post_Clarke* out_v, const struct Post_Park* in_v,
                  float flux_angle) {
    out_v->alpha = in_v->d * cos(flux_angle) - in_v->q * sin(flux_angle);
    out_v->beta = in_v->d * sin(flux_angle) + in_v->q * cos(flux_angle);
}

void reverseClarke(struct FOC_out* out, const struct Post_Clarke* in_v) {
    out->v_a = 2 * in_v->alpha / 3;
    out->v_b = -in_v->alpha / 3 + in_v->beta / sqrt(3);
    out->v_c = -in_v->alpha / 3 - in_v->beta / sqrt(3);
}


float getRotorTimeConstant(Motor_Properties* props) {
  return props->inductance / props->resistance;
}

// For finding the flux angle:

float getMagCurrent(struct FOC_in* in, struct Motor_Properties* props) {
  return in->magCurrent + (PWM_PERIOD / getRotorTimeConstant(props)) * (in->prev_i_d - in->magCurrent);
}

float getFluxSpeed(struct FOC_in* in, struct Motor_Properties* props) {
  in->magCurrent = getMagCurrent(in, props);

  return in->mechanicalSpeed + in->prev_i_q / (getRotorTimeConstant(props) * NOM_FLUX_SPD * in->magCurrent);
}

float getFluxAngle(struct FOC_in* in, struct Motor_Properties* props) {
  return in->fluxAngle + NOM_FLUX_SPD * getFluxSpeed(in, props) * PWM_PERIOD;
}