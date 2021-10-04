#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <errno.h>
#include "stm32f4xx.h"

#define _DEBUG_
#define _INFO_

#define COLOR(msg, code) "\033[1;" #code "m" msg "\033[0;m"
#define RED(msg)    COLOR(msg, 31)  	//红色
#define GREEN(msg)  COLOR(msg, 32)  	//绿色
#define YELLOW(msg) COLOR(msg, 33)  	//黄色
#define BLUE(msg)   COLOR(msg, 34) 		//蓝色
#define REDY(msg)   COLOR(msg, 35)  	//洋红
#define CYAN(msg)   COLOR(msg, 36)  	//青色

#ifdef _INFO_
#define TAG0  "[ INFO ] "  //打印普通信息
#define INFO(format,...) printf(GREEN(TAG0) format, ##__VA_ARGS__)
#else
#define INFO(format,...)
#endif

#ifdef _DEBUG_
#define TAG1  "[DEBUG ] "  //调试信息
#define DEBUG(format,...) printf(BLUE(TAG1)YELLOW("%s %d ") format, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

#define TAG2  "[ERROR ] "  //打印错误
#define ERROR(format,...) printf(RED(TAG2)YELLOW("%s %s %d %s ") format, __FILE__, __func__, __LINE__, __TIME__, ##__VA_ARGS__)

#define TAG3  "[NOTICE] "  //打印通知
#define NOTICE(format,...) printf(REDY(TAG3)YELLOW("%s %d ") format, __func__, __LINE__, ##__VA_ARGS__)


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
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (unsigned char) ch;      
	return ch;
}
#endif

#endif
