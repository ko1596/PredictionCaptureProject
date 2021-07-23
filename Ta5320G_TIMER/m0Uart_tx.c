

#include "m0All_Header.h"


//********************************************************************
//	Global
unsigned char   A53M0_TxBuf[UARTA53M0_TX_BUFFER_SIZE];
int 	A53M0_TxLength;
int 	A53M0_Tx;




//********************************************************************
//
//********************************************************************
void  uartA53M0_Tx_thread( int* fdPtr )
{
	int i;
  a53m0API_currentTime = time( NULL);
  a53m0API_current_tm = localtime (&a53m0API_currentTime);

  printf( "\n********** **********  Thread uartA53M0_Tx_thread Start  ********** ********** \n\n" );

  int res;

  
  sleep( 0.5 );
  int tst_a53TxCnt = 1;


  //******************************************************************
  while( 1 )  {

  	if( A53M0_TxLength > 0 )		{
			a53m0API_currentTime = time( NULL);
			a53m0API_current_tm = localtime (&a53m0API_currentTime);


	    res = write( fd_uartA53M0, A53M0_TxBuf, A53M0_TxLength );
	    if( res == -1 ) {
	      printf( "uartA53M0_Tx_thread XXXXXXXXXX res = [%d] \n", res );
	    }
	    else  {
	      //printf( "        ---------------------- res = [%d] \n", res );
	      //printf( "                                 res = [%d] \n", res );
	      A53M0_Tx += res;
	      //printf("  Tx[%d][%d]\n", res, A53M0_Tx );
	      if( A53M0_Tx >= A53M0_TxLength )	{
	      	usleep( 10000 );  	//10ms
	      	A53M0_TxLength = 0;
				}
	    } 

	    //usleep( 50000 );  	//50ms
	    //usleep( 25000 );  	//25ms
	    //usleep( 10000 );  	//10ms
	    //usleep( 5000 );  	//5ms
  	} 	//if( A53M0_TxLength > 0 )		{
  	else 	{
  		//usleep( 100000 );  	//100ms
  		//usleep( 50000 );  	//50ms
  		//usleep( 40000 );  	//40ms
  		//usleep( 30000 );  	//30ms
  		//usleep( 25000 );  	//25ms
  		//usleep( 20000 );  	//20ms
  		usleep( 10000 );  	//10ms
  	}
  		
  }   //while( 1 )  {

  printf( " ___________  Thread uartA53M0_Tx_thread End  ___________ \n\n" );  	
} 	//void  uartA53M0_Tx_thread( int* fdPtr )










//  ****************************************************************************
//  CRC16_CCITT_KERMIT
//  ZigBee use this
//  ****************************************************************************
UINT16  Get_Kermit_CRC16( UINT8* dataPtr, UINT32 length )
{
  UINT8   i;  
  UINT16  crc16 = 0x0000;
  

  while( length ) {
    crc16 ^= *dataPtr;
    

    for(i=0 ; i<8 ; i++)  {
      if(crc16 & 0x01)  { /* LSB(b0)=1 */
        //crc16 = (crc16>>1) ^ 0xA001;  //  CRC-16 (Modbus)
        crc16 = (crc16>>1) ^ 0x8408;  //  CRC-CCITT (Kermit)
      }
      else  {
        crc16 = crc16 >>1;
      }
    } //  for(i=0 ; i<8 ; i++)  {
    dataPtr++;
    length--;
  } //while( length )


  return  crc16;
}    //UINT16  Get_Kermit_CRC16( UINT8* dataPtr, UINT32 length )


//  ****************************************************************************
//  CRC16 Modbus
//  ****************************************************************************
UINT16  Get_ModBus_CRC16( UINT8* dataPtr, UINT32 length )
{
  UINT8 i;
  UINT16  crc16 = 0xFFFF;   //  CRC-16 (Modbus)

  while( length ) {
    crc16 ^= *dataPtr;
    for(i=0 ; i<8 ; i++)  {
      if(crc16 & 0x01)  { /* LSB(b0)=1 */
        crc16 = (crc16>>1) ^ 0xA001;  //  CRC-16 (Modbus)
      }
      else  {
        crc16 = crc16 >>1;
      }
    } //  for(i=0 ; i<8 ; i++)  {
    dataPtr++;
    length--;
  } //while( length )

  return  crc16;
}     //UINT16  Get_ModBus_CRC16( UINT8* dataPtr, UINT32 length )




















