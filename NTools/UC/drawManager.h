#pragma once
#include "wucdef.h"
#include "AC/SyncObject.h"
namespace wuc{
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-14 WS_EX_LAYERED���ڿؼ���һЩ����
struct BSLayerWndAttribute
{
	HWND	hWnd;				// �ؼ����ھ��
	bool	isVisiblePrev;		// ��¼�ؼ���ǰ�Ŀɼ�״̬��

	BSLayerWndAttribute()
		: hWnd(NULL), isVisiblePrev(true)
	{}
	BSLayerWndAttribute(HWND _hWnd)
		: hWnd(_hWnd), isVisiblePrev(true)
	{}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
class controlUC;
class optionUC;
class containerUC;
class verticalLayoutUC;
class horizontalLayoutUC;
class INotifyUC;
typedef std::vector<controlUC *>					ControlArrayType;
typedef std::vector<optionUC *>						OptionArrayType;
typedef std::map<std::wstring, controlUC *>			ControlMapType;
typedef std::vector<INotifyUC *>					NotifyArrayType;
typedef std::map<std::wstring, ControlArrayType>	GroupMapType;
typedef std::map<controlUC*, BSLayerWndAttribute>	LayerWndMapType;
typedef std::vector<containerUC *>					ContainerArrayType;
typedef std::vector<verticalLayoutUC *>				VContainerArrayType;
typedef std::vector<horizontalLayoutUC *>			HContainerArrayType;
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

	//BSEventUC()
	//	: pSender(nullptr), Type(0)
	//	, dwTimestamp(0), chKey(0), wKeyState(0)
	//	, wParam(NULL), lParam(NULL)
	//{
	//	memset(&ptMouse, 0, sizeof(ptMouse));
	//}
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
	// 
	BSNotifyUC()
		: pSender(nullptr)
		, dwTimestamp(GetTickCount())
		, wParam(NULL), lParam(NULL)
	{
		memset(&ptMouse, 0, sizeof(ptMouse));
	}
	BSNotifyUC(PCWSTR pType, controlUC *pControl)
		: wsType(pType? pType: L""), pSender(pControl)
		, dwTimestamp(GetTickCount())
		, wParam(NULL), lParam(NULL)
	{
		assert(!wsType.empty());
		memset(&ptMouse, 0, sizeof(ptMouse));
	}
} BSNotifyUC;

// Structure for relative position to the parent
typedef struct BSRelativePosUC
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
	//// 
	//BSRelativePosUC()
	//	: bRelative(false)
	//	, nMoveXPercent(0), nMoveYPercent(0)
	//	, nZoomXPercent(0), nZoomYPercent(0)
	//{
	//	memset(&szParent, 0, sizeof(szParent));
	//}
}BSRelativePosUC;

// Listener interface
class INotifyUC
{
public:
	virtual ~INotifyUC() {}
	virtual void notify(BSNotifyUC &msg) = 0;	// �������߿ؼ���֪ͨ
};

// MessageFilter interface
class IMessageFilterUC
{
public:
	virtual ~IMessageFilterUC() {}
	virtual LRESULT messageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};
typedef std::vector<IMessageFilterUC *> MessageFilterArrayType;

// 
class ITranslateAccelerator
{
public:
	virtual ~ITranslateAccelerator() {}
	virtual LRESULT translateAccelerator(MSG *pMsg) = 0;
};
typedef std::vector<ITranslateAccelerator *> TranslateAcceleratorArrayType;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IWindowUC  ��		2015-8-10
//	���Ӵ���ʹ�õĽӿ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class IWindowUC
{
public:
	virtual ~IWindowUC() {}
	virtual controlUC* getWindowOwner() = 0;
};

typedef struct BSTimerInfo
{
	controlUC*	pSender;
	UINT		nLocalID;
	HWND		hWnd;
	UINT		uWinTimer;
	bool		bKilled;
}BSTimerInfo;

typedef struct BSFindTabInfo
{
	controlUC* pFocus;
	controlUC* pLast;
	bool bForward;
	bool bNextIsIt;
}BSFindTabInfo;

typedef struct BSFindShortCut
{
	wchar_t ch;
	bool bPickNext;
}BSFindShortCut;
/////////////////////////////////////////////////////////////////////////////////////
//

class renderClip
{
public:
	~renderClip();
	RECT rcItem;
	HDC hDC;
	HRGN hRgn;
	HRGN hOldRgn;

