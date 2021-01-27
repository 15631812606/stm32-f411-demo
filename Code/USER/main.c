#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"
#include "dma.h"

//要发送的数据
const u8 SRC_Buffer[BUFFER_SIZE] = {1,2,3,4,5,6,7,8,9,10};
//数据存储位置
u32 DST_Buffer[BUFFER_SIZE]={0};


int main(void)
{
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	LED_GPIO_Init();
	Usart1_DMA2_init();		//使能DMA2传输（M-to-Usart1）
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	//printf("DMA2_Usart1\r\n");
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(100);
	}
}
