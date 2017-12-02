/**
 * @file        SerialPort.h
 * @brief       �V���A���|�[�g�ʐM���C�u����
 * @author      Keitetsu
 * @date        2014/08/10
 * @copyright   Copyright (c) 2014 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#pragma once    /* __SERIALPORT_H__ */


/**
 * @typedef SERIALPORT_T
 * @brief   �V���A���|�[�g���\����
 */
typedef struct SerialPort_st* SERIALPORT_T;


/**
 * @brief   �V���A���|�[�g���J��
 * @param[in]       portName        �V���A���|�[�g��
 * @param[in]       portBaudRate    �{�[���[�g
 * @retval          NULL            �ُ�I��
 * @retval          Others          �V���A���|�[�g���\����
 */
SERIALPORT_T SerialPort_open(LPCTSTR portName, unsigned int portBaudRate);


/**
 * @brief   �V���A���|�[�g�����
 * @param[in]       port            �V���A���|�[�g���\����
 * @retval          NULL            ����I��
 * @retval          Others          �ُ�I��
 */
SERIALPORT_T SerialPort_close(SERIALPORT_T port);


/**
 * @brief   �V���A���|�[�g����f�[�^��Ǎ���
 * @param[in]       port            �V���A���|�[�g���\����
 * @param[out]      portReadData    �f�[�^�o�b�t�@�̃|�C���^
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
int SerialPort_read(SERIALPORT_T port, void *portReadData);


/**
 * @brief   �V���A���|�[�g�Ƀf�[�^��������
 * @param[in]       port            �V���A���|�[�g���\����
 * @param[in]       portWriteData   �f�[�^�o�b�t�@�̃|�C���^
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
int SerialPort_write(SERIALPORT_T port, void *portWriteData);
