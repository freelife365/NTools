#pragma once
#include "ucdef.h"

namespace wuc{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	drawManager��	2014-07-30
//	���ڵĻ�ͼ������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class drawManager
{
public:
	drawManager(void);
	~drawManager(void);


protected:
	static HINSTANCE m_hInstance;	// �ڴ����Դ���ʵ��
	HWND		m_hWndPaint;
	HWND		m_hwndTooltip;
	TOOLINFO	m_ToolTip;

	//controlUC*	m_pRoot;
	//controlUC*	m_pFocus;
	//controlUC*	m_pEventHover;
	//controlUC*	m_pEventClick;
	//controlUC*	m_pEventKey;

};

}