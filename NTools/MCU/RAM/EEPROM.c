/******************************************************************************
-- ��Ƭ����صĶ��� 
-- 
--	
--	xu
-- 2013-06-01
*******************************************************************************/
#include <reg52.h>
#include "EEPROM.h"
#include <intrins.h>
/*********************************************************************
-- ���ܣ�����STC89C5xϵ�е�Ƭ���ڲ�EEPROM������������
-- ������
-- ���أ���ǰ���жϿ��ر��
-- xu��2013-06-09
**********************************************************************/
static bit ISP_Enable(void)
{
	bit oldea = EA;
	EA = 0;
	ISP_CONTR &= 0x18;
	ISP_CONTR |= ISP_WAIT_TIME | 0x80;

	return oldea;
}
/*********************************************************************
-- ���ܣ��ر�STC89C5xϵ�е�Ƭ���ڲ�EEPROM������������
-- ������oldea= ISP_Enable()������Χ���жϿ��ر��
-- ���أ�
-- ������xu,2013-06-09
**********************************************************************/
static void ISP_Disable(bit oldea)
{
	ISP_CONTR &= 0x7F;
	ISP_TRIG = 0;
	EA = oldea;
}
/*********************************************************************
-- ���ܣ�STC89C5xϵ�е�Ƭ��ISP,IAP���������
-- ������
-- ���أ�
-- ������xu,2013-06-09
**********************************************************************/
static void ISP_Trig(BYTE cmd)
{
	ISP_CMD &= 0xF8;
	ISP_CMD |= cmd;

	ISP_TRIG = 0x46;
	ISP_TRIG = 0xB9;
	_nop_();
	_nop_();
	//_nop_();
}
/*********************************************************************
-- ���ܣ�STC89C5xϵ�е�Ƭ���ڲ�EEPROM��������
-- ������
-- 		Address��EEPROM���������ĵ�ַ
-- xu��2013-06-08
**********************************************************************/
void ISP_Erase(UINT address)
{
	bit oldea = 0;
	UINT secaddr = address & 0xFE00;
	ISP_ADDRL = 0x0;
	ISP_ADDRH = (BYTE)(secaddr >> 8);
	// 
	//ISP_CMD &= 0xF8;
	//ISP_CMD |= ISP_SECTOR_ERASE;
	// 
	oldea = ISP_Enable();
	ISP_Trig(ISP_SECTOR_ERASE);
	ISP_Disable(oldea);

}		
/*********************************************************************
-- ���ܣ���ȡSTC89C5xϵ�е�Ƭ���ڲ�EEPROM���ֽ�����
-- ������
-- 		Address����EEPROM�ĵ�ַ
-- 		����ֵ����ȡ������
-- xu��2013-06-08
**********************************************************************/
BYTE ISP_Read(UINT address)
{	
	BYTE rtdata = 0;
	bit oldea = 0;
	ISP_ADDRL = (BYTE)(address&0x0FF);
	ISP_ADDRH = (BYTE)(address >> 8);

	//ISP_CMD &= 0xF8;
	//ISP_CMD |= ISP_BYTE_READ;
	oldea = ISP_Enable();
	ISP_Trig(ISP_BYTE_READ);
	ISP_Disable(oldea);

	rtdata = ISP_DATA;
	return rtdata;
}  
/*********************************************************************
-- ���ܣ���STC89C5xϵ�е�Ƭ���ڲ�EEPROM���б��
-- ������
-- 		Address����̵�ַ
--		data����̵�ֵ
-- xu��2013-06-08
**********************************************************************/
void ISP_Write(UINT address, BYTE edata)
{
	bit oldea = 0;
	ISP_ADDRL = (BYTE)(address&0x0FF);
	ISP_ADDRH = (BYTE)(address >> 8);

	//ISP_CMD &= 0xF8;
	//ISP_CMD |= ISP_BYTE_PROGRAM;
	ISP_DATA = edata;
	
	oldea = ISP_Enable();
	ISP_Trig(ISP_BYTE_PROGRAM);
	ISP_Disable(oldea);
}	
/*********************************************************************
-- ���ܣ���STC89C5xϵ�е�Ƭ���ڲ�EEPROM���б�̲���֤
-- ������address = д��ĵ�ַ
--		 edata = д���ֵ
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
BYTE ISP_WriteVerify(UINT address, BYTE edata)
{
	bit oldea = 0;
	ISP_ADDRL = (BYTE)(address & 0x0FF);
	ISP_ADDRH = (BYTE)(address >> 8);

	//ISP_CMD &= 0xF8;
	//ISP_CMD |= ISP_BYTE_PROGRAM;
	ISP_DATA = edata;
	
	oldea = ISP_Enable();
	ISP_Trig(ISP_BYTE_PROGRAM);
	// ���ж�ȡ��֤
	ISP_DATA = 0x0;
	//ISP_CMD &= 0xF8;
	//ISP_CMD |= ISP_BYTE_READ;
	ISP_Trig(ISP_BYTE_READ);
	ISP_Disable(oldea);

	return 	ISP_DATA == edata ? MCU_OK : MCU_ERROR;
}

/*********************************************************************
-- ���ܣ���ָ����ַ��ʼ������д�ֽ����鲢��֤�����������������
-- ������address = д�����ݵ���ʼ��ַ
--		 pvalue = д���ֵ����
--		 len = ֵ���鳤��
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
BOOL ISP_Writes(UINT address, BYTE *pvalue, UINT len)
{
	UINT inaddr = 0, idx = 0;
	bit oldea = 0;
	// ���������������
	if(len > SIZE_OF_SECTOR || pvalue == NULL)
		return MCU_ERROR;
	inaddr = address & 0x1FF;	// ���������ڵ�ƫ�����
	if((inaddr + len) > SIZE_OF_SECTOR)
		return MCU_ERROR;
	
	inaddr = address;
	// ���д�벢У��
	oldea = ISP_Enable();
	for(idx = 0; idx < len; idx++)
	{
		// д��
		ISP_ADDRL = (BYTE)(inaddr & 0x0FF);
		ISP_ADDRH = (BYTE)(inaddr >> 8);
		ISP_DATA = pvalue[idx];

		//ISP_CMD &= 0xF8;
		//ISP_CMD |= ISP_BYTE_PROGRAM;
		ISP_Trig(ISP_BYTE_PROGRAM);
		// ��ȡ��֤
		ISP_DATA = 0x0;
	//	ISP_CMD &= 0xF8;
		//ISP_CMD |= ISP_BYTE_READ;
		ISP_Trig(ISP_BYTE_READ);
		if(ISP_DATA != pvalue[idx])
		{
			ISP_Disable(oldea);
			return MCU_ERROR;
		}
		// 
		inaddr++;
	}
	ISP_Disable(oldea);
	return MCU_OK;
}

/*********************************************************************
-- ���ܣ���ָ����ַ��ʼ��������ȡ���ݣ����������������
-- ������address = ��ȡ���ݵ���ʼ��ַ
--		 pbuf = �������ݻ�����
--		 buflen = ���ݻ���������
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
BOOL ISP_Reads(UINT address, BYTE *pbuf, UINT buflen)
{
	UINT inaddr = 0, idx = 0;
	bit oldea = 0;
	// ���������������
	if(buflen > SIZE_OF_SECTOR || pbuf == NULL)
		return MCU_ERROR;
	inaddr = address & 0x1FF;	// ���������ڵ�ƫ�����
	if((inaddr + buflen) > SIZE_OF_SECTOR)
		return MCU_ERROR;
	
	inaddr = address;
	// ���д�벢У��
	oldea = ISP_Enable();
	for(idx = 0; idx < buflen; idx++)
	{
		// ��ȡ
		ISP_ADDRL = (BYTE)(inaddr & 0x0FF);
		ISP_ADDRH = (BYTE)(inaddr >> 8);
		ISP_DATA = 0;

	//	ISP_CMD &= 0xF8;
	//	ISP_CMD |= ISP_BYTE_READ;
		ISP_Trig(ISP_BYTE_READ);
		pbuf[idx] = ISP_DATA;
		inaddr++;
	}
	ISP_Disable(oldea);
	return MCU_OK;
}