#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//��ʼ���ӳٺ���:1us�ж�һ�Σ���С��ʱʱ��1us
void delay_init(u8 SYSCLK);							    

//ϵͳ��ʱ��--�жϴ�����
void TimingDelay_Decrement(void);

//	us	��ʱ����
void delay_us(u32 us);

//	ms	��ʱ����
void delay_ms(u32 ms);

//	s	��ʱ����:���ʱ4294s
void delay_s(u32 s);

#endif





























