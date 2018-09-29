#include "items.h"
#include "AC/drawdata.h"
#if defined(_AFXDLL)
	#include <afxwin.h>
	#include "AC/MFC/NTool.h"
#endif
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobj	2013-11-24 --- >>
//	��ͼ��Ԫͨ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
_IMPLEMENT_ROOT_ITOBJ_(itobj, BEDrawShapeTypeItem, 0x1)

int32_t		itobj::mc_iTrackerSize = 9;		// ��λ��Ŀ�͸�7����λ�����أ�
itobj::itobj(void)
	: m_wsText(L"")
	, /*m_clrBackColor(_DEFAULT_ITEM_BACKCOLOR_), */m_clrTextColor(_DEFAULT_ITEM_TEXTCOLOR_)
	, m_bAutoLine(false)
	//, m_iFontSize(105)
	, m_bVisible(true)
	, m_beAlign(BEAlignModeAllCenter)
	, m_beBorderStyle(BEBorderStyleRectangle)
	, m_bsPen(BEPenStyleSolid, 1, _DEFAULT_ITEM_FORECOLOR_)
	, m_bsBrush(BEBrushStyleNull, _DEFAULT_ITEM_FORECOLOR_, _DEFAULT_ITEM_BACKCOLOR_, BEHatchStyleHorizontal)
	, m_uiGroupFlag(0)
	//, m_uiZoom(100)
	, m_bsPosition(0,0,0,0)
	//, m_bBorder(false)
{
}
itobj::itobj(rect position)
	: m_wsText(L"")
	, /*m_clrBackColor(_DEFAULT_ITEM_BACKCOLOR_), */m_clrTextColor(_DEFAULT_ITEM_TEXTCOLOR_)
	, m_bAutoLine(false)
	//, m_iFontSize(105)
	, m_bVisible(true)
	, m_beAlign(BEAlignModeAllCenter)
	, m_beBorderStyle(BEBorderStyleRectangle)
	, m_bsPen(BEPenStyleSolid, 1, _DEFAULT_ITEM_FORECOLOR_)
	, m_bsBrush(BEBrushStyleNull, _DEFAULT_ITEM_FORECOLOR_, _DEFAULT_ITEM_BACKCOLOR_, BEHatchStyleHorizontal)
	, m_uiGroupFlag(0)
	//, m_uiZoom(100)
	, m_bsPosition(position)
	//, m_bBorder(false)
{
	//m_bsPosition.normalizeRect();
}
itobj::itobj(const itobj &item)
	: m_wsText(item.m_wsText)
	, /*m_clrBackColor(item.m_clrBackColor), */m_clrTextColor(item.m_clrTextColor)
	, m_bAutoLine(item.m_bAutoLine)
	//, m_iFontSize(item.m_iFontSize)
	, m_bVisible(item.m_bVisible)
	, m_beAlign(item.m_beAlign)
	, m_beBorderStyle(item.m_beBorderStyle)
	, m_bsPen(item.m_bsPen)
	, m_bsBrush(item.m_bsBrush)
	, m_bsFont(item.m_bsFont)
	, m_bsPosition(item.m_bsPosition)
	, m_uiGroupFlag(item.m_uiGroupFlag)
	//, m_uiZoom(item.m_uiZoom)
	//, m_bBorder(item.m_bBorder)
{
}

itobj& itobj::operator=(const itobj &item)
{
	if(this != &item)
	{
		m_wsText = item.m_wsText;
		/*m_clrBackColor = item.m_clrBackColor;*/
		m_clrTextColor = item.m_clrTextColor;
		m_bAutoLine = item.m_bAutoLine;
	    //m_iFontSize = item.m_iFontSize;
	    m_bVisible = item.m_bVisible;
	    m_beAlign = item.m_beAlign;
		m_beBorderStyle = item.m_beBorderStyle;
	    m_bsPen = item.m_bsPen;
	    m_bsBrush = item.m_bsBrush;
	    m_bsFont = item.m_bsFont;
	    m_bsPosition = item.m_bsPosition;
	    m_uiGroupFlag = item.m_uiGroupFlag;
	    //m_uiZoom = item.m_uiZoom;
		//m_bBorder = item.m_bBorder;
	}
	return *this;
}

itobj::~itobj(void)
{
#ifdef DEBUG
	DEBUG_LOG(L"free %s", description().c_str());
#endif
	freePropertys();
}

// ����
std::wstring itobj::description(int indent) const
{
	std::wostringstream os;
	gtc::outClassBegin(os, L"itobj", indent);
	gtc::outClassItem(os, L"����", m_wsText, indent << 1);
	//gtc::outClassItem(os, L"�ֺ�", m_iFontSize, indent << 1);
	gtc::outClassItem(os, L"�ɼ�", m_bVisible, indent << 1);
	gtc::outClassItem(os, L"����", m_uiGroupFlag, indent << 1);
	//gtc::outClassItem(os, L"�߿�", m_bBorder, indent << 1);
	gtc::outClassEnd(os, L"itobj", indent);
	return os.str();
}

/*****************************************************************************************
	<< --- itobj::getHandleCount	2013-11-29 --- >>
	˵����ѡ��ʱ��ʾ�Ķ�λ��������ͨ����λ���������λ�á���С...
	������
	����ֵ����λ����
*****************************************************************************************/
int32_t itobj::getHandleCount()
{
	return 8;
}

