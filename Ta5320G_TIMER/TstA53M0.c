

#include "m0All_Header.h"




//********************************************************************
//  Global
pthread_t thread_Keyboard_input;

UINT32 	Bat_v;
UINT32 	Bat_i;




//********************************************************************
//
//********************************************************************
void  Keyboard_input_thread( void )
{
	char key;

	struct input_event 	event_in;

	printf( "\n********** **********  Thread Keyboard_input_thread Start  ********** ********** \n\n" );


	unsigned char 	eep_Tx_Buf[256];
	unsigned char 	eep_Rx_Buf[256];
	UINT16  eep_addr = 0x0000;

	int  	i;


	// 	TEST 		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	#if  	Tst_JSON_T01
		Tst_JSON_Fun( );
	#endif  		//#if  	Tst_JSON_T01
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for( i=0 ; i<256 ; i++ )		{
		//eep_Tx_Buf[i] = i;
		eep_Tx_Buf[i] = 0x11;
	}



	while( 1 )		{
		/*
		scanf("%c", &key );
		printf( "\n\n ==>%c - %02X  \n\n", key, key );
		*/

		key = getchar( );

		//	Test Change Battery Source 		A / B 	------------------------
		if( (key=='A') || (key=='a') )	{
			//printf( " key=[%c] \n", key );
			uartA53M0_Set_Bat_Source( 0 );
		}
		else if( (key=='B') || (key=='b') )	{
			//printf( " key=[%c] \n", key );
			uartA53M0_Set_Bat_Source( 1 );
		}

		//	Test EEPROM 	------------------------------------------------
		else if( (key=='S') || (key=='s') )	{
			printf( " key=[%c] \n", key );
			printf( "Set A53 to SLEEP MODE - mem\n");
			usleep( 250000 );   //  250ms
			system( "echo mem > /sys/power/state" );
		}
		
		//	Test EEPROM 	------------------------------------------------
		//	Write EEP
		else if( (key=='0') )	{
			printf( " key=[%c] \n", key );

			uartA53M0_EEPROM_Save( eep_addr, 0x33, eep_Tx_Buf );
			eep_addr += 0x10;
			for( i=0 ; i<256 ; i++ )		{
				//eep_Tx_Buf[i] += 0x01;
				//eep_Tx_Buf[i] = 0x66;
				eep_Tx_Buf[i] += 0x11;
			}
		}

		//	Read EEPROM
		else if( (key=='1') )	{
			printf( " key=[%c] \n", key );
			uartA53M0_EEPROM_Read( 0x0000, 0x80, eep_Tx_Buf );
			uartA53M0_EEPROM_Read( 0x0080, 0x80, eep_Tx_Buf );
		}

		//	Clear EEPROM All to 0x00
		else if( (key=='9') )	{
			printf( " key=[%c] \n", key );
			for( i=0 ; i<256 ; i++ )		{
				eep_Tx_Buf[i] = 0x00;
			}
			uartA53M0_EEPROM_Save( 0x0000, 0x80, eep_Tx_Buf );			
			for( i=0 ; i<256 ; i++ )		{
				eep_Tx_Buf[i] = 0x11;
			}
		}
		//	Clear EEPROM All to 0x00
		else if( (key=='8') )	{
			printf( " key=[%c] \n", key );
			for( i=0 ; i<256 ; i++ )		{
				eep_Tx_Buf[i] = 0x00;
			}
			uartA53M0_EEPROM_Save( 0x0080, 0x80, eep_Tx_Buf );			
			for( i=0 ; i<256 ; i++ )		{
				eep_Tx_Buf[i] = 0x11;
			}
		}
		//	Clear All to 0x00
		else if( (key=='7') )	{
			eep_addr = 0x0000;
		}


		//================================================================
		//printf( " ___________ sleep \n" );
		//usleep( 100000 );   //  100ms
		//usleep( 200000 );   //  200ms
		//usleep( 500000 );   //  500ms
		usleep( 1000000 );   //  1000ms
	} 		//while( 1 )		{
} 		//void  Keyboard_input_thread( void )












//******************************************************************************
//
//******************************************************************************
void 	TstA53M0_test( void )
{
	struct sched_param 	my_params;
	cpu_set_t mask;

	return;
} 		//void 	TstA53M0_test( void )


//******************************************************************************
//
//******************************************************************************
//	radarparam.txt
//	wput ./radarparam.txt ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/
//	wget -c -t5 -P /home/root/a53m0 ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt
void 	radar_param_down ( void )
{
	char	sysCmdBuf[256];

	bzero( sysCmdBuf, 256 );
	//sprintf( sysCmdBuf, "rm ./radarparam.txt" );
	sprintf( sysCmdBuf, "rm /home/root/a53m0/radarparam.txt" );
	system( sysCmdBuf );
	usleep( 20000 );

	bzero( sysCmdBuf, 256 );
	//sprintf( sysCmdBuf, "wget ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt" );
	sprintf( sysCmdBuf, "wget -c -t5 -P /home/root/a53m0 ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt" );
	system( sysCmdBuf );
} 		//void 	radar_param_down ( void )


