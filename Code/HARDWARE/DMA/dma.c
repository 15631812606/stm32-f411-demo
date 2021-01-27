#include "dma.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_dma.h"

#define DMA2_Stream DMA2_Stream0

//DM2初始化（用于存储器到存储器进行传输数据，只能使用DMA2）
void DMA2_init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//使能DMA2的时钟
    DMA_DeInit(DMA2_Stream);       //复位DMA2流0

    DMA_InitStructure.DMA_Channel = DMA_Channel_0;      //DMA数据流通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Buffer;    //源数据地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DST_Buffer;       //目标地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;                 //存储器到存储器模式
    DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;           //数据流大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;     //使能源地址自动递增功能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             //使能目标地址自动递增
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;     //目标数据字大小
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//源数据字大小
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       //一次模式（存储器到存储器不能循环传输）
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 //优先级高
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;              //禁用FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;        
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //单次模式
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //单次模式
    DMA_Init(DMA2_Stream,&DMA_InitStructure);
    DMA_Cmd(DMA2_Stream,ENABLE);
}



