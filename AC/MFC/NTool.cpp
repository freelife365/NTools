#include "NTool.h"
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����
#include <afxole.h>
//#include "d:/VSTOOLS/AC/ATools.h"
//#include "d:/VSTOOLS/AC/BDevice/BDevice.h"
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/serialization/version.hpp>
//#include <boost/serialization/split_member.hpp>
//#include <boost/serialization/export.hpp>
//
//BOOST_CLASS_EXPORT(gtc::CAEntryItem)
//BOOST_CLASS_EXPORT(gtc::CAEntryItemValue)
//BOOST_CLASS_EXPORT(gtc::itobj)
//BOOST_CLASS_EXPORT(gtc::itobjRect)
//BOOST_CLASS_EXPORT(gtc::itobjGrid)
//BOOST_CLASS_EXPORT(gtc::itobjPage)
//BOOST_CLASS_EXPORT(gtc::itobjPageAxis)

namespace acm {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNPainterDoc
//	˵����������Ϊ�Զ����ͼ�ĵ��Ĺ������࣬�ڲ������˹�������
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CNPainterDoc, COleDocument)

BEGIN_MESSAGE_MAP(CNPainterDoc, COleDocument)
	//ON_COMMAND(ID_FILE_SAVE, &CNPainterDoc::OnFileSave)
END_MESSAGE_MAP()


// CMPainterDoc ����/����

CNPainterDoc::CNPainterDoc()
	: COleDocument()
{
	computePageSize();
}

CNPainterDoc::~CNPainterDoc()
{
}

BOOL CNPainterDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}


void CNPainterDoc::OnFileSave()
{
	//////COleDocument::OnFileSave();
	CString filepath;
	filepath.Empty();
	//static wchar_t filter[] = L"MPainter �ļ� (*.mpn)|*.mpn|�����ļ�*.*|*.*||";
	//DWORD flag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | AFX_IDS_SAVEFILE | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	//CFileDialog dlgFile(FALSE, L"*.mpn", NULL, flag, filter, NULL, 0);
	//if(dlgFile.DoModal() == IDOK)
	//{
	//	filepath = dlgFile.GetPathName();
	//	std::ofstream ofs(filepath.GetBuffer());
	//	boost::archive::binary_oarchive ar(ofs);
	//	ar & m_objAdmin;
	//	ofs.close();
	//}
}


// CNPainterDoc ���л�

void CNPainterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
///*****************************************************************************************
//	<< --- CNPainterDoc::draw	2013-12-08 --- >>
//	˵�������ƻ�ͼģ��
//	������
//	pdc	= ��ͼ����
//	����ֵ��
//*****************************************************************************************/
//void CNPainterDoc::draw(CDC *pdc)
//{
//	CNPainterView *pview = nullptr;
//	gtc::itobj *pobj = nullptr;
//	POSITION pos = GetFirstViewPosition();
//	if(pos)
//		pview = dynamic_cast<CNPainterView*>(GetNextView(pos));
//	for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < m_vecObjects.size(); ++idx)
//	{
//		pobj = m_vecObjects[idx];
//		pobj->draw(pdc);
//		if(pview->m_bActive && !pdc->IsPrinting() && pview->isSelected(pobj))
//			pobj->drawTracker(pdc, gtc::itobj::TSSelected,m_clrTracker);
//	}
//}

