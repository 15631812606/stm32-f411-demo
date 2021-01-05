/*----------------------------------------------------------------------------------
    spi相关代码
    edit--by--wrs--2020-12-27
-----------------------------------------------------------------------------------*/
#include "spi.h"

//SPI初始化
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


/*----------------------------------------------------------------------------------------------
    SPI发送一个字节byte
    byte:要发送的数据
    return:返回接收到的数据
    发送/接收超时：返回0XEE
------------------------------------------------------------------------------------------------*/
u8 SPI_Send_Byte(u8 Byte)
{
    u8 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }
    SPITimeout=TIMEOUT;
    SPI_I2S_SendData(SPI2,Byte);    //发送数据
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }

    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}

/*----------------------------------------------------------------------------------------------
    SPI读取一个字节byte
    return:返回接收到的数据
------------------------------------------------------------------------------------------------*/
u8 SPI_Read_Byte(void)
{
    u8 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }
    SPITimeout=TIMEOUT;
    SPI_I2S_SendData(SPI2,Dummy_Byte);    //发送数据
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }
    
    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}