//********************************************************************
//
//********************************************************************
void 	uartA53M0_Req_Dumming_Test( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	int  	i;


	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}	

	#if 	TST_TxRx_COUNT
		ftime( &time_Tx );
		Tst_Tx_Cnt++;
		//printf(" ========== uartA53M0_Req_Dumming_Test  [%05d] \n", Tst_Tx_Cnt);

		//printf(" ========== uartA53M0_REQM0_SystemInfo ---------- ---------- Tx[%08d]\n", Tst_Tx_Cnt);
		//printf("  ****Tx[%08d]\n", Tst_Tx_Cnt);
	#else
		printf(" ========== uartA53M0_Req_Dumming_Test ---------- ---------- \n");
	#endif 		//#if 	TST_TxRx_COUNT


  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uartCmd_REQ_M0_DUMMING;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

  //..................................................................
  *txPtr = 0x00; 						txPtr++; 		// Test sub-function

  //..................................................................
  //	Dumming Test Data
  for( i=0 ; i<1024 ; i++ )		{
  	*txPtr = i + 0x11;
  	txPtr++;
  }


	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );

} 		//void 	uartA53M0_Req_Dumming_Test( void )





















//********************************************************************
//
//********************************************************************
void 	uartA53M0_SetM0_currentRTC( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	printf(" ========== uartA53M0_SetM0_currentRTC ---------- ---------- \n");

  a53m0API_currentTime = time( NULL);
  a53m0API_current_tm = localtime (&a53m0API_currentTime);  


  *txPtr = 0xCC; 				txPtr++;

  //	UARTcmd 	Len
  *txPtr = 0x00; 				txPtr++;
  *txPtr = 0x00; 				txPtr++;

  //	UARTcmd 	Cmd
  *txPtr = uartCmd_SET_M0_RTC;
  txPtr++;

  //	UARTcmd 	CRC
  *txPtr = 0x00; 				txPtr++;
  *txPtr = 0x00; 				txPtr++;

  //	RTC data
  *txPtr = ((a53m0API_current_tm->tm_year+1900)-2000);
  txPtr++;
  *txPtr = a53m0API_current_tm->tm_mon + 1;
  txPtr++;
  *txPtr = a53m0API_current_tm->tm_mday;
  txPtr++;
  *txPtr = a53m0API_current_tm->tm_hour;
  txPtr++;
  *txPtr = a53m0API_current_tm->tm_min;
  txPtr++;
  *txPtr = a53m0API_current_tm->tm_sec;
  txPtr++;


  //==================================================================
  int tx_length = txPtr - A53M0_TxBuf;
  A53M0_TxBuf[1] = (tx_length-3) & 0x00FF;
  A53M0_TxBuf[2] = ((tx_length-3) >>8 ) & 0x00FF;

  UINT16 	crc16 = Get_ModBus_CRC16( A53M0_TxBuf+6, 6 );
  A53M0_TxBuf[4] = crc16 & 0x00FF;
  A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	#if 	TST_DEBUF_PRINT_001
		int i;
		printf("TX uartA53M0_SetM0_currentRTC : [");
		for( i=0 ; i<tx_length ; i++ )		{
			printf(" %02X", A53M0_TxBuf[i]);
		}
		printf("]\n");
		
	#endif 	//#if 	TST_DEBUF_PRINT_001

	//==================================================================
  A53M0_Tx = 0;
  A53M0_TxLength = tx_length;
} 	//void 	uartA53M0_SetM0_currentRTC( void )


