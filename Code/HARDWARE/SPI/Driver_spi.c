/*------------------------------------------
    # 编码类型：C语言  UTF-8
    # 作者：WRS
    # 功能：STM32F411--spi相关配置函数
    # 创建：2021-07-04
    # 更新：
    # 版本：V1.0.0
--------------------------------------------*/
#include "Driver_spi.h"
#include "Driver_usart.h"
//SPI2初始化
void SPI2_Flash_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能相关的GPIO和SPI时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //设置引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);//引脚复用为SPI2的CLK
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);//引脚复用为SPI2的MISO
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);//引脚复用为SPI2的MOSI

    //配置GPIO引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//CLK和MOSI和MISO
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;        //片选引脚CS
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                //拉高CS引脚，防止误传数据

    //配置SPI模式
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //全双工模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                           //CLK时钟极性默认高
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;    //时钟2分频
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=7;                              //默认CRC校验都选择7
    SPI_Cmd(SPI2,ENABLE);
    SPI_Init(SPI2,&SPI_InitStructure);
}

//SPI3初始化
void SPI3_LCD_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能相关的GPIO和SPI时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //设置引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3);//引脚复用为SPI3的CLK
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI3);//引脚复用为SPI3的MOSI

    //配置GPIO引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_5;//CLK和MOSI
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //配置SPI模式
    SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Tx;             //单线发送模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                           //CLK时钟极性默认高
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;    //时钟2分频
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=10;                              //默认CRC校验都选择7
    SPI_Cmd(SPI3,ENABLE);
    SPI_Init(SPI3,&SPI_InitStructure);
}

/*----------------------------------------------------------------------------------------------
    SPI发送一个字节byte
    byte:要发送的数据
    return:返回接收到的数据
    发送/接收超时：返回ERROR
------------------------------------------------------------------------------------------------*/
u8 SPI_Send_Byte(u8 Byte)
{
    u16 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    SPITimeout=TIMEOUT;
    SPI_I2S_SendData(SPI2,Byte);    //发送数据
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }

    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}

/*----------------------------------------------------------------------------------------------
    SPI读取一个字节byte
    return:返回接收到的数据
------------------------------------------------------------------------------------------------*/
u8 SPI_Read_Byte(void)
{
    u16 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    SPITimeout=TIMEOUT;
    SPI_I2S_SendData(SPI2,Dummy_Byte);    //发送数据
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    
    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}


/*----------------------------------------------------------------------------------------------
    SPI3发送一个字节byte
    byte:要发送的数据
    return:返回接收到的数据
    发送/接收超时：返回ERROR
------------------------------------------------------------------------------------------------*/
u8 SPI3_Send_Byte(u8 Byte)
{
    u16 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI3发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    SPITimeout=TIMEOUT;
    SPI_I2S_SendData(SPI3,Byte);    //发送数据
    while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("SPI3发送超时！！！");
            return ERROR;        //发送超时，返回ERROR
        }
    }

    return SPI_I2S_ReceiveData(SPI3);//读取数据寄存器，返回接收到的数据
}


/*
    LCD函数中使用SPI3进行发送数据
    data ： 数据
    size ： 大小
*/
void SPI3_WriteData(u16 data,u8 size)
{
    if(size==1)
        SPI3_Send_Byte((u8)data);
    else if(size==2)
    {
        SPI3_Send_Byte((u8)(data>>8));
        SPI3_Send_Byte((u8)data);
    }
}
