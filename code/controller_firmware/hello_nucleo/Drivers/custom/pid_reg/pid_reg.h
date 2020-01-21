/*
 * pid_reg.h
 *
 *  Created on: Nov 23, 2019
 *      Author: micha
 */

#ifndef PID_REG_PID_REG_H_
#define PID_REG_PID_REG_H_

#include <stdint.h>

typedef struct PID_Handle
{
  int16_t   hKpGain;              /*   gain used by PID component */
  int16_t   hKiGain;              /*   gain used by PID component */
  int32_t   wIntegralTerm;        /*   integral term */
  int32_t   wUpperIntegralLimit;  /*   Upper limit used to saturate the integral
                                       term given by @f$\frac{K_{ig}}{K_{id}} @f$ * integral of
                                       process variable error */
  int32_t   wLowerIntegralLimit;  /*   Lower limit used to saturate the integral
                                       term given by Ki / Ki divisor * integral of
                                       process variable error */
  int16_t   hUpperOutputLimit;    /*   Upper limit used to saturate the PI output */
  int16_t   hLowerOutputLimit;    /*   Lower limit used to saturate the PI output */
  uint16_t  hKpDivisor;           /*   Kp gain divisor, used in conjuction with
                                       Kp gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  uint16_t  hKiDivisor;           /*   Ki gain divisor, used in conjuction with
                                       Ki gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  uint16_t  hKpDivisorPOW2;       /*   Kp gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 as 2^9 = 512 */
  uint16_t  hKiDivisorPOW2;       /*   Ki gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 as 2^9 = 512 */
  int16_t   hKdGain;              /*   Kd gain used by PID component */
  uint16_t  hKdDivisor;           /*   Kd gain divisor, used in conjuction with
                                       Kd gain allows obtaining fractional values.
                                       If FULL_MISRA_C_COMPLIANCY is not defined
                                       the divisor is implemented through
                                       algebrical right shifts to speed up PI
                                       execution. Only in this case this parameter
                                       specifies the number of right shifts to be
                                       executed */
  uint16_t  hKdDivisorPOW2;       /*   Kd gain divisor expressed as power of 2.
                                       E.g. if gain divisor is 512 the value
                                       must be 9 as 2^9 = 512 */
  int32_t   wPrevProcessVarError; /*   previous process variable used by the
                                       derivative part of the PID component */
} PID_Handle_t;


void PID_HandleInit( PID_Handle_t * pHandle );

void PID_SetKP( PID_Handle_t * pHandle, int16_t hKpGain );
void PID_SetKI( PID_Handle_t * pHandle, int16_t hKiGain );
void PID_SetKD( PID_Handle_t * pHandle, int16_t hKdGain );

int16_t PID_GetKP( PID_Handle_t * pHandle );
int16_t PID_GetKI( PID_Handle_t * pHandle );
int16_t PID_GetKD( PID_Handle_t * pHandle );


void PID_SetIntegralTerm( PID_Handle_t * pHandle, int32_t wIntegralTermValue );

uint16_t PID_GetKPDivisor( PID_Handle_t * pHandle );
void PID_SetKPDivisorPOW2( PID_Handle_t * pHandle, uint16_t hKpDivisorPOW2 );

uint16_t PID_GetKIDivisor( PID_Handle_t * pHandle );
uint16_t PID_GetKDDivisor( PID_Handle_t * pHandle );


void PID_SetKIDivisorPOW2( PID_Handle_t * pHandle, uint16_t hKiDivisorPOW2 );
void PID_SetKDDivisorPOW2( PID_Handle_t * pHandle, uint16_t hKdDivisorPOW2 );

void PID_SetLowerIntegralTermLimit( PID_Handle_t * pHandle, int32_t wLowerLimit );
void PID_SetUpperIntegralTermLimit( PID_Handle_t * pHandle, int32_t wUpperLimit );

void PID_SetLowerOutputLimit( PID_Handle_t * pHandle, int16_t hLowerLimit );
void PID_SetUpperOutputLimit( PID_Handle_t * pHandle, int16_t hUpperLimit );

void PID_SetPrevError( PID_Handle_t * pHandle, int32_t wPrevProcessVarError );

/* Actual Controller */
int16_t PI_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError );

/* General Controller with on D term calculations */
int16_t PID_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError );



#endif /* PID_REG_PID_REG_H_ */