//********************************************************************
//
//********************************************************************
void 	uartA53M0_REQM0_SystemInfo( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}

	#if 	TST_TxRx_COUNT
		//Tst_Tx_Cnt++;
		//printf(" ========== uartA53M0_REQM0_SystemInfo ---------- ---------- Tx[%08d]\n", Tst_Tx_Cnt);
		//printf("  **Tx[%08d]\n", Tst_Tx_Cnt);
	#else
		printf(" ========== uartA53M0_REQM0_SystemInfo ---------- ---------- \n");
	#endif 		//#if 	TST_TxRx_COUNT

  *txPtr = 0xCC; 				txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x01; 				txPtr++;
  *txPtr = 0x00; 				txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uartCmd_REQ_M0_INFO;

	//==================================================================
  A53M0_Tx = 0;
  A53M0_TxLength = 4;
} 		//void 	uartA53M0_REQM0_SystemInfo( void )


//********************************************************************
//
//********************************************************************
//	Req. M0 temperature
void 	uartA53M0_REQM0_Temp( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	printf(" ========== uartA53M0_REQM0_Temp ---------- ---------- \n");

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_STATUS;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

	*txPtr = SUBTYPE_TEMPERATURE; 	txPtr++;


	//------------------------------------------------------------------
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], 1 );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_REQM0_Temp( void )


//********************************************************************
//
//********************************************************************
//	Req. M0 Power Meter 		LTC2944
void 	uartA53M0_REQM0_PwMeter( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	//printf(" ========== uartA53M0_REQM0_PwMeter ---------- ---------- \n");

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_STATUS;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

	*txPtr = SUBTYPE_PW_METER0; 	txPtr++;


	//------------------------------------------------------------------
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], 1 );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
}


//********************************************************************
//
//********************************************************************
void 	uartA53M0_REQM0_ADC( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	//printf(" ========== uartA53M0_REQM0_ADC ---------- ---------- \n");

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_STATUS;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

	*txPtr = SUBTYPE_ADC; 		txPtr++;


	//------------------------------------------------------------------
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], 1 );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_REQM0_ADC( void )


//********************************************************************
//
//********************************************************************
void 	uartA53M0_REQM0_RTC( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	printf(" ========== uartA53M0_REQM0_SystemInfo ---------- ---------- \n");

  *txPtr = 0xCC; 				txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x01; 				txPtr++;
  *txPtr = 0x00; 				txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uartCmd_REQ_M0_RTC;

	//==================================================================
  A53M0_Tx = 0;
  A53M0_TxLength = 4;
} 	//void 	uartA53M0_REQM0_RTC( void )





//********************************************************************
//
//********************************************************************
//	CMD 	uart_REQ_DEV_STATUS
//		SUBTYPE_RADAR_A						0x21
//		SUBTYPE_RADAR_B						0x22
void 	uartA53M0_Req_Radar_Status( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}
	printf(" ========== uartA53M0_REQM0_Radar Status ---------- ---------- \n");

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_STATUS;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

	*txPtr = SUBTYPE_RADAR_A; 		txPtr++;

	//------------------------------------------------------------------
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], 1 );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_Req_Radar_Status( void )





//********************************************************************
//
//********************************************************************
//	CMD 	21
//		SUBTYPE_RADAR_A						0x21
//		SUBTYPE_RADAR_B						0x22
void 	uartA53M0_Get_Radar_FWver( void )
{
	unsigned char 	*txPtr = A53M0_TxBuf;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}	
	printf(" ========== uartA53M0_Get_Radar_FWver ==========  ========== \n");

  *txPtr = 0xCC; 							txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_FW_VER;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;

 	*txPtr = SUBTYPE_RADAR_A; 	txPtr++;

	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_Get_Radar_FWver( void )


