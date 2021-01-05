#ifndef __SPI_H_
#define __SPI_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define TIMEOUT 200             //超时时间
#define Dummy_Byte 0xFFFF       //虚拟数据（用来产生时钟）
#define START   0               //启动
#define STOP    1               //停止

//SPI初始化
void SPI2_Flash_Init(void);

/*----------------------------------------------------------------------------------------------
    SPI发送一个字节byte
    byte:要发送的数据
    return:返回接收到的数据
    发送/接收超时：返回0XEE
------------------------------------------------------------------------------------------------*/
u8 SPI_Send_Byte(u8 Byte);

/*----------------------------------------------------------------------------------------------
    SPI读取一个字节byte
    return:返回接收到的数据
------------------------------------------------------------------------------------------------*/
u8 SPI_Read_Byte(void);





#endif



