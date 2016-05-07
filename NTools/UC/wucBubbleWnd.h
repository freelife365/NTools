#ifndef __WUCBUBBLEWND_H__
#define __WUCBUBBLEWND_H__
#include "wucdef.h"
#include "windowWnd.h"
#include "drawManager.h"
#include "wucBase.h"
namespace wuc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	bubbleWnd  ��		2015-8-10
//	����ʽ���ڻ���
//	�رշ�ʽ���رհ�ť��ʧȥ���㡢����ʱ���֣����С��رհ�ť���͡�ʧȥ���㡱����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class bubbleWnd : public windowWnd, public INotifyUC
{
public:
	enum {	BUBBLE_WND_CLOSE_TIMEID		};
public:
	bubbleWnd();
	~bubbleWnd();

	PCWSTR getWindowClassName() const {				return L"BubbleWnd";								}
	//virtual PCWSTR getWindowClassName() const = 0;
	//virtual PCWSTR getWindowCaption() const {		return NULL;										}
	virtual UINT getClassStyle() const {			return CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;			}
	virtual UINT getWindowStyle() const {			return WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;	}
	virtual UINT getWindowExStyle() const {			return WS_EX_TOOLWINDOW|WS_EX_TOPMOST;				}

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void notify(wuc::BSNotifyUC &msg);
	virtual void onFinalMessage(HWND hWnd);

public:
	// ��������ʼ������
	void init(controlUC *pOwner, RECT rcPos, UINT timeElapse = 0);

public:
	virtual void onInitDialog();
	virtual void onClose();
	LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
public:
	//buttonUC* getButtonClose() const;
	verticalLayoutUC* getRoot() const;
	void setExitAtLostfocus(bool bExit) {				m_bExitAtLostfocus = bExit;							}
	bool isExitAtLostfocus() const {					return m_bExitAtLostfocus;							}
protected:
	drawManager			m_drawManager;
	//IWindowUC			*m_pOwner;
	controlUC			*m_pOwner;			// ���������ڵĿؼ���һ�����ڱ�Ĵ���
	UINT				m_uiTimeElapse;			// >0ʱ��������ʱ�˳�
	bool				m_bExitAtLostfocus;		// ʧȥ����ʱ�Ƿ��˳�������
	verticalLayoutUC	*m_pRoot;
	//buttonUC			*m_buttonClose;		// ���رա���ť
};

}	// wuc

#endif	// __WUCBUBBLEWND_H__