//********************************************************************
//
//********************************************************************
//	UINT8 A_B : 	=0, RadarA
//								=1, RadarB
//	UINT8 L_R :		=0, Right side
//								=1, Left side
void 	uartA53M0_Set_Radar_Param( UINT8 A_B, UINT8 RW, M0_RADAR_SET_FRAME* radar_set_Ptr )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	UINT8* 	dataPtr = (UINT8*)radar_set_Ptr;
	int  i;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}

	if( A_B ==0 )	{
		printf(" ========== uartA53M0_Set_Radar_Param ========== A ========== \n");
	}
	else 	{
		printf(" ========== uartA53M0_Set_Radar_Param ========== B ========== \n");
	}
	

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_SET_DEV;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

  if( A_B ==0 )	{
  	*txPtr = SUBTYPE_RADAR_A; 		txPtr++;
  }
  else 	{
  	*txPtr = SUBTYPE_RADAR_B; 		txPtr++;
  }

	*txPtr = ((radar_set_Ptr->radar_X) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radar_set_Ptr->radar_X) & 0x00FF; 						txPtr++;
	*txPtr = ((radar_set_Ptr->radar_Y) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radar_set_Ptr->radar_Y) & 0x00FF; 						txPtr++;
	*txPtr = ((radar_set_Ptr->radar_L) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radar_set_Ptr->radar_L) & 0x00FF; 						txPtr++;
	*txPtr = ((radar_set_Ptr->radar_W) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radar_set_Ptr->radar_W) & 0x00FF; 						txPtr++;
	*txPtr = ((radar_set_Ptr->radar_H) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radar_set_Ptr->radar_H) & 0x00FF; 						txPtr++;

	*txPtr = radar_set_Ptr->Theta; 						txPtr++;
	*txPtr = radar_set_Ptr->Phi; 							txPtr++;
	*txPtr = radar_set_Ptr->L_R; 							txPtr++;
	
	for( i=0 ; i<9 ; i++ )		{
		*txPtr = 0x00; 														txPtr++;
	}
	

	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_Set_Radar_Param( UINT8 L_R, UINT8 RW, M0_RADAR_SET_FRAME* dataPtr )


//********************************************************************
//
//********************************************************************
//	BatA_B =0,	Bat_A
//	BatA_B =1,	Bat_B
void 	uartA53M0_Set_Bat_Source( UINT8 BatA_B )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	int  i;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}

	if( BatA_B == 0 )	{
		printf(" ========== uartA53M0_Set_Bat_Source ---------- A \n");
	}
	else if( BatA_B == 1 )	{
		printf(" ========== uartA53M0_Set_Bat_Source ---------- B \n");
	}
	else 	{
		return;
	}
	

	
  *txPtr = 0xCC; 							txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_SET_DEV;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;

  *txPtr = SUBTYPE_PW_BAT; 		txPtr++;
  
	*txPtr = 0x00; 							txPtr++;		//	Reserve
	*txPtr = BatA_B; 						txPtr++;		//	select Bat_A / Bat_B


	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_Set_Bat_Source( UINT8 BatA_B )


