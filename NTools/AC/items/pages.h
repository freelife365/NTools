#ifndef __PAGES_H__
#define __PAGES_H__
#pragma once

#include "AC/basedef.h"
#include "AC/baseobj.h"
#include "AC/drawdef.h"

#include "AC/items/items.h"
namespace gtc{



//////////////////////////////////////////////////////////////////////////
//	2015-9-17	
typedef wchar_t PAPERNAME[64];	// ֽ�����������磺A4���ŷ�
typedef wchar_t BINNAME[24];	// ��ӡ��ֽ������
// ֽ����Ϣ�������߼���λ��0.1���ף�
typedef struct BSPaperInfo {
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & wsName;  
		ar & iSize;
		ar & bsSize;
		ar & iOrientation;
	}
#endif
public:
	std::wstring	wsName;			// ֽ��������
	int32_t			iSize;			// ֽ�ųߴ�������Ϊ0ʱ����bsSize����ĳߴ�
	size			bsSize;			// ֽ�ųߴ�0.1����
	int16_t			iOrientation;	// ֽ�ŷ��򣻺���2'DMORIENT_LANDSCAPE'������1'DMORIENT_PORTRAIT'

	BSPaperInfo() : wsName(L"A4"), bsSize(2100,2970), iSize(9/*DMPAPER_A4*/)
		, iOrientation(1/*DMORIENT_PORTRAIT*/)
	{
	}
	BSPaperInfo(bool isLandscape)
		: wsName(L"A4"), bsSize(2100,2970), iSize(9/*DMPAPER_A4*/)
		, iOrientation(isLandscape?2:1)
	{
	}
	BSPaperInfo(const wchar_t *pname, int32_t width, int32_t height, int32_t sizeIndex)
		: wsName(pname?pname:L""), bsSize(width, height), iSize(sizeIndex)
		, iOrientation(1/*DMORIENT_PORTRAIT*/)
	{
	}
	BSPaperInfo(const BSPaperInfo &paper) : wsName(paper.wsName), bsSize(paper.bsSize), iSize(paper.iSize)
		, iOrientation(paper.iOrientation)
	{
	}
	BSPaperInfo& operator=(const BSPaperInfo &paper)
	{
		if(this != &paper)
		{
			wsName = paper.wsName;
			bsSize = paper.bsSize;
			iSize = paper.iSize;
			iOrientation = paper.iOrientation;
		}
		return *this;
	}
	// ����
	std::wstring description(int indent = 4) const
	{
		std::wostringstream os;
		wchar_t buf[20]={0};

		gtc::outClassBegin(os, L"BSPaperInfo", indent);
		gtc::outClassItem(os, L"ֽ��", wsName, indent << 1);
		std::swprintf(buf, 20, L"%d %dx%d)", iSize, bsSize.width, bsSize.height);
		gtc::outClassItem(os, L"ֽ�ųߴ�", buf, indent << 1);
		std::swprintf(buf, 20, L"%s", iOrientation==1?L"����":L"����");
		gtc::outClassItem(os, L"����", buf, indent << 1);
		gtc::outClassEnd(os, L"BSPaperInfo", indent);
		return os.str();
	}
public:
	bool isLandscape() const {				return iOrientation == 2;					}



}BSPaperInfo;

//////////////////////////////////////////////////////////////////////////
//	2015-9-17	
// ֽ����Ϣ
typedef struct BSBINInfo {
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & wsName;  
		ar & iSource;
	}
#endif
public:
	std::wstring	wsName;		// ֽ����
	int32_t			iSource;	// ֽ��������see the description of the dmDefaultSource member of the DEVMODE structure

	BSBINInfo() : wsName(L""), iSource(0)
	{
	}
	BSBINInfo(const wchar_t *pname, int32_t binIndex)
		: wsName(pname?pname:L""), iSource(binIndex)
	{
	}
	BSBINInfo(const BSBINInfo &bin) : wsName(bin.wsName), iSource(bin.iSource)
	{
	}
	BSBINInfo& operator=(const BSBINInfo &bin)
	{
		if(this != &bin)
		{
			wsName = bin.wsName;
			iSource = bin.iSource;
		}
		return *this;
	}
	// ����
	std::wstring description(int indent = 4) const
	{
		std::wostringstream os;

		gtc::outClassBegin(os, L"BSBINInfo", indent);
		gtc::outClassItem(os, L"ֽ��", wsName, indent << 1);
		gtc::outClassItem(os, L"����", iSource, indent << 1);
		gtc::outClassEnd(os, L"BSBINInfo", indent);
		return os.str();
	}


}BSBINInfo;
//////////////////////////////////////////////////////////////////////////
//	2015-9-17	
// ҳü��Ϣ��������ҳ�߾��ڲࡣ�������߼���λ��0.1���ף�
typedef struct BSHeaderFooterInfo {
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & wsTitle;  
		ar & uiHeight;
		ar & beAlign;
		ar & bsFont;

		ar & iPageStyle;
		ar & bePageAlign;
		ar & bsPageFont;
		ar & vecLogo;
		ar & bsLogo;
	}
