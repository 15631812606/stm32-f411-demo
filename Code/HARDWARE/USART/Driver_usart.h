#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"	
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h" 

//初始化USART1串口
void usart1_init(u32 bound);

//串口发送数据流
void UARTx_send_datastream(USART_TypeDef *USARTx, s8 *p, u32 length);

#endif


