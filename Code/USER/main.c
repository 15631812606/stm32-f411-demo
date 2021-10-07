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
    W25Qxx_init();
	while(1)
	{
        LED_R_TOGGLE();
        delay_ms(1000);
	}
}
