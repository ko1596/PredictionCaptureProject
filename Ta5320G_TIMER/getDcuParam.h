

//********************************************************************
//  getDcuParam.h
//********************************************************************

#ifndef GET_DCU_PARAM_H
#define GET_DCU_PARAM_H




//********************************************************************
//	define
typedef struct  {
	UINT8			name[16];
	UINT16 		param[12];
}RADAR_PARAM_1_PARAM;


typedef struct  {
	
	UINT8			name[64];
	UINT8			createtime[64];
	UINT8			modifytime[64];
	UINT8			effectivetime[64];
	UINT8 		spacenum[2][64];

	UINT16		jsonver;
	UINT16 		baynum;
	UINT16 		meterposition;
	UINT16 		bayenable[2];	
	UINT16 		takephotoroutine;
	UINT16 		logenable;
	UINT16 		grace[2];

	RADAR_PARAM_1_PARAM 	radar_param1;


}DCU_DL_PARAM;



#if  !JSON_TST_DISABLE


//********************************************************************
//	Global






//********************************************************************
//  Function
uint8_t 	get_DCU_param( void );


#endif 		//#if  !JSON_TST_DISABLE


#endif    //GET_DCU_PARAM_H

