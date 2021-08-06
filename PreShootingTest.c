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

void *KBhit(void *parm)
{
	int err;
	while (true)
	{
		
		fgets(str, 10, stdin);
		if (str[0] == 'l')
			pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&WriteCSV, (void *)1);
		else if (str[0] == 'r')
			pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&WriteCSV, (void *)0);
		else if (str[0] == 'c'){
			err = pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&Radar_TakePicture, "3 1 test");
			if (err<0) printf("error: %s", strerror(errno));
		}
			
	}
}

void *WriteCSV(void *parm)
{
	int LF = (int *)parm;
	char filename[128];
	FILE *fp;
	M0_RADAR_DATA_FRAME data;

	GetCSVName(filename);
	fp = fopen(filename, "w+");

	fprintf(fp, "RadarLR, X, Y, Z, Distance, Power");

	while (str[0] != 's')
	{
		data = LF ? M0_radarA.data : M0_radarB.data;

		fprintf(fp, "\n%d,%d,%d,%d,%d,%d",
				data.L_R,
				data.obj_position_X,
				data.obj_position_Y,
				data.obj_position_Z,
				data.obj_distance_R,
				data.power);

		printf("recording\n\r");
		sleep(1);
	};

	fclose(fp);
	printf("\n %sfile created\n\r", filename);
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