/*****************************************************************************************
	<< --- itobj::getHandle	2013-11-29 --- >>
	˵�����õ�ָ����λ������ĵ㣨�߼���λ��
	������
	vhandle	= ��λ������
				1-----2-----3
				|			|
				8			4
				|			|
				7-----6-----5
	����ֵ����λ������ĵ�
*****************************************************************************************/
point itobj::getHandle(int32_t vhandle)
{
	rect rt = m_bsPosition;
	//rect.normalizeRect();
	point center = rt.center();
	switch (vhandle)
	{
	case 1:		// ���Ͻ�
		return rt.topLeft();
		break;
	case 2:		// �ϱ���
		return point(center.x, rt.getTop());
		break;
	case 3:		// ���Ͻ�
		return point(rt.getRight(), rt.getTop());
		break;
	case 4:		// �ұ���
		return point(rt.getRight(), center.x);
		break;
	case 5:		// ���½�
		return rt.bottomRight();
		break;
	case 6:		// �±���
		return point(center.x, rt.getBottom());
		break;
	case 7:		// ���½�
		return point(rt.getLeft(), rt.getBottom());
		break;
	case 8:		// �����
		return point(rt.getLeft(), center.y);
		break;
	default:
		break;
	}
	return point();

}
//////////////////////////////////////////////////////////////////////////////////////////////
// WIN32 protected
//#if defined(_WIN32)
//// 
///*****************************************************************************************
//	<< --- itobj::drawBorder	2013-12-23 --- >>
//	˵�������ݱ߽���ʽ�ͻ��ʻ��Ʊ߿�
//	������
//	pdc	= ��ͼ����
//	����ֵ����λ�������߼����꣩
//*****************************************************************************************/
//void itobj::drawBorder(CDC *pdc)
//{
//	if(!m_bVisible || !pdc)
//		return;
//	CPen *oldpen, pen;
//	if(!pen.CreatePenIndirect(&m_bsPen.toMSPen()))
//		return;
//	oldpen = pdc->SelectObject(&pen);
//	rect rect = m_bsPosition;
//	rect.normalizeRect();
//	if(m_beBorderStyle & BEBorderStyleTop)
//	{
//		pdc->MoveTo(rect.topLeft().toMSPoint());
//		pdc->LineTo(rect.right, rect.top);
//	}
//	if(m_beBorderStyle & BEBorderStyleBottom)
//	{
//		pdc->MoveTo(rect.left, rect.bottom);
//		pdc->LineTo(rect.bottomRight().toMSPoint());
//	}
//	if(m_beBorderStyle & BEBorderStyleLeft)
//	{
//		pdc->MoveTo(rect.topLeft().toMSPoint());
//		pdc->LineTo(rect.left, rect.bottom);
//	}
//	if(m_beBorderStyle & BEBorderStyleRight)
//	{
//		pdc->MoveTo(rect.right, rect.top);
//		pdc->LineTo(rect.bottomRight().toMSPoint());
//	}
//
//	// 
//	if(oldpen)
//		pdc->SelectObject(oldpen);
//}
//
//#endif

