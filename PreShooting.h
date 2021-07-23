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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "Ta5320G_TIMER/m0All_Header.h"

#define   INT8       int8_t
#define   INT16      int16_t
#define   INT32      int32_t

#define PRINTLF(LF) printf("--------------------------#LF--------------------------\n\r")

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
#define RADAR_PREDICTIONSTATUS_PARKING       ((Radar_PredictionStatus)  1)
#define RADAR_PREDICTIONSTATUS_PARKED        ((Radar_PredictionStatus)  2)
#define RADAR_PREDICTIONSTATUS_LEAVING       ((Radar_PredictionStatus)  3)
#define RADAR_PREDICTIONSTATUS_INVALID       ((Radar_PredictionStatus)  4)
/** @} Radar_PredictionStatus_group */


/** @defgroup Radar_ObjectAreaStatus_group Defines Object in Area Status
 *  Determine whether it is within the area
 *  @{
 */
typedef int8_t Radar_ObjectAreaStatus;

#define RADAR_OBJECTAREASTATUS_OUT           ((Radar_ObjectAreaStatus)  0)    /*!< Nothing in the area*/
#define RADAR_OBJECTAREASTATUS_IN            ((Radar_ObjectAreaStatus)  1)    /*!< Something in the area*/
/** @} Radar_ObjectAreaStatus_group */

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
} Radar_PredictionData_t;

/**
 * @brief Return the radar status
 *
 * @param   pPredictionData               all of the prediction data 
 * @return  RADAR_ERROR_NONE              Success
 * @return  "Other error code"            See ::Radar_Error
 */
Radar_Error Radar_GetObjectStatus(Radar_PredictionData_t *pPredictionData);

/**
 * @brief Return Object Speed Data
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return Radar_Error 
 */
Radar_Error Radar_GetObjectSpeedData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data);

/**
 * @brief Run the take photo bash that named cap.sh in /home/root/
 * 
 * @return  RADAR_ERROR_NONE                Success
 * @return  RADAR_ERROR_FILE_NOT_EXIST      File is not exist
 */
Radar_Error Radar_TakePicture(void);

/**
 * @brief Print all data of the prediction status and DeltaV and DeltaX
 * 
 * @param pPredictionData    all of the prediction data
 * @param data               for radar distance
 * @return Radar_Error 
 */
Radar_Error Radar_PrintData(Radar_PredictionData_t *pPredictionData, M0_RADAR_DATA_FRAME data);

/**
 * @brief Initial all of the data and status
 * 
 * @param pPredictionData all of the prediction data
 * @return Radar_Error 
 */
Radar_Error Radar_InitData(Radar_PredictionData_t *pPredictionData);
#endif  /* PRESHOOTING_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/