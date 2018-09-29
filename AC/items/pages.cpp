#include "pages.h"
#include "AC/drawdata.h"
#include "AC/items/itobjProperty.h"
namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPage	2013-12-02 --- >>
//	���ƣ���������ֽ�ţ�ֽ�ű�����ҳüҳ�ŵȵ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
_IMPLEMENT_ITOBJ_(itobjPage, itobj, BEDrawShapeTypePage, 0x1)
// itobjPage������Ĭ��ֵ
#define _DEFAULT_ITEMPAGE_WORKERCOLOR_	0xFFEAEAEA	// ����̨����ɫ
#define _DEFAULT_ITEMPAGE_SHADOWCOLOR_	0xFF808080	// ҳ����Ӱɫ
#define _DEFAULT_ITEMPAGE_GRIDCOLOR_	0xFFC0C0C0	// ҳ����Ӱɫ
itobjPage::itobjPage(bool isLandscape, bool margins, bool headerfooter, bool grid)
	: itobj()
	//, m_wsPrinter(L"")
	, m_bsPaper(isLandscape)
	, m_bMargins(margins)
	, m_uiMarginTop(254),m_uiMarginBottom(254),m_uiMarginLeft(317),m_uiMarginRight(317)
	, m_bHeaderFooter(headerfooter)
	, m_bsHeader(150), m_bsFooter(175)
	, m_clrWorker(_DEFAULT_ITEMPAGE_WORKERCOLOR_), m_clrPaperShadow(_DEFAULT_ITEMPAGE_SHADOWCOLOR_)
	, m_bGrid(grid), m_bsGrid(BEPenStyleDot, 1, _DEFAULT_ITEMPAGE_GRIDCOLOR_)
{
	refreshPagePosition();
}
itobjPage::itobjPage(const itobjPage &page)
	: itobj(page)
	//, m_wsPrinter(L"")
	, m_bsPaper(page.m_bsPaper)
	//, m_bsBIN(page.m_bsBIN)
	, m_bMargins(page.m_bMargins)
	, m_uiMarginTop(page.m_uiMarginTop),m_uiMarginBottom(page.m_uiMarginBottom),m_uiMarginLeft(page.m_uiMarginLeft),m_uiMarginRight(page.m_uiMarginRight)
	, m_bHeaderFooter(page.m_bHeaderFooter)
	, m_bsHeader(page.m_bsHeader), m_bsFooter(page.m_bsFooter)
	, m_clrWorker(page.m_clrWorker), m_clrPaperShadow(page.m_clrPaperShadow)
	, m_bGrid(page.m_bGrid), m_bsGrid(page.m_bsGrid)
{
}
itobjPage& itobjPage::operator=(const itobjPage &page)
{
	if(this != &page)
	{
		itobj::operator=(page);
		//m_wsPrinter = page.m_wsPrinter;
		m_bsPaper = page.m_bsPaper;
		//m_bsBIN = page.m_bsBIN;

		m_bMargins = page.m_bMargins;
		m_uiMarginTop = page.m_uiMarginTop;
		m_uiMarginBottom = page.m_uiMarginBottom;
		m_uiMarginLeft = page.m_uiMarginLeft;
		m_uiMarginRight = page.m_uiMarginRight;

		m_bHeaderFooter = page.m_bHeaderFooter;
		m_bsHeader = page.m_bsHeader;
		m_bsFooter = page.m_bsFooter;

		m_clrWorker = page.m_clrWorker;
		m_clrPaperShadow = page.m_clrPaperShadow;
		m_bGrid = page.m_bGrid;
		m_bsGrid = page.m_bsGrid;

	}
	return *this;
}
itobjPage::~itobjPage()
{
#ifdef DEBUG
	DEBUG_LOG(L"free %s", description().c_str());
#endif
}
// ����
inline std::wstring itobjPage::description(int indent) const
{
	std::wostringstream os;
	wchar_t buf[128] = {0};

	gtc::outClassBegin(os, L"itobjPage", indent);
	//gtc::outClassItem(os, L"��ӡ��", m_wsPrinter, indent << 1);
	gtc::outClassItem(os, L"ֽ��", m_bsPaper.description(indent << 1), indent << 1);
	//gtc::outClassItem(os, L"ֽ��", m_bsBIN.description(indent << 1), indent << 1);
	std::swprintf(buf, 128, L"%s (�ϣ�%d,�£�%d,��%d,�ң�%d)", m_bMargins?L"֧��":L"��֧��", 
		m_uiMarginTop, m_uiMarginBottom, m_uiMarginLeft, m_uiMarginRight);
	gtc::outClassItem(os, L"ҳ�߾�", buf, indent << 1);
	std::swprintf(buf, 128, L"%s", m_bHeaderFooter?L"֧��":L"��֧��");
	gtc::outClassItem(os, L"ҳüҳ��", buf, indent << 1);
	gtc::outBaseClass(os, *(itobj*)this, indent << 1);
	gtc::outClassEnd(os, L"itobjPage", indent);
	return os.str();
}
/*****************************************************************************************
	<< --- itobj::clone	2013-11-29 --- >>
	˵����ˢ����Ϣ���磺�ı�ҳ��ߴ�����Ҫ���ñ�����
	������
	����ֵ��
*****************************************************************************************/
void itobjPage::refresh()
{
	size szPaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())	// �ߴ��������ʾ�ġ�2=����
		std::swap(szPaper.width, szPaper.height);

	m_bsPosition.setRect(point(0,0), szPaper);
	//m_bsPosition.normalizeRect();
}

