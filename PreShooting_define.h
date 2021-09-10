/**
  ******************************************************************************
  * File Name          : PreShooting_define.h
  * Description        : This file contains all the pre shooting definess
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 Joey Ke
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PRESHOOTING_DEFINE_H
#define PRESHOOTING_DEFINE_H


#define   INT8       int8_t
#define   INT16      int16_t
#define   INT32      int32_t

#define LEFT_DETECT_DISTANCE           15
#define RIGHT_DETECT_DISTANCE          30

#define PRINTLF(LF) printf("--------------------------%s--------------------------\n\r",#LF)

#define RADAR_PRINT               1
#define RADAR_PRINT_SPEED					1
#define RADAR_PRINT_XYZ	 	  			1
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
  Radar_ObjectSpeedData_t SpeedData;       /*!< all possible prediction status mode>*/
  Radar_PredictionStatus Status;           /*!< One Speed measurement data for each target.*/
  int target;                             /*!< Start capture thread.>*/
  int delete_file_flag;                   /*!< Sent delete command to A53.>*/
  int conter;                              /*!< Calculate the time after startup.*/
  char filename[30];                       /*!< File full path and file name*/
} Radar_PredictionData_t;



#endif  /* PRESHOOTING_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/