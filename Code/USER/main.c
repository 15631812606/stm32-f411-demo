/*--------------------------------------------------------------
 * File main.c
 * Author WRS (1352509846@qq.com)
 * 功能: main.c文件，主函数存放的文件
 * Version 0.1
 * Date 2021-10-04 
 * 
 * @copyright Copyright (c) 2021
 * 
----------------------------------------------------------------*/

#include "main.h"

int main(void)
{
	delay_init();
    usart1_init(115200);
    led_gpio_init();
   
    u8 i=210;
    UARTx_send_datastream(USART1,&i,sizeof(i));

	while(1)
	{
        LED_R_TOGGLE();
        delay_ms(400);
        LED_R_TOGGLE();

        LED_G_TOGGLE();
        delay_ms(400);
        LED_G_TOGGLE();

        LED_B_TOGGLE();
        delay_ms(400);
        LED_B_TOGGLE();
	}
}
