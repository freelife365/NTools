#pragma once
#include "wucControl.h"
#include "AC\SyncObject.h"

namespace wuc {

class scrollBarUC;

class IContainerUC
{
public:
	virtual controlUC* getItemAt(int iIndex) const = 0;
	virtual int getItemIndex(controlUC* pControl) const  = 0;
	virtual bool setItemIndex(controlUC* pControl, int iIndex)  = 0;
	virtual int getCount() const = 0;
	virtual bool add(controlUC* pControl) = 0;
	virtual bool addAt(controlUC* pControl, int iIndex)  = 0;
	virtual bool remove(controlUC* pControl) = 0;
	virtual bool removeAt(int iIndex)  = 0;
	virtual void removeAll() = 0;
	virtual controlUC* front() const = 0;
	virtual controlUC* back() const = 0;
};

class containerUC : public controlUC, public IContainerUC
{
public:
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-8 �����m_items�ı�����	
#if defined(GTC_BOOST_THREAD)
	protected:
		mutable boost::shared_mutex m_rwMutex;
		mutable bool				m_bReadLocked;
		mutable bool				m_bWriteLocked;
	public:
#define READ_LOCK	gtc::readLock	rdLock(m_rwMutex, m_bReadLocked, __FILEW__, __FUNCTIONW__);
#define WRITE_LOCK	gtc::writeLock	wtLock(m_rwMutex, m_bWriteLocked, __FILEW__, __FUNCTIONW__);
#else
	#define READ_LOCK
	#define WRITE_LOCK
#endif
public:
	containerUC(void);
	virtual ~containerUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void Event(BSEventUC &event);
	virtual void paint(HDC hdc, const RECT &rcPaint);

	virtual RECT getInset() const;
	virtual void setInset(RECT rcInset); // �����ڱ߾࣬�൱�����ÿͻ���
	virtual void setInset(LONG left, LONG top, LONG right, LONG bottom);
	virtual int getChildPadding() const;
	virtual void setChildPadding(int iPadding);
	virtual bool isMouseChildEnabled() const;
	virtual void setMouseChildEnabled(bool bEnable = true);
	virtual bool isAutoDestroy() const;
	virtual void setAutoDestroy(bool bAuto);
	virtual bool isDelayedFree() const;
	virtual void setDelayedFree(bool bDelayed);
	virtual bool isDelayedDestroy() const;
	virtual void setDelayedDestroy(bool bDelayed);

	virtual void setVisible(bool bVisible = true);
	virtual void setInternVisible(bool bVisible = true);
	virtual void setMouseEnabled(bool bEnabled = true);

	virtual void setManager(drawManager *pManager, controlUC *pParent, bool bInit = true);
	virtual controlUC* findControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	virtual bool existsControl(controlUC *pControl);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	virtual controlUC* getItemAt(int iIndex) const;
	virtual int getItemIndex(controlUC* pControl) const ;
	virtual bool setItemIndex(controlUC* pControl, int iIndex);
	virtual int getCount() const;
	virtual bool add(controlUC* pControl);
	virtual bool addAt(controlUC* pControl, int iIndex);
	virtual bool remove(controlUC* pControl);
	virtual bool removeAt(int iIndex) ;
	virtual void removeAll();
	virtual controlUC* front() const;
	virtual controlUC* back() const;

	virtual int findSelectable(int iIndex, bool bForward = true) const;

	virtual void setPosition(RECT pos);


