#include "sys.h"
#include "usart.h"	
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
/*-----------------------------------------------------------------------------
	初始化USART1串口
	bound：波特率（用户自定义）
-------------------------------------------------------------------------------*/
void Usart1_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 	//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART-TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//USART-RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用功能
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;						//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	//Usart1 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//设置中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);				//开启串口空闲中断
	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能串口1接收中断
	USART_Cmd(USART1,ENABLE);  //使能串口1 
}

//USART1发送一个字节
void Usart1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//检测发送数据寄存器中数据是否取完
	USART_SendData(USART1,(u16 )data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//检测发送移位寄存器中数据是否发完
}

//USART1发送字符串
void Usart1_Send_String(u8 *data)
{
	while(*data != '\0')
	{
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);	
		USART_SendData(USART1,(u16 )*data); 		//串口发送字节
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		data++;
	}
}

void USART1_IRQHandler(void)//串口1中断服务程序
{
//	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		//Usart1_Send_String(&DST_Buffer);
  	} 
} 
	

 



