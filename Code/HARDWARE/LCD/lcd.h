#ifndef __LCD_H_
#define __LCD_H_
#include "sys.h"

extern u16	POINT_COLOR;	//默认画笔颜色
extern u16	BACK_COLOR;		//默认背景颜色

//LCD的宽和高定义
#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
    #define LCD_Width 135
    #define LCD_Height 240
#else
    #define LCD_Width 240
    #define LCD_Height 135
#endif

//LCD缓存大小设置，修改此值时请注意！！！！修改这两个值时可能会影响以下函数	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*135*2)
#define LCD_Buf_Size 1152
static u8 lcd_buf[LCD_Buf_Size];


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF 
#define RED           	 0xF800 //红色
#define MAGENTA       	 0xF81F //品红色
#define GREEN         	 0x07E0 //绿色
#define CYAN          	 0x7FFF //青色
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


/*----------------------------------
    LCD PWR --- PB7 背光
    LCD RST --- PB6 复位
    LCD DC  --- PB4 数据/命令选择
    LCD SDA --- PB5 数据SDA
    LCD SCL --- PB3 时钟SCL
    CS 片选引脚-直接接GND，一直使能
--------------------------------------*/
#define LCD_PWR(x) (x?(PBout(7)=1):(PBout(7)=0))
#define LCD_RST(x) (x?(PBout(6)=1):(PBout(6)=0))
#define LCD_DC(x)  (x?(PBout(4)=1):(PBout(4)=0))


/*------------------------------------------
    LCD PWR --- PB7 背光
    LCD RST --- PB6 复位
    LCD DC  --- PB4 数据/命令选择
    LCD SDA --- PB5 数据SDA
    LCD SCL --- PB3 时钟SCL
    CS 片选引脚-直接接GND，一直使能
-------------------------------------------*/
void LCD_GPIO_init(void);

/*------------------------------------------
    LCD底层SPI发送数据函数
    data:数据起始地址
-------------------------------------------*/
static void LCD_SPI_Send(u16 data,u8 size);

/*--------------------------------
    写命令到LCD屏幕
    cmd：需要发送的命令
----------------------------------*/
static void LCD_Write_Cmd(u8 cmd);

/*----------------------------------
    写数据到LCD屏幕
    data ：要写的数据
-----------------------------------*/
static void LCD_Write_Data(u8 data);

/*----------------------------------------
    写半个字到LCD
-----------------------------------------*/
void LCD_Write_HalfWord(const u16 data);

/*-----------------------------------------------------
    设置数据写入LCD缓存区域
    x1,y1	起点坐标
    x2,y2	终点坐标
-------------------------------------------------------*/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);

/*打开LCD显示*/
void LCD_DisplayOn(void);

/*关闭LCD显示*/
void LCD_DisplayOff(void);

//使用color颜色进行清屏
void LCD_Clear(u16 color);

/*------------------------------------------------------------
    用一种颜色（color）填充整个区域
    x_start，y_start:起始点坐标
    x_end，y_end：终止点坐标
    color：填充的颜色
--------------------------------------------------------------*/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

/*------------------------------------------------
    画点函数（一个像素的点太小了，几乎看不出来）
    点的颜色可以自定义
--------------------------------------------------*/
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);

/*---------------------------------------------------------------------
    画线函数（直线、斜线）
    x_start,y_start:线的起始位置
    x_end，y_end：线的终止点
    color：线的颜色
------------------------------------------------------------------------*/
void LCD_Draw_Line(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);

/*---------------------------------------------------------------
      函数说明：画矩形
      入口数据： x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
----------------------------------------------------------------*/
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);

/*--------------------------------------------------
      函数说明： 画圆
      入口数据： x,y   圆心坐标
                r       半径
                color   圆的颜色
-----------------------------------------------------*/
void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color);

/*-------------------------------------------------------------------------------
    函数说明：绘制一个字符
    入口数据：x，y 显示位置
             chr  要显示的字符
             size 字符大小（12、16、24、32需要提前制作对应大小的字库）
             point_color 字符颜色
             back_color  背景颜色
--------------------------------------------------------------------------------*/
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size,u16 point_color,u16 back_color);

/*---------------------------------------------------------------------------------------------------------
    函数说明：绘制字符串
    入口数据：x，y 显示位置
             width:显示宽度  height：显示高度
             size 字符大小（12、16、24、32需要提前制作对应大小的字库）
             *p：字符串指针
             point_color 字符颜色
             back_color  背景颜色
----------------------------------------------------------------------------------------------------------*/
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p,u16 point_color,u16 back_color);

/*----------------------------------------------------------------------------------
    函数功能：显示数字
    x,y:显示位置
    num：要显示的数字,数字范围(0~4294967295)
    len：需要显示的位数
    size:字体大小
    piont_color:字体颜色
    back_color:背景颜色
--------------------------------------------------------------------------------------*/
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size,u16 point_color,u16 back_color);

/*---------------------------------------------------------------------------------------
      函数说明：显示两位小数
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数(不包括小数点)
                size 字号
                point_color 字的颜色
                back_color 字的背景色               
----------------------------------------------------------------------------------------*/
void LCD_ShowFloatNum(u16 x,u16 y,float num,u8 len,u8 size,u16 point_color,u16 back_color);

/*------------------------------------------------------------------------
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
--------------------------------------------------------------------------*/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);

//汉字显示函数
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode);

//LCD初始化
void LCD_Init(void);








#endif

