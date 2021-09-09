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
	chdir("/home/root/Samuel/test_programs/tmp/");
	fp = fopen(filename, "a+");
	fprintf(fp, "Time, RadarLR, Distance,parking_status, Status, RadarLR, Distance, parking_status, Status");
	fclose(fp);
	chdir("/");

	while (1)
	{
		time_t now = time(NULL);
		char timestr[32];
		struct tm *newtime = localtime(&now);
		memset(timestr,'\0', 32);
		strftime(timestr, 128, "%H:%M:%S", newtime);

		chdir("/home/root/Samuel/test_programs/tmp/");
		fp = fopen(filename, "a+");
		fprintf(fp, "\n%s,%d,%d,%d,%d,%d,%d,%d,%d",
				timestr,
				M0_radarA.data.L_R,
				M0_radarA.data.obj_distance_R,
				M0_radarA.data.parking_status,
				ABData->RadarA->Status,
				
				M0_radarB.data.L_R,
				M0_radarB.data.obj_distance_R,
				M0_radarB.data.parking_status,
				ABData->RadarB->Status);

		printf("recording\n\r");
		fclose(fp);
		chdir("/");
		sleep(1);
	};

	pthread_exit(NULL);
	return 0;
}

void GetCSVName(char *name)
{
	time_t now = time(NULL);
	struct tm *newtime = localtime(&now);
	strftime(name, 128, "/home/root/Samuel/test_programs/tmp/%Y%m%d%H%M%S", newtime);
	printf("\nCreating %s.csv file\n\r", name);
	strcat(name, ".csv");
}

/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/