/*****************************************************************************************
	<< --- CNPainterDoc::computePageSize	2013-12-04 --- >>
	˵��������ϵͳ�趨�Ĵ�ӡ���õ�itobjPage��Ϣ
	������
	����ֵ��
*****************************************************************************************/
void CNPainterDoc::computePageSize()
{
	PRINTDLG pd = {0};
	CSize oldSize = m_objAdmin.getPager()->getPaperInfo().bsSize.toMSSize();// m_objPaper.getPaperInfo().bsSize.toMSSize();
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		DEVMODE *pdev = (DEVMODE*)GlobalLock(pd.hDevMode);
		if(pdev)
		{
			gtc::BSPaperInfo &paper = m_objAdmin.getPager()->getPaperInfo();
			paper.iOrientation = pdev->dmOrientation;
			paper.iSize = pdev->dmPaperSize;
			paper.bsSize.width = pdev->dmPaperWidth;
			paper.bsSize.height = pdev->dmPaperLength;
			paper.wsName = pdev->dmFormName;
			GlobalUnlock(pd.hDevMode);

			m_objAdmin.getPager()->refresh();
		}
	}
	//if(oldSize != m_objPaper.getPaperInfo().bsSize.toMSSize())
	{	// ˢ�¹�����
		POSITION pos = GetFirstViewPosition();
		if(pos != NULL)
			((CNPainterView*)GetNextView(pos))->OnUpdate(nullptr, HINT_UPDATE_WINDOW, nullptr);
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNPainterView
//	˵����������Ϊ�Զ����ͼ��ͼ�Ĺ������࣬�ڲ������˹�������
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ͼ������Զ���������ʽ
CLIPFORMAT CNPainterView::mc_cfBItemObjects = (CLIPFORMAT)::RegisterClipboardFormat(L"BItem Objects");
BEGIN_MESSAGE_MAP(CNPainterView, CScrollView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_ME_ITEM_PROPERTY_CHANGED, &CNPainterView::OnMyItemPropertyChanged)
	ON_MESSAGE(WM_ME_ITEM_ACTION, &CNPainterView::OnMyItemAction)
	ON_MESSAGE(WM_ME_ITEM_REDRAW, &CNPainterView::OnMyItemRedraw)
END_MESSAGE_MAP()



CNPainterView::CNPainterView()
	:m_bActive(false)
	, m_szWorker(3500,3500)/*2970+400, 2100+400)*/
{

}
///*****************************************************************************************
//	<< --- CNPainterView::drawBackground	2013-12-02 --- >>
//	˵�������ƹ�������ֽ��
//	������
//	pdc	= CDC*
//	����ֵ��
//*****************************************************************************************/
//void CNPainterView::drawBackground(CDC *pdc)
//{
//	CNPainterDoc *pdoc = GetDocument();
//	if(!pdoc || !pdc || pdc->IsPrinting())
//		return;
//	gtc::itobjPage &page = pdoc->getPaper();
//	CSize szworker = m_szWorker;
//	CSize szpaper = page.getPaperInfo().bsSize.toMSSize();
//	CRect rtworker, rtpaper;
//	rtworker.left = -(szworker.cx-szpaper.cx)/2;
//	rtworker.right = (szworker.cx+szpaper.cx)/2;
//	rtworker.top = (szworker.cy+szpaper.cy)/2;
//	rtworker.bottom = -(szworker.cy-szpaper.cy)/2;
//	if(page.supportMargins())
//	{
//#if defined(WORK_AXIS_CAD)
//		rtworker.OffsetRect(-page.getMarginLeft(), -page.getMarginBottom());
//#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
//		rtworker.OffsetRect(-page.getMarginLeft(), -page.getMarginTop());
//#endif
//	}
//	if(page.supportHeaderFooter())
//	{	// Y�᷽��ԭ��(0,0)��������
//#if defined(WORK_AXIS_CAD)
//		rtworker.OffsetRect(0, -page.getFooter().uiHeight);
//#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
//		rtworker.OffsetRect(0, -page.getHeader().uiHeight);
//#endif
//	}
//	rtworker.NormalizeRect();
//	rtworker.DeflateRect(10,10);
//	pdc->FillRect(rtworker, &CBrush(0xFFEAEAEA));
//	pdoc->getPaper().draw(pdc);
////#if defined(WORK_AXIS_CAD)
////	rtpaper.NormalizeRect();
////	rtpaper.OffsetRect(10, -10);
////	pdc->FillRect(rtpaper, &CBrush(0xFF808080));
////	rtpaper.OffsetRect(-10, 10);
////	pdc->FillRect(rtpaper, &CBrush(0xFFFFFFFF));
////#elif defined(WORK_AXIS_DOC)
////	rtpaper.NormalizeRect();
////	rtpaper.OffsetRect(10, 10);
////	pdc->FillRect(rtpaper, &CBrush(0xFF808080));
////	rtpaper.OffsetRect(-10, -10);
////	pdc->FillRect(rtpaper, &CBrush(0xFFFFFFFF));
////#else	// defined(WORK_AXIS_NORMAL)
////	rtpaper.NormalizeRect();
////	rtpaper.OffsetRect(10, 10);
////	pdc->FillRect(rtpaper, &CBrush(0xFF808080));
////	rtpaper.OffsetRect(-10, -10);
////	pdc->FillRect(rtpaper, &CBrush(0xFFFFFFFF));
////#endif
//}
/*****************************************************************************************
	<< --- CNPainterView::resetScrollSizes	2013-12-02 --- >>
	˵�������ݴ��ڿͻ������豸���꣺���أ��趨�������䣬ʹֽ�����ڴ��ڿͻ�������
	������
	clientArea	= ���ڿͻ�������GetClientRect��OnSize�õ����豸���꣩
	����ֵ��
*****************************************************************************************/
void CNPainterView::resetScrollSizes(CSize clientArea)
{
	CNPainterDoc *pdoc = GetDocument();
	if(!pdoc)
		return;
	CRect rtclient;
	const CSize client = clientArea;
	if(clientArea.cx <= 0 || clientArea.cy <= 0)
	{
		GetClientRect(rtclient);
		rtclient.NormalizeRect();
		clientArea.SetSize(rtclient.Width(), rtclient.Height());
	}
	CClientDC dc(NULL);
	// ת�����߼�����
	clientArea.cx = MulDiv(clientArea.cx, 254, dc.GetDeviceCaps(LOGPIXELSX));
	clientArea.cy = MulDiv(clientArea.cy, 254, dc.GetDeviceCaps(LOGPIXELSY));
	// ����̨�ߴ磨�߼���λ�� > ֽ�ųߴ� > ���ڿͻ���
	int32_t cx = 0;// max(clientArea.cx, pdoc->getAdminer().getPager().getPaperInfo().bsSize.cx)+20;
	int32_t cy = 0;// max(clientArea.cy, pdoc->getAdminer().getPager().getPaperInfo().bsSize.cy)+20;
	gtc::BSPaperInfo &paper = pdoc->getAdminer().getPager()->getPaperInfo();
	if(paper.isLandscape())	// Ĭ��������1 ����2
	{
		cx = max(clientArea.cx, paper.bsSize.height)+20;
		cy = max(clientArea.cy, paper.bsSize.width)+20;
		//std::swap(cx, cy);
	}
	else
	{
		cx = max(clientArea.cx, paper.bsSize.width)+20;
		cy = max(clientArea.cy, paper.bsSize.height)+20;
	}
	if(m_szWorker.cx < cx)
		m_szWorker.cx = cx;
	if(m_szWorker.cy < cy)
		m_szWorker.cy = cy;
	// 
	CSize szWorker = m_szWorker;
	szWorker.cx = MulDiv(szWorker.cx, dc.GetDeviceCaps(LOGPIXELSX), 254);
	szWorker.cy = MulDiv(szWorker.cy, dc.GetDeviceCaps(LOGPIXELSY), 254);
	SetScrollSizes(MM_TEXT, szWorker);
	// ˮƽ����������
	SCROLLINFO scrinfo = {0};
	scrinfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ, &scrinfo);
	SetScrollPos(SB_HORZ, (scrinfo.nMax-scrinfo.nMin-scrinfo.nPage)/2);
	// ��ֱ��ʾֽ��
	GetScrollInfo(SB_VERT, &scrinfo);
	CSize szPaper = paper.bsSize.toMSSize();
	if(paper.isLandscape())	// Ĭ��������
		std::swap(szPaper.cx, szPaper.cy);
	szPaper.cx = MulDiv(szPaper.cx, dc.GetDeviceCaps(LOGPIXELSX), 254);
	szPaper.cy = MulDiv(szPaper.cy, dc.GetDeviceCaps(LOGPIXELSY), 254);
	SetScrollPos(SB_VERT, (scrinfo.nMax-scrinfo.nMin-scrinfo.nPage)*(szPaper.cy+50)/szWorker.cy);

}

