/**
  ******************************************************************************
  * File Name          : PreShooting.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 Joey Ke
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PRESHOOTING_H
#define PRESHOOTING_H
  /* Includes ------------------------------------------------------------------*/
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "Ta5320G_TIMER/m0All_Header.h"

#define   INT8       int8_t
#define   INT16      int16_t
#define   INT32      int32_t

#define LEFT_DETECT_DISTANCE           18
#define RIGHT_DETECT_DISTANCE          30
#define LEFT_PICTURE_AMOUNT            "3 1 "
#define RIGHT_PICTURE_AMOUNT           "3 0 "
#define SAVE_PICTURE_PATH              "rm -r /home/root/pic/"

#define PRINTLF(LF) printf("--------------------------%s--------------------------\n\r",#LF)

#define RADAR_PRINT               1
#define RADAR_PRINT_SPEED					0
#define RADAR_PRINT_XYZ	 	  			0
#define RADAR_PRINT_STATUS        1

/** @defgroup Radar_define_Error_group Error and Warning code returned by API
 *  The following DEFINE are used to identify the PAL ERROR
 *  @{
 */
typedef int8_t Radar_Error;

#define RADAR_ERROR_NONE                        ((Radar_Error)  0)
#define RADAR_ERROR_INVALID_STATU               ((Radar_Error) - 1)       /*!< Warning invalid status*/
#define RADAR_ERROR_FILE_NOT_EXIST              ((Radar_Error) - 2)       /*!< The file not exist when opening file*/
#define RADAR_ERROR_WORNG_DISTANCE              ((Radar_Error) - 3)       /*!< The X and Y distance equal 0*/
/** @} Radar_define_Error_group */


/** @defgroup Radar_PredictionStatus_group Defines Prediction Status
 *  Defines all possible prediction status mode
 *  @{
 */
typedef uint8_t Radar_PredictionStatus;

#define RADAR_PREDICTIONSTATUS_COMING        ((Radar_PredictionStatus)  0)
#define RADAR_PREDICTIONSTATUS_LEAVING       ((Radar_PredictionStatus)  1)
#define RADAR_PREDICTIONSTATUS_PARKING       ((Radar_PredictionStatus)  3)
#define RADAR_PREDICTIONSTATUS_PARKED        ((Radar_PredictionStatus)  4)
#define RADAR_PREDICTIONSTATUS_EMPTY         ((Radar_PredictionStatus)  5)
#define RADAR_PREDICTIONSTATUS_INVALID       ((Radar_PredictionStatus)  6)
/** @} Radar_PredictionStatus_group */


/**
 * @struct  Radar_ObjectSpeedData_t
 * @brief   One Speed measurement data for each target.
 *
 */
typedef struct Radar_ObjectSpeedData_t
{
  INT16 InitialDistance;
    /*!< Distance from the beginning (10cm)*/
  INT16 InitialSpeed;
    /*!< Speed from the beginning (10cm/s)*/
  INT16 DeltaX;
		/*!< Distance variation  */
  INT16 DeltaV;
    /*!< Speed variation   */ 
} Radar_ObjectSpeedData_t;

/**
 * @struct  Radar_PredictionData_t
 * 
 * @brief Structure for storing the set of prediction results
 */
typedef struct Radar_PredictionData_t
{
  Radar_ObjectSpeedData_t SpeedData;       /*!< all possible prediction status mode*/
  Radar_PredictionStatus Status;           /*!< One Speed measurement data for each target.*/
  bool target;
  int conter;
  char filename[20];
  char time[10];
} Radar_PredictionData_t;

/**
 * @brief Return the radar status
 *
 * @param   pPredictionData               all of the prediction data 
 * @param data                            for radar distance
 * @return  RADAR_ERROR_NONE              Success
 * @return  "Other error code"            See ::Radar_Error
 */
Radar_Error Radar_GetObjectStatus(
    Radar_PredictionData_t *pPredictionData,
    M0_RADAR_DATA_FRAME data);

/**
 * @brief Return Object Speed Data
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return Radar_Error 
 */
Radar_Error Radar_GetObjectSpeedData(
    Radar_PredictionData_t *pPredictionData, 
    M0_RADAR_DATA_FRAME data);

/**
 * @brief Run the take photo bash that named cap.sh in /home/root/
 * 
 */
void *Radar_TakePicture(void *parm);

/**
 * @brief Print all data of the prediction status and DeltaV and DeltaX
 * 
 * @param pPredictionData    all of the prediction data
 * @param data               for radar distance
 * @return Radar_Error 
 */
Radar_Error Radar_PrintData(
    Radar_PredictionData_t *pPredictionData,
    M0_RADAR_DATA_FRAME data);

/**
 * @brief Initial all of the data and status
 * 
 * @param pPredictionData all of the prediction data
 * @return Radar_Error 
 */
Radar_Error Radar_InitData(Radar_PredictionData_t *pPredictionData);

/**
 * @brief Clear the data detected this time 
 * 
 * @param pPredictionData 
 * @return Radar_Error 
 */
Radar_Error Radar_CleanData(Radar_PredictionData_t *pPredictionData);

/**
 * @brief delete the worng picture file
 * 
 * @param filename the floder name
 */
Radar_Error Radar_DeleteFile(char *filename);

/**
 * @brief prediction shooting
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return bool -						  prediction shooting resulte
 */
bool IsPreShoot(
    Radar_PredictionData_t *pPredictionData, 
    M0_RADAR_DATA_FRAME data);

/**
 * @brief Determine whether to take a picture 
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return Radar_Error 
 */
Radar_Error Radar_PreShoot(
    Radar_PredictionData_t *pPredictionData, 
    M0_RADAR_DATA_FRAME data);
#endif  /* PRESHOOTING_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/