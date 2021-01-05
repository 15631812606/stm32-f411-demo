#include "flash.h"
#include "spi.h"

//Flash写一个字节，byte：要写入的字节
u8 Flash_Write_byte(u8 byte)
{
    u8 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }
    SPITimeout=TIMEOUT;
    
    SPI_I2S_SendData(SPI2,byte);    //发送数据

    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)==SET)  //等待发送结束
    {
        if((SPITimeout--)==0)
            return 0XEE;        //发送超时，返回0XEE
    }
    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}


//Flash读一个字节
u8 Flash_Read_byte(void)
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

//读取Flash芯片ID号
u32 Flash_Read_ID(void)
{
    u32 FlashID=0;
    u8 Temp0=0,Temp1=0,Temp2=0;

    FLASH_CS_Port = START;        //启动发送
    
    Flash_Write_byte(FLASH_DeviceID);
    Temp0 = Flash_Read_byte();
    Temp1 = Flash_Read_byte();
    Temp2 = Flash_Read_byte();

    FLASH_CS_Port = STOP;        //停止发送  

    FlashID = (Temp0<<16)|(Temp1<<8)|Temp2;
    return FlashID;
}

//Flash芯片--写使能
void Flash_Write_Enable(void)
{
    FLASH_CS_Port = START;
    Flash_Write_byte(FLASH_WriteEnable);
    FLASH_CS_Port = STOP;
}


//获取Flash状态,等待写入结束
void Flash_WaitForWriteEnd(void)
{
    u8 FLASH_Status = 0;
    u8 SPITimeout=TIMEOUT;

    FLASH_CS_Port = START;

    Flash_Write_byte(FLASH_ReadStatusReg);

    do{
        FLASH_Status = Flash_Read_byte();
        if((SPITimeout--)==0)
        {
            
        }
    }while((FLASH_Status&0x01)==SET);//正在写入

    FLASH_CS_Port = STOP;
}






