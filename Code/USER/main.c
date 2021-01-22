#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"

u8 tx_str[]="hello_world-1234567890";
u8 rx_str[50];
int main(void)
{
	u32 Temp;
	LED_GPIO_Init();
	delay_init(100);//主频100MHz
	Usart1_Init(115200);
	SPI2_Flash_Init();
	Temp=Flash_Read_ID();
	Flash_Sector_Erase(0x000000);

	Flash_Page_Write(tx_str,0x000100,sizeof(tx_str));
	Flash_Data_Read(rx_str,0x000100,sizeof(tx_str));
	
	DEBUG_Info("FlashID=%x",Temp);
	printf("flash中写入的数据是：%s\r\n",rx_str);
	
	while(1)
	{
		LED_R_TOGGLE;		
		delay_ms(100);
	}
}