/*****************************************************************************************
	<< --- CNPainterView::moveHandleTo	2013-11-30 --- >>
	˵����ѡ��ָ�������ڵĶ���
	������
	rect	= ����
	bAdd	= trueʱ����ѡ�ж�����׷�Ӷ��󣬷��������ѡ�м�¼��
	����ֵ��
*****************************************************************************************/
void CNPainterView::selectWithinRect(CRect rect, bool bAdd)
{
	if(!bAdd)
		select(nullptr);
	clientToDoc(rect);
	gtc::rect bsrt(rect.left, rect.top, rect.right, rect.bottom);
	gtc::itobj::PtrArrayType &objs = GetDocument()->getAdminer().getItems();
	for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < objs.size(); ++idx)
	{
		if(objs[idx]->intersects(bsrt))
			select(objs[idx], true);
	}

}
/*****************************************************************************************
	<< --- CNPainterView::editProperties	2013-11-30 --- >>
	˵����ͨ�����ԶԻ���༭pobj������
	������
	pobj	= ���༭����
	����ֵ��
*****************************************************************************************/
void CNPainterView::editProperties(gtc::itobj* pobj)
{
}

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
LRESULT CNPainterView::OnMyItemPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	gtc::itobj* pobj = (gtc::itobj*)lParam;
	if(pobj)
	{
		invalidate(pobj);
	}
	return 0;
}

/*****************************************************************************************
	<< --- �Զ����¼���WM_ME_ITEM_ACTION	2013-12-14 --- >>
	˵����ˢ�»�ͼ����
	������
	WPARAM	= �������BEItemAction
	LPARAM	= ��ѡ�еĶ��󼯺�ָ��(itobj::PtrArrayType*)�������߸����ͷš�
	����ֵ��
*****************************************************************************************/
LRESULT CNPainterView::OnMyItemAction(WPARAM wParam, LPARAM lParam)
{
	BSActionSender *psender = (BSActionSender *)wParam;
	gtc::itobj* pobj = (gtc::itobj*)lParam;
	return 0;
}

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
LRESULT CNPainterView::OnMyItemRedraw(WPARAM wParam, LPARAM lParam)
{
	gtc::itobj *pobj = (gtc::itobj*)wParam;
	gtc::rect *prect = (gtc::rect*)lParam;
	if (pobj)
	{
		if (prect)
		{
			invalidate(*prect);
			//CClientDC dc(this);
			//OnPrepareDC(&dc, NULL);
			//RECT rc = *prect;
			//dc.Rectangle(&rc);
			//RECT rc = prect->operator RECT();
			//docToClient(rc);
			//::InflateRect(&rc, 1, 1);
			//invalidate(rc);
			//docToClient(*prect);
			//prect->inflate(1,1);
			//invalidate(*prect);
		}
		else
		{
			invalidate(pobj);
		}
	}
	delete prect;

	return 0;
}


