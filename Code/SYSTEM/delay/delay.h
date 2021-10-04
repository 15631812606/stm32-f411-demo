#ifndef __DELAY_H_
#define __DELAY_H_ 			   
#include "sys.h"
#include "misc.h"
#include "core_cm4.h"

//初始化延迟函数
void delay_init(void);							    


//	us	延时函数
void delay_us(u32 n_us);

//	ms	延时函数
void delay_ms(u32 n_ms);

//	s	延时函数
void delay_s(u32 s);

#endif





























