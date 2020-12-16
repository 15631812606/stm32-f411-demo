#include "delay.h"
#include "sys.h"
#include "misc.h"
static u32 TimingDelay;			   

//��ʼ���ӳٺ���:1us�ж�һ�Σ���С��ʱʱ��1us
void delay_init(u8 SYSCLK)
{
	if(SysTick_Config(SYSCLK))  
		while (1);//ϵͳ��ʱ����ʼ��ʧ��
}								    

//ϵͳ��ʱ��--�жϴ�������
void TimingDelay_Decrement(void)
{
	if(TimingDelay!=0x00)
		TimingDelay--;
}

//	us	��ʱ���������ʱ4294967295us
void delay_us(u32 us)
{
	TimingDelay=us;
	while(TimingDelay!=0);
}

//	ms	��ʱ����:���ʱ4294966ms
void delay_ms(u32 ms)
{
	TimingDelay=ms*1000;
	while (TimingDelay!=0);
}

//	s	��ʱ����:���ʱ4294s
void delay_s(u32 s)
{
	TimingDelay=s*1000000;
	while (TimingDelay!=0);
}



















