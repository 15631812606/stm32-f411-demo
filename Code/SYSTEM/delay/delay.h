#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//初始化延迟函数:1us中断一次，最小延时时间1us
void delay_init(u8 SYSCLK);							    

//系统定时器--中断处理函数
void TimingDelay_Decrement(void);

//	us	延时函数
void delay_us(u32 us);

//	ms	延时函数
void delay_ms(u32 ms);

//	s	延时函数:最长延时4294s
void delay_s(u32 s);

#endif





























