// Texas Instruments DRV8353Rx register map
#ifndef _DRV8353RX_H_
#define _DRV8353RX_H_

#include <stdint.h>
//#include "gpio.h"
#include "spi.h"
#include "stm32f3xx_hal.h"

typedef union 
{
  uint16_t R;
  struct
  {
    uint16_t VDS_LC       : 1;
    uint16_t VDS_HC       : 1;
    uint16_t VDS_LB       : 1;
    uint16_t VDS_HB       : 1;
    uint16_t VDS_LA       : 1;
    uint16_t VDS_HA       : 1;
    uint16_t OTSD         : 1;
    uint16_t UVLO         : 1;
    uint16_t GDF          : 1;
    uint16_t VDS_OCP      : 1;
    uint16_t FAULT        : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } fault_status1;
} fault_status1_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint16_t VGS_LC       : 1;
    uint16_t VGS_HC       : 1;
    uint16_t VGS_LB       : 1;
    uint16_t VGS_HB       : 1;
    uint16_t VGS_LA       : 1;
    uint16_t VGS_HA       : 1;
    uint16_t GDUV         : 1;
    uint16_t OTW          : 1;
    uint16_t SC_OC        : 1;
    uint16_t SB_OC        : 1;
    uint16_t SA_OC        : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } VGS_status2;
} VGS_status2_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t clr_flt       : 1;
    uint8_t brake         : 1;
    uint8_t coast         : 1;
    uint8_t pwm1_dir      : 1;
    uint8_t pwm1_com      : 1;
    uint8_t pwm_mode      : 2;
    uint8_t otw_rep       : 1;
    uint8_t dis_gdf       : 1;
    uint8_t dis_gduv      : 1;
    uint8_t ocp_act       : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } driver_control;
} driver_control_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t idriven_hs    : 4;
    uint8_t idrivep_hs    : 4;
    uint8_t lock          : 3;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } gate_drive_hs;
} gate_drive_hs_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t idriven_ls    : 4;
    uint8_t idrivep_ls    : 4;
    uint8_t tdrive        : 2;
    uint8_t cbc           : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } gate_drive_ls;
} gate_drive_ls_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t vds_lvl       : 4;
    uint8_t ocp_deg       : 2;
    uint8_t ocp_mode      : 2;
    uint8_t dead_time     : 2;
    uint8_t tretry        : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } ocp_control;
} ocp_control_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t sen_lvl       : 2;
    uint8_t csa_cal_c     : 1;
    uint8_t csa_cal_b     : 1;
    uint8_t csa_cal_a     : 1;
    uint8_t dis_sen       : 1;
    uint8_t csa_gain      : 2;
    uint8_t ls_ref        : 1;
    uint8_t vref_div      : 1;
    uint8_t csa_fet       : 1;
    uint8_t address       : 4;
    uint8_t rw            : 1;
  } csa_control;
} csa_control_t;

typedef union 
{
  uint16_t R;
  struct
  {
    uint8_t cal_mode      : 1;
    uint16_t dontcare     : 10;
    uint16_t address      : 4;
    uint8_t rw            : 1;
  } reserved;
} reserved_t;

typedef struct
{
  SPI_Module_TypeDef *spi_module;
  GPIO_TypeDef *cs_port;
  uint32_t cs_pin;
  uint8_t op_mode;
} DRV_OpConfig_TypeDef;

#define DRV_MODE_SENSORLESS_FOC 1 // Default mode used in the GUI
#define DRV_MODE_RESOLVER_FOC 2   // FOC mode with a resolver TODO: implement
#define DRV_MODE_SENSORED_TRPZ 3  // Mode for trapezoidal control with hall
#define DRV_MODE_BRUSHED_DC 4     // Mode for brushed DC motor control

#define DRV_SPI_DEAD_TIME for (int i = 0; i < 5; ++i);

/**
  * @brief DRV communications initialization
  * @param spi_module_in:  SPI module to use
  * @param cs_port_in:     GPIO port to be used for chipselect
  * @param cs_pin_in:      GPIO pin to be used for chipselect
  * @param op_mode_in:     operational mode of the DRV (see defines above)
  * @retval None
  */
void DRV_init(
    SPI_Module_TypeDef *spi_module_in,
    GPIO_TypeDef *cs_port_in,
    uint32_t cs_pin_in,
    uint8_t op_mode_in);

void DRV_writeFaultStatus(fault_status1_t fs);

fault_status1_t DRV_readFaultStatus();

void DRV_writeVGSStatus(VGS_status2_t vgss);

VGS_status2_t DRV_readVGSStatus();

void DRV_writeDriverControl(driver_control_t dc);

driver_control_t DRV_readDriverControl();

void DRV_writeGateDriveHS(gate_drive_hs_t gdhs);

gate_drive_hs_t DRV_readGateDriveHS();

void DRV_writeGateDriveLS(gate_drive_ls_t gdls);

gate_drive_ls_t DRV_readGateDriveLS();

void DRV_writeOCPControl(ocp_control_t ocpc);

ocp_control_t DRV_readOCPControl();

void DRV_writeCSAControl(csa_control_t csac);

csa_control_t DRV_readCSAControl();

void DRV_writeAll();

void DRV_readAll();

void DRV_SPI_writeHelper(uint16_t data);

uint16_t DRV_SPI_readHelper(uint16_t data);

void DRV_regInit();

void DRV_setupMode(uint8_t op_mode);

#endif
