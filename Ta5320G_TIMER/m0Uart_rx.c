

#include "m0All_Header.h"


//********************************************************************
//	Global
unsigned char   A53M0_RxBuf[UARTA53M0_RX_BUFFER_SIZE];
int 	A53M0_RxLength;
int 	A53M0_rx;

unsigned char   A53M0_Rx_QBuf[UARTA53M0_RX_Q_BUFFER_SIZE + 0x10];
M0_UART_FIFO		A53M0_RxQ;

UINT8 	rx_payload[UARTA53M0_RX_BUFFER_SIZE];


UINT32 	crc_ErrorCnt = 0;

#if 	Tst_PW_Change
	int 	Tst_PW_ChangeCnt;
	int 	Tst_PrePW_state;
#endif 	//#if 	Tst_PW_Change



//********************************************************************
void  uartA53M0_uart_Clear( void );
int  uartA53M0_uart_Get( UINT8 *rx_data );

void 	uartM0_rsp_m0SysInfo( UINT8* rx_payload );
void 	uartM0_rsp_DevStatus( UINT8* rx_payload );
void 	uartM0_rsp_ENC( UINT8* rx_payload );
void 	uartM0_rsp_M0RTC( UINT8* rx_payload );
void 	uartM0_rsp_read_EEP( UINT8* rx_payload );

void 	uartM0_rsp_M0_Alarm( UINT8* rx_payload );
void 	uartM0_rsp_DummingTest( UINT8* rx_payload );