void CNPainterView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		//invalidate((CDrawObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < m_vecSels.size(); ++idx)
				invalidate(m_vecSels[idx]);
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		break;
	case HINT_UPDATE_OLE_ITEMS:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CNPainterView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	// invalidate selections when active status changes
	if(m_bActive != (bActivate == TRUE))
	{
		if(bActivate)
			m_bActive = true;
		if(!m_vecSels.empty())
			OnUpdate(nullptr, HINT_UPDATE_SELECTION, nullptr);
		m_bActive = bActivate == TRUE;
	}

}


void CNPainterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	resetScrollSizes();

}


void CNPainterView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	CNPainterDoc *pdoc = GetDocument();
	if(!pdoc)
		return;
	////1.SetWindowOrg(x, y) �ǰ��豸�����ԭ��(�ӿ�)ӳ�䵽�߼������(X, Y)�� ���鿴�������⴦
	////2.SetViewportOrg(x, y) �ǰ��߼������ԭ��(����)ӳ�䵽�豸�����(X, Y)��
	// 
#if defined(WORK_AXIS_CAD)	// ���ҡ��ϵ�����1�߼���λ����0.1����
	pDC->SetMapMode(MM_ISOTROPIC);

	long wpin = pDC->GetDeviceCaps(LOGPIXELSX);	// ��Ļˮƽ����ÿӢ�����ص���	
	long hpin = pDC->GetDeviceCaps(LOGPIXELSY);	// ��Ļ��ֱ����ÿӢ�����ص���	

	pDC->SetWindowExt(254, -254);		// ���ڵĳߴ磬�߼����ꣻĬ��ʱ���ң����µ������为ʱ�ͷ�������
	pDC->SetViewportExt(wpin, hpin);	// �����ĳߴ磬�豸���꣨���أ�
	pDC->SetWindowOrg(pdoc->getAdminer().getPager()->getWindowOrg(gtc::size(m_szWorker.cx, m_szWorker.cy), pDC->IsPrinting()).toMSPoint());
	//pDC->OffsetWindowOrg(300, 300);
#elif defined(WORK_AXIS_DOC)	// ���ҡ��µ�����1�߼���λ����0.1����
	pDC->SetMapMode(MM_ISOTROPIC);

	long wpin = pDC->GetDeviceCaps(LOGPIXELSX);	// ��Ļˮƽ����ÿӢ�����ص���	
	long hpin = pDC->GetDeviceCaps(LOGPIXELSY);	// ��Ļ��ֱ����ÿӢ�����ص���	

	pDC->SetWindowExt(254, 254);		// ���ڵĳߴ磬�߼����ꣻĬ��ʱ���ң����µ������为ʱ�ͷ�������
	pDC->SetViewportExt(wpin, hpin);	// �����ĳߴ磬�豸���꣨���أ�
	pDC->SetWindowOrg(pdoc->getAdminer().getPager().getWindowOrg(gtc::size(m_szWorker.cx, m_szWorker.cy), pDC->IsPrinting()).toMSPoint());

	//CSize worker, paper;
	//// ����������С���߼�����ߴ磩ӳ�䵽�豸���꣨���أ�
	//worker.cx = MulDiv(wpin, m_bsWorker.cx, 254);
	//worker.cy = MulDiv(hpin, m_bsWorker.cy, 254);
	//// ����ӡֽ��С���߼�����ߴ磩ӳ�䵽�豸���꣨���أ�
	//paper.cx = MulDiv(wpin, m_bsPaper.cx, 254);
	//paper.cy = MulDiv(hpin, m_bsPaper.cy, 254);
	////1.SetWindowOrg(x, y) �ǰ��豸�����ԭ��(�ӿ�)ӳ�䵽�߼������(X, Y)�� ���鿴�������⴦
	////2.SetViewportOrg(x, y) �ǰ��߼������ԭ��(����)ӳ�䵽�豸�����(X, Y)��
	//CPoint ptorg;
	//ptorg.x = (m_bsWorker.cx-m_bsPaper.cx)/2;
	//ptorg.y = (m_bsWorker.cy-m_bsPaper.cy)/2;
	//pDC->SetWindowOrg(-ptorg.x, -ptorg.y);
	
#else //defined(WORK_AXIS_NORMAL)
	pDC->SetMapMode(MM_TEXT);
#endif

}



