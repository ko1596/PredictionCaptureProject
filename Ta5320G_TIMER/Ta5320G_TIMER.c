

//	******************************************************************
//		Ta5320G_TIMER
//	******************************************************************

#include "m0All_Header.h"




//********************************************************************
//
//********************************************************************
int main( int argc, char *argv[] )
{ 
  //unsigned char   wBuf[1024];
  //signal(SIGCHLD, SIG_IGN);


  a53m0API_currentTime = time( NULL);
  a53m0API_current_tm = localtime (&a53m0API_currentTime);
  printf( "\n" );
  printf( "          [%s]  ---   %02d/%02d/%02d %02d:%02d:%02d   [%s] \n", 
  			dev_UART_A53M0, 
				((a53m0API_current_tm->tm_year+1900)-2000),
				a53m0API_current_tm->tm_mon + 1,
				a53m0API_current_tm->tm_mday,
				a53m0API_current_tm->tm_hour,
				a53m0API_current_tm->tm_min,
				a53m0API_current_tm->tm_sec,
  			FwVer_Ta53201_UART_HS );
  printf( "\n" );

  printf( "argv[1]=[%s]\n", argv[1] );    //  =1, Correction time

  int flg_CorrectionTime = 0;
  if( argv[1] != NULL ) {
  	flg_CorrectionTime = atoi( argv[1] );
  	if( flg_CorrectionTime != 0 )	{
  		flg_CorrectionTime = 1;
  	}

  }




  //******************************************************************
	//******************************************************************
	//	A53_M0 API 		Start
	//******************************************************************
  

  //==================================================================
  //	open UART port  
  //==================================================================
  //fd_uartA53M0 = open( dev_UART_A53M0, O_RDWR | O_NOCTTY | O_NDELAY );
  //fd_uartA53M0 = open( dev_UART_A53M0, O_RDWR | O_NONBLOCK );
  fd_uartA53M0 = open( dev_UART_A53M0, O_RDWR );
  
  if(fd_uartA53M0 == -1){
    printf( "[%s]  xxxxx    open Error    xxxxx \n", dev_UART_A53M0 );
    return -1;
    
  }
  else {
    printf( "[%s]  open Success ======================================== \n", dev_UART_A53M0 );

    if(flock( fd_uartA53M0, LOCK_EX | LOCK_NB )==0)  {

	    struct termios uart_settings;
	    tcgetattr(fd_uartA53M0, &uart_settings);
	    //bzero(&uart_settings, sizeof(uart_settings));
	    uart_settings.c_cflag = UARTA53M0_SPEED | CS8 | CLOCAL | CREAD;
	    uart_settings.c_iflag = IGNPAR;
	    uart_settings.c_oflag = 0;
	    uart_settings.c_lflag = 0;
	    uart_settings.c_cc[VTIME] = 0;
	    //uart_settings.c_cc[VMIN] = 1;
	    uart_settings.c_cc[VMIN] = 0;

			cfsetispeed(&uart_settings,UARTA53M0_SPEED);
			cfsetospeed(&uart_settings,UARTA53M0_SPEED);
			
	    tcsetattr(fd_uartA53M0, TCSANOW, &uart_settings);
	    tcflush(fd_uartA53M0, TCIFLUSH);

    } 	//if(flock( fd_uartA53M0, LOCK_EX | LOCK_NB )==0)  {
    else	{
    	printf("XXXXXXXXXXXXXXXXXXX [%s]] was locked by other process.\n", dev_UART_A53M0 );
    	return -1;
    }

  } 	//if(fd_uartA53M0 == -1){				else


  //==================================================================
  unsigned char 	fname_folder[64];
  bzero( fname_folder, 64 );
  sprintf( fname_folder, "mkdir ./TL_SAM" );
  system( fname_folder );
  bzero( fname_folder, 64 );
  sprintf( fname_folder, "chmod 777 -R ./TL_SAM" );
  system( fname_folder );

  bzero( fname_folder, 64 );
  sprintf( fname_folder, "mkdir ./radar" );
  system( fname_folder );

  bzero( fname_folder, 64 );
  sprintf( fname_folder, "chmod 777 -R ./radar" );
  system( fname_folder );




  pthread_attr_t  attr_Tx, attr_Rx;
  int 	policy_Rx;
  int 	policy_Tx;
  struct sched_param 	ached_Rx_params;
  struct sched_param 	ached_Tx_params;

  TstA53M0_test( );






	//==================================================================
	//  Create A53 rpmsg rx
	//==================================================================
	pthread_attr_init(&attr_Rx);

	//pthread_attr_setschedpolicy( &attr_Rx, SCHED_FIFO );
	//ached_Rx_params.sched_priority = 90;
	//pthread_attr_setschedparam (&attr_Rx, &ached_Rx_params);

	pthread_create(&thread_uartA53M0_Rx, &attr_Rx, (void*)&uartA53M0_Rx_thread, &fd_uartA53M0);  
	/*
	pthread_attr_getschedpolicy( &attr_Rx, &policy_Rx);
	switch(policy_Rx)	{
		case SCHED_FIFO:
			printf(" policy Rx SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf(" policy Rx SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf(" policy Rx SCHED_OTHER\n");
			break;

		default:
			printf(" policy Rx XXX\n");
			break;
	}
	*/

	/*
	pthread_attr_setschedpolicy( &attr_Rx, SCHED_FIFO );
	ached_Rx_params.sched_priority = 99;
	pthread_attr_setschedparam (&attr_Rx, &ached_Rx_params);

	pthread_attr_getschedpolicy( &attr_Rx, &policy_Rx);
	switch(policy_Rx)	{
		case SCHED_FIFO:
			printf(" policy Rx SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf(" policy Rx SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf(" policy Rx SCHED_OTHER\n");
			break;

		default:
			printf(" policy Rx XXX\n");
			break;
	}
	*/


	//==================================================================
	//  Create A53 rpmsg Tx
	//==================================================================
	pthread_attr_init(&attr_Tx);

	//pthread_attr_setschedpolicy( &attr_Tx, SCHED_FIFO );
	//ached_Tx_params.sched_priority = 55;
	//pthread_attr_setschedparam (&attr_Tx, &ached_Tx_params);

	pthread_create(&thread_uartA53M0_Tx, &attr_Tx, (void*)&uartA53M0_Tx_thread, &fd_uartA53M0);
	/*
	pthread_attr_getschedpolicy( &attr_Tx, &policy_Tx);	
	switch(policy_Tx)	{
		case SCHED_FIFO:
			printf(" policy Tx SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf(" policy Tx SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf(" policy Tx SCHED_OTHER\n");
			break;

		default:
			printf(" policy Tx XXX\n");
			break;
	}
	*/
	/*
	pthread_attr_setschedpolicy( &attr_Tx, SCHED_FIFO );
	ached_Tx_params.sched_priority = 10;
	pthread_attr_setschedparam (&attr_Tx, &ached_Tx_params);

	pthread_attr_getschedpolicy( &attr_Tx, &policy_Tx);
	switch(policy_Tx)	{
		case SCHED_FIFO:
			printf(" policy Tx SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printf(" policy Tx SCHED_RR\n");
			break;
		case SCHED_OTHER:
			printf(" policy Tx SCHED_OTHER\n");
			break;

		default:
			printf(" policy Tx XXX\n");
			break;
	}
	*/






  //	Test 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  Debug port Keyboard Input
	pthread_create(&thread_Keyboard_input, NULL, (void*)&Keyboard_input_thread, NULL);


  //******************************************************************
	//	A53_M0 API 		END
	//******************************************************************
	//******************************************************************
  sleep( 2 );





  //------------------------------------------------------------------
  if( flg_CorrectionTime )  	{
	  uartA53M0_SetM0_currentRTC( );
	  //sleep( 1 );  	
  }




	//	TEST 		--------------------------------------------------------
	//	TL-SAM 		Encryption
	uint8_t 	dcu_fName[64];
	uint8_t 	dcu_Data[1024];

  a53m0API_currentTime = time( NULL);
  a53m0API_current_tm = localtime (&a53m0API_currentTime);
  sprintf( dcu_fName, "./TL_SAM/Enc%02d%02d%02d%02d%02d%02d",
				((a53m0API_current_tm->tm_year+1900)-2000),
				a53m0API_current_tm->tm_mon + 1,
				a53m0API_current_tm->tm_mday,
				a53m0API_current_tm->tm_hour,
				a53m0API_current_tm->tm_min,
				a53m0API_current_tm->tm_sec );

  for( int i=0 ; i<1000 ; i++ )		{
  	dcu_Data[i] = i+0x11;
  }
  //uartA53M0_TLSAM_Encrypt( (uint8_t*)dcu_fName, (uint8_t*)dcu_Data, 16 );
  sleep( 1 );



  	//------------------------------------------------------------------
	//	TEST 		--------------------------------------------------------
	//uartA53M0_Get_Radar_FWver( );

	//	TEST 		--------------------------------------------------------
	//	Set Radar-A/B
	/*
	M0_radarA_set.radar_X = 32;
	M0_radarA_set.radar_Y = 32;
	M0_radarA_set.radar_L = 302;
	M0_radarA_set.radar_W = 182;
	M0_radarA_set.radar_H = 112;
	M0_radarA_set.Theta = 12;
	M0_radarA_set.Phi = 12;
	M0_radarA_set.L_R = 0;

	M0_radarB_set.radar_X = 34;
	M0_radarB_set.radar_Y = 34;
	M0_radarB_set.radar_L = 334;
	M0_radarB_set.radar_W = 184;
	M0_radarB_set.radar_H = 114;
	M0_radarB_set.Theta = 24;
	M0_radarB_set.Phi = 24;
	M0_radarB_set.L_R = 1;
	*/
	//uartA53M0_Set_Radar_ReStart( &M0_radarA_set, &M0_radarB_set );
	uartA53M0_Set_Radar_ReStart( 0, 0 );
	//uartA53M0_Set_Radar_ReStart( 0, &M0_radarB_set );
	//uartA53M0_Set_Radar_ReStart( &M0_radarA_set, 0 );
	//sleep( 1 );


  //	TEST 		--------------------------------------------------------
  printf("\n");
  uartA53M0_REQM0_SystemInfo( );
  sleep( 1 );









	#if 	TST_TxRx_COUNT
		Tst_Tx_Cnt = 0;
		Tst_Rx_Cnt = 0;
	#endif 	//#if 	TST_TxRx_COUNT
  //******************************************************************
  //	Waiting thread
  //******************************************************************
	sleep( 2 );

	struct timeb  	time_T1;
	struct timeb  	time_T2;
	ftime( &time_T1 );
  
  while( 1 )  {
  	//printf( "\n" );
  	//uartA53M0_REQM0_PwMeter( );
  	//uartA53M0_REQM0_ADC( );

  	uartA53M0_Req_Dumming_Test( );


  	//uartA53M0_REQM0_SystemInfo( );
  	//uartA53M0_REQM0_RTC( );
  	
  	//uartA53M0_REQM0_Temp( );

  	// 	Test - TL-SAM 	Read F/W version		!!!!!!!!!!!!!!!!!!!!!!!!!!

  	// 	Test - TL-SAM 	Encryption		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  	/*
		a53m0API_currentTime = time( NULL);
		a53m0API_current_tm = localtime (&a53m0API_currentTime);
		sprintf( dcu_fName, "./TL_SAM/Enc%02d%02d%02d%02d%02d%02d",
					((a53m0API_current_tm->tm_year+1900)-2000),
					a53m0API_current_tm->tm_mon + 1,
					a53m0API_current_tm->tm_mday,
					a53m0API_current_tm->tm_hour,
					a53m0API_current_tm->tm_min,
					a53m0API_current_tm->tm_sec );
		uartA53M0_TLSAM_Encrypt( (uint8_t*)dcu_fName, (uint8_t*)dcu_Data, 1000 );
		//uartA53M0_TLSAM_Encrypt( (uint8_t*)dcu_fName, (uint8_t*)dcu_Data, 16 );
		*/

  	//printf("  Test  --  ");
  	//uartA53M0_Req_Radar_Status( );


	  //----------------------------------------------------------------
	  //usleep(  15000 );   //  15ms
	  //usleep(  25000 );   //  25ms
	  //usleep(  50000 );   //  50ms
	  //usleep(  75000 );   //  75ms
	  //usleep( 90000 );   //  90ms
	  //usleep( 100000 );   //  100ms
	  //usleep( 120000 );   //  120ms
	  //usleep( 150000 );   //  150ms
	  //usleep( 200000 );   //  200ms
	  //usleep( 500000 );   //  500ms

  	//sleep(1);
  	//sleep(2);
  	//sleep(3);
    //sleep(5);
    //sleep(10);
    //sleep(20);
  	//sleep(30);
    //sleep(60); 		//	1min
    //sleep(600);		//	10min

	  //	Test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  ftime( &time_T2 );
	  //printf( "  %d  \n", ((time_T2.time*1000)+time_T2.millitm)-((time_T1.time*1000)+time_T1.millitm) );
	  if( ((time_T2.time*1000)+time_T2.millitm)-((time_T1.time*1000)+time_T1.millitm) >  10000 )		{  		//	10sec
	  	time_T1 = time_T2;

			uartA53M0_REQM0_PwMeter( );
			uartA53M0_REQM0_ADC( );
	  }


	  //	Test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  /*
    #if 	TST_TxRx_COUNT
    	if( Tst_Tx_Cnt >= 10000 )	{
    		break;
    	}
    #endif 		//#if 	TST_TxRx_COUNT
    */
  } 		//while( 1 )  {
  
  
  //printf("  __________ \n\nb");
  //pthread_join( thread_uartA53M0_Tx, NULL );
  //sleep( 15 );
  sleep( 2 );
  pthread_cancel( thread_uartA53M0_Rx );
  pthread_cancel( thread_uartA53M0_Tx );

  #if 	TST_TxRx_COUNT
  	//printf(" Tx[%08d]  Rx[%08d]\n", Tst_Tx_Cnt, Tst_Rx_Cnt );
  #endif 		//#if 	TST_TxRx_COUNT

  //  Ta5320G_TIMER
  close(fd_uartA53M0);
  printf( " ___________     Ta5320G_TIMER End     ___________ \n\n" );
}   //int main()


//  scp ./gpio2hl root@192.168.1.116:/home/root/Tgcc01
