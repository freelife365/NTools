#ifndef __NTOOL_H__
#define __NTOOL_H__
#pragma once

#include <vector>
#include <memory>

#include <afxwin.h>
#include <afxole.h>

#include "AC/items/items.h"
#include "AC/items/itobjAdmin.h"
// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4
// �Զ����¼�
/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_ACTION	2013-12-14 --- >>
	˵������ĳ����ͼ����������ʱ����
	������
	WPARAM	= �������BSActionSender*
	LPARAM	= ��ѡ�еĶ��󼯺�ָ��(itobj**)�������߸����ͷš�
	�����ߣ�
	CMainFrame		= ���¸����������ݣ������Դ��ڣ�����������
	CNPainterView	= ˢ�»�ͼ����
	����ֵ��
*****************************************************************************************/
#define WM_ME_ITEM_ACTION				WM_USER+100	
/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_ACTIONS	2013-12-14 --- >>
	˵������ĳ����ͼ����������ʱ����
	������
	WPARAM	= �������BSActionSender*
	LPARAM	= ��ѡ�еĶ��󼯺�ָ��(itobj::PtrArrayType*)�������߸����ͷš�
	�����ߣ�
	CMainFrame		= ���¸����������ݣ������Դ��ڣ�����������
	CNPainterView	= ˢ�»�ͼ����
	����ֵ��
*****************************************************************************************/
#define WM_ME_ITEM_ACTIONS				WM_USER+101	
/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_PROPERTY_CHANGED 	2013-12-23 --- >>
	˵����һ�����ڶ�������Է����ı�ʱ�������������Ķ����͸����Ա༭����
	������
	WPARAM	= �����ı����ԵĶ���
	LPARAM	= �������Լ���ָ��(itobjProp::PtrArrayType*)�������߸����ͷš�
	�����ߣ�
	CMainFrame		= ���¸����������ݣ������Դ��ڣ�����������
	����ֵ��
*****************************************************************************************/
#define WM_ME_ITEM_PROPERTY_CHANGED		WM_USER+102

/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_REDRAW 	2014-03-15 --- >>
	˵�������»��ƶ���
	������
	WPARAM	= �����ƵĶ���(itobj*)
	LPARAM	= ���Ƶ�����rect*���ͻ�����0.1mm���������߸����ͷš�
	�����ߣ�
	CNPainterView	= ˢ�»�ͼ����
	����ֵ��
*****************************************************************************************/
#define WM_ME_ITEM_REDRAW				WM_USER+103