void CNPainterView::OnDraw(CDC* pDC)
{
	CNPainterDoc* pdoc = GetDocument();
	ASSERT_VALID(pdoc);
	if (!pdoc)
		return;

	CRect client, rect;
	
	CDC dc;
	CDC *pdrawdc = pDC;
	CBitmap bitmap;
	CBitmap* poldbmp = nullptr;
	pDC->GetClipBox(client);
	rect = client;
	docToClient(rect);
	CString str;
	str.Format(_T("CNPainterView::OnDraw clipRect:<%d,%d,%d,%d %d,%d>"), rect.left, rect.top, rect.right, rect.bottom, rect.Width(), rect.Height());
	AfxGetMainWnd()->SetWindowText(str);
	if(!pDC->IsPrinting())
	{
		if(dc.CreateCompatibleDC(pDC))
		{
			if(bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pdrawdc = &dc;
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				poldbmp = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				//dc.SelectClipRgn(NULL);
				dc.IntersectClipRect(client);
			}
		}
	}
	//// ���±���
	//if(!pdrawdc->IsPrinting())
	//	DoDrawBackground(pdrawdc);
	//// ����ͼƬ
	//pdoc->m_adminObjs.DrawPaperBack(pdrawdc);
	////// ������ VSP3.1 �������Ѿ����ɵ�itobjPage::Draw()
	////if(!pdrawdc->IsPrinting() && pdoc->m_adminObjs.IsVisibleGrid())
	////	DoDrawGrid(pdrawdc);
	//pdoc->m_adminObjs.DrawObjects(pdrawdc);
	//// ����ѡ�б�ʶ
	//DoDrawSelectItems(pdrawdc);
	// 
	pdoc->getAdminer().drawBackground(pdrawdc, m_szWorker);
	//// ������
	pdoc->getAdminer().draw(pdrawdc, this);
	pdoc->draw(pdrawdc);
	// ˢ�¿ͻ���
	
	//if(pdrawdc != pDC)
	//{	
	//	pDC->BitBlt(client.left, client.top, client.Width(), client.Height(), pdrawdc, client.left, client.top, SRCCOPY);
	//	if(poldbmp)
	//		pdrawdc->SelectObject(poldbmp);
	//}
	if (pdrawdc != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(poldbmp);
	}

	

}


void CNPainterView::OnSize(UINT nType, int cx, int cy)
{
//#ifdef DEBUG
//	DEBUG_LOG(L"exec CNPainterView::OnSize ......");
//#endif
	CScrollView::OnSize(nType, cx, cy);
	if(m_bActive)
		resetScrollSizes(CSize(cx, cy));
}

void CNPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bActive)
		return;
	CNTool *ptool = CNTool::findTool(CNTool::mc_drawShape);
	if(ptool)
		ptool->OnLButtonDown(this, nFlags, point);
	//CScrollView::OnLButtonDown(nFlags, point);
}



void CNPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bActive)
		return;
	CNTool *ptool = CNTool::findTool(CNTool::mc_drawShape);
	if(ptool)
		ptool->OnLButtonUp(this, nFlags, point);
	//CScrollView::OnLButtonUp(nFlags, point);
}



void CNPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bActive)
		return;
	CNTool *ptool = CNTool::findTool(CNTool::mc_drawShape);
	if(ptool)
		ptool->OnMouseMove(this, nFlags, point);
	//CScrollView::OnMouseMove(nFlags, point);
}
void CNPainterView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!m_bActive)
		return;
	CNTool *ptool = CNTool::findTool(CNTool::mc_drawShape);
	if(ptool)
		ptool->OnKeyDown(this, nChar, nRepCnt, nFlags);
	//CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CNPainterView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_bActive)
		return;
	CNTool *ptool = CNTool::findTool(CNTool::mc_drawShape);
	if(ptool)
		ptool->OnLButtonDblClk(this, nFlags, point);
	//CScrollView::OnLButtonDblClk(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNTool
//	˵���������Ĺ�������
//	���ڣ�2013-11-29
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��̬������ʼ��
std::vector<CNTool*>	CNTool::mc_vecTools;
CPoint CNTool::mc_down = CPoint(0,0);
UINT CNTool::mc_nDownFlags;
CPoint CNTool::mc_last;
BEToolShape CNTool::mc_drawShape = BEToolShapeSelection;
bool CNTool::mc_isDrawing = false;
// ʵ��������
static CNToolSelect selectTool;
static CNToolRect lineTool(BEToolShapeLine);
static CNToolRect rectTool(BEToolShapeRect);
static CNToolRect roundRectTool(BEToolShapeRoundRect);
static CNToolRect ellipseTool(BEToolShapeEllipse);
static CNToolGrid gridTool;
//static CNToolRect polyTool;

inline CNTool::CNTool(BEToolShape shape)
	: m_beDrawShape(shape)
{
	mc_vecTools.push_back(this);
}

inline CNTool* CNTool::findTool(BEToolShape drawShape)
{
	for(std::vector<CNTool*>::size_type idx = 0; idx < mc_vecTools.size(); ++idx)
	{
		if(mc_vecTools[idx]->m_beDrawShape == drawShape)
			return mc_vecTools[idx];
	}
	return nullptr;
}

