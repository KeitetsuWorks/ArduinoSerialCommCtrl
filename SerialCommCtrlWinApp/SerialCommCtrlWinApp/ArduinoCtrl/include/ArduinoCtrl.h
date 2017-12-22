/**
 * @file        ArduinoCtrl.h
 * @brief       シリアル通信によるArduino制御
 * @author      Keitetsu
 * @date        2017/12/02
 * @copyright   Copyright (c) 2017 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#pragma once    /* __ARDUINOCTRL_H__ */


/**
 * @typedef ARDUINO_T
 * @brief   Arduino情報構造体
 */
typedef struct Arduino_st* ARDUINO_T;


/**
 * @brief   Arduinoを開く
 * @param[in]       portName        シリアルポート名
 * @retval          NULL            異常終了
 * @retval          Others          Arduino情報構造体
 */
ARDUINO_T Arduino_open(LPCTSTR portName);


/**
 * @brief   Arduinoを閉じる
 * @param[in]       arduino         Arduino情報構造体
 * @retval          NULL            正常終了
 * @retval          Others          Arduino
 */
ARDUINO_T Arduino_close(ARDUINO_T arduino);


/**
 * @brief   Arduino制御のメインループ処理を実行する
 * @param[in]       arduino         Arduino情報構造体
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
int Arduino_execMainLoop(ARDUINO_T arduino);
