
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

int main(int argc, char *argv[])
{
	/* USER CODE BEGIN PV */
	/* Private variables ---------------------------------------------------------*/
	Radar_PredictionData_t PredictionDataA, PredictionDataB;
	Radar_PredictionData_t *pPredictionDataA = &PredictionDataA;
	Radar_PredictionData_t *pPredictionDataB = &PredictionDataB;
	Radar_Error Status;
	int target = 0;
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
	sleep(2); //wait for ladar load
	void *ret; // 子執行緒傳回值
	while (Status == RADAR_ERROR_NONE) //main loop
	{
		Status = Radar_GetObjectSpeedData(pPredictionDataA, M0_radarA.data);
		Status = Radar_GetObjectStatus(pPredictionDataA, M0_radarA.data);
		Status = Radar_PrintData(pPredictionDataA, M0_radarA.data);
		if (PredictionDataA.Status == RADAR_PREDICTIONSTATUS_COMING && target == 0)
		{
			pthread_create(&thread_uartA53M0_Tx, NULL, Radar_TakePicture, NULL);
			//Status = Radar_TakePicture();
			target++;
		}
		else if (PredictionDataA.Status == RADAR_PREDICTIONSTATUS_EMPTY)
			target = 0;

		sleep(1);
	}

	printf("Error code: %d\n\r", Status);
	return 0;
}
