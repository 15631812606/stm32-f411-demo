#ifndef __LCD_H_
#define __LCD_H_
#include "sys.h"

extern u16	POINT_COLOR;	//Ĭ�ϻ�����ɫ
extern u16	BACK_COLOR;		//Ĭ�ϱ�����ɫ

//LCD�Ŀ�͸߶���
#define USE_HORIZONTAL 2  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
    #define LCD_Width 135
    #define LCD_Height 240
#else
    #define LCD_Width 240
    #define LCD_Height 135
#endif

//LCD�����С���ã��޸Ĵ�ֵʱ��ע�⣡�������޸�������ֵʱ���ܻ�Ӱ�����º���	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*135*2)
#define LCD_Buf_Size 1152
static u8 lcd_buf[LCD_Buf_Size];


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF 
#define RED           	 0xF800 //��ɫ
#define MAGENTA       	 0xF81F //Ʒ��ɫ
#define GREEN         	 0x07E0 //��ɫ
#define CYAN          	 0x7FFF //��ɫ
#define YELLOW        	 0xFFE0 //��ɫ
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


/*----------------------------------
    LCD PWR --- PB7 ����
    LCD RST --- PB6 ��λ
    LCD DC  --- PB4 ����/����ѡ��
    LCD SDA --- PB5 ����SDA
    LCD SCL --- PB3 ʱ��SCL
    CS Ƭѡ����-ֱ�ӽ�GND��һֱʹ��
--------------------------------------*/
#define LCD_PWR(x) (x?(PBout(7)=1):(PBout(7)=0))
#define LCD_RST(x) (x?(PBout(6)=1):(PBout(6)=0))
#define LCD_DC(x)  (x?(PBout(4)=1):(PBout(4)=0))


/*------------------------------------------
    LCD PWR --- PB7 ����
    LCD RST --- PB6 ��λ
    LCD DC  --- PB4 ����/����ѡ��
    LCD SDA --- PB5 ����SDA
    LCD SCL --- PB3 ʱ��SCL
    CS Ƭѡ����-ֱ�ӽ�GND��һֱʹ��
-------------------------------------------*/
void LCD_GPIO_init(void);

/*------------------------------------------
    LCD�ײ�SPI�������ݺ���
    data:������ʼ��ַ
-------------------------------------------*/
static void LCD_SPI_Send(u16 data,u8 size);

/*--------------------------------
    д���LCD��Ļ
    cmd����Ҫ���͵�����
----------------------------------*/
static void LCD_Write_Cmd(u8 cmd);

/*----------------------------------
    д���ݵ�LCD��Ļ
    data ��Ҫд������
-----------------------------------*/
static void LCD_Write_Data(u8 data);

/*----------------------------------------
    д����ֵ�LCD
-----------------------------------------*/
void LCD_Write_HalfWord(const u16 data);

/*-----------------------------------------------------
    ��������д��LCD��������
    x1,y1	�������
    x2,y2	�յ�����
-------------------------------------------------------*/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);

/*��LCD��ʾ*/
void LCD_DisplayOn(void);

/*�ر�LCD��ʾ*/
void LCD_DisplayOff(void);

//ʹ��color��ɫ��������
void LCD_Clear(u16 color);

/*------------------------------------------------------------
    ��һ����ɫ��color�������������
    x_start��y_start:��ʼ������
    x_end��y_end����ֹ������
    color��������ɫ
--------------------------------------------------------------*/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

/*------------------------------------------------
    ���㺯����һ�����صĵ�̫С�ˣ���������������
    �����ɫ�����Զ���
--------------------------------------------------*/
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);

/*---------------------------------------------------------------------
    ���ߺ�����ֱ�ߡ�б�ߣ�
    x_start,y_start:�ߵ���ʼλ��
    x_end��y_end���ߵ���ֹ��
    color���ߵ���ɫ
------------------------------------------------------------------------*/
void LCD_Draw_Line(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);

/*---------------------------------------------------------------
      ����˵����������
      ������ݣ� x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
----------------------------------------------------------------*/
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);

/*--------------------------------------------------
      ����˵���� ��Բ
      ������ݣ� x,y   Բ������
                r       �뾶
                color   Բ����ɫ
-----------------------------------------------------*/
void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color);

/*-------------------------------------------------------------------------------
    ����˵��������һ���ַ�
    ������ݣ�x��y ��ʾλ��
             chr  Ҫ��ʾ���ַ�
             size �ַ���С��12��16��24��32��Ҫ��ǰ������Ӧ��С���ֿ⣩
             point_color �ַ���ɫ
             back_color  ������ɫ
--------------------------------------------------------------------------------*/
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size,u16 point_color,u16 back_color);

/*---------------------------------------------------------------------------------------------------------
    ����˵���������ַ���
    ������ݣ�x��y ��ʾλ��
             width:��ʾ���  height����ʾ�߶�
             size �ַ���С��12��16��24��32��Ҫ��ǰ������Ӧ��С���ֿ⣩
             *p���ַ���ָ��
             point_color �ַ���ɫ
             back_color  ������ɫ
----------------------------------------------------------------------------------------------------------*/
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p,u16 point_color,u16 back_color);

/*----------------------------------------------------------------------------------
    �������ܣ���ʾ����
    x,y:��ʾλ��
    num��Ҫ��ʾ������,���ַ�Χ(0~4294967295)
    len����Ҫ��ʾ��λ��
    size:�����С
    piont_color:������ɫ
    back_color:������ɫ
--------------------------------------------------------------------------------------*/
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size,u16 point_color,u16 back_color);

/*---------------------------------------------------------------------------------------
      ����˵������ʾ��λС��
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��(������С����)
                size �ֺ�
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ               
----------------------------------------------------------------------------------------*/
void LCD_ShowFloatNum(u16 x,u16 y,float num,u8 len,u8 size,u16 point_color,u16 back_color);

/*------------------------------------------------------------------------
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
--------------------------------------------------------------------------*/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);

//������ʾ����
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode);

//LCD��ʼ��
void LCD_Init(void);








#endif

