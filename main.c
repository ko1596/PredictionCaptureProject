
/*
 * Copyright (C) Samuel Chiang
 *
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "Ta5320G_TIMER/m0All_Header.h"
#include "PreShooting.h"

/* Global variables ---------------------------------------------------------*/
char str[1]; //for the KBhit to save detect keyboard event character

/* Private function ---------------------------------------------------------*/
/**
 * @brief scan the keyboard event to create the thread
 * 
 * @param parm 		NONE
 * @return void* 
 */
void *KBhit(void *parm);

/**
 * @brief Write radar information to CSV file 
 * 
 * @param parm 		detect left or right var.
 * @return void* 
 */
void *WriteCSV(void *parm);

/**
 * @brief Use today's date as the file name 
 * 
 * @param name the filename ptr
 */
void GetCSVName(char *name);

/**
 * @brief prediction shooting
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return bool -						  prediction shooting resulte
 */
bool IsPreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data);



int main(int argc, char *argv[])
{
	/* USER CODE BEGIN PV */
	/* Private variables ---------------------------------------------------------*/
	Radar_PredictionData_t PredictionDataA, PredictionDataB;
	Radar_PredictionData_t *pPredictionDataA = &PredictionDataA;
	Radar_PredictionData_t *pPredictionDataB = &PredictionDataB;
	Radar_Error Status;
	int targetL = 0;
	int targetR = 0;
	int conterL = 0;
	int conterR = 0;
	char filenamel[20];
	char filenamer[20];
	char timeL[10];
	char timeR[10];
	int err;
	time_t now = time(NULL);
	struct tm *newtime = localtime(&now);
	/* USER CODE END PV */

	/* Initialize all parameters */
	Status = RADAR_ERROR_NONE;
	Status = Radar_InitData(pPredictionDataA);
	Status = Radar_InitData(pPredictionDataB);

	time_t rawtime;
	struct tm *info;
	char buffer[80];

	time(&rawtime);
	info = localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
	printf("\n%s\n", buffer);

	fd_uartA53M0 = open(dev_UART_A53M0, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd_uartA53M0 == -1)
	{
		printf("[%s] xxxx   open Error   xxxx \n", dev_UART_A53M0);
		return -1;
	}
	else
	{
		printf("[%s] Open Success ============\n", dev_UART_A53M0);

		if (flock(fd_uartA53M0, LOCK_EX | LOCK_NB) == 0)
		{
			struct termios uart_settings;
			tcgetattr(fd_uartA53M0, &uart_settings);
			bzero(&uart_settings, sizeof(uart_settings));
			uart_settings.c_cflag = UARTA53M0_SPEED | CS8 | CLOCAL | CREAD;
			uart_settings.c_iflag = IGNPAR;
			uart_settings.c_oflag = 0;
			uart_settings.c_lflag = 0;
			uart_settings.c_cc[VTIME] = 0;
			uart_settings.c_cc[VMIN] = 1;

			cfsetispeed(&uart_settings, UARTA53M0_SPEED);
			cfsetospeed(&uart_settings, UARTA53M0_SPEED);
			tcsetattr(fd_uartA53M0, TCSANOW, &uart_settings);
		}
		else
		{
			printf("XXXXXXXX [%s] was locked by other process.\n", dev_UART_A53M0);
			return -1;
		}
	}

	pthread_attr_t attr_Tx, attr_Rx;
	int policy_Rx;
	int policy_Tx;
	struct sched_param ached_Rx_params;
	struct sched_param ached_Tx_params;

	pthread_attr_init(&attr_Rx);

	pthread_create(&thread_uartA53M0_Rx, &attr_Rx, (void *)&uartA53M0_Rx_thread, &fd_uartA53M0);

	pthread_attr_getschedpolicy(&attr_Rx, &policy_Rx);
	switch (policy_Rx)
	{
	case SCHED_FIFO:
		printf(" policy Rx SCHED_FIFO\n");
		break;
	case SCHED_RR:
		printf(" policy Rx SCHED_RR\n");
		break;
	case SCHED_OTHER:
		printf(" policy Rx SCHED_OTHER\n");
		break;
	default:
		printf(" policy Rx XXX\n");
		break;
	}

	pthread_attr_init(&attr_Tx);

	pthread_create(&thread_uartA53M0_Tx, &attr_Tx, (void *)&uartA53M0_Tx_thread, &fd_uartA53M0);

	pthread_attr_getschedpolicy(&attr_Tx, &policy_Tx);

	switch (policy_Rx)
	{
	case SCHED_FIFO:
		printf(" policy Tx SCHED_FIFO\n");
		break;
	case SCHED_RR:
		printf(" policy Tx SCHED_RR\n");
		break;
	case SCHED_OTHER:
		printf(" policy Tx SCHED_OTHER\n");
		break;

	default:
		printf(" policy Tx XXX\n");
		break;
	}

	uartA53M0_SetM0_currentRTC();

	printf("FOR Test!!!\n");

	while (Status == RADAR_ERROR_NONE) //main loop
	{
		//system("clear");
		pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&KBhit, NULL);

		Status = Radar_GetObjectSpeedData(pPredictionDataA, M0_radarA.data);
		Status = Radar_GetObjectStatus(pPredictionDataA, M0_radarA.data);
		Status = Radar_PrintData(pPredictionDataA, M0_radarA.data);

		Status = Radar_GetObjectSpeedData(pPredictionDataB, M0_radarB.data);
		Status = Radar_GetObjectStatus(pPredictionDataB, M0_radarB.data);
		Status = Radar_PrintData(pPredictionDataB, M0_radarB.data);

		Status = Radar_CleanData(pPredictionDataA);
		Status = Radar_CleanData(pPredictionDataB);

		if (IsPreShoot(pPredictionDataA, M0_radarA.data) && targetL == 0)
		{
			sleep(2);
			strcat(filenamel, "3 1 ");
			strftime(timeL, 128, "%Y%m%d%H%M%S_LEFT", newtime);
			strcat(filenamel, timeL);
			err = pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&Radar_TakePicture, filenamel);
			if (err != 0) printf("\ncan't create thread :[%s]", strerror(err));
			targetL++;
		}

		if (IsPreShoot(pPredictionDataB, M0_radarB.data) && targetR == 0)
		{
			strcat(filenamer, "3 0 ");
			strftime(timeR, 128, "%Y%m%d%H%M%S_RIGHT", newtime);
			strcat(filenamer, timeR);
			err = pthread_create(&thread_uartA53M0_Tx, NULL, (void *)&Radar_TakePicture, filenamer);
			if (err != 0) printf("\ncan't create thread :[%s]", strerror(err));
			targetR++;
		}

		if (targetL>0) conterL++;
		if (targetR>0) conterR++;


		sleep(1);
	}

	return 0;
}

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

bool IsPreShoot(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data)
{
	if(data.L_R == 0)
		return pPredictionData->Status == (RADAR_PREDICTIONSTATUS_PARKING || RADAR_PREDICTIONSTATUS_COMING) && data.obj_distance_R < 30;
	else
		return pPredictionData->Status == (RADAR_PREDICTIONSTATUS_PARKING || RADAR_PREDICTIONSTATUS_COMING) && data.obj_distance_R < 18;
}

