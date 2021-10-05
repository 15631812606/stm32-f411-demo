#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <errno.h>
#include "stm32f4xx.h"

#define _INFO_
#define _DEBUG_
#define _ERROR_

#ifdef _INFO_
#define TAG0  "[ INFO ] "  //打印普通信息
#define INFO(format,...) printf(TAG0  format "\r\n", ##__VA_ARGS__)
#else
#define INFO(format,...)
#endif

#ifdef _DEBUG_
#define TAG1  "[DEBUG ] "  //调试信息
#define DEBUG(format,...) printf(TAG1  "%s %d " format "\r\n", __func__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

#ifdef _ERROR_
#define TAG2  "[ERROR ] "  //打印错误
#define ERROR(format,...) printf(TAG2  "%s %s %d %s " format "\r\n", __FILE__, __func__, __LINE__, __TIME__, ##__VA_ARGS__)
#else
#define ERROR(format,...)
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
    #pragma import(__use_no_semihosting)             
    //标准库需要的支持函数                 
    struct __FILE 
    { 
        int handle; 
    }; 

    FILE __stdout;       
    //定义_sys_exit()以避免使用半主机模式    
    void _sys_exit(int x) 
    { 
        x = x; 
    } 
    //重定义fputc函数 
    int fputc(int ch, FILE *f)
    {
        while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
        USART_SendData(USART1, (u8)ch);
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//检测发送移位寄存器中数据是否发完

        return ch;
    }
#endif

#endif
