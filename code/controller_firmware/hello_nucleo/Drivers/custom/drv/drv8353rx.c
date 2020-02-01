#include "drv8353rx.h"

DRV_OpConfig_TypeDef DRV_OpConfig;

fault_status1_t DRV_fault_status;
VGS_status2_t DRV_VGS_status2;
driver_control_t DRV_driver_control;
gate_drive_hs_t DRV_gate_drive_hs;
gate_drive_ls_t DRV_gate_drive_ls;
ocp_control_t DRV_ocp_control;
csa_control_t DRV_csa_control;
reserved_t DRV_reserved;

void DRV_init(
    SPI_Module_TypeDef *spi_module_in,
    GPIO_TypeDef *cs_port_in,
    uint32_t cs_pin_in,
    uint8_t op_mode_in)
{

  // Setup DRV comms and mode
  DRV_OpConfig.spi_module = spi_module_in;
  DRV_OpConfig.cs_port = cs_port_in;
  DRV_OpConfig.cs_pin = cs_pin_in;
  DRV_OpConfig.op_mode = op_mode_in;

  DRV_regInit();
  DRV_setupMode(DRV_OpConfig.op_mode);
}

void DRV_writeFaultStatus(fault_status1_t fs)
{
  DRV_fault_status.R = fs.R;
  DRV_SPI_writeHelper(fs.R);
}

fault_status1_t DRV_readFaultStatus()
{
  fault_status1_t fs;
  fs.R = DRV_SPI_readHelper(DRV_fault_status.R);
}

void DRV_writeVGSStatus(VGS_status2_t vgss)
{
  DRV_VGS_status2.R = vgss.R;
  DRV_SPI_writeHelper(vgss.R);
}

VGS_status2_t DRV_readVGSStatus()
{
  VGS_status2_t vgss;
  vgss.R = DRV_SPI_readHelper(DRV_VGS_status2.R);
}

void DRV_writeDriverControl(driver_control_t dc)
{
  DRV_driver_control.R = dc.R;
  DRV_SPI_writeHelper(dc.R);
}

driver_control_t DRV_readDriverControl()
{
  driver_control_t dc;
  dc.R = DRV_SPI_readHelper(DRV_driver_control.R);
}

void DRV_writeGateDriveHS(gate_drive_hs_t gdhs)
{
  DRV_gate_drive_hs.R = gdhs.R;
  DRV_SPI_writeHelper(gdhs.R);
}

gate_drive_hs_t DRV_readGateDriveHS()
{
  gate_drive_hs_t gdhs;
  gdhs.R = DRV_SPI_readHelper(DRV_gate_drive_hs.R);
}

void DRV_writeGateDriveLS(gate_drive_ls_t gdls)
{
  DRV_gate_drive_ls.R = gdls.R;
  DRV_SPI_writeHelper(gdls.R);
}

gate_drive_ls_t DRV_readGateDriveLS()
{
  gate_drive_hs_t gdls;
  gdls.R = DRV_SPI_readHelper(DRV_gate_drive_ls.R);
}

void DRV_writeOCPControl(ocp_control_t ocpc)
{
  DRV_ocp_control.R = ocpc.R;
  DRV_SPI_writeHelper(ocpc.R);
}

ocp_control_t DRV_readOCPControl()
{
  ocp_control_t ocpc;
  ocpc.R = DRV_SPI_readHelper(DRV_ocp_control.R);
}

void DRV_writeCSAControl(csa_control_t csac)
{
  DRV_csa_control.R = csac.R;
  DRV_SPI_writeHelper(csac.R);
}

csa_control_t DRV_readCSAControl()
{
  ocp_control_t csac;
  csac.R = DRV_SPI_readHelper(DRV_csa_control.R);
}

void DRV_writeAll()
{
  DRV_SPI_writeHelper(DRV_fault_status.R);
  DRV_SPI_writeHelper(DRV_VGS_status2.R);
  DRV_SPI_writeHelper(DRV_driver_control.R);
  DRV_SPI_writeHelper(DRV_gate_drive_hs.R);
  DRV_SPI_writeHelper(DRV_gate_drive_ls.R);
  DRV_SPI_writeHelper(DRV_ocp_control.R);
  DRV_SPI_writeHelper(DRV_csa_control.R);
  DRV_SPI_writeHelper(DRV_reserved.R);
}

