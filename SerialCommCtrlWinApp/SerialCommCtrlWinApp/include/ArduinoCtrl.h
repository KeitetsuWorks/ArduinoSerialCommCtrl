/**
 * @file        ArduinoCtrl.h
 * @brief       �V���A���ʐM�ɂ��Arduino����
 * @author      Keitetsu
 * @date        2017/12/02
 * @copyright   Copyright (c) 2017 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#pragma once    /* __ARDUINOCTRL_H__ */


/**
 * @typedef ARDUINO_T
 * @brief   Arduino���\����
 */
typedef struct Arduino_st* ARDUINO_T;


/**
 * @brief   Arduino���J��
 * @param[in]       portName        �V���A���|�[�g��
 * @retval          NULL            �ُ�I��
 * @retval          Others          Arduino���\����
 */
ARDUINO_T Arduino_open(LPCTSTR portName);


/**
 * @brief   Arduino�����
 * @param[in]       arduino         Arduino���\����
 * @retval          NULL            ����I��
 * @retval          Others          Arduino
 */
ARDUINO_T Arduino_close(ARDUINO_T arduino);


/**
 * @brief   Arduino����̃��C�����[�v���������s����
 * @param[in]       arduino         Arduino���\����
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
int Arduino_execMainLoop(ARDUINO_T arduino);