//
void CNTool::OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	// deactivate any in-place active item on this view!
	COleClientItem* pActiveItem = pView->GetDocument()->GetInPlaceActiveItem(pView);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
		ASSERT(pView->GetDocument()->GetInPlaceActiveItem(pView) == NULL);
	}	
	pView->SetCapture();
	mc_nDownFlags = nFlags;
	mc_down = point;
	mc_last = point;
	mc_isDrawing = true;

}

void CNTool::OnLButtonDblClk(CNPainterView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
}
void CNTool::OnMouseMove(CNPainterView* /*pView*/, UINT /*nFlags*/, const CPoint& point)
{
	mc_last = point;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CNTool::OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	//ReleaseCapture();

	//if (point == mc_down)
	//	mc_drawShape = BEToolShapeSelection;
}

void CNTool::OnKeyDown(CNPainterView* pView, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!pView)
		return;
	// ѡ�ж�������������ƶ�Ĭ��Ϊ1mm/�Σ��簴��Ctrl����΢��0.2mm/��
	bool isMove = false;
	int32_t off = 10, offx = 0, offy = 0;
	if((nFlags & MK_CONTROL) != 0)
		off = 2;

	switch(nChar)
	{
	case VK_SPACE:	// �������λ�ͼ����
		releaseTool();
		break;
	case VK_ESCAPE:	// ȡ������ѡ�ж����ѡ�б��
		{
			releaseTool();
			gtc::itobj::PtrArrayType vecSels = pView->m_vecSels;
			pView->m_vecSels.clear();
			for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < vecSels.size(); ++idx)
			{
				CRect rt = vecSels[idx]->getPosition();
				pView->docToClient(rt);
				int space = gtc::itobj::mc_iTrackerSize/2;
				//if(m_bActive && isSelected(pobj))
				{
					rt.left -= space;
					rt.top -= space+1;
					rt.right += space+1;
					rt.bottom += space;
				}
				rt.InflateRect(1,1);
				pView->InvalidateRect(rt, FALSE);
			}
		}
		break;
	case VK_UP:		// ����
		isMove = true;
		offy = -off;
		break;
	case VK_DOWN:	// ����
		isMove = true;
		offy = off;
		break;
	case VK_LEFT:	// ����
		isMove = true;
		offx = -off;
		break;
	case VK_RIGHT:	// ����
		isMove = true;
		offx = off;
		break;
	}
	if(isMove)
	{
		gtc::rect rt;
		for(gtc::itobj::PtrArrayType::iterator iter = pView->m_vecSels.begin(), end = pView->m_vecSels.end(); iter != end; ++iter)
		{
			rt = (*iter)->getPosition();
			rt.offset(offx, offy);
			(*iter)->moveTo(rt, pView);
			std::vector<int32_t> vecProp;
			vecProp.push_back(gtc::itobj::BEItemPropPositionX);
			vecProp.push_back(gtc::itobj::BEItemPropPositionY);
			vecProp.push_back(gtc::itobj::BEItemPropPositionWidth);
			vecProp.push_back(gtc::itobj::BEItemPropPositionHeight);
			(*iter)->messageProperty(gtc::BEDrawShapeTypeItem, vecProp);
		}
	}
}

void CNTool::OnEditProperties(CNPainterView* /*pView*/)
{
}

void CNTool::OnCancel()
{
	mc_drawShape = BEToolShapeSelection;
}

/*****************************************************************************************
	CNToolSelect
	˵��������ѡ�к����ز������磺��λ���ı��С...
	���ڣ�2013-11-29
*****************************************************************************************/

enum BESelectMode
{
	BESelectModeNone,
	BESelectModeNetSelect,
	BESelectModeMove,
	BESelectModeSize		// ͨ����λ��ı����ߴ�
};

BESelectMode g_beSelectMode = BESelectModeNone;
int g_iDragHandle;

CPoint lastPoint;

CNToolSelect::CNToolSelect()
	: CNTool(BEToolShapeSelection)
{
}

void CNToolSelect::OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point)
{

	CPoint local = point;
	pView->clientToDoc(local);

	gtc::itobj* pobj = nullptr;
	g_beSelectMode = BESelectModeNone;

	// Check for resizing (only allowed on single selections)
	if (pView->m_vecSels.size() == 1)
	{
		//pObj = pView->m_selection.GetHead();
		//nDragHandle = pObj->HitTest(local, pView, TRUE);
		//if (nDragHandle != 0)
		//	selectMode = size;
		pobj = pView->m_vecSels[0];
		g_iDragHandle = pobj->hitTest(local, pView, true);// CNTool::hitTest(pobj, local, pView, true);
		if(g_iDragHandle > 0)
			g_beSelectMode = BESelectModeSize;
	}

	// See if the click was on an object, select and start move if so
	if (g_beSelectMode == BESelectModeNone)
	{
		pobj = pView->GetDocument()->objectAt(local);

		if (pobj != nullptr)
		{
			if(pobj->getType() == gtc::BEDrawShapeTypeGrid)
			{	// ѡ�е�Ԫ��
				pView->invalidate(pobj);
			}
			g_beSelectMode = BESelectModeMove;

			if (!pView->isSelected(pobj))
				pView->select(pobj, (nFlags & MK_SHIFT) != 0);

			// Ctrl+Click clones the selection...
			if ((nFlags & MK_CONTROL) != 0)
				pView->cloneSelection();
		}
	}

	// Click on background, start a net-selection
	if (g_beSelectMode == BESelectModeNone)
	{
		if ((nFlags & MK_SHIFT) == 0)
			pView->select(nullptr);

		g_beSelectMode = BESelectModeNetSelect;// netSelect;

		CClientDC dc(pView);
		CRect rt(point.x, point.y, point.x, point.y);
		rt.NormalizeRect();
		dc.DrawFocusRect(rt);
	}

	lastPoint = local;
	CNTool::OnLButtonDown(pView, nFlags, point);
}

