#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

/*-----------------------------------------------------------------------------
	初始化USART1串口
	bound：波特率（用户自定义）
-------------------------------------------------------------------------------*/
void Usart1_Init(u32 bound);

//USART1发送一个字节
void Usart1_Send_Byte(u8 data);

//USART1发送字符串
void Usart1_Send_String(u8 *data);





#endif