//********************************************************************
//
//********************************************************************
void  uartA53M0_Rx_thread( int* fdPtr )
{
	int i;
  a53m0API_currentTime = time( NULL);
  a53m0API_current_tm = localtime (&a53m0API_currentTime);

  int  Tst_M0_RxCnt;
  int  Tst_M0_RxLen;

	UINT8 	uart_data;	
	UINT16 	crc16;

	

#if 	Tst_PW_Change
	Tst_PW_ChangeCnt = 0;
#endif 	//#if 	Tst_PW_Change
  
  printf( "\n********** **********  Thread uartA53M0_Rx_thread Start  ********** ********** \n\n" );

  //	Device status Init. 	------------------------------------------
  M0_Status_Update = 0x0000;

  
  //==================================================================
  A53M0_RxQ.buffer = A53M0_Rx_QBuf;
  A53M0_RxQ.front = A53M0_RxQ.tail = A53M0_RxQ.buffull = 0;
  A53M0_RxQ.CMD = A53M0_RxQ.PTR = A53M0_RxQ.STATE = A53M0_RxQ.LENGTH = 0;


  int res;
  //==================================================================
  //
  //==================================================================
  while( 1 )  {
    a53m0API_currentTime = time( NULL);
    a53m0API_current_tm = localtime (&a53m0API_currentTime);

    //================================================================
    res = read( *fdPtr, (A53M0_RxBuf), UARTA53M0_RX_BUFFER_SIZE );

    if( res > 0 )		{
    	#if 	TST_RXUART_SHOW_01
	    	printf("res[%04d]: [", res);
		    for( i=0 ; i<res ; i++ )	{
		    	A53M0_RxQ.buffer[ A53M0_RxQ.tail ] = A53M0_RxBuf[ i ];
		    	A53M0_RxQ.tail = (A53M0_RxQ.tail+1) & (UARTA53M0_RX_Q_BUFFER_SIZE - 1);
		    	printf(" %02X", A53M0_RxBuf[i]);
		    } 	//for( i=0 ; i<res ; i++ )	{
		    printf("]\n");
    	#else
		    #if 	TST_RXUART_SHOW_02
		    	printf("Rx res[%d]\n", res );
		    #endif 		//#if 	TST_RXUART_SHOW_02

				for( i=0 ; i<res ; i++ )	{
					A53M0_RxQ.buffer[ A53M0_RxQ.tail ] = A53M0_RxBuf[ i ];
					A53M0_RxQ.tail = (A53M0_RxQ.tail+1) & (UARTA53M0_RX_Q_BUFFER_SIZE - 1);
				} 	//for( i=0 ; i<res ; i++ )	{
    	#endif 	//TST_RXUART_SHOW_01  	

    	//==============================================================
    	//	Get UART Cmd string 
    	//==============================================================
    	//printf(" Cmd[");
    	#if  A53M0_UART_COMMAND
		  	while( uartA53M0_uart_Get( &uart_data ) )  {
		  		//printf(" %02X", uart_data);

		  		switch( A53M0_RxQ.STATE )  {
		  			case 0:
			        if(uart_data == 0xCC) {
			          A53M0_RxQ.PTR = 0;
			          A53M0_RxQ.LENGTH = 0xFF;
			          A53M0_RxQ.STATE = 1;
			        }
			        break;

		 				case 1:
			        A53M0_RxQ.LENGTH = uart_data;
			        A53M0_RxQ.STATE = 2;
		 					break;
		 				case 2:
			        A53M0_RxQ.LENGTH |= (uart_data<<8);
			        A53M0_RxQ.STATE = 3;
			        if( A53M0_RxQ.LENGTH > (UARTA53M0_RX_BUFFER_SIZE-3) )		{
			        	printf("RxM0  -xxxLENGTHxxx- [%05d]\n", A53M0_RxQ.LENGTH );
			        	uartA53M0_uart_Clear( );
			        } 	//if( A53M0_RxQ.LENGTH > (UARTA53M0_RX_BUFFER_SIZE-3) )		{
		 					break;

		 				case 3:
			        A53M0_RxQ.CMD = uart_data;
			        A53M0_RxQ.STATE = 4;
		 					break;

		 				case 4:
			        A53M0_RxQ.CRC = uart_data;
			        A53M0_RxQ.STATE = 5;
		 					break;
		 				case 5:
			        A53M0_RxQ.CRC |= (uart_data<<8);
			        A53M0_RxQ.STATE = 6;
		 					break;

		 				//============================================================
		 				//
		 				//============================================================
		 				case 6:
		 					//	Get payload data 
		 					rx_payload[ A53M0_RxQ.PTR ] = uart_data;
		 					A53M0_RxQ.PTR++;

		 					if( A53M0_RxQ.PTR >= (A53M0_RxQ.LENGTH-3) )  {
		 						crc16 = Get_ModBus_CRC16( rx_payload, A53M0_RxQ.PTR );
		 						if( crc16 == A53M0_RxQ.CRC ) 		{

		 							//**************************************************
		 							//	UART CMD
		 							//**************************************************
		 							switch( A53M0_RxQ.CMD )		{

		 								case uartCmd_RSP_M0_INFO:
		 									//printf("--]\n");
		 									//printf("Rx uartCmd_RSP_M0_INFO ---------- ---------- \n");
		 									uartM0_rsp_m0SysInfo( rx_payload );
		 									break;

		 								case uart_RSP_DEV_STATUS:
		 									uartM0_rsp_DevStatus( rx_payload );
		 									break;

		 								case uartCmd_RSP_M0_RTC:
		 									uartM0_rsp_M0RTC( rx_payload );
		 									break;

		 								case uart_RSP_READ_EEPROM:
		 									uartM0_rsp_read_EEP( rx_payload );
		 									break;

		 								case uart_RSP_ENC:
		 									uartM0_rsp_ENC( rx_payload );
		 									break;

		 								case uart_RSP_ALARM:
		 									uartM0_rsp_M0_Alarm( rx_payload );
		 									break;



		 								case uartCmd_RSP_M0_DUMMING:
		 									uartM0_rsp_DummingTest( rx_payload );
		 									break;

		 							} 		//switch( A53M0_RxQ.CMD )		{
		 						} 		//if( crc16 == A53M0_RxQ.CRC ) 		{
		 						else	{
		 							crc_ErrorCnt++;
		 							printf("RxM0  -xxxCRCxxx-[%04X][%04X]  crcError[%012d]\n", crc16, A53M0_RxQ.CRC, crc_ErrorCnt );
		 						}

		 						//====================================================
		 						//printf(" uartA53M0_uart_Clear\n");
		 						uartA53M0_uart_Clear( );
		 					} 	//if( A53M0_RxQ.PTR >= A53M0_RxQ.LENGTH )  {
		 					break; 		////switch( A53M0_RxQ.STATE )  { 		case 6:


		 				//============================================================
		 				//============================================================
			      default:
			        //uartA53M0_uart_Clear( );
			        break;
		  		} 	//switch( A53M0_RxQ.STATE )  {
		  	} 	//while( uartA53M0_uart_Get( &uart_data ) )  {
		  	//printf("]\n");
	  	#endif 	//#if  A53M0_UART_COMMAND

		  //usleep( 10000 );   //  10ms
		  //usleep( 5000 );   //  5ms
    } 		//if( res > 0 )		{
    else {
    	//printf( ".r" );
    	//    	
    	//usleep( 100000 );   //  100ms
    	//usleep( 50000 );   //  50ms
    	//usleep( 30000 );   //  30ms
    	//usleep( 25000 );   //  25ms
    	//usleep( 20000 );   //  20ms
    	//usleep( 10000 );   //  10ms

    	//usleep( 3000 );   //  3ms    	
    	//usleep( 2000 );   //  2ms, 	OK
    	usleep( 1500 );   //  1.5ms,
    	//usleep( 1000 );   //  1ms, 	OK
    } 	//if( res < 1 )	{		else
  }
} 	//void  uartA53M0_Rx_thread( int* fdPtr )