/*****************************************************************************************
	<< --- itobj::getWindowOrg	2013-12-04 --- >>
	˵�������豸�����ԭ��(�ӿ�)ӳ�䵽�߼������(X, Y)��
	������
	bsWorker	= ����̨�ߴ�
	isprinter	= �Ƿ��Ǵ�ӡ��
	����ֵ��
*****************************************************************************************/
point itobjPage::getWindowOrg(size bsWorker, bool isprinter)
{
	point ptorg(0,0);
	size szPaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())	// �ߴ��������ʾ�ġ�2=����
		std::swap(szPaper.width, szPaper.height);
	if(isprinter)
	{
		//rect rt(0,0,szPaper.width,szPaper.height);// = m_bsPosition;
		//ptorg.x = min(rt.left, rt.right);
		//ptorg.y = max(rt.top, rt.bottom);
		if(supportMargins())
		{	// ԭ��(0,0)Ҫ����
#if defined(WORK_AXIS_CAD)
			ptorg.x -= m_uiMarginLeft;
			ptorg.y -= m_uiMarginBottom;
#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			ptorg.x -= m_uiMarginLeft;
			ptorg.y -= m_uiMarginBottom;
#endif
		}
		if(supportHeaderFooter())
		{	// Y�᷽��ԭ��(0,0)��������
#if defined(WORK_AXIS_CAD)
			ptorg.y -= m_bsFooter.uiHeight;
#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			ptorg.y -= m_bsHeader.uiHeight;
#endif
		}
	}
	else
	{
#if defined(WORK_AXIS_CAD)	// ���ҡ��ϵ�����1�߼���λ����0.1����
		ptorg.x = -(bsWorker.width-szPaper.width)/2;
		ptorg.y = (bsWorker.height+szPaper.height)/2;
#elif defined(WORK_AXIS_DOC)	// ���ҡ��µ�����1�߼���λ����0.1����
		ptorg.x = -(bsWorker.width-szPaper.width)/2;
		ptorg.y = -(bsWorker.height-szPaper.height)/2;
#endif
		if(supportMargins())
		{	// ԭ��(0,0)Ҫ����
#if defined(WORK_AXIS_CAD)
			ptorg.x -= m_uiMarginLeft;
			ptorg.y -= m_uiMarginBottom;
#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			ptorg.x -= m_uiMarginLeft;
			ptorg.y -= m_uiMarginBottom;
#endif
		}
		if(supportHeaderFooter())
		{	// Y�᷽��ԭ��(0,0)��������
#if defined(WORK_AXIS_CAD)
			ptorg.y -= m_bsFooter.uiHeight;
#else	//defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			ptorg.y -= m_bsHeader.uiHeight;
#endif
		}
	}

	return ptorg;
}
// ֽ�����򣬼���m_bsPosition
inline void itobjPage::refreshPagePosition()
{
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	m_bsPosition.setRect(point(0,0), szpaper);
	//m_bsPosition.normalizeRect();
	if(m_bMargins)
	{
		szpaper.width -= m_uiMarginLeft + m_uiMarginRight;
		szpaper.height -= m_uiMarginTop + m_uiMarginBottom;
	}

	if(m_bHeaderFooter)
	{
		szpaper.height -= m_bsHeader.uiHeight + m_bsFooter.uiHeight;
	}
	rect area;
#if defined(WORK_AXIS_CAD)
	if(m_bMargins)
		m_bsPosition.offset(-m_uiMarginLeft, m_uiMarginBottom);
	if(m_bHeaderFooter)
		m_bsPosition.offset(0, m_bsFooter.uiHeight);
	//area.left = 0;
	//area.right = szpaper.cx;
	//area.top = szpaper.cy;
	//area.bottom = 0;
	
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	if(m_bMargins)
		m_bsPosition.offset(-m_uiMarginLeft, -m_uiMarginBottom);
	if(m_bHeaderFooter)
		m_bsPosition.offset(0, -m_bsFooter.uiHeight);
	//area.left = 0;
	//area.right = szpaper.cx;
	//area.top = 0;
	//area.bottom = szpaper.cy;
#endif
}


inline rect itobjPage::areaPaperWork() const
{
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	if(m_bMargins)
	{
		szpaper.width -= m_uiMarginLeft + m_uiMarginRight;
		szpaper.height -= m_uiMarginTop + m_uiMarginBottom;
	}

	if(m_bHeaderFooter)
	{
		szpaper.height -= m_bsHeader.uiHeight + m_bsFooter.uiHeight;
	}
	rect area;

	area.setRect(point(0,0), szpaper);

	return area;
}

inline rect itobjPage::areaMarginTop() const 
{
	rect area;
	if(!m_bMargins)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t header = m_bHeaderFooter?m_bsHeader.uiHeight:0;
	int32_t footer = m_bHeaderFooter?m_bsFooter.uiHeight:0;
#if defined(WORK_AXIS_CAD)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = szpaper.cy - m_uiMarginBottom - footer;
	//area.bottom = area.top - m_uiMarginTop;
	area.x =0;
	area.y = szpaper.height - m_uiMarginBottom - footer;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_uiMarginTop;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = -m_uiMarginTop;
	//area.bottom = 0;
	area.x = 0;
	area.y = -m_uiMarginTop;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_uiMarginTop;
#endif
	return area;
}
inline rect itobjPage::areaMarginBottom() const
{
	rect area;
	if(!m_bMargins)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t header = m_bHeaderFooter?m_bsHeader.uiHeight:0;
	int32_t footer = m_bHeaderFooter?m_bsFooter.uiHeight:0;
#if defined(WORK_AXIS_CAD)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = -footer;
	//area.bottom = area.top-m_uiMarginBottom;
	area.x = 0;
	area.y = -footer-m_uiMarginBottom;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_uiMarginBottom;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = szpaper.cy - m_uiMarginTop - m_uiMarginBottom;
	//area.bottom = szpaper.cy - m_uiMarginTop;
	area.x = 0;
	area.y = szpaper.height - m_uiMarginTop - m_uiMarginBottom;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_uiMarginBottom;
#endif
	return area;
}
rect itobjPage::areaMarginLeft() const
{
	rect area;
	if(!m_bMargins)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t header = m_bHeaderFooter?m_bsHeader.uiHeight:0;
	int32_t footer = m_bHeaderFooter?m_bsFooter.uiHeight:0;
#if defined(WORK_AXIS_CAD)
	//area.left = -m_uiMarginLeft;
	//area.right = 0;
	//area.top = szpaper.cy - m_uiMarginBottom - m_uiMarginTop - footer - header;
	//area.bottom = 0;
	area.x = -m_uiMarginRight;
	area.y = 0;
	area.width = m_uiMarginLeft;
	area.height = szpaper.height - m_uiMarginBottom - m_uiMarginTop - footer - header;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = -m_uiMarginLeft;
	//area.right = 0;
	//area.top = 0;
	//area.bottom = szpaper.cy - m_uiMarginTop - m_uiMarginBottom - footer - header;
	area.x = -m_uiMarginRight;
	area.y = 0;
	area.width = m_uiMarginLeft;
	area.height = szpaper.height - m_uiMarginBottom - m_uiMarginTop - footer - header;
#endif
	return area;
}

inline rect itobjPage::areaMarginRight() const
{
	rect area;
	if(!m_bMargins)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t header = m_bHeaderFooter?m_bsHeader.uiHeight:0;
	int32_t footer = m_bHeaderFooter?m_bsFooter.uiHeight:0;
#if defined(WORK_AXIS_CAD)
	//area.left = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.right = szpaper.cx - m_uiMarginLeft;
	//area.top = szpaper.cy - m_uiMarginBottom - m_uiMarginTop - footer - header;
	//area.bottom = 0;
	area.x = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.y = 0;
	area.width = m_uiMarginRight;
	area.height = szpaper.height - m_uiMarginBottom - m_uiMarginTop - footer - header;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.right = szpaper.cx - m_uiMarginLeft;
	//area.top = 0;
	//area.bottom = szpaper.cy - m_uiMarginTop - m_uiMarginBottom - footer - header;
	area.x = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.y = 0;
	area.width = m_uiMarginRight;
	area.height = szpaper.height - m_uiMarginBottom - m_uiMarginTop - footer - header;
#endif
	return area;
}
rect itobjPage::areaHeader() const
{
	rect area;
	if(!m_bHeaderFooter)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t margtop = m_bMargins?m_uiMarginTop:0;
	int32_t margbottom = m_bMargins?m_uiMarginBottom:0;
#if defined(WORK_AXIS_CAD)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = szpaper.cy - m_uiMarginBottom - m_bsFooter.uiHeight-margtop;
	//area.bottom = area.top - m_bsHeader.uiHeight;
	area.x = 0;
	area.y = szpaper.height - m_uiMarginBottom - m_bsFooter.uiHeight-margtop;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_bsHeader.uiHeight;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = -m_uiMarginTop;
	//area.bottom = 0;
	area.x = 0;
	area.y = -m_bsHeader.uiHeight;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_bsHeader.uiHeight;
#endif
	return area;
}