	static void generateClip(HDC hDC, RECT rc, renderClip& clip);
	static void generateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, renderClip& clip);
	static void useOldClipBegin(HDC hDC, renderClip& clip);
	static void useOldClipEnd(HDC hDC, renderClip& clip);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	drawManager��	2014-07-30
//	���ڵĻ�ͼ������
////////////////////////////////////////////////////////////////////////////////////////////////////////

class drawManager
{
public:
public:
	drawManager(void);
	~drawManager(void);

public:
	void init(HWND hWnd);
	void needUpdate();
	void invalidate(RECT& rcItem);

	HWND getPaintWindow() const;
	HDC  getPaintDC() const;
	HWND getTooltipWindow() const;

	bool attachDialog(controlUC* pControl);
	bool initControls(controlUC* pControl, controlUC* pParent = NULL);
	void reapObject(controlUC* pControl);

	void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);	// ���ô�������ֵ
	
	void clientToScreen(RECT &rect);
	void clientToScreen(POINT &pt);
	void screenToClient(RECT &rect);
	void screenToClient(POINT &pt);

	POINT getMousePos() const;
	SIZE getClientSize() const;
	SIZE getInitSize();
	void setInitSize(int cx, int cy);
	RECT& getSizeBox();
	void setSizeBox(RECT& rcSizeBox);
	RECT& getCaptionRect();
	void setCaptionRect(LONG height);
	void setCaptionRect(RECT& rcCaption);
	SIZE getRoundCorner() const;
	void setRoundCorner(int cx, int cy);
	SIZE getMinInfo() const;
	void setMinInfo(int cx, int cy);
	SIZE getMaxInfo() const;
	void setMaxInfo(int cx, int cy);

	void setTransparent(int nOpacity);
	void setBackgroundTransparent(bool bTrans);
	bool isBackgroundTransparent() const;
	void setLayerWindowTransparent(bool bTrans);
	bool isLayerWindowTransparent() const;
	//void setBackgroupImage(PCWSTR imgKeyName);

	DWORD getDefaultDisabledColor() const;
	void setDefaultDisabledColor(DWORD dwColor);
	DWORD getDefaultFontColor() const;
	void setDefaultFontColor(DWORD dwColor);
	DWORD getDefaultLinkFontColor() const;
	void setDefaultLinkFontColor(DWORD dwColor);
	DWORD getDefaultLinkHoverFontColor() const;
	void setDefaultLinkHoverFontColor(DWORD dwColor);
	DWORD getDefaultSelectedBkColor() const;
	void setDefaultSelectedBkColor(DWORD dwColor);

	static const BSFontInfoUC* getDefaultFontInfo();
	static void setDefaultFont(PCWSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);

	static gtc::image* findImage(PCWSTR pKeyName);
	static const BSFontInfoUC* findFont(int fontIndex);
	static HFONT getFont(int fontIndex);
	static HMENU findMenu(PCWSTR pKeyName);
	static PCWSTR getDefaultAttributeList(PCWSTR pControlKey);

	void setCapture();
	void releaseCapture();
	bool isCaptured();

	controlUC* getRoot() const;
	controlUC* findControl(POINT pt) const;
	controlUC* findControl(PCWSTR pstrName) const;
	controlUC* findSubControlByPoint(controlUC* pParent, POINT pt) const;
	controlUC* findSubControlByName(controlUC* pParent, PCWSTR pstrName) const;
	controlUC* findSubControlByClass(controlUC* pParent, PCWSTR pstrClass, int iIndex = 0);
	ControlArrayType& findSubControlsByClass(controlUC* pParent, PCWSTR pstrClass);
	ControlArrayType& getSubControlsByClass();

	static bool addOptionGroup(PCWSTR pstrGroupName, controlUC* pControl);
	static ControlArrayType getOptionGroup(PCWSTR pstrGroupName);
	static void removeOptionGroup(PCWSTR pstrGroupName, controlUC* pControl);
	static void removeAllOptionGroups();

	controlUC* getFocus() const;
	void setFocus(controlUC* pControl);
	void setFocusNeeded(controlUC* pControl);

	bool setNextTabControl(bool bForward = true);

	void addDelayedCleanup(controlUC* pControl);
	void addDelayedFree(controlUC* pControl);
	bool existsDelayedFree(controlUC* pControl);	// �鿴�ؼ��Ƿ��Ѿ����ӳ��ͷſؼ�������
	void clearDelayedFree();	// �ͷŲ�����ӳ��ͷſؼ�
	bool emptyDelayedFree();	// �Ƿ������Ҫ�ӳ��ͷŵ¿ؼ�
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-14 	���WS_EX_LAYERED���ڰ����Ŀؼ�
	void addLayerWindowControl(controlUC *pOwner, HWND hWnd);
	void removeLayerWindowControl(controlUC *pOwner);
	void recordLayerWindowControlAttr();		// ��¼��ǰ�ؼ�����
	void restoreLayerWindowControlAttr();		// �ָ���ǰ����Ŀؼ�����
	void showLayerWindowControl(bool bShow = true);		// 

	bool addNotifier(INotifyUC* pControl);
	bool removeNotifier(INotifyUC* pControl);   
	void sendNotify(BSNotifyUC& Msg, bool bAsync = false);
	void sendNotify(controlUC* pControl, PCWSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

	bool setTimer(controlUC* pControl, UINT nTimerID, UINT uElapse);
	bool killTimer(controlUC* pControl, UINT nTimerID);
	void killTimer(controlUC* pControl);
	void removeAllTimers();

	bool addMessageFilter(IMessageFilterUC *pFilter);
	bool removeMessageFilter(IMessageFilterUC *pFilter);

	bool addTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool removeTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool translateAccelerator(LPMSG pMsg);

	bool messageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	bool preMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-4-27 �趨�ų�ˢ������
	void setBesidesArea(RECT rcBesidesArea);
	void freeBesidesArea();
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-4-28 ���³����ų���������������	
	void updateWindowOutBesidesArea();
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-6-8 �����Ϸ���WM_LBUTTONDOWN��WM_MOUSEMOVE��������û�ж�Ӧ��WM_LBUTTONUP���˴�ģ��һ��
	void endDragDrop();
public:
	// static 
	static HINSTANCE getInstance();
	static void setInstance(HINSTANCE hInst);
	static gtc::resourceManager& getResource();

	static void messageLoop();
	static bool translateMessage(const LPMSG pMsg);
public:
	// wuc�ڲ�ʹ�õ���Ϣ
	static UINT	WUC_MSG_WINDOW_INIT;		// ����windowinit֪ͨʱ˳�������ڷ��͵���Ϣ WPARAM=NULL LPARAM=NULL
//protected:
	void updateLayeredWindow();
	// ����������ʾ�ߴ�
	SIZE estimateToolTipSize();

protected:
	// ��Է��ͷŷ��Զ��ͷſؼ����ӳ��ͷ�
	static unsigned int WINAPI threadDelayedFreeControl(void *pParam/*gtc::thread::_BSThreadInfo *pthr*/);
	void startDelayedFreeControl();
	void stopDelayedFreeControl();

protected:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-8 �����m_vecDelayedFree�ı�����	
#if defined(GTC_BOOST_THREAD)
	protected:
		mutable boost::shared_mutex m_rwMutexDelayedFree;
		mutable bool				m_bReadLockedDelayedFree;
		mutable bool				m_bWriteLockedDelayedFree;
	public:
#define READ_LOCK_DLYFREE	gtc::readLock	rdLockDelayedFree(m_rwMutexDelayedFree, m_bReadLockedDelayedFree, __FILEW__, __FUNCTIONW__);
#define WRITE_LOCK_DLYFREE	gtc::writeLock	wtLockDelayedFree(m_rwMutexDelayedFree, m_bWriteLockedDelayedFree, __FILEW__, __FUNCTIONW__);
#else
#define READ_LOCK_DLYFREE
#define WRITE_LOCK_DLYFREE
#endif

private:
	static controlUC* CALLBACK __findControlFromNameHash(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromCount(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromPoint(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromTab(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromShortcut(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromUpdate(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromName(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlFromClass(controlUC* pThis, LPVOID pData);
	static controlUC* CALLBACK __findControlsFromClass(controlUC* pThis, LPVOID pData);
	//public:
	static HINSTANCE			mc_hInstance;	// �ڴ����Դ���ʵ��
	static gtc::resourceManager	mc_resManager;	// ͼƬ���˵�����Դ����

	HWND			m_hwndDrawer;
	HDC				m_hdcDrawer;
	HDC				m_hdcOffscreen;
	HDC				m_hdcBackground;
	HBITMAP			m_hbmpOffscreen;
	HBITMAP			m_hbmpBackground;
	LPBYTE			m_pBmpBackgroundBits;

	HWND			m_hwndTooltip;
	TOOLINFO		m_tipToolTip;

	//
	controlUC		*m_pRoot;
	controlUC		*m_pFocus;
	controlUC		*m_pEventHover;
	controlUC		*m_pEventClick;
	controlUC		*m_pEventKey;
	controlUC		*m_pEventWheel;

	bool			m_bFirstLayout;
	bool			m_bUpdateNeeded;
	bool			m_bFocusNeeded;
	POINT			m_ptLastMousePos;
	SIZE			m_szInitWindowSize;			// ���ڳ�ʼ��С
	RECT			m_rcSizeBox;				// �߿����ק��ֵΪ���������±߿�������С�������������ɱ䣬����ʾ�����϶��߿������ʹĳ�߿򲻿��ƶ���ʹ��Ϊ0���ɡ�
	SIZE			m_szRoundCorner;			// �����Ƿ���Բ�Ǽ����С
	RECT			m_rcCaption;				// ���ڿ��϶��ı�������С�ı߾�,���һ��������ָ���ϱ߿�ľ���,��(0,0,0,28)
	SIZE			m_szMinWindow;				// ������С�ߴ�
	SIZE			m_szMaxWindow;				// �������ߴ�
	bool			m_bAlphaBackground;			// ��:����͸��Բ�Ǵ���
	bool			m_bOffscreenPaint;
	bool			m_bAlphaLayerWindow;		// ͨ��pngͼƬ��Ϊ������ʵ��͸�Ӵ��ڣ���Ҫ����������WS_EX_LAYERED��ʽ����ActiveX�ؼ���ͻ��ʹAX�ؼ����ܻ���
												// ע�����������к��޷���ʾ�����ڣ�����Ҫ��WM_ACTIVATE��Ϣ��ִ��InvalidateRect()
	LayerWndMapType	m_mapLayerWnd;				// ���WS_EX_LAYERED��ʽ���ڣ���¼�������ؼ�key���ؼ�uc��value���ؼ����
	//gtc::image		*m_pImageBk;				// ����ͼ
	DWORD			m_dwDefaultDisabledColor;	// ����ȱʡ����ɫ
	DWORD			m_dwDefaultFontColor;
	DWORD			m_dwDefaultLinkFontColor;
	DWORD			m_dwDefaultLinkHoverFontColor;
	DWORD			m_dwDefaultSelectedBkColor;
	//BSFontInfoUC	m_bsDefaultFontInfo;

	bool			m_bMouseTracking;			// �����Ƿ���ٵ����
	bool			m_bMouseCapture;			// �����Ƿ񲶻����

	UINT			m_uTimerID;					// �ڲ�ʱ��m_vecTimers����ʼID
	NotifyArrayType	m_vecNotifys;				// ����֪ͨ��Ϣ�Ĵ��ڵ����ã����������ͷ�
	std::vector<BSTimerInfo *>	m_vecTimers;	// ʱ�Ӽ���
	//public:
	ControlArrayType		m_vecDelayedFree;		// �ӳ��ͷſؼ����Ƚ������¼���ɾ���Լ�
	gtc::criticalSection	m_csDelayedFree;		// ��m_vecDelayedFree���ݵı���
	volatile LONG			m_lDelayedFreeSwitch;	// threadDelayedFreeControl�߳̿���
	HANDLE					m_eventDelayedFree;		// threadDelayedFreeControl�ڲ��ȴ��¼�
	HANDLE					m_threadDelayedFree;	// 

	ControlArrayType		m_vecDelayedCleanup;	// �ӳ�ɾ���ؼ�������ĳ�����ؼ��ڲ�һ���ӿؼ���mousemove���������ʱֱ��ɾ���ͻ��쳣
	//gtc::swmrg			m_smDelayedCleanup;
	ControlArrayType		m_vecFoundControls;		// ��ǰ���ҵ��ؼ��ļ���
	ControlMapType			m_mapNameControls;		// �ؼ����Ƽ����ӳ�伯��
	static GroupMapType		mc_mapGroupControls;	// ��ѡ�鰴ť���ϣ���ǰ����ֻ����һ������ѡ��״̬���� key=������ value=optionUC���˵���Ȱ�ť����
	
	MessageFilterArrayType			m_vecMessageFilter;				// �������ڴ������Ϣ
	TranslateAcceleratorArrayType	m_vecTranslateAccelerator;		// 

	static std::vector<drawManager *> mc_vecPreMessages;

	bool			m_bBesidesArea;				// �Ƿ������ų����򣬼�ָ��������Ҫˢ��
	RECT			m_rcBesidesArea;			// ��Чˢ�µ�����
	
};

}