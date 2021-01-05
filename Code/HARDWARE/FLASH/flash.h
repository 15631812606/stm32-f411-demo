#ifndef __FLASH_H_
#define __FLASH_H_
#include "stm32f4xx_conf.h"
#include "sys.h"
/*------------------命令定义--------------------*/
#define FLASH_WriteEnable           0x06        //使能Flash芯片命令
#define FLASH_WriteDisable          0x04        //失能Flash芯片命令
#define FLASH_ReadStatusReg         0x05        //读取状态寄存器命令
#define FLASH_WriteStatusReg        0x01        //写入状态寄存器命令
#define FLASH_ReadData              0x03        //读取数据
#define FLASH_FastRead              0x0B        //快速读取数据
#define FLASH_PageProgram           0x02        //写入一页数据（每页256字节）
#define FLASH_SectorErase           0x20        //擦除一扇区数据（每扇区4KB）
#define FLASH_BlockErase32          0x52        //擦除一块数据（每块32Kb）
#define FLASH_BlockErase64          0xD8        //擦除一块数据（每块64Kb）
#define FLASH_ChipErase             0xC7        //擦除所有数据（W25Q32共4MB）
#define FLASH_PowerDown             0xB9        //FLASH芯片掉电模式
#define FLASH_ReleasePowerdown      0xAB        //FLASH芯片从掉电模式中恢复
#define FLASH_DeviceID              0x9F        //读取ID

#define FLASH_CS_Port   PBout(12)   //Flash芯片的片选引脚（在SPI初始化函数中使能）

u32 Flash_Read_ID(void);//读取Flash芯片ID

#endif