rect itobjPage::areaFooter() const
{
	rect area;
	if(!m_bHeaderFooter)
		return area;
	size szpaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())
		std::swap(szpaper.width, szpaper.height);
	int32_t margtop = m_bMargins?m_uiMarginTop:0;
	int32_t margbottom = m_bMargins?m_uiMarginBottom:0;
#if defined(WORK_AXIS_CAD)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = 0;
	//area.bottom = area.top-m_bsFooter.uiHeight;
	area.x = 0;
	area.y = -m_bsFooter.uiHeight;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_bsFooter.uiHeight;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	//area.left = 0;
	//area.right = szpaper.cx - m_uiMarginLeft - m_uiMarginRight;
	//area.top = szpaper.cy - m_uiMarginTop - m_uiMarginBottom;
	//area.bottom = szpaper.cy - m_uiMarginTop;
	area.x = 0;
	area.y = -m_bsFooter.uiHeight;
	area.width = szpaper.width - m_uiMarginLeft - m_uiMarginRight;
	area.height = m_bsFooter.uiHeight;
#endif
	return area;
}


#if defined(_WIN32)
void itobjPage::draw(CDC *pdc)
{
	if(!pdc)
		return;
	Gdiplus::Graphics grap(pdc->GetSafeHdc());
	// ����ֽ�š�ֽ�������ο��ߵ�
	size szPaper = m_bsPaper.bsSize;
	if(m_bsPaper.isLandscape())	// �ߴ��������ʾ�ġ�2=����
		std::swap(szPaper.width, szPaper.height);
	rect rtPaper(0,0,szPaper.width, szPaper.height);
	if(!pdc->IsPrinting())
	{
		if(supportMargins())
		{	// ԭ��(0,0)Ҫ����
#if defined(WORK_AXIS_CAD)
			rtPaper.offset(-m_uiMarginLeft, -m_uiMarginBottom);
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			rtPaper.offset(-m_uiMarginLeft, -m_uiMarginTop);
#endif
		}
		if(supportHeaderFooter())
		{	// Y�᷽��ԭ��(0,0)��������
#if defined(WORK_AXIS_CAD)
			rtPaper.offset(0, -m_bsFooter.uiHeight);
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
			rtPaper.offset(0, -m_bsHeader.uiHeight);
#endif
		}
		// ��ֽ����
		int32_t offy = 0;
#if defined(WORK_AXIS_CAD)
		offy = -10;
#else	// defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		offy = 10;
#endif
		rtPaper.offset(10, offy);
		grap.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(m_clrPaperShadow)), rtPaper.toGdiplus());
		// ��ֽ
		rtPaper.offset(-10, -offy);
		grap.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::White), rtPaper.toGdiplus());
		// ���ο���
		if(supportGrid())
		{
		}
		// ����ҳü��ҳ��
		if(supportHeaderFooter())
		{
		}
	}
	else
	{

	}

}
/*****************************************************************************************
	<< --- itobjPage::hitTest	2013-12-09 --- >>
	˵�������Ե��Ƿ��ڶ����λ����
	������
	point		= �����Ե㣨�߼����꣩
	pView		= ��ͼ����������ͼ
	bSelected	= trueʱ��λ�飬������Զ���
	����ֵ��
		= 0��δѡ��
		> 0����λ������
*****************************************************************************************/
int32_t itobjPage::hitTest(CPoint point, acm::CNPainterView *pView, bool bSelected)
{
	return itobj::hitTest(point, pView, bSelected);
}
/*****************************************************************************************
	<< --- itobjPage::drawTracker	2013-12-09 --- >>
	˵�������ƶ�λ����������
	������
	vpdc		= ��ͼ����
	vstate		= ��λ���״̬
	vbkcolor	= ������ɫ
	����ֵ��
*****************************************************************************************/
void itobjPage::drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor)
{
	itobj::drawTracker(pdc, vstate, vbkcolor);
}
/*****************************************************************************************
	<< --- itobjPage::allocPropertySectionArray	2013-12-06 --- >>
	˵���������������
	������
	����ֵ��
*****************************************************************************************/
std::size_t itobjPage::allocPropertySectionArray(itobj* pitem, bool isShare)
{
	itobjPropSection::PtrArrayType &vecSection = pitem->getPropertys();
	if(vecSection.size() > 0) pitem->freePropertys();
	// 
	itobjPropSection *psec = nullptr, *psubsec = nullptr, *psubsec1 = nullptr;
	entry::PtrArrayType vec;
	itobjPage *ppage = dynamic_cast<itobjPage *>(pitem);
	if(ppage && (psec = new itobjPropSection(L"ҳ����ʽ")) != nullptr) 
	{
		psec->addProperty(new itobjPropColor(L"����̨����ɫ", ppage->m_clrWorker, _DEFAULT_ITEMPAGE_WORKERCOLOR_, L"����̨����ɫ��", BEDrawShapeTypePage,BEPagePropWorkercolor)); 
		psec->addProperty(new itobjPropColor(L"ҳ����Ӱ", ppage->m_clrPaperShadow, _DEFAULT_ITEMPAGE_SHADOWCOLOR_, L"ҳ����Ӱ��ɫ������ͻ����ʾҳ�档", BEDrawShapeTypePage,BEPagePropPapershadow)); 
		// 
		psec->addProperty(new itobjPropBoolean(L"����", ppage->m_bGrid, L"�Ƿ���ʾ���񣬿���Ϊ��ͼ�Ĳο��ߣ����ڻ��Ʋ�����", BEDrawShapeTypePage, BEPagePropGrid));
		psubsec = new itobjPropSection(L"������ʽ", nullptr, true);
		psubsec->addProperty(new itobjPropColor(L"��ɫ", ppage->m_bsGrid.color, _DEFAULT_ITEMPAGE_GRIDCOLOR_, L"", BEDrawShapeTypePage, BEPagePropGridcolor));
		vec.assign(std::begin(gc_tbPenStyle), std::end(gc_tbPenStyle));
		psubsec->addProperty(new itobjPropCombo(L"��ʽ", ppage->m_bsGrid.style, vec, false, L"", BEDrawShapeTypePage, BEPagePropGridstyle));
		psec->addProperty(psubsec);
		//
		psubsec = new itobjPropSection(L"ֽ��", nullptr, true);
		psubsec->addProperty(new itobjPropEdit(L"�ߴ�", ppage->m_bsPaper.wsName.c_str(), false, L"�ô�С�����ڡ�ҳ�����á��Ի����и��ġ�", BEDrawShapeTypePage, BEPagePropPaperName));
		vec.assign(std::begin(gc_tbPaperOrientation), std::end(gc_tbPaperOrientation));
		psubsec->addProperty(new itobjPropCombo(L"����", ppage->m_bsPaper.iOrientation, vec, false, L"", BEDrawShapeTypePage, BEPagePropPaperOrientation));
		psec->addProperty(psubsec);
		// 
		int32_t maxw = ppage->m_bsPaper.iOrientation == 1?ppage->m_bsPaper.bsSize.width/2:ppage->m_bsPaper.bsSize.height/2;
		int32_t maxh = ppage->m_bsPaper.iOrientation == 1?ppage->m_bsPaper.bsSize.height/2:ppage->m_bsPaper.bsSize.width/2;
		psec->addProperty(new itobjPropBoolean(L"ҳ�߾�", ppage->m_bMargins, L"�Ƿ�����ҳ�߾࣬����ҳ�߾���ı��ͼ���(0,0)λ�á�", BEDrawShapeTypePage, BEPagePropMargins));
		psubsec = new itobjPropSection(L"ҳ�߾�ߴ�", nullptr, true);
		psubsec->addProperty(new itobjPropSpin(L"��߾�", ppage->m_uiMarginLeft, 0, maxw, L"",		BEDrawShapeTypePage,BEPagePropMarginleft));
		psubsec->addProperty(new itobjPropSpin(L"�ұ߾�", ppage->m_uiMarginRight, 0, maxw, L"",		BEDrawShapeTypePage,BEPagePropMarginright));
		psubsec->addProperty(new itobjPropSpin(L"�ϱ߾�", ppage->m_uiMarginTop, 0, maxh, L"",		BEDrawShapeTypePage,BEPagePropMargintop));
		psubsec->addProperty(new itobjPropSpin(L"�±߾�", ppage->m_uiMarginBottom, 0, maxh, L"",		BEDrawShapeTypePage,BEPagePropMarginbottom));
		psec->addProperty(psubsec);
		// 
		maxh = ppage->m_bsPaper.iOrientation == 1?ppage->m_bsPaper.bsSize.height/4:ppage->m_bsPaper.bsSize.width/4;
		psec->addProperty(new itobjPropBoolean(L"ҳüҳ��", ppage->m_bHeaderFooter, L"�Ƿ�����ҳüҳ�ţ����ú��ı��ͼ���(0,0)λ�á�", BEDrawShapeTypePage, BEPagePropHeaderfooter));
		psubsec = new itobjPropSection(L"ҳü", nullptr, true);
		psubsec->addProperty(new itobjPropSpin(L"�߶�", ppage->m_bsHeader.uiHeight, 0, maxh, L"",		BEDrawShapeTypePage, BEPagePropHeaderHeight));
		psubsec->addProperty(new itobjPropEdit(L"��������", ppage->m_bsHeader.wsTitle.c_str(), true, L"",	BEDrawShapeTypePage, BEPagePropHeaderTitle));
		vec.assign(std::begin(gc_tbAlignMode), std::end(gc_tbAlignMode));
		psubsec->addProperty(new itobjPropCombo(L"�������", ppage->m_bsHeader.beAlign, vec, false, L"",			BEDrawShapeTypePage, BEPagePropHeaderAlign));
		psubsec->addProperty(new itobjPropFont(L"��������", ppage->m_bsHeader.bsFont, L"",				BEDrawShapeTypePage, BEPagePropHeaderFont));
		vec.assign(std::begin(gc_tbPageNoStyle), std::end(gc_tbPageNoStyle));
		psubsec->addProperty(new itobjPropCombo(L"ҳ����ʽ", ppage->m_bsHeader.iPageStyle, vec, false, L"",		BEDrawShapeTypePage, BEPagePropHeaderPagestyle));
		vec.assign(std::begin(gc_tbAlignMode), std::end(gc_tbAlignMode));
		psubsec->addProperty(new itobjPropCombo(L"ҳ�����", ppage->m_bsHeader.bePageAlign, vec, false, L"",		BEDrawShapeTypePage, BEPagePropHeaderPagealign));
		psubsec->addProperty(new itobjPropFont(L"ҳ������", ppage->m_bsHeader.bsPageFont, L"",			BEDrawShapeTypePage, BEPagePropHeaderPagefont));
		psubsec->addProperty(new itobjPropFile(L"ͼ��", nullptr, L".jpeg", gc_wsImageFilter, true, L"",	BEDrawShapeTypePage, BEPagePropHeaderLogo));
		psubsec1 = new itobjPropSection(L"ͼ��λ��", nullptr, true);
		maxw = (ppage->m_bsPaper.iOrientation == 1?ppage->m_bsPaper.bsSize.width:ppage->m_bsPaper.bsSize.height)-ppage->m_uiMarginLeft-ppage->m_uiMarginRight;
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsHeader.bsLogo.x, 0, ppage->m_bsHeader.uiHeight, L"",	BEDrawShapeTypePage,BEPagePropHeaderLogoposX));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsHeader.bsLogo.y, 0, ppage->m_bsHeader.uiHeight, L"",	BEDrawShapeTypePage,BEPagePropHeaderLogoposY));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsHeader.bsLogo.width, 0, maxw, L"",						BEDrawShapeTypePage,BEPagePropHeaderLogoposWidth));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsHeader.bsLogo.height, 0, maxw, L"",					BEDrawShapeTypePage,BEPagePropHeaderLogoposHeight));
		psubsec->addProperty(psubsec1);
		psec->addProperty(psubsec);
		//////////////
		psubsec = new itobjPropSection(L"ҳ��", nullptr, true);
		psubsec->addProperty(new itobjPropSpin(L"�߶�", ppage->m_bsFooter.uiHeight, 0, maxh, L"",		BEDrawShapeTypePage, BEPagePropFooterHeight));
		psubsec->addProperty(new itobjPropEdit(L"��������", ppage->m_bsFooter.wsTitle.c_str(), true, L"",	BEDrawShapeTypePage, BEPagePropFooterTitle));
		vec.assign(std::begin(gc_tbAlignMode), std::end(gc_tbAlignMode));
		psubsec->addProperty(new itobjPropCombo(L"�������", ppage->m_bsFooter.beAlign, vec, false, L"",			BEDrawShapeTypePage, BEPagePropFooterAlign));
		psubsec->addProperty(new itobjPropFont(L"��������", ppage->m_bsFooter.bsFont, L"",				BEDrawShapeTypePage, BEPagePropFooterFont));
		vec.assign(std::begin(gc_tbPageNoStyle), std::end(gc_tbPageNoStyle));
		psubsec->addProperty(new itobjPropCombo(L"ҳ����ʽ", ppage->m_bsFooter.iPageStyle, vec, false, L"",		BEDrawShapeTypePage, BEPagePropFooterPagestyle));
		vec.assign(std::begin(gc_tbAlignMode), std::end(gc_tbAlignMode));
		psubsec->addProperty(new itobjPropCombo(L"ҳ�����", ppage->m_bsFooter.bePageAlign, vec, false, L"",		BEDrawShapeTypePage, BEPagePropFooterPagealign));
		psubsec->addProperty(new itobjPropFont(L"ҳ������", ppage->m_bsFooter.bsPageFont, L"",			BEDrawShapeTypePage, BEPagePropFooterPagefont));
		psubsec->addProperty(new itobjPropFile(L"ͼ��", nullptr, L".jpeg", gc_wsImageFilter, true, L"",	BEDrawShapeTypePage, BEPagePropFooterLogo));
		psubsec1 = new itobjPropSection(L"ͼ��λ��", nullptr, true);
		maxw = (ppage->m_bsPaper.iOrientation == 1?ppage->m_bsPaper.bsSize.width:ppage->m_bsPaper.bsSize.height)-ppage->m_uiMarginLeft-ppage->m_uiMarginRight;
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsFooter.bsLogo.x, 0, ppage->m_bsFooter.uiHeight, L"",	BEDrawShapeTypePage,BEPagePropFooterLogoposX));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsFooter.bsLogo.y, 0, ppage->m_bsFooter.uiHeight, L"",	BEDrawShapeTypePage,BEPagePropFooterLogoposY));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsFooter.bsLogo.width, 0, maxw, L"",						BEDrawShapeTypePage,BEPagePropFooterLogoposWidth));
		psubsec1->addProperty(new itobjPropSpin(L"��", ppage->m_bsFooter.bsLogo.height, 0, maxw, L"",					BEDrawShapeTypePage,BEPagePropFooterLogoposHeight));
		psubsec->addProperty(psubsec1);
		psec->addProperty(psubsec);

		// 
		vecSection.push_back(psec);
	}
	return vecSection.size();
}

