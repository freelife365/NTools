
// testNedmalloc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestNedmallocApp:
// �йش����ʵ�֣������ testNedmalloc.cpp
//

class CtestNedmallocApp : public CWinApp
{
public:
	CtestNedmallocApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestNedmallocApp theApp;