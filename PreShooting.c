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

extern Radar_Error Radar_GetObjectStatus(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	if (Status == RADAR_ERROR_NONE)
	{

		if (data.parking_status == 0)
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

extern Radar_Error Radar_GetObjectSpeedData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
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
	Radar_Error Status = RADAR_ERROR_NONE;
	char path[30] = "./capture.sh";
	char sysCmdBuf[256];
	char *argc = (char *)parm;
	char blank[1] = " ";
	strcat(path, blank);
	strcat(path, argc);

	printf("%s\n\r", path);

	if (Status == RADAR_ERROR_NONE)
	{
		chdir("/home/root/Samuel/test_programs/");
		bzero(sysCmdBuf, 256);
		sprintf(sysCmdBuf, path);
		system(sysCmdBuf);
		chdir("/");
		usleep(20000);
	}

	pthread_exit(0);
}

extern Radar_Error Radar_PrintData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	#if RADAR_PRINT
	data.L_R ? PRINTLF(LEFT) : PRINTLF(RIGHT); //PRINTLF(LF) printf("--------------------------#LF--------------------------\n\r")

	#if RADAR_PRINT_SPEED
		printf("initX: %3d\tDeltaX: %2d\n\rinitV: %3d\tDeltaV: %2d\n\r",
			pPredictionData->SpeedData.InitialDistance,
			pPredictionData->SpeedData.DeltaX,
			pPredictionData->SpeedData.InitialSpeed,
			pPredictionData->SpeedData.DeltaV);
	#endif

	#if RADAR_PRINT_STATUS
		printf("Status: ");
		switch (pPredictionData->Status)
		{
		case RADAR_PREDICTIONSTATUS_COMING:
			printf("Comeing\n\r");
			break;
		case RADAR_PREDICTIONSTATUS_LEAVING:
			printf("Leaving\n\r");
			break;
		case RADAR_PREDICTIONSTATUS_PARKING:
			printf("Parking\n\r");
			break;
		case RADAR_PREDICTIONSTATUS_PARKED:
			printf("Parked\n\r");
			break;
		case RADAR_PREDICTIONSTATUS_EMPTY:
			printf("Empty\n\r");
			break;
		case RADAR_PREDICTIONSTATUS_INVALID:
			printf("Invalid\n\r");
			break;
		default:
			break;
		}
	#endif

	#if RADAR_PRINT_XYZ
		printf("RadarA X[%02d] Y[%02d] Z[%02d] D[%02d] P[%04d]\n\r",
			data.obj_position_X,
			data.obj_position_Y,
			data.obj_position_Z,
			data.obj_distance_R,
			data.CarChange_status);
	#endif

	printf("--------------------------------------------------------\n\r");
	#endif
	return Status;
}

extern Radar_Error Radar_InitData(Radar_PredictionData_t *pPredictionData)
{
	printf("data initialized\n\r");
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionData->SpeedData.DeltaV = 0;
	pPredictionData->SpeedData.DeltaX = 0;
	pPredictionData->SpeedData.InitialDistance = 0;
	pPredictionData->SpeedData.InitialSpeed = 0;
	pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;
	pPredictionData->target = false;
	pPredictionData->captured = false;
	pPredictionData->conter = 0;
	memset(pPredictionData->filename, 0, 30);
	memset(pPredictionData->time, 0, 30);
	return Status;
}

extern Radar_Error Radar_CleanData(Radar_PredictionData_t *pPredictionData)
{
	Radar_Error Status = RADAR_ERROR_NONE;

	if (pPredictionData->Status == RADAR_PREDICTIONSTATUS_EMPTY)
	{
		
		pPredictionData->target = false;
		if(pPredictionData->conter > 0 && pPredictionData->conter <= 4)
			pPredictionData->conter++;
		else if (pPredictionData->conter > 4 && pPredictionData->conter < 10)
		{
			Status = Radar_DeleteFile(pPredictionData->time);
			pPredictionData->conter = 0;
			memset(pPredictionData->time, 0, 30);
		}
		else
		{
			pPredictionData->conter = 0;
			memset(pPredictionData->time, 0, 30);
		}

		memset(pPredictionData->filename, 0, 30);
		

	}

	if (pPredictionData->conter > 4)
		pPredictionData->captured = true;
	else
		pPredictionData->captured = false;

	return Status;
}

Radar_Error Radar_DeleteFile(char *filename)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	char delpath[40] = SAVE_PICTURE_PATH;
	strcat(delpath, filename);
	system(delpath);
	printf("It's too short delete %s\n\r", filename);
	return Status;
}

bool IsPreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	if (pPredictionData->Status == RADAR_PREDICTIONSTATUS_PARKING || pPredictionData->Status == RADAR_PREDICTIONSTATUS_COMING)
		return data.L_R ?  data.obj_distance_R < LEFT_DETECT_DISTANCE : data.obj_distance_R < RIGHT_DETECT_DISTANCE;
	else
		return false;
}

extern Radar_Error Radar_PreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	time_t now = time(NULL);
	struct tm *newtime = localtime(&now);
	Radar_Error Status = RADAR_ERROR_NONE;
	int err;

	if (IsPreShoot(pPredictionData, data) && pPredictionData->target == false)
		if (data.L_R)
		{
			strcat(pPredictionData->filename, LEFT_PICTURE_AMOUNT);
			strftime(pPredictionData->time, 128, "%Y%m%d%H%M%S_LEFT", newtime);
			strcat(pPredictionData->filename, pPredictionData->time);
			err = pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&Radar_TakePicture, pPredictionData->filename);
			if (err != 0)
				printf("\ncan't create thread :[%s]", strerror(err));
			pPredictionData->target = true;
		}
		else
		{
			strcat(pPredictionData->filename, RIGHT_PICTURE_AMOUNT);
			strftime(pPredictionData->time, 128, "%Y%m%d%H%M%S_RIGHT", newtime);
			strcat(pPredictionData->filename, pPredictionData->time);
			err = pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&Radar_TakePicture, pPredictionData->filename);
			if (err != 0)
				printf("\ncan't create thread :[%s]", strerror(err));
			pPredictionData->target = true;
		}

	if (pPredictionData->target)
		pPredictionData->conter++;
	return Status;
}

/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/