/*****************************************************************************************
	<< --- itobjPage::updatePropertyValue	2013-12-06 --- >>
	˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
	������
	pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
	����ֵ��
*****************************************************************************************/
bool itobjPage::updatePropertyValue(itobjProp *pprop)
{
	if(pprop && __super::updatePropertyValue(pprop))
		return true;
	if(!pprop || pprop->getTypekey() != getType())
		return false;
	bool exists = true;


	return exists;
}
#endif




////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPageAxis	2014-01-03 --- >>
//	������ϵ��ҳ
////////////////////////////////////////////////////////////////////////////////////////////////////////
_IMPLEMENT_ITOBJ_(itobjPageAxis, itobjPage, BEDrawShapeTypePageAxis, 0x1)
// ���¾�Ϊ�߼���λ
int32_t itobjPageAxis::mc_iLineThick			= 6;		// ���ߴ�
int32_t itobjPageAxis::mc_iScaleLength			= 15;		// ��̶ȳ�
int32_t itobjPageAxis::mc_iScaleShortLength		= 10;		// ��̶ȳ�
int32_t itobjPageAxis::mc_iAxisArrow			= 6;		// �����Ჿ�֣�����������ļ�ͷ��С����ñ��λ������
int32_t itobjPageAxis::mc_iScaleInfoSpace		= 15;		// �̶�ֵ������̱�ļ��
int32_t itobjPageAxis::mc_iScaleInfoWidth		= 200;		// �̶�ֵ����Ŀ��
int32_t itobjPageAxis::mc_iScaleInfoHeight		= 100;		// �̶�ֵ����ĸ߶�
int32_t itobjPageAxis::mc_iAxisInfoSpace		= 80;		// ����Ϣ��̶�ֵ����ļ��
int32_t itobjPageAxis::mc_iAxisInfoWidth		= 600;		// ����Ϣ�Ŀ��
int32_t itobjPageAxis::mc_iAxisInfoHeight		= 60;		// ����Ϣ�ĸ߶�
int32_t itobjPageAxis::mc_iAxisNameWidth		= 200;		// �����ƵĿ��
int32_t itobjPageAxis::mc_iAxisNameHeight		= 100;		// �����Ƶĸ߶�


