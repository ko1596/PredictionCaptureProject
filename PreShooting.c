/**
******************************************************************************
* File Name          : PreShooting.c
* Description        : Algorithm that allows the camera to determine whether to shoot.
******************************************************************************
*
* COPYRIGHT(c) 2021 Joey Ke
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "PreShooting.h"

Radar_Error Radar_GetObjectStatus(Radar_PredictionData_t *pPredictionData)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	Radar_ObjectAreaStatus ObjectAreaStatus = RADAR_OBJECTAREASTATUS_OUT;

	pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;

	if (M0_radarA.data.obj_position_X == 0 && M0_radarA.data.obj_position_Y == 0)
		Status = RADAR_ERROR_WORNG_DISTANCE;

	if (Status == RADAR_ERROR_NONE)
	{
		if (M0_radarA.data.obj_position_X < 500 && M0_radarA.data.obj_position_Y < 280)
			ObjectAreaStatus = RADAR_OBJECTAREASTATUS_IN;

		if (ObjectAreaStatus == RADAR_OBJECTAREASTATUS_IN)
		{	
			if(pPredictionData->SpeedData.DeltaV < -5)
				pPredictionData->Status = RADAR_PREDICTIONSTATUS_COMING;
			else if(pPredictionData->SpeedData.DeltaV > 5)
				pPredictionData->Status = RADAR_PREDICTIONSTATUS_LEAVING;
			else if(pPredictionData->SpeedData.DeltaV <= 5 && pPredictionData->SpeedData.DeltaV >= -5)
				pPredictionData->Status = RADAR_PREDICTIONSTATUS_PARKING;
			else if(M0_radarA.data.obj_type == 1)
				pPredictionData->Status = RADAR_PREDICTIONSTATUS_PARKED;
			else
				pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;
		}
	}

	return Status;
}

Radar_Error Radar_GetObjectSpeedData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	if (data.obj_distance_R == 0)
		Status = RADAR_ERROR_INVALID_STATU;

	if (Status == RADAR_ERROR_NONE)
	{
		
	 	pPredictionData -> SpeedData.DeltaX = data.obj_distance_R - pPredictionData -> SpeedData.InitialDistance;
		pPredictionData -> SpeedData.InitialDistance = data.obj_distance_R;

		pPredictionData -> SpeedData.DeltaV = pPredictionData -> SpeedData.DeltaX - pPredictionData -> SpeedData.InitialSpeed;
		pPredictionData -> SpeedData.InitialSpeed = pPredictionData -> SpeedData.DeltaX;
	}

	return Status;
}

Radar_Error Radar_TakePicture(void)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	const char *path = "./capture.sh";
	char sysCmdBuf[256];

	if (access(path, F_OK) == -1)
		Status = RADAR_ERROR_FILE_NOT_EXIST;

	if(Status == RADAR_ERROR_NONE){
		bzero(sysCmdBuf, 256);
		sprintf(sysCmdBuf, path);
		system(sysCmdBuf);
		usleep(20000);
	}

	return Status;
}

Radar_Error Radar_PrintData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data){
	Radar_Error Status = RADAR_ERROR_NONE;

	data.L_R ? PRINTLF(LEFT) : PRINTLF(RIGHT);		//PRINTLF(LF) printf("--------------------------#LF--------------------------\n\r")

	printf("initX: %3d\tDeltaX: %2d\n\rinitV: %3d\tDeltaV: %2d\n\rStatus: %1d\n\rDistance: %3d\n\r",
		pPredictionData ->SpeedData.InitialDistance,
		pPredictionData->SpeedData.DeltaX,
		pPredictionData->SpeedData.InitialSpeed,
		pPredictionData->SpeedData.DeltaV,
		pPredictionData->Status,
		data.obj_distance_R
		);
	printf("--------------------------------------------------------\n\r");
	return Status;
}

Radar_Error Radar_InitData(Radar_PredictionData_t *pPredictionData){
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionData->SpeedData.DeltaV=0;
	pPredictionData->SpeedData.DeltaX=0;
	pPredictionData->SpeedData.InitialDistance=0;
	pPredictionData->SpeedData.InitialSpeed=0;
	pPredictionData->Status=RADAR_PREDICTIONSTATUS_INVALID;
	return Status;
}