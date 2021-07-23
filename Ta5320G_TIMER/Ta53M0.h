

//********************************************************************
//  Ta53M0.h
//********************************************************************

#ifndef TA53M0_UART_H
#define TA53M0_UART_H




//********************************************************************
//	define
#define   UARTA53M0_TX_BUFFER_SIZE    	2048
//#define   UARTA53M0_RX_BUFFER_SIZE    	(2048+1024)
#define   UARTA53M0_RX_BUFFER_SIZE    	2048
#define   UARTA53M0_SPEED    						B115200
//#define   UARTA53M0_SPEED    						B460800
#define   UARTA53M0_RX_Q_BUFFER_SIZE    0x1000 		//	4096


//====================================================================
//  UART Command
#define    uartCmd_REQ_M0_DUMMING 	0x00
#define    uartCmd_RSP_M0_DUMMING 	0x80


#define    uartCmd_REQ_M0_INFO 			0x01
#define    uartCmd_RSP_M0_INFO 			0x81

#define    uart_REQ_DEV_FW_VER 			0x21

#define    uart_SET_DEV 						0x50
#define    uart_RSP_DEV 						0xD0

#define    uart_REQ_DEV_STATUS 			0x51
#define    uart_RSP_DEV_STATUS 			0xD1

#define    uart_REQ_SAVE_EEPROM 		0x5A
#define    uart_RSP_SAVE_EEPROM 		0xDA

#define    uart_REQ_READ_EEPROM 		0x5B
#define    uart_RSP_READ_EEPROM 		0xDB

#define    uart_REQ_DEV_RESTART			0x5F
#define    uart_RSP_DEV_RESTART			0xDF

#define    uart_REQ_ENC							0x68
#define    uart_RSP_ENC							0xE8

#define    uartCmd_SET_M0_RTC       0x60
#define    uartCmd_REQ_M0_RTC       0x60
#define    uartCmd_RSP_M0_RTC       0xE0

#define    uart_REQ_ALARM 					0x70
#define    uart_RSP_ALARM 					0xF0


//====================================================================
//	Alarm Sub-Type;			M0 alarm A53
//	#define    uart_REQ_ALARM 					0x70
//	#define    uart_RSP_ALARM 					0xF0
#define    uart_ALARM_TEMP 								0x10

#define    uart_ALARM_RADAR_PWOFF					0x70
#define    uart_ALARM_RADAR_PWON					0x71


//====================================================================
//	ATSAML22 M0
//====================================================================



//********************************************************************
//  Global 		-- M0 device information
//********************************************************************
/*
		New information upload from M0 	----------------------------------
		bit0 :		M0 RTC update, 									(M0_RTC_TIME_FRAME) 			M0_rtc;
		bit1 :		M0 mgc3130 update, 							(M0_MGC_DATA_FRAME) 			M0_mgc3130;
		bit2 :		M0 G-sensor update, 						(G_SENSOR_DATA_FRAME) 		g_icm_Data;
		bit3 :		M0 temperature update

		bit4 :		M0 Radar-A update, 							(M0_RADAR_FRAME) 					M0_radarA;
		bit5 :		M0 Radar-B update, 							(M0_RADAR_FRAME) 					M0_radarB;
		bit6 :
		bit7 :

		bit8 :		M0 Power of Battery A update
		bit9 :		M0 Power of Battery B update
		bit10 :
		bit11 :

		bit12 :		M0 ADC of Battery-A update
		bit13 :		M0 ADC of Battery-B update
		bit14 :		M0 ADC of Battery-Solar update
		bit15 :

		bit16 :
		bit17 :
		bit18 :
		bit19 :

		bit20 :
		bit21 :
		bit22 :
		bit23 :		M0 SAM update

		bit24 :
		bit25 :
		bit26 :
		bit27 :

		bit28 :		M0 Button - Burn/ QC Mode
		bit29 :		M0 Button - Test Mode
		bit30 :
		bit31 :		M0 System Information, 					(M0_SYSTEM_INFO) 			M0_System_info;
*/
extern 	UINT32 						M0_Status_Update;

/*
	typedef struct {
	  UINT8   Year;
	  UINT8   Month;
	  UINT8   Day;
	  UINT8   WeekDay;

	  UINT8   Hour;
	  UINT8   Minute;
	  UINT8   Second;  
	  UINT8   nc;
	}M0_RTC_TIME_FRAME;
*/
extern 	M0_RTC_TIME_FRAME 	M0_rtc;