itobjPageAxis::itobjPageAxis()
	: itobjPage(true, true, true, false)
	, m_arrAxis(2, nullptr)/*��ά����x,y*/
	, m_arrInfo(6, nullptr)/*ֽ���Ϊ6����Ϣ����1��������*/
{
	// ����Ϣ
	// x�ᣬ���ӣ�ÿ�̶�=1����
	m_arrAxis[BEAxisFlagX] = new coordinateAxis(BEUnitTimeSecond, 0, 20);
	// y�ᣬ�ˣ�ÿ�̶�=5��
	m_arrAxis[BEAxisFlagY] = new coordinateAxis(BEUnitWeightGram, 0, 5);
	int32_t offy = 0, offx = 0;
	// ��Ϣ1��y������
	itobj* pobj = nullptr;
	//const rect rtwork = areaPaperWork();
	rect rt, rc = areaMarginLeft();
	rt = rc;
	rt.width = mc_iAxisNameWidth;
	rt.height = mc_iAxisNameHeight;
#if defined(WORK_AXIS_CAD)
	offy = rc.height - mc_iAxisNameHeight;
	offx = rc.width-mc_iScaleInfoSpace-mc_iAxisArrow-mc_iAxisNameWidth;
#else // defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	offy = mc_iAxisNameHeight;
	offx = rc.width-mc_iScaleInfoSpace-mc_iAxisArrow-mc_iAxisNameWidth;
#endif
	rt.offset(offx, offy);
	pobj = new itobj(rt);
	pobj->setAlignMode(BEAlignModeRightTop);
	pobj->setText(m_arrAxis[BEAxisFlagY]->getUnitName());
	pobj->getFont().setFontSize(BEFontSizeFiveSmall);
	pobj->setBorderStyle(BEBorderStyleNone);
	//pobj->setBackColor(0xFF0000ff);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoYName] = pobj;
	// ��Ϣ2��x������
	rt = rc = areaFooter();//areaMarginBottom();
	rt.width = mc_iAxisNameWidth;
	rt.height = mc_iAxisNameHeight;
#if defined(WORK_AXIS_CAD)
	offx = rc.width-mc_iAxisNameWidth;
	offy = rc.height -mc_iScaleInfoSpace-mc_iAxisArrow - mc_iAxisNameHeight;
#else // defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	offx = rc.width-mc_iAxisNameWidth;
	offy = mc_iScaleInfoSpace+mc_iAxisArrow;
#endif
	rt.offset(offx, offy);
	pobj = new itobj(rt);
	pobj->setAlignMode(BEAlignModeRightTop);
	pobj->setText(m_arrAxis[BEAxisFlagX]->getUnitName());
	pobj->getFont().setFontSize(BEFontSizeFiveSmall);
	pobj->setBorderStyle(BEBorderStyleNone);
	pobj->getPen().style = gtc::BEPenStyleDashDotDot;
	pobj->setBorderStyle(BEBorderStyleRectangle);
	//pobj->setBackColor(0xFF0000ff);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoXName] = pobj;
	// ��Ϣ3��y�����
	rt = rc = areaMarginLeft();
	rt.width = rc.width - mc_iAxisInfoSpace*2;
	rt.height = rc.height - mc_iAxisNameHeight*2;
#if defined(WORK_AXIS_CAD)
	offx = mc_iAxisInfoSpace;
	offy = mc_iAxisNameHeight;
#else // defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	offx = mc_iAxisInfoSpace;
	offy = mc_iAxisNameHeight;
#endif
	rt.offset(offx, offy);
	pobj = new itobj(rt);
	pobj->setText(L"");//���ز���ֵ
	pobj->setAlignMode(BEAlignModeCenterCenter);
	pobj->getFont().iEscapement = 900;
	pobj->setBorderStyle(BEBorderStyleNone);
	//pobj->setBackColor(0xFF00ff00);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoYTitle] = pobj;
	// ��Ϣ4��x�����
	rt = rc = areaFooter();// areaMarginBottom();
	rt.width = rc.width - mc_iScaleInfoWidth*2;
	rt.height = rc.height - mc_iAxisInfoSpace;
#if defined(WORK_AXIS_CAD)
	offx = mc_iScaleInfoWidth;
	offy = 0;
#else // defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
	offx = mc_iScaleInfoWidth;
	offy = 0;
#endif
	rt.offset(offx, offy);
	pobj = new itobj(rt);
	pobj->setAlignMode(BEAlignModeCenterCenter);
	pobj->setText(L"");//���ز���
	pobj->setBorderStyle(BEBorderStyleNone);
	//pobj->setBackColor(0xFF00ff00);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoXTitle] = pobj;
	// ��Ϣ5������ϵ����
	rt = areaHeader();// areaMarginTop();
	pobj = new itobj(rt);
	pobj->setAlignMode(BEAlignModeCenterCenter);
	pobj->setText(L"����");
	pobj->setBorderStyle(BEBorderStyleNone);
	//pobj->setBackColor(0xFFff0000);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoTitle] = pobj;
	// ��Ϣ6������ϵ�Ҳ����
	rt = areaMarginRight();
	rt.width -= mc_iAxisNameHeight;
	pobj = new itobj(rt);
	pobj->setAlignMode(BEAlignModeCenterTop);
	pobj->setText(L"");//ͼ��
	pobj->setBorderStyle(BEBorderStyleNone);
	pobj->getFont().iEscapement = 900;
	//pobj->setBackColor(0xFFff0000);
	//pobj->getBrush().style = BEBrushStyleSolid;
	m_arrInfo[BEAxisInfoRightTitle] = pobj;

	// �趨�������Ӧ�ķ�Χ
	rt = areaPaperWork();
	m_arrAxis[BEAxisFlagX]->setDevRange(rt.width);
	m_arrAxis[BEAxisFlagY]->setDevRange(rt.height);
}

