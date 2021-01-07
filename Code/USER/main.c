#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"

u8 tx_str[]="hello_world";
u8 rx_str[15];
int main(void)
{
	u32 Temp=0;
	LED_GPIO_Init();
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	SPI2_Flash_Init();
	Temp=Flash_Read_ID();
	DEBUG_Error("测试出错打印");
	Flash_Sector_Erase(0x00000);
	Flash_Page_Write(tx_str,0x00000,12);
	Flash_Data_Read(rx_str,0x00000,12);
	printf("flash中写入的数据是：%s\r\n",rx_str);
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(100);
		DEBUG_Info("FlashID=%x",Temp);
	}
}
