/**
 * @file        SerialPort.c
 * @brief       �V���A���|�[�g�ʐM���C�u����
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
 * @brief   �V���A���|�[�g���\����
 */
struct SerialPort_st {
    HANDLE handle;  /*!< �V���A���|�[�g�̃n���h�� */
    LPCTSTR name;   /*!< �V���A���|�[�g�� */
};


SERIALPORT_T SerialPort_open(LPCTSTR portName, unsigned int portBaudRate)
{
    SERIALPORT_T port;
    DCB portDCB;
    COMMTIMEOUTS portTimeouts;
    BOOL retVal;

    /* �C���X�^���X�̐��� */
    port = (SERIALPORT_T)malloc(sizeof(struct SerialPort_st));
    if (port == NULL) {
        return NULL;
    }
    ZeroMemory(port, sizeof(struct SerialPort_st));
    port->name = portName;

    /* �V���A���|�[�g���J�� */
    port->handle = CreateFile(
        port->name,                     /* �V���A���|�[�g�� */
        GENERIC_READ | GENERIC_WRITE,   /* �A�N�Z�X���[�h */
        0,                              /* ���L���[�h */
        0,                              /* �Z�L�����e�B�L�q�q */
        OPEN_EXISTING,                  /* �쐬���@ */
        0,                              /* �t�@�C������ */
        0                               /* �e���v���[�g�t�@�C���̃n���h�� */
    );
    if (port->handle == INVALID_HANDLE_VALUE) {
        free(port);
        return NULL;
    }

    /* ����M�o�b�t�@�T�C�Y��ݒ� */
    retVal = SetupComm(
        port->handle,   /* �V���A���|�[�g�̃n���h�� */
        8192,           /* ��M�o�b�t�@�T�C�Y */
        8192            /* ���M�o�b�t�@�T�C�Y */
    );
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    /* ����M�o�b�t�@�������� */
    retVal = PurgeComm(
        port->handle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retVal == FALSE) {
        free(port);
        return NULL;
    }

    /* �V���A���|�[�g�̐���ݒ� */
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

    /* �^�C���A�E�g�p�����[�^�̐ݒ� */
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
    DWORD portErrors;       /* �G���[�R�[�h */
    COMSTAT portComStat;    /* �ʐM��ԃo�b�t�@ */
    DWORD portReadBytes;    /* �V���A���|�[�g����Ǎ��񂾃o�C�g�� */
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
    DWORD portErrors;       /* �G���[�R�[�h */
    COMSTAT portComStat;    /* �ʐM��ԃo�b�t�@ */
    DWORD portWrittenBytes; /* �V���A���|�[�g�ɏ����񂾃o�C�g�� */
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
