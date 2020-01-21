/*
 * pid_reg.c
 *
 *  Created on: Nov 23, 2019
 *      Author: micha
 */

#include "pid_reg.h"
#include "stdint.h"


/**
 * @brief  It initializes the handle
 * @param  pidHandle: handler of the current instance of the PID component
 * @retval None
 */
void PID_HandleInit(PID_Handle_t* pidHandle)
{
  pidHandle->hKpGain =  1u;
  pidHandle->hKiGain =  0u;
  pidHandle->hKdGain =  0u;
  pidHandle->wIntegralTerm = 0x00000000UL;
  pidHandle->wPrevProcessVarError = 0x00000000UL;
}

void PID_SetKP(PID_Handle_t* pidHandle, int16_t hKpGain)
{
  pidHandle->hKpGain = hKpGain;
}

void PID_SetKI(PID_Handle_t* pidHandle, int16_t hKiGain)
{
  pidHandle->hKiGain = hKiGain;
}

int16_t PID_GetKP(PID_Handle_t* pidHandle)
{
  return pidHandle->hKpGain;
}

int16_t PID_GetKI(PID_Handle_t* pidHandle)
{
  return pidHandle->hKiGain;
}

void PID_SetIntegralTerm(PID_Handle_t* pidHandle, int32_t wIntegralTermValue)
{
  pidHandle->wIntegralTerm = wIntegralTermValue;
}

uint16_t PID_GetKPDivisor( PID_Handle_t * pidHandle )
{
  return pidHandle->hKpDivisor;
}

void PID_SetKPDivisorPOW2(PID_Handle_t* pidHandle, uint16_t hKpDivisorPOW2)
{
  pidHandle->hKpDivisorPOW2 = hKpDivisorPOW2;
  pidHandle->hKpDivisor = ((uint16_t)1u << hKpDivisorPOW2);
}

uint16_t PID_GetKIDivisor(PID_Handle_t* pidHandle)
{
  return pidHandle->hKiDivisor;
}

void PID_SetKIDivisorPOW2(PID_Handle_t* pidHandle, uint16_t hKiDivisorPOW2)
{
  int32_t wKiDiv = (int32_t)1u << hKiDivisorPOW2;

  pidHandle->hKiDivisorPOW2 = hKiDivisorPOW2;
  pidHandle->hKiDivisor = (uint16_t)(wKiDiv);

  PID_SetUpperIntegralTermLimit( pidHandle, (int32_t)INT16_MAX * wKiDiv );
  PID_SetLowerIntegralTermLimit( pidHandle, (int32_t) - INT16_MAX * wKiDiv );
}

void PID_SetLowerIntegralTermLimit(PID_Handle_t * pidHandle, int32_t wLowerLimit)
{
  pidHandle->wLowerIntegralLimit = wLowerLimit;
}

void PID_SetUpperIntegralTermLimit(PID_Handle_t * pidHandle, int32_t wUpperLimit)
{
  pidHandle->wUpperIntegralLimit = wUpperLimit;
}

void PID_SetLowerOutputLimit(PID_Handle_t * pidHandle, int16_t hLowerLimit)
{
  pidHandle->hLowerOutputLimit = hLowerLimit;
}

void PID_SetUpperOutputLimit(PID_Handle_t * pidHandle, int16_t hUpperLimit)
{
  pidHandle->hUpperOutputLimit = hUpperLimit;
}

void PID_SetPrevError(PID_Handle_t * pidHandle, int32_t wPrevProcessVarError)
{
  pidHandle->wPrevProcessVarError = wPrevProcessVarError;
  return;
}

void PID_SetKD( PID_Handle_t * pidHandle, int16_t hKdGain )
{
  pidHandle->hKdGain = hKdGain;
}

int16_t PID_GetKD( PID_Handle_t * pidHandle )
{
  return pidHandle->hKdGain;
}

uint16_t PID_GetKDDivisor( PID_Handle_t * pidHandle )
{
  return pidHandle->hKdDivisor;
}

void PID_SetKDDivisorPOW2(PID_Handle_t * pidHandle, uint16_t hKdDivisorPOW2)
{
  pidHandle->hKdDivisorPOW2 = hKdDivisorPOW2;
  pidHandle->hKdDivisor = ( ( uint16_t )( 1u ) << hKdDivisorPOW2 );
}


/*
 * @brief  This function compute the output of a PI regulator sum of its
 *         proportional and integral terms
 * @param  pidHandle: handler of the current instance of the PID component
 * @param  wProcessVarError: current process variable error, intended as the reference
 *         value minus the present process variable value
 * @retval computed PI output
 */
int16_t PI_Controller(PID_Handle_t * pidHandle, int32_t wProcessVarError)
{
  int32_t wProportional_Term, wIntegral_Term, wOutput_32, wIntegral_sum_temp;
  int32_t wDischarge = 0;
  int16_t hUpperOutputLimit = pidHandle->hUpperOutputLimit;
  int16_t hLowerOutputLimit = pidHandle->hLowerOutputLimit;

  /* Proportional term computation*/
  wProportional_Term = pidHandle->hKpGain * wProcessVarError;

  /* Integral term computation */
  if (pidHandle->hKiGain == 0)
  {
    pidHandle->wIntegralTerm = 0;
  }
  else
  {
	/* Doin some integrating */
    wIntegral_Term = pidHandle->hKiGain * wProcessVarError;
    wIntegral_sum_temp = pidHandle->wIntegralTerm + wIntegral_Term;

    /* Overflow Check */
    if (wIntegral_sum_temp < 0)
    {
	  if (pidHandle->wIntegralTerm > 0 && wIntegral_Term > 0)
	  {
		  wIntegral_sum_temp = INT32_MAX;
	  }
    }
    else
    {
      if (pidHandle->wIntegralTerm < 0 && wIntegral_Term > 0)
      {
          wIntegral_sum_temp = -INT32_MAX;
      }
    }

    /* Apply Integral Term Limits */
    if (wIntegral_sum_temp > pidHandle->wUpperIntegralLimit)
    {
    	pidHandle->wIntegralTerm = pidHandle->wUpperIntegralLimit;
    }
    else if (wIntegral_sum_temp < pidHandle->wLowerIntegralLimit)
    {
    	pidHandle->wIntegralTerm = pidHandle->wLowerIntegralLimit;
    }
    else
    {
    	pidHandle->wIntegralTerm = wIntegral_sum_temp;
    }
  }

  /* Apply pi constant dividers */
#ifdef WEAK_SAUCE_MODE
  wOutput_32 = (wProportional_Term / (int32_t)pidHandle->hKpDivisor) + (pidHandle->wIntegralTerm / (int32_t)pidHandle->hKiDivisor );
#else
  wOutput_32 = (wProportional_Term >> pidHandle->hKpDivisorPOW2) + (pidHandle->wIntegralTerm >> pidHandle->hKiDivisorPOW2);
#endif

  /* Apply Output Limits */
  if (wOutput_32 > hUpperOutputLimit)
  {
    wDischarge = hUpperOutputLimit - wOutput_32;
    wOutput_32 = hUpperOutputLimit;
  }
  else if (wOutput_32 < hLowerOutputLimit)
  {

    wDischarge = hLowerOutputLimit - wOutput_32;
    wOutput_32 = hLowerOutputLimit;
  }

  pidHandle->wIntegralTerm += wDischarge;

  return (int16_t)wOutput_32;
}
