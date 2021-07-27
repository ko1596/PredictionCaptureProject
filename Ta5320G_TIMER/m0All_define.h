

#ifndef ALL_DEFINE_H
#define ALL_DEFINE_H


//********************************************************************
#define 	A53M0_UART_COMMAND						1




//	TEST	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define 	TST_TxRx_COUNT								1

//#define 	TST_DEBUF_PRINT_001						1
//#define 	TST_RXUART_SHOW_01						1
//#define 	TST_RXUART_SHOW_02						1

#define 	TST_M0_SYSINFO_PRINT					1
#define 	TST_M0_MGC_DATA_PRINT					1
#define 	TST_M0_G_DATA_PRINT						1
#define 	TST_M0_RTC_PRINT							1
#define 	TST_M0_TEMP_PRINT							1
#define 	TST_M0_PW_METER_PRINT					1

#define 	TST_M0_ADC_DATA_PRINT					1
#define 	TST_M0_ADC_DATA_PRINT2				1

#define 	TST_M0_RADAR_A_DATA_PRINT			0
#define 	TST_M0_RADAR_B_DATA_PRINT			0

#define 	TST_M0_RADAR_INIT_01					1
#define 	TST_M0_RADAR_INIT_02					1

#define 	TST_M0_TLSAM_01								1
#define 	TST_M0_TLSAM_SAVE_FILE				1

#define 	Tst_Radar_PW_OFFON						1

#define 	Tst_PW_Change									1

#define  	Tst_JSON_T01 									1

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





//********************************************************************
//====================================================================
//--------------------------------------------------------------------
//....................................................................
#define   UINT8       uint8_t
#define   UINT16      uint16_t
#define   UINT32      uint32_t

#define   FAILURE     0
#define   SUCCESS     1


//====================================================================
//  Device SubType of UART CMD
//====================================================================
#define 	SUBTYPE_RTC								0x01
#define 	SUBTYPE_MGC3130						0x11
#define 	SUBTYPE_ADC								0x12
#define 	SUBTYPE_G_SENSOR					0x13

#define 	SUBTYPE_RADAR_A						0x21
#define 	SUBTYPE_RADAR_B						0x22

#define 	SUBTYPE_TEMPERATURE				0x30

#define 	SUBTYPE_PW_METER0					0x38
#define 	SUBTYPE_PW_METER1					0x39
#define 	SUBTYPE_PW_METER2					0x3A
#define 	SUBTYPE_PW_METER3					0x3B

#define 	SUBTYPE_PW_BAT						0x40
#define 	SUBTYPE_PW_BAT_A					0x41
#define 	SUBTYPE_PW_BAT_B					0x42

#define 	SUBTYPE_TLSAM							0x71

#define 	SUBTYPE_ALL								0xFF



//--------------------------------------------------------------------
//	flag of M0_Status_Update
//	M0_Status_Update
#define  	M0_DEV_UPDATE_RTC 					0x00000001  		//0000 0000  0000 0000  0000 0000  0000 0001
#define  	M0_DEV_UPDATE_MGC 					0x00000002  		//0000 0000  0000 0000  0000 0000  0000 0010
#define  	M0_DEV_UPDATE_G_SENSOR			0x00000004  		//0000 0000  0000 0000  0000 0000  0000 0100
#define  	M0_DEV_UPDATE_TEMP					0x00000008  		//0000 0000  0000 0000  0000 0000  0000 1000

#define  	M0_DEV_UPDATE_ADC_1					0x00001000  		//0000 0000  0000 0000  0001 0000  0000 0000
#define  	M0_DEV_UPDATE_ADC_2					0x00002000  		//0000 0000  0000 0000  0010 0000  0000 0000
#define  	M0_DEV_UPDATE_ADC_3					0x00004000  		//0000 0000  0000 0000  0100 0000  0000 0000

#define  	M0_DEV_UPDATE_RADAR_A				0x00000010  		//0000 0000  0000 0000  0000 0000  0001 0000
#define  	M0_DEV_UPDATE_RADAR_B				0x00000020  		//0000 0000  0000 0000  0000 0000  0010 0000

#define  	M0_DEV_UPDATE_PWMETER0			0x00000100  		//0000 0000  0000 0000  0000 0001  0000 0000
#define  	M0_DEV_UPDATE_PWMETER1			0x00000200  		//0000 0000  0000 0000  0000 0010  0000 0000
#define  	M0_DEV_UPDATE_PWMETER2			0x00000400  		//0000 0000  0000 0000  0000 0100  0000 0000
#define  	M0_DEV_UPDATE_PWMETER3			0x00000800  		//0000 0000  0000 0000  0000 1000  0000 0000

#define  	M0_DEV_UPDATE_TLSAM					0x08000000  		//0000 1000  0000 0000  0000 0000  0000 0000

