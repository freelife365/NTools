#pragma once
#include "AC/sysupport.h"
namespace gtc{
//////////////////////////////////////////////////////////////////////////
// bmpλͼ�ļ�ͷ
#pragma pack(push) //�������״̬ 
#pragma pack(2)//�趨Ϊ4�ֽڶ��� 
typedef struct BSBitmapFileHeader { /* bmfh */

	WORD	wType;			//�ļ����ͣ�������0x424D�����ַ���BM��  
	DWORD	dwSize;			//�ļ���С  
	WORD	wReserved1;		//�����֣���������Ϊ0
	WORD	wReserved2;		//�����֣���������Ϊ0
	DWORD	dwOffBits;		//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���  
}BSBitmapFileHeader;
#pragma pack(pop)//�ָ�����״̬
//////////////////////////////////////////////////////////////////////////
// λͼ��Ϣͷ
typedef struct BSBitmapInfoHeader{  
	DWORD	dwSize;			//��Ϣͷ��С  
	LONG	lWidth;			//ͼ���ȣ�������Ϊ��λ
	LONG	lHeight;		//ͼ��߶ȣ�������Ϊ��λ
	WORD	wPlanes;		//λƽ����������Ϊ1  
	WORD	wBitCount;		//ÿ����λ��  
	DWORD	dwCompression;	//ѹ������  
	DWORD	dwSizeImage;	//ѹ��ͼ���С�ֽ���  
	LONG	lXPelsPerMeter;	//ˮƽ�ֱ���  
	LONG	lYPelsPerMeter;	//��ֱ�ֱ���  
	DWORD	dwClrUsed;		//λͼʵ���õ���ɫ����  
	DWORD	dwClrImportant;	//��λͼ����Ҫ��ɫ����  
}BSBitmapInfoHeader;

//////////////////////////////////////////////////////////////////////////
// ��ɫ�嶨��
typedef struct BSRGBQuad{  
	BYTE rgbBlue;			//����ɫ����ɫ����  
	BYTE rgbGreen;			//����ɫ����ɫ����  
	BYTE rgbRed;			//����ɫ�ĺ�ɫ����  
	BYTE rgbReserved;		//����ֵ  
}BSRGBQuad;


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	bmpFile��	2014-09-29
//	bmp�ļ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class bmpFile
{
public:
	bmpFile(void);
	~bmpFile(void);
	// ��ȡbmp�ļ�����
	bool read(std::wstring wsFullPath);
	// ������ɫ��clrFill��bmp�ļ�
	bool create(std::wstring wsFullPath, LONG width, LONG height, COLORREF clrFill);


public:
	const BSBitmapInfoHeader& getBitmapInfo() const;
protected:
	BSBitmapFileHeader	m_bsFileHeader;
	BSBitmapInfoHeader	m_bsInfoHeader;
	BSRGBQuad			*m_pbsQuad;
	BYTE				*m_pbtData;
};

}