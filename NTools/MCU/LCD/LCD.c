/******************************************************************************
-- ��LCD�豸����ʾ���� 
-- 
-- 1. �豸����ʾ�ַ�������_LCD_ROWS_��ÿ������ʾ���ٸ��ַ�_LCD_COLS_
-- 2. �ǵ�һ���ַ�Ϊ���Ͻǣ�����Ϊ(0,0)
--	xu
-- 2013-08-15
*******************************************************************************/
#include "lcd.h"
#include <intrins.H>
#include <stdio.h>
#include "D:\VSTOOLS\MCU\MCUDefine.h"


/*********************************************************************
-- ���ܣ���LCD�豸����ָ��
-- ������cmd=ָ��
-- ���أ�
-- ������xu,2013-08-14
**********************************************************************/
void LCD_WriteCMD(BYTE cmd)
{
	LCDRS = 0;
	LCDRW = 0;
	LCDEN = 0;
	_nop_();
	_nop_();
	P0 = cmd;
	__NOP4__
	LCDEN = 1;
	__NOP4__
	LCDEN = 0;
	//Delay(10);
	__NOP4__
   /*
	//   while(lcd_busy());
    LCD_RS = 0;
//    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10);*/
}

/*********************************************************************
-- ���ܣ��ڵ�ǰλ����ʾһ���ַ�
-- ������chr=Ҫ��ʾ���ַ�
-- ���أ�
-- ������xu,2013-08-16
**********************************************************************/
void LCD_DisplayChar(char chr)
{
	LCDRS = 1;
	LCDRW = 0;
	LCDEN = 0;
	P0 = chr;
	__NOP4__
	LCDEN = 1;
    __NOP4__
    LCDEN = 0;
	__NOP4__
    //Delay(10);
	  /*
//   while(lcd_busy());
    LCD_RS = 1;
//    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10); */
}

/*********************************************************************
-- ���ܣ���ʼ��LCD�豸
-- ������
-- ���أ�
-- ������xu,2013-08-14
**********************************************************************/
void LCD_Init()
{
	MX_Delayms(15);
	LCD_WriteCMD(0x01);
	LCD_WriteCMD(0x38);
	MX_Delayms(5);
	LCD_WriteCMD(0x38);
	MX_Delayms(5);
	LCD_WriteCMD(0x38);
	MX_Delayms(5);
	LCD_WriteCMD(0x08);
	MX_Delayms(5);

	LCD_WriteCMD(0x01);
	MX_Delayms(5);
	LCD_WriteCMD(0x06);
	MX_Delayms(5);
	LCD_WriteCMD(0x0C);
	MX_Delayms(5);
	/*LCD_WriteCMD(0x01);
	MX_Delayms(5);
	LCD_WriteCMD(0x06);
	MX_Delayms(5);
	LCD_WriteCMD(0x0C);
	MX_Delayms(5); */
		 /*
    delay1(15);   
    lcd_wcmd(0x01);      //���LCD����ʾ����            
    lcd_wcmd(0x38);      //16*2��ʾ��5*7����8λ����
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);

    lcd_wcmd(0x0c);      //����ʾ������ʾ���  
    delay1(5);

    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay1(5);	 */
}

/*********************************************************************
-- ���ܣ���ʼ��LCD�豸���ַ�λ������
-- ������x=ˮƽ�ַ�λ��
-- 		 y=��ֱ�ַ�λ��
-- ���أ�ture=��λ�ɹ���false=��λʧ��
-- ������xu,2013-08-14
**********************************************************************/
BOOL LCD_Position(int x, int y)
{
	LCD_WriteCMD(0x80+(y==1?0x40:0x00)+x);
	return 1;
}


