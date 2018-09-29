#ifndef __WUCWND_H__
#define __WUCWND_H__
#pragma once
#include "windowWnd.h"
#include "wucBase.h"

namespace wuc {
class wucWnd;
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-20 �ܵĽӿڣ����ؼ�ӵ���ߵĴ��壨��ؼ����ؼ�ʵ�֡�һ����wucWnd�ഥ��
class IWndXCallback
{
public:
	// IWndXCallback �ӿ�
	// �������յ� WM_SHOWWINDOW����wParam=TRUE ��Ϣ�󴥷������Ը��ؼ��ṩһЩ��ʼ���ݵ�
	virtual void onxInit(wuc::wucWnd *pWnd) {}
	// �������ܵ� WM_SETFOCUS ʱ�����������趨���λ�ã�ѡ���ı���
	virtual void onxSetFocus(wuc::wucWnd *pWnd) {}
	// �������ܵ� WM_KILLFOCUS ʱ���������Եõ��ؼ����ݣ�����ֹ�˳�(bExit = false)
	virtual void onxKillFocus(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */) {}
	// �������ܵ� WM_DESTROY ʱ���������Եõ��ؼ����ݣ�����ֹ�˳�(bExit = false)
	virtual void onxExit(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */) {}
	// �������ܵ� WM_NCDESTROY ʱ������
	virtual void onxFinal(wuc::wucWnd *pWnd) {}

	//// IWndXCallback �ӿ�
	//virtual void onxInit(wuc::wucWnd *pWnd);
	//virtual void onxSetFocus(wuc::wucWnd *pWnd);
	//virtual void onxKillFocus(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */);
	//virtual void onxExit(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */);
	//virtual void onxFinal(wuc::wucWnd *pWnd, HWND hWnd);

};
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-20 ���ظ������ߵĽӿ�
class IEditWndXCallback : public IWndXCallback
{
public:
	// IEditWndXCallback�ӿ�
	// ���ܵ�EN_CHANGE��Ϣʱ����
	virtual void onxEditChanged(wuc::wucWnd *pWnd) {}
	// �����յ�VK_RETURN��ʱ����
	virtual void onxEditReturn(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */) { }
	// �����յ�WM_CHAR��ʱ���������Թ���ĳЩ��Ч�ַ�
	virtual void onxChar(wuc::wucWnd *pWnd, UINT nChar, UINT nFlag, UINT nRepCnt, bool& bHandled) {}

	//// IEditWndXCallback:IWndXCallback �ӿ�
	//virtual void onxEditChanged(wuc::wucWnd *pWnd);
	//virtual void onxEditReturn(wuc::wucWnd *pWnd, bool &bExit /* �ڲ���ʼΪtrue */);
	//virtual void onxChar(UINT nChar, UINT nFlag, UINT nRepCnt, bool& bHandled);
};

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-26 �������໯�ؼ������ӿ�	
class IWndX
{
public:
	// IWndX �ӿ�
	// ���໯�ؼ���ʵ����
	virtual IWndXCallback* getCallback() const = 0;
	virtual void setCallback(IWndXCallback *pCallback) = 0;
	// �õ����໯�ؼ����������ӿڵ�ʵ���ߣ�
	virtual controlUC* getSelf() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	wucWnd  ��		2015-3-20
//	�������κ�wuc�ؼ��Ļ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class wucWnd : public windowWnd
{
public:
	wucWnd();
	~wucWnd();
protected:
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);
//public:
//	virtual LRESULT sendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0L*/) {
//		return sendMessage(uMsg, wParam, lParam);
//	}
//	virtual LRESULT postMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0L*/) {
//		return postMessage(uMsg, wParam, lParam);
//	}
//
//public:
//	HWND getHWND() const {					return __super::getHWND();						}
//	operator HWND() const {					return __super::operator HWND();				}
//	bool isVisible() const {				return __super::isVisible();					}
//	bool isEnabled() const {				return __super::isEnabled();					}
//	void setEnabled(bool enabled = true) {	__super::setEnabled(enabled);					}

public:
	bool modifyStyle(DWORD dwRemove, DWORD dwAdd);
	bool create(IWndX *pOwner, HWND hParent, RECT rcPos, DWORD dwStyle, DWORD dwExStyle = 0);
	IWndX* getOwner() const;

protected:
	IWndX	*m_pOwner;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	editUC��		2015-2-11
//	��ͨ�ı��༭�ؼ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class editWnd;

class editUC : public labelUC, public IWndX
{
	friend editWnd;
public:
	editUC();
	~editUC();
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void setEnabled(bool bEnable = true);
	virtual void setText(PCWSTR pstrText);

	virtual void setPosition(RECT pos);
	virtual void setVisible(bool bVisible = true);
	virtual void setInternVisible(bool bVisible = true);

	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void Event(BSEventUC &event);

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	virtual void paintStatusImage(HDC hdc);
	virtual void paintText(HDC hdc);
public:
	// IWndX �ӿ�
	// ���໯�ؼ���ʵ����
	virtual IWndXCallback* getCallback() const;
	virtual void setCallback(IWndXCallback *pCallback);
	// �õ����໯�ؼ����������ӿڵ�ʵ���ߣ�
	virtual controlUC* getSelf();
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-4 ѡ��ָ����Χ�ڵ��ַ�
	void setSelRange(int iStart, int iEnd);
public:
	//void setOwner(IEditWndXCallback *pOwner);
	//IEditWndXCallback* getOwner() const;
	void setMaxChar(UINT uMax);
	UINT getMaxChar();
	void setReadOnly(bool bReadOnly);
	bool isReadOnly() const;
	void setPasswordMode(bool bPasswordMode);
	bool isPasswordMode() const;
	void setPasswordChar(wchar_t cPasswordChar);
	wchar_t getPasswordChar() const;
	void setMultiLine(bool bMultiLine);
	bool isMultiLine() const;
	void setBorder(bool bBorder);
	bool supportBorder() const;

	bool getNormalImage(RECT &rc);
	void setNormalImage(RECT rcImgCell);
	bool getHotImage(RECT &rc);
	void setHotImage(RECT rcImgCell);
	bool getFocusedImage(RECT &rc);
	void setFocusedImage(RECT rcImgCell);
	bool getDisabledImage(RECT &rc);
	void setDisabledImage(RECT rcImgCell);

	void setNativeEditBkColor(DWORD dwBkColor);
	DWORD getNativeEditBkColor() const;

	void setPromptInfo(PCWSTR pPrompt);
	PCWSTR getPromptInfo() const;
	void setPromptTextColor(DWORD dwColor);
	DWORD getPromptTextColor() const;
protected:
	editWnd*			m_pWindow;
	IEditWndXCallback	*m_pOwner;

	UINT			m_uMaxChar;
	bool			m_bBorder;
	bool			m_bReadOnly;
	bool			m_bPasswordMode;
	bool			m_bMultiLine;
	wchar_t			m_cPasswordChar;
	UINT			m_uButtonState;
	DWORD			m_dwEditbkColor;

	std::wstring	m_wsPromptInfo;			// ��ʾ�û�������룬�磺������...
	DWORD			m_dwPromptTextColor;	// ��ʾ�ļ���ʾɫ
};


}	// end wuc
#endif	// end __WUCWND_H__