#endif
public:
	std::wstring			wsTitle;		// ����
	uint16_t				uiHeight;		// �߶�
	BEAlignMode				beAlign;		// ������뷽ʽ
	BSFont					bsFont;			// ��������

	int32_t					iPageStyle;		// ҳ����ʽ���磺0=�գ�1=�ڣ�ҳ ��xҳ��2=����
	BEAlignMode				bePageAlign;	// βע���뷽ʽ
	BSFont					bsPageFont;		// βע����
	std::vector<uint8_t>	vecLogo;	// logoͼƬ
	rect					bsLogo;		// logoλ�ã���������Ͻǵ�ƫ��λ�ã�����ƽ�����Ż���

	BSHeaderFooterInfo(uint16_t height = 150) 
		: uiHeight(height), wsTitle(L""), beAlign(BEAlignModeAllCenter)
		, iPageStyle(0), bePageAlign(BEAlignModeAllRight)
	{
	}
	BSHeaderFooterInfo(const BSHeaderFooterInfo &info)
		: uiHeight(info.uiHeight), wsTitle(info.wsTitle), beAlign(info.beAlign), bsFont(info.bsFont)
		, iPageStyle(info.iPageStyle), bePageAlign(info.bePageAlign), bsPageFont(info.bsPageFont)
		, vecLogo(info.vecLogo), bsLogo(info.bsLogo)
	{
	}
	BSHeaderFooterInfo& operator=(const BSHeaderFooterInfo &info)
	{
		if(this != &info)
		{
			uiHeight = info.uiHeight;
			wsTitle = info.wsTitle;
			beAlign = info.beAlign;
			bsFont = info.bsFont;

			iPageStyle = info.iPageStyle;
			bePageAlign = info.bePageAlign;
			bsPageFont = info.bsPageFont;

			vecLogo = info.vecLogo;
			bsLogo = info.bsLogo;
		}
		return *this;
	}
	// ����
	std::wstring description(int indent = 4) const
	{
		std::wostringstream os;

		gtc::outClassBegin(os, L"BSHeaderFooterInfo", indent);
		gtc::outClassItem(os, L"�߶�", uiHeight, indent << 1);
		gtc::outClassItem(os, L"����", wsTitle, indent << 1);
		gtc::outClassItem(os, L"ҳ���ʽ", iPageStyle, indent << 1);
		wchar_t buf[20] = {0};
		std::swprintf(buf, 120, L"%s", vecLogo.size()>0?L"֧��":L"��֧��");
		gtc::outClassItem(os, L"logo", buf, indent << 1);

		gtc::outClassEnd(os, L"BSHeaderFooterInfo", indent);
		return os.str();
	}


}BSHeaderFooterInfo;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPage	2013-12-02 --- >>
//	���ƣ���������ֽ�ţ�ֽ�ű�����ҳüҳ�ŵȵ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		*��defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)ģʽ�µ�ԭ�㣨0��0��
//		x��defined(WORK_AXIS_CAD)ģʽ�µ�ԭ�㣨0��0��
//		_________________________________
//		|	|						|	|
//		|	|		MarginTop		|	|
//		|	|						|	|
//		������������������������������������������������������������������
//		|	|						|	|
//		|	|		 header			|	|
//		|	|						|	|
//		��������*��������������������������������������������������������
//		| M	|						| M	|
//		| a	|						| a	|
//		| r	|						| r	|
//		| g	|						| g	|
//		| i	|						| i	|
//		| n	|						| n	|
//		| L	|		PaperWork		| R	|
//		| e	|						| i	|
//		| f	|						| g	|
//		| t	|						| h	|
//		|	|						| t	|
//		|	|						|	|
//		|	|						|	|
//		|	|						|	|
//		��������X��������������������������������������������������������
//		|	|						|	|
//		|	|		 footer			|	|
//		|	|						|	|
//		������������������������������������������������������������������
//		|	|	   MarginBottom		|	|
//		|	|						|	|
//		������������������������������������������������������������������
// 
// 
class itobjPage : public itobj
{
	_DECLARE_ITOBJ_(itobjPage)
protected:
	typedef enum BEPageProp {
		BEPagePropPaper=1,BEPagePropPaperName,BEPagePropPaperOrientation,
		BEPagePropMargins,BEPagePropMargintop,BEPagePropMarginbottom,BEPagePropMarginleft,BEPagePropMarginright,
		BEPagePropHeaderfooter,
		BEPagePropHeader,BEPagePropHeaderHeight,BEPagePropHeaderTitle,BEPagePropHeaderAlign,BEPagePropHeaderFont,
		BEPagePropHeaderPagestyle,BEPagePropHeaderPagealign,BEPagePropHeaderPagefont,
		BEPagePropHeaderLogo,
		BEPagePropHeaderLogopos,BEPagePropHeaderLogoposX,BEPagePropHeaderLogoposY,BEPagePropHeaderLogoposWidth,BEPagePropHeaderLogoposHeight,
		BEPagePropFooter,BEPagePropFooterHeight,BEPagePropFooterTitle,BEPagePropFooterAlign,BEPagePropFooterFont,
		BEPagePropFooterPagestyle,BEPagePropFooterPagealign,BEPagePropFooterPagefont,
		BEPagePropFooterLogo,
		BEPagePropFooterLogopos,BEPagePropFooterLogoposX,BEPagePropFooterLogoposY,BEPagePropFooterLogoposWidth,BEPagePropFooterLogoposHeight,
		BEPagePropWorkercolor,BEPagePropPapershadow,
		BEPagePropGrid,BEPagePropGridstyle,BEPagePropGridcolor
	}BEPageProp;

public:
	itobjPage(bool isLandscape = false, bool margins = false, bool headerfooter = false, bool grid = false);
	itobjPage(const itobjPage &page);
	itobjPage& operator=(const itobjPage &page);
	virtual ~itobjPage();
	// ����
	std::wstring description(int indent = 4) const;
	/*****************************************************************************************
		<< --- itobjPage::clone	2013-11-29 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual itobj* clone() {	
		itobj* pitem = new itobjPage(*this);
		return pitem;
	}
public:
	/*****************************************************************************************
		<< --- itobjPage::remove	2013-11-29 --- >>
		˵�����ͷ��Լ�
		������
		����ֵ��
	*****************************************************************************************/
	virtual void remove() {
		delete this;
	}
	/*****************************************************************************************
		<< --- itobjPage::refresh	2013-11-29 --- >>
		˵����ˢ����Ϣ���磺�ı�ҳ��ߴ�����Ҫ���ñ�����
		������
		����ֵ��
	*****************************************************************************************/
	void refresh();
	/*****************************************************************************************
		<< --- itobjPage::getWindowOrg	2013-12-04 --- >>
		˵�������豸�����ԭ��(�ӿ�)ӳ�䵽�߼������(X, Y)��
		������
		bsWorker	= ����̨�ߴ�
		isprinter	= �Ƿ��Ǵ�ӡ��
		����ֵ��
	*****************************************************************************************/
	// _WIN32
	point getWindowOrg(size bsWorker, bool isprinter);
#if defined(_WIN32)
	virtual void draw(CDC *pdc);
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
	virtual int32_t hitTest(CPoint point, acm::CNPainterView *pView, bool bSelected);
	/*****************************************************************************************
		<< --- itobjPage::drawTracker	2013-12-09 --- >>
		˵�������ƶ�λ����������
		������
		vpdc		= ��ͼ����
		vstate		= ��λ���״̬
		vbkcolor	= ������ɫ
		����ֵ��
	*****************************************************************************************/
	virtual void drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor = 0xFF00FF00);
	/*****************************************************************************************
		<< --- static itobjPage::allocPropertySectionArray	2013-12-06 --- >>
		˵���������������
		������
		����ֵ��
	*****************************************************************************************/
	static std::size_t allocPropertySectionArray(itobj* pitem, bool isShare);
	virtual std::size_t allocPropertySectionArray(bool isShare) {
		return allocPropertySectionArray(this, isShare);
	}
	/*****************************************************************************************
		<< --- itobjPage::updatePropertyValue	2013-12-06 --- >>
		˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
		������
		pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
		����ֵ��
	*****************************************************************************************/
	virtual bool updatePropertyValue(itobjProp *pprop);