//******************************************************************************
//
//******************************************************************************
//	/home/root/a53m0/radarparam.txt
//	M0_radarA_set		M0_radarB_set
void 	radar_param_set ( void )
{
	FILE*	f_radarParam;
	unsigned char  R_lineBuf[256];

	int 	scan_interval;


	f_radarParam = fopen( "/home/root/a53m0/radarparam.txt" , "r+b" ) ;
	if( f_radarParam )		{
		
		//	Read line 	--------------------------------------------------
		int rLength;
		int paramCnt = 0;

		while( !feof( f_radarParam ) )		{
			bzero( R_lineBuf, 256 );

			rLength = fscanf( f_radarParam, "%s", R_lineBuf );
			if( rLength <= 0 )		{				break;				}
			//printf("Rparam [%04d]  [%s][%04d] \n", rLength, R_lineBuf, atoi( R_lineBuf ) );

			switch( paramCnt )		{
				case 0:
					M0_radarA_set.radar_X = atoi( R_lineBuf );
					break;
				case 1:
					M0_radarA_set.radar_Y = atoi( R_lineBuf );
					break;
				case 2:
					M0_radarA_set.radar_L = atoi( R_lineBuf );
					break;
				case 3:
					M0_radarA_set.radar_W = atoi( R_lineBuf );
					break;
				case 4:
					M0_radarA_set.radar_H = atoi( R_lineBuf );
					break;
				case 5:
					M0_radarA_set.Theta = atoi( R_lineBuf );
					break;
				case 6:
					M0_radarA_set.Phi = atoi( R_lineBuf );
					break;
				case 7:
					M0_radarA_set.L_R = atoi( R_lineBuf );
					break;
				case 8:
					scan_interval = atoi( R_lineBuf );
					break;
				case 9:
					M0_radarB_set.radar_X = atoi( R_lineBuf );
					break;
				case 10:
					M0_radarB_set.radar_Y = atoi( R_lineBuf );
					break;
				case 11:
					M0_radarB_set.radar_L = atoi( R_lineBuf );
					break;
				case 12:
					M0_radarB_set.radar_W = atoi( R_lineBuf );
					break;
				case 13:
					M0_radarB_set.radar_H = atoi( R_lineBuf );
					break;
				case 14:
					M0_radarB_set.Theta = atoi( R_lineBuf );
					break;
				case 15:
					M0_radarB_set.Phi = atoi( R_lineBuf );
					break;
				case 16:
					M0_radarB_set.L_R = atoi( R_lineBuf );
					break;
				case 17:
					scan_interval = atoi( R_lineBuf );
					break;
			} 		//switch( paramCnt )		{
			paramCnt++;
		} 		//while( !feof()f_radarParam )		{
		printf("  __________ \n");

		uartA53M0_Set_Radar_ReStart( &M0_radarA_set, &M0_radarB_set );
		fclose( f_radarParam );
		return;
	} 	//if( f_radarParam )		{

	uartA53M0_Set_Radar_ReStart( 0, 0 );
} 		//void 	radar_param_set ( void )









//********************************************************************
//
//********************************************************************
// 	Test  "system" / "popen"
void 	Tst_JSON_Fun01( void )
{
	char	sysCmdBuf[4096];

	printf( "\n" );
	printf(" __________  Tst_JSON_Fun01 __________\n");

	printf("\n __________  Tst_JSON_Fun01  T01\n");
	bzero( sysCmdBuf, 4096 );
	//sprintf( sysCmdBuf, "wget ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt" );
	sprintf( sysCmdBuf, "wget -c -t5 -P /home/root/a53m0 ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt" );
	system( sysCmdBuf );

	
	printf("\n __________  Tst_JSON_Fun01  T02\n");
	bzero( sysCmdBuf, 4096 );
	//sprintf( sysCmdBuf, "wget ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.txt" );
	sprintf( sysCmdBuf, "wget -c -t5 -P /home/root/a53m0 ftp://parkmeter:gitpark@ftp.green-ideas.com.tw/Tst_radar/radarparam.TTT" );
	system( sysCmdBuf );

	
	printf("\n __________  Tst_JSON_Fun01  T03\n");
	bzero( sysCmdBuf, 4096 );
	FILE* pp;

	if((pp = popen("ls -l", "r")) == NULL) 	{
		printf("popen() error!\n");
		return;
	}

	while (fgets(sysCmdBuf, sizeof sysCmdBuf, pp)) {  
	   printf("  popen r[%s]", sysCmdBuf);  
	}  
	printf( "-----\n" );
	pclose(pp);  

	printf("\n __________  Tst_JSON_Fun01  T04\n");
	bzero( sysCmdBuf, 4096 );
	//FILE* pp;

	if((pp = popen("llllls -l", "r")) == NULL) 	{
		printf("popen() error!\n");
		return;
	}

	while (fgets(sysCmdBuf, sizeof sysCmdBuf, pp)) {  
	   printf("  popen r[%s]", sysCmdBuf);  
	}  
	printf( "-----\n" );
	pclose(pp);  


	printf( "\n" );
} 		//void 	Tst_JSON_Fun01( void )



