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
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "PreShooting.h"
#include "Ta5320G_TIMER/m0All_Header.h"

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

#endif    /* PRESHOOTINGTEST_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/