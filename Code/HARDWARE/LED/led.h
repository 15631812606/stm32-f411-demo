#ifndef __LED_H_
#define __LED_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define LED_R_TOGGLE PAout(0)=~PAout(0)
#define LED_G_TOGGLE PAout(1)=~PAout(1)
#define LED_B_TOGGLE PAout(2)=~PAout(2)

void LED_GPIO_Init(void);




#endif