//********************************************************************
//
//********************************************************************
void 	uartA53M0_Set_Radar_ReStart( M0_RADAR_SET_FRAME* radarA_set_Ptr, M0_RADAR_SET_FRAME* radarB_set_Ptr )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}


	M0_RADAR_SET_FRAME 	param_null;
	UINT8* 	dataPtr;
	int  i;

	printf(" ========== uartA53M0_Set_Radar_ReStart ---------- ---------- \n");
	if( (radarA_set_Ptr == 0) || (radarB_set_Ptr == 0) )		{		
		memset( (UINT8*)&param_null, 0x00, sizeof(M0_RADAR_SET_FRAME) );
		param_null.L_R = 0xFF;
		printf(" .......... uartA53M0_Set_Radar_ReStart  param=0 \n");
	}

	//==================================================================
  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_DEV_RESTART;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

	*txPtr = SUBTYPE_RADAR_A; 		txPtr++; 			//	*txPtr = SUBTYPE_RADAR_B; 		txPtr++;

	//	Radar A
	if( radarA_set_Ptr == 0 )	{
		radarA_set_Ptr = (M0_RADAR_SET_FRAME*)&param_null;
	}
	*txPtr = ((radarA_set_Ptr->radar_X) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarA_set_Ptr->radar_X) & 0x00FF; 						txPtr++;
	*txPtr = ((radarA_set_Ptr->radar_Y) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarA_set_Ptr->radar_Y) & 0x00FF; 						txPtr++;
	*txPtr = ((radarA_set_Ptr->radar_L) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarA_set_Ptr->radar_L) & 0x00FF; 						txPtr++;
	*txPtr = ((radarA_set_Ptr->radar_W) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarA_set_Ptr->radar_W) & 0x00FF; 						txPtr++;
	*txPtr = ((radarA_set_Ptr->radar_H) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarA_set_Ptr->radar_H) & 0x00FF; 						txPtr++;

	*txPtr = radarA_set_Ptr->Theta; 						txPtr++;
	*txPtr = radarA_set_Ptr->Phi; 							txPtr++;
	*txPtr = radarA_set_Ptr->L_R; 							txPtr++;

	for( i=0 ; i<9 ; i++ )		{
		*txPtr = 0x00; 														txPtr++;
	}

	//	Radar B
	if( radarB_set_Ptr == 0 )	{
		radarB_set_Ptr = (M0_RADAR_SET_FRAME*)&param_null;
	}
	*txPtr = ((radarB_set_Ptr->radar_X) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarB_set_Ptr->radar_X) & 0x00FF; 						txPtr++;
	*txPtr = ((radarB_set_Ptr->radar_Y) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarB_set_Ptr->radar_Y) & 0x00FF; 						txPtr++;
	*txPtr = ((radarB_set_Ptr->radar_L) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarB_set_Ptr->radar_L) & 0x00FF; 						txPtr++;
	*txPtr = ((radarB_set_Ptr->radar_W) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarB_set_Ptr->radar_W) & 0x00FF; 						txPtr++;
	*txPtr = ((radarB_set_Ptr->radar_H) >> 8) & 0x00FF; 			txPtr++;
	*txPtr = (radarB_set_Ptr->radar_H) & 0x00FF; 						txPtr++;

	*txPtr = radarB_set_Ptr->Theta; 						txPtr++;
	*txPtr = radarB_set_Ptr->Phi; 							txPtr++;
	*txPtr = radarB_set_Ptr->L_R; 							txPtr++;

	for( i=0 ; i<9 ; i++ )		{
		*txPtr = 0x00; 														txPtr++;
	}

	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_Set_Radar_ReStart( M0_RADAR_SET_FRAME* radarA_set_Ptr, M0_RADAR_SET_FRAME* radarB_set_Ptr )


//********************************************************************
//
//********************************************************************
void 	uartA53M0_TLSAM_Encrypt( uint8_t *enc_file_name, uint8_t *enc_data, uint16_t enc_dataLength )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	int  i;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}


	printf(" ========== uartA53M0_TLSAM_Encrypt ========== [%s] \n", enc_file_name);

	if( enc_dataLength > 1000 )		{
		printf("    !!!!!  uartA53M0_TLSAM_Encrypt Data too much [%d]  !!!!! \n", enc_dataLength);
		return;
	}
	

  *txPtr = 0xCC; 						txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_ENC;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 						txPtr++;
  *txPtr = 0x00; 						txPtr++;

  //..................................................................
  //	reserve
  *txPtr = 0x00; 						txPtr++;

  //..................................................................
  //	enc_file_name
  unsigned char  fname[64];
  bzero( fname, 64 );
  sprintf( fname, "%s", enc_file_name );
  for( i=0 ; i<64 ; i++ )		{
  	*txPtr = fname[i]; 						txPtr++;
  }

  //..................................................................
  //	enc_dataLength
  *txPtr = (enc_dataLength>>8) & 0x00FF; 			txPtr++;
  *txPtr = (enc_dataLength) & 0x00FF; 			txPtr++;

  //..................................................................
  //	enc_data
  for( i=0 ; i<enc_dataLength ; i++ )		{
  	*txPtr = enc_data[i]; 						txPtr++;
  }


	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );

	#if 	TST_TxRx_COUNT
		//Tst_Tx_Cnt ++;

		a53m0API_currentTime = time( NULL);
		a53m0API_current_tm = localtime (&a53m0API_currentTime);			
		printf("    __________ __________ __________ __________ %02d/%02d/%02d %02d:%02d:%02d  Tst_Tx_Cnt[%04d]\n", 
					((a53m0API_current_tm->tm_year+1900)-2000),
					a53m0API_current_tm->tm_mon + 1,
					a53m0API_current_tm->tm_mday,
					a53m0API_current_tm->tm_hour,
					a53m0API_current_tm->tm_min,
					a53m0API_current_tm->tm_sec,
					Tst_Tx_Cnt );
		printf("\n");
	#endif 	//#if 	TST_TxRx_COUNT
} 		//void 	uartA53M0_TLSAM_Encrypt( uint8_t *enc_file_name, uint8_t *enc_data, uint8_t enc_dataLength )