#endif
public:
	// ֽ�����򣬼���m_bsPosition
	void			refreshPagePosition();
	// ��ӡֽ�Ĺ�������x��y����������򣬼�:��ҳ�߾�,ҳüҳ��������
	rect			areaPaperWork() const;
	BSPaperInfo&	getPaperInfo() {								return m_bsPaper;							}
	void			setPaperInfo(const BSPaperInfo &paper) {		m_bsPaper = paper;							}

	bool			supportMargins() const {						return m_bMargins;							}
	void			setMargins(bool isMargins) {					m_bMargins = isMargins;						}
	uint16_t		getMarginTop() const {							return m_uiMarginTop;						}
	void			setMarginTop(uint16_t mTop) {					m_uiMarginTop = mTop;						}
	rect			areaMarginTop() const;

	uint16_t		getMarginBottom() const {						return m_uiMarginBottom;					}
	void			setMarginBottom(uint16_t mBottom) {				m_uiMarginBottom = mBottom;					}
	rect			areaMarginBottom() const;

	uint16_t		getMarginLeft() const {							return m_uiMarginLeft;						}
	void			setMarginLeft(uint16_t mLeft) {					m_uiMarginLeft = mLeft;						}
	rect			areaMarginLeft() const;

	uint16_t		getMarginRight() const {						return m_uiMarginRight;						}
	void			setMarginRight(uint16_t mRight) {				m_uiMarginRight = mRight;					}
	rect			areaMarginRight() const;

	bool			supportHeaderFooter(void) const {				return m_bHeaderFooter;						}
	void			setHeaderFooter(bool isHFer) {					m_bHeaderFooter = isHFer;					}
	BSHeaderFooterInfo& getHeader() {								return m_bsHeader;							}
	void			setHeader(const BSHeaderFooterInfo &info) {		m_bsHeader = info;							}
	rect			areaHeader() const;
	BSHeaderFooterInfo& getFooter() {								return m_bsFooter;							}
	void			setFooter(const BSHeaderFooterInfo &info) {		m_bsFooter = info;							}
	rect			areaFooter() const;

	uint32_t		getWorkerColor() const {						return m_clrWorker;							}
	void			setWorkerColor(uint32_t color) {				m_clrWorker = color;						}
	uint32_t		getPaperShadowColor() const {					return m_clrPaperShadow;					}
	void			setPaperShadowColor(uint32_t color) {			m_clrPaperShadow = color;					}

	bool			supportGrid() const {							return m_bGrid;								}
	void			setGrid(bool grid) {							m_bGrid = grid;								}
	BSPen			getGridColor() const {							return m_bsGrid;							}
	void			setGridColor(const BSPen &pen) {				m_bsGrid = pen;								}


