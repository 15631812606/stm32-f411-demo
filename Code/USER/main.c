#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"
int main(void)
{
	u32 Temp=0;
	LED_GPIO_Init();
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	SPI2_Flash_Init();
	Temp=Flash_Read_ID();
	DEBUG_Error("测试出错打印");
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(100);
		DEBUG_Info("FlashID=%x",Temp);
	}
}
