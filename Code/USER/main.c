#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
int main(void)
{
	LED_GPIO_Init();
	delay_init(100);//主频100MHz
	Usart1_Init(9600);
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(200);
		printf("hello_world\r\n");
	}
}
