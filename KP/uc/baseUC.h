#pragma once
#include "controlUC.h"
#include "containerUC.h"
#include "AC/image.h"

namespace gtc{

#define UC_TITLE_HEIGHT				26	// ���ڱ���ߵ�Ĭ��ֵ
#define UC_CONTAINER_SPACE_TOP		5	// �����ؼ��ڲ��ڵ㵽�ϱ߿�Ŀ�϶
#define UC_CONTAINER_SPACE_BOTTOM	5	// �����ؼ��ڲ��ڵ㵽�ױ߿�Ŀ�϶
#define UC_CONTAINER_SPACE_LEFT		7	// �����ؼ��ڲ��ڵ㵽��߿�Ŀ�϶
#define UC_CONTAINER_SPACE_RIGHT	10	// �����ؼ��ڲ��ڵ㵽�ұ߿�Ŀ�϶

#define UC_REFERENCE_SPACEX			8	// ����������ұ߿����UC_REFERENCE_SPACEXʱ��Ϊ�ɻ��ο���
#define UC_REFERENCE_SPACEY			8	// �������ϻ��±߿����UC_REFERENCE_SPACEXʱ��Ϊ�ɻ��ο���
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	labelUC��	2014-06-23
//	���ڵı�������������ʼ���ڴ��������棬�Ҹ߶ȹ̶�����ȵ��ڴ��ڵĿ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class labelUC : public controlUC
{
public:
	labelUC(void);
	virtual ~labelUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void Event(BSEventUC &event);
	virtual void paintText(HDC hdc);
public:
	void setTextStyle(UINT uStyle);
	UINT getTextStyle() const;
	void setTextColor(DWORD dwTextColor);
	DWORD getTextColor() const;
	void setDisabledTextColor(DWORD dwTextColor);
	DWORD getDisabledTextColor() const;
	void setFont(int index);
	int getFont() const;
	RECT getTextPadding() const;
	void setTextPadding(RECT rc);
	bool isShowHtml();
	void setShowHtml(bool bShowHtml = true);
protected:
	DWORD	m_dwTextColor;
	DWORD	m_dwDisabledTextColor;
	int		m_iFont;
	UINT	m_uiTextStyle;
	RECT	m_rcTextPadding;
	bool	m_bShowHtml;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	buttonUC��	2014-06-23
//	�Ի水ť
////////////////////////////////////////////////////////////////////////////////////////////////////////
class buttonUC : public labelUC
{
public:
	buttonUC(void);
	virtual ~buttonUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void paintBkColor(HDC hdc);
	virtual void paintBorder(HDC hdc);
	virtual void paintStatusImage(HDC hdc);
	virtual void Event(BSEventUC &event);

	UINT getState() const {			return m_uiState;								}
	void setImage(gtc::imageResource *pimage);
protected:
	UINT				m_uiState;	// ��ť������״̬
	gtc::imageResource	*m_pImage;// ��ť״̬��Ӧ��ͼƬ
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	editUC��	2014-07-15
//	�ı��༭�ؼ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class editWnd;
class editUC : public labelUC
{
	friend editWnd;
public:
	editUC();
	virtual ~editUC() {				//TRACE_CLASS_END(this)								
	}
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	void setEnabled(bool bEnable = true);
	void setText(PCWSTR pstrText);
	void setMaxChar(UINT uMax);
	UINT getMaxChar();
	void setReadOnly(bool bReadOnly);
	bool isReadOnly() const;
	void setPasswordMode(bool bPasswordMode);
	bool isPasswordMode() const;
	void setPasswordChar(TCHAR cPasswordChar);
	wchar_t getPasswordChar() const;

	void setPos(RECT rc);
	void setVisible(bool bVisible = true);

	virtual void Event(BSEventUC &event);

	virtual void paintStatusImage(HDC hdc);
	virtual void paintText(HDC hdc);
protected:
	editWnd* m_pWindow;

	UINT	m_uiMaxChar;
	bool	m_bReadOnly;
	bool	m_bPasswordMode;
	wchar_t m_chPasswordChar;
	UINT	m_uiButtonState;
	//std::wstring m_sNormalImage;
	//std::wstring m_sHotImage;
	//std::wstring m_sFocusedImage;
	//std::wstring m_sDisabledImage;
	DWORD	m_dwEditbkColor;

};



////////////////////////////////////////////////////////////////////////////////////////////////////////
//	editWnd��	2014-07-15
//	�ı��༭����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class editWnd : public windowWnd
{
public:
	editWnd();
	~editWnd() {			//TRACE_CLASS_END(this)								
	}
	void init(editUC* pOwner);
	RECT calPos();

	PCWSTR getWindowClassName() const;
	PCWSTR getSuperClassName() const;
	void onFinalMessage(HWND hWnd);

	LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	editUC* m_pOwner;
	HBRUSH m_hBkBrush;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	scrollBarUC��	2014-07-17
//	��������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class scrollBarUC : public controlUC
{
public:
	scrollBarUC(void);
	virtual ~scrollBarUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void paint(HDC hdc);
	//virtual void paintBorder(HDC hdc);
	//virtual void paintStatusImage(HDC hdc);
	virtual void Event(BSEventUC &event);

	virtual void setPosition(RECT &pos);
public:
	void refreshLayout();
	int getBarSize() const {			return m_iBarSize;								}
	int getScrollRange() const {		return m_iRange;								}
	void setScrollRange(int range);
	int getScrollPos() const {			return m_iScrollPos;							}
	void setScrollPos(int pos);
	int getFixedHeight() const {		return m_iBarSize;								}
	int getFixedWidth() const {			return m_iBarSize;								}
	void setContainer(containerUC *pContainer) {	m_pContainer = pContainer;			}
	containerUC* getContainer() {		return m_pContainer;							}

	void setThumbImage(gtc::imageResource *pImage);
protected:
	enum { 
		//DEFAULT_SCROLLBAR_SIZE = 16,
		DEFAULT_TIMERID = 10
	};

	BYTE		m_btAlpha;		// ͸����[0,255]Խ��Խ͸��
	containerUC	*m_pContainer;	// �����������鴰��
	int			m_iBarSize;		// ��ֱ�������Ŀ�ȣ�ˮƽ�������ĸ߶�
	int			m_iRange;		// ������Χ[0,m_nRange]
	int			m_iScrollPos;	// ��ǰλ��
	bool		m_bHorizontal;	// ˮƽ�����Ǵ�ֱ��

	gtc::imageResource	*m_pThumbImage;			// 
	RECT			m_rtThumb;				// ����λ��
	bool			m_bMoveThumb;			// �Ƿ�����ͨ�������ƶ�
	POINT			m_ptLastMouse;			// ��ʼ��������ǰ������λ��
	int				m_iLastScrollPos;		// ��ʼ��������ǰ��λ��
	int				m_iLastScrollOffset;	// 

	UINT		m_uiState;		// �ؼ�����״̬
};



}