//******************************************************************************
//
//******************************************************************************
void  uartA53M0_uart_Clear( void )
{
  A53M0_RxQ.CMD = A53M0_RxQ.PTR = A53M0_RxQ.STATE = A53M0_RxQ.LENGTH = 0;
}   //void  uartM4_Clear_State( void )



//********************************************************************
//
//********************************************************************
int  uartA53M0_uart_Get( UINT8 *rx_data )
{
  if( (A53M0_RxQ.tail==A53M0_RxQ.front) && (!A53M0_RxQ.buffull) ) {
    return FAILURE;
  }

  //------------------------------------------------------------------
  *rx_data = A53M0_RxQ.buffer[ A53M0_RxQ.front ];
  A53M0_RxQ.front = (A53M0_RxQ.front + 1) & (UARTA53M0_RX_Q_BUFFER_SIZE - 1);
  
  if( (A53M0_RxQ.buffull) && (A53M0_RxQ.tail!=A53M0_RxQ.front) )    {
    A53M0_RxQ.buffull = 0;
  }

  return  SUCCESS;
} 	//int  uartA53M0_uart_Get( UINT8 *rx_data )

















/*

	Response from M0

*/
//******************************************************************************
//
//******************************************************************************
void 	uartM0_rsp_m0SysInfo( UINT8* rx_payload )
{
	UINT8 *dataPtr = (UINT8*)&M0_System_info;

	memset( dataPtr, 0x00, sizeof(M0_System_info) );
	memcpy( dataPtr, rx_payload, sizeof(M0_System_info) );

	M0_Status_Update |= M0_DEV_UPDATE_SYSINFO;


	//printf("\n----uartM0_rsp_m0SysInfo ========== ========== \n");
	#if 	TST_M0_SYSINFO_PRINT

		//#if 	!TST_TxRx_COUNT
		printf( "\n  **********  " );
		printf("RxM0 SysInfo FW:[");
		int  i;
		for (int i = 0; i < 32; ++i)
		{
			printf("%c", M0_System_info.FW_ver[i] );
		}
		printf("] ");
		//#endif 	//#if 	!TST_TxRx_COUNT

		#if 	TST_TxRx_COUNT
			/*
			Tst_Rx_Cnt++;			
			printf("[%02d-%02d-%02d %02d:%02d:%02d] Rx[%08d]", 
						M0_System_info.mcu_RTC.Year,
						M0_System_info.mcu_RTC.Month,
						M0_System_info.mcu_RTC.Day,
						M0_System_info.mcu_RTC.Hour,
						M0_System_info.mcu_RTC.Minute,
						M0_System_info.mcu_RTC.Second,
						Tst_Rx_Cnt	);

			printf("  **Rx[%08d]", Tst_Rx_Cnt );
			*/

			printf(" [%02d-%02d-%02d %02d:%02d:%02d] ", 
						M0_System_info.mcu_RTC.Year,
						M0_System_info.mcu_RTC.Month,
						M0_System_info.mcu_RTC.Day,
						M0_System_info.mcu_RTC.Hour,
						M0_System_info.mcu_RTC.Minute,
						M0_System_info.mcu_RTC.Second	);
		#else
			printf("[%02d-%02d-%02d %02d:%02d:%02d] ", 
						M0_System_info.mcu_RTC.Year,
						M0_System_info.mcu_RTC.Month,
						M0_System_info.mcu_RTC.Day,
						M0_System_info.mcu_RTC.Hour,
						M0_System_info.mcu_RTC.Minute,
						M0_System_info.mcu_RTC.Second	);
		#endif 		//#if 	TST_TxRx_COUNT


		printf( "  **********  " );
		printf(" \n");
		printf(" \n");
	#endif 	//#if 	TST_M0_SYSINFO_PRINT
} 		//void 	uartM0_rsp_m0SysInfo( UINT8* rx_payload )