#if defined(_WIN32)
void itobj::draw(CDC *pdc)
{
	if(!m_bVisible || !pdc)
		return;
	Gdiplus::Graphics grap(pdc->GetSafeHdc());
	grap.SetPageUnit(Gdiplus::UnitPixel);
	// ��䱳��
	drawer::fillRectangle(grap, m_bsBrush, m_bsPosition);
	// ���Ʊ߽�
	//grap.drawBorder(m_bsPen, m_bsPosition, m_beBorderStyle);
	/*if(m_beBorderStyle & BEBorderStyleTop)
	{
	grap.DrawLine(ppen, rect.left, rect.top, rect.right, rect.top);
	}
	if(m_beBorderStyle & BEBorderStyleBottom)
	{
	grap.DrawLine(ppen, rect.left, rect.bottom, rect.right, rect.bottom);
	}
	if(m_beBorderStyle & BEBorderStyleLeft)
	{
	grap.DrawLine(ppen, rect.left, rect.top, rect.left, rect.bottom);
	}
	if(m_beBorderStyle & BEBorderStyleRight)
	{
	grap.DrawLine(ppen, rect.right, rect.top, rect.right, rect.bottom);
	}*/
	// ��ʾ���֣�����ֻ���������1��ˮƽ�������ҡ�2����ֱ�������£�
	//LOGFONT lgfont = {0};
	//m_bsFont.toMSFont(&lgfont);
	//Gdiplus::Font font(pdc->GetSafeHdc(), &lgfont);
	//drawString(grap, m_wsText.c_str(), font, m_beAlign, nullptr, m_bsFont.iEscapement != 0);
	drawer::drawString(grap, m_wsText.c_str(), m_bsPosition, m_bsFont, m_beAlign, getTextColor());
	//drawer::drawString(grap, m_wsText.c_str(), font, m_beAlign, getPosition(), getTextColor());
}
/*****************************************************************************************
	<< --- itobj::getHandleCursor	2013-12-11 --- >>
	˵�����õ�ָ����λ����
	������
	nHandle		= ��λ������
	����ֵ����λ����
*****************************************************************************************/
HCURSOR itobj::getHandleCursor(int nHandle)
{
	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

/*****************************************************************************************
	<< --- itobj::drawTracker	2013-12-09 --- >>
	˵�������Ե��Ƿ��ڶ����λ����
	������
	point		= �����Ե㣨�߼����꣩
	pView		= ��ͼ����������ͼ
	bSelected	= trueʱ��λ�飬������Զ���
	����ֵ��
		= 0��δѡ��
		> 0����λ������
*****************************************************************************************/
int32_t itobj::hitTest(CPoint point, acm::CNPainterView *pView, bool bSelected)
{
	//if(!pView)
	//	return 0;
	//point pt(point.x, point.y);
	//rect rc;
	//if (bSelected)
	//{
	//	int nHandleCount = getHandleCount();
	//	for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
	//	{
	//		// GetHandleRect returns in logical coords
	//		rc = getHandleRect(nHandle,pView);
	//		rc.normalizeRect();
	//		if(rc.ptInRect(pt))
	//			return nHandle;
	//	}
	//}
	//else
	//{
	//	rc = m_bsPosition;
	//	rc.normalizeRect();
	//	if(rc.ptInRect(pt))
	//		return 1;
	//}
	return 0;
}

// ����ѡ�б��
void itobj::drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor)
{
	//if(!pdc || !m_bVisible)
	//	return;
	//switch (vstate)
	//{
	//case gtc::itobj::TSNormal:
	//	break;
	//case gtc::itobj::TSSelected:
	//case gtc::itobj::TSActive:
	//	{
	//		int nHandleCount = getHandleCount();
	//		CPen pen(PS_SOLID, 1, RGB(0,0,0)), *poldpen = nullptr;
	//		CBrush brush(vbkcolor), *poldbrush = nullptr;
	//		CBitmap bitmap, *poldbmp = nullptr;
	//		CDC dc;
	//		
	//		if(dc.CreateCompatibleDC(pdc))
	//		{
	//			if(bitmap.CreateCompatibleBitmap(pdc, mc_iTrackerSize, mc_iTrackerSize))
	//			{
	//				poldbmp = dc.SelectObject(&bitmap);
	//				poldpen = dc.SelectObject(&pen);
	//				poldbrush = dc.SelectObject(&brush);
	//				dc.Rectangle(CRect(0,0,mc_iTrackerSize, mc_iTrackerSize));
	//			}
	//		}
	//		for(int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
	//		{
	//			CRect rect = getHandleRect(nHandle, pdc);
	//			pdc->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0, 0, mc_iTrackerSize,mc_iTrackerSize,SRCCOPY);
	//		}
	//		// release
	//		if(poldpen)
	//			dc.SelectObject(poldpen);
	//		if(poldbrush)
	//			dc.SelectObject(poldbrush);
	//		if(poldbmp)
	//			dc.SelectObject(poldbmp);
	//		pen.DeleteObject();
	//		brush.DeleteObject();
	//		bitmap.DeleteObject();
	//	}
	//	break;
	//default:
	//	break;
	//}
}
/*****************************************************************************************
	<< --- itobj::moveHandleTo	2013-11-30 --- >>
	˵����ͨ��ָ����λ��ı����ߴ�
	������
	nHandle	= �������Ķ�λ������
	point	= ��λ��������
	pView		= ���ƶ�����������ͼ
	����ֵ��
*****************************************************************************************/
void itobj::moveHandleTo(int nHandle, CPoint point, acm::CNPainterView *pView)
{
	CRect position = m_bsPosition;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}

	moveTo(position, pView);
}
/*****************************************************************************************
	<< --- itobj::moveTo	2013-11-30 --- >>
	˵����������pobj�Ƶ���λ��
	������
	position	= ��λ��
	pView		= ���ƶ�����������ͼ
	����ֵ��
*****************************************************************************************/
void itobj::moveTo(const CRect &position, acm::CNPainterView *pView)
{
	if (position == m_bsPosition)
		return;

	if (pView)
	{
		pView->invalidate(this);
		m_bsPosition = position;
		pView->invalidate(this);
		pView->GetDocument()->SetModifiedFlag();
		// ˢ�����Դ���
	}
}
/*****************************************************************************************
	<< --- itobj::allocPropertySectionArray	2013-12-06 --- >>
	˵���������������
	������
	����ֵ��
*****************************************************************************************/
std::size_t itobj::allocPropertySectionArray(itobj* pitem, bool isShare)
{

	/*itobjPropSection::PtrArrayType vecSection;
	itobj *pobj = dynamic_cast<itobj *>(pitem);
	itobjPropSection *psec = nullptr, *psubsec = nullptr;
	entry::ArrayType vec;
	if(pobj && (psec = new itobjPropSection(L"�豸��Ϣ")) != nullptr)
	{
		psec->addProperty(new itobjPropEdit(L"����", pdev->getName().c_str(), true, L"",		BEDrawShapeTypeDevice, BEDevicePropName));
		psec->addProperty(new itobjPropEdit(L"�ͺ�", pdev->getModelName().c_str(), false, L"",	BEDrawShapeTypeDevice, BEDevicePropModelName));

		// 
		vecSection.push_back(psec);
	}
	// 
	return vecSection;	
	*/
	assert(pitem);
	itobjPropSection::PtrArrayType &vecSection = pitem->getPropertys();
	if(vecSection.size() > 0) pitem->freePropertys();

	itobjPropSection *psec = nullptr, *psubsec = nullptr;

	if((psec = new itobjPropSection(L"��������")) != nullptr)
	{
		//case gtc::itobj::BEItemPropText:
		psec->addProperty(new itobjPropEdit(L"����", pitem->m_wsText.c_str(), true, nullptr, BEDrawShapeTypeItem, BEItemPropText));
		//	break;
		//case gtc::itobj::BEItemPropBackcolor:
		psec->addProperty(new itobjPropColor(L"����ɫ", pitem->getBackColor(), _DEFAULT_ITEM_BACKCOLOR_, L"��ͼ���򱳾�ɫ��", BEDrawShapeTypeItem,BEItemPropBackColor)); 
		//	break;
		//case gtc::itobj::BEItemPropTextcolor:
		psec->addProperty(new itobjPropColor(L"�ı�ɫ", pitem->m_clrTextColor, _DEFAULT_ITEM_TEXTCOLOR_, L"��ͼ����ǰ��ɫ����������ɫ��", BEDrawShapeTypeItem,BEItemPropTextColor)); 
		//	break;
		//case gtc::itobj::BEItemPropAutoline:
		psec->addProperty(new itobjPropBoolean(L"�Զ�����", pitem->m_bAutoLine, L"���ı����ݳ�����ʾ��Χ���Ƿ��Զ������ı����Ա���ʾȫ�����ݡ�", BEDrawShapeTypeItem, BEItemPropAutoline));
		//	break;
		//case gtc::itobj::BEItemPropVisible:
		psec->addProperty(new itobjPropBoolean(L"����ɼ�", pitem->m_bVisible, L"�Ƿ���ʾ����ͼ����", BEDrawShapeTypeItem, BEItemPropVisible));
		//	break;
		//case gtc::itobj::BEItemPropAlign:
		entry::PtrArrayType vec(std::begin(gc_tbAlignMode), std::end(gc_tbAlignMode));
		psec->addProperty(new itobjPropCombo(L"����", pitem->m_beAlign, vec, false, L"�����ı��������ڵĶ��뷽ʽ��", BEDrawShapeTypeItem, BEItemPropAlign));
		psec->addProperty(new itobjPropFont(L"����", pitem->m_bsFont, L"�������塣", BEDrawShapeTypeItem, BEItemPropFont));
		//	break;
		//case gtc::itobj::BEItemPropPen:
		psubsec = new itobjPropSection(L"�߿�", nullptr, true);
		vec.assign(std::begin(gc_tbBoardStyle), std::end(gc_tbBoardStyle));
		psubsec->addProperty(new itobjPropCombo(L"��ʽ", pitem->m_beBorderStyle, vec, false, L"", BEDrawShapeTypeItem, BEItemPropBorderStyle));
		psubsec->addProperty(new itobjPropColor(L"��ɫ", pitem->getForeColor(), _DEFAULT_ITEM_FORECOLOR_, L"", BEDrawShapeTypeItem,BEItemPropForeColor));
		vec.assign(std::begin(gc_tbPenStyle), std::end(gc_tbPenStyle));
		psubsec->addProperty(new itobjPropCombo(L"����", pitem->m_bsPen.style, vec, false, L"",	BEDrawShapeTypeItem, BEItemPropPenStyle));
		psubsec->addProperty(new itobjPropSpin(L"�߿�", pitem->m_bsPen.width, 1, 10, L"",	BEDrawShapeTypeItem, BEItemPropPenWidth));
		psec->addProperty(psubsec);
		//	break;
		//case gtc::itobj::BEItemPropBrush:
		//psubsec = new itobjPropSection(L"���");
		////psubsec->addProperty(new itobjPropColor(L"��ɫ", pitem->m_bsBrush.color, 0xFF000000, L"", BEDrawShapeTypeItem,BEItemPropBrushColor));
		//vec.assign(std::begin(gc_tbBrushStyle), std::end(gc_tbBrushStyle));
		//psubsec->addProperty(new itobjPropCombo(L"��ʽ", pitem->m_bsBrush.style, vec, L"", BEDrawShapeTypeItem, BEItemPropBrushStyle));
		//vec.assign(std::begin(gc_tbBrushHatch), std::end(gc_tbBrushHatch));
		//psubsec->addProperty(new itobjPropCombo(L"��Ӱ", pitem->m_bsBrush.hatch, vec, L"", BEDrawShapeTypeItem, BEItemPropBrushHatch));
		//psec->addProperty(psubsec);
		//	break;
		//case gtc::itobj::BEItemPropFont:
		//	break;
		//case gtc::itobj::BEItemPropPosition:
		psubsec = new itobjPropSection(L"λ��");
		psubsec->addProperty(new itobjPropSpin(L"��", pitem->m_bsPosition.x, INT_MIN, INT_MAX, L"",		BEDrawShapeTypeItem,BEItemPropPositionX));
		psubsec->addProperty(new itobjPropSpin(L"��", pitem->m_bsPosition.y, INT_MIN, INT_MAX, L"",		BEDrawShapeTypeItem,BEItemPropPositionY));
		psubsec->addProperty(new itobjPropSpin(L"��", pitem->m_bsPosition.width, INT_MIN, INT_MAX, L"",	BEDrawShapeTypeItem,BEItemPropPositionWidth));
		psubsec->addProperty(new itobjPropSpin(L"��", pitem->m_bsPosition.height, INT_MIN, INT_MAX, L"",	BEDrawShapeTypeItem,BEItemPropPositionHeight));
		psec->addProperty(psubsec);
		//	break;
		//case gtc::itobj::BEItemPropGroupflag:
		psec->addProperty(new itobjPropSpin(L"������", pitem->m_uiGroupFlag, 0, 10000, L"", BEDrawShapeTypeItem, BEItemPropGroupflag));
		//	break;
		//case gtc::itobj::BEItemPropBordervisible:
		//psec->addProperty(new itobjPropBoolean(L"�߿�ɼ�", m_bBorder, L"�Ƿ���ʾ����ͼ����ı߿�", BEDrawShapeTypeItem, BEItemPropBordervisible));
		//	break;

		vecSection.push_back(psec);
	}
	return vecSection.size();
}
void itobj::freePropertys()
{
	for (itobjPropSection::PtrArrayType::iterator iter = m_vecPropertySection.begin(); iter != m_vecPropertySection.end(); ++iter)
	{
		delete *iter;
	}
	m_vecPropertySection.clear();
}
/*****************************************************************************************
	<< --- itobj::updatePropertyValue	2013-12-06 --- >>
	˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
	������
	pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
	����ֵ��
*****************************************************************************************/
bool itobj::updatePropertyValue(itobjProp *pprop)
{

	/*if(pprop && __super::updatePropertyValue(pprop))
		return true;
	if(!pprop || pprop->getTypekey() != getType())
		return false;
	bool exists = true;
	COleVariant var = pprop->getValue();
	switch (pprop->getItemkey())
	{
	case gtc::device::BEDevicePropName:
		var.ChangeType(VT_BSTR);
		setName(_bstr_t(var.bstrVal));
		break;
	default:
		exists = false;
		break;
	}
	return exists;
	*/

	if(!pprop || pprop->getTypekey() != BEDrawShapeTypeItem)
		return false;
	bool exists = true;
	COleVariant vval = pprop->getValue();
	switch (pprop->getItemkey())
	{
	case gtc::itobj::BEItemPropText:
		vval.ChangeType(VT_BSTR);
		m_wsText = _bstr_t(vval.bstrVal);
		break;
	case gtc::itobj::BEItemPropBackColor:
		vval.ChangeType(VT_UI4);
		setBackColor(vval.ulVal);
		break;
	case gtc::itobj::BEItemPropTextColor:
		vval.ChangeType(VT_UI4);
		m_clrTextColor = vval.ulVal;
		break;
	case gtc::itobj::BEItemPropForeColor:
		vval.ChangeType(VT_UI4);
		setForeColor(vval.ulVal);
		break;
	case gtc::itobj::BEItemPropAutoline:
		vval.ChangeType(VT_BOOL);
		m_bAutoLine = vval.boolVal==VARIANT_TRUE;
		break;
	case gtc::itobj::BEItemPropVisible:
		vval.ChangeType(VT_BOOL);
		m_bVisible = vval.boolVal== VARIANT_TRUE;
		break;
	case gtc::itobj::BEItemPropAlign:
		vval.ChangeType(VT_I4);
		m_beAlign = BEAlignMode(vval.lVal);
		break;
	case BEItemPropBorderStyle:
		vval.ChangeType(VT_I4);
		setBorderStyle(BEBorderStyle(vval.lVal));
		break;
	case gtc::itobj::BEItemPropPen:
		break;
	case gtc::itobj::BEItemPropPenWidth:
		vval.ChangeType(VT_I4);
		m_bsPen.width = vval.lVal;
		break;
	//case gtc::itobj::BEItemPropPenColor:
	//	vval.ChangeType(VT_I4);
	//	m_bsPen.color = vval.ulVal;
	//	break;
	case gtc::itobj::BEItemPropPenStyle:
		vval.ChangeType(VT_I4);
		m_bsPen.style = BEPenStyle(vval.lVal);
		break;
	case gtc::itobj::BEItemPropBrush:
		break;
	//case gtc::itobj::BEItemPropBrushColor:
	//	vval.ChangeType(VT_UI4);
	//	m_bsBrush.backColor = vval.ulVal;
	//	break;
	case gtc::itobj::BEItemPropBrushStyle:
		vval.ChangeType(VT_I4);
		m_bsBrush.style = BEBrushStyle(vval.lVal);
		break;
	case gtc::itobj::BEItemPropBrushHatch:
		vval.ChangeType(VT_I4);
		m_bsBrush.hatch = BEHatchStyle(vval.lVal);
		break;
	case gtc::itobj::BEItemPropFont:
		m_bsFont.fromData(convert::toData(vval));
		break;
	case gtc::itobj::BEItemPropPosition:
		break;
	case gtc::itobj::BEItemPropPositionX:
		vval.ChangeType(VT_I4);
		m_bsPosition.x = vval.lVal;
		break;
	case gtc::itobj::BEItemPropPositionY:
		vval.ChangeType(VT_I4);
		m_bsPosition.y = vval.lVal;
		break;
	case gtc::itobj::BEItemPropPositionWidth:
		vval.ChangeType(VT_I4);
		m_bsPosition.width = vval.lVal;
		break;
	case gtc::itobj::BEItemPropPositionHeight:
		vval.ChangeType(VT_I4);
		m_bsPosition.height = vval.lVal;
		break;
	case gtc::itobj::BEItemPropGroupflag:
		vval.ChangeType(VT_UI4);
		m_uiGroupFlag = vval.ulVal;
		break;
	default:
		exists = false;
		break;
	}
	return exists;
}

