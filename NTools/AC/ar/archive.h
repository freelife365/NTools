#pragma once

#include "AC/sysupport.h"

namespace gtc {

//////////////////////////////////////////////////////////////////////////
// archive�� 2014-08-24
// ���л�����
//////////////////////////////////////////////////////////////////////////
class archive
{
public:
	// ����ģʽ��д����ȡ
	typedef enum BEMode { BEModeStore = 0, BEModeLoad = 1 } BEMode;
	// ���ݸ�ʽ
	typedef enum BEFormat { BEFormatBinary = 0, BEFormatXML } BEFormat;
protected:
	archive(const archive &ar);
	archive& operator =(const archive &ar);
public:
	
public:
	// ����
	bool isStoring() const;
	bool isLoading() const;

	
protected:
	BEMode		m_beMode;		// ����ģʽ
	BEFormat	m_beFormat;		// ���ݸ�ʽ
};

}