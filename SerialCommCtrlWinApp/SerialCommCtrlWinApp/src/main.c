/**
 * @file        main.c
 * @brief       Arduino����pWindows�A�v���P�[�V����
 * @author      Keitetsu
 * @date        2017/12/02
 * @copyright   Copyright (c) 2017 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <tchar.h>

#include "ArduinoCtrl.h"


int main(int argc, _TCHAR* argv[])
{
    ARDUINO_T arduinoLED;
    int retVal;

    /* ���P�[���̐ݒ� */
    _tsetlocale(LC_ALL, _T("Japanese_Japan.932"));

    /* Arduino���J�� */
    arduinoLED = Arduino_open(_T("\\\\.\\COM3"));
    if (arduinoLED == NULL) {
        exit(EXIT_FAILURE);
    }

    /* Arduino���� */
    retVal = Arduino_execMainLoop(arduinoLED);
    
    /* Arduino����� */
    Arduino_close(arduinoLED);

    if (retVal == FALSE) {
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
