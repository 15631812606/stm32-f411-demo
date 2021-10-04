/*--------------------------------------------------------------
 * File Driver_usart.c
 * Author WRS (1352509846@qq.com)
 * 功能 
 * Version 0.1
 * Date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
----------------------------------------------------------------*/

#include "Driver_usart.h"	

/*--------------------------------------------------------------
 * 功能: 配置串口的基本功能参数（波特率、数据位、校验位、停止位等）
 * 参数: USARTx 串口号
 * 参数: bound 波特率
----------------------------------------------------------------*/
static void UARTx_function_config(USART_TypeDef* USARTx,u32 bound)
{
    USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = bound;						//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
    USART_Init(USARTx, &USART_InitStructure);                       //初始化串口1
}

/*--------------------------------------------------------------
 * 功能: 配置串口的中断优先级
 * 参数: USARTx_IRQn 中断通道
----------------------------------------------------------------*/
static void UARTx_nvic_config(uint8_t USARTx_IRQn)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//设置中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;		//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	
    NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器
}

/*--------------------------------------------------------------
 * 功能: 串口1初始化
 * 参数: bound 波特率
----------------------------------------------------------------*/
void usart1_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;

	//使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 	//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART-TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//GPIOA10复用为USART-RX
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //USART-TX和USART-RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    UARTx_function_config(USART1,bound);                    //配置串口功能
    UARTx_nvic_config(USART1_IRQn);                         //配置中断优先级
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		    //使能串口1接收中断
	USART_Cmd(USART1,ENABLE);                               //使能串口1 
}


/*--------------------------------------------------------------
 * 功能: 串口发送数据
 * 参数: USARTx 串口号
 * 参数: p 要发送的字节流首地址
 * 参数: length 字节流长度
----------------------------------------------------------------*/
void UARTx_send_datastream(USART_TypeDef *USARTx, s8 *p, u32 length)
{
    if(p == NULL)
        return ;
    for(u32 i=0; i<length; i++)
    {
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//检测发送数据寄存器中数据是否取完
        USART_SendData(USARTx, *(p+i));
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//检测发送移位寄存器中数据是否发完
    }
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
	
/******************** 重定向printf到串口 **************/
int fputc(int ch, FILE *f)
{
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    USART_SendData(USART1, (int8_t)ch);
    return ch;
}
 



