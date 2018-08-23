/**
 * @file        SerialPort.c
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2014/08/10
 * @copyright   Copyright (c) 2014 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <windows.h>

#include "SerialPort.h"


/**
 * @struct  SerialPort_st
 * @brief   シリアルポート情報構造体
 */
struct SerialPort_st {
    HANDLE handle;  /**< シリアルポートのハンドラ */
    LPCTSTR name;   /**< シリアルポート名 */
};


SERIALPORT_T SerialPort_open(LPCTSTR portName, unsigned int portBaudRate)
{
    SERIALPORT_T port;
    DCB portDCB;
    COMMTIMEOUTS portTimeouts;
    BOOL retVal;

    /* インスタンスの生成 */
    port = (SERIALPORT_T)malloc(sizeof(struct SerialPort_st));
    if (port == NULL) {
        return NULL;
    }
    ZeroMemory(port, sizeof(struct SerialPort_st));
    port->name = portName;

    /* シリアルポートを開く */
    port->handle = CreateFile(
        port->name,                     /* シリアルポート名 */
        GENERIC_READ | GENERIC_WRITE,   /* アクセスモード */
        0,                              /* 共有モード */
        0,                              /* セキュリティ記述子 */
        OPEN_EXISTING,                  /* 作成方法 */
        0,                              /* ファイル属性 */
        0                               /* テンプレートファイルのハンドル */
    );
    if (port->handle == INVALID_HANDLE_VALUE) {
        free(port);
        return NULL;
    }

    /* 送受信バッファサイズを設定 */
    retVal = SetupComm(
        port->handle,   /* シリアルポートのハンドラ */
        8192,           /* 受信バッファサイズ */
        8192            /* 送信バッファサイズ */
    );
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    /* 送受信バッファを初期化 */
    retVal = PurgeComm(
        port->handle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    /* シリアルポートの制御設定 */
    GetCommState(port->handle, &portDCB);
    portDCB.BaudRate = portBaudRate;
    portDCB.ByteSize = 8;
    portDCB.Parity = NOPARITY;
    portDCB.StopBits = ONESTOPBIT;
    portDCB.fBinary = TRUE;
    portDCB.fParity = FALSE;
    portDCB.fOutxCtsFlow = FALSE;
    portDCB.fOutxDsrFlow = FALSE;
    portDCB.fAbortOnError = TRUE;
    retVal = SetCommState(port->handle, &portDCB);
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    /* タイムアウトパラメータの設定 */
    GetCommTimeouts(port->handle, &portTimeouts);
    portTimeouts.ReadIntervalTimeout = 100;
    portTimeouts.ReadTotalTimeoutMultiplier = 10;
    portTimeouts.ReadTotalTimeoutConstant = 100;
    portTimeouts.WriteTotalTimeoutMultiplier = 10;
    portTimeouts.WriteTotalTimeoutConstant = 100;
    retVal = SetCommTimeouts(port->handle, &portTimeouts);
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    return port;
}


SERIALPORT_T SerialPort_close(SERIALPORT_T port)
{
    if (port->handle != INVALID_HANDLE_VALUE) {
        CloseHandle(port->handle);
        free(port);
        port = NULL;
    }

    return port;
}


int SerialPort_read(SERIALPORT_T port, void *portReadData)
{
    DWORD portErrors;       /* エラーコード */
    COMSTAT portComStat;    /* 通信状態バッファ */
    DWORD portReadBytes;    /* シリアルポートから読込んだバイト数 */
    BOOL retVal;
    int result;

    result = TRUE;

    if (port->handle != INVALID_HANDLE_VALUE) {
        ClearCommError(port->handle, &portErrors, &portComStat);
        retVal = ReadFile(port->handle, portReadData, sizeof(portReadData), &portReadBytes, NULL);
        if (retVal == FALSE) {
            result = FALSE;
        }
    }
    else {
        result = FALSE;
    }

    return result;
}


int SerialPort_write(SERIALPORT_T port, void *portWriteData)
{
    DWORD portErrors;       /* エラーコード */
    COMSTAT portComStat;    /* 通信状態バッファ */
    DWORD portWrittenBytes; /* シリアルポートに書込んだバイト数 */
    BOOL retVal;
    int result;

    result = TRUE;

    if (port->handle != INVALID_HANDLE_VALUE) {
        ClearCommError(port->handle, &portErrors, &portComStat);
        retVal = WriteFile(port->handle, portWriteData, sizeof(portWriteData), &portWrittenBytes, NULL);
        if (retVal == FALSE) {
            result = FALSE;
        }
    }
    else {
        result = FALSE;
    }

    return result;
}
