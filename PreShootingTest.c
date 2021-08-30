/**
  ******************************************************************************
  * File Name          : PreShootingTest.h
  * Description        : All the test functinos
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 Joey Ke
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "PreShootingTest.h"

void *WriteCSV(void *parm)
{
	Radar_ABData_t *ABData;
	ABData = (Radar_ABData_t *)parm;
	
	char filename[128];
	FILE *fp;
	M0_RADAR_DATA_FRAME data;

	GetCSVName(filename);
	fp = fopen(filename, "a+");
	fprintf(fp, "RadarLR, Distance, Status, RadarLR, Distance, Status");
	fclose(fp);

	while (1)
	{
		fp = fopen(filename, "a+");
		fprintf(fp, "\n%d,%d,%d,%d,%d,%d",
				M0_radarA.data.L_R,
				M0_radarA.data.obj_distance_R,
				ABData->RadarA->Status,
				
				M0_radarB.data.L_R,
				M0_radarB.data.obj_distance_R,
				ABData->RadarB->Status);

		printf("recording\n\r");
		fclose(fp);
		sleep(1);
	};

	pthread_exit(NULL);
	return 0;
}

void GetCSVName(char *name)
{
	time_t now = time(NULL);
	struct tm *newtime = localtime(&now);
	strftime(name, 128, "%Y%m%d%H%M%S", newtime);
	printf("\nCreating %s.csv file\n\r", name);
	strcat(name, ".csv");
}

/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/