#ifndef __DELAY_H_
#define __DELAY_H_ 			   
#include "sys.h"
#include "misc.h"
#include "core_cm4.h"

//��ʼ���ӳٺ���
void delay_init(void);							    


//	us	��ʱ����
void delay_us(u32 us);

//	ms	��ʱ����
void delay_ms(u32 ms);

//	s	��ʱ����
void delay_s(u32 s);

#endif





