/*
	typedef struct  {
	  UINT8           FW_ver[32];
	  M0_RTC_TIME_FRAME  mcu_RTC;

	  //  TEST !!!
	  UINT32 	sysTick;
	  UINT32  TstCnt;   //  TEST
	  UINT32  TstBuf_t; 
	  UINT32  uartM0_RxCnt;
	}M0_SYSTEM_INFO;
*/
extern 	M0_SYSTEM_INFO 				M0_System_info;

/*
	typedef struct  {
	  UINT32 mgc;
	  M0_RTC_TIME_FRAME  mcu_RTC;
	}M0_MGC_DATA_FRAME;
*/
extern 	M0_MGC_DATA_FRAME 		M0_mgc3130;

/*
	12 bits ADC; 0 ~ 4095
	6.2V ~ 8.4V
	//	8.4V	:	ADC 	3445 ~ 3430
	//	8.0V	:	ADC[]
	//	6.2V	:	ADC[]
	typedef struct  {	
		UINT32 adc0;
		UINT32 adc1;
		UINT32 adc2;
		UINT32 adc3;
		M0_RTC_TIME_FRAME  mcu_RTC;
	}M0_ADC_DATA_FRAME;
		*/
extern 	M0_ADC_DATA_FRAME 	M0_adc;

/*
	typedef struct  {
		UINT32 								temp0; 		//	xxx.xx;  negative : 0x80000000 true
		M0_TEMP_ALARMM_FRAME	alarm;
		M0_RTC_TIME_FRAME  		mcu_RTC;
	}M0_TEMP_DATA_FRAME;
*/
extern	M0_TEMP_DATA_FRAME 		M0_temperature;

/*
	typedef struct  {
		UINT32 								bat_v;
		UINT32 								bat_i;
		UINT32 								temp;	
		M0_RTC_TIME_FRAME  		mcu_RTC;
	}M0_PW_METER_DATA_FRAME;
*/
extern 	M0_PW_METER_DATA_FRAME	M0_PwMeter;


/*
//	"210610-020Ce0" 		Radar-RB-60P(台灣版) UART Portocol_210609 	................
typedef struct  {
	UINT16 		car_change;
	UINT16 		obj_position_X;

	UINT16 		obj_position_Y;
	UINT16 		obj_position_Z;

	UINT16 		obj_distance_R;
	UINT16 		radar_X;
	
	UINT16 		radar_Y;
	UINT16 		radar_L;

	UINT16 		radar_W;
	UINT16 		radar_H;

	UINT8 		parking_status;
	UINT8 		CarChange_status;
	UINT8 		obj_type;
	UINT8 		Theta;

	UINT8 		Phi;
	UINT8 		L_R;
	
	//  M0 used
	UINT8 		pre_parking_status;		//"   210524-020Be0"
	UINT8 		pre_car_change;				//"...210608-020Cc0"

	UINT8 		Reserved_01[4];
}M0_RADAR_DATA_FRAME;
*/
extern 	M0_RADAR_FRAME 				M0_radarA;
extern 	M0_RADAR_FRAME 				M0_radarB;

/*
typedef struct  {
	UINT16 		radar_X;
	
	UINT16 		radar_Y;
	UINT16 		radar_L;

	UINT16 		radar_W;
	UINT16 		radar_H;
	UINT8 		Theta;

	UINT8 		Phi;
	UINT8 		L_R;
	//UINT8 		scan_interval;
}M0_RADAR_SET_FRAME;
*/
extern 	M0_RADAR_SET_FRAME 		M0_radarA_set;
extern 	M0_RADAR_SET_FRAME 		M0_radarB_set;

/*
typedef struct _icm20602_data_frame_{
	UINT32 	g_fact;			//	+-2g : 16384 LSB/g
											//	+-4g : 8192 LSB/g
											//	+-8g : 4096 LSB/g
											//	+-16g : 2048 LSB/g

	//	mg
	UINT32	acc_X;
	UINT32	acc_Y;
	UINT32	acc_Z;
	
	UINT8		acc_wom_X;
	UINT8		acc_wom_Y;
	UINT8		acc_wom_Z;
	
	UINT8 	flg_INT;
}ICM20602_DATA_FRAME;

typedef struct  {
	ICM20602_DATA_FRAME 	icm20602;
	M0_RTC_TIME_FRAME  		mcu_RTC;
}M0_G_SENSOR_FRAME;
*/
extern 	M0_G_SENSOR_FRAME  g_icm_Data;

