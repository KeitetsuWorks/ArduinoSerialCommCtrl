/**
 * @file        SerialPort.h
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2014/08/10
 * @copyright   Copyright (c) 2014 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#pragma once    /* __SERIALPORT_H__ */


/**
 * @typedef SERIALPORT_T
 * @brief   シリアルポート情報構造体
 */
typedef struct SerialPort_st* SERIALPORT_T;


/**
 * @brief   シリアルポートを開く
 * @param[in]       portName        シリアルポート名
 * @param[in]       portBaudRate    ボーレート
 * @retval          NULL            異常終了
 * @retval          Others          シリアルポート情報構造体
 */
SERIALPORT_T SerialPort_open(LPCTSTR portName, unsigned int portBaudRate);


/**
 * @brief   シリアルポートを閉じる
 * @param[in]       port            シリアルポート情報構造体
 * @retval          NULL            正常終了
 * @retval          Others          異常終了
 */
SERIALPORT_T SerialPort_close(SERIALPORT_T port);


/**
 * @brief   シリアルポートからデータを読込む
 * @param[in]       port            シリアルポート情報構造体
 * @param[out]      portReadData    データバッファのポインタ
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
int SerialPort_read(SERIALPORT_T port, void *portReadData);


/**
 * @brief   シリアルポートにデータを書込む
 * @param[in]       port            シリアルポート情報構造体
 * @param[in]       portWriteData   データバッファのポインタ
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
int SerialPort_write(SERIALPORT_T port, void *portWriteData);