void DRV_readAll()
{
  DRV_fault_status.R      |= (0x7FF & DRV_SPI_readHelper(DRV_fault_status.R));
  DRV_VGS_status2.R       |= (0x7FF & DRV_SPI_readHelper(DRV_VGS_status2.R));
  DRV_driver_control.R    |= (0x7FF & DRV_SPI_readHelper(DRV_driver_control.R));
  DRV_gate_drive_hs.R     |= (0x7FF & DRV_SPI_readHelper(DRV_gate_drive_hs.R));
  DRV_gate_drive_ls.R     |= (0x7FF & DRV_SPI_readHelper(DRV_gate_drive_ls.R));
  DRV_ocp_control.R       |= (0x7FF & DRV_SPI_readHelper(DRV_ocp_control.R));
  DRV_csa_control.R       |= (0x7FF & DRV_SPI_readHelper(DRV_csa_control.R));
  DRV_reserved.R          |= (0x7FF & DRV_SPI_readHelper(DRV_reserved.R));
}

void DRV_SPI_writeHelper(uint16_t data)
{
  data |= (0 << 15); // Set to write
  HAL_GPIO_WritePin(DRV_OpConfig.cs_port, DRV_OpConfig.cs_pin, 0);
  DRV_SPI_DEAD_TIME
  SPI_send16(DRV_OpConfig.spi_module, (uint16_t)(data));
  DRV_SPI_DEAD_TIME
  HAL_GPIO_WritePin(DRV_OpConfig.cs_port, DRV_OpConfig.cs_pin, 1);
}

uint16_t DRV_SPI_readHelper(uint16_t data)
{
  data |= (1 << 15); // Set to read
  data &= 0xF8;      // clear data (technically not necessary)
  HAL_GPIO_WritePin(DRV_OpConfig.cs_port, DRV_OpConfig.cs_pin, 0);
  DRV_SPI_DEAD_TIME
  SPI_send16(DRV_OpConfig.spi_module, (uint16_t)(data));
  DRV_SPI_DEAD_TIME
  HAL_GPIO_WritePin(DRV_OpConfig.cs_port, DRV_OpConfig.cs_pin, 1);

  return SPI_read16(DRV_OpConfig.spi_module);
}

void DRV_regInit()
{
  // Set config values (not yet written to DRV)
  DRV_fault_status.R    = 0;
  DRV_VGS_status2.R     = 0;
  DRV_driver_control.R  = 0;
  DRV_gate_drive_hs.R   = 0;
  DRV_gate_drive_ls.R   = 0;
  DRV_ocp_control.R     = 0;
  DRV_csa_control.R     = 0;
  DRV_reserved.R        = 0;

  DRV_fault_status.fault_status1.address          = 0;
  DRV_VGS_status2.VGS_status2.address             = 1;
  DRV_driver_control.driver_control.address       = 2;
  DRV_gate_drive_hs.gate_drive_hs.address         = 3;
  DRV_gate_drive_ls.gate_drive_ls.address         = 4;
  DRV_ocp_control.ocp_control.address             = 5;
  DRV_csa_control.csa_control.address             = 6; 
  DRV_reserved.reserved.address                   = 7;
}

