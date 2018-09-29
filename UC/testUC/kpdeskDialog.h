#pragma once
#include "UC/wucdef.h"
#include "UC/windowWnd.h"
#include "UC/drawManager.h"
//using namespace wuc;
class kpdeskDialog: public wuc::windowWnd, public wuc::INotifyUC
{
public:
	kpdeskDialog(void);
	~kpdeskDialog(void);

	virtual PCWSTR getWindowClassName() const {		return L"UCKPDeskDialog";						}
	virtual UINT getClassStyle() const {			return CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS/*UC_CLASSSTYLE_DIALOG*/;					}
	virtual UINT getWindowStyle() const {			return WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS;	}
	//virtual UINT getWindowExStyle() const {			return WS_EX_LAYERED/*WS_EX_TOOLWINDOW*/;						}
protected:
	virtual void notify(wuc::BSNotifyUC& msg);
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);

	LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	wuc::drawManager m_drawManager;
};
