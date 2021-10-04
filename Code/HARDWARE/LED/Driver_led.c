/**
 * @file Driver_led.c
 * @author WRS (1352509846@qq.com)
 * @brief led灯相关驱动程序
 * - 主要是GPIO引脚的初始化
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Driver_led.h"

/**
 * @brief 初始化led的GPIO引脚
 * - LED是三色RGB灯，所以初始化了3个GPIO引脚
 * - GPIO_A0----red灯
 * - GPIO_A1----green灯
 * - GPIO_A2----blue灯
 */
void led_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
