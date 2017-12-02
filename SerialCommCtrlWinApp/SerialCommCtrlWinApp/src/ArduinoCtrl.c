/**
 * @file        ArduinoCtrl.c
 * @brief       �V���A���ʐM�ɂ��Arduino����
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
 * @name    �L�[
 */
/*! @{ */
#define KEY_ESC             0x1B    /*!< ESC�L�[ */

/**
 * @name    �R�}���h
 */
/*! @{ */
#define CMD_SIZE            2       /*!< �R�}���h�T�C�Y */
#define CMD_TYPE            0       /*!< �R�}���h�\���f�[�^: �R�}���h��� */
#define CMD_PIN             1       /*!< �R�}���h�\���f�[�^: �s���ԍ� */
#define CMD_WRITE_LOW       0       /*!< �R�}���h���: LOW������ */
#define CMD_WRITE_HIGH      1       /*!< �R�}���h���: HIGH������ */
#define CMD_WRITE_TOGGLE    2       /*!< �R�}���h���: �s����Ԑؑւ� */
#define CMD_READ            4       /*!< �R�}���h���: �s����ԓǍ��� */
/*! @} */

/**
 * @name    ���X�|���X
 */
/*! @{ */
#define RES_SIZE            2       /*!< ���X�|���X�T�C�Y */
#define RES_PIN             0       /*!< ���X�|���X�\���f�[�^: �s���ԍ� */
#define RES_PIN_VALUE       1       /*!< ���X�|���X�\���f�[�^: �s����� */
/*! @} */

/**
 * @name    �s���A�T�C��
 */
/*! @{ */
#define PIN_LED             13      /*!< �s���ԍ�: LED */
/*! @} */

/**
 * @name    �s�����
 */
/*! @{ */
#define PIN_TYPE_LED        0      /*!< �s�����: LED */
/*! @} */

/**
 * @name    LED
 */
/*! @{ */
#define LED_OFF             0       /*!< LED: ���� */
#define LED_ON              1       /*!< LED: �_�� */
/*! @} */


/**
 * @typedef Arduino_st
 * @brief   Arduino���\����
 */
struct Arduino_st {
    SERIALPORT_T port;  /*!< �V���A���|�[�g���\���� */
    LPCTSTR portName;   /*!< �V���A���|�[�g�� */
};


/**
 * @brief   Arduino����R�}���h�����s����
 * @param[in]       arduino         Arduino���\����
 * @param[in]       pin             �s���ԍ�
 * @param[in]       pinType         �s�����
 * @param[in]       cmdType         �R�}���h���
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
static int Arduino_execCmd(ARDUINO_T arduino, uint8_t pin, unsigned int pinType, uint8_t cmdType);


/**
 * @brief   LED�̏�Ԃ�\������
 * @param[in]       res             ���X�|���X
 */
static void Arduino_printLEDStatus(uint8_t *res);


ARDUINO_T Arduino_open(LPCTSTR portName)
{
    ARDUINO_T arduino;

    /* �C���X�^���X�̐��� */
    arduino = (ARDUINO_T)malloc(sizeof(struct Arduino_st));
    if (arduino == NULL) {
        return NULL;
    }
    ZeroMemory(arduino, sizeof(struct Arduino_st));
    arduino->portName = portName;

    /* �V���A���|�[�g���J�� */
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

    _tprintf(_T("�X�y�[�X�L�[������LED�̓_���E�����Ɛ؂�ւ��܂�\n"));
    _tprintf(_T("ESC�L�[�����Ńv���O�������I�����܂�\n"));
    while (loop) {
        if (_kbhit() != 0) {
            key = _getch();
            switch (key) {
            case KEY_ESC:
                _tprintf(_T("ESC�L�[����������܂���\n"));
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
    _tprintf(_T("�v���O�������I�����܂���\n"));

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

    /* �R�}���h���M */
    cmd[CMD_TYPE] = cmdType;
    cmd[CMD_PIN] = pin;
    retVal = SerialPort_write(arduino->port, cmd);
    if (retVal == FALSE) {
        _tprintf(_T("���M�G���[���������܂���\n"));
        result = FALSE;
    }
    else {
        /* ���X�|���X��M */
        retVal = SerialPort_read(arduino->port, res);
        if (retVal == FALSE) {
            _tprintf(_T("��M�G���[���������܂���\n"));
            result = FALSE;
        }
        else {
            /* ���X�|���X�\�� */
            switch (pinType) {
            case PIN_TYPE_LED:
                Arduino_printLEDStatus(res);
                break;
            default:
                _tprintf(_T("�s���ԍ�: %d, �s�����: %d\n"), res[RES_PIN], res[RES_PIN_VALUE]);
                break;
            }
        }
    }

    return result;
}


static void Arduino_printLEDStatus(uint8_t *res)
{
    _tprintf(_T("�s���ԍ�: %d, �s�����: %d, LED: "), res[RES_PIN], res[RES_PIN_VALUE]);
    switch (res[RES_PIN_VALUE]) {
    case LED_OFF:
        _tprintf(_T("����"));
        break;
    case LED_ON:
        _tprintf(_T("�_��"));
        break;
    default:
        _tprintf(_T("��ԕs��"));
        break;
    }
    _tprintf(_T("\n"));

    return;
}