void CNToolSelect::OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	if (pView->GetCapture() != pView)
	{
		if (mc_drawShape == BEToolShapeSelection && pView->m_vecSels.size() == 1)
		{	// ���ֻѡ����һ�����󣬾�ͨ����λ��ı�ߴ�
			//CDrawObj* pObj = pView->m_selection.GetHead();
			//CPoint local = point;
			//pView->ClientToDoc(local);
			//int nHandle = pObj->HitTest(local, pView, TRUE);
			//if (nHandle != 0)
			//{
			//	SetCursor(pObj->GetHandleCursor(nHandle));
			//	return; // bypass CDrawTool
			//}
			gtc::itobj* pobj = pView->m_vecSels[0];
			CPoint local = point;
			pView->clientToDoc(local);
			int nHandle = pobj->hitTest(local, pView, true);// hitTest(pobj, local, pView, true);
			if(nHandle > 0)
			{
				SetCursor(pobj->getHandleCursor(nHandle));
				return;
			}

		}
		if (mc_drawShape == BEToolShapeSelection)
			CNTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	if (g_beSelectMode == BESelectModeNetSelect)
	{
		CClientDC dc(pView);
		CRect rt(mc_down.x, mc_down.y, mc_last.x, mc_last.y);
		rt.NormalizeRect();
		dc.DrawFocusRect(rt);
		rt.SetRect(mc_down.x, mc_down.y, point.x, point.y);
		rt.NormalizeRect();
		dc.DrawFocusRect(rt);

		CNTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	CPoint local = point;
	pView->clientToDoc(local);
	CPoint delta = (CPoint)(local - lastPoint);
	for(gtc::itobj::PtrArrayType::size_type idx = 0; idx < pView->m_vecSels.size(); ++idx)
	{
		gtc::itobj* pobj = pView->m_vecSels[idx];
		CRect pos = pobj->getPosition();
		if(g_beSelectMode == BESelectModeMove)
		{	// �����ƶ�
			pos += delta;
			pobj->moveTo(pos, pView);
			//pView->moveTo(pos, pobj);
		}
		else if(g_iDragHandle != 0)
		{	// �ı�ߴ�
			pobj->moveHandleTo(g_iDragHandle, local, pView);
			//pView->moveHandleTo(g_iDragHandle, local, pobj);
		}
	}

	/*POSITION pos = pView->m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = pView->m_selection.GetNext(pos);
		CRect position = pObj->m_position;

		if (selectMode == move)
		{
			position += delta;
			pObj->MoveTo(position, pView);
		}
		else if (nDragHandle != 0)
		{
			pObj->MoveHandleTo(nDragHandle, local, pView);
		}
	}*/

	lastPoint = local;

	if (g_beSelectMode == BESelectModeSize && mc_drawShape == BEToolShapeSelection)
	{
		mc_last = point;
		SetCursor(pView->m_vecSels[0]->getHandleCursor(g_iDragHandle));
		//SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
		return; // bypass CDrawTool
	}

	mc_last = point;

	if (mc_drawShape == BEToolShapeSelection)
		CNTool::OnMouseMove(pView, nFlags, point);
}

void CNToolSelect::OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	if (pView->GetCapture() == pView)
	{
		if (g_beSelectMode == BESelectModeNetSelect)
		{
			CClientDC dc(pView);
			CRect rt(mc_down.x, mc_down.y, mc_last.x, mc_last.y);
			rt.NormalizeRect();
			dc.DrawFocusRect(rt);

			pView->selectWithinRect(rt, true);
		}
		else if (g_beSelectMode != BESelectModeNone)
		{
			pView->GetDocument()->UpdateAllViews(pView);
		}
		// 
		BSActionSender *psender = new BSActionSender(BESenderFlagView, BEItemActionSelect);
		AfxGetMainWnd()->SendMessage(WM_ME_ITEM_ACTION, (WPARAM)psender, 0L);
	}
	if(mc_drawShape == BEToolShapeSelection)
	{
		releaseTool();
	}

	CNTool::OnLButtonUp(pView, nFlags, point);
}

