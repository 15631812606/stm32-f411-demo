#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "lcd.h"
#include "alientek_log.h"
#include "pic.h"

int main(void)
{
	delay_init(100000);//Ö÷Æµ100MHz
	Usart1_Init(115200);
	LED_GPIO_Init();
    printf("LCDÆÁÄ»Çý¶¯³ÌÐò\r\n");

    LCD_GPIO_init();
    //LCD³õÊ¼»¯
    LCD_Init();
    LCD_Clear(BLACK);
    // LCD_DrawRectangle(10,10,100,100,RED);
    // LCD_Draw_Circle(55,55,45,RED);
    // LCD_ShowChar(55,55,'H',32,RED,WHITE);
    // LCD_ShowString(20,20,16*11,32,32,"HELLO-WO123",RED,WHITE);
    // LCD_ShowNum(160,60,6789,6,16,RED,YELLOW);
    // LCD_ShowFloatNum(160,90,123.4,5,24,RED,YELLOW);
    // LCD_ShowPicture(0,0,240,135,ALIENTEK_LOGO);
    // LCD_ShowPicture(2,0,131,240,gImage_pic);
    LCD_ShowChinese(20,20,"ÆÁÄ»ºº×ÖÏÔÊ¾",RED,BLACK,32,0);
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(1000);
	}
}
