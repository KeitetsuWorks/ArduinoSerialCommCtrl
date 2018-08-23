/**
 * @file        ArduinoCtrl.c
 * @brief       シリアル通信によるArduino制御
 * @author      Keitetsu
 * @date        2017/12/02
 * @copyright   Copyright (c) 2017 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>

#include "SerialPort.h"
#include "ArduinoCtrl.h"


/**
 * @name    キー
 */
/** @{ */
#define KEY_ESC             0x1B    /**< ESCキー */
/** @} */

/**
 * @name    コマンド
 */
/** @{ */
#define CMD_SIZE            2       /**< コマンドサイズ */
#define CMD_TYPE            0       /**< コマンド構成データ: コマンド種類 */
#define CMD_PIN             1       /**< コマンド構成データ: ピン番号 */
#define CMD_WRITE_LOW       0       /**< コマンド種類: LOW書込み */
#define CMD_WRITE_HIGH      1       /**< コマンド種類: HIGH書込み */
#define CMD_WRITE_TOGGLE    2       /**< コマンド種類: ピン状態切替え */
#define CMD_READ            4       /**< コマンド種類: ピン状態読込み */
/** @} */

/**
 * @name    レスポンス
 */
/** @{ */
#define RES_SIZE            2       /**< レスポンスサイズ */
#define RES_PIN             0       /**< レスポンス構成データ: ピン番号 */
#define RES_PIN_VALUE       1       /**< レスポンス構成データ: ピン状態 */
/** @} */

/**
 * @name    ピンアサイン
 */
/** @{ */
#define PIN_LED             13      /**< ピン番号: LED */
/** @} */

/**
 * @name    ピン種類
 */
/** @{ */
#define PIN_TYPE_LED        0       /**< ピン種類: LED */
/** @} */

/**
 * @name    LED
 */
/** @{ */
#define LED_OFF             0       /**< LED: 消灯 */
#define LED_ON              1       /**< LED: 点灯 */
/** @} */


/**
 * @struct  Arduino_st
 * @brief   Arduino情報構造体
 */
struct Arduino_st {
    SERIALPORT_T port;  /**< シリアルポート情報構造体 */
    LPCTSTR portName;   /**< シリアルポート名 */
};


/**
 * @brief   Arduino制御コマンドを実行する
 * @param[in]       arduino         Arduino情報構造体
 * @param[in]       pin             ピン番号
 * @param[in]       pinType         ピン種類
 * @param[in]       cmdType         コマンド種類
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
static int Arduino_execCmd(ARDUINO_T arduino, uint8_t pin, unsigned int pinType, uint8_t cmdType);


/**
 * @brief   LEDの状態を表示する
 * @param[in]       res             レスポンス
 */
static void Arduino_printLEDStatus(uint8_t *res);


ARDUINO_T Arduino_open(LPCTSTR portName)
{
    ARDUINO_T arduino;

    /* インスタンスの生成 */
    arduino = (ARDUINO_T)malloc(sizeof(struct Arduino_st));
    if (arduino == NULL) {
        return NULL;
    }
    ZeroMemory(arduino, sizeof(struct Arduino_st));
    arduino->portName = portName;

    /* シリアルポートを開く */
    arduino->port = SerialPort_open(arduino->portName, 9600);
    if (arduino->port == NULL) {
        free(arduino);
        return NULL;
    }

    return arduino;
}


ARDUINO_T Arduino_close(ARDUINO_T arduino)
{
    if (arduino != NULL) {
        SerialPort_close(arduino->port);
        free(arduino);
        arduino = NULL;
    }

    return arduino;
}


int Arduino_execMainLoop(ARDUINO_T arduino)
{
    int loop;
    int key;
    int retVal;
    int result;

    loop = TRUE;
    result = TRUE;

    _tprintf(_T("スペースキー押下でLEDの点灯・消灯と切り替えます\n"));
    _tprintf(_T("ESCキー押下でプログラムを終了します\n"));
    while (loop) {
        if (_kbhit() != 0) {
            key = _getch();
            switch (key) {
            case KEY_ESC:
                _tprintf(_T("ESCキーが押下されました\n"));
                loop = FALSE;
                break;
            case 'l':
            case 'L':
                retVal = Arduino_execCmd(arduino, PIN_LED, PIN_TYPE_LED, CMD_WRITE_LOW);
                if (retVal == FALSE) {
                    loop = FALSE;
                    result = FALSE;
                }
                break;
            case 'h':
            case 'H':
                retVal = Arduino_execCmd(arduino, PIN_LED, PIN_TYPE_LED, CMD_WRITE_HIGH);
                if (retVal == FALSE) {
                    loop = FALSE;
                    result = FALSE;
                }
                break;
            case ' ':
                retVal = Arduino_execCmd(arduino, PIN_LED, PIN_TYPE_LED, CMD_WRITE_TOGGLE);
                if (retVal == FALSE) {
                    loop = FALSE;
                    result = FALSE;
                }
                break;
            case 'r':
            case 'R':
                retVal = Arduino_execCmd(arduino, PIN_LED, PIN_TYPE_LED, CMD_READ);
                if (retVal == FALSE) {
                    loop = FALSE;
                    result = FALSE;
                }
                break;
            default:
                break;
            }
        }
    }
    _tprintf(_T("プログラムを終了しました\n"));

    return result;
}


static int Arduino_execCmd(ARDUINO_T arduino, uint8_t pin, unsigned int pinType, uint8_t cmdType)
{
    uint8_t cmd[CMD_SIZE];
    uint8_t res[RES_SIZE];
    int retVal;
    int result;

    result = TRUE;

    ZeroMemory(cmd, sizeof(cmd));
    ZeroMemory(res, sizeof(res));

    /* コマンド送信 */
    cmd[CMD_TYPE] = cmdType;
    cmd[CMD_PIN] = pin;
    retVal = SerialPort_write(arduino->port, cmd);
    if (retVal == FALSE) {
        _tprintf(_T("送信エラーが発生しました\n"));
        result = FALSE;
    }
    else {
        /* レスポンス受信 */
        retVal = SerialPort_read(arduino->port, res);
        if (retVal == FALSE) {
            _tprintf(_T("受信エラーが発生しました\n"));
            result = FALSE;
        }
        else {
            /* レスポンス表示 */
            switch (pinType) {
            case PIN_TYPE_LED:
                Arduino_printLEDStatus(res);
                break;
            default:
                _tprintf(_T("ピン番号: %d, ピン状態: %d\n"), res[RES_PIN], res[RES_PIN_VALUE]);
                break;
            }
        }
    }

    return result;
}


static void Arduino_printLEDStatus(uint8_t *res)
{
    _tprintf(_T("ピン番号: %d, ピン状態: %d, LED: "), res[RES_PIN], res[RES_PIN_VALUE]);
    switch (res[RES_PIN_VALUE]) {
    case LED_OFF:
        _tprintf(_T("消灯"));
        break;
    case LED_ON:
        _tprintf(_T("点灯"));
        break;
    default:
        _tprintf(_T("状態不明"));
        break;
    }
    _tprintf(_T("\n"));

    return;
}
