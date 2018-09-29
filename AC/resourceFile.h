#ifndef __RESOURCEFILE_H__
#define __RESOURCEFILE_H__
#include "AC/sysupport.h"
#include "AC/image.h"
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	resourceFile  ��		2015-5-27
//	����Դ�ļ��ж�ȡ����
//	�ο�����
//	1��icon�ļ�˵�� https://msdn.microsoft.com/en-us/library/ms997538.aspx
//	2��һ��icon������ http://code.it168.com/v/vcqingsongzhuaquexewenjiantubiao/Icons.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////////
class resourceFile {
public:
	resourceFile(PCWSTR presFile = NULL);
	~resourceFile();

	// ����ICON
	HICON findIcon(UINT iconID);
	HICON findIcon(UINT iconID, int cxIcon, int cyIcon);
	// ����pngͼ��
	bool findPNG(UINT pngID, gtc::image	&imgPng);
	// ����bitmapͼƬ
	HBITMAP findBitmap(UINT bmpID);
public:
	void setResourceFile(PCWSTR presFile);
	std::wstring getResourceFile() const;

private:
	std::wstring	m_wsResfile;	// ��Դ�ļ�
	HMODULE			m_hmdlRes;		// ���ص���Դ�ļ����
};


}	// end gtc
#endif	// end __RESOURCEFILE_H__