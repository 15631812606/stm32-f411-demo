#ifndef __DRIVER_FLASH_
#define __DRIVER_FLASH_

#include "Driver_spi.h"
#include "stm32f4xx.h"
#include "debug.h"

#define START   0                   //启动
#define STOP    1                   //停止
#define W25Qxx_CS_Port   PBout(12)  //Flash芯片的片选引脚（在SPI初始化函数中使能）

/*------------------命令定义--------------------*/
#define W25Q32ID                     0xEF4016    //芯片的ID号
#define W25Qxx_WriteEnable           0x06        //使能Flash芯片命令
#define W25Qxx_WriteDisable          0x04        //失能Flash芯片命令
#define W25Qxx_ReadStatusReg         0x05        //读取状态寄存器命令
#define W25Qxx_WriteStatusReg        0x01        //写入状态寄存器命令
#define W25Qxx_ReadData              0x03        //读取数据
#define W25Qxx_FastRead              0x0B        //快速读取数据
#define W25Qxx_PageProgram           0x02        //写入一页数据（每页256字节）
#define W25Qxx_SectorErase           0x20        //擦除一扇区数据（每扇区4KB）
#define W25Qxx_BlockErase32          0x52        //擦除一块数据（每块32Kb）
#define W25Qxx_BlockErase64          0xD8        //擦除一块数据（每块64Kb）
#define W25Qxx_ChipErase             0xC7        //擦除所有数据（W25Q32共4MB）
#define W25Qxx_PowerDown             0xB9        //FLASH芯片掉电模式
#define W25Qxx_ReleasePowerdown      0xAB        //FLASH芯片从掉电模式中恢复
#define W25Qxx_DeviceID              0x9F        //读取ID

u8 W25Qxx_init(void);

#endif
