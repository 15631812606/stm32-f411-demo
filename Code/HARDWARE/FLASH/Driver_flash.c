/*--------------------------------------------------------------
 * File: Driver_flash.c 编码类型：C语言  UTF-8
 * Author WRS (1352509846@qq.com)
 * 功能: W25Qxx驱动程序
 * Version 0.1
 * Date 2021-10-07 
 * 
 * @copyright Copyright (c) 2021
 * 
----------------------------------------------------------------*/

#include "Driver_flash.h"

static u32 W25Qxx_Read_ID(void)
{
    u32 W25QxxID = 0;
    u8 temp[3] = {0};

    W25Qxx_CS_Port = START;     //启动发送
    spix_read_write_byte(SPI2,W25Qxx_DeviceID);
    temp[0] = spix_read_write_byte(SPI2,0xff);  
    temp[1] = spix_read_write_byte(SPI2,0xff);
    temp[2] = spix_read_write_byte(SPI2,0xff);
    W25Qxx_CS_Port = STOP;      //停止发送
	W25QxxID = (temp[0]<<16)|(temp[1]<<8)|temp[2];
    return W25QxxID;
}

/*--------------------------------------------------------------
 * 功能: 初始化W25QXX
 * 需要根据不同型号的芯片设置不同的ID号
 * 返回值: u8 成功返回success  失败返回ERROR
----------------------------------------------------------------*/
u8 W25Qxx_init(void)
{
    spi2_init();

    //初始化W25Qxx的片选引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;                            //片选引脚CS
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;                         //推挽输出
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                                    //拉高CS引脚，防止误传数据
    
    //读取W25Qxx的ID号
    if(W25Q32ID == W25Qxx_Read_ID())
    {
        INFO("W25Q32 init SUCCEED");
        return SUCCESS;
    }
    else
    {
        MY_ERROR("W25Q32 init Fail");
        return ERROR;
    }
}


