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
//#include "PreShooting_header.h"
#include "All_Header.h"


int IsCapture(M0_RADAR_FRAME *radar_data)
{
	return radar_data->data.L_R ? Radar_PreShoot(pPredictionDataA, radar_data->data) : Radar_PreShoot(pPredictionDataB, radar_data->data);
}

int IsDelete(M0_RADAR_FRAME *radar_data)
{
	return radar_data->data.L_R ? PredictionDataA.delete_file_flag : PredictionDataB.delete_file_flag;
}

Radar_Error Radar_Hanlder(void)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	Status = Radar_GetObjectSpeedData(pPredictionDataA, M0_AllDevStatus.radar_A.data);
	Status = Radar_GetObjectSpeedData(pPredictionDataB, M0_AllDevStatus.radar_B.data);

	Status = Radar_GetObjectStatus(pPredictionDataA, M0_AllDevStatus.radar_A.data);
	Status = Radar_GetObjectStatus(pPredictionDataB, M0_AllDevStatus.radar_B.data);	

	Status = Radar_CleanData(pPredictionDataA);
	Status = Radar_CleanData(pPredictionDataB);

	Status = Radar_PrintData(pPredictionDataA, M0_AllDevStatus.radar_A.data);
	Status = Radar_PrintData(pPredictionDataB, M0_AllDevStatus.radar_B.data);
    return Status;
}

Radar_Error Radar_GetObjectStatus(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
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

Radar_Error Radar_PrintData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
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
			data.power);
	#endif

	printf("--------------------------------------------------------\n\r");
	#endif
	return Status;
}

Radar_Error Radar_InitPredictionData(Radar_PredictionData_t *pPredictionData)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionData->SpeedData.DeltaV = 0;
	pPredictionData->SpeedData.DeltaX = 0;
	pPredictionData->SpeedData.InitialDistance = 0;
	pPredictionData->SpeedData.InitialSpeed = 0;
	pPredictionData->Status = RADAR_PREDICTIONSTATUS_INVALID;
	pPredictionData->target = 0;
	pPredictionData->delete_file_flag = 0;
	pPredictionData->conter = 0;
	memset(pPredictionData->filename, 0, 30);
	return Status;
}

Radar_Error Radar_Initialization(void)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionDataA = &PredictionDataA;
	pPredictionDataB = &PredictionDataB;
	Status = Radar_InitPredictionData(pPredictionDataA);
	Status = Radar_InitPredictionData(pPredictionDataB);
	return Status;
}

Radar_Error Radar_CleanData(Radar_PredictionData_t *pPredictionData)
{
	Radar_Error Status = RADAR_ERROR_NONE;
	pPredictionData->delete_file_flag = 0;
	if (pPredictionData->Status == RADAR_PREDICTIONSTATUS_EMPTY)
	{
		pPredictionData->target = 0;
		if(pPredictionData->conter > 0 && pPredictionData->conter <= 4)
			pPredictionData->conter++;
		else if (pPredictionData->conter > 4 && pPredictionData->conter < 10)
		{
			pPredictionData->conter = 0;
			pPredictionData->delete_file_flag = 1;
		}
		else
		{
			pPredictionData->conter = 0;
		}

		memset(pPredictionData->filename, 0, 30);
	}

	return Status;
}


int IsPreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	if (pPredictionData->Status == RADAR_PREDICTIONSTATUS_PARKING || pPredictionData->Status == RADAR_PREDICTIONSTATUS_COMING)
		return data.L_R ?  data.obj_distance_R < LEFT_DETECT_DISTANCE : data.obj_distance_R < RIGHT_DETECT_DISTANCE;
	else
		return 0;
}

int Radar_PreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	if (IsPreShoot(pPredictionData, data) && pPredictionData->target == 0)
	{
		pPredictionData->target = 1;
		return 1;
	}
		
	if (pPredictionData->target)
		pPredictionData->conter++;
		
	return 0;
}

/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/