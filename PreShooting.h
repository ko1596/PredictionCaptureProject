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

  /* Global Var ------------------------------------------------------------------*/
Radar_PredictionData_t PredictionDataA, PredictionDataB;
Radar_PredictionData_t *pPredictionDataA, *pPredictionDataB;




/* Use function beginning --------------------------------------------------------*/

/**
 * @brief Initial all of the data and status
 * 
 * @return Radar_Error 
 */
Radar_Error Radar_Initialization(void);

/**
 * @brief run all the preshooting functions
 *
 * @return  RADAR_ERROR_NONE              Success
 * @return  "Other error code"            See ::Radar_Error
 */
Radar_Error Radar_Hanlder(void);

/**
 * @brief Whether to transmit camera signal
 * 
 * @return int 
 */
int IsCapture(M0_RADAR_FRAME *radar_data);

/**
 * @brief Whether to transfer deleted files
 * 
 * @return int 
 */
int IsDelete(M0_RADAR_FRAME *radar_data);

/* Use function end ------------------------------------------------------------------*/

















/* Private function start ------------------------------------------------------------------*/
/**
 * @brief Return the radar status
 *
 * @param   pPredictionData               all of the prediction data 
 * @param   data                          for radar distance
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
Radar_Error Radar_InitPredictionData(Radar_PredictionData_t *pPredictionData);

/**
 * @brief Clear the data detected this time 
 * 
 * @param pPredictionData 
 * @return Radar_Error 
 */
Radar_Error Radar_CleanData(Radar_PredictionData_t *pPredictionData);

/**
 * @brief prediction shooting
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return int -						  prediction shooting resulte
 */
int IsPreShoot(
    Radar_PredictionData_t *pPredictionData, 
    M0_RADAR_DATA_FRAME data);

/**
 * @brief Determine whether to take a picture 
 * 
 * @param pPredictionData                 all of the prediction data 
 * @param data                            for radar distance
 * @return Radar_Error 
 */
int Radar_PreShoot(
    Radar_PredictionData_t *pPredictionData, 
    M0_RADAR_DATA_FRAME data);
/* Private function end ------------------------------------------------------------------*/

#endif  /* PRESHOOTING_H */
/************************ (C) COPYRIGHT Joey Ke *****END OF FILE****/