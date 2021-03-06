#include "delay.h"
#include "sys.h"
#include "misc.h"
static u32 TimingDelay;			   

//初始化延迟函数:1ms中断一次，最小延时时间1ms
void delay_init(u32 SYSCLK)
{
	if(SysTick_Config(SYSCLK))  
		while (1);//系统定时器初始化失败
}								    

//系统定时器--中断处理函数
void TimingDelay_Decrement(void)
{
	if(TimingDelay!=0x00)
		TimingDelay--;
}

//	us	延时函数：最长延时4294967295us
void delay_us(u32 us)//最小不能是微秒延时（这样进中断太快，比较消耗资源）
{
	TimingDelay=us;
	while(TimingDelay!=0);
}

//	ms	延时函数:最长延时4294966ms
void delay_ms(u32 ms)
{
	TimingDelay=ms;
	while (TimingDelay!=0);
}

//	s	延时函数:最长延时4294s
void delay_s(u32 s)
{
	TimingDelay=s*1000;
	while (TimingDelay!=0);
}




