void DRV_setupMode(uint8_t op_mode)
{
  switch (op_mode)
  {
  case DRV_MODE_SENSORED_TRPZ:

    DRV_fault_status.R = 0;
    DRV_VGS_status2.R = 0;
    // Driver Control
    DRV_driver_control.driver_control.clr_flt     = 0b0;
    DRV_driver_control.driver_control.brake       = 0b0;
    DRV_driver_control.driver_control.coast       = 0b0;
    DRV_driver_control.driver_control.pwm1_dir    = 0b0;
    DRV_driver_control.driver_control.pwm1_com    = 0b0;
    DRV_driver_control.driver_control.pwm_mode    = 0b10;
    DRV_driver_control.driver_control.otw_rep     = 0b1;
    DRV_driver_control.driver_control.dis_gdf     = 0b0;
    DRV_driver_control.driver_control.dis_gduv    = 0b0;
    DRV_driver_control.driver_control.ocp_act     = 0b0;

    // Gate Drive HS
    DRV_gate_drive_hs.gate_drive_hs.idriven_hs    = 0b0011;
    DRV_gate_drive_hs.gate_drive_hs.idrivep_hs    = 0b0011;
    DRV_gate_drive_hs.gate_drive_hs.lock          = 0b100;

    // Gate Drive LS
    DRV_gate_drive_ls.gate_drive_ls.idriven_ls    = 0b0011;
    DRV_gate_drive_ls.gate_drive_ls.idrivep_ls    = 0b0011;
    DRV_gate_drive_ls.gate_drive_ls.tdrive        = 0b11;
    DRV_gate_drive_ls.gate_drive_ls.cbc           = 0b1;

    // OCP Control
    DRV_ocp_control.ocp_control.vds_lvl           = 0b1101;
    DRV_ocp_control.ocp_control.ocp_deg           = 0b10;
    DRV_ocp_control.ocp_control.ocp_mode          = 0b01;
    DRV_ocp_control.ocp_control.dead_time         = 0b11;
    DRV_ocp_control.ocp_control.tretry            = 0b0;

    // CSA Control
    DRV_csa_control.csa_control.sen_lvl           = 0b11;
    DRV_csa_control.csa_control.csa_cal_c         = 0b0;
    DRV_csa_control.csa_control.csa_cal_b         = 0b0;
    DRV_csa_control.csa_control.csa_cal_a         = 0b0;
    DRV_csa_control.csa_control.dis_sen           = 0b0;
    DRV_csa_control.csa_control.csa_gain          = 0b10;
    DRV_csa_control.csa_control.ls_ref            = 0b0;
    DRV_csa_control.csa_control.vref_div          = 0b1;
    DRV_csa_control.csa_control.csa_fet           = 0b0;

    break;
  case DRV_MODE_BRUSHED_DC:
    // set reg values for brushed motor
    break;
  case DRV_MODE_RESOLVER_FOC:
    // set reg values for resolver based foc
    break;
  case DRV_MODE_SENSORLESS_FOC:
	  DRV_fault_status.R = 0;
	      DRV_VGS_status2.R = 0;
	      // Driver Control
	      DRV_driver_control.driver_control.clr_flt     = 0b0;
	      DRV_driver_control.driver_control.brake       = 0b0;
	      DRV_driver_control.driver_control.coast       = 0b0;
	      DRV_driver_control.driver_control.pwm1_dir    = 0b0;
	      DRV_driver_control.driver_control.pwm1_com    = 0b0;
	      DRV_driver_control.driver_control.pwm_mode    = 0b00; // From 0b10 to 0b00 (1x to 6x PWM)
	      DRV_driver_control.driver_control.otw_rep     = 0b1;
	      DRV_driver_control.driver_control.dis_gdf     = 0b0;
	      DRV_driver_control.driver_control.dis_gduv    = 0b0;
	      DRV_driver_control.driver_control.ocp_act     = 0b0;

	      // Gate Drive HS
	      DRV_gate_drive_hs.gate_drive_hs.idriven_hs    = 0b0011;
	      DRV_gate_drive_hs.gate_drive_hs.idrivep_hs    = 0b0011;
	      DRV_gate_drive_hs.gate_drive_hs.lock          = 0b100;

	      // Gate Drive LS
	      DRV_gate_drive_ls.gate_drive_ls.idriven_ls    = 0b0011;
	      DRV_gate_drive_ls.gate_drive_ls.idrivep_ls    = 0b0011;
	      DRV_gate_drive_ls.gate_drive_ls.tdrive        = 0b11;
	      DRV_gate_drive_ls.gate_drive_ls.cbc           = 0b1;

	      // OCP Control
	      DRV_ocp_control.ocp_control.vds_lvl           = 0b1101;
	      DRV_ocp_control.ocp_control.ocp_deg           = 0b10;
	      DRV_ocp_control.ocp_control.ocp_mode          = 0b01;
	      DRV_ocp_control.ocp_control.dead_time         = 0b11;
	      DRV_ocp_control.ocp_control.tretry            = 0b0;

	      // CSA Control
	      DRV_csa_control.csa_control.sen_lvl           = 0b11;
	      DRV_csa_control.csa_control.csa_cal_c         = 0b0;
	      DRV_csa_control.csa_control.csa_cal_b         = 0b0;
	      DRV_csa_control.csa_control.csa_cal_a         = 0b0;
	      DRV_csa_control.csa_control.dis_sen           = 0b0;
	      DRV_csa_control.csa_control.csa_gain          = 0b00; // Changed from 0b10 to 0b00 (20 V/V to 5 V/V)
	      DRV_csa_control.csa_control.ls_ref            = 0b0;
	      DRV_csa_control.csa_control.vref_div          = 0b1;
	      DRV_csa_control.csa_control.csa_fet           = 0b0;
    break;
  default:
    // handle a case in which no mode is set if necessary, or place sensorless foc code here
    break;
  }
}
