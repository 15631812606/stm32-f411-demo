#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
int main(void)
{
	LED_GPIO_Config();
	delay_init(100);
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		delay_ms(100);
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		delay_ms(100);
	}
}
