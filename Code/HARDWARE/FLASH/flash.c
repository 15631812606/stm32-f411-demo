#include "flash.h"
#include "spi.h"
#include "usart.h"
#include "delay.h"

//Flash写一个字节，byte：要写入的字节
u8 Flash_Write_byte(u8 byte)
{
    u16 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("Flash芯片写入超时出错!!!");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    SPITimeout=TIMEOUT;
    
    SPI_I2S_SendData(SPI2,byte);    //发送数据

    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)==SET)  //等待发送结束
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("Flash芯片写入超时出错!!!");
            return ERROR;        //发送超时，返回ERROR
        }
    }
    return SPI_I2S_ReceiveData(SPI2);//读取数据寄存器，返回接收到的数据
}


//Flash读一个字节
u8 Flash_Read_byte(void)
{
    u16 SPITimeout=TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)//等待发送为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("Flash芯片读取超时出错!!!");
            return ERROR;        //读取超时，返回ERROR
        }
    }
    SPITimeout=TIMEOUT;

    SPI_I2S_SendData(SPI2,Dummy_Byte);    //发送数据

    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)//等待接收不为空
    {
        if((SPITimeout--)==0)
        {
            DEBUG_Error("Flash芯片读取超时出错!!!");
            return ERROR;        //读取超时，返回ERROR
        }
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
    u16 SPITimeout=TIMEOUT;

    FLASH_CS_Port = START;

    Flash_Write_byte(FLASH_ReadStatusReg);

    do{
        FLASH_Status = Flash_Read_byte();
        if((SPITimeout--)==0)
        {
            DEBUG_Error("Flash芯片-写入/擦除-超时出错!!!<<<<");
            return;
        }
        delay_ms(1);
    }while((FLASH_Status&0x01)==SET);//正在写入

    FLASH_CS_Port = STOP;
}

//flash数据扇区擦除（每个扇区4KB，扇区是最小擦除单位）SectorAddr：扇区首地址
void Flash_Sector_Erase(u32 SectorAddr)
{
   
    Flash_Write_Enable();       //发送Flash写使能
    Flash_WaitForWriteEnd();    //等待发送结束

    FLASH_CS_Port = START;

    Flash_Write_byte(FLASH_SectorErase);                //发送扇区擦除指令
    // Flash_Write_byte(SectorAddr & 0xFF000000>>24);   //发送要擦除的扇区地址的高8位(大于128MB才有高8位地址)
    Flash_Write_byte((SectorAddr & 0xFF0000)>>16);      //发送要擦除的扇区地址的中高8位
    Flash_Write_byte((SectorAddr & 0xFF00)>>8);         //发送要擦除的扇区地址的低高8位
    Flash_Write_byte(SectorAddr & 0xFF);                //发送要擦除的扇区地址的低高8位

    FLASH_CS_Port = STOP;
    Flash_WaitForWriteEnd();        //等待发送完毕
}

//Flash的页写入（每次最多写入256字节，因为每页大小是256字节）
void Flash_Page_Write(u8* str,u32 WriteAddr,u16 NumByteToWrite)
{
    //写发送使能命令
    Flash_Write_Enable();

    FLASH_CS_Port = START;//启动发送
    
    Flash_Write_byte(FLASH_PageProgram);
    // Flash_Write_byte(WriteAddr & 0xFF000000>>24);    //发送要擦除的扇区地址的高8位
    Flash_Write_byte((WriteAddr & 0xFF0000)>>16);       //发送要擦除的扇区地址的中高8位
    Flash_Write_byte((WriteAddr & 0xFF00)>>8);          //发送要擦除的扇区地址的低高8位
    Flash_Write_byte(WriteAddr & 0xFF);                 //发送要擦除的扇区地址的低高8位

    if(NumByteToWrite>256)
    {
        NumByteToWrite = 256;
        DEBUG_Error("Flash页写入内容太长了，超过每页的范围");
    }

    while(NumByteToWrite--)
    {
        Flash_Write_byte(*str);
        str++;
    }

    FLASH_CS_Port = STOP;
    
    Flash_WaitForWriteEnd();//等待写入完成
}

//不定量的数据写入
void Flash_Data_Write(u8* str,u32 WriteAddr,u16 NumByteToWrite)
{
    u8 NumOfPage = 0;
    u8 NumOfSingle = 0;
    u8 Addr = 0;
    u8 count = 0;

    //mod运算求余数，若WriteAddr是256字节的整数倍，则写入整数页
    Addr = WriteAddr%256;
    count = 256-Addr;

    //计算出要写入的整数页数
    NumOfPage = NumByteToWrite/256;
    NumOfSingle = NumByteToWrite%256;

    if(Addr==0)//写入地址刚好页对齐
    {
        if(NumOfPage==0)//写入的数据小于每页的256字节大小
        {
            Flash_Page_Write(str,WriteAddr,NumByteToWrite);
        }
        else
        {
            while(NumOfPage--)//先把整数页写完
            {
                Flash_Page_Write(str,WriteAddr,256);
                WriteAddr+=256;
                str+=256;
            }
            Flash_Page_Write(str,WriteAddr,NumOfSingle);//写入剩下的不满一页的数据
        }
    }
    else//写入的地址没有也对齐
    {
        NumByteToWrite-=count;
        NumOfPage = NumByteToWrite/256;//整页数
        NumOfSingle = NumByteToWrite%256;//剩下的不足一页的数据长度

        Flash_Page_Write(str,WriteAddr,count);
        WriteAddr+=count;
        str+=count;

        //写整数页
        while(NumOfPage--)
        {
            Flash_Page_Write(str,WriteAddr,256);
            WriteAddr+=256;
            str+=256;
        }
        if(NumOfSingle!=0)//写剩下的（不足一页的）数据
        {
            Flash_Page_Write(str,WriteAddr,NumOfSingle);//写入剩下的不满一页的数据
        }
    }
}


//读取Flash中的数据
void Flash_Data_Read(u8* str,u32 ReadAddr,u16 NumByteToRead)
{
    FLASH_CS_Port = START;

    Flash_Write_byte(FLASH_ReadData);
    // Flash_Write_byte(ReadAddr & 0xFF000000>>24);  //发送要擦除的扇区地址的高8位
    Flash_Write_byte((ReadAddr & 0xFF0000)>>16);    //发送要擦除的扇区地址的中高8位
    Flash_Write_byte((ReadAddr & 0xFF00)>>8);       //发送要擦除的扇区地址的低高8位
    Flash_Write_byte(ReadAddr & 0xFF);              //发送要擦除的扇区地址的低高8位

    while(NumByteToRead--)
    {
        *str = Flash_Write_byte((u8)Dummy_Byte);
        str++;
    }

    FLASH_CS_Port = STOP;
}

