/*--------------------------------------------------------------
 * File: Driver_spi.c 编码类型：C语言  UTF-8
 * Author WRS (1352509846@qq.com)
 * 功能: STM32F411的SPI驱动
 * Version 0.1
 * Date 2021-10-06 
 * 
 * @copyright Copyright (c) 2021
 * 
----------------------------------------------------------------*/

#include "Driver_spi.h"

/*--------------------------------------------------------------
 * 功能: 初始化SPI1的驱动
 * 片选引脚使用GPIOA4引脚
----------------------------------------------------------------*/
void spi1_init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能相关的GPIO和SPI时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    //设置引脚复用
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);          //引脚复用为SPI2的CLK
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);          //引脚复用为SPI2的MISO
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);          //引脚复用为SPI2的MOSI

    //配置GPIO引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  //CLK和MOSI和MISO
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;                         //片选引脚CS
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;                     //推挽输出
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);                                 //拉高CS引脚，防止误传数据

    //配置SPI模式
    SPI_DeInit(SPI1);                                                   //复位SPI2的寄存器
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //全双工模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                           //CLK时钟极性默认高
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;    //时钟2分频
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=7;                              //默认CRC校验都选择7
    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}

/*--------------------------------------------------------------
 * 功能: 初始化SPI2的驱动
 * 片选引脚使用GPIO_B12引脚
----------------------------------------------------------------*/
void spi2_init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能相关的GPIO和SPI时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //设置引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);          //引脚复用为SPI2的CLK
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);          //引脚复用为SPI2的MISO
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);          //引脚复用为SPI2的MOSI

    //配置GPIO引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //CLK和MOSI和MISO
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;                            //片选引脚CS
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;                         //推挽输出
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                                    //拉高CS引脚，防止误传数据

    //配置SPI模式
    SPI_DeInit(SPI2);                                                   //复位SPI2的寄存器
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //全双工模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                           //CLK时钟极性默认高
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;    //时钟2分频
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=7;                              //默认CRC校验都选择7
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
}

/*--------------------------------------------------------------
 * 功能: 初始化SPI3的驱动
 * 单线发送模式，控制LCD屏幕
----------------------------------------------------------------*/
void spi3_init(void)
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
    SPI_DeInit(SPI3);
    SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Tx;             //单线发送模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                           //CLK时钟极性默认高
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;    //时钟2分频
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=10;                              //默认CRC校验
    SPI_Init(SPI3,&SPI_InitStructure);
    SPI_Cmd(SPI3,ENABLE);
}

u8 spix_read_write_byte(SPI_TypeDef* spix, u8 data)
{
    u16 SPITimeout=TIMEOUT; //设置超时时间

    while(SPI_I2S_GetFlagStatus(spix,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            ERROR("SPI发送超时！！！");
            return 0;
        }
    }
    SPITimeout=TIMEOUT;

    SPI_I2S_SendData(spix,data);    //发送数据

    while(SPI_I2S_GetFlagStatus(spix,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
        {
            ERROR("SPI发送超时！！！");
            return 0;
        }
    }

    return SPI_I2S_ReceiveData(spix);//读取数据寄存器，返回接收到的数据
}

/*--------------------------------------------------------------
 * 功能: LCD函数中使用SPI3进行发送数据
 * 
 * 参数: data 数据 
 * 参数: size 大小
----------------------------------------------------------------*/
void SPI3_WriteData(u16 data,u8 size)
{
    if(size == 1)
    {
        SPI3_Send_Byte((u8)data);
    }
    else if(size == 2)
    {
        SPI3_Send_Byte((u8)(data>>8));
        SPI3_Send_Byte((u8)data);
    }
}
