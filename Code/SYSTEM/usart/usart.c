#include "sys.h"
#include "usart.h"	
#include "dma.h"
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
/*-----------------------------------------------------------------------------
	��ʼ��USART1����
	bound�������ʣ��û��Զ��壩
-------------------------------------------------------------------------------*/
void Usart1_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ�����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 	//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART-TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//USART-RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���ù���
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;						//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1

	//Usart1 NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//�����жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);				//�������ڿ����ж�
	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���1�����ж�
	USART_Cmd(USART1,ENABLE);  //ʹ�ܴ���1 
}

//USART1����һ���ֽ�
void Usart1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//��ⷢ�����ݼĴ����������Ƿ�ȡ��
	USART_SendData(USART1,(u16 )data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//��ⷢ����λ�Ĵ����������Ƿ���
}

//USART1�����ַ���
void Usart1_Send_String(u8 *data)
{
	while(*data != '\0')
	{
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);	
		USART_SendData(USART1,(u16 )*data); 		//���ڷ����ֽ�
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		data++;
	}
}

void USART1_IRQHandler(void)//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		Usart1_Send_String(&DST_Buffer);
  	} 
} 
	

 



