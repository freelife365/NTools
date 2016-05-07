#pragma once
#include "ucdef.h"

//
namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
class controlUC;
//////////////////////////////////////////////////////////////////////////
// �Զ�����̲����ӿ�
class IKeyEvent
{	// nFlag(shift ctrl alt���Ƿ񱻰���
	virtual void onKeyDown(UINT nChar, UINT nFlag, UINT nRepCnt) = 0;
	virtual void onKeyUp(UINT nChar, UINT nFlag, UINT nRepCnt) = 0;
	virtual void onChar(UINT nChar, UINT nFlag, UINT nRepCnt) = 0;
};
//////////////////////////////////////////////////////////////////////////
// �Զ����������ӿ�
class IMouseEvent
{
	virtual void onLButtonDown(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onLButtonUp(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onMouseMove(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onMouseWheel(UINT nFlags, short zDelta, POINT pt, bool& bHandled) = 0;
	virtual void onMouseEntry(bool& bHandled) = 0;
	virtual void onMouseLeave(bool& bHandled) = 0;
	virtual void onMouseHover(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual LRESULT onNcHitTest(POINT pt, bool& bHandled) = 0;

};
//////////////////////////////////////////////////////////////////////////
// �Զ�����ק�ӿ�
class IDrapDrop
{
public:
	//virtual void onDragEnter(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled);
	//virtual void onDragOver(DWORD keyState, POINT pt, bool &bHandled);
	//virtual void onDragLeave(bool &bHandled);
	//virtual void onDrop(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled);
	/*****************************************************************************************
		<< --- virtual IDrapDrop::dragEnter		2014-07-05 --- >>
		˵����׼����ʼ��ק����������Ŀ�괰��ʱ����
		������
		hgdata		= ����ק������
		keyState	= ����״̬MK_CONTROL��MK_SHIFT��MK_ALT��MK_BUTTON��MK_LBUTTON
		pt			= ��ǰ������Ļ����
		bHandled	= �Ƿ����ִ����ק����
		����ֵ��
	*****************************************************************************************/
	virtual void onDragEnter(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled) = 0;
	/*****************************************************************************************
	virtual void onDrop(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled);
		<< --- virtual IDrapDrop::dragOver		2014-07-05 --- >>
		˵����������dragEnter֮�󣬼����������ק���ƶ�������Ѱ��Ŀ���
		������
		keyState	= ����״̬MK_CONTROL��MK_SHIFT��MK_ALT��MK_BUTTON��MK_LBUTTON
		pt			= ��ǰ������Ļ����
		bHandled	= �Ƿ����ִ����ק����
		����ֵ��	*****************************************************************************************/
	virtual void onDragOver(DWORD keyState, POINT pt, bool &bHandled) = 0;
	/*****************************************************************************************
		<< --- virtual IDrapDrop::dragLeave		2014-07-05 --- >>
		˵��������뿪��ק����ʱ���������Դ���ָ�����״̬�Ȳ���
		������
		bHandled	= �Ƿ�ִ�гɹ�
		����ֵ��
	virtual void onDrop(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled);
	*****************************************************************************************/
	virtual void onDragLeave(bool &bHandled) = 0;
	/*****************************************************************************************
		<< --- virtual IDrapDrop::drop		2014-07-05 --- >>
		˵������ק��������β�������罫��ק���ݷ���Ŀ�괰��
		������
		hgdata		= ����ק������
		keyState	= ����״̬MK_CONTROL��MK_SHIFT��MK_ALT��MK_BUTTON��MK_LBUTTON
		pt			= ��ǰ������Ļ����
		bHandled	= �Ƿ�ɹ�ִ����ק����
		����ֵ��
	*****************************************************************************************/
	virtual void onDrop(HGLOBAL hgdata, DWORD keyState, POINT pt, bool &bHandled) = 0;
};
//////////////////////////////////////////////////////////////////////////
// ��ǰ����f
typedef enum BEUCActiveFlag
{
	BEUCActiveFlagNone			= 0,	// 
	BEUCActiveFlagSize			,		// �ı䴰��λ�û�ߴ�
	BEUCActiveFlagSysMenu		,		// �˵�
	BEUCActiveFlagEditTitle		,		// ����������
	BEUCActiveFlagScroll		,		// ����������
	BEUCActiveFlagUserMenu		,		// �鴰���Ҽ��˵�
	BEUCActiveFlagDrapDrop				// ��קͼ��
}BEUCActiveFlag;

static std::wstring toString(BEUCActiveFlag flag)
{
	switch(flag)
	{
	case BEUCActiveFlagNone:
		return L"ActiveNone";
		break;
	case BEUCActiveFlagSize:
		return L"ActiveSize";
		break;
	case BEUCActiveFlagSysMenu:
		return L"ActiveSysMenu";
		break;
	case BEUCActiveFlagEditTitle:
		return L"ActiveEditTitle";
		break;
	}
	return L"";
}
//////////////////////////////////////////////////////////////////////////
// �߽���
typedef enum BEUCBorderFlag {
	BEUCBorderFlagLeft		= 1,	
	BEUCBorderFlagTop		,		
	BEUCBorderFlagRight		,		
	BEUCBorderFlagBottom				
}BEUCBorderFlag;
/////////////////////////////////////////////////////////////////////////////////////
// UC�ؼ��¼�
typedef enum BEUCEvent
{
	BEUCEventFirst = 1,
	BEUCEventKeyBegin,
	BEUCEventKeyDown,
	BEUCEventKeyUp,
	BEUCEventChar,
	BEUCEventSysKey,
	BEUCEventKeyEnd,
	BEUCEventMouseBegin,
	BEUCEventMouseMove,
	BEUCEventMouseLeave,
	BEUCEventMouseEnter,
	BEUCEventMouseHover,
	BEUCEventButtonDown,
	BEUCEventButtonUp,
	BEUCEventRButtonDown,
	BEUCEventDBLClick,
	BEUCEventContextMenu,
	BEUCEventScrollWheel,
	BEUCEventMouseEnd,
	BEUCEventKillFocus,
	BEUCEventSetFocus,
	BEUCEventWindowZize,
	BEUCEventSetCursor,
	BEUCEventTimer,
	BEUCEventNotify,
	BEUCEventCommand,
	BEUCEventLast,
}BEUCEvent;

// Flags used for controlling the paint
#define UCSTATE_FOCUSED      0x00000001
#define UCSTATE_SELECTED     0x00000002
#define UCSTATE_DISABLED     0x00000004
#define UCSTATE_HOT          0x00000008
#define UCSTATE_PUSHED       0x00000010
#define UCSTATE_READONLY     0x00000020
#define UCSTATE_CAPTURED     0x00000040


// Structure for notifications from the system
// to the control implementation.
typedef struct BSEventUC
{
	int Type;
	controlUC* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	wchar_t chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
} BSEventUC;

// Structure for notifications to the outside world
typedef struct BSNotifyUC
{
	std::wstring wsType;
	controlUC* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} BSNotifyUC;

// Listener interface
class INotifyUC
{
public:
	virtual void notify(BSNotifyUC& msg) = 0;	// �������߿ؼ���֪ͨ
};

/////////////////////////////////////////////////
typedef struct BSFontInfoUC
{
	HFONT hFont;
	std::wstring sFontName;
	int iSize;
	bool bBold;
	bool bUnderline;
	bool bItalic;
	TEXTMETRIC tm;
} BSFontInfoUC;

typedef struct BSTimerInfo
{
	controlUC*	pSender;
	UINT		nLocalID;
	HWND		hWnd;
	UINT		uWinTimer;
	bool		bKilled;
}BSTimerInfo;

//// IMessageUC
//class IMessageUC
//{
//public:
//	virtual void onLButtonDown(UINT nFlags, POINT pt, bool& bHandled);
//	virtual void onLButtonUp(UINT nFlags, POINT pt, bool& bHandled);
//	virtual void onMouseMove(UINT nFlags, POINT pt, bool& bHandled);
//	virtual void onMouseWheel(UINT nFlags, short zDelta, POINT pt, bool& bHandled);
//};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	windowUC��	2014-06-21
//	�Ի洰�ڵĻ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class windowUC : public IDrapDrop, public IMouseEvent, public IKeyEvent
{
public:
	enum BESelectMode {
		BESelectModeNone	= 0,	// 
		BESelectModeSize	,		// �߿�˴θı�
		BESelectModeMove			// �����ƶ�
	};
public:
	windowUC();
	virtual ~windowUC();

	virtual void draw(HDC hdc);
public:
	// ��Ļ����
	RECT getWindowPos() const;
	// ��Ļ����
	void setWindowPos(RECT rt);
	// �����ƶ����Ĵ���λ��
	RECT getWindowNcPos() const;

	// ������������
	RECT getClientRect() const;
	RECT getClientTitle() const;
	void clientToScreen(RECT *prect);
	void clientToScreen(POINT *ppt);
	void screenToClient(RECT *prect);
	void screenToClient(POINT *ppt);
	// �鿴���Ƿ��ڱ�������
	bool ptIn(const POINT &pt, bool inclusionNc = false);
	/*****************************************************************************************
		<< --- windowUC::invalidate	2014-06-23 --- >>
		˵�����Ի洰����������������
		������
		rcItem		= ��Ҫ�ػ�����
		isClient	= true �ͻ����꣬false��Ļ����
		����ֵ��
	*****************************************************************************************/
	void invalidate(RECT& rcItem, bool isClient = true);
	void invalidate(controlUC *pUC);
	void invalidate();
	void updateWindow();

	void needUpdate();

	bool activate() const;
	void setActivate(bool bActive = true);
	// �����е�ÿ���ؼ������ڻ�����������ע��
	void addControl(controlUC *pControl);
	// ɾ���ؼ��ڴ��ڵ�ע�᣻ע�⣺����Ҫ�ͷ��ڴ�
	void removeControl(controlUC *pControl);
	// 
	/*****************************************************************************************
		<< --- windowUC::initControl	2014-06-23 --- >>
		˵������ʼ���ؼ�����������Ӧ��ϵ
		������
		pControl	= ����ʼ���Ŀؼ�
		pParent		= �������ؼ�
		����ֵ��
	*****************************************************************************************/
	bool initControl(controlUC *pControl, controlUC *pParent = nullptr);
	// �ҵ����㣨��Ļ���꣩���ڵĿؼ�
	controlUC* findControl(const POINT &pt, bool bLast = true);
	// ���ݿؼ������ҿؼ�
	controlUC* findControl(const std::wstring &wsName);
	/*****************************************************************************************
		<< --- windowUC::getCursor		2014-06-23 --- >>
		˵�����õ����Ե�����λ�õĹ��
		������
		handle	= ���Ե��ǣ�һ����hitTest�õ�
		����ֵ�������
	*****************************************************************************************/
	virtual HCURSOR getCursor(int handle);

public:
	virtual void onKeyDown(UINT nChar, UINT nFlag, UINT nRepCnt);
	virtual void onKeyUp(UINT nChar, UINT nFlag, UINT nRepCnt);
	virtual void onChar(UINT nChar, UINT nFlag, UINT nRepCnt);

public:
	virtual void onLButtonDblClk(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onLButtonDown(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onLButtonUp(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onRButtonDown(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onRButtonUp(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onMouseMove(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onMouseWheel(UINT nFlags, short zDelta, POINT pt, bool& bHandled);
	virtual void onMouseEntry(bool& bHandled);
	virtual void onMouseLeave(bool& bHandled);
	virtual void onMouseHover(UINT nFlags, POINT pt, bool& bHandled);
	virtual LRESULT onNcHitTest(POINT pt, bool& bHandled);
public:
	bool addNotifier(INotifyUC* pControl);
	bool removeNotifier(INotifyUC* pControl);   
	void sendNotify(BSNotifyUC& Msg, bool bAsync = false);
	void sendNotify(controlUC* pControl, PCWSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

public:
	bool setTimer(controlUC* pControl, UINT nTimerID, UINT uElapse);
	bool killTimer(controlUC* pControl, UINT nTimerID);
	void killTimer(controlUC* pControl);
	void removeAllTimers();

protected:
	/*****************************************************************************************
		<< --- windowUC::ptInSizeBox		2014-06-23 --- >>
		˵�����鿴����Ƿ��ڴ��ڱ߽�ĸı�ߴ���϶������������Ӧ�ı���״̬
		������
		pt	= ��������
		����ֵ���õ����Ե���
	*****************************************************************************************/
	virtual int hitTest(const POINT &pt);


	/*****************************************************************************************
		<< --- windowUC::moveTo		2014-06-24 --- >>
		˵�����������ƶ����µ�λ��
		������
		position	= �µ�λ�ã���Ļ���꣩
		����ֵ��
	*****************************************************************************************/
	virtual void moveTo(const RECT &position, bool mustMove = false);
	/*****************************************************************************************
		<< --- windowUC::moveHandleTo		2014-06-24 --- >>
		˵����ͨ���߽�ı䴰�ڵĳߴ�
		������
		handle	= �߽�λ��
		pt		= �߽��µ�λ�ã���Ļ���꣩
		����ֵ��
	*****************************************************************************************/
	virtual void moveHandleTo(int handle, const POINT &pt);

	/*****************************************************************************************
		<< --- windowUC::close		2014-07-17 --- >>
		˵�����˳����ڣ����ͷ���Դ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void close();
public:
	/*****************************************************************************************
		<< --- static windowUC::init		2014-06-23 --- >>
		˵�����Ի洰����������������
		������
		hMain	= ��������
		����ֵ��
	*****************************************************************************************/
	static void init(HWND hMain);

	// ʱ�ӣ������������������ק�Ȳ���
	static VOID CALLBACK doTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
public:
	void setTitle(PCWSTR ptitle);
	void setTitle(std::wstring wstitle);
	PCWSTR getTitle() const {						return m_wsTitle.c_str();				}
	void setTitleHeight(int height) {				m_iTitleHeight = height;				}
	int getTitleHeight() const {					return m_iTitleHeight;					}
	BESelectMode getSelectMode() const {			return m_beSelectMode;					}
	bool isVisible() const;
	bool isEnabled() const;
	void showWindow(bool show = true);
	void setEnabled(bool enabled = true);
	BSFontInfoUC* getDefaultFont();
	void setDefaultFont(PCWSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	bool isLockPos() const {						return m_bLockPos;						}
	void lockPos() {								m_bLockPos = true;						}
	void unlockPos() {								m_bLockPos = false;						}
	BEUCActiveFlag getActiveFlag() const {			return m_beActive;						}
	void clearActiveFlag() {						m_beActive = BEUCActiveFlagNone;		}
	bool isFocus() const;
	void setFocus(bool bFocus = true);
	bool isInitial() const;
	int getOperateHandle() const;
	gtc::imageResource* getBkImage() const;
	void setBkImage(gtc::imageResource *pImage);
	gtc::imageResource* getBkImageSelected() const;
	SIZE getRoundCorner() const;
	void setBkImageSelected(gtc::imageResource *pImage);
protected:
	static HWND		mc_hWnd;	// �����Ļ�ͼ���ڣ��磺���洰��
	static std::vector<BSTimerInfo *>	mc_vecTimers;	// ʱ�Ӽ���
	static UINT		mc_uTimerID;			// ʵ��ʱ��ID
	
	BEUCActiveFlag	m_beActive;			// ��ǰ�������

	POINT			m_lastMousePos;		// ��굱ǰ����
	controlUC		*m_pEventClick;		// ��ǰ�¼�������
	controlUC		*m_pEventHover;		// ��ǰ�¼�������

	std::vector<controlUC *>	m_vecControl;	// ���е����ã����������ͷ�
	std::vector<INotifyUC *>	m_vecNotifys;	// ����֪ͨ��Ϣ�Ĵ��ڵ����ã����������ͷ�
	std::wstring	m_wsTitle;			// ���ڱ���

	int				m_iOperateHandle;	// ��ǰ�����������
	int				m_iTitleHeight;		// ���ڱ������߶�
	RECT			m_rcSizeBox;		// ���ڳߴ���Ըı䣬��һ�߲����϶�������Ϊ0
	BYTE			m_nAlphaClient;		// �ͻ���͸����
	BYTE			m_nAlphaTitle;		// ������͸����

	RECT			m_rtPos;			// ���ڵ���Ļ����λ��
	bool			m_bUpdateNeeded;	// �Ƿ���Ҫ���»���
	bool			m_bActivate;		// ��ǰ�Ƿ��ڻ״̬
	bool			m_bLockPos;			// �����Ĵ��ڲ�����λҲ���ܸı��С
	bool			m_bFocus;			// �õ����
	BESelectMode	m_beSelectMode;		// ��ǰ�Ĳ���ģʽ

	bool			m_bInitial;			// ����û�в���ǰ��״̬�������entry���false
	bool			m_bVisible;
	bool			m_bEnabled;
	BSFontInfoUC	m_bsDefaultFont;	// Ĭ������

	SIZE			m_szRoundCorner;	// ����Բ�������С
	gtc::imageResource	*m_pimgBk;			// ����ͼ�������ͷ��ڴ�
	gtc::imageResource	*m_pimgBkSelected;	// ѡ��ʱ�ı���ͼ�������ͷ��ڴ�
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	windowWnd��	2014-06-21
//	��Ϣ���ڵĻ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class windowWnd
{
public:
	windowWnd(void);
	~windowWnd(void);
	HWND getHWND() const;
	operator HWND() const;

	bool registerWindowClass();
	bool registerSuperclass();
	HWND create(HWND hwndParent, PCWSTR pstrName, const RECT rc);
	HWND create(HWND hwndParent, PCWSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND create(HWND hwndParent, PCWSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	//HWND create(HWND hwndParent, WNDPROC fnWndProc, PCWSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND subclass(HWND hWnd);
	void unsubclass();
	void setIcon(UINT nRes);

	void showWindow(bool bShow = true, bool bTakeFocus = true);
	bool showModal();
	void close();
	void centerWindow();
	bool isVisible() const;
	bool isEnabled() const;
	void setEnabled(bool enabled = true);

	virtual RECT getWindowPos() const;
	virtual void setWindowPos(RECT rt);
	void setCapture();
	void releaseCapture();
	bool isCaptured();
public:
	virtual LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	virtual LRESULT postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
public:
	static void setInstance(HINSTANCE hInstance);
	static void messageLoop();
protected:
	virtual PCWSTR getWindowClassName() const = 0;
	virtual PCWSTR getSuperClassName() const {		return nullptr;													}
	virtual UINT getClassStyle() const {			return 0;														}
	virtual UINT getWindowStyle() const {			return 0;														}
	virtual UINT getWindowExStyle() const {			return 0;														}

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onFinalMessage(HWND hWnd);

	static LRESULT CALLBACK __wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __controlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	static HINSTANCE	mc_hInstance;	// 
protected:
	bool			m_bMouseCapture;	// 
	bool			m_bSubclassed;
	HWND			m_hWnd;				// ���ھ��
	WNDPROC			m_OldWndProc;

};

}