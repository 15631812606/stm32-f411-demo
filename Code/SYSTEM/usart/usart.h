#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#define DEBUG

//���������Ϣ��ʹ��printf����������C���Գ��򶼿�����ֲ��
#define DEBUG_Info(str,arg...) printf("INFO:   "str"\r\n",##arg)

#ifdef 	DEBUG
	#define DEBUG_Error(str,arg...) printf("----->>ERROR:  %s  %s  %d line "str"\r\n",__FILE__,__FUNCTION__,__LINE__, ##arg)
#else
	#define DEBUG_Error(str,arg...) 
#endif

/*-----------------------------------------------------------------------------
	��ʼ��USART1����
	bound�������ʣ��û��Զ��壩
-------------------------------------------------------------------------------*/
void Usart1_Init(u32 bound);

//USART1����һ���ֽ�
void Usart1_Send_Byte(u8 data);

//USART1�����ַ���
void Usart1_Send_String(u8 *data);





#endif


