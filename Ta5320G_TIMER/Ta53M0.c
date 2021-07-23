

#include "m0All_Header.h"



//********************************************************************
//  Global
unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210625-20G100"; 		// New M0_RADAR_DATA_FRAME
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210625-20G06"; 		//#TIMER Test

//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210623-20F33"; 		//#define 	Tst_Radar_PW_OFFON						1   OFF
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210622-20F32"; 		//	test UART lost, TST_TxRx_COUNT
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210622-20F30T03"; 		//	test UART lost, TST_TxRx_COUNT

//--------------------------------------------------------------------
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210610-20D4aT10"; 		//	GET_RADAR_RESULT_AND_SETTING
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210610-20D42T02"; 		//	Radar-RB-60P(台灣版) UART Portocol_210609
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210609-20D31"; 		//	G ICM20602
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210609-20D20"; 		//	M0_RADAR_DATA_FRAME.pre_car_change
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210609-20D10"; 		//	"S" "s" : echo mem > /sys/power/state
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320G_TIMER  20210607-20D01"; 		//	ICM20602
//unsigned char   FwVer_Ta53201_UART_HS[] = "iMX8mm Ta5320C_Radar  20210607-20C10"; 		//	24AA64 	EEPROM




//====================================================================
//  
unsigned char   dev_UART_A53M0[] = "/dev/ttymxc2";


//********************************************************************
//  Global 		-- M0 device information
//********************************************************************

UINT32 						M0_Status_Update;

M0_RTC_TIME_FRAME 		M0_rtc;

M0_SYSTEM_INFO 				M0_System_info;

M0_MGC_DATA_FRAME 		M0_mgc3130;

M0_ADC_DATA_FRAME 		M0_adc;

M0_TEMP_DATA_FRAME 		M0_temperature;

M0_PW_METER_DATA_FRAME	M0_PwMeter;

M0_RADAR_FRAME 				M0_radarA;
M0_RADAR_FRAME 				M0_radarB;
M0_RADAR_SET_FRAME 		M0_radarA_set;
M0_RADAR_SET_FRAME 		M0_radarB_set;


M0_G_SENSOR_FRAME  	g_icm_Data;

M0_TLSAM_DATA_FRAME 	M0_TLSAM_DATA;




//====================================================================
pthread_t thread_uartA53M0_Tx;
pthread_t thread_uartA53M0_Rx;

int fd_uartA53M0;


//********************************************************************
//  Global 		-- M0 device
M0_SYSTEM_INFO 		M0_System_info;
M0_MGC_DATA_FRAME 		M0_mgc3130;









//====================================================================
time_t a53m0API_currentTime;
struct tm *a53m0API_current_tm;

int  tx_period;
int  tx_No;


// 	TEST 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int index_rx = 0;

#if 	TST_TxRx_COUNT
	uint32_t 	Tst_Tx_Cnt;
	uint32_t 	Tst_Rx_Cnt;

	struct timeb  	time_Tx;
	struct timeb  	time_Rx;
#endif 	//#if 	TST_TxRx_COUNT