	virtual SIZE getScrollPos() const;
	virtual SIZE getScrollRange() const;
	virtual void setScrollPos(SIZE szPos);
	virtual void lineUp(int step = 0);
	virtual void lineDown(int step = 0);
	virtual void pageUp();
	virtual void pageDown();
	virtual void homeUp();
	virtual void endDown();
	virtual void lineLeft();
	virtual void lineRight();
	virtual void pageLeft();
	virtual void pageRight();
	virtual void homeLeft();
	virtual void endRight();
	//virtual void buildScrollBar(scrollBarUC *pScrollbarVertical, scrollBarUC *pScrollbarHorizontal = nullptr);
	virtual void enableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
	virtual scrollBarUC* getVerticalScrollBar() const;
	virtual scrollBarUC* getHorizontalScrollBar() const;
protected:
	virtual void setFloatPos(int iIndex);
	virtual void processScrollBar(RECT rc, int cxRequired, int cyRequired);
public:
	// ������С�ߴ�
	virtual SIZE getMinSize() const;
	bool isAutoHideScroll() const;
	void setAutoHideScroll(bool bAutoHide);
	bool isDriftScroll() const;
	void setDriftScroll(bool bDrift = true);
protected:
	ControlArrayType	m_items;
	//mutable gtc::swmrg			m_swItems;	// ��m_items���ݱ���
	RECT	m_rcInset;				// �����ؼ����ڱ߾�
	int		m_iChildPadding;		// �ӿؼ���Ķ�����
	bool	m_bMouseChildEnabled;	// ����Ƿ��ܲ����ӿؼ�
	bool	m_bAutoDestroy;			// �Ƿ������ӿؼ��ͷš���comboWnd����Ŀ������comboUC�����Ե��ͷ�comboWndʱ�����ͷ����ڵ���Ŀ��
	bool	m_bDelayedFree;			// ɾ���ӿؼ�ʱҪ�ӳ��ͷš����Կؼ���click��Ϣ���ͷ�ʱ������Ҫ��m_bAutoDestroy=false����ʱ���ͷſؼ�����threadDelayedFreeControl�߳�
	bool	m_bDelayedDestroy;		// һ�㶼��Ҫ�ӳ��ͷţ���Ϊ���ӿؼ����ܱ�mousemove����Ϣ����
	bool	m_bAutoHideScroll;		// �Ƿ��Զ����ع�����������������ʱ��ʾ���뿪ʱ���أ�
	bool	m_bDriftScroll;			// �������Ƿ������������Ϸ�������true=���������������������ݣ�false=���������������ݵ���ࣩ
	bool	m_bScrollProcess;		// ��ֹSetPosѭ������
	//SIZE	m_szSpace;				// �ڵ�����cy����cx��϶

	//int m_iChildPadding;
	//bool m_bAutoDestroy;
	//bool m_bMouseChildEnabled;
	//bool m_bScrollProcess; // ��ֹSetPosѭ������

	scrollBarUC* m_pVerticalScrollBar;
	scrollBarUC* m_pHorizontalScrollBar;

};



/////////////////////////////////////////////////////////////////////////////////////
//

class verticalLayoutUC : public containerUC
{
public:
	verticalLayoutUC();
	~verticalLayoutUC() {		//TRACE_CLASS_END(this)
		//gtc::app::writeVSLogs(L"free verticalLayoutUC <%s>", m_wsName.c_str());
	}
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void Event(BSEventUC& event);

	void setSepHeight(int iHeight);
	int getSepHeight() const;
	void setSepImmMode(bool bImmediately);
	bool isSepImmMode() const;

	virtual void setPosition(RECT pos);

	RECT getThumbRect(bool bUseNew = false) const;

protected:
	int		m_iSepHeight;		// �ָ����߶�,������ʾ�ָ����ڶ������ǵײ�,��(4)
	UINT	m_uButtonState;		
	POINT	ptLastMouse;
	RECT	m_rcNewPos;
	bool	m_bImmMode;			// �϶��ָ����Ƿ������ı��С,��(false)
};


/////////////////////////////////////////////////////////////////////////////////////
//

class horizontalLayoutUC : public containerUC
{
public:
	horizontalLayoutUC();
	~horizontalLayoutUC() {				//TRACE_CLASS_END(this)					
	}
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void Event(BSEventUC& event);

	void setSepWidth(int iWidth);
	int getSepWidth() const;
	void setSepImmMode(bool bImmediately);
	bool isSepImmMode() const;

	virtual void setPosition(RECT pos);

	RECT getThumbRect(bool bUseNew = false) const;

protected:
	int		m_iSepWidth;		// �ָ�����,������ʾ�ָ�������߻����ұ�,��(-4)
	UINT	m_uButtonState;
	POINT	ptLastMouse;
	RECT	m_rcNewPos;
	bool	m_bImmMode;			// �϶��ָ����Ƿ������ı��С,��(false)
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	tabLayoutUC  ��		2015-7-27
//	ֻ��һ���ɼ�Ԫ�ص�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class tabLayoutUC : public containerUC
{
public:
	tabLayoutUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual bool add(controlUC* pControl);
	virtual bool addAt(controlUC* pControl, int iIndex);
	virtual bool remove(controlUC* pControl);
	virtual void removeAll();
	int getCurSel() const;
	bool selectItem(int iIndex);

	virtual void setPosition(RECT rc);

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
protected:
	int		m_iCurSel;
};


}