void CNToolSelect::OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	if ((nFlags & MK_SHIFT) != 0)
	{
		// Shift+DblClk deselects object...
		CPoint local = point;
		pView->clientToDoc(local);
		gtc::itobj* pobj = pView->GetDocument()->objectAt(local);
		if (pobj != nullptr)
			pView->deselect(pobj);
	}
	else
	{
		// "Normal" DblClk opens properties, or OLE server...
		if(pView->m_vecSels.size() == 1)
		{
			OnEditProperties(pView);
		}
/*		if (pView->m_selection.GetCount() == 1)
			pView->m_selection.GetHead()->OnOpen(pView);*/
	}

	CNTool::OnLButtonDblClk(pView, nFlags, point);
}

void CNToolSelect::OnEditProperties(CNPainterView* pView)
{
	//if (pView->m_selection.GetCount() == 1)
	//	pView->m_selection.GetHead()->OnEditProperties();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNToolRect
//	˵��������ѡ�к����ز������磺��λ���ı��С...
//	���ڣ�2013-11-30
////////////////////////////////////////////////////////////////////////////////////////////////////////
CNToolRect::CNToolRect(BEToolShape drawShape)
	: CNTool(drawShape)
{
}

void CNToolRect::OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	if(releaseTool() && pView->m_vecSels.size() == 1)
	{
		//pView->m_vecSels[0]->getPosition().normalizeRect();
		return;
	}
	CNTool::OnLButtonDown(pView, nFlags, point);
	pView->select(nullptr);
	CPoint local = point;
	pView->clientToDoc(local);
	gtc::itobjRect *pobj = new gtc::itobjRect(gtc::rect(local.x, local.y, 1, 1));//local.x, local.y));
	switch (m_beDrawShape)
	{
	default:
		ASSERT(FALSE); // unsuported shape!

	case BEToolShapeRect:
		pobj->setShape(gtc::itobjRect::BEShapeRectangle);
		break;

	case BEToolShapeRoundRect:
		pobj->setShape(gtc::itobjRect::BEShapeRoundRectangle);
		break;

	case BEToolShapeEllipse:
		pobj->setShape(gtc::itobjRect::BEShapeEllipse);
		break;

	case BEToolShapeLine:
		pobj->setShape(gtc::itobjRect::BEShapeLine);
		break;
	}
	pView->GetDocument()->add(pobj);
	pView->select(pobj);

	g_beSelectMode = BESelectModeSize;
	g_iDragHandle = 1;
	lastPoint = local;
}

void CNToolRect::OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	selectTool.OnMouseMove(pView, nFlags, point);
}

void CNToolRect::OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	//if (point == mc_down && pView->m_vecSels.size() > 0)
	//{
	//	// Don't create empty objects...
	//	gtc::itobj* pobj = pView->m_vecSels[pView->m_vecSels.size()-1];
	//	pView->remove(pobj);
	//	pView->GetDocument()->remove(pobj);
	//	//pobj->remove();
	//	//CDrawObj *pObj = pView->m_selection.GetTail();
	//	//pView->GetDocument()->Remove(pObj);
	//	//pObj->Remove();
	//	selectTool.OnLButtonDown(pView, nFlags, point); // try a select!
	//}

	selectTool.OnLButtonUp(pView, nFlags, point);
}

//
void CNToolRect::OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	CNTool::OnLButtonDblClk(pView, nFlags, point);
}
//

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CNToolGrid
//	˵��������ѡ�к����ز������磺��λ���ı��С...
//	���ڣ�2013-12-17
////////////////////////////////////////////////////////////////////////////////////////////////////////

CNToolGrid::CNToolGrid()
	: CNTool(BEToolShapeGrid)
{
}

// Implementation
void CNToolGrid::OnLButtonDown(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	//if(releaseTool() && pView->m_vecSels.size() == 1)
	//{
	//	pView->m_vecSels[0]->getPosition().normalizeRect();
	//	return;
	//}
	//CNTool::OnLButtonDown(pView, nFlags, point);
	//pView->select(nullptr);
	//CPoint local = point;
	//pView->clientToDoc(local);
	//gtc::itobjGrid *pgrid = new gtc::itobjGrid(gtc::rect(local.x, local.y, local.x, local.y));
	//
	//pView->GetDocument()->add(pgrid);
	//pView->select(pgrid);

	//g_beSelectMode = BESelectModeSize;
	//g_iDragHandle = 1;
	//lastPoint = local;
}
void CNToolGrid::OnMouseMove(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	selectTool.OnMouseMove(pView, nFlags, point);
}
void CNToolGrid::OnLButtonUp(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	selectTool.OnLButtonUp(pView, nFlags, point);
}
void CNToolGrid::OnLButtonDblClk(CNPainterView* pView, UINT nFlags, const CPoint& point)
{
	CNTool::OnLButtonDblClk(pView, nFlags, point);
}

}
