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

Radar_Error Radar_GetObjectStatus(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	if (Status == RADAR_ERROR_NONE)
	{

		if (data.obj_distance_R == 0 && pPredictionData->SpeedData.DeltaV == 0 && pPredictionData->SpeedData.DeltaX == 0)
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_EMPTY;

		else if (pPredictionData->SpeedData.DeltaV < -5)
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_COMING;

		else if (pPredictionData->SpeedData.DeltaV > 5)
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_LEAVING;

		else if (data.obj_distance_R > 0 && pPredictionData->SpeedData.DeltaV == 0 && pPredictionData->SpeedData.DeltaX == 0)
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_PARKED;

		else if (pPredictionData->SpeedData.DeltaV <= 5 && pPredictionData->SpeedData.DeltaV >= -5)
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_PARKING;

		else
			pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;

	}

	return Status;
}

Radar_Error Radar_GetObjectSpeedData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	if (Status == RADAR_ERROR_NONE)
	{
		if (pPredictionData->SpeedData.InitialDistance != 0)
			pPredictionData->SpeedData.DeltaX = data.obj_distance_R - pPredictionData->SpeedData.InitialDistance;
		else 
			pPredictionData->SpeedData.DeltaX = 0;

		pPredictionData->SpeedData.InitialDistance = data.obj_distance_R;

		if (pPredictionData->SpeedData.InitialSpeed != 0)
			pPredictionData->SpeedData.DeltaV = pPredictionData->SpeedData.DeltaX - pPredictionData->SpeedData.InitialSpeed;
		else
			pPredictionData->SpeedData.DeltaV = 0;

		pPredictionData->SpeedData.InitialSpeed = pPredictionData->SpeedData.DeltaX;
	}

	return Status;
}

void *Radar_TakePicture(void *parm)
{
	pthread_mutex_lock(&mutex1);
	
	Radar_Error Status = RADAR_ERROR_NONE;
	char path[30] = "./capture.sh";
	char sysCmdBuf[256];
	char *amount = (char*) parm;
	char test[1] = " ";
	strcat(path, test);
	strcat(path, amount);

	printf("%s\n\r", path);


	if (Status == RADAR_ERROR_NONE)
	{
		bzero(sysCmdBuf, 256);
		sprintf(sysCmdBuf, path);
		system(sysCmdBuf);
		usleep(20000);
	}

	pthread_mutex_unlock(&mutex1);
	pthread_exit(0);
}

Radar_Error Radar_PrintData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	data.L_R ? PRINTLF(LEFT) : PRINTLF(RIGHT); //PRINTLF(LF) printf("--------------------------#LF--------------------------\n\r")

	#if RADAR_PRINT_SPEED
		printf("initX: %3d\tDeltaX: %2d\n\rinitV: %3d\tDeltaV: %2d\n\rStatus: %1d\n\r",
			pPredictionData->SpeedData.InitialDistance,
			pPredictionData->SpeedData.DeltaX,
			pPredictionData->SpeedData.InitialSpeed,
			pPredictionData->SpeedData.DeltaV,
			pPredictionData->Status);
	#endif

	#if RADAR_PRINT_XYZ
		printf("RadarA X[%02d] Y[%02d] Z[%02d] D[%02d] P[%04d]\n\r",
		   data.obj_position_X,
		   data.obj_position_Y,
		   data.obj_position_Z,
		   data.obj_distance_R,
		   data.power);
	#endif

	printf("--------------------------------------------------------\n\r");
	return Status;
}

Radar_Error Radar_InitData(Radar_PredictionData_t *pPredictionData)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionData->SpeedData.DeltaV = 0;
	pPredictionData->SpeedData.DeltaX = 0;
	pPredictionData->SpeedData.InitialDistance = 0;
	pPredictionData->SpeedData.InitialSpeed = 0;
	pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;

	return Status;
}