#ifndef __WUCMENU_H__
#define __WUCMENU_H__
#pragma once
#include "wucBase.h"
#include "wucList.h"
#include "windowWnd.h"


namespace wuc {
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-8-25 	
enum BEMenuAlign {
	BEMenuAlignLeft			= 0x0000,			// �ڻ������
	BEMenuAlignRight		= 0x0001,			// �ڻ����Ҳ�
	BEMenuAlignTop			= 0x0002,			// �ڻ�����
	BEMenuAlignBottom		= 0x0004			// ��
};

class menuWnd;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	menuElementUC  ��		2015-8-24
//	�˵���
//	�˵����ǣ��ָ��ߡ��Ӳ˵�������˵�
//		�ָ���	= �ı�Ϊ�ջ�ΪL"-"Ϊ�ָ���
//		�Ӳ˵�	= �����Ӳ˵�
//		����˵�	= ��������ı���Ϊ�ջ�L"-"
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class menuElementUC : public listContainerElementUC
{
public:
	typedef std::vector<menuElementUC *>	PtrArrayType;
	friend menuWnd;
public:
	menuElementUC();
	~menuElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void paint(HDC hdc, const RECT &rcPaint);
	virtual void Event(BSEventUC &event);
	virtual bool activate();
	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	// IListItemUC
	virtual bool select(bool bSelect = true);
	virtual void drawItem(HDC hdc, const RECT& rcItem);
	virtual void drawItemBk(HDC hdc, const RECT& rcItem);
	// 
	bool getNormalImage(RECT &rc);
	void setNormalImage(RECT rcImgCell);
	void setNormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotImage(RECT &rc);
	void setHotImage(RECT rcImgCell);
	void setHotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedImage(RECT &rc);
	void setPushedImage(RECT rcImgCell);
	void setPushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledImage(RECT &rc);
	void setDisabledImage(RECT rcImgCell);
	void setDisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getNormalImageSelected(RECT &rc);
	void setNormalImageSelected(RECT rcImgCell);
	void setNormalImageSelected(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotImageSelected(RECT &rc);
	void setHotImageSelected(RECT rcImgCell);
	void setHotImageSelected(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedImageSelected(RECT &rc);
	void setPushedImageSelected(RECT rcImgCell);
	void setPushedImageSelected(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledImageSelected(RECT &rc);
	void setDisabledImageSelected(RECT rcImgCell);
	void setDisabledImageSelected(LONG left, LONG top, LONG right, LONG bottom);

public:
	menuWnd* getMenuWnd();
	void createMenuWnd();
public:
	buttonUC* createIconButton(PCWSTR pAttr = NULL);
	std::wstring getGroup() const;
	void setGroup(PCWSTR pstrGroupName = NULL);

	void setCmdID(UINT cmdID);
	UINT getCmdID() const;

	void setMsgWnd(HWND hWnd);
	HWND getMsgWnd() const;
//public:
//	gtc::eventSource	OnMenuCommand;
protected:
	UINT			m_uiCmdID;			// �˵�����ID
	std::wstring	m_wsGroupName;		// �˵����ǣ�ͬ��ֻ����һ����ѡ�У�
	menuWnd			*m_pWindow;

	buttonUC		*m_buttonIcon;

	HWND			m_hwndMsg;			// ���ղ˵���Ϣ

};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	menuUC  ��		2015-8-24
//	�˵���װ��
//	m_rcInset���˵��Ի�����������
//	m_rcPadding���˵�������ͼ�������
//	m_bsInfo.iFont���˵����Ĭ������
//	�˵������4̬��DISABLED PUSHED HOT NORMAL SELECTED������ check�˵� option�˵���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class menuUC : public listUC
{
public:
public:
	menuUC();
	~menuUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual int getItemIndex(controlUC* pControl) const ;
	virtual bool setItemIndex(controlUC* pControl, int iIndex);
	virtual bool add(controlUC* pControl);
	virtual bool addAt(controlUC* pControl, int iIndex);
	virtual bool remove(controlUC* pControl);

	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	void setDefaultItemHeight(int height);
	int getDefaultItemHeight() const;
	void setSeparatorColor(DWORD dwClr);
	DWORD getSeparatorColor() const;
public:
	// �����Ӳ˵�
	void createMenuWnd();
	// �����˵�
	bool trackPopupMenu(HWND hParent, LONG x, LONG y, DWORD dwFlag = BEMenuAlignLeft|BEMenuAlignTop);
	// 
	menuElementUC* addSeparator();
	// ��ͨ�˵�������ܴ�����̬ͼ�����ͨ��ť
	menuElementUC* addMenu(UINT cmdID, PCWSTR pText, PCWSTR pbtnAttr = NULL);
	menuElementUC* addMenu(PCWSTR pText, menuElementUC::PtrArrayType &vecSubMenu);

public:
	// ֻ���������
	menuElementUC* createSeparator();
	// ��ͨ�˵�������ܴ�����̬ͼ�����ͨ��ť
	menuElementUC* createMenu(UINT cmdID, PCWSTR pText, PCWSTR pAttrList = NULL);
	// �����˵��������Ӳ˵�
	menuElementUC* createMenu(PCWSTR pText, menuElementUC::PtrArrayType &vecSubMenu);
public:
	POINT getBasePoint() const;
	DWORD getPopAlign() const;

public:
	// �õ���ͨ��ͼ��˵�������
	static std::wstring getNormalAttributeList(PCWSTR pImage, RECT *pNormal, RECT *pHot = NULL, RECT *pPushed = NULL, RECT *pDisabled = NULL);

public:
	////////////////////////////////////////////////////////////////////////////////////////////
	//// 2015-12-14		kp007	���ַ�ʽ���ã�������ģʽ�Ի���ʱ���˵����ڽ��յ�kill_focus��Ϣ�����ڲ��رղ˵����ڣ�Ҳ���Ƿ��ˣ�����OnMenuCommand�����ڵ����У�ģʽ���ڣ�
	//gtc::eventSource	OnMenuCommand;

	
	void registerMessageWnd(HWND hWnd);
	HWND getMessageWnd() const;

	static UINT getSelfRegMsg();
protected:
	POINT	m_ptBase;					// �˵�����λ��
	DWORD	m_dwAlign;					// �˵���������
	int		m_iDefaultItemHeight;		// �˵����Ĭ�ϸ߶�
	DWORD	m_clrSeparator;				// �ָ�����ɫ
	//int		m_iIconAreaWidth;			// ���ͼ��������
	//int		m_iSubiconAreaWidth;		// ���Ӳ˵�ʱ�Ҳ�ͼ�����������

	HWND	m_hwndMsg;					// ���ղ˵���Ϣ

	static UINT	mc_ucMsgMenu;			// ע����Ϣ��wuc�˵�����(post)��������Ϣ
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	menuWnd  ��		2015-8-24
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class menuWnd : public windowWnd
{
public:
	typedef std::vector<menuWnd *> PtrArrayType;
public:
	menuWnd();
	~menuWnd();
	virtual PCWSTR getWindowClassName() const {		return L"menuWnd";								}
	virtual UINT getWindowStyle() const {			return WS_POPUP|WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	}
	virtual UINT getWindowExStyle() const {			return WS_EX_TOOLWINDOW | WS_EX_TOPMOST;		}

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);

	void init(HWND hParent, menuUC *pLayout);
	void init(menuElementUC *pMenuElem);
public:
	//static void AddWnd(menuWnd *pWnd);
	static void closeAll();
	static void closeWnd(HWND hWnd);
public:
	menuUC* getLayout() const;
protected:
	drawManager		m_drawManager;

	menuUC			*m_pLayout;
	menuElementUC	*m_pOwner;

	static PtrArrayType	mc_vecWnd;
};

}	// wuc
#endif	// __WUCMENU_H__