//********************************************************************
//
//********************************************************************
void 	uartA53M0_EEPROM_Save( uint16_t addr, uint16_t length, uint8_t* dataPtr )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	int  i;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}

	printf(" ========== uartA53M0_EEPROM_Save ---------- ---------- \n");

	//==================================================================
  *txPtr = 0xCC; 							txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_SAVE_EEPROM;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;

	//
	*txPtr = addr & 0x00FF; 					txPtr++;
	*txPtr = (addr>>8) & 0x00FF; 			txPtr++;
	*txPtr = length & 0x00FF; 				txPtr++;
	*txPtr = (length>>8) & 0x00FF; 		txPtr++;

	//
	uint8_t* 	ptr = dataPtr;
	for( i=0 ; i<length ; i++ )		{
		*txPtr = *ptr;
		txPtr++;
		ptr++;
	} 		//for( i=0 ; i<length ; i++ )		{


	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );
} 		//void 	uartA53M0_EEPROM_Save( uint16_t addr, uint16_t length, uint8_t* dataPtr )


//********************************************************************
//
//********************************************************************
void 	uartA53M0_EEPROM_Read( uint16_t addr, uint16_t length, uint8_t* dataPtr )
{
	unsigned char 	*txPtr = A53M0_TxBuf;
	int  i;

	while( A53M0_TxLength != 0 )	{
		usleep( 10000 );	//	10ms;
	}

	printf(" ========== uartA53M0_EEPROM_Read ---------- ---------- \n");

	//==================================================================
  *txPtr = 0xCC; 							txPtr++;
  //	UARTcmd 	Len
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;
  //	UARTcmd 	Cmd
  *txPtr = uart_REQ_READ_EEPROM;
  txPtr++;
  //	UARTcmd 	crc16
  *txPtr = 0x00; 							txPtr++;
  *txPtr = 0x00; 							txPtr++;

	//
	*txPtr = addr & 0x00FF; 					txPtr++;
	*txPtr = (addr>>8) & 0x00FF; 			txPtr++;
	*txPtr = length & 0x00FF; 				txPtr++;
	*txPtr = (length>>8) & 0x00FF; 		txPtr++;

	//------------------------------------------------------------------
	//	Get length & CRC16
	A53M0_TxBuf[1] = (txPtr - A53M0_TxBuf -3) & 0x00FF;
	A53M0_TxBuf[2] = ((txPtr - A53M0_TxBuf -3)>>8) & 0x00FF;

	UINT16 crc16 = Get_ModBus_CRC16( (UINT8*)&A53M0_TxBuf[6], (txPtr - A53M0_TxBuf -3-3) );
	A53M0_TxBuf[4] = crc16 & 0x00FF;
	A53M0_TxBuf[5] = (crc16>>8) & 0x00FF;

	//==================================================================
	//printf(" ... TEST uartA53M0_REQM0_ADC [%d] [%02X] \n", (txPtr - A53M0_TxBuf), crc16 );
  A53M0_Tx = 0;
  A53M0_TxLength = (txPtr - A53M0_TxBuf );

} 		//void 	uartA53M0_EEPROM_Read( uint16_t addr, uint16_t length, uint8_t* dataPtr )