#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobj>(*this);
		ar & m_clrWorker;  
		ar & m_clrPaperShadow;

		ar & m_bGrid;
		ar & m_bsGrid;

		ar & m_bsPaper;

		ar & m_bMargins;
		ar & m_uiMarginTop;
		ar & m_uiMarginBottom;
		ar & m_uiMarginLeft;
		ar & m_uiMarginRight;

		ar & m_bHeaderFooter;
		ar & m_bsHeader;
		ar & m_bsFooter;
	}
#endif
protected:
	// �������߼���λ 1=0.1����
	uint32_t		m_clrWorker;		// ����̨����ɫ
	uint32_t		m_clrPaperShadow;	// ֽ����Ӱ

	bool			m_bGrid;			// �Ƿ���Ʋο�������
	BSPen			m_bsGrid;			// ��������ʽ

	BSPaperInfo		m_bsPaper;			// ֽ����Ϣ�������߼���λ 1=0.1����

	bool			m_bMargins;			// �Ƿ�����ҳ�߾�֧��
	uint16_t		m_uiMarginTop;		// ��ҳ�߾࣬�߼���λ0.1����
	uint16_t		m_uiMarginBottom;	// ��ҳ�߾࣬�߼���λ0.1����
	uint16_t		m_uiMarginLeft;		// ��ҳ�߾࣬�߼���λ
	uint16_t		m_uiMarginRight;	// ��ҳ�߾࣬�߼���λ

	bool				m_bHeaderFooter;// �Ƿ�����ҳüҳ��֧�֣���ҳ�߾��ڲ�
	BSHeaderFooterInfo	m_bsHeader;		// ҳü�ߣ��߼���λ0.1����
	BSHeaderFooterInfo	m_bsFooter;		// ҳ�Ÿߣ��߼���λ0.1����
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPageAxis	2014-01-03 --- >>
//	������ϵ��ҳ
#define _DEFAULT_ITEM_AXISCOLOR_	0xFF000000
////////////////////////////////////////////////////////////////////////////////////////////////////////
class coordinateAxis {

public:
	typedef std::vector<coordinateAxis *> PtrArrayType;
protected:
	coordinateAxis()
		: m_beUnitType(BEUnitTypeWeight), m_iUnitValue(1), m_iOrigin(0), m_iPreScale(1), m_iScales(11)
		, m_iDevRange(1), m_fltDevPreLogic(0.0f), m_fltDevOrigin(0.0f)
	{
	}
public:
	coordinateAxis(BEUnitWeight unit, int32_t origin = 0, int32_t prescale = 1)
		: m_beUnitType(BEUnitTypeWeight), m_iUnitValue(unit), m_iOrigin(origin), m_iPreScale(prescale), m_iScales(11)
		, m_iDevRange(1), m_fltDevPreLogic(0.0f), m_fltDevOrigin(0.0f)
	{
		assert(prescale > 0);
	}
	coordinateAxis(BEUnitTime unit, int32_t origin = 0, int32_t prescale = 1)
		: m_beUnitType(BEUnitTypeTime), m_iUnitValue(unit), m_iOrigin(origin), m_iPreScale(prescale), m_iScales(11)
		, m_iDevRange(1), m_fltDevPreLogic(0.0f), m_fltDevOrigin(0.0f)
	{
		assert(prescale > 0);
	}
	coordinateAxis(const coordinateAxis &axis)
		: m_beUnitType(axis.m_beUnitType), m_iUnitValue(axis.m_iUnitValue), m_iOrigin(axis.m_iOrigin), m_iPreScale(axis.m_iPreScale)
		, m_iScales(axis.m_iScales)
		, m_iDevRange(axis.m_iDevRange), m_fltDevPreLogic(axis.m_fltDevPreLogic), m_fltDevOrigin(axis.m_fltDevOrigin)
	{
	}
	coordinateAxis& operator=(const coordinateAxis &axis)
	{
		if(this != &axis)
		{
			m_beUnitType = axis.m_beUnitType;
			m_iUnitValue = axis.m_iUnitValue;
			m_iOrigin = axis.m_iOrigin;
			m_iPreScale = axis.m_iPreScale;
			m_iDevRange = axis.m_iDevRange;
			m_fltDevPreLogic = axis.m_fltDevPreLogic;
			m_fltDevOrigin = axis.m_fltDevOrigin;
		}
		return *this;
	}
	coordinateAxis* clone() {
		return new coordinateAxis(*this);
	}
	void remove() {
		delete this;
	}

public:
	/*****************************************************************************************
		<< --- coordinateAxis::getUnitName 2014-01-21 --- >>
		˵����������ĵ�λ����
		������
		����ֵ��������ĵ�λ����
	*****************************************************************************************/
	std::wstring getUnitName() const {
		switch (m_beUnitType)
		{
		case gtc::BEUnitTypeWeight:
			return toString(BEUnitWeight(m_iUnitValue));
			break;
		case gtc::BEUnitTypeTime:
			return toString(BEUnitTime(m_iUnitValue));
			break;
		default:
			break;
		}
		return L"";
	}
	/*****************************************************************************************
		<< --  setDevRange		2015-1-7 --- >>
		˵�����趨�����᳤�ȶ�Ӧ�Ĺ���������
		������
			idevRange	= ���������ȣ���λ0.1����
		����ֵ��
	*****************************************************************************************/
	void setDevRange(int32_t idevRange)
	{
		assert(idevRange > 0);
		m_iDevRange = idevRange;
		refresh();
	}
	/*****************************************************************************************
		<< --- coordinateAxis::scalingFactor 2014-01-21 --- >>
		˵�����õ������ᵥλ����ڱ�׼��λ(ʱ��:���룻����:��)�ı���
		������
		����ֵ��������ĵ�λ����
	*****************************************************************************************/
	float scalingFactor() const {
		float scale = 1.0f;
		switch (m_beUnitType)
		{
		case gtc::BEUnitTypeWeight:
			{
				switch (BEUnitWeight(m_iUnitValue))
				{
				case gtc::BEUnitWeightMilligram:
					scale = 1000.0f;
					break;
				case gtc::BEUnitWeightGram:
					scale = 1.0f;
					break;
				case gtc::BEUnitWeightLiang:
					scale = 0.02f;
					break;
				case gtc::BEUnitWeightJing:
					scale = 0.01f;
					break;
				case gtc::BEUnitWeightKilogram:
					scale = 0.001f;
					break;
				case gtc::BEUnitWeightTon:
					scale = 0.000001f;
					break;
				default:
					break;
				}
			}

			break;
		case gtc::BEUnitTypeTime:
			{
				switch (BEUnitTime(m_iUnitValue))
				{
				case gtc::BEUnitTimeMillisecond:
					scale = 1.0f;
					break;
				case gtc::BEUnitTimeSecond:
					scale = 0.001f;
					break;
				case gtc::BEUnitTimeMinute:
					scale = 1.666667e-5f;
					break;
				default:
					break;
				}	
			}

			break;
		default:
			break;
		}
		return scale;
	}
	/*****************************************************************************************
		<< --  toClient		2015-1-7 --- >>
		˵�������߼�����ת���ɹ������꣨��λ0.1���ף�
		������
			fltLogic	= �߼�ֵ
		����ֵ����������ֵ0.1����
	*****************************************************************************************/
	float toClient(float fltLogic)
	{
		float dev = 0.0f;
		//// ת����������ֵ
		//float axisValue = scalingFactor() * fltLogic;	// ����-->x����
		// ������ֵ��ռ���豸����ֵ��
		dev = fltLogic*m_fltDevPreLogic;	// x����-->y��0.1mm
		// ���ԭ���ƶ�
		dev -= m_fltDevOrigin;
		return dev;
	}
protected:
	/*****************************************************************************************
		<< --  replace		2015-1-8 --- >>
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	void refresh() {
		m_fltDevPreLogic = (float)m_iDevRange / (m_iPreScale * m_iScales);
		m_fltDevPreLogic *= scalingFactor();
		m_fltDevOrigin = m_fltDevPreLogic * m_iOrigin;
	}
public:
	BEUnitType getUnitType() const {			return m_beUnitType;						}
	void setUnitType(BEUnitType unitType) {		
		m_beUnitType = unitType;
		refresh();
	}
	int32_t getUnitValue() const {				return m_iUnitValue;						}
	void setUnitValue(int32_t unitValue) {		
		m_iUnitValue = unitValue;
		refresh();
	}
	int32_t getOrigin() const {					return m_iOrigin;							}
	void setOrigin(int32_t origin) {
		m_iOrigin = origin;
		refresh();
	}
	int32_t getPreScale() const {				return m_iPreScale;							}
	void setPreScale(int32_t preScale) {
		m_iPreScale = preScale;
		refresh();
	}
	int32_t getScales() const {					return m_iScales;							}
	void setScales(int32_t scales) {
		m_iScales = scales;
		refresh();
	}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_beUnitType;  
		ar & m_iUnitValue;
		ar & m_iOrigin;
		ar & m_iPreScale;
		ar & m_iScales;

		ar & m_iDevRange;
		ar & m_fltDevPreLogic;
		ar & m_fltDevOrigin;
	}
#endif

private:
	BEUnitType		m_beUnitType;		// ����õĵ�λ���ͣ��磺������ʱ���
	int32_t			m_iUnitValue;		// �ᵥλ���Ͷ�Ӧ��ֵ���磺�������ˣ���...BEUnitTypeWeight����ʱ���Ӧ�ģ��룬��...BEUnitTypeTime��
	int32_t			m_iOrigin;			// ����ԭ��ֵ���߼���λ��
	int32_t			m_iPreScale;		// ÿ�ȷݶ�Ӧ��ֵ���߼���λ�����������Ϊ11�ȷݣ�����һ���ȷ���Ϊ���ͷ��
	int32_t			m_iScales;			// �������Ӧ�ĵȷ���������һ���ȷ���Ϊ���ͷ��

	int32_t			m_iDevRange;		// �᳤�ȣ��������꣬��λ0.1���ף�
	float			m_fltDevPreLogic;	// ÿ�߼���λ��Ӧ�Ĺ������꣨��λ0.1���ף�
	float			m_fltDevOrigin;		// �߼�ԭ���Ӧ�Ĺ�������
};

class itobjPageAxis : public itobjPage
{
	_DECLARE_ITOBJ_(itobjPageAxis)
public:
	typedef enum BEAxisFlag {
		BEAxisFlagX	= 0,		// X��
		BEAxisFlagY				// Y��
	}BEAxisFlag;

	typedef enum BEAxisInfo {
		BEAxisInfoYName			= 0,	// ��Ϣ1��y������
		BEAxisInfoXName			,		// ��Ϣ2��x������
		BEAxisInfoYTitle		,		// ��Ϣ3��y�����
		BEAxisInfoXTitle		,		// ��Ϣ4��x�����
		BEAxisInfoTitle			,		// ��Ϣ5������ϵ����
		BEAxisInfoRightTitle			// ��Ϣ6������ϵ�Ҳ����
	}BEAxisInfo;
	typedef enum BEAxisProp {
		BEAxisPropTitle=0,BEAxisPropTitleName,BEAxisPropTitleFont,BEAxisPropTitleColor,
		BEAxisPropRightTitle,BEAxisPropRightTitleName,BEAxisPropRightTitleFont,BEAxisPropRightTitleColor,
		BEAxisPropAxisX,BEAxisPropAxisXUnit,BEAxisPropAxisXOrigin,BEAxisPropAxisXPrescale,BEAxisPropAxisXTitle,BEAxisPropAxisXFont,BEAxisPropAxisXColor,
		BEAxisPropAxisY,BEAxisPropAxisYUnit,BEAxisPropAxisYOrigin,BEAxisPropAxisYPrescale,BEAxisPropAxisYTitle,BEAxisPropAxisYFont,BEAxisPropAxisYColor
	}BEAxisProp;

public:
	itobjPageAxis();
	itobjPageAxis(const itobjPageAxis &page);
	itobjPageAxis& operator=(const itobjPageAxis &page);
	virtual ~itobjPageAxis();
	// ����
	std::wstring description(int indent = 4) const;
	/*****************************************************************************************
		<< --- itobjPageAxis::clone	2013-11-29 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual itobj* clone() {	
		itobj* pitem = new itobjPageAxis(*this);
		return pitem;
	}
public:
	/*****************************************************************************************
		<< --- itobjPageAxis::logicToDoc	2014-01-13 --- >>
		˵�������߼�����ӳ�䵽�������ꣻ(m_iUserScaleOriginX,m_iUserScaleOriginY)��Ӧ�Ĺ�������Ϊ(0,0)
		������
		pt	= ��ӳ��������
		����ֵ��
	*****************************************************************************************/
	virtual void logicToDoc(pointf &pt);
	virtual void logicToDoc(rectf &rc);
	virtual void logicToDoc(const float &msvX, const float &msvY, float &devX, float &devY, bool reloadAxis = false);

public:
#if defined(_WIN32)
	virtual void draw(CDC *pdc);
	/*****************************************************************************************
		<< --- static itobjPageAxis::allocPropertySectionArray	2013-12-06 --- >>
		˵���������������
		������
		����ֵ��
	*****************************************************************************************/
	static std::size_t allocPropertySectionArray(itobj* pitem, bool isShare);
	virtual std::size_t allocPropertySectionArray(bool isShare) {
		return allocPropertySectionArray(this, isShare);
	}
	/*****************************************************************************************
		<< --- itobjPageAxis::updatePropertyValue	2013-12-06 --- >>
		˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
		������
		pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
		����ֵ��
	*****************************************************************************************/
	virtual bool updatePropertyValue(itobjProp *pprop);

#endif
public:
	// ����

