#pragma once
#include "ucdef.h"
#include "windowUC.h"
namespace gtc{
class controlUC
{
public:
	controlUC(void);
	virtual ~controlUC(void);
public:
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void Event(BSEventUC &event);
	virtual const std::wstring& getName() const;
	virtual void setName(PCWSTR pstrName);

	virtual void paint(HDC hdc);
	virtual void paintBkColor(HDC hdc);
	virtual void paintBkImage(HDC hdc);
	virtual void paintStatusImage(HDC hdc);
	virtual void paintText(HDC hdc);
	virtual void paintBorder(HDC hdc);

	virtual bool activate();
	virtual windowUC* getManager();
	virtual void setManager(windowUC *pManager, controlUC *pParent);
	virtual controlUC* getParent() const;

	// �ı����
	virtual PCWSTR getText() const;
	virtual void setText(PCWSTR pstrText);
	// 
	DWORD getBkColor() const;
	void setBkColor(DWORD dwBackColor);
	DWORD getBorderColor() const;
	void setBorderColor(DWORD dwBorderColor);
	DWORD getFocusBorderColor() const;
	void setFocusBorderColor(DWORD dwBorderColor);
	int getBorderSize() const;
	void setBorderSize(int nSize);
	SIZE getBorderRound() const;
	void setBorderRound(SIZE cxyRound);
	// 
	virtual int getMinWidth() const;
	virtual void setMinWidth(int cx);
	virtual int getMaxWidth() const;
	virtual void setMaxWidth(int cx);
	virtual int getMinHeight() const;
	virtual void setMinHeight(int cy);
	virtual int getMaxHeight() const;
	virtual void setMaxHeight(int cy);
	// һЩ��Ҫ������
	virtual bool isVisible() const;
	virtual void setVisible(bool bVisible = true);
	virtual bool isEnabled() const;
	virtual void setEnabled(bool bEnable = true);
	virtual bool isMouseEnabled() const;
	virtual void setMouseEnabled(bool bEnable = true);
	virtual bool isFocused() const;
	virtual void setFocus(bool bFocus = true);
	virtual bool isFloat() const;
	virtual void setFloat(bool bFloat = true);

	void invalidate();
	bool isUpdateNeeded() const;
	void needUpdate();
	void needParentUpdate();
	// �ͻ�����
	RECT getPosition() const;
	virtual void setPosition(RECT &pos);
	/*****************************************************************************************
		<< ---  groupUC::ptIn		2014-06-24 --- >>
		˵�����鿴���Ƿ��ڿؼ���
		������
		pt = �ο��㣨�ͻ����꣩
		����ֵ���Ƿ���
	*****************************************************************************************/
	bool ptIn(const POINT &pt);

	void clientToScreen(RECT *prect);
	void clientToScreen(POINT *ppt);

	// �˵�
	virtual bool isContextMenuUsed() const;
	virtual void setContextMenuUsed(bool bMenuUsed);
	// �����ʾ
	virtual std::wstring getToolTip() const;
	virtual void setToolTip(PCWSTR pstrText);
	// ��ݼ�
	virtual TCHAR getShortcut() const;
	virtual void setShortcut(TCHAR ch);

protected:
	windowUC		*m_pwndManager;		// �ؼ���������
	std::wstring	m_wsName;			// �ؼ����ƣ�ͬһ�����ڵĿؼ����Ʊ���Ψһ
	controlUC		*m_pParent;			// �ؼ��ĸ��ؼ�

	RECT			m_rcItem;			// �ؼ���m_pWnd�ڵ��߼�����
	SIZE			m_cxyMin;
	SIZE			m_cxyMax;

	std::wstring	m_wsText;			// �ؼ�����
	DWORD			m_dwBackColor;
	DWORD			m_dwBorderColor;
	DWORD			m_dwFocusBorderColor;
	bool			m_bVisible;			// 
	bool			m_bFocused;			// 
	bool			m_bEnabled;			// 
	bool			m_bFloat;
	bool			m_bMouseEnabled;
	bool			m_bUpdateNeeded;	// �Ƿ���Ҫ���»���
	bool			m_bMenuUsed;		// ֧�ֵ���ʽ�˵�		
	
	std::wstring	m_wsToolTip;
	wchar_t			m_chShortcut;

	int				m_nBorderSize;
	SIZE			m_cxyBorderRound;
};



}