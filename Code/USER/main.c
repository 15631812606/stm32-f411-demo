/**
 * @file main.c
 * @author WRS (1352509846@qq.com)
 * @brief main.c文件，主函数存放的文件
 * - 本次实现的功能是点亮LED灯
 * - 配置滴答定时器，配置delay函数
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main.h"

int main(void)
{
	delay_init();
    led_gpio_init();
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
