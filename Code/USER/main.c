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
u8 DST_Buffer[BUFFER_SIZE]={0};


int main(void)
{
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	LED_GPIO_Init();
	Usart1_DMA2_init();		//使能DMA2传输（M-to-Usart1）

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	//printf("DMA2_Usart1\r\n");
	while(1)
	{
		// DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);	//清除通道1的传输完成标志位（必须清除标志位才能进行下一次传输）
		// DMA_Cmd(DMA2_Stream7,ENABLE);				//再次使能DMA，重新启动一次传输
		LED_R_TOGGLE;		
		delay_ms(100);
	}
}
