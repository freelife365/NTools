#include "dialogWTL.h"

extern CAppModule _Module;

namespace wuc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	dialogWTL��	2015-1-20
//	ģʽ����ģʽ�Ի���
//	ע�⣺ʹ�ô���Ҫ��ϵͳ���붨��һ���Ի�����Դ
////////////////////////////////////////////////////////////////////////////////////////////////////////
//template <class T>
//LRESULT dialogWTL<T>::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
//{
//	bHandled = FALSE;
//	return S_OK;
//}
//void dialogWTL::onFinalMessage(HWND hWnd)
//{
//
//}
//LRESULT dialogWTL::onInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//{
//	// register object for message filtering and idle updates
//	CMessageLoop* pLoop = _Module.GetMessageLoop();
//	ATLASSERT(pLoop != NULL);
//	pLoop->AddMessageFilter(this);
//	pLoop->AddIdleHandler(this);
//
//	UIAddChildWindowContainer(m_hWnd);
//
//	return S_OK;
//}
//LRESULT dialogWTL::onDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//{
//	CMessageLoop* pLoop = _Module.GetMessageLoop();
//	ATLASSERT(pLoop != NULL);
//	pLoop->RemoveMessageFilter(this);
//	pLoop->RemoveIdleHandler(this);
//
//	return S_OK;
//}
//BOOL dialogWTL::PreTranslateMessage(MSG* pMsg)
//{
//	return CWindow::IsDialogMessage(pMsg);
//}
//BOOL dialogWTL::OnIdle()
//{
//	UIUpdateChildWindows();
//	return FALSE;
//}
//
//BEAnswerState dialogWTL::getAnswerState() const
//{
//	return m_beAnswerState;
//}
//void dialogWTL::setAnswerState(BEAnswerState state)
//{
//	m_beAnswerState = state;
//}


}	// end wuc