/*
	typedef struct  {
		UINT8 								FW_version[2];
		UINT16 								status;
		UINT8 								name[64];
		UINT32 								dataLength;
		UINT8 								data[1600];
	}M0_TLSAM_FRAME;

typedef struct  {
	M0_TLSAM_FRAME 		tlsam_Data;
	uint8_t 					status;
	uint8_t 					nc1;
	uint8_t 					nc2;
	uint8_t 					nc3;
}M0_TLSAM_DATA_FRAME;
*/
extern 	M0_TLSAM_DATA_FRAME			M0_TLSAM_DATA;





//********************************************************************
//	A53-M0 Global
extern 	unsigned char   FwVer_Ta53201_UART_HS[];

extern	unsigned char   dev_UART_A53M0[];

extern	pthread_t thread_uartA53M0_Tx;
extern	pthread_t thread_uartA53M0_Rx;

extern 	int fd_uartA53M0;



extern	unsigned char   A53M0_TxBuf[];
extern	unsigned char   A53M0_RxBuf[];
extern	int 	A53M0_TxLength;
extern	int 	A53M0_RxLength;

//	------------------------------------------------------------------
extern	time_t a53m0API_currentTime;
extern	struct tm *a53m0API_current_tm;

extern  int  tx_period;
extern  int  tx_No;




//******************************************************************************
//  Function
extern	void  uartA53M0_Tx_thread( int* fdPtr );
extern	void  uartA53M0_Rx_thread( int* fdPtr );

extern 	UINT16  Get_Kermit_CRC16( UINT8* dataPtr, UINT32 length );
extern 	UINT16  Get_ModBus_CRC16( UINT8* dataPtr, UINT32 length );



// 	TEST 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern 	int index_rx;

#if 	TST_TxRx_COUNT
	extern 	uint32_t 	Tst_Tx_Cnt;
	extern 	uint32_t 	Tst_Rx_Cnt;

	extern 	struct timeb  	time_Tx;
	extern 	struct timeb  	time_Rx;
#endif 	//#if 	TST_TxRx_COUNT

// TEST   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define   TST_TX_REQ_M0_SYS 			1




//******************************************************************************
//	API Function
//******************************************************************************

//	M0 System 		--------------------------------------------------------------
extern 	void 	uartA53M0_REQM0_SystemInfo( void ); 		// Resp.: 	(M0_SYSTEM_INFO)M0_System_info

extern 	void 	uartA53M0_SetM0_currentRTC( void ); 		//	DateTime of Linux
extern 	void 	uartA53M0_REQM0_RTC( void ); 						// Resp.: 	(M0_RTC_TIME_FRAME)M0_rtc


//	M0 - Devices 		------------------------------------------------------------
//	ADC
extern 	void 	uartA53M0_REQM0_ADC( void ); 						// Resp. 		(M0_ADC_DATA_FRAME)M0_adc


//	M0 - Devices 		------------------------------------------------------------
//	Temperature, 	LM75A
extern	void 	uartA53M0_REQM0_Temp( void );

//	M0 - Devices 		------------------------------------------------------------
//	Power Meter 	LTC29444
extern	void 	uartA53M0_REQM0_PwMeter( void );


//	Radar 		------------------------------------------------------------------
extern 	void 	uartA53M0_Set_Radar_ReStart( M0_RADAR_SET_FRAME* radarA_set_Ptr, M0_RADAR_SET_FRAME* radarB_set_Ptr );
extern 	void 	uartA53M0_Req_Radar_Status( void );

//extern 	void 	uartA53M0_Get_Radar_FWver( void );
//extern 	void 	uartA53M0_Set_Radar_Param( UINT8 A_B, UINT8 RW, M0_RADAR_SET_FRAME* radar_set_Ptr );


//	M0  EEPROM 		--------------------------------------------------------------
//	24AA64-I : 	512 Kbits
//	uint16_t addr,  must be n*0x10
//	uint16_t length,  MAX. 128 bytes
extern 	void 	uartA53M0_EEPROM_Save( uint16_t addr, uint16_t length, uint8_t* dataPtr );
extern 	void 	uartA53M0_EEPROM_Read( uint16_t addr, uint16_t length, uint8_t* dataPtr );

//	Power Source 		------------------------------------------------------------
extern 	void 	uartA53M0_Set_Bat_Source( UINT8 BatA_B );



//	TL-SAM Encryption 	--------------------------------------------------------
extern 	void 	uartA53M0_TLSAM_Encrypt( uint8_t *enc_file_name, uint8_t *enc_data, uint16_t enc_dataLength );


// 	TEST 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern 	void 	uartA53M0_Req_Dumming_Test( void );
//	check 	(UINT32)M0_Status_Update  to get New information of devices


#endif    //TA53M0_UART_H

