#include "lcd.h"
#include "tft_font.h"
#include "spi.h"
#include "delay.h"
#include "lcdfont.h"

u16	POINT_COLOR = WHITE;	    //������ɫ	Ĭ��Ϊ��ɫ
u16	BACK_COLOR 	= BLACK;	    //������ɫ	Ĭ��Ϊ��ɫ

/*------------------------------------------
    LCD PWR --- PB7 ����
    LCD RST --- PB6 ��λ
    LCD DC  --- PB4 ����/����ѡ��
    LCD SDA --- PB5 ����SDA
    LCD SCL --- PB3 ʱ��SCL
    CS Ƭѡ����-ֱ�ӽ�GND��һֱʹ��
-------------------------------------------*/
void LCD_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7);
	GPIO_Init(GPIOB,&GPIO_InitStruct);

    SPI3_LCD_Init();//��SPI��صĹ��ܳ�ʼ��
}

/*------------------------------------------
    LCD�ײ�SPI�������ݺ���
    data:������ʼ��ַ
-------------------------------------------*/
static void LCD_SPI_Send(u16 data,u8 size)
{
    SPI3_WriteData(data,size);
}

/*--------------------------------
    д���LCD��Ļ
    cmd����Ҫ���͵�����
----------------------------------*/
static void LCD_Write_Cmd(u8 cmd)
{
    LCD_DC(0);              //����DC���ţ�ѡ�������ģʽ
    LCD_SPI_Send((u8)cmd,1);  //��������
}

/*----------------------------------
    д���ݵ�LCD��Ļ
    data ��Ҫд������
-----------------------------------*/
static void LCD_Write_Data(u8 data)
{
    LCD_DC(1);                  //����DC���ţ�ѡ�����ݷ���ģʽ
    LCD_SPI_Send((u8 )data, 1); //��������
}

/*----------------------------------------
    д����ֵ�LCD
-----------------------------------------*/
void LCD_Write_HalfWord(const u16 data)
{
    LCD_DC(1);
    LCD_SPI_Send(data, 2);
}

/*-----------------------------------------------------
    ��������д��LCD��������
    x1,y1	�������
    x2,y2	�յ�����
-------------------------------------------------------*/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+52);
		LCD_Write_HalfWord(x2+52);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+53);
		LCD_Write_HalfWord(x2+53);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+53);
		LCD_Write_HalfWord(y2+53);
		LCD_Write_Cmd(0x2c);//������д
	}
	else
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+52);
		LCD_Write_HalfWord(y2+52);
		LCD_Write_Cmd(0x2c);//������д
	}
}

/*��LCD��ʾ*/
void LCD_DisplayOn(void)
{
    LCD_PWR(0);
}

/*�ر�LCD��ʾ*/
void LCD_DisplayOff(void)
{
    LCD_PWR(1);
}

//ʹ��color��ɫ��������
void LCD_Clear(u16 color)
{
    LCD_Fill(0,0,LCD_Width,LCD_Height,color);
}

/*------------------------------------------------------------
    ��һ����ɫ��color�������������
    x_start��y_start:��ʼ������
    x_end��y_end����ֹ������
    color��������ɫ
--------------------------------------------------------------*/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_Write_HalfWord(color);
		}
	} 					  	    
}

/*------------------------------------------------
    ���㺯����һ�����صĵ�̫С�ˣ���������������
    �����ɫ�����Զ���
--------------------------------------------------*/
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(color);
}

/*---------------------------------------------------------------------
    ���ߺ�����ֱ�ߡ�б�ߣ�
    x_start,y_start:�ߵ���ʼλ��
    x_end��y_end���ߵ���ֹ��
    color���ߵ���ɫ
------------------------------------------------------------------------*/
void LCD_Draw_Line(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x_end-x_start; //������������ 
	delta_y=y_end-y_start;
	uRow=x_start;//�����������
	uCol=y_start;

	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}

	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}

	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_Draw_ColorPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/*---------------------------------------------------------------
      ����˵����������
      ������ݣ� x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
----------------------------------------------------------------*/
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_Draw_Line(x1,y1,x2,y1,color);
	LCD_Draw_Line(x1,y1,x1,y2,color);
	LCD_Draw_Line(x1,y2,x2,y2,color);
	LCD_Draw_Line(x2,y1,x2,y2,color);
}

/*--------------------------------------------------
      ����˵���� ��Բ
      ������ݣ� x,y   Բ������
                r       �뾶
                color   Բ����ɫ
-----------------------------------------------------*/
void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_Draw_ColorPoint(x-b,y-a,color);             //3           
		LCD_Draw_ColorPoint(x+b,y-a,color);             //0           
		LCD_Draw_ColorPoint(x-a,y+b,color);             //1                
		LCD_Draw_ColorPoint(x-a,y-b,color);             //2             
		LCD_Draw_ColorPoint(x+b,y+a,color);             //4               
		LCD_Draw_ColorPoint(x+a,y-b,color);             //5
		LCD_Draw_ColorPoint(x+a,y+b,color);             //6 
		LCD_Draw_ColorPoint(x-b,y+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/*-------------------------------------------------------------------------------
    ����˵��������һ���ַ�
    ������ݣ�x��y ��ʾλ��
             chr  Ҫ��ʾ���ַ�
             size �ַ���С��12��16��24��32��Ҫ��ǰ������Ӧ��С���ֿ⣩
             point_color �ַ���ɫ
             back_color  ������ɫ
--------------------------------------------------------------------------------*/
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size,u16 point_color,u16 back_color)
{
    u8 temp, t1, t;
    u8 csize;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    u16 colortemp;
    u8 sta;

    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

    if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

    LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == 16) || (size == 32) )	    //16��32������
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)
                temp = asc2_1608[chr][t];	//����1608����
            else if(size == 32)
                temp = asc2_3216[chr][t];	//����3216����
            else 
                return;			            //û�е��ֿ�
            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) 
                    colortemp = point_color;
                else 
                    colortemp = back_color;
                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
	else if(size == 12)	//12������
	{
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];
            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = point_color;
                else colortemp = back_color;
                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
	else if(size == 24)		//24������
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];
            if(t % 2 == 0)
                sta = 8;
            else 
                sta = 4;
            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) 
                    colortemp = point_color;
                else 
                    colortemp = back_color;
                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------------------
    ����˵���������ַ���
    ������ݣ�x��y ��ʾλ��
             width:��ʾ���  height����ʾ�߶�
             size �ַ���С��12��16��24��32��Ҫ��ǰ������Ӧ��С���ֿ⣩
             *p���ַ���ָ��
             point_color �ַ���ɫ
             back_color  ������ɫ
