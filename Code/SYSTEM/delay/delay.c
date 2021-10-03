/**
 * @file delay.c
 * @author WRS (1352509846@qq.com)
 * @brief ��ʱ�����������
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
 * @brief ��ʱ���ܳ�ʼ��
 * - ʹ�õδ�ʱ��������ʱ
 * - �������жϣ���ʡоƬ��Դ
 * - STM32F411оƬ��Ƶ��100MHz
 */
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //�δ�ʱ����Ƶʹ��100M������Ƶ
    fac_us = SystemCoreClock / 1000000; //100Mhz��Ƶ��ÿ΢����Ҫ����100��ʱ��
    fac_ms = (u32)fac_us * 1000;        //ÿ������Ҫ����100*1000��
}								    

/**
 * @brief ΢�뼶��ʱ����
 * 
 * @param n_us ��ʱn΢��
 */
void delay_us(u32 n_us)
{
	u32 temp = 0;
    SysTick->LOAD = n_us * fac_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //������ʱ
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1<<16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief ���뼶��ʱ�����������⣩
 * @param n_ms ��ʱn����(���ܴ���167����)
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
 * @brief ���뼶��ʱ����
 * - ���ڵδ�ʱ����������24λ�ģ�������ʱ���ܴ���167����
 * - ���ԶԺ�����ʱ�������·�װ����������ʱ��ĺ�����ʱ
 * @param n_ms ��ʱ�ĺ�����
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
 * @brief �뼶��ʱ����
 * @param s ��ʱ������
 */
void delay_s(u32 s)
{
	delay_ms(s*1000);
}




















