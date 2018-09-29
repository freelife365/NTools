#include "itobjAdmin.h"
#include "AC\MFC\NTool.h"
namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjAdmin	2013-12-13 --- >>
//	��ͼ��Ԫ������
////////////////////////////////////////////////////////////////////////////////////////////////////////

	
itobjAdmin::itobjAdmin()
	: m_clrTracker(0xFFFF6633)
	, m_pobjPage(new itobjPage())
{
#if defined(WORK_AXIS_CAD)
	m_beWorkaxis = BEWorkAxisCAD;
#elif defined(WORK_AXIS_DOC)
	m_beWorkaxis = BEWorkAxisDOC;
#elif defined(WORK_AXIS_NORMAL)
	m_beWorkaxis = BEWorkAxisNormal;
#endif
	//rect rt(0,0,1000,1000);
	//rt.normalizeRect();
	//mpobj = new itobj(rt);
	//mpobj->setAlignMode(BEAlignModeRightCenter);
	////mpobj->getFont().iEscapement = 900;
	//mpobj->setText(L"dAQq��֮332yPqg");
	//mpobj->setBackColor(0xFF0000ff);

}
itobjAdmin::itobjAdmin(const itobjAdmin &admin)
	: m_clrTracker(admin.m_clrTracker), m_beWorkaxis(admin.m_beWorkaxis)
{
	if(m_pobjPage)
		m_pobjPage->remove();
	if(admin.m_pobjPage)
		m_pobjPage = dynamic_cast<itobjPage *>(admin.m_pobjPage->clone());
	copy(admin.m_vecItems);
}
itobjAdmin& itobjAdmin::operator=(const itobjAdmin &admin)
{
	if(this != &admin)
	{
		m_clrTracker = admin.m_clrTracker;
		m_beWorkaxis = admin.m_beWorkaxis;
		if(m_pobjPage)
			m_pobjPage->remove();
		if(admin.m_pobjPage)
			m_pobjPage = dynamic_cast<itobjPage *>(admin.m_pobjPage->clone());

	removeAll();
		copy(admin.m_vecItems);
	}
	return *this;
}
itobjAdmin::~itobjAdmin()
{
	removeAll();
	if(m_pobjPage)
		m_pobjPage->remove();
}
#if defined(_WIN32)
/*****************************************************************************************
	<< --- itobjAdmin::draw	2013-12-13 --- >>
	˵�������ƻ�ͼģ��
	������
	pdc	= ��ͼ����
	����ֵ��
*****************************************************************************************/
void itobjAdmin::draw(CDC *pdc, CView* pView)
{
	if(!pdc)
		return;
	//mpobj->draw(pdc);
	// ��ͼ��Ԫ
	acm::CNPainterView *pview = dynamic_cast<acm::CNPainterView *>(pView);
	itobj *pobj = nullptr;
	for(itobj::PtrArrayType::size_type idx = 0; idx < m_vecItems.size(); ++idx)
	{
		pobj = m_vecItems[idx];
		pobj->draw(pdc);
		if(pview->m_bActive && !pdc->IsPrinting() && pview->isSelected(pobj))
			pobj->drawTracker(pdc, itobj::TSSelected,m_clrTracker);
	}
}
/*****************************************************************************************
	<< --- itobjAdmin::drawBackground	2013-12-02 --- >>
	˵�������ƹ�������ֽ��
	������
	pdc			= ��ͼ����
	szWorker	= ����̨�ߴ�
	����ֵ��
*****************************************************************************************/
void itobjAdmin::drawBackground(CDC *pdc, const CSize &szWorker)
{
	if(!pdc)
		return;
	size szpaper = m_pobjPage->getPaperInfo().bsSize;
	rect rtworker/*, rtpaper*/;
	if(m_pobjPage->getPaperInfo().isLandscape())	// Ĭ��������
		std::swap(szpaper.width, szpaper.height);
	//rtworker.left = -(szWorker.cx-szpaper.cx)/2;
	//rtworker.right = (szWorker.cx+szpaper.cx)/2;
	//rtworker.top = (szWorker.cy+szpaper.cy)/2;
	//rtworker.bottom = -(szWorker.cy-szpaper.cy)/2;

	rtworker.x = -(szWorker.cx-szpaper.width)/2;
	rtworker.y = -(szWorker.cy-szpaper.height)/2;
	rtworker.width = szWorker.cx;
	rtworker.height = szWorker.cy;

	if(!pdc->IsPrinting())
	{
		if(m_pobjPage->supportMargins())
		{	// ԭ��(0,0)Ҫ����
#if defined(WORK_AXIS_CAD)
			rtworker.offset(-m_pobjPage->getMarginLeft(), -m_pobjPage->getMarginBottom());
#else // defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			rtworker.offset(-m_pobjPage->getMarginLeft(), -m_pobjPage->getMarginTop());
#endif
		}
		if(m_pobjPage->supportHeaderFooter())
		{	// Y�᷽��ԭ��(0,0)��������
#if defined(WORK_AXIS_CAD)
			rtworker.offset(0, -m_pobjPage->getFooter().uiHeight);
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			rtworker.offset(0, -m_pobjPage->getHeader().uiHeight);
#endif
		}
		// ���ƹ���̨
		Gdiplus::Graphics grap(pdc->GetSafeHdc());
		rtworker.deflate(10,10);
		grap.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(m_pobjPage->getWorkerColor())), rtworker.toGdiplus());
	}

	// ����ֽ����Ϣ
	m_pobjPage->draw(pdc);

}
/*****************************************************************************************
	<< --- itobjAdmin::prepareDC	2013-12-13 --- >>
	˵������λ��ͼ����ϵ����ͼԭ��
	������
	pdc			= ��ͼ����
	szWorker	= ����̨�ߴ�
	����ֵ����λ����
*****************************************************************************************/
void itobjAdmin::prepareDC(CDC* pdc, const CSize &szWorker)
{
	////1.SetWindowOrg(x, y) �ǰ��豸�����ԭ��(�ӿ�)ӳ�䵽�߼������(X, Y)�� ���鿴�������⴦
	////2.SetViewportOrg(x, y) �ǰ��߼������ԭ��(����)ӳ�䵽�豸�����(X, Y)��
	if(!pdc)
		return;
	switch (m_beWorkaxis)
	{
	case BEWorkAxisNormal:
		{
		pdc->SetMapMode(MM_ISOTROPIC);

		long wpin = pdc->GetDeviceCaps(LOGPIXELSX);	// ��Ļˮƽ����ÿӢ�����ص���	
		long hpin = pdc->GetDeviceCaps(LOGPIXELSY);	// ��Ļ��ֱ����ÿӢ�����ص���	

		pdc->SetWindowExt(254, -254);		// ���ڵĳߴ磬�߼����ꣻĬ��ʱ���ң����µ������为ʱ�ͷ�������
		pdc->SetViewportExt(wpin, hpin);	// �����ĳߴ磬�豸���꣨���أ�
		pdc->SetWindowOrg(m_pobjPage->getWindowOrg(szWorker, pdc->IsPrinting()).toMSPoint());
		}
		break;
	case BEWorkAxisDOC:
		{
		pdc->SetMapMode(MM_ISOTROPIC);

		long wpin = pdc->GetDeviceCaps(LOGPIXELSX);	// ��Ļˮƽ����ÿӢ�����ص���	
		long hpin = pdc->GetDeviceCaps(LOGPIXELSY);	// ��Ļ��ֱ����ÿӢ�����ص���	

		pdc->SetWindowExt(254, 254);		// ���ڵĳߴ磬�߼����ꣻĬ��ʱ���ң����µ������为ʱ�ͷ�������
		pdc->SetViewportExt(wpin, hpin);	// �����ĳߴ磬�豸���꣨���أ�
		pdc->SetWindowOrg(m_pobjPage->getWindowOrg(szWorker, pdc->IsPrinting()).toMSPoint());
		}
		break;
	case BEWorkAxisCAD:
		pdc->SetMapMode(MM_TEXT);
		break;
	default:
		break;
	}
}

#endif


}