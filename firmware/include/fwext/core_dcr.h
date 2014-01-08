/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/* Blue Gene/Q                                                      */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/* US Government Users Restricted Rights - Use, duplication or      */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM    */
/*   Corp.                                                          */
/*                                                                  */
/* This software is available to you under the Eclipse Public       */
/* License (EPL).                                                   */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
#include <firmware/include/fwext/sd_rcb.h>
#include <firmware/include/fwext/sd_helper.h>
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Automatically generated with dcr-h-to-perl, do not modify
// stk@de.ibm.com
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//   Name:        rcbus
extern int LEFT_RCBUS;
extern int LEFT_RCBUS_WMASK[];
extern int LEFT_RCBUS_MASKED_WRITE[];
extern int LEFT_RCBUS_DATA[];
//   Name:        rcbus_config
extern int LEFT_RCBUS_CONFIG;
extern int LEFT_RCBUS_CONFIG_ADDRESS[];
extern int LEFT_RCBUS_CONFIG_RESET[];
//   Name:        sd_config
extern int LEFT_SD_CONFIG;
extern int LEFT_SD_CONFIG_HSSL2PLLON[];
extern int LEFT_SD_CONFIG_PCS_TESTMODE[];
extern int LEFT_SD_CONFIG_HALFRATE[];
//   Name:        ts_ctl_a
extern int LEFT_TS_CTL_A;
extern int LEFT_TS_CTL_A_PCS_RESET[];
extern int LEFT_TS_CTL_A_PIPE_RESETN[];
extern int LEFT_TS_CTL_A_RX_RESET_M[];
extern int LEFT_TS_CTL_A_RX_RESET_P[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_RUN_OVR_M[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_RUN_OVR_P[];
extern int LEFT_TS_CTL_A_RX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_A_TX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_A_TX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_A_TX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_A_TX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_A_XMIT_SCRAMBLE_M[];
extern int LEFT_TS_CTL_A_XMIT_SCRAMBLE_P[];
extern int LEFT_TS_CTL_A_RCV_DESCRAMBLE_M[];
extern int LEFT_TS_CTL_A_RCV_DESCRAMBLE_P[];
extern int LEFT_TS_CTL_A_SEND_TRAIN_M[];
extern int LEFT_TS_CTL_A_SEND_TRAIN_P[];
extern int LEFT_TS_CTL_A_RCV_TRAIN_M[];
extern int LEFT_TS_CTL_A_RCV_TRAIN_P[];
extern int LEFT_TS_CTL_A_LOOPBACK_EN[];
extern int LEFT_TS_CTL_A_DATALOOP_EN_M[];
extern int LEFT_TS_CTL_A_DATALOOP_EN_P[];
extern int LEFT_TS_CTL_A_PARLPBK_EN_M[];
extern int LEFT_TS_CTL_A_PARLPBK_EN_P[];
extern int LEFT_TS_CTL_A_REFCLK_DISABLE[];
extern int LEFT_TS_CTL_A_LANE_RESET_M[];
extern int LEFT_TS_CTL_A_LANE_RESET_P[];
extern int LEFT_TS_CTL_A_HSS_AREFEN[];
//   Name:        ts_ctl_b
extern int LEFT_TS_CTL_B;
extern int LEFT_TS_CTL_B_PCS_RESET[];
extern int LEFT_TS_CTL_B_PIPE_RESETN[];
extern int LEFT_TS_CTL_B_RX_RESET_M[];
extern int LEFT_TS_CTL_B_RX_RESET_P[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_RUN_OVR_M[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_RUN_OVR_P[];
extern int LEFT_TS_CTL_B_RX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_B_TX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_B_TX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_B_TX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_B_TX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_B_XMIT_SCRAMBLE_M[];
extern int LEFT_TS_CTL_B_XMIT_SCRAMBLE_P[];
extern int LEFT_TS_CTL_B_RCV_DESCRAMBLE_M[];
extern int LEFT_TS_CTL_B_RCV_DESCRAMBLE_P[];
extern int LEFT_TS_CTL_B_SEND_TRAIN_M[];
extern int LEFT_TS_CTL_B_SEND_TRAIN_P[];
extern int LEFT_TS_CTL_B_RCV_TRAIN_M[];
extern int LEFT_TS_CTL_B_RCV_TRAIN_P[];
extern int LEFT_TS_CTL_B_LOOPBACK_EN[];
extern int LEFT_TS_CTL_B_DATALOOP_EN_M[];
extern int LEFT_TS_CTL_B_DATALOOP_EN_P[];
extern int LEFT_TS_CTL_B_PARLPBK_EN_M[];
extern int LEFT_TS_CTL_B_PARLPBK_EN_P[];
extern int LEFT_TS_CTL_B_REFCLK_DISABLE[];
extern int LEFT_TS_CTL_B_LANE_RESET_M[];
extern int LEFT_TS_CTL_B_LANE_RESET_P[];
extern int LEFT_TS_CTL_B_HSS_AREFEN[];
//   Name:        ts_ctl_c
extern int LEFT_TS_CTL_C;
extern int LEFT_TS_CTL_C_PCS_RESET[];
extern int LEFT_TS_CTL_C_PIPE_RESETN[];
extern int LEFT_TS_CTL_C_RX_RESET_M[];
extern int LEFT_TS_CTL_C_RX_RESET_P[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_RUN_OVR_M[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_RUN_OVR_P[];
extern int LEFT_TS_CTL_C_RX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_C_TX_ELASTIC_BUF_RUN_M[];
extern int LEFT_TS_CTL_C_TX_ELASTIC_BUF_LAT_M[];
extern int LEFT_TS_CTL_C_TX_ELASTIC_BUF_RUN_P[];
extern int LEFT_TS_CTL_C_TX_ELASTIC_BUF_LAT_P[];
extern int LEFT_TS_CTL_C_XMIT_SCRAMBLE_M[];
extern int LEFT_TS_CTL_C_XMIT_SCRAMBLE_P[];
extern int LEFT_TS_CTL_C_RCV_DESCRAMBLE_M[];
extern int LEFT_TS_CTL_C_RCV_DESCRAMBLE_P[];
extern int LEFT_TS_CTL_C_SEND_TRAIN_M[];
extern int LEFT_TS_CTL_C_SEND_TRAIN_P[];
extern int LEFT_TS_CTL_C_RCV_TRAIN_M[];
extern int LEFT_TS_CTL_C_RCV_TRAIN_P[];
extern int LEFT_TS_CTL_C_LOOPBACK_EN[];
extern int LEFT_TS_CTL_C_DATALOOP_EN_M[];
extern int LEFT_TS_CTL_C_DATALOOP_EN_P[];
extern int LEFT_TS_CTL_C_PARLPBK_EN_M[];
extern int LEFT_TS_CTL_C_PARLPBK_EN_P[];
extern int LEFT_TS_CTL_C_REFCLK_DISABLE[];
extern int LEFT_TS_CTL_C_LANE_RESET_M[];
extern int LEFT_TS_CTL_C_LANE_RESET_P[];
extern int LEFT_TS_CTL_C_HSS_AREFEN[];
//   Name:        ts_align_stat_a
extern int LEFT_TS_ALIGN_STAT_A;
extern int LEFT_TS_ALIGN_STAT_A_LANE0_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_A_LANE0_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_A_LANE1_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_A_LANE1_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_A_LANE2_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_A_LANE2_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_A_LANE3_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_A_LANE3_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_A_BYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_A_BYTE_ALIGNED_P[];
extern int LEFT_TS_ALIGN_STAT_A_FOURBYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_A_FOURBYTE_ALIGNED_P[];
//   Name:        ts_align_stat_b
extern int LEFT_TS_ALIGN_STAT_B;
extern int LEFT_TS_ALIGN_STAT_B_LANE0_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_B_LANE0_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_B_LANE1_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_B_LANE1_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_B_LANE2_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_B_LANE2_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_B_LANE3_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_B_LANE3_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_B_BYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_B_BYTE_ALIGNED_P[];
extern int LEFT_TS_ALIGN_STAT_B_FOURBYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_B_FOURBYTE_ALIGNED_P[];
//   Name:        ts_align_stat_c
extern int LEFT_TS_ALIGN_STAT_C;
extern int LEFT_TS_ALIGN_STAT_C_LANE0_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_C_LANE0_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_C_LANE1_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_C_LANE1_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_C_LANE2_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_C_LANE2_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_C_LANE3_MUXSEL_M[];
extern int LEFT_TS_ALIGN_STAT_C_LANE3_MUXSEL_P[];
extern int LEFT_TS_ALIGN_STAT_C_BYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_C_BYTE_ALIGNED_P[];
extern int LEFT_TS_ALIGN_STAT_C_FOURBYTE_ALIGNED_M[];
extern int LEFT_TS_ALIGN_STAT_C_FOURBYTE_ALIGNED_P[];
//   Name:        ts_hss_pll_stat
extern int LEFT_TS_HSS_PLL_STAT;
extern int LEFT_TS_HSS_PLL_STAT_A_PLLA_LOCKED[];
extern int LEFT_TS_HSS_PLL_STAT_B_PLLA_LOCKED[];
extern int LEFT_TS_HSS_PLL_STAT_C_PLLA_LOCKED[];
//   Name:        ts_hss_lane_stat
extern int LEFT_TS_HSS_LANE_STAT;
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_A_M[];
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_A_P[];
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_B_M[];
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_B_P[];
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_C_M[];
extern int LEFT_TS_HSS_LANE_STAT_LANE_READY_C_P[];
//   Name:        prbs_ctl_a
extern int LEFT_PRBS_CTL_A;
extern int LEFT_PRBS_CTL_A_TX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_A_TX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_A_RX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_A_RX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_A_PRBS_SEL_M[];
extern int LEFT_PRBS_CTL_A_PRBS_SEL_P[];
extern int LEFT_PRBS_CTL_A_PRBS_RST_M[];
extern int LEFT_PRBS_CTL_A_PRBS_RST_P[];
//   Name:        prbs_ctl_b
extern int LEFT_PRBS_CTL_B;
extern int LEFT_PRBS_CTL_B_TX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_B_TX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_B_RX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_B_RX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_B_PRBS_SEL_M[];
extern int LEFT_PRBS_CTL_B_PRBS_SEL_P[];
extern int LEFT_PRBS_CTL_B_PRBS_RST_M[];
extern int LEFT_PRBS_CTL_B_PRBS_RST_P[];
//   Name:        prbs_ctl_c
extern int LEFT_PRBS_CTL_C;
extern int LEFT_PRBS_CTL_C_TX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_C_TX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_C_RX_PRBS_EN_M[];
extern int LEFT_PRBS_CTL_C_RX_PRBS_EN_P[];
extern int LEFT_PRBS_CTL_C_PRBS_SEL_M[];
extern int LEFT_PRBS_CTL_C_PRBS_SEL_P[];
extern int LEFT_PRBS_CTL_C_PRBS_RST_M[];
extern int LEFT_PRBS_CTL_C_PRBS_RST_P[];
//   Name:        prbs_stat_a
extern int LEFT_PRBS_STAT_A;
extern int LEFT_PRBS_STAT_A_SYNC_M[];
extern int LEFT_PRBS_STAT_A_SYNC_P[];
extern int LEFT_PRBS_STAT_A_ERROR_M[];
extern int LEFT_PRBS_STAT_A_ERROR_P[];
//   Name:        prbs_stat_b
extern int LEFT_PRBS_STAT_B;
extern int LEFT_PRBS_STAT_B_SYNC_M[];
extern int LEFT_PRBS_STAT_B_SYNC_P[];
extern int LEFT_PRBS_STAT_B_ERROR_M[];
extern int LEFT_PRBS_STAT_B_ERROR_P[];
//   Name:        prbs_stat_c
extern int LEFT_PRBS_STAT_C;
extern int LEFT_PRBS_STAT_C_SYNC_M[];
extern int LEFT_PRBS_STAT_C_SYNC_P[];
extern int LEFT_PRBS_STAT_C_ERROR_M[];
extern int LEFT_PRBS_STAT_C_ERROR_P[];
//   Name:        error_inject_timer_val
extern int LEFT_ERROR_INJECT_TIMER_VAL;
extern int LEFT_ERROR_INJECT_TIMER_VAL_RND[];
extern int LEFT_ERROR_INJECT_TIMER_VAL_COUNT[];
//   Name:        error_inject_limit
extern int LEFT_ERROR_INJECT_LIMIT;
extern int LEFT_ERROR_INJECT_LIMIT_RND_MASK[];
extern int LEFT_ERROR_INJECT_LIMIT_LOW_LIMIT[];
//   Name:        error_inject_control
extern int LEFT_ERROR_INJECT_CONTROL;
extern int LEFT_ERROR_INJECT_CONTROL_ENABLE[];
extern int LEFT_ERROR_INJECT_CONTROL_LINK_SELECT[];
extern int LEFT_ERROR_INJECT_CONTROL_MASK[];
//   Name:        interrupt_internal_error_sw_info
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO_READ[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO_WRITE[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO_PRIVILEGED[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO_TEST_INT[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_SW_INFO_ADDRESS[];
//   Name:        interrupt_internal_error_hw_info
extern int LEFT_INTERRUPT_INTERNAL_ERROR_HW_INFO;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_HW_INFO_ADDRESS_PARITY_GOOD[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_HW_INFO_DATA_PARITY_GOOD[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_HW_INFO_ADDRESS[];
//   Name:        interrupt_internal_error_data_info
extern int LEFT_INTERRUPT_INTERNAL_ERROR_DATA_INFO;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_DATA_INFO_DATA[];
// Interrupt State Register: serdes_interrupt_state
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
extern int LEFT_SERDES_INTERRUPT_STATE_STATE;
extern int LEFT_SERDES_INTERRUPT_STATE_FIRST;
extern int LEFT_SERDES_INTERRUPT_STATE_FORCE;
extern int LEFT_SERDES_INTERRUPT_STATE_MACHINE_CHECK;
extern int LEFT_SERDES_INTERRUPT_STATE_CRITICAL;
extern int LEFT_SERDES_INTERRUPT_STATE_NONCRITICAL;
extern int LEFT_SERDES_INTERRUPT_STATE_LOCAL_RING[];
extern int LEFT_SERDES_INTERRUPT_STATE_A_PLLA_LOCK_LOST[];
extern int LEFT_SERDES_INTERRUPT_STATE_B_PLLA_LOCK_LOST[];
extern int LEFT_SERDES_INTERRUPT_STATE_C_PLLA_LOCK_LOST[];
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
extern int LEFT_INTERRUPT_INTERNAL_ERROR_STATE;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_FIRST;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_FORCE;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_MACHINE_CHECK;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CRITICAL;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_NONCRITICAL;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_BAD_ADDRESS[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_BAD_PRIV[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_BAD_DATA_PARITY[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_BAD_ADDRESS_PARITY[];
// Interrupt Control Register: serdes_interrupt_state_control_low
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_LOW;
// Interrupt Control Register: serdes_interrupt_state_control_high
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_HIGH;
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_LOCAL_RING[];
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_A_PLLA_LOCK_LOST[];
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_B_PLLA_LOCK_LOST[];
extern int LEFT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_C_PLLA_LOCK_LOST[];
// Interrupt Control Register: interrupt_internal_error_control_high
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH;
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_ADDRESS[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_PRIV[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_DATA_PARITY[];
extern int LEFT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_ADDRESS_PARITY[];
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Automatically generated with dcr-h-to-perl, do not modify
// stk@de.ibm.com
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//   Name:        rcbus
extern int RIGHT_RCBUS;
extern int RIGHT_RCBUS_WMASK[];
extern int RIGHT_RCBUS_MASKED_WRITE[];
extern int RIGHT_RCBUS_DATA[];
//   Name:        rcbus_config
extern int RIGHT_RCBUS_CONFIG;
extern int RIGHT_RCBUS_CONFIG_ADDRESS[];
extern int RIGHT_RCBUS_CONFIG_RESET[];
//   Name:        sd_config
extern int RIGHT_SD_CONFIG;
extern int RIGHT_SD_CONFIG_PCIE_DISABLE[];
extern int RIGHT_SD_CONFIG_HSSL2PLLON[];
extern int RIGHT_SD_CONFIG_PCS_TESTMODE[];
extern int RIGHT_SD_CONFIG_HALFRATE[];
//   Name:        ts_ctl_d
extern int RIGHT_TS_CTL_D;
extern int RIGHT_TS_CTL_D_PCS_RESET[];
extern int RIGHT_TS_CTL_D_PIPE_RESETN[];
extern int RIGHT_TS_CTL_D_RX_RESET_M[];
extern int RIGHT_TS_CTL_D_RX_RESET_P[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_RUN_M[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_RUN_OVR_M[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_LAT_M[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_RUN_P[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_RUN_OVR_P[];
extern int RIGHT_TS_CTL_D_RX_ELASTIC_BUF_LAT_P[];
extern int RIGHT_TS_CTL_D_TX_ELASTIC_BUF_RUN_M[];
extern int RIGHT_TS_CTL_D_TX_ELASTIC_BUF_LAT_M[];
extern int RIGHT_TS_CTL_D_TX_ELASTIC_BUF_RUN_P[];
extern int RIGHT_TS_CTL_D_TX_ELASTIC_BUF_LAT_P[];
extern int RIGHT_TS_CTL_D_XMIT_SCRAMBLE_M[];
extern int RIGHT_TS_CTL_D_XMIT_SCRAMBLE_P[];
extern int RIGHT_TS_CTL_D_RCV_DESCRAMBLE_M[];
extern int RIGHT_TS_CTL_D_RCV_DESCRAMBLE_P[];
extern int RIGHT_TS_CTL_D_SEND_TRAIN_M[];
extern int RIGHT_TS_CTL_D_SEND_TRAIN_P[];
extern int RIGHT_TS_CTL_D_RCV_TRAIN_M[];
extern int RIGHT_TS_CTL_D_RCV_TRAIN_P[];
extern int RIGHT_TS_CTL_D_LOOPBACK_EN[];
extern int RIGHT_TS_CTL_D_DATALOOP_EN_M[];
extern int RIGHT_TS_CTL_D_DATALOOP_EN_P[];
extern int RIGHT_TS_CTL_D_PARLPBK_EN_M[];
extern int RIGHT_TS_CTL_D_PARLPBK_EN_P[];
extern int RIGHT_TS_CTL_D_REFCLK_DISABLE[];
extern int RIGHT_TS_CTL_D_LANE_RESET_M[];
extern int RIGHT_TS_CTL_D_LANE_RESET_P[];
extern int RIGHT_TS_CTL_D_HSS_AREFEN[];
//   Name:        ts_ctl_e
extern int RIGHT_TS_CTL_E;
extern int RIGHT_TS_CTL_E_PCS_RESET[];
extern int RIGHT_TS_CTL_E_PIPE_RESETN[];
extern int RIGHT_TS_CTL_E_RX_RESET_M[];
extern int RIGHT_TS_CTL_E_RX_RESET_P[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_RUN_M[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_RUN_OVR_M[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_LAT_M[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_RUN_P[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_RUN_OVR_P[];
extern int RIGHT_TS_CTL_E_RX_ELASTIC_BUF_LAT_P[];
extern int RIGHT_TS_CTL_E_TX_ELASTIC_BUF_RUN_M[];
extern int RIGHT_TS_CTL_E_TX_ELASTIC_BUF_LAT_M[];
extern int RIGHT_TS_CTL_E_TX_ELASTIC_BUF_RUN_P[];
extern int RIGHT_TS_CTL_E_TX_ELASTIC_BUF_LAT_P[];
extern int RIGHT_TS_CTL_E_XMIT_SCRAMBLE_M[];
extern int RIGHT_TS_CTL_E_XMIT_SCRAMBLE_P[];
extern int RIGHT_TS_CTL_E_RCV_DESCRAMBLE_M[];
extern int RIGHT_TS_CTL_E_RCV_DESCRAMBLE_P[];
extern int RIGHT_TS_CTL_E_SEND_TRAIN_M[];
extern int RIGHT_TS_CTL_E_SEND_TRAIN_P[];
extern int RIGHT_TS_CTL_E_RCV_TRAIN_M[];
extern int RIGHT_TS_CTL_E_RCV_TRAIN_P[];
extern int RIGHT_TS_CTL_E_LOOPBACK_EN[];
extern int RIGHT_TS_CTL_E_DATALOOP_EN_M[];
extern int RIGHT_TS_CTL_E_DATALOOP_EN_P[];
extern int RIGHT_TS_CTL_E_PARLPBK_EN_M[];
extern int RIGHT_TS_CTL_E_PARLPBK_EN_P[];
extern int RIGHT_TS_CTL_E_REFCLK_DISABLE[];
extern int RIGHT_TS_CTL_E_LANE_RESET_M[];
extern int RIGHT_TS_CTL_E_LANE_RESET_P[];
extern int RIGHT_TS_CTL_E_HSS_AREFEN[];
//   Name:        ts_ctl_io
extern int RIGHT_TS_CTL_IO;
extern int RIGHT_TS_CTL_IO_PCS_RESET[];
extern int RIGHT_TS_CTL_IO_PIPE_RESETN[];
extern int RIGHT_TS_CTL_IO_RX_RESET[];
extern int RIGHT_TS_CTL_IO_RX_ELASTIC_BUF_RUN[];
extern int RIGHT_TS_CTL_IO_RX_ELASTIC_BUF_RUN_OVR[];
extern int RIGHT_TS_CTL_IO_RX_ELASTIC_BUF_LAT[];
extern int RIGHT_TS_CTL_IO_TX_ELASTIC_BUF_RUN[];
extern int RIGHT_TS_CTL_IO_TX_ELASTIC_BUF_LAT[];
extern int RIGHT_TS_CTL_IO_XMIT_SCRAMBLE[];
extern int RIGHT_TS_CTL_IO_RCV_DESCRAMBLE[];
extern int RIGHT_TS_CTL_IO_SEND_TRAIN[];
extern int RIGHT_TS_CTL_IO_RCV_TRAIN[];
extern int RIGHT_TS_CTL_IO_LOOPBACK_EN[];
extern int RIGHT_TS_CTL_IO_DATALOOP_EN[];
extern int RIGHT_TS_CTL_IO_PARLPBK_EN[];
extern int RIGHT_TS_CTL_IO_REFCLK_DISABLE[];
extern int RIGHT_TS_CTL_IO_LANE_RESET[];
extern int RIGHT_TS_CTL_IO_HSS_AREFEN[];
//   Name:        ts_align_stat_d
extern int RIGHT_TS_ALIGN_STAT_D;
extern int RIGHT_TS_ALIGN_STAT_D_LANE0_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE0_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE1_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE1_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE2_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE2_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE3_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_D_LANE3_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_D_BYTE_ALIGNED_M[];
extern int RIGHT_TS_ALIGN_STAT_D_BYTE_ALIGNED_P[];
extern int RIGHT_TS_ALIGN_STAT_D_FOURBYTE_ALIGNED_M[];
extern int RIGHT_TS_ALIGN_STAT_D_FOURBYTE_ALIGNED_P[];
//   Name:        ts_align_stat_e
extern int RIGHT_TS_ALIGN_STAT_E;
extern int RIGHT_TS_ALIGN_STAT_E_LANE0_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE0_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE1_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE1_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE2_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE2_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE3_MUXSEL_M[];
extern int RIGHT_TS_ALIGN_STAT_E_LANE3_MUXSEL_P[];
extern int RIGHT_TS_ALIGN_STAT_E_BYTE_ALIGNED_M[];
extern int RIGHT_TS_ALIGN_STAT_E_BYTE_ALIGNED_P[];
extern int RIGHT_TS_ALIGN_STAT_E_FOURBYTE_ALIGNED_M[];
extern int RIGHT_TS_ALIGN_STAT_E_FOURBYTE_ALIGNED_P[];
//   Name:        ts_align_stat_io
extern int RIGHT_TS_ALIGN_STAT_IO;
extern int RIGHT_TS_ALIGN_STAT_IO_LANE0_MUXSEL[];
extern int RIGHT_TS_ALIGN_STAT_IO_LANE1_MUXSEL[];
extern int RIGHT_TS_ALIGN_STAT_IO_LANE2_MUXSEL[];
extern int RIGHT_TS_ALIGN_STAT_IO_LANE3_MUXSEL[];
extern int RIGHT_TS_ALIGN_STAT_IO_BYTE_ALIGNED[];
extern int RIGHT_TS_ALIGN_STAT_IO_FOURBYTE_ALIGNED[];
//   Name:        ts_hss_pll_stat
extern int RIGHT_TS_HSS_PLL_STAT;
extern int RIGHT_TS_HSS_PLL_STAT_D_PLLA_LOCKED[];
extern int RIGHT_TS_HSS_PLL_STAT_IO_PLLA_LOCKED[];
extern int RIGHT_TS_HSS_PLL_STAT_E_PLLB_LOCKED[];
//   Name:        ts_hss_lane_stat
extern int RIGHT_TS_HSS_LANE_STAT;
extern int RIGHT_TS_HSS_LANE_STAT_LANE_READY_D_M[];
extern int RIGHT_TS_HSS_LANE_STAT_LANE_READY_D_P[];
extern int RIGHT_TS_HSS_LANE_STAT_LANE_READY_E_M[];
extern int RIGHT_TS_HSS_LANE_STAT_LANE_READY_E_P[];
extern int RIGHT_TS_HSS_LANE_STAT_LANE_READY_IO[];
//   Name:        prbs_ctl_d
extern int RIGHT_PRBS_CTL_D;
extern int RIGHT_PRBS_CTL_D_TX_PRBS_EN_M[];
extern int RIGHT_PRBS_CTL_D_TX_PRBS_EN_P[];
extern int RIGHT_PRBS_CTL_D_RX_PRBS_EN_M[];
extern int RIGHT_PRBS_CTL_D_RX_PRBS_EN_P[];
extern int RIGHT_PRBS_CTL_D_PRBS_SEL_M[];
extern int RIGHT_PRBS_CTL_D_PRBS_SEL_P[];
extern int RIGHT_PRBS_CTL_D_PRBS_RST_M[];
extern int RIGHT_PRBS_CTL_D_PRBS_RST_P[];
//   Name:        prbs_ctl_e
extern int RIGHT_PRBS_CTL_E;
extern int RIGHT_PRBS_CTL_E_TX_PRBS_EN_M[];
extern int RIGHT_PRBS_CTL_E_TX_PRBS_EN_P[];
extern int RIGHT_PRBS_CTL_E_RX_PRBS_EN_M[];
extern int RIGHT_PRBS_CTL_E_RX_PRBS_EN_P[];
extern int RIGHT_PRBS_CTL_E_PRBS_SEL_M[];
extern int RIGHT_PRBS_CTL_E_PRBS_SEL_P[];
extern int RIGHT_PRBS_CTL_E_PRBS_RST_M[];
extern int RIGHT_PRBS_CTL_E_PRBS_RST_P[];
//   Name:        prbs_ctl_io
extern int RIGHT_PRBS_CTL_IO;
extern int RIGHT_PRBS_CTL_IO_TX_PRBS_EN[];
extern int RIGHT_PRBS_CTL_IO_RX_PRBS_EN[];
extern int RIGHT_PRBS_CTL_IO_PRBS_SEL[];
extern int RIGHT_PRBS_CTL_IO_PRBS_RST[];
//   Name:        prbs_stat_d
extern int RIGHT_PRBS_STAT_D;
extern int RIGHT_PRBS_STAT_D_SYNC_M[];
extern int RIGHT_PRBS_STAT_D_SYNC_P[];
extern int RIGHT_PRBS_STAT_D_ERROR_M[];
extern int RIGHT_PRBS_STAT_D_ERROR_P[];
//   Name:        prbs_stat_e
extern int RIGHT_PRBS_STAT_E;
extern int RIGHT_PRBS_STAT_E_SYNC_M[];
extern int RIGHT_PRBS_STAT_E_SYNC_P[];
extern int RIGHT_PRBS_STAT_E_ERROR_M[];
extern int RIGHT_PRBS_STAT_E_ERROR_P[];
//   Name:        prbs_stat_io
extern int RIGHT_PRBS_STAT_IO;
extern int RIGHT_PRBS_STAT_IO_SYNC[];
extern int RIGHT_PRBS_STAT_IO_ERROR[];
//   Name:        error_inject_timer_val
extern int RIGHT_ERROR_INJECT_TIMER_VAL;
extern int RIGHT_ERROR_INJECT_TIMER_VAL_RND[];
extern int RIGHT_ERROR_INJECT_TIMER_VAL_COUNT[];
//   Name:        error_inject_limit
extern int RIGHT_ERROR_INJECT_LIMIT;
extern int RIGHT_ERROR_INJECT_LIMIT_RND_MASK[];
extern int RIGHT_ERROR_INJECT_LIMIT_LOW_LIMIT[];
//   Name:        error_inject_control
extern int RIGHT_ERROR_INJECT_CONTROL;
extern int RIGHT_ERROR_INJECT_CONTROL_ENABLE[];
extern int RIGHT_ERROR_INJECT_CONTROL_LINK_SELECT[];
extern int RIGHT_ERROR_INJECT_CONTROL_MASK[];
//   Name:        interrupt_internal_error_sw_info
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO_READ[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO_WRITE[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO_PRIVILEGED[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO_TEST_INT[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_SW_INFO_ADDRESS[];
//   Name:        interrupt_internal_error_hw_info
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_HW_INFO;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_HW_INFO_ADDRESS_PARITY_GOOD[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_HW_INFO_DATA_PARITY_GOOD[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_HW_INFO_ADDRESS[];
//   Name:        interrupt_internal_error_data_info
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_DATA_INFO;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_DATA_INFO_DATA[];
// Interrupt State Register: serdes_interrupt_state
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
extern int RIGHT_SERDES_INTERRUPT_STATE_STATE;
extern int RIGHT_SERDES_INTERRUPT_STATE_FIRST;
extern int RIGHT_SERDES_INTERRUPT_STATE_FORCE;
extern int RIGHT_SERDES_INTERRUPT_STATE_MACHINE_CHECK;
extern int RIGHT_SERDES_INTERRUPT_STATE_CRITICAL;
extern int RIGHT_SERDES_INTERRUPT_STATE_NONCRITICAL;
extern int RIGHT_SERDES_INTERRUPT_STATE_LOCAL_RING[];
extern int RIGHT_SERDES_INTERRUPT_STATE_D_PLLA_LOCK_LOST[];
extern int RIGHT_SERDES_INTERRUPT_STATE_E_PLLB_LOCK_LOST[];
extern int RIGHT_SERDES_INTERRUPT_STATE_IO_PLLA_LOCK_LOST[];
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_STATE;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_FIRST;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_FORCE;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_MACHINE_CHECK;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CRITICAL;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_NONCRITICAL;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_BAD_ADDRESS[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_BAD_PRIV[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_BAD_DATA_PARITY[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_BAD_ADDRESS_PARITY[];
// Interrupt Control Register: serdes_interrupt_state_control_low
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_LOW;
// Interrupt Control Register: serdes_interrupt_state_control_high
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_HIGH;
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_LOCAL_RING[];
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_D_PLLA_LOCK_LOST[];
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_E_PLLB_LOCK_LOST[];
extern int RIGHT_SERDES_INTERRUPT_STATE_CONTROL_HIGH_IO_PLLA_LOCK_LOST[];
// Interrupt Control Register: interrupt_internal_error_control_high
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH;
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_ADDRESS[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_PRIV[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_DATA_PARITY[];
extern int RIGHT_INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_BAD_ADDRESS_PARITY[];
