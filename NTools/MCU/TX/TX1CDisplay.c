 /******************************************************************************
 -- TX-1C������������ʾ����
 -- 
 --	
 --	
 -- 
 *******************************************************************************/
#include <reg52.h>
#include "TX1CDisplay.h"
#include "TX1CDefine.h"
//#include "D:\VSTOOLS\MCU\MCUDefine.h"
// ����ܵı����
BYTE code g_btDigitalTubeTable[] = {
	0x3F, 0x06, 0x5B, 0x4F, // 0,1,2,3
	0x66, 0x6D, 0x7D, 0x07, // 4,5,6,7
	0x7F, 0x6F, 0x77, 0x7C, // 8,9,A,B
	0x39, 0x5E, 0x79, 0x71  // C,D,E,F
	};

/***********************************
-- TX-1C������������ʾ����
-- ��ʾ��Χ0~999999������֧���Ժ��ǡ�
-- ���ö�̬��ʾ��T0��Ϊ��ʱ���ṩ��ʾƵ��֧��
-- xu
-- 2013-06-01
***********************************/  
void TX1CDTDisplay(long val)
{
	BYTE delay = 5;
	BYTE flag0 = 7;
	BYTE wei6,wei5,wei4,wei3,wei2,wei1;
	wei6 = val/100000;//val%1000000/100000;
	if(wei6 == 0)
		flag0 = 6;
	wei5 = val%100000/10000;
	if(flag0 == 6 && wei5 == 0)
		flag0 = 5;
	wei4 = val%10000/1000;
	if(flag0 == 5 && wei4 == 0)
		flag0 = 4;
	wei3 = val%1000/100;
	if(flag0 == 4 && wei3 == 0)
		flag0 = 3;
	wei2 = val%100/10;
	if(flag0 == 3 && wei2 == 0)
		flag0 = 2;
	wei1 = val%10;

	
	// ��λ
	//if(wei1 != cwei1)
	{
		SW_WELA = 1;
		P0 = 0xDF;
		SW_WELA = 0;	
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei1];
		SW_DULA = 0;
		P0 = 0xFF;	
		MX_Delayms(delay);
	}
	// ʮλ
	if(flag0 > 2)
	{
		SW_WELA = 1;
		P0 = 0xEF;
		SW_WELA = 0;	
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei2];
		SW_DULA = 0;
		P0 = 0xFF;	
		MX_Delayms(delay);
	}
	// ��λ
	if(flag0 > 3)
	{
		SW_WELA = 1;
		P0 = 0xF7;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei3];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(delay);
	}
	// ǧλ
	if(flag0 > 4)
	{
		SW_WELA = 1;
		P0 = 0xFB;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei4];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5);
	}
	// ��λ
	if(flag0 > 5)
	{
		SW_WELA = 1;
		P0 = 0xFD;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei5];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5);
	}
	// ʮ��λ
	if(flag0 > 6)
	{
		SW_WELA = 1;
		P0 = 0xFE;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[wei6];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5); 
	}

}

/*********************************************************************
-- ���ܣ���������ͨѶ���ر��жϵ�
-- ������pvalue=����ʾ����
--		 len=���ݳ���(��������,�����ʾ3byte)
-- ���أ�
-- ������xu,2013-06-16
**********************************************************************/
void TX1CDTDisplayHex(BYTE *pvalue, BYTE len)
{
	BYTE delay = 5;
	BYTE val = 0;
	// �����ʾ3���ֽ�
	if(len > 3)
		len = 3;

	if(pvalue == NULL || len == 0)
	{
		SW_WELA = 1;
		P0 = 0xDF;
		SW_WELA = 0;	
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[0];
		SW_DULA = 0;
		P0 = 0xFF;	
		MX_Delayms(delay);
		return;
	}
	if(len >= 1)
	{
		val = pvalue[0];
		// 1λ,��4bit
		SW_WELA = 1;
		P0 = 0xDF;
		SW_WELA = 0;	
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val & 0x0F];
		SW_DULA = 0;
		P0 = 0xFF;	
		MX_Delayms(delay);
		
		// 2λ,��4bit
		SW_WELA = 1;
		P0 = 0xEF;
		SW_WELA = 0;	
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val >> 4];
		SW_DULA = 0;
		P0 = 0xFF;	
		MX_Delayms(delay);
	
	}
	if(len >= 2)
	{
		val = pvalue[1];
		// 3λ,��4bit
		SW_WELA = 1;
		P0 = 0xF7;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val & 0x0F];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(delay);
		// 4λ,��4bit
		SW_WELA = 1;
		P0 = 0xFB;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val >> 4];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5);
	}
	if(len >= 3)
	{
		val = pvalue[2];
		// 5λ,��4bit
		SW_WELA = 1;
		P0 = 0xFD;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val & 0x0F];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5);
		// 6λ,��4bit
		SW_WELA = 1;
		P0 = 0xFE;
		SW_WELA = 0;
		SW_DULA = 1;
		P0 = g_btDigitalTubeTable[val >> 4];
		SW_DULA = 0;
		P0 = 0xFF;
		MX_Delayms(5); 
	}
}