/*********************************************************************
-- ���ܣ���LCD�豸����ַ������Ͻǵ�һ���ַ�����Ϊ(0,0)
-- ������xstart=�ַ�ˮƽλ�����
-- 		 ystart=�ַ���ֱλ�����
-- 		 pstring=��Ҫ��ʾ���ַ�
-- 		 cbstring=��Ҫ��ʾ���ַ���
-- ���أ�
-- ������xu,2013-08-14
**********************************************************************/
BOOL LCD_TextOut(int xstart, int ystart, const char* pstring, int cbstring)
{
	int idx, cb;
	if(ystart >= _LCD_ROWS_)
		return 0;  
	idx = 0;
	cb = cbstring > _LCD_COLS_ ? _LCD_COLS_ : cbstring;
	LCD_Position(xstart, ystart);
//	LCD_WriteCMD(0x80);
	//LCD_DisplayChar('M');
	 // lcd_wdat(0x3a);
	for(idx = 0; idx < cb; idx++)
	{
		LCD_DisplayChar(pstring[idx]);
		//MX_Delayms(10);
	}
	return 1;
}

/*********************************************************************
-- ���ܣ���LCD�豸��ʮ����������ַ�������ַ���Ϊcbdata*2�����Ͻǵ�һ���ַ�����Ϊ(0,0)
-- ������xstart=�ַ�ˮƽλ�����
-- 		 ystart=�ַ���ֱλ�����
-- 		 vpdata=��Ҫ��ʾ���ַ�
-- 		 cbdata=��Ҫ��ʾ���ַ���
-- ���أ�
-- ������xu,2013-08-14
**********************************************************************/
BOOL LCD_TextOutHex(int xstart, int ystart, const BYTE* vpdata, int cbdata)
{
	int idx, cb;
	if(ystart >= _LCD_ROWS_)
		return 0;  
	idx = 0;
	cb = cbdata > _LCD_COLS_ ? _LCD_COLS_ : cbdata;
	LCD_Position(xstart, ystart);
//	LCD_WriteCMD(0x80);
	//LCD_DisplayChar('M');
	 // lcd_wdat(0x3a);
	for(idx = 0; idx < cb; idx++)
	{
		LCD_DisplayChar(mx_toHexChar(vpdata[idx]>>4));
		LCD_DisplayChar(mx_toHexChar(vpdata[idx]&0xF));
		//MX_Delayms(10);
	}
	return 1;
}

/*********************************************************************
-- ���ܣ���LCD�豸����ַ����Ҷ��롣���Ͻǵ�һ���ַ�����Ϊ(0,0)
-- ������xstart=�ַ�ˮƽλ����㣬����0
-- 		 ystart=�ַ���ֱλ����㣬����0
-- 		 xspace=��������ʾ���ַ���
-- 		 cbstring=��Ҫ��ʾ����ֵ
-- ���أ�
-- ������xu,2013-08-22
**********************************************************************/
BOOL LCD_ValueOut(int xstart, int ystart, BYTE xspace, int val)
{
	int len = 0;
	char buf[_LCD_COLS_];
	int offset = 0;
	if(xspace <= 0)
		return 0;
	len = sprintf(buf, "%d", val);
	if(len > xspace)
	{
		offset = len-xspace;
		len = xspace;
	}
	LCD_TextOut(xstart-len, ystart, buf+offset, len);
	return 1;
}

BOOL LCD_ValueOut1(int xstart, int ystart, BYTE xspace, float val, BYTE dots)
{
	int len = 0;
	char buf[_LCD_COLS_];
	char fmt[8];
	int offset = 0;
	if(xspace <= 0)
		return 0;
	if(dots > 0)
	{
		len = sprintf(fmt, "%%0.%df", (int)dots);
		//LCD_TextOut(0,0,fmt,len);
		//LCD_ValueOut(0,1,8,len);
		len = sprintf(buf, fmt, val);
	}
	else
	{
		len = sprintf(buf, "%d", (int)val);
	}
	if(len > xspace)
	{
		offset = len-xspace;
		len = xspace;
	}
	LCD_TextOut(xstart-len, ystart, buf+offset, len);
	return 1;
}