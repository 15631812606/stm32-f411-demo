#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

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


