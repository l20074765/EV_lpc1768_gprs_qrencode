/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           key.h
** Last modified Date:  2013-01-08
** Last Version:         
** Descriptions:        2*8��������ͷ�ļ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-10
** Version:             V0.1
** Descriptions:        The original version       
********************************************************************************************************/
#ifndef __KEY_H 
#define __KEY_H

//�û�����
void InitKeyboard(void);
void EINT2_IsrHandler(void);//�жϷ�������Ӧ�ò��õ���
unsigned char ReadKeyValue(void);
void ClearKey(void);
void DisabaleKeyBoard(void);
void EnableKeyBoard(void);
//���̲��Ժ���,ά��ģʽ�м����Լ����
void TestKeyBoard(void);
//������������
unsigned char ReadMaintainKeyValue(void);
unsigned char ReadPayBackKeyValue(void);
unsigned char ReadGameKeyValue(void);
unsigned char ReadCloseHumanKeyValue(void);
void gameLedControl(unsigned char flag);
unsigned char GetKeyInValue(void);

 #endif
/**************************************End Of File*******************************************************/