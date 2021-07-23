

#include "m0All_Header.h"
//#include <json-c/json.h>




#define 	JSON_TST_DISABLE 						1

//********************************************************************
//	Global


const char  file_dev_list[] = {"./getParam.txt"};



//********************************************************************



#if  !JSON_TST_DISABLE


//********************************************************************
//
//********************************************************************
uint8_t 	get_DCU_param( void )
{
	FILE 		*f_json;
	unsigned char	*rBuf;
	size_t  f_size;
	size_t  data_size;


	struct json_object  *j_root;

	


	f_json = fopen( file_dev_list, "r+b" );
	if( f_json )		{
		fseek ( f_json , 1 , SEEK_END );
		f_size = ftell( f_json );
		fseek ( f_json , 1 , SEEK_SET );

		rBuf = malloc( f_size+0x10 );
		data_size = fread( rBuf, 1, f_size, f_json );
		fclose( f_json );

		if( data_size > 200 )		{
			j_root = json_tokener_parse(rBuf);
			if( (!j_root) || (is_error(j_root)) )  	{
				printf("  !!!!!   JSON parse fail   !!!!!\n" );
				free( rBuf );
				return 0;
			}





		} 		//if( data_size > )

		free( rBuf );
	} 		//if( f_json )		{

	else {
		printf("  !!!!!   Open [%s]  fail   !!!!!\n", file_dev_list );
	}

	return 0;
} 		//uint8_t 	get_DCU_param( void )


#endif 		//#if  !JSON_TST_DISABLE