	//int32_t		getMinUserX() const {					return m_iMinUserX;					}
	//void		setMinUserX(int32_t minuserX) {			m_iMinUserX = minuserX;				}
	//int32_t		getMaxUserX() const {					return m_iMaxUserX;					}
	//void		setMaxUserX(int32_t maxuserX) {			m_iMaxUserX = maxuserX;				}
	//int32_t		getMinUserY() const {					return m_iMinUserX;					}
	//void		setMinUserY(int32_t minuserY) {			m_iMinUserY = minuserY;				}
	//int32_t		getMaxUserY() const {					return m_iMaxUserX;					}
	//void		setMaxUserY(int32_t maxuserY) {			m_iMaxUserY = maxuserY;				}
	//
	//int32_t		getMinWorkX() const {					return m_iMinWorkX;					}
	//void		setMinWorkX(int32_t minworkX) {			m_iMinWorkX = minworkX;				}
	//int32_t		getMaxWorkX() const {					return m_iMaxWorkX;					}
	//void		setMaxWorkX(int32_t maxworkX) {			m_iMaxWorkX = maxworkX;				}
	//int32_t		getMinWorkY() const {					return m_iMinWorkX;					}
	//void		setMinWorkY(int32_t minworkY) {			m_iMinWorkY = minworkY;				}
	//int32_t		getMaxWorkY() const {					return m_iMaxWorkX;					}
	//void		setMaxWorkY(int32_t maxworkY) {			m_iMaxWorkY = maxworkY;				}
	//int32_t		getScaleOriginX() const {				return m_iUserScaleOriginX;			}
	//void		setScaleOriginX(int32_t originX) {		m_iUserScaleOriginX = originX;		}
	//int32_t		getScaleOriginY() const {				return m_iUserScaleOriginY;			}
	//void		setScaleOriginY(int32_t originY) {		m_iUserScaleOriginY = originY;		}
	//int32_t		getUserScaleX() const {					return m_iUserScaleX;				}
	//void		setUserScaleX(int32_t scaleX) {			m_iUserScaleX = scaleX;				}
	//int32_t		getUserScaleY() const {					return m_iUserScaleY;				}
	//void		setUserScaleY(int32_t scaleY) {			m_iUserScaleY = scaleY;				}