/*****************************************************************************************
	<< --- itobj::messageProperty	2013-12-24 --- >>
	˵��������ָ�����Ա�ǵ�����ֵ��������ϢWM_ME_ITEM_PROPERTY_CHANGED���͸����Ա༭����
	������
	typeKey		= �������ͣ��磺BEDrawShapeType
	vecItemKey	= ��Ҫ���µ����Ա��
	����ֵ��
*****************************************************************************************/
void itobj::messageProperty(int32_t typeKey, std::vector<int32_t> &vecItemKey)
{
	if(typeKey == BEDrawShapeTypeItem) return;

	gtc::itobjProp::PtrArrayType *pvecProp = new gtc::itobjProp::PtrArrayType();
	itobjProp *pitem = nullptr;
	COleVariant var;

	for(std::vector<int32_t>::iterator iter = vecItemKey.begin(), end = vecItemKey.end(); iter != end; ++iter)
	{
		pitem = itobjProp::findPropertyItem(typeKey, *iter);
		if(!pitem)
			break;
		var.Clear();
		bool exists = true;
		switch (BEItemProp(*iter))
		{
		case gtc::itobj::BEItemPropText:
			var = getText().c_str();
			break;
		case gtc::itobj::BEItemPropBackColor:
			var.ChangeType(VT_UI4);
			var.ulVal = getBackColor();
			break;
		case gtc::itobj::BEItemPropForeColor:
			var.ChangeType(VT_UI4);
			var.ulVal = getForeColor();
			break;
		case gtc::itobj::BEItemPropAutoline:
			var.ChangeType(VT_BOOL);
			var.boolVal = isAutoLine()?VARIANT_TRUE:VARIANT_FALSE;
			break;
		case gtc::itobj::BEItemPropVisible:
			var.ChangeType(VT_BOOL);
			var.boolVal = isVisible()?VARIANT_TRUE:VARIANT_FALSE;
			break;
		case gtc::itobj::BEItemPropAlign:
			var = LONG(getAlignMode());
			break;
		case gtc::itobj::BEItemPropBorderStyle:
			var = LONG(getBorderStyle());
			break;
		case gtc::itobj::BEItemPropPen:
			break;
		case gtc::itobj::BEItemPropPenWidth:
			var = LONG(m_bsPen.width);
			break;
		//case gtc::itobj::BEItemPropPenColor:
		//	var.ChangeType(VT_UI4);
		//	var.ulVal = m_bsPen.color;
		//	break;
		case gtc::itobj::BEItemPropPenStyle:
			var = LONG(m_bsPen.style);
			break;
		case gtc::itobj::BEItemPropBrush:
			break;
		//case gtc::itobj::BEItemPropBrushColor:
		//	var.ChangeType(VT_UI4);
		//	var.ulVal = m_bsBrush.backColor;
		//	break;
		case gtc::itobj::BEItemPropBrushStyle:
			var = LONG(m_bsBrush.style);
			break;
		case gtc::itobj::BEItemPropBrushHatch:
			var = LONG(m_bsBrush.hatch);
			break;
		case gtc::itobj::BEItemPropFont:
			convert::toVariant(var, m_bsFont.toData());
			break;
		case gtc::itobj::BEItemPropPosition:
			break;
		case gtc::itobj::BEItemPropPositionX:
			var = LONG(m_bsPosition.x);
			break;
		case gtc::itobj::BEItemPropPositionY:
			var = LONG(m_bsPosition.y);
			break;
		case gtc::itobj::BEItemPropPositionWidth:
			var = LONG(m_bsPosition.width);
			break;
		case gtc::itobj::BEItemPropPositionHeight:
			var = LONG(m_bsPosition.height);
			break;
		case gtc::itobj::BEItemPropGroupflag:
			var = LONG(getGroupFlag());
			break;
		default:
			exists = false;
			break;
		}	
		if(exists)
		{
			pitem->setValue(var);
			pvecProp->push_back(pitem);
		}
	}
	if(pvecProp && pvecProp->size() < 1)
		delete pvecProp;
	if(pvecProp && pvecProp->size() > 0)
	{
		app::getMainWnd()->PostMessage(WM_ME_ITEM_PROPERTY_CHANGED, 0L, (LPARAM)pvecProp);
	}
}
itobjProp* itobj::findProperty(int32_t typeKey, int32_t itemKey)
{
	return itobjPropSection::findProperty(typeKey, itemKey, m_vecPropertySection);
}

