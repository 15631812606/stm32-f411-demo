#include "nrf24l01.h"
#include "spi.h"
/*------------------------------------------
    NRF24L01无线通信模块-驱动程序
--------------------------------------------*/

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址

/*-------------------------------------------------------------------------------
    2.4G无线模块使用SPI2接口，和外部Flash公用一个SPI2，无线模块和Flash的SPI时序不同，
    后期联合使用可能有问题，到时候遇到问题在考虑还SPI外设
    IRQ--PB8    CE--PB9    CSN--PB10   SCK--PB13  MISO--PB14  MOSI--PB15
---------------------------------------------------------------------------------*/
void NRF24L01_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //SPI_Cmd(SPI2,DISABLE);//关闭SPI2外设，防止配置不成功

    //使能相关的GPIO和SPI时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //设置引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);//引脚复用为SPI2的CLK
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);//引脚复用为SPI2的MISO
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);//引脚复用为SPI2的MOSI

    //配置GPIO引脚（SPI2使用的引脚）
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//CLK和MOSI和MISO
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //外部Flash的片选引脚PB12,初始化拉高，防止误传数据
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;        //片选引脚CS
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                //拉高CS引脚，防止误传数据
    
    //NRF24L01使用的引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;//PB9--CE PB10--CSN
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//PB8--IRQ
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //配置SPI模式
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //全双工模式
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                         //主机模式
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                     //8位字节
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;                           //CLK时钟极性默认低
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;                          //第二个边沿采样
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                             //片选引脚用软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;    //时钟8分频(NRF24L01最高速率是10M，所以2分频不行，最低的128分频和256分频也不行)
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                    //MSB在前
    SPI_InitStructure.SPI_CRCPolynomial=7;                              //默认CRC校验都选择7
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
    
    NRF24L01_CE=0; 			//使能24L01
	NRF24L01_CSN=1;			//SPI片选取消
}

/*-------------------------------------------------------------------------------
    检测2.4G无线模块是否连接成功,返回0：成功   返回1：失败
---------------------------------------------------------------------------------*/
u8 NRF24L01_Check(void)
{
    u8 i;
    u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
    for(i=0;i<5;i++)
        if(buf[i]!=0XA5)
            break;	 							   
    if(i!=5)
        return 1;   //检测24L01错误	
    return 0;		//检测到24L01
}

/*-------------------------------------------------------------------------------
    SPI2写NRF24L01寄存器   reg:指定寄存器地址  value:写入的值
---------------------------------------------------------------------------------*/
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;	
    NRF24L01_CSN=0;                 //使能SPI传输
    status =SPI_Send_Byte(reg);     //发送寄存器号 
    SPI_Send_Byte(value);           //写入寄存器的值
    NRF24L01_CSN=1;                 //禁止SPI传输	   
    return(status);       			//返回状态值
}

/*-------------------------------------------------------------------------------
    SPI2读取NRF24L01寄存器   reg:指定寄存器地址  返回值：返回读取的寄存器值
---------------------------------------------------------------------------------*/
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;	    
    NRF24L01_CSN = 0;          //使能SPI传输		
    SPI_Send_Byte(reg);        //发送寄存器号
    reg_val=SPI_Read_Byte();   //读取寄存器内容
    NRF24L01_CSN = 1;          //禁止SPI传输		    
    return(reg_val);           //返回读取的寄存器值
}

/*-------------------------------------------------------------------------------
    SPI2在NRF24L01指定位置写入指定长度数据
    reg:寄存器(位置)
    *pBuf:数据指针
    len:数据长度
    返回值,此次读到的状态值
---------------------------------------------------------------------------------*/
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,i;	    
    NRF24L01_CSN = 0;               //使能SPI传输
    status = SPI_Send_Byte(reg);    //发送寄存器值(位置),并读取状态值

    for(i=0; i<len; i++)
        SPI_Send_Byte(*pBuf++);     //写入数据	 

    NRF24L01_CSN = 1;               //关闭SPI传输
    return status;                  //返回读到的状态值
}	

/*-------------------------------------------------------------------------------
    SPI2在NRF24L01指定位置读取指定长度数据
    reg:寄存器(位置)
    *pBuf:数据指针
    len:数据长度
    返回值,此次读到的状态值
---------------------------------------------------------------------------------*/
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,i;	       
    NRF24L01_CSN = 0;               //使能SPI传输
    status=SPI_Send_Byte(reg);      //发送寄存器值(位置),并读取状态值   	   
    
    for(i=0;i<len;i++)
        pBuf[i]=SPI_Read_Byte();    //读出数据

    NRF24L01_CSN=1;                 //关闭SPI传输
    return status;                  //返回读到的状态值
}

/*-------------------------------------------------------------------------------
    启动NRF24L01发送一次数据
    txbuf:待发送数据首地址
    返回值:发送完成状况
---------------------------------------------------------------------------------*/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
    NRF24L01_CE=1;//启动发送

    while(NRF24L01_IRQ!=0);         //等待发送完成
    sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	
    if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

/*-------------------------------------------------------------------------------
    启动NRF24L01接收一次数据
    rxbuf:接收数据的数组
    返回值:返回0接收成功，返回其他值 接收失败
---------------------------------------------------------------------------------*/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	sta=NRF24L01_Read_Reg(STATUS);                  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);   //清除TX_DS或MAX_RT中断标志
	
    if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}

/*-------------------------------------------------------------------------------
    设置NRF24L01为RX模式
    设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
    当CE变高后,即进入TX模式,并可以接收数据了
---------------------------------------------------------------------------------*/
void NRF24L01_RX_Mode(void)
{
    NRF24L01_CE=0;	  
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
        
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
    NRF24L01_CE = 1; //CE为高,进入接收模式 
}			

/*-------------------------------------------------------------------------------
    设置NRF24L01为TX模式
    设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR PWR_UP,CRC使能
    当CE变高后,即进入RX模式,并可以接收数据了
---------------------------------------------------------------------------------*/
void NRF24L01_TX_Mode(void)
{														 
    NRF24L01_CE=0;	    
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
    NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
    NRF24L01_CE=1;//CE为高,10us后启动发送
}