itobjPageAxis::itobjPageAxis(const itobjPageAxis &page)
	: itobjPage(page)
{

}

itobjPageAxis& itobjPageAxis::operator=(const itobjPageAxis &page)
{
	if(this != &page)
	{
		itobjPage::operator=(page);
	}
	return *this;
}

itobjPageAxis::~itobjPageAxis()
{
	DEBUG_LOG(L"free %s", description().c_str());
}
void itobjPageAxis::logicToDoc(pointf &pt) {
	float devX = 0.0f, devY = 0.0f;
	logicToDoc(pt.x, pt.y, devX, devY);
	pt.x = devX;
	pt.y = devY;
}
void itobjPageAxis::logicToDoc(rectf &rc)
{
	pointf ptLT(rc.getLeft(), rc.getTop());
	pointf ptRB(rc.getRight(), rc.getBottom());
	logicToDoc(ptLT);
	logicToDoc(ptRB);
	rc.x = min(ptLT.x, ptRB.x);
	rc.y = min(ptLT.y, ptRB.y);
	rc.width = abs(ptLT.x - ptRB.x);
	rc.height = abs(ptLT.y - ptRB.y);
}
void itobjPageAxis::logicToDoc(const float &msvX, const float &msvY, float &devX, float &devY, bool reloadAxis /*= false*/) {
	if (reloadAxis)
	{
		rect rcWorker = areaPaperWork();
		m_arrAxis[BEAxisFlagX]->setDevRange(rcWorker.width);
		m_arrAxis[BEAxisFlagY]->setDevRange(rcWorker.height);
	}
	devX = m_arrAxis[BEAxisFlagX]->toClient(msvX);
	devY = m_arrAxis[BEAxisFlagY]->toClient(msvY);
	//static coordinateAxis *paxisX = nullptr;
	//static coordinateAxis *paxisY = nullptr;
	//static float devPreLogicX = 0.0f;		// 1����-->b�豸����ֵ
	//static float devPreLogicY = 0.0f;
	//static float originOffsetX = 0.0f;
	//static float originOffsetY = 0.0f;

	//if (reloadAxis || paxisX == nullptr || paxisY == nullptr)
	//{
	//	paxisX = getAxis(itobjPageAxis::BEAxisFlagX);
	//	paxisY = getAxis(itobjPageAxis::BEAxisFlagY);
	//	// 
	//	rect rcWorker = areaPaperWork();
	//	devPreLogicX = rcWorker.width/(paxisX->iPreScale*11);		// 1����-->b�豸����ֵ
	//	devPreLogicY = rcWorker.height/(paxisY->iPreScale*11);
	//	// 
	//	originOffsetX = paxisX->iOrigin*devPreLogicX;
	//	originOffsetY = paxisY->iOrigin*devPreLogicY;
	//}
	//// ת����������ֵ
	//float axisValueX = paxisX->scalingFactor() * msvX;		// ����-->x����
	//float axisValueY = paxisY->scalingFactor() * msvY;
	//// ������ֵ��ռ���豸����ֵ��
	//devX = axisValueX*devPreLogicX;	// x����-->y������
	//devY = axisValueY*devPreLogicY;
	//// ���ԭ���ƶ�
	//devX -= originOffsetX;
	//devY -= originOffsetY;
}
// ����
std::wstring itobjPageAxis::description(int indent) const
{
	std::wostringstream os;
	wchar_t buf[128] = {0};

	gtc::outClassBegin(os, L"itobjPageAxis", indent);
	////gtc::outClassItem(os, L"��ӡ��", m_wsPrinter, indent << 1);
	//gtc::outClassItem(os, L"ֽ��", m_bsPaper.description(indent << 1), indent << 1);
	////gtc::outClassItem(os, L"ֽ��", m_bsBIN.description(indent << 1), indent << 1);
	//std::swprintf(buf, 128, L"%s (�ϣ�%d,�£�%d,��%d,�ң�%d)", m_bMargins?L"֧��":L"��֧��", 
	//	m_uiMarginTop, m_uiMarginBottom, m_uiMarginLeft, m_uiMarginRight);
	//gtc::outClassItem(os, L"ҳ�߾�", buf, indent << 1);
	//std::swprintf(buf, 128, L"%s", m_bHeaderFooter?L"֧��":L"��֧��");
	//gtc::outClassItem(os, L"ҳüҳ��", buf, indent << 1);
	gtc::outBaseClass(os, *(itobjPage*)this, indent << 1);
	gtc::outClassEnd(os, L"itobjPageAxis", indent);
	return os.str();
}
#if defined(_WIN32)
void itobjPageAxis::draw(CDC *pdc)
{
	if(!pdc)
		return;
	itobjPage::draw(pdc);
	// ����������
	rect rtworker = areaPaperWork();
	if(!rtworker.isEmptyArea())
	{
		Gdiplus::Graphics grap(pdc->GetSafeHdc());
		grap.SetPageUnit(Gdiplus::UnitPixel);
		Gdiplus::Pen pen(0xff000000, mc_iLineThick);
		Gdiplus::GraphicsPath grapPath;
		grapPath.AddLine(0,0, mc_iAxisArrow/3, -mc_iAxisArrow);
		grapPath.AddLine(0,0, -mc_iAxisArrow/3, -mc_iAxisArrow);
		Gdiplus::CustomLineCap cap(nullptr, &grapPath);
		pen.SetCustomEndCap(&cap);
		// x,y��ķָ������磺11
		const int32_t iScalesX = getAxis(BEAxisFlagX)->getScales();
		const int32_t iScalesY = getAxis(BEAxisFlagY)->getScales();
		// Y��
		grap.DrawLine(&pen, rtworker.x, rtworker.y, rtworker.x, rtworker.y+rtworker.height);
		// X��
		grap.DrawLine(&pen, rtworker.x, rtworker.y, rtworker.x+rtworker.width, rtworker.y);
		// Y��̶�
		Gdiplus::Pen penScale(0xff000000, mc_iLineThick);
		const int32_t infoLen = 10;
		wchar_t info[infoLen] = {0};
		rect rtinfo;
		const int32_t scaleY = rtworker.height/iScalesY;
		int32_t scale = scaleY;
		//LOGFONT lgfont = {0};
		//m_arrInfo[BEAxisInfoYName]->getFont().toMSFont(&lgfont);
		//Gdiplus::Font fonty(pdc->GetSafeHdc(), &lgfont);
		//m_arrInfo[BEAxisInfoXName]->getFont().toMSFont(&lgfont);
		//Gdiplus::Font fontx(pdc->GetSafeHdc(), &lgfont);
		// Yԭ���ֵ
		//rtinfo.setRect(-mc_iScaleInfoWidth, mc_iScaleInfoHeight/2, 0, -mc_iScaleInfoHeight/2);
		rtinfo.setRect(-mc_iScaleInfoWidth, -mc_iScaleInfoHeight/2, mc_iScaleInfoWidth, mc_iScaleInfoHeight);
		rtinfo.offset(-mc_iScaleInfoSpace, 0);
		std::swprintf(info, infoLen, L"%d", m_arrAxis[BEAxisFlagY]->getOrigin());
	    drawer::drawString(grap, info, rtinfo, m_arrInfo[BEAxisInfoYName]->getFont(), BEAlignModeAllRight);
		for(int32_t idx = 0; idx < iScalesY-1; ++idx)
		{
			// �̱�
			grap.DrawLine(&penScale, 0, scale, mc_iScaleLength, scale);
			// ��ֵ
			rtinfo.offset(0, scaleY);
			std::swprintf(info, infoLen, L"%d", m_arrAxis[BEAxisFlagY]->getOrigin()+m_arrAxis[BEAxisFlagY]->getPreScale()*(idx+1));
			drawer::drawString(grap, info, rtinfo, m_arrInfo[BEAxisInfoYName]->getFont(), BEAlignModeAllRight);
			// 
			scale += scaleY;
		}
		// X��̶�
		const int32_t scaleX = rtworker.width/iScalesX;
		scale = scaleX;
		// Xԭ���ֵ
		//rtinfo.setRect(-mc_iScaleInfoWidth/2, 0, mc_iScaleInfoWidth/2, -mc_iScaleInfoHeight);
		rtinfo.setRect(-mc_iScaleInfoWidth/2, -mc_iScaleInfoHeight, mc_iScaleInfoWidth, mc_iScaleInfoHeight);
		rtinfo.offset(0, -mc_iScaleInfoSpace);
		std::swprintf(info, infoLen, L"%d", m_arrAxis[BEAxisFlagX]->getOrigin());
		drawer::drawString(grap, info, rtinfo, m_arrInfo[BEAxisInfoXName]->getFont(), BEAlignModeCenterTop);
		for(int32_t idx = 0; idx < iScalesX-1; ++idx)
		{
			// �̱�
			grap.DrawLine(&penScale, scale, 0, scale, mc_iScaleLength);
			// ��ֵ
			rtinfo.offset(scaleX, 0);
			std::swprintf(info, infoLen, L"%d", m_arrAxis[BEAxisFlagX]->getOrigin()+m_arrAxis[BEAxisFlagX]->getPreScale()*(idx+1));
			drawer::drawString(grap, info, rtinfo, m_arrInfo[BEAxisInfoXName]->getFont(), BEAlignModeCenterTop);
			// 
			scale += scaleX;
		}

	}
	// ����������Ϣ
	for(itobj::PtrArrayType::iterator iter = m_arrInfo.begin(), end = m_arrInfo.end(); iter != end; ++iter)
		(*iter)->draw(pdc);
}