bool itobj::existsProperty(const itobjProp *pProp) const
{
	bool bExists = false;
	for (itobjPropSection::PtrArrayType::const_iterator iter = m_vecPropertySection.cbegin(); iter != m_vecPropertySection.cend(); ++iter)
	{
		bExists = (*iter)->existsProperty(pProp);
		if(bExists) return true;
	}
	
	return false;
}
#endif
	// WIN32 ��������
#if defined(_WIN32)
/*****************************************************************************************
	<< --- itobj::getHandleRect	2013-12-06 --- >>
	˵�����õ���λ����������
	������
	handle	= ��λ����������������1
	pview	= ����ͼ����������ͼ
	����ֵ����λ�������߼����꣩
*****************************************************************************************/
rect itobj::getHandleRect(int32_t handle, CDC *pdc)
{
	rect rt;
	//POINT pt = getHandle(handle).toMSPoint();
	//int32_t space = mc_iTrackerSize/2;
	//if(pdc)
	//{
	//	//pview->docToClient(pt);
	//	pdc->LPtoDP(&pt);
	//	rect.setRect(pt.x, pt.y, pt.x, pt.y);
	//	rect.inflateRect(space, space);
	//	//rect.SetRect(pt.x-space, pt.y-space, pt.x+space, pt.y+space);
	//	//pview->clientToDoc(rect);
	//	CRect rt = rect;
	//	//rt.NormalizeRect();
	//	//int offx=0,offy=0;
	//	//if(handle == 1 || handle == 8 || handle == 7)
	//	//	offx = 1;
	//	//else if(handle == 3 || handle == 5 || handle == 5)
	//	//	offx = -1;
	//	//if(handle == 1 || handle == 2 || handle == 3)
	//	//	offy = 1;
	//	//else if(handle == 5 || handle == 6 || handle == 7)
	//	//	offy = -1;
	//	//rt.OffsetRect(offx, offy);
	//	pdc->DPtoLP(rt);
	//	rect.setRect(rt);
	//}
	return rt;
}
rect itobj::getHandleRect(int32_t handle, acm::CNPainterView *pview)
{
	rect rt;
	//CPoint pt = getHandle(handle).toMSPoint();
	//int32_t space = mc_iTrackerSize/2;
	//if(pview)
	//{
	//	pview->docToClient(pt);
	//	rect.setRect(pt.x, pt.y, pt.x, pt.y);
	//	rect.inflateRect(space, space);
	//	//rect.SetRect(pt.x-space, pt.y-space, pt.x+space, pt.y+space);
	//	CRect rt = rect;
	//	//rt.NormalizeRect();
	//	//int offx=0,offy=0;
	//	//if(handle == 1 || handle == 8 || handle == 7)
	//	//	offx = 1;
	//	//else if(handle == 3 || handle == 5 || handle == 5)
	//	//	offx = -1;
	//	//if(handle == 1 || handle == 2 || handle == 3)
	//	//	offy = 1;
	//	//else if(handle == 5 || handle == 6 || handle == 7)
	//	//	offy = -1;
	//	//rt.OffsetRect(offx, offy);
	//	pview->clientToDoc(rt);
	//	rect.setRect(rt);
	//}
	return rt;
}
#endif
/*****************************************************************************************
	<< --- static itobj::findCommunityBasecalss	2013-12-12 --- >>
	˵��������ָ��������Ĺ����������ͱ�ʾ
	������
	vecItems	= ��ɸѡ�Ķ�������
	����ֵ����ͼ��������
*****************************************************************************************/
BSItobjSchema* itobj::findCommunityBasecalss(PtrArrayType &vecItems)
{
	if(vecItems.size() <= 0)
		return nullptr;
	BSItobjSchema *pbase = vecItems[0]->getSchemaNode();
	for(PtrArrayType::size_type idx = 1; idx < vecItems.size(); ++idx)
	{
		pbase = BSItobjSchema::findCommunityBasecalss(pbase, vecItems[idx]->getSchemaNode());
		if(pbase && pbase->classType == BEDrawShapeTypeItem)
			break;
	}

	return pbase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjRect	2013-11-24 --- >>
//	���ƣ����Σ�Բ�Ǿ��Σ���Բ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
_IMPLEMENT_ITOBJ_(itobjRect, itobj, BEDrawShapeTypeRect, 0x1)

itobjRect::itobjRect()
	: itobj()
	, m_beShape(BEShapeRectangle)
	, m_roundness(16, 16)
{

}

itobjRect::itobjRect(const rect& vrect)
	: itobj(vrect)
	, m_beShape(BEShapeRectangle)
	, m_roundness(16, 16)
{

}

itobjRect::itobjRect(const rect& vrect, BEShape vshape, point vround)
	: itobj(vrect)
	, m_beShape(vshape)
	, m_roundness(vround)
{
}

itobjRect::itobjRect(const itobjRect& virect)
	: itobj(virect)
	, m_beShape(virect.m_beShape)
	, m_roundness(virect.m_roundness)
{
}

itobjRect& itobjRect::operator =(const itobjRect& virect)
{
	if(this != &virect)
	{
		itobj::operator =(virect);

		m_beShape = virect.m_beShape;
		m_roundness = virect.m_roundness;
	}
	return *this;
}

itobjRect::~itobjRect()
{
	DEBUG_LOG(L"free %s", description().c_str());
}

// ����
std::wstring itobjRect::description(int indent) const
{
	std::wostringstream os;
	wchar_t buf[20];
	std::swprintf(buf, 20, L"(%d,%d)", m_roundness.x, m_roundness.y);

	gtc::outClassBegin(os, L"itobjRect", indent);
	gtc::outClassItem(os, L"��״", toStringBEShape(m_beShape), indent << 1);
	gtc::outClassItem(os, L"�ս�", buf, indent << 1);
	gtc::outBaseClass(os, *(itobj*)this, indent << 1);
	gtc::outClassEnd(os, L"itobjRect", indent);
	return os.str();
}

int32_t itobjRect::getHandleCount()
{
	return m_beShape == BEShapeLine ? 2 :
		itobj::getHandleCount() + (m_beShape == BEShapeRoundRectangle);
}
// returns center of handle in logical coordinates
point itobjRect::getHandle(int32_t vhandle)
{
	if (m_beShape == BEShapeLine && vhandle == 2)
		vhandle = 5;
	else if (m_beShape == BEShapeRoundRectangle && vhandle == 9)
	{
		rect rt = m_bsPosition;
		//rect.normalizeRect();
		point point = rt.bottomRight();
		point.x -= m_roundness.x/2;
		point.y -= m_roundness.y/2;
		return point;
	}

	return itobj::getHandle(vhandle);
}
bool itobjRect::intersects(const rect& vrect)
{
	
	//rect rectT = vrect;
	//rectT.normalizeRect();

	rect fixed = m_bsPosition;
	//fixed.normalizeRect();
	
	if (!(fixed & vrect))
		return false;

#if defined(_WIN32)
	CRgn rgn;
	switch (m_beShape)
	{
	case BEShapeRectangle:
		return true;

	case BEShapeRoundRectangle:
		rgn.CreateRoundRectRgn(fixed.getLeft(), fixed.getTop(), fixed.getRight(), fixed.getBottom(),
			m_roundness.x, m_roundness.y);
		break;

	case BEShapeEllipse:
		{
			RECT rc = fixed;
			rgn.CreateEllipticRgnIndirect(&rc);

		}
		break;

	case BEShapeLine:
		{
			//int x = (m_logpen.lopnWidth.x + 5) / 2;
			//int y = (m_logpen.lopnWidth.y + 5) / 2;
			int x = (m_bsPen.width+5)/2;
			int y = (m_bsPen.width+5)/2;
			POINT points[4];
			points[0].x = fixed.getLeft();
			points[0].y = fixed.getTop();
			points[1].x = fixed.getLeft();
			points[1].y = fixed.getTop();
			points[2].x = fixed.getRight();
			points[2].y = fixed.getBottom();
			points[3].x = fixed.getRight();
			points[3].y = fixed.getBottom();

			if (fixed.getLeft() < fixed.getRight())
			{
				points[0].x -= x;
				points[1].x += x;
				points[2].x += x;
				points[3].x -= x;
			}
			else
			{
				points[0].x += x;
				points[1].x -= x;
				points[2].x -= x;
				points[3].x += x;
			}

			if (fixed.getTop() < fixed.getBottom())
			{
				points[0].y -= y;
				points[1].y += y;
				points[2].y += y;
				points[3].y -= y;
			}
			else
			{
				points[0].y += y;
				points[1].y -= y;
				points[2].y -= y;
				points[3].y += y;
			}
			rgn.CreatePolygonRgn(points, 4, ALTERNATE);
		}
		break;
	}
	RECT rc = fixed;
	return rgn.RectInRegion(&rc) == TRUE;
#endif
	return false;
}
#if defined(_WIN32)
void itobjRect::draw(CDC *pdc)
{
	if(!m_bVisible || !pdc)
		return;
	CFont *poldfont = nullptr,font;
	CBrush* poldbrush = nullptr, brush;
	CPen* poldpen = nullptr, pen;
	LOGFONT lgfont = {0};
	//LOGBRUSH lgbrush = {0};
	int oldbkm = pdc->GetBkMode();
	m_bsFont.toMSFont(&lgfont);
	if(!font.CreateFontIndirect(&lgfont))
		return;
	//lgbrush = m_bsBrush.toMSBrush();
	//lgbrush.lbColor = m_clrBackColor;
	if(!brush.CreateBrushIndirect(&m_bsBrush.toMSBrush()))
		return;
	if(!pen.CreatePenIndirect(&m_bsPen.toMSPen()))
		return;
	//if(!font.CreatePointFontIndirect(&lgfont))
	//	return;
	CRect rect = m_bsPosition;
	rect.NormalizeRect();
	poldbrush = pdc->SelectObject(&brush);
	poldfont = pdc->SelectObject(&font);
	poldpen = pdc->SelectObject(&pen);
	// ������״������ʾ�ı�
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(getTextColor());
	UINT algin = m_beAlign;
	if(m_bAutoLine)
	{
		algin = algin & (~(DT_SINGLELINE))& (~(DT_VCENTER));
		algin |= DT_WORDBREAK;
	}
	CRect rttxt = rect;
	rttxt.DeflateRect(2,2);
	switch (m_beShape)
	{
	case gtc::itobjRect::BEShapeRectangle:
		pdc->Rectangle(rect);
		pdc->DrawText(m_wsText.c_str(), rttxt, algin);
		break;
	case gtc::itobjRect::BEShapeRoundRectangle:
		pdc->RoundRect(rect, m_roundness.toMSPoint());
		pdc->DrawText(m_wsText.c_str(), rttxt, algin);
		break;
	case gtc::itobjRect::BEShapeEllipse:
		pdc->Ellipse(rect);
		pdc->DrawText(m_wsText.c_str(), rttxt, algin);
		break;
	case gtc::itobjRect::BEShapeLine:
		pdc->MoveTo(m_bsPosition.topLeft().toMSPoint());
		pdc->LineTo(m_bsPosition.bottomRight().toMSPoint());
		// �ı�����ƽ��
		//pdc->DrawText(m_wsText.c_str(), rttxt, algin);
		break;
	default:
		break;
	}

	// release
	pdc->SetBkMode(oldbkm);
	if(poldbrush)
		pdc->SelectObject(poldbrush);
	if(poldfont)
		pdc->SelectObject(poldfont);
	if(poldpen)
		pdc->SelectObject(poldpen);
}

/*****************************************************************************************
	<< --- itobjRect::getHandleCursor	2013-12-11 --- >>
	˵�����õ�ָ����λ����
	������
	nHandle		= ��λ������
	����ֵ����λ����
*****************************************************************************************/
HCURSOR itobjRect::getHandleCursor(int nHandle)
{
	if (m_beShape == BEShapeLine && nHandle == 2)
		nHandle = 5;
	else if (m_beShape == BEShapeRoundRectangle && nHandle == 9)
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	return itobj::getHandleCursor(nHandle);
}

/*****************************************************************************************
	<< --- itobjRect::moveHandleTo	2013-11-30 --- >>
	˵����ͨ��ָ����λ��ı����ߴ�
	������
	nHandle	= �������Ķ�λ������
	point	= ��λ��������
	pView		= ���ƶ�����������ͼ
	����ֵ��
*****************************************************************************************/
void itobjRect::moveHandleTo(int nHandle, CPoint point, acm::CNPainterView *pView)
{
	if (m_beShape == BEShapeLine && nHandle == 2)
		nHandle = 5;
	else if (m_beShape == BEShapeRoundRectangle && nHandle == 9 && pView)
	{
		CRect rect = m_bsPosition;
		rect.NormalizeRect();
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < rect.left + rect.Width() / 2)
			point.x = rect.left + rect.Width() / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < rect.top + rect.Height() / 2)
			point.y = rect.top + rect.Height() / 2;
		m_roundness.x = 2 * (rect.right - point.x);
		m_roundness.y = 2 * (rect.bottom - point.y);
		pView->GetDocument()->SetModifiedFlag();
		pView->invalidate(this);
		return;
	}

	itobj::moveHandleTo(nHandle, point, pView);
}
/*****************************************************************************************
	<< --- itobj::drawTracker	2013-12-09 --- >>
	˵�������Ե��Ƿ��ڶ����λ����
	������
	point		= �����Ե㣨�߼����꣩
	pView		= ��ͼ����������ͼ
	bSelected	= trueʱ��λ�飬������Զ���
	����ֵ��
		= 0��δѡ��
		> 0����λ������
*****************************************************************************************/
int32_t itobjRect::hitTest(CPoint point, acm::CNPainterView *pView, bool bSelected)
{
	return itobj::hitTest(point, pView, bSelected);
}
/*****************************************************************************************
	<< --- itobj::drawTracker	2013-12-09 --- >>
	˵�������ƶ�λ����������
	������
	vpdc		= ��ͼ����
	vstate		= ��λ���״̬
	vbkcolor	= ������ɫ
	����ֵ��
*****************************************************************************************/
void itobjRect::drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor)
{
	itobj::drawTracker(pdc, vstate, vbkcolor);
}
/*****************************************************************************************
	<< --- static itobjRect::allocPropertySectionArray	2013-12-06 --- >>
	˵���������������
	������
	����ֵ��
*****************************************************************************************/
std::size_t itobjRect::allocPropertySectionArray(itobj* pitem, bool isShare)
{
	return itobj::allocPropertySectionArray(pitem, isShare);
	// �˴�Ҫ���ݲ�ͬ��״���·�������
}

/*****************************************************************************************
	<< --- itobjRect::updatePropertyValue	2013-12-06 --- >>
	˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
	������
	pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
	����ֵ��
*****************************************************************************************/
bool itobjRect::updatePropertyValue(itobjProp *pprop)
{
	if(pprop && itobj::updatePropertyValue(pprop))
		return true;
	if(!pprop || pprop->getTypekey() != BEDrawShapeTypeRect)
		return false;
	bool exists = true;


	return exists;
}
#endif


}