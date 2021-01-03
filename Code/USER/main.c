#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
int main(void)
{
	u32 Temp=0;
	LED_GPIO_Init();
	delay_init(100);//主频100MHz
	Usart1_Init(9600);
	SPI2_Flash_Init();
	Temp=SPI_Flash_ID();
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(200);
		printf("FlashID=%x\r\n",Temp);
	}
}
