#pragma once
#include "controlUC.h"
//#include "scrollbarWnd.h"
namespace gtc {

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
};

class containerUC : public controlUC, public IContainerUC
{
public:
	containerUC(void);
	~containerUC(void);

	virtual PCWSTR getClass() const;
	virtual void Event(BSEventUC &event);
	virtual void paint(HDC hdc);

	virtual controlUC* getItemAt(int iIndex) const;
	virtual int getItemIndex(controlUC* pControl) const ;
	virtual bool setItemIndex(controlUC* pControl, int iIndex);
	virtual int getCount() const;
	virtual bool add(controlUC* pControl);
	virtual bool addAt(controlUC* pControl, int iIndex);
	virtual bool remove(controlUC* pControl);
	virtual bool removeAt(int iIndex) ;
	virtual void removeAll();

	virtual void setFloatPos(int iIndex);
	virtual void setPosition(RECT &pos);


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
	virtual void buildScrollBar(scrollBarUC *pScrollbarVertical, scrollBarUC *pScrollbarHorizontal = nullptr);
	virtual void enableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
	virtual scrollBarUC* getVerticalScrollBar() const;
	virtual scrollBarUC* getHorizontalScrollBar() const;

public:
	// ������С�ߴ�
	virtual SIZE getMinSize() const;
protected:
	std::vector<controlUC *> m_items;
	RECT	m_rcInset;		// �����ؼ����ڱ߾�
	//SIZE	m_szSpace;				// �ڵ�����cy����cx��϶

	//int m_iChildPadding;
	//bool m_bAutoDestroy;
	//bool m_bDelayedDestroy;
	//bool m_bMouseChildEnabled;
	//bool m_bScrollProcess; // ��ֹSetPosѭ������

	scrollBarUC* m_pVerticalScrollBar;
	scrollBarUC* m_pHorizontalScrollBar;

};



}