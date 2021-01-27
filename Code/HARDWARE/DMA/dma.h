#ifndef __DMA_H__
#define __DMA_H__
#include "sys.h"
#include "stdint.h"

#define BUFFER_SIZE	32
extern const u32 SRC_Buffer[];
extern u32 DST_Buffer[];


void DMA2_init(void);




#endif