	coordinateAxis*	getAxis(BEAxisFlag axis) {				return m_arrAxis[axis];		}
	void		setAxisInfo(BEAxisInfo axisInfo, const wchar_t *pinfo);
	gtc::itobj* getAxisInfo(BEAxisInfo axisInfo) {		return m_arrInfo[axisInfo];	}

private:
	static int32_t mc_iLineThick;			// ���ߴ�
	static int32_t mc_iScaleLength;			// ��̶ȳ�
	static int32_t mc_iScaleShortLength;	// ��̶ȳ�
	static int32_t mc_iAxisArrow;			// �����Ჿ�֣�����������ļ�ͷ
	static int32_t mc_iScaleInfoSpace;		// �̶�ֵ������̱�ļ��
	static int32_t mc_iScaleInfoWidth;		// �̶�ֵ����Ŀ��
	static int32_t mc_iScaleInfoHeight;		// �̶�ֵ����ĸ߶�
	static int32_t mc_iAxisInfoSpace;		// ����Ϣ��̶�ֵ����ļ��
	static int32_t mc_iAxisInfoWidth;		// ����Ϣ�Ŀ��
	static int32_t mc_iAxisInfoHeight;		// ����Ϣ�ĸ߶�
	static int32_t mc_iAxisNameWidth;		// �����ƵĿ��
	static int32_t mc_iAxisNameHeight;		// �����Ƶĸ߶�

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjPage>(*this);
		ar & m_arrAxis;  
		ar & m_arrInfo;
	}
