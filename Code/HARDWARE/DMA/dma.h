#ifndef __DMA_H__
#define __DMA_H__
#include "sys.h"
#include "stdint.h"

#define BUFFER_SIZE	32
extern const u8 SRC_Buffer[];
extern u8 DST_Buffer[];

void Usart1_DMA2_init(void);
void DMA2_init(void);




#endif


