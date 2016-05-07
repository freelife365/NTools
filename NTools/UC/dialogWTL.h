#ifndef __DIALOGWTL_H__
#define __DIALOGWTL_H__
#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include "wucdef.h"
#include "wucContainer.h"

extern CAppModule _Module;

namespace wuc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	dialogWTL��	2015-1-20
//	ģʽ����ģʽ�Ի���
//	ע�⣺ʹ�ô���Ҫ��ϵͳ���붨��һ���Ի�����Դ
////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T/*, class TBase = CWindow*/>
class dialogWTL : public CDialogImpl<T>, public CUpdateUI<T>,
	public CMessageFilter, public CIdleHandler, public wuc::INotifyUC
{
public:
	BEGIN_UPDATE_UI_MAP(dialogWTL)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(dialogWTL)
		WTL_TO_WUC_MSG_HANDLE()
		WTL_TO_WUC_MSG_FINAL()
		WTL_TO_WUC_MSG_INITDIALOG()
		WTL_TO_WUC_MSG_DESTROY()
	END_MSG_MAP()
//private:
//	class dialogFore : public dialogWTL/*<dialogFore>*/, public wuc::INotifyUC
//	{
//	public:
//		enum { IDD = IDD_MAINDLG };
//	public:
//		dialogFore()
//			:m_clrKey(0x00FF00)
//		{}
//	public:
//		virtual void notify(wuc::BSNotifyUC& msg)
//		{
//
//		}
//		virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
//		{
//			LRESULT lRes = 0;
//			bool bDone = true;
//			switch(uMsg)
//			{
//			case WM_CTLCOLOR:
//				{
//					if(lParam == CTLCOLOR_DLG)
//						return (LRESULT)CreateSolidBrush(m_clrKey);
//				}
//				break;
//			default:
//				bDone = false;
//				break;
//			}
//
//			bHandled = bDone;
//			if(bHandled) return lRes;
//			if(m_drawManager.messageHandler(uMsg, wParam, lParam, lRes)) return lRes;
//			return __super::handleMessage(uMsg, wParam, lParam, bHandled);
//		}
//		virtual BOOL onInitDialog()
//		{
//			__super::onInitDialog();
//
//			// �޸���ʽ
//			ModifyStyle(WS_BORDER | WS_SIZEBOX | WS_DLGFRAME | DS_MODALFRAME|WS_CAPTION|DS_3DLOOK, WS_POPUP);
//			ModifyStyleEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, WS_EX_LAYERED);
//
//			::SetLayeredWindowAttributes(m_hWnd, m_clrKey, 0, LWA_COLORKEY); 
//
//
//			return TRUE;
//		}
//
//		virtual void onDestroy()
//		{
//			__super::onDestroy();
//
//		}
//	public:
//		int showWindow(HWND hWndParent)
//		{
//			if(Create(hWndParent) == NULL)
//			{
//				ATLTRACE(_T("Main dialog creation failed!\n"));
//				return 0;
//			}
//			
//			// center the dialog on the screen
//			RECT rc = {0};
//			::GetWindowRect(GetParent(), &rc);
//			::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rectWidth(rc), rectHeight(rc), SWP_SHOWWINDOW);
//
//			//ShowWindow(SW_SHOWDEFAULT);
//			CMessageLoop* pLoop = _Module.GetMessageLoop();
//			ATLASSERT(pLoop != NULL);
//			//::EnableWindow(hWndParent, FALSE);
//			int nRet = pLoop->Run();
//			//nRet = theLoop.Run();
//			//::EnableWindow(hWndParent, TRUE);
//			return nRet;
//		}
//	protected:
//		drawManager		m_drawManager;
//		COLORREF		m_clrKey;
//	};
public:
	dialogWTL()
		: m_isModel(true)/*, m_beAnswerState(BEAnswerStateNone)*/
		, m_pRoot(nullptr)
		//, m_pforeDialog(nullptr)
	{}
public:
	/****************************************************************************************
		<< --- showWindow		2015-1-20 --- >>         
		˵������ʾģʽ����ģʽ�Ի���
		������
			hWndParent	= ����ʾ���ڵĸ����ھ��
			isModel		= ��ģʽtrue������ģʽfalse�Ի���
			prcParent	= ����ڱ�������У������棩
		����ֵ�������˳���
	*****************************************************************************************/
	int showWindow(HWND hWndParent = ::GetActiveWindow(), bool isModel = true, const RECT *prcParent = NULL)
	{
		int nRet = -1;
		m_isModel = false;
		//m_isModel = isModel;
		//if (m_isModel)
		//{
		//	//CMessageLoop theLoop;
		//	//_Module.AddMessageLoop(&theLoop);
		//	nRet = DoModal(hWndParent);
		//	//_Module.RemoveMessageLoop();
		//}
		//else
		{
			//CMessageLoop theLoop;
			//_Module.AddMessageLoop(&theLoop);

			if(Create(hWndParent) == NULL)
			{
				ATLTRACE(_T("Main dialog creation failed!\n"));
				return 0;
			}
			DWORD dwStyle = GetWindowLongPtr(GWL_STYLE);
			dwStyle &= ~(WS_CAPTION|DS_3DLOOK);
			SetWindowLongPtr(GWL_STYLE, dwStyle);

			if (prcParent)
			{
				RECT rc = {0};
				GetWindowRect(&rc);
				POINT pt = {prcParent->left, prcParent->top};
				pt.x += (rectWidth(*prcParent)-rectWidth(rc))/2;
				pt.y += (rectHeight(*prcParent)-rectHeight(rc))/2;
				SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
				//SEND_LOG(L"dialogWTL::showWindow pos:<%s>", gtc::format::rect(rc, true).c_str());
			}
			ShowWindow(SW_SHOWDEFAULT);

			CMessageLoop* pLoop = _Module.GetMessageLoop();
			ATLASSERT(pLoop != NULL);
			if(isModel) ::EnableWindow(hWndParent, FALSE);
			nRet = pLoop->Run();
			//nRet = theLoop.Run();
			if(isModel) ::EnableWindow(hWndParent, TRUE);

			//_Module.RemoveMessageLoop();
		}
		return nRet;
	}

	/****************************************************************************************
		<< --- closeDialog		2015-1-20 --- >>         
		˵������ͬ�ĶԻ���ģʽ�˳���һ��Ŷ
		������
			beRetCode	= ���ظ��û����˳���
		����ֵ��
	*****************************************************************************************/
	void closeDialog(BEAnswerState beRetCode)
	{
		if (m_isModel)
		{
			EndDialog(int(beRetCode));
		}
		else
		{
			DestroyWindow();
		}
		::PostQuitMessage(int(beRetCode));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-20	���WUC::handleMessage����bHandled�������������ж��û��Ƿ�����˶���Ϣ����
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		bHandled = FALSE;
		//if(!wuc::windowWnd::isOutMsg(uMsg))
		//	SEND_LOG(L"....... msg:%s", wuc::windowWnd::stringMsg(uMsg).c_str() );

		return S_OK;
	}
	virtual void onFinalMessage(HWND hWnd)
	{
		m_hWnd = NULL;	// 
	}
	virtual void notify(wuc::BSNotifyUC& msg)
	{
		if (msg.wsType == L"click" && msg.pSender)
		{
			if (msg.pSender->getName() == UC_DIALOG_BUTTON_OK)
			{
				closeDialog(wuc::BEAnswerStateOK);
				return;
			}
			else if (msg.pSender->getName() == UC_DIALOG_BUTTON_CANCEL)
			{
				closeDialog(wuc::BEAnswerStateCancel);
				return;
			}
			else if (msg.pSender->getName() == UC_DIALOG_BUTTON_CLOSE)
			{
				closeDialog(wuc::BEAnswerStateClose);
				return;
			}
		}
	}
	// ����WUC������Դ
	virtual BOOL onInitDialog()
	{

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		// 
		m_drawManager.init(m_hWnd);
		// ���ؿؼ�
		m_pRoot = new wuc::verticalLayoutUC();
		assert(m_pRoot);
		m_pRoot->setName(L"root");
		m_drawManager.attachDialog(m_pRoot);
		m_drawManager.addNotifier(this);

		//m_pforeDialog = new dialogFore();
		//if(m_pforeDialog) m_pforeDialog->showWindow(m_hWnd);

		return TRUE;
	}
	virtual void onDestroy()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

	}
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-5-11 
	containerUC* getRoot() const {
		return m_pRoot;
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	�趨�Ի����С
	void setWindowSize(SIZE sz) {
		m_drawManager.setInitSize(sz.cx, sz.cy);
		if (m_hWnd)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, sz.cx, sz.cy, /*���ڴ�С*/
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	void setWindowSize(int cx, int cy) {
		SIZE sz = {cx, cy};
		setWindowSize(sz);
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	�趨�����Ƿ�֧��͸��
	void setBackgroundTransparent(bool bTrans = true) {
		m_drawManager.setBackgroundTransparent(bTrans);
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	�趨������
	void setCaption(RECT rcCaption) {
		m_drawManager.setCaptionRect(rcCaption);
	}
	void setCaption(int height) {
		RECT rc = {0,0,0,height};
		setCaption(rc);
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	
	void setBackgroupImage(PCWSTR pbkImage) {
		//m_drawManager.setBackgroupImage(pbkImage);
		m_drawManager.getRoot()->setImage(pbkImage);
		gtc::image *pimgBk = m_drawManager.findImage(pbkImage);
		if(!pimgBk) return ;
		RECT rc = {0,0,pimgBk->getWidth(), pimgBk->getHeight()};
		m_drawManager.getRoot()->setAttribute(L"bkimage", gtc::format::rect(rc).c_str());
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	��ӿؼ�
	void addControl(controlUC *pControl) {
		assert(pControl);
		//if (pControl->getInterface(L"IWnd"))
		//{
		//	//if (!m_pforeDialog)
		//	//{
		//	//	m_pforeDialog = new dialogFore();
		//	//}
		//	assert(m_pforeDialog);
		//	if(m_pforeDialog->getRoot()) m_pforeDialog->getRoot()->add(pControl);
		//	return;
		//}
		m_pRoot->add(pControl);
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-5-10	���ؼ������ҿؼ�
	controlUC* findControl(PCWSTR pName) const {
		assert(pName);
		controlUC *pControl = m_drawManager.findControl(pName);
		//if (!pControl && m_pforeDialog)
		//{
		//	return m_pforeDialog->findControl(pName);
		//}
		return pControl;
	}
	//BEAnswerState getAnswerState() const
	//{
	//	return m_beAnswerState;
	//}
	//void setAnswerState(BEAnswerState state)
	//{
	//	m_beAnswerState = state;
	//}


protected:
	drawManager			m_drawManager;
	verticalLayoutUC	*m_pRoot;
	bool				m_isModel;			// ��ģʽ�Ի�������ģʽ�Ի���

	//dialogFore			*m_pforeDialog;		// �������໯��׼�ؼ�ʱ�Ŵ���
	//BEAnswerState	m_beAnswerState;	// �Ի������Ӧ״̬

};
}	// end wuc
#endif