/*****************************************************************************************
	<< --- static itobjPageAxis::allocPropertySectionArray	2013-12-06 --- >>
	˵���������������
	������
	����ֵ��
*****************************************************************************************/
std::size_t itobjPageAxis::allocPropertySectionArray(itobj* pitem, bool isShare)
{
	itobjPropSection::PtrArrayType &vecSection = pitem->getPropertys();
	if(vecSection.size() > 0) pitem->freePropertys();
	//vecSection = itobj::allocPropertySectionArray(pitem, isShare);
	itobjPageAxis *paxis = dynamic_cast<itobjPageAxis *>(pitem);
	itobjPropSection *psec = nullptr, *psubsec = nullptr;
	entry::PtrArrayType vec;
	if(paxis && (psec = new itobjPropSection(L"ҳ����Ϣ")) != nullptr)
	{
		// 
		psubsec = new itobjPropSection(L"����", nullptr, true);
		psubsec->addProperty(new itobjPropEdit(L"����", paxis->getAxisInfo(BEAxisInfoTitle)->getText().c_str(), true, L"�������ݡ�", BEDrawShapeTypePageAxis, BEAxisPropTitleName));
		psubsec->addProperty(new itobjPropFont(L"����", paxis->getAxisInfo(BEAxisInfoTitle)->getFont(), L"�����������塣", BEDrawShapeTypePageAxis, BEAxisPropTitleFont));
		psubsec->addProperty(new itobjPropColor(L"��ɫ", paxis->getAxisInfo(BEAxisInfoTitle)->getTextColor(), _DEFAULT_ITEM_TEXTCOLOR_, L"������ɫ��", BEDrawShapeTypePageAxis, BEAxisPropTitleColor)); 
		psec->addProperty(psubsec);
		// 
		psubsec = new itobjPropSection(L"������", nullptr, true);
		psubsec->addProperty(new itobjPropEdit(L"����", paxis->getAxisInfo(BEAxisInfoRightTitle)->getText().c_str(), true, L"���������ݡ�", BEDrawShapeTypePageAxis, BEAxisPropRightTitleName));
		psubsec->addProperty(new itobjPropFont(L"����", paxis->getAxisInfo(BEAxisInfoRightTitle)->getFont(), L"�������������塣", BEDrawShapeTypePageAxis, BEAxisPropRightTitleFont));
		psubsec->addProperty(new itobjPropColor(L"��ɫ", paxis->getAxisInfo(BEAxisInfoRightTitle)->getTextColor(), _DEFAULT_ITEM_TEXTCOLOR_, L"��������ɫ��", BEDrawShapeTypePageAxis, BEAxisPropRightTitleColor)); 
		psec->addProperty(psubsec);
		// �����������ɫ����_DEFAULT_ITEM_AXISCOLOR_
		psubsec = new itobjPropSection(L"X��", nullptr, true);
		vec.assign(std::begin(gc_tbUnitTime), std::end(gc_tbUnitTime));
		psubsec->addProperty(new itobjPropCombo(L"��λ", paxis->getAxis(BEAxisFlagX)->getUnitValue(), vec, false, L"", BEDrawShapeTypePageAxis, BEAxisPropAxisXUnit));
		psubsec->addProperty(new itobjPropSpin(L"ԭ��ֵ", paxis->getAxis(BEAxisFlagX)->getOrigin(), INT32_MIN, INT32_MAX, L"������ԭ�㡣",	BEDrawShapeTypePageAxis, BEAxisPropAxisXOrigin));
		psubsec->addProperty(new itobjPropSpin(L"��λ�̶�ֵ", paxis->getAxis(BEAxisFlagX)->getPreScale(), INT32_MIN, INT32_MAX, L"������ÿ�̶ȶ�Ӧ���߼�ֵ��",	BEDrawShapeTypePageAxis, BEAxisPropAxisXPrescale));
		psubsec->addProperty(new itobjPropEdit(L"����", paxis->getAxisInfo(BEAxisInfoXTitle)->getText().c_str(), true, L"���������ݡ�", BEDrawShapeTypePageAxis, BEAxisPropAxisXTitle));
		psubsec->addProperty(new itobjPropFont(L"����", paxis->getAxisInfo(BEAxisInfoXTitle)->getFont(), L"�������������塣", BEDrawShapeTypePageAxis, BEAxisPropAxisXFont));
		psubsec->addProperty(new itobjPropColor(L"��ɫ", paxis->getAxisInfo(BEAxisInfoXTitle)->getTextColor(), _DEFAULT_ITEM_TEXTCOLOR_, L"��������ɫ��", BEDrawShapeTypePageAxis, BEAxisPropAxisXColor)); 
		psec->addProperty(psubsec);
		// �����������ɫ����_DEFAULT_ITEM_AXISCOLOR_
		psubsec = new itobjPropSection(L"Y��", nullptr, true);
		vec.assign(std::begin(gc_tbUnitWeight), std::end(gc_tbUnitWeight));
		psubsec->addProperty(new itobjPropCombo(L"��λ", paxis->getAxis(BEAxisFlagY)->getUnitValue(), vec, false, L"", BEDrawShapeTypePageAxis, BEAxisPropAxisYUnit));
		psubsec->addProperty(new itobjPropSpin(L"ԭ��ֵ", paxis->getAxis(BEAxisFlagY)->getOrigin(), INT32_MIN, INT32_MAX, L"������ԭ�㡣",	BEDrawShapeTypePageAxis, BEAxisPropAxisYOrigin));
		psubsec->addProperty(new itobjPropSpin(L"��λ�̶�ֵ", paxis->getAxis(BEAxisFlagY)->getPreScale(), INT32_MIN, INT32_MAX, L"������ÿ�̶ȶ�Ӧ���߼�ֵ��",	BEDrawShapeTypePageAxis, BEAxisPropAxisYPrescale));
		psubsec->addProperty(new itobjPropEdit(L"����", paxis->getAxisInfo(BEAxisInfoYTitle)->getText().c_str(), true, L"���������ݡ�", BEDrawShapeTypePageAxis, BEAxisPropAxisYTitle));
		psubsec->addProperty(new itobjPropFont(L"����", paxis->getAxisInfo(BEAxisInfoYTitle)->getFont(), L"�������������塣", BEDrawShapeTypePageAxis, BEAxisPropAxisYFont));
		psubsec->addProperty(new itobjPropColor(L"��ɫ", paxis->getAxisInfo(BEAxisInfoYTitle)->getTextColor(), _DEFAULT_ITEM_TEXTCOLOR_, L"��������ɫ��", BEDrawShapeTypePageAxis, BEAxisPropAxisYColor)); 
		psec->addProperty(psubsec);
		// 
		vecSection.push_back(psec);
	}

	// 
	return vecSection.size();
}

