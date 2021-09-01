/**
  ******************************************************************************
  * File Name          : PreShootingTest.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 Joey Ke
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PRESHOOTINGTEST_H
#define PRESHOOTINGTEST_H
  /* Includes ------------------------------------------------------------------*/
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "PreShooting.h"
#include "../Ta5320G_TIMER/m0All_Header.h"



/* Private function ---------------------------------------------------------*/

/**
 * @brief Write radar information to CSV file 
 * 
 * @param parm 		detect left or right var.
 * @return void* 
 */
void *WriteCSV(void *parm);

void GetCSVName(char *name);


#endif    /* PRESHOOTINGTEST_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/