namespace acm {
///////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������
typedef enum BEToolShape
{
	BEToolShapeSelection = 0,	// �ƶ����ı��С�Ȳ���
	BEToolShapeLine,			// ��
	BEToolShapeRect,			// ����
	BEToolShapeRoundRect,		// Բ�Ǿ���
	BEToolShapeEllipse,			// ��Բ
	BEToolShapePoly,			// �����
	BEToolShapeGrid				// ���
}BEToolShape;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNPainterDoc
//	˵����������Ϊ�Զ����ͼ�ĵ��Ĺ������࣬�ڲ������˹�������
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNPainterDoc : public COleDocument
{
protected: // �������л�����
	CNPainterDoc();
	DECLARE_DYNCREATE(CNPainterDoc)
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	afx_msg void OnFileSave();
// ʵ��
public:
	virtual ~CNPainterDoc();
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	// ��������
	/*****************************************************************************************
		<< --- CNPainterDoc::computePageSize	2013-12-04 --- >>
		˵��������ϵͳ�趨�Ĵ�ӡ���õ�itobjPage��Ϣ
		������
		����ֵ��
	*****************************************************************************************/
	void computePageSize();
	/*****************************************************************************************
		<< --- CNPainterDoc::getAdminer	2013-12-02 --- >>
		˵�������ض��󼯺ϵ�����
		������
		����ֵ�����ض��󼯺ϵ����� 
	*****************************************************************************************/
	gtc::itobjAdmin& getAdminer() {			return m_objAdmin;					}
public:
	// �ӿڶ���
	virtual void draw(CDC *pdc) 
	{
		DEBUG_LOG(L"---> CNPainterDoc::draw ");
	}

	/*****************************************************************************************
		<< --- CNPainterDoc::remove	2013-12-02 --- >>
		˵�����Ƴ�ָ������
		������
		pobj	= ���Ƴ��Ķ���
		����ֵ��
	*****************************************************************************************/
	virtual void remove(gtc::itobj* pobj)
	{
		BSActionSender *psender = new BSActionSender(BESenderFlagDoc, BEItemActionDelete);
		AfxGetMainWnd()->SendMessage(WM_ME_ITEM_ACTION, (WPARAM)psender, (LPARAM)pobj);
		m_objAdmin.remove(pobj);
	}

	/*****************************************************************************************
		<< --- CNPainterDoc::add	2013-12-02 --- >>
		˵����׷��һ����ͼ����
		������
		pobj	= ����ӵĻ�ͼ����
		����ֵ��
	*****************************************************************************************/
	virtual void add(gtc::itobj* pobj)
	{
		m_objAdmin.add(pobj);
		BSActionSender *psender = new BSActionSender(BESenderFlagDoc, BEItemActionAdd);
		AfxGetMainWnd()->SendMessage(WM_ME_ITEM_ACTION, (WPARAM)psender, (LPARAM)pobj);
	}

	/*****************************************************************************************
		<< --- CNPainterDoc::objectAt	2013-12-02 --- >>
		˵�������ص�point���ĵ�һ����������������ӵ��Ⱥ�˳��
		������
		point	= �ο���
		����ֵ���оͷ��ض�����ָ�룬���򷵻ؿ�ָ��
	*****************************************************************************************/
	virtual gtc::itobj* objectAt(const CPoint &point)
	{
		return m_objAdmin.objectAt(point);
	}
protected:
	gtc::itobjAdmin	m_objAdmin;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNPainterView
//	˵����������Ϊ�Զ����ͼ��ͼ�Ĺ������࣬�ڲ������˹�������
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNPainterView : public CScrollView
{
public:
	CNPainterView();
public:
	CNPainterDoc* GetDocument()
		{ return (CNPainterDoc*)m_pDocument; }
public:
	///*****************************************************************************************
	//	<< --- CNPainterView::drawBackground	2013-12-02 --- >>
	//	˵�������ƹ�������ֽ��
	//	������
	//	pdc	= CDC*
	//	����ֵ��
	//*****************************************************************************************/
	//void drawBackground(CDC *pdc);
	/*****************************************************************************************
		<< --- CNPainterView::resetScrollSizes	2013-12-02 --- >>
		˵�������ݴ��ڿͻ������豸���꣺���أ��趨�������䣬ʹֽ�����ڴ��ڿͻ�������
		������
		clientArea	= ���ڿͻ�������GetClientRect��OnSize�õ����豸���꣩
		����ֵ��
	*****************************************************************************************/
	void resetScrollSizes(CSize clientArea = CSize(0,0));
	/*****************************************************************************************
		<< --- CNPainterView::remove	2013-11-30 --- >>
		˵������ָ�������ѡ�м������Ƴ�
		������
		pobj	= ���Ƴ�����
		����ֵ��
	*****************************************************************************************/
	void remove(gtc::itobj* pobj)
	{
		gtc::itobj::PtrArrayType::iterator iter, end = m_vecSels.end();
		iter = std::find(m_vecSels.begin(), end, pobj);
		if(iter != end)
		{
			//(*iter)->remove();����Ҫ�ͷ�
			m_vecSels.erase(iter);
		}
	}
	/*****************************************************************************************
		<< --- CNPainterView::moveHandleTo	2013-11-30 --- >>
		˵����ѡ��ָ�������ڵĶ���
		������
		rect	= ����
		bAdd	= trueʱ����ѡ�ж�����׷�Ӷ��󣬷��������ѡ�м�¼��
		����ֵ��
	*****************************************************************************************/
	void selectWithinRect(CRect rect, bool bAdd = false);
	///*****************************************************************************************
	//	<< --- CNPainterView::moveHandleTo	2013-11-30 --- >>
	//	˵����ͨ��ָ����λ��ı����ߴ�
	//	������
	//	nHandle	= �������Ķ�λ������
	//	point	= ��λ��������
	//	pobj	= ���ƶ��Ķ���
	//	����ֵ��
	//*****************************************************************************************/
	//void moveHandleTo(int nHandle, CPoint point, gtc::itobj* pobj);
	///*****************************************************************************************
	//	<< --- CNPainterView::moveTo	2013-11-30 --- >>
	//	˵����������pobj�Ƶ���λ��
	//	������
	//	position	= ��λ��
	//	pobj		= ���ƶ��Ķ���
	//	����ֵ��
	//*****************************************************************************************/
	//void moveTo(const CRect &position, gtc::itobj* pobj);
	///*****************************************************************************************
	//	<< --- CNPainterView::getHandleCursor	2013-11-30 --- >>
	//	˵�������ݶ�λ��λ�õõ����Ӧ�Ĺ��
	//	������
	//	nHandle	= ��λ������
	//	pobj	= �������󣻿ɸ��ݲ�ͬ����õ���ͬ���
	//	����ֵ�����
	//*****************************************************************************************/
	//HCURSOR getHandleCursor(int nHandle, gtc::itobj* pobj);
	/*****************************************************************************************
		<< --- CNPainterView::editProperties	2013-11-30 --- >>
		˵����ͨ�����ԶԻ���༭pobj������
		������
		pobj	= ���༭����
		����ֵ��
	*****************************************************************************************/
	void editProperties(gtc::itobj* pobj);
	/*****************************************************************************************
		<< --- CNPainterView::cloneSelection	2013-11-30 --- >>
		˵������¡ѡ�м����б��еĶ��󣬲�׷�ӵ��ĵ���
		������
		����ֵ��
	*****************************************************************************************/
	void cloneSelection()
	{
		acm::CNPainterDoc *pdoc = GetDocument();
		if(pdoc == nullptr)
			return;
		for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < m_vecSels.size(); ++idx)
			pdoc->add(m_vecSels[idx]->clone());
	}

	/*****************************************************************************************
		<< --- CNPainterView::invalidate	2013-11-30 --- >>
		˵����ˢ�¶���pobj��������
		������
		pobj	= ��ˢ�¶���
		����ֵ��
	*****************************************************************************************/
	void invalidate(gtc::itobj* pobj)
	{
		assert(pobj);
		CRect rect = pobj->getPosition();
		docToClient(rect);
		if(m_bActive && isSelected(pobj))
		{
			int space = gtc::itobj::mc_iTrackerSize/2;
			rect.left -= space;
			rect.top -= space+1;
			rect.right += space+1;
			rect.bottom += space;
		}
		rect.InflateRect(1,1);
		InvalidateRect(rect, FALSE);

	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-1-31	rc�����ĵ�����
	void invalidate(RECT rc) {
		docToClient(rc);
		::InflateRect(&rc, 4, 4);
		InvalidateRect(&rc, FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	//	2015-1-31	rc�����ĵ�����
	void invalidate(gtc::rect rc) {
		if(rc.width <= 0) rc.width = 1;
		if(rc.height <= 0) rc.height = 1;
		RECT rcc = rc;
		invalidate(rcc);
	}
	// ȫ��ˢ��
	void invalidate()
	{
		Invalidate(FALSE);
	}
	void deselect(gtc::itobj* pobj)
	{
		gtc::itobj::PtrArrayType::const_iterator iter, end = m_vecSels.cend();
		iter = std::find(m_vecSels.cbegin(), end, pobj);
		if(iter != end)
		{
			invalidate(*iter);
			m_vecSels.erase(iter);
		}
		/*POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
		{
			InvalObj(pObj);
			m_selection.RemoveAt(pos);
		}*/
	}
	/*****************************************************************************************
		<< --- CNPainterView::select	2013-11-30 --- >>
		˵����������pobj��ӵ���ѡ�ж����б���
		������
		pobj	= ����ӵĶ���
		isadd	= true׷�ӵ��б��У����������ԭ�б�������
		����ֵ��true���ڣ����򲻴���
	*****************************************************************************************/
	void select(gtc::itobj* pobj, bool isadd = false)
	{
		//if (!bAdd)
		//{
		//	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		//	m_selection.RemoveAll();
		//}

		//if (pObj == NULL || IsSelected(pObj))
		//	return;

		//m_selection.AddTail(pObj);
		//InvalObj(pObj);
		if(!isadd)
		{
			OnUpdate(nullptr, HINT_UPDATE_SELECTION, nullptr);
			m_vecSels.clear();
		}
		if (pobj == nullptr || isSelected(pobj))
		{
			return;
		}
		m_vecSels.push_back(pobj);
		invalidate(pobj);
		// 
		
	}

	/*****************************************************************************************
		<< --- CNPainterView::isSelected	2013-11-30 --- >>
		˵�����������pobj�Ƿ�����ѡ�ж����б���
		������
		pobj	= ���������
		����ֵ��true���ڣ����򲻴���
	*****************************************************************************************/
	bool isSelected(const gtc::itobj* pobj) const
	{
		//CDrawObj* pDrawObj = (CDrawObj*)pDocItem;
		//if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawItem)))
		//	pDrawObj = ((CDrawItem*)pDocItem)->m_pDrawObj;
		//return m_selection.Find(pDrawObj) != NULL;
		gtc::itobj::PtrArrayType::const_iterator end = m_vecSels.cend();
		return std::find(m_vecSels.cbegin(), end, pobj)!=end;
	}

	/*****************************************************************************************
		<< --- CNPainterView::clientToDoc	2013-11-29 --- >>
		˵�����豸���굽�߼������ת��
		������
		point	= ��ת���ĵ�
		����ֵ��
	*****************************************************************************************/
	void clientToDoc(CPoint& point)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc, NULL);
		dc.DPtoLP(&point);
	}
	/*****************************************************************************************
		<< --- CNPainterView::clientToDoc	2013-11-29 --- >>
		˵�����豸���굽�߼������ת��
		������
		rect	= ��ת��������
		����ֵ��
	*****************************************************************************************/
	void clientToDoc(CRect& rect)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc, NULL);
		dc.DPtoLP(rect);
		//ASSERT(rect.left <= rect.right);
		//ASSERT(rect.bottom <= rect.top);
	}
	/*****************************************************************************************
		<< --- CNPainterView::docToClient	2013-11-29 --- >>
		˵�����߼����굽�豸�����ת��
		������
		point	= ��ת���ĵ�
		����ֵ��
	*****************************************************************************************/
	void docToClient(CPoint& point)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc, NULL);
		dc.LPtoDP(&point);
	}
	/*****************************************************************************************
		<< --- CNPainterView::docToClient	2013-11-29 --- >>
		˵�����߼����굽�豸�����ת��
		������
		point	= ��ת��������
		����ֵ��
	*****************************************************************************************/
	void docToClient(RECT& rc)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc, NULL);
		dc.LPtoDP(&rc);
		
		//rc.NormalizeRect();
	}
	void docToClient(gtc::rect &rc)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc, NULL);
		POINT pt = {rc.x, rc.y};
		dc.LPtoDP(&pt);
		SIZE sz = {rc.width, rc.height};
		dc.LPtoDP(&sz);
		rc.setRect(pt.x, pt.y, sz.cx, sz.cy);
	}
public:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw(CDC* /*pDC*/);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_PROPERTY_CHANGED 	2013-12-23 --- >>
	˵����һ�����ڶ�������Է����ı�ʱ�������������Ķ����͸����Ա༭����
	������
	WPARAM	= �����ı����ԵĶ���
	LPARAM	= �������Լ���ָ��(itobjProp::PtrArrayType*)�������߸����ͷš�
	�����ߣ�
	CMainFrame		= ���¸����������ݣ������Դ��ڣ�����������
	����ֵ��
*****************************************************************************************/
	LRESULT OnMyItemPropertyChanged(WPARAM wParam, LPARAM lParam);
	/*****************************************************************************************
		<< --- �Զ����¼���WM_ME_ITEM_ACTION	2013-12-14 --- >>
		˵����ˢ�»�ͼ����
		������
		WPARAM	= �������BEItemAction
		LPARAM	= ��ѡ�еĶ��󼯺�ָ��(itobj::PtrArrayType*)�������߸����ͷš�
		����ֵ��
	*****************************************************************************************/
	LRESULT OnMyItemAction(WPARAM wParam, LPARAM lParam);
	/*****************************************************************************************
		<< --- �Զ����¼���WM_ME_ITEM_REDRAW 	2014-03-15 --- >>
		˵�������»��ƶ���
		������
		WPARAM	= �����ƵĶ���(itobj*)
		LPARAM	= ���Ƶ�����rect*���ͻ�����0.1mm���������߸����ͷš�
		�����ߣ�
		CNPainterView	= ˢ�»�ͼ����
		����ֵ��
	*****************************************************************************************/
	LRESULT OnMyItemRedraw(WPARAM wParam, LPARAM lParam);