/*****************************************************************************************
	<< --- itobjPageAxis::updatePropertyValue	2013-12-06 --- >>
	˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
	������
	pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
	����ֵ��
*****************************************************************************************/
bool itobjPageAxis::updatePropertyValue(itobjProp *pprop)
{
	if(pprop && itobj::updatePropertyValue(pprop))
		return true;
	if(!pprop || pprop->getTypekey() != BEDrawShapeTypePageAxis)
		return false;
	bool exists = true;
	COleVariant var = pprop->getValue();
	switch (BEAxisProp(pprop->getItemkey()))
	{
	case gtc::itobjPageAxis::BEAxisPropTitle:
		break;
	case gtc::itobjPageAxis::BEAxisPropTitleName:
		var.ChangeType(VT_BSTR);
		m_arrInfo[BEAxisInfoTitle]->setText(_bstr_t(var.bstrVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropTitleFont:
		m_arrInfo[BEAxisInfoTitle]->getFont().fromData(convert::toData(var));
		break;
	case gtc::itobjPageAxis::BEAxisPropTitleColor:
		var.ChangeType(VT_UI4);
		m_arrInfo[BEAxisInfoTitle]->setTextColor(RGB_FLIP(var.ulVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropRightTitle:
		break;
	case gtc::itobjPageAxis::BEAxisPropRightTitleName:
		var.ChangeType(VT_BSTR);
		m_arrInfo[BEAxisInfoRightTitle]->setText(_bstr_t(var.bstrVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropRightTitleFont:
		m_arrInfo[BEAxisInfoRightTitle]->getFont().fromData(convert::toData(var));
		break;
	case gtc::itobjPageAxis::BEAxisPropRightTitleColor:
		var.ChangeType(VT_UI4);
		m_arrInfo[BEAxisInfoRightTitle]->setTextColor(RGB_FLIP(var.ulVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisX:
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXUnit:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagX]->setUnitValue(var.lVal);
		m_arrInfo[BEAxisInfoXName]->setText(m_arrAxis[BEAxisFlagX]->getUnitName());
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXOrigin:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagX]->setOrigin(var.lVal);
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXPrescale:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagX]->setPreScale(var.lVal);
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXTitle:
		var.ChangeType(VT_BSTR);
		m_arrInfo[BEAxisInfoXTitle]->setText(_bstr_t(var.bstrVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXFont:
		m_arrInfo[BEAxisInfoXTitle]->getFont().fromData(convert::toData(var));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisXColor:
		var.ChangeType(VT_UI4);
		m_arrInfo[BEAxisInfoXTitle]->setTextColor(RGB_FLIP(var.ulVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisY:
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYUnit:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagY]->setUnitValue(var.lVal);
		m_arrInfo[BEAxisInfoYName]->setText(m_arrAxis[BEAxisFlagY]->getUnitName());
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYOrigin:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagY]->setOrigin(var.lVal);
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYPrescale:
		var.ChangeType(VT_I4);
		m_arrAxis[BEAxisFlagY]->setPreScale(var.lVal);
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYTitle:
		var.ChangeType(VT_BSTR);
		m_arrInfo[BEAxisInfoYTitle]->setText(_bstr_t(var.bstrVal));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYFont:
		m_arrInfo[BEAxisInfoYTitle]->getFont().fromData(convert::toData(var));
		break;
	case gtc::itobjPageAxis::BEAxisPropAxisYColor:
		var.ChangeType(VT_UI4);
		m_arrInfo[BEAxisInfoYTitle]->setTextColor(RGB_FLIP(var.ulVal));
		break;
	default:
		exists = false;
		break;
	}
	return exists;
}


#endif
}