#define  	M0_DEV_UPDATE_SYSINFO				0x80000000  		//1000 0000  0000 0000  0000 0000  0000 0000


//--------------------------------------------------------------------
#define 	PW_EN_BAT_A			0x01
#define 	PW_EN_BAT_B			0x02
#define 	PW_EN_DC_IN			0x04
#define 	PW_EN_SOLAR			0x05




//********************************************************************
//	A53 - M0  UART
//********************************************************************
typedef struct _uart_fifo {
    UINT8*  buffer; // used as a ring buffer
    UINT16  front;          // Head index of queue
    UINT16  tail;         // next new index of queue
    UINT8   buffull;        // =1, queue full
    /*
    UINT8   NC1;          // No used
    UINT8   NC2;          // No used
    UINT8   NC3;          // No used
    */
    UINT8     STATE;
  	UINT16    PTR;
  	UINT16    LENGTH;
  	UINT16    CMD;
  	UINT16    CRC;
}M0_UART_FIFO;






//********************************************************************
//	M0 device 
//********************************************************************

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


typedef struct  {
  UINT8           FW_ver[32];
  M0_RTC_TIME_FRAME  mcu_RTC;

  //  TEST !!!
  UINT32 	sysTick;
  UINT32  TstCnt;   //  TEST
  UINT32  TstBuf_t; 
  UINT32  uartM0_RxCnt;
}M0_SYSTEM_INFO;




//==============================================================================
//  device status
//==============================================================================

//  M0 ALarm A53  	------------------------------------------------------------
typedef struct  {
	UINT32 	alarm;
	M0_RTC_TIME_FRAME  	mcu_RTC;
}M0_ALARM_A53_FRAME;

//  ADC   ----------------------------------------------------------------------
/*
	#define 	PW_EN_BAT_A			0x01
	#define 	PW_EN_BAT_B			0x02
	#define 	PW_EN_DC_IN			0x04
	#define 	PW_EN_SOLAR			0x05
*/
typedef struct  {	
  UINT32 adc0;
  UINT32 adc1;
  UINT32 adc2;
  UINT32 adc3;
	UINT32 pw_EN;		//	bit0,	Bat_A
									//	bit1,	Bat_B
									//	bit2,	DC_In
									//	bit3,	Solar
  M0_RTC_TIME_FRAME  mcu_RTC;
}M0_ADC_DATA_FRAME;

//  LM75-Sensor   --------------------------------------------------------------
typedef struct  {
	UINT32 alarm_temp_H;
	UINT32 alarm_temp_L;
}M0_TEMP_ALARMM_FRAME;

typedef struct  {
	UINT32 								temp0; 		//	xxx.xx;  negative : 0x80000000 true
	M0_TEMP_ALARMM_FRAME	alarm;
	M0_RTC_TIME_FRAME  		mcu_RTC;
}M0_TEMP_DATA_FRAME;


//  MGC3130   ------------------------------------------------------------------
typedef struct  {
  UINT32 mgc;
  M0_RTC_TIME_FRAME  mcu_RTC;
}M0_MGC_DATA_FRAME;


//  Radar   	------------------------------------------------------------------
//	Radar-RB-60P(台灣版) UART Portocol_210609
//	UINT8 L_R :		=0, Right side
//								=1, Left side
typedef struct  {
	UINT16 		radar_X;
	
	UINT16 		radar_Y;
	UINT16 		radar_L;

	UINT16 		radar_W;
	UINT16 		radar_H;

	UINT8 		Theta;
	UINT8 		Phi;

	UINT8 		L_R;
	UINT8 		nc[9]; 			// 	"210610-020Cea"
}M0_RADAR_SET_FRAME;


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
	
	UINT16 		power;
	UINT8  		recordPos_X;
	UINT8  		recordPos_Y;

	UINT8  		recordPos_Z;
	UINT8  		record_Distance;
	UINT16  	record_power;

	UINT16  	motion_index1;
	UINT16  	motion_index2;

	UINT16  	block_index;
	UINT16  	nc01;	
}M0_RADAR_DATA_FRAME;


typedef struct  {
	UINT8 								FW_version[4];
	M0_RADAR_DATA_FRAME		data;
	M0_RTC_TIME_FRAME  		mcu_RTC;
}M0_RADAR_FRAME;


//  DC Power Meter 		----------------------------------------------------------
//	LTC2944
typedef struct  {
	UINT32 								bat_v;
	UINT32 								bat_i;
	UINT32 								temp;	
	M0_RTC_TIME_FRAME  		mcu_RTC;
}M0_PW_METER_DATA_FRAME;


//  G sensor, ICM20602 		------------------------------------------------------
// 	ICM20602
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


//  TL-SAM   	------------------------------------------------------------------
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




//	TEST 		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define 	TST_SHOW_UART_TxData 		1
//#define 	TST_SHOW_UART_RxData 		1
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#endif    //ALL_DEFINE_H