----------------------------------------------------------------------------------------------------------*/
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p,u16 point_color,u16 back_color)
{
    u8 x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }
        if(y >= height)
            break; //�˳�
        LCD_ShowChar(x, y, *p, size, point_color, back_color);
        x += size / 2;
        p++;
    }
}


static u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while(n--)result *= m;
    return result;
}
/*----------------------------------------------------------------------------------
    �������ܣ���ʾ����
    x,y:��ʾλ��
    num��Ҫ��ʾ������,���ַ�Χ(0~4294967295)
    len����Ҫ��ʾ��λ��
    size:�����С
    piont_color:������ɫ
    back_color:������ɫ
--------------------------------------------------------------------------------------*/
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size,u16 point_color,u16 back_color)
{
    u8 t, temp;
    u8 enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size,point_color,back_color);
                continue;
            }
            else enshow = 1;
        }
        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size,point_color,back_color);
    }
}

/*---------------------------------------------------------------------------------------
      ����˵������ʾ��λС��
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��(������С����)
                size �ֺ�
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ               
----------------------------------------------------------------------------------------*/
void LCD_ShowFloatNum(u16 x,u16 y,float num,u8 len,u8 size,u16 point_color,u16 back_color)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=size/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/LCD_Pow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',size,point_color,back_color);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,size,point_color,back_color);
	}
}

/*------------------------------------------------------------------------
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
--------------------------------------------------------------------------*/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_Write_Data(pic[k*2]);
			LCD_Write_Data(pic[k*2+1]);
			k++;
		}
	}			
}

/******************************************************************************
      ����˵������ʾ����12x12����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ
                size �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(size/8+((size%8)?1:0))*size;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+size-1,y+size-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))
                            LCD_Write_HalfWord(point_color);
						else 
                            LCD_Write_HalfWord(back_color);
						m++;
						if(m%size==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,point_color);//��һ����
						x++;
						if((x-x0)==size)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ
                size �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
    TypefaceNum=(size/8+((size%8)?1:0))*size;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+size-1,y+size-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_Write_HalfWord(point_color);
						else LCD_Write_HalfWord(back_color);
						m++;
						if(m%size==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,point_color);//��һ����
						x++;
						if((x-x0)==size)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ
                size �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(size/8+((size%8)?1:0))*size;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+size-1,y+size-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_Write_HalfWord(point_color);
						else LCD_Write_HalfWord(back_color);
						m++;
						if(m%size==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,point_color);//��һ����
						x++;
						if((x-x0)==size)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ
                size �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(size/8+((size%8)?1:0))*size;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+size-1,y+size-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_Write_HalfWord(point_color);
						else LCD_Write_HalfWord(back_color);
						m++;
						if(m%size==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,point_color);//��һ����
						x++;
						if((x-x0)==size)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                point_color �ֵ���ɫ
                back_color �ֵı���ɫ
                size �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 point_color,u16 back_color,u8 size,u8 mode)
{
	while(*s!=0)
	{
		if(size==12) LCD_ShowChinese12x12(x,y,s,point_color,back_color,size,mode);
		else if(size==16) LCD_ShowChinese16x16(x,y,s,point_color,back_color,size,mode);
		else if(size==24) LCD_ShowChinese24x24(x,y,s,point_color,back_color,size,mode);
		else if(size==32) LCD_ShowChinese32x32(x,y,s,point_color,back_color,size,mode);
		else return;
		s+=2;
		x+=size;
	}
}
//LCD��ʼ��
void LCD_Init(void)
{
    LCD_PWR(0);
    delay_ms(120);
    LCD_RST(0);
    delay_ms(120);
    LCD_RST(1);
    delay_ms(120);

    LCD_Write_Cmd(0x11);//�˳�˯��ģʽ
    delay_ms(120);
    /* Memory Data Access Control */
    LCD_Write_Cmd(0x36);
		if(USE_HORIZONTAL==0)LCD_Write_Data(0x00);
		else if(USE_HORIZONTAL==1)LCD_Write_Data(0xC0);
		else if(USE_HORIZONTAL==2)LCD_Write_Data(0x70);
		else LCD_Write_Data(0xA0);

    /* RGB 5-6-5-bit  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x05);

    /* Porch Setting */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);

    /*  Gate Control */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x35);

    /* VCOM Setting */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x19);   //Vcom=1.625V

    /* LCM Control */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);

    /* VDV and VRH Command Enable */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);

    /* VRH Set */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x12);

    /* VDV Set */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);

    /* Frame Rate Control in Normal Mode */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);	//60MHZ

    /* Power Control 1 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);

    /* Positive Voltage Gamma Control */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);

    /* Negative Voltage Gamma Control */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);

    /* Display Inversion On */
    LCD_Write_Cmd(0x21);

    LCD_Write_Cmd(0x29);

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    /*����ʾ*/
    LCD_PWR(1);
}