//******************************************************************************
//
//******************************************************************************
//	printf("Rx uartCmd_RSP_M0_RTC ---------- ---------- \n");
void 	uartM0_rsp_M0RTC( UINT8* rx_payload )
{
	UINT8	*dataPtr;
	int  	i;

	dataPtr = (UINT8*)&M0_rtc;
	for( i=0 ; i<sizeof(M0_RTC_TIME_FRAME) ; i++ )		{
		dataPtr[i] = rx_payload[i];
	} 		//for( i=0 ; i<sizeof(M0_RTC_TIME_FRAME) ; i++ )		{

	M0_Status_Update |= M0_DEV_UPDATE_RTC;

	//	TEST 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	#if 	TST_M0_RTC_PRINT
		printf("RxM0 RTC : [%02d-%02d-%02d %02d:%02d:%02d] \n",
					M0_rtc.Year,
					M0_rtc.Month,
					M0_rtc.Day,
					M0_rtc.Hour,
					M0_rtc.Minute,
					M0_rtc.Second );
	#endif 	//#if 	TST_M0_RTC_PRINT
} 	//void 	uartM0_rsp_M0RTC( UINT8* rx_payload )




//******************************************************************************
//
//******************************************************************************
void 	uartM0_rsp_DevStatus( UINT8* rx_payload )
{
	UINT8*	dataPtr;
	int  	i;


	switch( rx_payload[0] )		{

		//==========================================================================
		case SUBTYPE_MGC3130:
			M0_Status_Update |= M0_DEV_UPDATE_MGC;

			dataPtr = (UINT8*)&M0_mgc3130;
			for( i=0 ; i<sizeof(M0_MGC_DATA_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			// 	TEST		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			#if 	TST_M0_MGC_DATA_PRINT
				printf("RxM0 MGC : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X] mgc[%02d]\n",
							M0_mgc3130.mcu_RTC.Year,
							M0_mgc3130.mcu_RTC.Month,
							M0_mgc3130.mcu_RTC.Day,
							M0_mgc3130.mcu_RTC.Hour,
							M0_mgc3130.mcu_RTC.Minute,
							M0_mgc3130.mcu_RTC.Second,
							M0_Status_Update,
							M0_mgc3130.mgc	);
				
			#endif 	//#if 	TST_M0_MGC_DATA_PRINT
			break;


		//==========================================================================
		case SUBTYPE_G_SENSOR:
			M0_Status_Update |= M0_DEV_UPDATE_G_SENSOR;

			dataPtr = (UINT8*)&g_icm_Data;
			for( i=0 ; i<sizeof(M0_G_SENSOR_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			// 	TEST		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			#if 	TST_M0_G_DATA_PRINT
				printf("RxM0 G-sensor : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X]    G : XYZ[%04d %04d %04d]\n",
							g_icm_Data.mcu_RTC.Year,
							g_icm_Data.mcu_RTC.Month,
							g_icm_Data.mcu_RTC.Day,
							g_icm_Data.mcu_RTC.Hour,
							g_icm_Data.mcu_RTC.Minute,
							g_icm_Data.mcu_RTC.Second,
							M0_Status_Update,
							g_icm_Data.icm20602.acc_X,
							g_icm_Data.icm20602.acc_Y,
							g_icm_Data.icm20602.acc_Z		);
			#endif 	//#if 	TST_M0_G_DATA_PRINT
			break;


		//==========================================================================
		case SUBTYPE_ADC:
			M0_Status_Update |= M0_DEV_UPDATE_ADC_1;

			dataPtr = (UINT8*)&M0_adc;
			for( i=0 ; i<sizeof(M0_ADC_DATA_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			// 	TEST		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			#if 	TST_M0_ADC_DATA_PRINT
				/*
				printf("RxM0 ADC : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X] pwEn[%02X] ADC %c%04d%c%04d%c%04d\n",
							M0_adc.mcu_RTC.Year,
							M0_adc.mcu_RTC.Month,
							M0_adc.mcu_RTC.Day,
							M0_adc.mcu_RTC.Hour,
							M0_adc.mcu_RTC.Minute,
							M0_adc.mcu_RTC.Second,
							M0_Status_Update,
							M0_adc.pw_EN,
							0x09,
							M0_adc.adc0,
							0x09,
							M0_adc.adc1,
							0x09,
							M0_adc.adc2
							);
							*/
				if( M0_adc.pw_EN != Tst_PrePW_state )		{
					Tst_PW_ChangeCnt = 0;
				}
				/*
				printf("RxM0 ADC : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X]  ADC %c%04d%c%04d%c%04d - pwEn[%02X] -%02d\n",
							M0_adc.mcu_RTC.Year,
							M0_adc.mcu_RTC.Month,
							M0_adc.mcu_RTC.Day,
							M0_adc.mcu_RTC.Hour,
							M0_adc.mcu_RTC.Minute,
							M0_adc.mcu_RTC.Second,
							M0_Status_Update,							
							0x09,
							M0_adc.adc0,
							0x09,
							M0_adc.adc1,
							0x09,
							M0_adc.adc2,
							M0_adc.pw_EN,
							Tst_PW_ChangeCnt
							);
				*/
				//printf("RxM0 ADC : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X]  ADC %c%04d%c%04d%c%04d - pwEn[%02X] -%02d\n",
				printf("%02d-%02d-%02d %02d:%02d:%02d%c%04X%c%04d%c%04d%c%04d%c%02X%c%04d%c%03d%c%04d\n",
							M0_adc.mcu_RTC.Year,
							M0_adc.mcu_RTC.Month,
							M0_adc.mcu_RTC.Day,
							M0_adc.mcu_RTC.Hour,
							M0_adc.mcu_RTC.Minute,
							M0_adc.mcu_RTC.Second,
							0x09,
							M0_Status_Update,
							0x09,
							M0_adc.adc0,
							0x09,
							M0_adc.adc1,
							0x09,
							M0_adc.adc2,
							0x09,
							M0_adc.pw_EN,
							0x09,
							Bat_v,
							0x09,
							Bat_i,
							0x09,
							Tst_PW_ChangeCnt
							);
				Tst_PW_ChangeCnt ++;
				Tst_PrePW_state = M0_adc.pw_EN;
			#elif 	TST_M0_ADC_DATA_PRINT2
				
			#endif 	//#if 	TST_M0_ADC_DATA_PRINT
			break;


		//==========================================================================
		case SUBTYPE_RTC:
			break;


		//==========================================================================
		case SUBTYPE_TEMPERATURE:
			M0_Status_Update |= M0_DEV_UPDATE_TEMP;

			dataPtr = (UINT8*)&M0_temperature;
			for( i=0 ; i<sizeof(M0_TEMP_DATA_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}
			
			#if 	TST_M0_TEMP_PRINT
				printf("RxM0 TEMP : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X] temp.[%d] \n",
							M0_temperature.mcu_RTC.Year,
							M0_temperature.mcu_RTC.Month,
							M0_temperature.mcu_RTC.Day,
							M0_temperature.mcu_RTC.Hour,
							M0_temperature.mcu_RTC.Minute,
							M0_temperature.mcu_RTC.Second,
							M0_Status_Update,
							M0_temperature.temp0	);
			#endif 		//#if 	TST_M0_TEMP_PRINT
			break;


		//==========================================================================
		case SUBTYPE_PW_METER0:
			M0_Status_Update |= M0_DEV_UPDATE_PWMETER0;

			dataPtr = (UINT8*)&M0_PwMeter;
			for( i=0 ; i<sizeof(M0_PW_METER_DATA_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			#if 	TST_M0_PW_METER_PRINT
				/*
				printf("RxM0 PwMeter : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X] bat_v[%d] bat_i[%d] temp[%d]\n",
							M0_PwMeter.mcu_RTC.Year,
							M0_PwMeter.mcu_RTC.Month,
							M0_PwMeter.mcu_RTC.Day,
							M0_PwMeter.mcu_RTC.Hour,
							M0_PwMeter.mcu_RTC.Minute,
							M0_PwMeter.mcu_RTC.Second,
							M0_Status_Update,
							M0_PwMeter.bat_v,
							M0_PwMeter.bat_i,
							M0_PwMeter.temp 		);
							*/				
				/*
				printf("RxM0 PwMeter : [%02d-%02d-%02d %02d:%02d:%02d]  M0_Status_Update[%04X]   - bat_i[%3d]         bat_v[%4d]\n",
							M0_PwMeter.mcu_RTC.Year,
							M0_PwMeter.mcu_RTC.Month,
							M0_PwMeter.mcu_RTC.Day,
							M0_PwMeter.mcu_RTC.Hour,
							M0_PwMeter.mcu_RTC.Minute,
							M0_PwMeter.mcu_RTC.Second,
							M0_Status_Update,
							M0_PwMeter.bat_i,
							M0_PwMeter.bat_v		);
				*/
				Bat_v = M0_PwMeter.bat_v;
				Bat_i = M0_PwMeter.bat_i;


			#endif 		//#if 	TST_M0_PW_METER_PRINT

			break;


		//==========================================================================
		case SUBTYPE_RADAR_A:
			dataPtr = (UINT8*)&M0_radarA;
			for( i=0 ; i<sizeof(M0_RADAR_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			M0_Status_Update |= M0_DEV_UPDATE_RADAR_A;

			// 	TEST		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			#if 	TST_M0_RADAR_A_DATA_PRINT
				printf("RxM0 Radar-A [%02d-%02d-%02d %02d:%02d:%02d] FW%02X%02X%02X%02X radar[%d]  CC[%d] Parking[%d] LR[%d]   type[%d] xyz[%d-%d-%d] [%d,%d,%d,%d,%d] [%d_%d]\n",

							M0_radarA.mcu_RTC.Year,
							M0_radarA.mcu_RTC.Month,
							M0_radarA.mcu_RTC.Day,
							M0_radarA.mcu_RTC.Hour,
							M0_radarA.mcu_RTC.Minute,
							M0_radarA.mcu_RTC.Second,

							M0_radarA.FW_version[0],
							M0_radarA.FW_version[1],
							M0_radarA.FW_version[2],
							M0_radarA.FW_version[3],

							M0_radarA.data.CarChange_status,
							M0_radarA.data.car_change,
							M0_radarA.data.parking_status,
							M0_radarA.data.L_R,
							M0_radarA.data.obj_type,

							M0_radarA.data.obj_position_X,
							M0_radarA.data.obj_position_Y,
							M0_radarA.data.obj_position_Z,

							M0_radarA.data.radar_X,
							M0_radarA.data.radar_Y,
							M0_radarA.data.radar_L,
							M0_radarA.data.radar_W,
							M0_radarA.data.radar_H,

							M0_radarA.data.Theta,
							M0_radarA.data.Phi			);

			#endif 		//#if 	TST_M0_RADAR_A_DATA_PRINT
			break;


		//==========================================================================
		case SUBTYPE_RADAR_B:
			dataPtr = (UINT8*)&M0_radarB;
			for( i=0 ; i<sizeof(M0_RADAR_FRAME) ; i++ )		{
				dataPtr[i] = rx_payload[i+1];
			}

			M0_Status_Update |= M0_DEV_UPDATE_RADAR_B;

			// 	TEST		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			#if 	TST_M0_RADAR_B_DATA_PRINT
				printf("RxM0 Radar-B [%02d-%02d-%02d %02d:%02d:%02d] FW%02X%02X%02X%02X radar[%d]  CC[%d] Parking[%d] LR[%d]   type[%d] xyz[%d-%d-%d] [%d,%d,%d,%d,%d] [%d_%d]\n",

							M0_radarB.mcu_RTC.Year,
							M0_radarB.mcu_RTC.Month,
							M0_radarB.mcu_RTC.Day,
							M0_radarB.mcu_RTC.Hour,
							M0_radarB.mcu_RTC.Minute,
							M0_radarB.mcu_RTC.Second,

							M0_radarB.FW_version[0],
							M0_radarB.FW_version[1],
							M0_radarB.FW_version[2],
							M0_radarB.FW_version[3],

							M0_radarB.data.CarChange_status,
							M0_radarB.data.car_change,
							M0_radarB.data.parking_status,
							M0_radarB.data.L_R,
							M0_radarB.data.obj_type,

							M0_radarB.data.obj_position_X,
							M0_radarB.data.obj_position_Y,
							M0_radarB.data.obj_position_Z,

							M0_radarB.data.radar_X,
							M0_radarB.data.radar_Y,
							M0_radarB.data.radar_L,
							M0_radarB.data.radar_W,
							M0_radarB.data.radar_H,

							M0_radarB.data.Theta,
							M0_radarB.data.Phi			);
				printf("\n");
			#endif 		//#if 	TST_M0_RADAR_B_DATA_PRINT
			break;


		//==========================================================================
		//==========================================================================
		default:
			printf( "RxM0   device SUBTYPE_??? \n" );
			break;


	} 	//switch( rx_payload[0] )		{
} 	//void 	uartM0_rsp_DevStatus( UINT8* rx_payload )




//******************************************************************************
//
//******************************************************************************
/*
	typedef struct  {
	 UINT8         FW_version[2];
	 UINT16         status;
	 UINT8         name[64];
	 UINT32         dataLength;
	 UINT8         data[1600];
	}M0_TLSAM_FRAME;
*/
void 	uartM0_rsp_ENC( UINT8* rx_payload )
{
	uint8_t*	dataPtr;
	int  	i;

	printf("\n");
	printf("RxM0     uartM0_rsp_ENC ---------- ---------- \n" );

	memset( (uint8_t*)&(M0_TLSAM_DATA.tlsam_Data), 0x00, sizeof(M0_TLSAM_FRAME) );

	dataPtr = (uint8_t*)&(M0_TLSAM_DATA.tlsam_Data);
	for( i=0 ; i<A53M0_RxQ.LENGTH ; i++ )		{
		*dataPtr = *rx_payload;
		dataPtr++;
		rx_payload++;
	}


	// 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	#if 	TST_M0_TLSAM_01
		/*
		printf("    [" );
		for( int i=0 ; i<M0_TLSAM_DATA.tlsam_Data.dataLength ; i++ )		{
			printf(" %02X", M0_TLSAM_DATA.tlsam_Data.data[i] );
		}
		printf("]\n");
		*/
		printf("    [%02X]-[%02X][%02X] \n", M0_TLSAM_DATA.tlsam_Data.status, M0_TLSAM_DATA.tlsam_Data.FW_version[0], M0_TLSAM_DATA.tlsam_Data.FW_version[1] );
		printf("    [%04d] [%s] \n", M0_TLSAM_DATA.tlsam_Data.dataLength, M0_TLSAM_DATA.tlsam_Data.name );
		#if 	TST_TxRx_COUNT
			Tst_Rx_Cnt ++;

			a53m0API_currentTime = time( NULL);
			a53m0API_current_tm = localtime (&a53m0API_currentTime);
			printf("    __________ __________ __________ ---------- %02d/%02d/%02d %02d:%02d:%02d  Tst_Rx_Cnt[%04d]\n",
						((a53m0API_current_tm->tm_year+1900)-2000),
						a53m0API_current_tm->tm_mon + 1,
						a53m0API_current_tm->tm_mday,
						a53m0API_current_tm->tm_hour,
						a53m0API_current_tm->tm_min,
						a53m0API_current_tm->tm_sec,
						Tst_Rx_Cnt );
		#endif 	//#if 	TST_TxRx_COUNT
	#endif 		//#if 	TST_M0_TLSAM_01

	if( (strlen(M0_TLSAM_DATA.tlsam_Data.name)>0) && (M0_TLSAM_DATA.tlsam_Data.dataLength>0) )		{
		M0_Status_Update |= M0_DEV_UPDATE_TLSAM;
	} 		//if( (strlen(M0_TLSAM_DATA.tlsam_Data.name)>0) && (M0_TLSAM_DATA.tlsam_Data.dataLength>0) )		{

	// 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	#if TST_M0_TLSAM_SAVE_FILE
		FILE *f_tlsam;
		unsigned char f_name[128];
		
		//	save binary file 		----------------------------------------------------
		bzero( f_name, 64 );
		sprintf( f_name, "%s.tlsam", M0_TLSAM_DATA.tlsam_Data.name );

		if( (M0_Status_Update & M0_DEV_UPDATE_TLSAM) > 0 )		{
			f_tlsam = fopen( f_name, "w+b" );
			if( f_tlsam )		{
				fwrite( M0_TLSAM_DATA.tlsam_Data.data, 1, M0_TLSAM_DATA.tlsam_Data.dataLength, f_tlsam );
				fclose( f_tlsam );
			} 		//if( f_tlsam )		{
		} 		//if( strlen(M0_TLSAM_DATA.tlsam_Data.name) > 0 )		{

		//	save text file 		------------------------------------------------------
		unsigned char put_Data[16];
		bzero( f_name, 64 );
		sprintf( f_name, "%s.txt", M0_TLSAM_DATA.tlsam_Data.name );

		if( (M0_Status_Update & M0_DEV_UPDATE_TLSAM) > 0 )		{
			f_tlsam = fopen( f_name, "w+b" );
			if( f_tlsam )		{
				for( i=0 ; i<M0_TLSAM_DATA.tlsam_Data.dataLength ; i++ )		{
					bzero( put_Data, 16 );
					sprintf( put_Data, "%02X", M0_TLSAM_DATA.tlsam_Data.data[i] );
					fwrite( put_Data, 1, strlen(put_Data), f_tlsam );

					if( i < (M0_TLSAM_DATA.tlsam_Data.dataLength-1) )	{
						putc( 0x20, f_tlsam );
					}
				} 		//for( i=0 ; i<M0_TLSAM_DATA.tlsam_Data.dataLength ; i++ )		{
				
				fclose( f_tlsam );
			} 		//if( f_tlsam )		{
		} 		//if( strlen(M0_TLSAM_DATA.tlsam_Data.name) > 0 )		{
	#endif 		//#if TST_M0_TLSAM_SAVE_FILE

	printf("\n");
} 		//void 	uartM0_rsp_ENC( UINT8* rx_payload )












//******************************************************************************
//
//******************************************************************************
//case uart_RSP_ALARM:
void 	uartM0_rsp_M0_Alarm( UINT8* rx_payload )
{
	M0_ALARM_A53_FRAME 	m0_Alarm;

	memcpy( (uint8_t*)&m0_Alarm, rx_payload, sizeof(M0_ALARM_A53_FRAME) );

	/*
	uint8_t*	dataPtr = rx_payload;
	printf("\n [");
	for (int i = 0; i < (A53M0_RxQ.LENGTH-3) ; ++i)
	{
		printf(" %02X", *dataPtr );
		dataPtr++;
	}
	printf("] \n");
	*/


	//	Alarm Sub-Type;			M0 alarm A53
	switch( m0_Alarm.alarm )		{

		case uart_ALARM_TEMP:
			break;


		#if  	Tst_Radar_PW_OFFON
			case uart_ALARM_RADAR_PWON:
				memcpy( (uint8_t*)&m0_Alarm, rx_payload, sizeof(m0_Alarm) );
				printf("RxM0     uartM0_rsp_M0_Alarm : uart_ALARM_RADAR_PWON ---------- ---------- [%02d-%02d-%02d %02d:%02d:%02d] \n",
							m0_Alarm.mcu_RTC.Year,
							m0_Alarm.mcu_RTC.Month,
							m0_Alarm.mcu_RTC.Day,
							m0_Alarm.mcu_RTC.Hour,
							m0_Alarm.mcu_RTC.Minute,
							m0_Alarm.mcu_RTC.Second );

				radar_param_set( );
				break;

			case uart_ALARM_RADAR_PWOFF:
				memcpy( (uint8_t*)&m0_Alarm, rx_payload, sizeof(m0_Alarm) );
				printf("RxM0     uartM0_rsp_M0_Alarm : uart_ALARM_RADAR_PWOFF ---------- ---------- [%02d-%02d-%02d %02d:%02d:%02d] \n",
							m0_Alarm.mcu_RTC.Year,
							m0_Alarm.mcu_RTC.Month,
							m0_Alarm.mcu_RTC.Day,
							m0_Alarm.mcu_RTC.Hour,
							m0_Alarm.mcu_RTC.Minute,
							m0_Alarm.mcu_RTC.Second );

				radar_param_down( );
				break;
		#endif 		//#if  	Tst_Radar_PW_OFFON

	} 		//switch( m0_Alarm.alarm )		{

} 		//void 	uartM0_rsp_M0_Alarm( UINT8* rx_payload )












//******************************************************************************
//
//******************************************************************************
void 	uartM0_rsp_read_EEP( UINT8* rx_payload )
{
	printf("RxM0     uartM0_rsp_read_EEP ---------- ---------- \n" );

	uint16_t  	Addr;
	uint16_t  	Length;


	Addr = *rx_payload;
	Addr |= *(rx_payload+1) << 8;

	Length = *(rx_payload+2);
	Length |= *(rx_payload+3) << 8;

	printf( "  A[%04X] L[%04d] : [", Addr, Length );
	for( int i=0 ; i < Length ; i++ )		{
		if( (i%16)==0 )		{
			printf( "\n" );
		}
		printf( " %02X", *(rx_payload+4+i) );
	}
	printf( "]\n" );

} 		//void 	uartM0_rsp_read_EEP( UINT8* rx_payload )


//******************************************************************************
//
//******************************************************************************
void 	uartM0_rsp_DummingTest( UINT8* rx_payload )
{
	uint32_t 	Rx_M0 = 0;

		#if 	TST_TxRx_COUNT
			ftime( &time_Rx );

			Rx_M0 = *rx_payload;
			Rx_M0 |= *(rx_payload+1) << 8;
			Rx_M0 |= *(rx_payload+2) << 16;
			Rx_M0 |= *(rx_payload+3) << 24;

			Tst_Rx_Cnt++;
			//printf("  ****Rx[%04d][%08d]\n", A53M0_RxQ.LENGTH, Tst_Rx_Cnt );
			//printf("  - Rx[%04d][%05d][%05d]\n", A53M0_RxQ.LENGTH, Tst_Rx_Cnt, Rx_M0 );
			printf("  - Rx[%05d][%05d] [%05d]  x%04d-%ldms\n", 
						Tst_Tx_Cnt, 
						Tst_Rx_Cnt, 
						Rx_M0, 
						crc_ErrorCnt,
						((time_Rx.time*1000)+time_Rx.millitm)-((time_Tx.time*1000)+time_Tx.millitm) );
		#else

		#endif 		//#if 	TST_TxRx_COUNT

} 		//void 	uartM0_rsp_DummingTest( UINT8* rx_payload )

