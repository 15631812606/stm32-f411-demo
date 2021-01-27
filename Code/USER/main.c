#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"
#include "dma.h"

//要发送的数据
const u32 SRC_Buffer[BUFFER_SIZE] = 
{
	0x11111111,
	0x22222222,
	0x33333333,
	0x44444444,
	0x55555555,
	0x66666666,
	0x77777777,
	0x88888888,
	0x99999999,
	0xAAAAAAAA
};
//数据存储位置
u32 DST_Buffer[BUFFER_SIZE]={0};


int main(void)
{
	u8 i=0;
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	LED_GPIO_Init();
	DMA2_init();		//使能DMA2传输（M-to-M）
	
	for(i=0;i<10;i++)
	{
		printf("SRC_Buffer:0x%x\r\n",SRC_Buffer[i]);
	}
	for(i=0;i<10;i++)
	{
		printf("DST_Buffer:0x%x\r\n",DST_Buffer[i]);
	}
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(100);
	}
}