//********************************************************************
//
//********************************************************************
// 	Test  "system" / "popen"
unsigned char   FILE_JSON_TEST[128] = "/home/root/a53m0/getParam.txt";

void 	Tst_JSON_Fun( void )
{
	printf( "\n" );
	printf(" __________  Tst_JSON_Fun __________ [%s]\n", FILE_JSON_TEST);


	FILE *f_json;
	int  	f_size;
	char* jsonBuf;

	
	f_json = fopen( FILE_JSON_TEST, "r+b");
	
	if( f_json == NULL ) 	{
		printf( "  xxxxx  Tst_JSON_Fun, Open [%s] Error !!!  xxxxx\n", FILE_JSON_TEST );
		return;
	}
	//printf( "  Tst_JSON_Fun, Open [%s] Ready. \n", FILE_JSON_TEST );

	// 	Get File Size
	fseek( f_json, 0, SEEK_END);
	f_size = ftell( f_json );
	fseek( f_json, 0, SEEK_SET);
	printf( "  Tst_JSON_Fun, Open [%s] Ready.  [%d]\n", FILE_JSON_TEST, f_size );

	jsonBuf = malloc( (f_size/4)*4+16 );

	if( !jsonBuf )		{
		printf( "  xxxxx  Tst_JSON_Fun, malloc Fial  xxxxx\n" );
		return;
	}
	fread( jsonBuf, f_size, 1, f_json );
	/*
	printf( "\n[[" );
	for( int i=0 ; i<f_size ; i++ ) 	{
		printf( " %c", jsonBuf[i] );
	}
	printf( "]]\n" );
	*/
	fclose( f_json );


	//  	JSON Start  ==================================================

	struct json_object *Jobj_main;

	Jobj_main = json_tokener_parse( jsonBuf );
	//printf("1-tokener parse: %s\n", json_object_to_json_string(Jobj_main));
	if( json_object_to_json_string(Jobj_main) == NULL )	{
		printf( "  xxxxx  Tst_JSON_Fun, Get JSON Fial  xxxxx\n" );
		free( jsonBuf );
		return;
	}
	printf( "  Tst_JSON_Fun, Get JSON [[%s]]\n", json_object_to_json_string(Jobj_main) );
	printf( "\n" );
	


	struct json_object *Jobj_Params1 = json_object_object_get(Jobj_main, "Params1");
	if( Jobj_Params1 == NULL )		{
		printf( "  xxxxx  Tst_JSON_Fun, Get JSON Fial 2 xxxxx\n" );
		free( jsonBuf );
		return;
	}


	struct json_object *Jobj_rb60p1 = json_object_object_get(Jobj_Params1, "rb60p1");
	struct json_object *Jobj_rb60p2 = json_object_object_get(Jobj_Params1, "rb60p2");

	if( (Jobj_rb60p1 == NULL) || (Jobj_rb60p2 == NULL ) )		{
	//if( (Jobj_rb60p1 == NULL) )		{
		printf( "  xxxxx  Tst_JSON_Fun, Get JSON Fial 3 xxxxx\n" );
		free( jsonBuf );
		return;
	}

	int  	i;
	//  Jobj_rb60p1  	------------------------------------------------------
	printf( "  ----------  rb60p1 : [" );
	for( i=0 ; i<8 ; i++ )		{
		printf( " %d", json_object_get_int(json_object_array_get_idx(Jobj_rb60p1, i)) );
	}
	printf( " ]\n" );


	//  Jobj_rb60p2  	------------------------------------------------------
	printf( "  ----------  rb60p2 : [" );
	for( i=0 ; i<8 ; i++ )		{
		printf( " %d", json_object_get_int(json_object_array_get_idx(Jobj_rb60p2, i)) );
	}
	printf( " ]\n" );


	//json_object_put(Jobj_rb60p1);
	//json_object_put(Jobj_rb60p2);
	//json_object_put(Jobj_Params1);

	json_object_put(Jobj_main);

printf(" __________  Tst_JSON_Fun --- End \n");

	//==================================================================
	free( jsonBuf );

	printf( "\n" );
} 		//void 	Tst_JSON_Fun( void )

