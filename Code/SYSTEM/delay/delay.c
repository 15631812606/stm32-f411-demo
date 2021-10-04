/**
 * @file delay.c
 * @author WRS (1352509846@qq.com)
 * @brief 延时函数相关内容
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "delay.h"

static u32  fac_us = 0;
static u32 fac_ms = 0;		   

/**
 * @brief 延时功能初始化
 * - 使用滴答定时器进行延时
 * - 不进入中断，节省芯片资源
 * - STM32F411芯片主频是100MHz
 */
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //滴答定时器主频使用100M，不分频
    fac_us = SystemCoreClock / 1000000; //100Mhz主频，每微秒需要跳动100次时钟
    fac_ms = (u32)fac_us * 1000;        //每毫秒需要跳动100*1000次
}								    

/**
 * @brief 微秒级延时函数
 * - 最长延时167772微秒
 * @param n_us 延时n微秒
 */
void delay_us(u32 n_us)
{
	u32 temp = 0;
    SysTick->LOAD = n_us * fac_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //启动定时
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1<<16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief 毫秒级延时函数（不对外）
 * - 最长延时167毫秒
 * @param n_ms 延时n毫秒(不能大于167毫秒)
 */
static void delay_xms(u32 n_ms)
{
    u32 temp = 0;
    SysTick->LOAD = (uint32_t)n_ms * fac_ms;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}

/**
 * @brief 毫秒级延时函数
 * - 由于滴答定时器计数器是24位的，所以延时不能大于167毫秒
 * - 所以对毫秒延时函数重新封装，可以任意时间的毫秒延时
 * - 延时超过一秒可以使用delay_s秒级延时函数
 * @param n_ms 延时的毫秒数
 */
void delay_ms(u32 n_ms)
{
    u32 repeat = n_ms / 167;
    u32 remain = n_ms % 167;

    while(repeat)
    {
        delay_xms(167);
        repeat--;
    }
    if(remain)
        delay_xms(remain);
}

/**
 * @brief 秒级延时函数
 * @param s 延时的秒数
 */
void delay_s(u32 s)
{
	delay_ms(s * 1000);
}




