#endif
protected:
	/*****************************************************************************
	����ϵ˵����
		1��y�᣺areaPaperWork().cy���ֳ�11�ȷݣ�ÿ�ȷ�ӳ���û�����Ϊm_iUserScaleY
		2��x�᣺areaPaperWork().cx���ֳ�11�ȷݣ�ÿ�ȷ�ӳ���û�����Ϊm_iUserScaleX
		3����11���̶Ȳ���ʾ���䱻��ñռ�ã�
		4�������������ɫ����_DEFAULT_ITEM_AXISCOLOR_���ֺţ�С��

						OriginX+PreScaleX*10	��y��			��Ϣ5
										��Ϣ1	|         /\
												|        /  \
										��		|   /\  /    \						��
										Ϣ		|  /  \/      \      /				Ϣ
										3		| /            \____/				6
												|/
										OriginY	.-------------------------------->
											OriginX						OriginX+PreScaleX*10
																				��Ϣ2
															��Ϣ4
	*****************************************************************************/
	//int32_t		m_iMinUserX;	// �û�x�����ݣ��磺���ÿ�ʼʱ��
	//int32_t		m_iMaxUserX;	// �û�x�����ݣ��磺���ý�ֹʱ��
	//int32_t		m_iMinUserY;	// �û�y�����ݣ��磺��С���ܳ���ֵ0
	//int32_t		m_iMaxUserY;	// �û�y�����ݣ��磺�����ܳ���ֵ500��
	//int32_t		m_iMinWorkX;	// ����x�����ݣ��磺��С�߼�ֵ0
	//int32_t		m_iMaxWorkX;	// ����x�����ݣ��磺����߼�ֵ500����
	//int32_t		m_iMinWorkY;	// ����y�����ݣ��磺��С�߼�ֵ0
	//int32_t		m_iMaxWorkY;	// ����y�����ݣ��磺����߼�ֵ500����
	//BEUnitType		m_beYUnitType;			// Y�ᵥλ����
	//BEUnitType		m_beXUnitType;			// X�ᵥλ����
	//int32_t			m_iYUnitValue;			// Y�ᵥλ
	//int32_t			m_iXUnitValue;			// X�ᵥλ
	//int32_t			m_iUserScaleOriginY;	// �û��������ֵ���磺10��
	//int32_t			m_iUserScaleOriginX;	// �û��������ֵ���磺1���ӻ�GetTickCount
	//int32_t			m_iUserScaleY;			// ÿ�ȷ�ӳ���û����ݣ��磺10�ˣ���������ϵ�ܱ�ʾ11*10+m_iUserScaleOriginY��
	//int32_t			m_iUserScaleX;			// ÿ�ȷ�ӳ���û����ݣ��磺1���ӣ���������ϵ�ܱ�ʾ11+m_iUserScaleOriginX����

	coordinateAxis::PtrArrayType	m_arrAxis;
	itobj::PtrArrayType				m_arrInfo;
	//std::array<coordinateAxis*, 2>	m_arrAxis;	// ����Ϣ
	//std::array<itobj*, 6>	m_arrInfo;	// ��Ϣ��
};

}


#endif