public:
	// ��ͼ������Զ���������ʽ
	static CLIPFORMAT mc_cfBItemObjects;
public:
	CSize m_szWorker;		// ����̨�ߴ磨�߼���λ�� > ֽ�ųߴ� > ���ڿͻ���
	//CSize m_bsWorker, m_bsPaper;
	bool m_bActive;	// ��ͼ�Ƿ��ڼ���״̬
	gtc::itobj::PtrArrayType	m_vecSels;	// ��ѡ�еĶ��󼯺�
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNTool
//	˵���������Ĺ�������
//	����CAD��ͼ����
//		1����갴�µ����ʼ��ͼ����
//		2����esc��ȡ����ǰ��ͼ����
//		3���ٴΰ��»�space���������λ�ͼ����
//		4����shift��ʱ�ɶ�ѡ����
//		5����ctrl��ʱ����ѡ�ж���
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNTool
{
protected:
	CNTool() {}
	CNTool(BEToolShape shape);
	virtual ~CNTool(void) {}
public:
// Overridables
	virtual void OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnKeyDown(CNPainterView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnEditProperties(CNPainterView* pView);
	virtual void OnCancel();
	/*****************************************************************************************
		<< --- CNTool::releaseTool	2013-12-11 --- >>
		˵�����ͷŵ�ǰ������һ�㷢����ѡ�в�����OnLButtonUp����ͼ������OnLButtonDown���Լ�space��esc��
		������
		����ֵ��
	*****************************************************************************************/
	virtual bool releaseTool() {
		if(mc_isDrawing)
		{
			mc_isDrawing = false;
			ReleaseCapture();
			mc_drawShape = BEToolShapeSelection;
			return true;
		}
		return false;
	}

public:
	// ����ָ���������͵Ķ���
	static CNTool* findTool(BEToolShape drawShape);
public:
// ����
	BEToolShape	m_beDrawShape;

	static std::vector<CNTool*>	mc_vecTools;	// ÿ������Ҫ����̬ʵ������һ���������󣬲����뱾������
	static CPoint		mc_down;
	static UINT			mc_nDownFlags;
	static CPoint		mc_last;
	static BEToolShape	mc_drawShape;
	static bool			mc_isDrawing;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNToolSelect
//	˵��������ѡ�к����ز������磺��λ���ı��С...
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNToolSelect : public CNTool
{
// Constructors
public:
	CNToolSelect();

// Implementation
	virtual void OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CNPainterView* pView);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNToolRect
//	˵��������ѡ�к����ز������磺��λ���ı��С...
//	���ڣ�2013-11-30
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNToolRect : public CNTool
{
// Constructors
public:
	CNToolRect(BEToolShape drawShape);

// Implementation
	virtual void OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNToolGrid
//	˵��������ѡ�к����ز������磺��λ���ı��С...
//	���ڣ�2013-12-17
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CNToolGrid : public CNTool
{
// Constructors
public:
	CNToolGrid();

// Implementation
	virtual void OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point);
};

}
#endif