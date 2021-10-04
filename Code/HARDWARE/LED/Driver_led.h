#ifndef __DRIVER_LED_H_
#define __DRIVER_LED_H_
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define LED_R_TOGGLE()  PAout(0) = ~PAout(0)    //红灯闪烁
#define LED_G_TOGGLE()  PAout(1) = ~PAout(1)    //绿灯闪烁
#define LED_B_TOGGLE()  PAout(2) = ~PAout(2)    //蓝灯闪烁

//LED灯的使用的GPIO引脚的初始化函数
void led_gpio_init(void);

#endif

