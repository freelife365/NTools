#ifndef __ITEMS_H__
#define __ITEMS_H__
#pragma once
#include <cstdint>
#include <string>
#include <deque>
#include <sstream>
#include <algorithm>
#include "AC/basedef.h"
#include "AC/drawdef.h"
#include "AC/baseobj.h"
#include "AC/draw/drawer.h"
#include "AC/items/itobjProperty.h"
#if defined(_AFXDLL)
	#include <afxwin.h>
	#include <atltypes.h>
	namespace acm {
		class CNPainterView;
	}
#endif

//// itobj���л�
//#if defined(BOOST_SERIALIZE)
//	BOOST_CLASS_EXPORT(gtc::itobjRect)
//	BOOST_CLASS_EXPORT(gtc::itobjPropSpin)
//	BOOST_CLASS_EXPORT(gtc::itobjPropBoolean)
//	BOOST_CLASS_EXPORT(gtc::itobjPropEdit)
//	BOOST_CLASS_EXPORT(gtc::itobjPropColor)
//	BOOST_CLASS_EXPORT(gtc::itobjPropCombo)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFont)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFolder)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFile)
//#endif

namespace gtc{

class itobj;

///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ͼ�������ͣ�ÿ����һ����ͼ��������ʱ�˴�ҲҪ����
typedef enum BEDrawShapeType {
	BEDrawShapeTypeItem		= 0x0,			// ������
	BEDrawShapeTypePage		,				// ֽ�ţ���ӡҳ
	BEDrawShapeTypePageAxis	,				// ֽ�ţ�������ϵ
	BEDrawShapeTypeRect		,				// ���Ρ���Բ����
	BEDrawShapeTypeGrid		,				// ���ݱ��

	BEDrawShapeTypeDevice					= 0x8000,		// �豸���ͻ���SerialPort
	BEDrawShapeTypeDeviceWeigherBase		= 0x8001,
	BEDrawShapeTypeDeviceSerialPort			= 0x8101,		// ����ͨѶ�豸
	BEDrawShapeTypeDeviceWeigher			= 0x8102,		// �������ݲɼ�
	BEDrawShapeTypeDeviceWeigherSando		= 0x8103,		// �������ݲɼ���ɽ��AD-S321
	BEDrawShapeTypeDeviceSCR				= 0x8200,		// �ɿع�
	BEDrawShapeTypeDeviceMotor				= 0x8300,		// �������
	BEDrawShapeTypeDeviceScale				= 0x8F00		// ���ӳ�
}BEDrawShapeType;
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- BSItobjSchema	2013-12-12 --- >>
//	����һ����̳й�ϵ��B�������£�
//		depth:0				classA
//							/    \
//		depth:1			classB	 classC
//						/		  /	  |	\	\
//		depth:2		classD	   classE F  G   H
//						|		|
//		depth:3		classI	   classJ
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

// itobj�༰������Ľṹ��Ϣ
typedef struct BSItobjSchema  {
	//typedef gtc::itobjPropSection::PtrArrayType (*PFuncAllocPropertySection)(itobj *pitem, bool isShare);
	std::string						className;
	int32_t							classType;		// �����ͣ�����������Ψһkey;=BEDrawShapeType
	int32_t							classDepth;		// ��̳����
	uint32_t						classVersion;	// ��汾��
	BSItobjSchema					*pparentClass;	// ����
	std::vector<BSItobjSchema *>	vecSubClass;	// ����
	//PFuncAllocPropertySection		pfuncAllocPropertySection;	// �������Ծ�̬�ຯ��
	BSItobjSchema(const char *pname, int32_t type, int32_t depth, BSItobjSchema *pitobj, BSItobjSchema *pparentItobj, uint32_t version/*, PFuncAllocPropertySection func*/)
		: className(pname?pname:""), classType(type), classDepth(depth)
		, pparentClass(pparentItobj)/*, pfuncAllocPropertySection(func)*/
		, classVersion(version)
	{
		assert(className.length() > 0);
		//assert(depth != 0 && pitobj && pparentItobj);
		if(pitobj && pparentClass)
			pparentClass->vecSubClass.push_back(pitobj);
	}
	// ��������
	BSItobjSchema *getNodeByDepth(int32_t depth) {
		if(depth > classDepth)
			return nullptr;
		BSItobjSchema *pnode = this;
		while(pnode)
		{
			if(pnode->classDepth == depth)
				return pnode;
			pnode = pnode->pparentClass;
		}
		return pnode;
	}
	/*****************************************************************************************
		<< --- static findCommunityBasecalss	2013-12-12 --- >>
		˵��������ָ��������Ĺ�����������
		������
		pnode1	= ���ȽϽڵ�1
		pnode2	= ���ȽϽڵ�2
		����ֵ����������������
	*****************************************************************************************/
	static BSItobjSchema* findCommunityBasecalss(BSItobjSchema* pnode1, BSItobjSchema *pnode2)
	{
		// �ڵ����ྲ̬�����ģ�����ͬһ����Ľڵ��ַֻ��һ��
		if(pnode1 == pnode2)
			return pnode1;
		if(pnode1 && pnode2)
		{
			// ��λ��ͬһ���
			if(pnode1->classDepth < pnode2->classDepth)
				pnode2 = pnode2->getNodeByDepth(pnode1->classDepth);
			else if(pnode1->classDepth > pnode2->classDepth)
				pnode1 = pnode1->getNodeByDepth(pnode2->classDepth);
			while(pnode1 && pnode2)
			{
				if(pnode1 == pnode2)
					return pnode1;
				pnode1 = pnode1->pparentClass;
				pnode2 = pnode2->pparentClass;
			}
		}
		return nullptr;
	}
}BSItobjSchema;
// �ඨ���ļ���ʹ��
#define _DECLARE_ITOBJ_(class_name) \
public: \
	static BSItobjSchema class##class_name; \
public: \
	virtual BSItobjSchema* getSchemaNode() const { \
		return &class##class_name; \
	} \
	virtual int32_t getType(int32_t depth) const { /*������ȵõ����������*/\
		BSItobjSchema *pnode = &class##class_name; \
		while(pnode) \
		{ \
			if(pnode->classDepth == depth) \
				return pnode->classType; \
			pnode = pnode->pparentClass; \
		} \
		return -1; \
	} \
	virtual int32_t getType() const { \
		return class##class_name.classType; \
	}
// ��ʵ���ļ���ʹ�ã�������ʹ��
#define _IMPLEMENT_ROOT_ITOBJ_(class_root, class_type, class_version) \
	BSItobjSchema class_root::class##class_root = BSItobjSchema( \
	#class_root, class_type, 0, nullptr, nullptr, class_version/*, class_root::allocPropertySectionArray*/); 
// ��ʵ���ļ���ʹ�ã���������ʹ��
#define _IMPLEMENT_ITOBJ_(class_name, base_class_name, class_type, class_version) \
	BSItobjSchema class_name::class##class_name = BSItobjSchema( \
	#class_name, class_type, base_class_name::class##base_class_name.classDepth+1, \
	&class_name::class##class_name, &base_class_name::class##base_class_name, class_version/*, class_name::allocPropertySectionArray*/); 



////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobj	2014-06-07 --- >>
//	��ͼ��Ԫͨ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
// itobj������Ĭ��ֵ
#define _DEFAULT_ITEM_BACKCOLOR_	0xFFFFFFFF	// Ĭ�ϱ���ɫ����ɫ
#define _DEFAULT_ITEM_FORECOLOR_	0xFF000000	// Ĭ��ǰ��ɫ����ɫ
//#define _DEFAULT_ITEM_PENCOLOR_		0xFF000000	// Ĭ�ϻ���ɫ����ɫ
#define _DEFAULT_ITEM_TEXTCOLOR_	0xFF000000	// Ĭ���ı�ɫ����ɫ

class itobj
{
	_DECLARE_ITOBJ_(itobj)
public:
	//typedef gtc::itobj	*PtrType;
	typedef std::vector<itobj *> PtrArrayType;
public:
	enum BETrackerState { TSNormal, TSSelected, TSActive };
	// 
	typedef enum BEItemProp{
		BEItemPropText=1,BEItemPropTextColor,BEItemPropBackColor,BEItemPropForeColor,BEItemPropAutoline,BEItemPropVisible,BEItemPropAlign,
		BEItemPropBorderStyle,
		BEItemPropPen,BEItemPropPenWidth/*,BEItemPropPenColor*/,BEItemPropPenStyle,
		BEItemPropBrush/*,BEItemPropBrushColor*/,BEItemPropBrushStyle,BEItemPropBrushHatch,
		BEItemPropFont,
		BEItemPropPosition,BEItemPropPositionX,BEItemPropPositionY,BEItemPropPositionWidth,BEItemPropPositionHeight,
		BEItemPropGroupflag
	}BEItemProp;
public:
	itobj();
	itobj(rect position);
	itobj(const itobj &item);
	itobj& operator=(const itobj &item);
	virtual ~itobj();
	// ����
	std::wstring description(int indent = 4) const;
public:
	// ��̬��static
	/*****************************************************************************************
		<< --- static itobj::exist	2014-03-02 --- >>
		˵�����鿴ָ�������ڼ������Ƿ����
		������
		datas	= ���󼯺�
		pobj	= �����ҵĶ���
		����ֵ���Ƿ����
	*****************************************************************************************/
	static bool exist(PtrArrayType &datas, itobj* pobj)
	{
		for(PtrArrayType::iterator iter = datas.begin(), end = datas.end(); iter != end; ++iter)
			if(*iter == pobj)
				return true;
		return false;
	}

public:
	/*****************************************************************************************
		<< --- itobj::remove	2013-11-29 --- >>
		˵�����ͷ��Լ�
		������
		����ֵ��
	*****************************************************************************************/
	virtual void remove() {
		delete this;
	}

	/*****************************************************************************************
		<< --- itobj::clone	2013-11-29 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual itobj* clone() {	
		itobj* pitem = new itobj(*this);
		return pitem;
	}
	/*****************************************************************************************
		<< --- itobj::getHandleCount	2013-11-29 --- >>
		˵����������ѡ��ʱ��ʾ�Ķ�λ��������ͨ����λ���������λ�á���С...
		������
		����ֵ����λ����
	*****************************************************************************************/
	virtual int32_t getHandleCount();
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
	virtual point getHandle(int32_t vhandle);
	// ������vrect�Ľ����㷨, ���vpsubobj��Ϊ�վͷ��ض��Ӷ��������
	/*****************************************************************************************
		<< --- itobj::intersects	2013-11-29 --- >>
		˵����������vrect���Ƿ��н���
		������
		vrect	= ����������
		����ֵ���Ƿ��н���
	*****************************************************************************************/
	virtual bool intersects(const rect& vrect)
	{
		rect fixed = m_bsPosition;
		rect rt = vrect;
		//fixed.normalizeRect();
		//rt.normalizeRect();
		return !(rt & fixed);//.isEmptyArea();//.isEmpty();
	}
public:
	//////////////////////////////////////////////////////////////////////////////////////////////
	// WIN32 ����̳�
#if defined(_WIN32)
	// 
	virtual void draw(CDC *pdc);
	/*****************************************************************************************
		<< --- itobj::getHandleCursor	2013-12-11 --- >>
		˵�����õ�ָ����λ����
		������
		nHandle		= ��λ������
		����ֵ����λ����
	*****************************************************************************************/
	virtual HCURSOR getHandleCursor(int nHandle);
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
	virtual int32_t hitTest(CPoint point, acm::CNPainterView *pView, bool bSelected);
	/*****************************************************************************************
		<< --- itobj::drawTracker	2013-12-09 --- >>
		˵�������ƶ�λ����������
		������
		vpdc		= ��ͼ����
		vstate		= ��λ���״̬
		vbkcolor	= ������ɫ
		����ֵ��
	*****************************************************************************************/
	virtual void drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor = 0xFF00FF00);
	/*****************************************************************************************
		<< --- itobj::moveHandleTo	2013-11-30 --- >>
		˵����ͨ��ָ����λ��ı����ߴ�
		������
		nHandle	= �������Ķ�λ������
		point	= ��λ��������
		pView		= ���ƶ�����������ͼ
		����ֵ��
	*****************************************************************************************/
	virtual void moveHandleTo(int nHandle, CPoint point, acm::CNPainterView *pView);
	/*****************************************************************************************
		<< --- itobj::moveTo	2013-11-30 --- >>
		˵����������pobj�Ƶ���λ��
		������
		position	= ��λ��
		pView		= ���ƶ�����������ͼ
		����ֵ��
	*****************************************************************************************/
	virtual void moveTo(const CRect &position, acm::CNPainterView *pView);
	/*****************************************************************************************
		<< --- static itobj::allocPropertySectionArray	2013-12-06 --- >>
		˵���������������
		������
		����ֵ��
	*****************************************************************************************/
	static std::size_t allocPropertySectionArray(itobj* pitem, bool isShare);
	virtual std::size_t allocPropertySectionArray(bool isShare) {
		return allocPropertySectionArray(this, isShare);
	}
	void freePropertys();
	/*****************************************************************************************
		<< --- itobj::updatePropertyValue	2013-12-06 --- >>
		˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
		������
		pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
		����ֵ�����ĳɹ�
	*****************************************************************************************/
	virtual bool updatePropertyValue(itobjProp *pprop);
	/*****************************************************************************************
		<< --- itobj::messageProperty	2013-12-24 --- >>
		˵��������ָ�����Ա�ǵ�����ֵ��������ϢWM_ME_ITEM_PROPERTY_CHANGED���͸����Ա༭����
		������
		typeKey		= �������ͣ��磺BEDrawShapeType
		vecItemKey	= ��Ҫ���µ����Ա��
		����ֵ��
	*****************************************************************************************/
	virtual void messageProperty(int32_t typeKey, std::vector<int32_t> &vecItemKey);

	/*****************************************************************************************
		<< --- itobj::findProperty	2014-07-26 --- >>
		˵��������ָ������
		������
		typeKey		= �������ͣ��磺BEDrawShapeType
		itemKey		= ���Ա��
		����ֵ��
			!=null ���Զ���,����Ϊ��
	*****************************************************************************************/
	itobjProp* findProperty(int32_t typeKey, int32_t itemKey);
	/*****************************************************************************************
		<< --- itobj::existsProperty	2014-09-06 --- >>
		˵��������ָ�������Ƿ����
		������
		pProp		= �����ҵ�����
		����ֵ���Ƿ����
	*****************************************************************************************/
	bool existsProperty(const itobjProp *pProp) const;
#endif
	//// �õ���ʾ����߼����귶Χ????????????
	//virtual rect GetHandleRect(int vhandle, CDC* vpdc);
	//// ������Ŀ??????????????????????
	//virtual void Draw(CDC* vpdc);
	//// ����ѡ�б��????????????????
	//virtual void DrawTracker(CDC* vpdc, BETrackerState vstate, uint32_t vbkcolor = 0xFF00FF00);
	//// ���ض���ѡ�б�ǵ�Ĺ����״??????????????
	//virtual HCURSOR GetHandleCursor(int vhandle);
	//// ͨ���ƶ���ʾ��ı����ĳߴ�??????????????
	//virtual void MoveHandleTo(int vhandle, point vpoint, CView* vpview);
	//// �ı�����λ��??????????????
	//virtual void MoveTo(const rect& vposition, CView* vpview);
	//// ����vpoint���ڵĶ����ʾ�㣬�����ظõ�����??????????????
	//virtual int HitTest(point vpoint, bool visselected, CDC* vpdc);
public:
	//////////////////////////////////////////////////////////////////////////////////////////////
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
	rect getHandleRect(int32_t handle, CDC *pdc);
	rect getHandleRect(int32_t handle, acm::CNPainterView *pview);
#endif

protected:
	//////////////////////////////////////////////////////////////////////////////////////////////
	// WIN32 protected
//#if defined(_WIN32)
//	// 
//	/*****************************************************************************************
//		<< --- itobj::drawBorder	2013-12-23 --- >>
//		˵�������ݱ߽���ʽ�ͻ��ʻ��Ʊ߿�
//		������
//		pdc	= ��ͼ����
//		����ֵ����λ�������߼����꣩
//	*****************************************************************************************/
//	void drawBorder(CDC *pdc);
//
//#endif
public:
	// static
	/*****************************************************************************************
		<< --- static itobj::findCommunityBasecalss	2013-12-12 --- >>
		˵��������ָ��������Ĺ����������ͱ�ʾ
		������
		vecItems	= ��ɸѡ�Ķ�������
		����ֵ����ͼ��������
	*****************************************************************************************/
	static BSItobjSchema* findCommunityBasecalss(PtrArrayType &vecItems);
public:
	std::wstring	getText() const {						return m_wsText;						}
	void			setText(const wchar_t *ptext) {			m_wsText = ptext==nullptr?L"":ptext;	}
	void			setText(const std::wstring &text) {		m_wsText = text;						}
	uint32_t		getBackColor() const {					return m_bsBrush.backColor;				}
	void			setBackColor(uint32_t color) {			m_bsBrush.backColor = color;			}
	uint32_t		getForeColor() const {					return m_bsPen.color;					}
	void			setForeColor(uint32_t color) {			
		m_bsPen.color = color;
		m_bsBrush.foreColor = color;
	}
	uint32_t		getTextColor() const {					return m_clrTextColor;					}
	void			setTextColor(uint32_t color) {			m_clrTextColor = color;					}
	bool			isAutoLine() const {					return m_bAutoLine;						}
	void			setAutoLine(bool isauto) {				m_bAutoLine = isauto;					}
	//int32_t			getFontSize() const {					return m_iFontSize;						}
	//void			setFontSize(int32_t fontsize) {			m_iFontSize = fontsize;					}
	bool			isVisible() const {						return m_bVisible;						}
	void			setVisible(bool visible) {				m_bVisible = visible;					}
	BEAlignMode getAlignMode() const {					return m_beAlign;						}
	void			setAlignMode(BEAlignMode align) {	m_beAlign = align;						}

	BEBorderStyle	getBorderStyle() const {				return m_beBorderStyle;					}
	void			setBorderStyle(BEBorderStyle borderStyle) {
		m_beBorderStyle = borderStyle;
		if(m_beBorderStyle==BEBorderStyleNone)
			m_bsPen.style = BEPenStyleNull; /*NULL*/
	}
	BSPen&			getPen() {								return m_bsPen;							}
	void			setPen(const BSPen &pen) {				m_bsPen = pen;							}
	BSBrush&		getBrush() {							return m_bsBrush;						}
	void			setBrush(const BSBrush &brush) {		m_bsBrush = brush;						}
	BSFont&			getFont() {								return m_bsFont;						}
	void			setFont(const BSFont &font) {			m_bsFont = font;						}
	rect&			getPosition() {							return m_bsPosition;					}
	void			setPosition(const rect &position) {		m_bsPosition = position;				}

	uint32_t		getGroupFlag() const {					return m_uiGroupFlag;					}
	void			setGroupFlag(uint32_t group) {			m_uiGroupFlag = group;					}
	//uint32_t		getZoom() const {						return m_uiZoom;						}
	//void			setZoom(uint32_t zoom) {				m_uiZoom = zoom;						}
	//bool			isVisibleBorder() const {				return m_bBorder;						}
	//void			setBorderVisible(bool visible) {		m_bBorder = visible;					}
	itobjPropSection::PtrArrayType& getPropertys() {		return m_vecPropertySection;			}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_wsText;  
		ar & m_clrTextColor;
		ar & m_bAutoLine;
		ar & m_bVisible;
		ar & m_beAlign;
		ar & m_beBorderStyle;
		ar & m_bsPen;
		ar & m_bsBrush;
		ar & m_bsFont;
		ar & m_bsPosition;
		ar & m_uiGroupFlag;
		//ar & m_vecPropertySection;
	}
#endif	
public:
	static int32_t		mc_iTrackerSize;	// ��λ��Ŀ�͸�7����λ�����أ�
protected:
	std::wstring	m_wsText;		// ��ʾ�ı�
	//uint32_t		m_clrBackColor;	// ����ɫ=��ˢ���ɫ������ȡ���ˣ������Է���������
	//uint32_t		m_clrForeColor;	// ǰ��ɫ=����ɫ�ͻ�ˢ����ɫ������ȡ���ˣ������Է���������
	uint32_t		m_clrTextColor;	// �ı�ɫ
	bool			m_bAutoLine;	// �ı�������Χ���Ƿ��Զ�����
	//int32_t			m_iFontSize;	// ����ĳߴ�
	bool			m_bVisible;		// �����Ƿ�ɼ�
	BEAlignMode		m_beAlign;	// ���䷽ʽ
	
	BEBorderStyle	m_beBorderStyle;// �߿���ʽ
	BSPen			m_bsPen;		// ���ʣ���Ӧ�ڱ߿���ʽ����ɫ�����Ρ�������

	BSBrush			m_bsBrush;		// ��ˢ��ֻ�������������1����ɫ��������ˢ��2����ɫ����
	BSFont			m_bsFont;		// ����
	rect			m_bsPosition;	// ��Χ�ߴ緶Χ

	uint32_t		m_uiGroupFlag;	// ��������־; 0=�޷��飬����Ϊ������
	//uint32_t		m_uiZoom;		// ���ű�����ԭʼ����Ϊ100
	//rect			m_bsOldPosition;// ��ֵ����Ҫ���л�
	//bool			m_bBorder;		// �Ƿ��б߿�

	itobjPropSection::PtrArrayType	m_vecPropertySection;	// ���Է���;��initPropertySectionArray()�г�ʼ��,��Ҫ�ͷ�

};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjRect	2013-11-24 --- >>
//	���ƣ����Σ�Բ�Ǿ��Σ���Բ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjRect : public itobj
{
	_DECLARE_ITOBJ_(itobjRect)
protected:
	itobjRect();
	typedef enum BERectProp {
		BERectPropShape=1,BERectPropRoundness
	}BERectProp;
public:
	enum BEShape { BEShapeRectangle, BEShapeRoundRectangle, BEShapeEllipse, BEShapeLine };	// ���Σ�Բ�Ǿ��Σ���Բ����
	std::wstring toStringBEShape(BEShape shape) const {
		switch (shape)
		{
		case BEShapeRectangle:
			return L"BEShapeRectangle";
			break;
		case BEShapeRoundRectangle:
			return L"BEShapeRoundRectangle";
			break;
		case BEShapeEllipse:
			return L"BEShapeEllipse";
			break;
		case BEShapeLine:
			return L"BEShapeLine";
			break;
		default:
			break;
		}
		return L"";
	}

public:
	itobjRect(const rect& vrect);
	itobjRect(const rect& vrect, BEShape vshape, point vround = point(16, 16 ));

	itobjRect(const itobjRect& vipsql);
	itobjRect& operator =(const itobjRect& vipsql);
	virtual ~itobjRect();
	// ����
	std::wstring description(int indent = 4) const;
	virtual itobj* clone(void){					
		itobj* prect = new itobjRect(*this);
		return prect;
	}

public:
#if defined(_WIN32)
	virtual void draw(CDC *pdc);
	/*****************************************************************************************
		<< --- itobjRect::getHandleCursor	2013-12-11 --- >>
		˵�����õ�ָ����λ����
		������
		nHandle		= ��λ������
		����ֵ����λ����
	*****************************************************************************************/
	virtual HCURSOR getHandleCursor(int nHandle);
	/*****************************************************************************************
		<< --- itobjRect::moveHandleTo	2013-11-30 --- >>
		˵����ͨ��ָ����λ��ı����ߴ�
		������
		nHandle	= �������Ķ�λ������
		point	= ��λ��������
		pView		= ���ƶ�����������ͼ
		����ֵ��
	*****************************************************************************************/
	virtual void moveHandleTo(int nHandle, CPoint point, acm::CNPainterView *pView);
	/*****************************************************************************************
		<< --- itobjRect::drawTracker	2013-12-09 --- >>
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
		<< --- itobjRect::drawTracker	2013-12-09 --- >>
		˵�������ƶ�λ����������
		������
		vpdc		= ��ͼ����
		vstate		= ��λ���״̬
		vbkcolor	= ������ɫ
		����ֵ��
	*****************************************************************************************/
	virtual void drawTracker(CDC* pdc, BETrackerState vstate, uint32_t vbkcolor = 0xFF00FF00);
	/*****************************************************************************************
		<< --- static itobjRect::allocPropertySectionArray	2013-12-06 --- >>
		˵���������������
		������
		����ֵ��
	*****************************************************************************************/
	static std::size_t allocPropertySectionArray(itobj* pitem, bool isShare);
	virtual std::size_t allocPropertySectionArray(bool isShare) {
		return allocPropertySectionArray(this, isShare);
	}
	/*****************************************************************************************
		<< --- itobjRect::updatePropertyValue	2013-12-06 --- >>
		˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
		������
		pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
		����ֵ��
	*****************************************************************************************/
	virtual bool updatePropertyValue(itobjProp *pprop);
#endif
public:
	void						setShape(BEShape shape)						{	m_beShape = shape;				}
	BEShape						getShape(void)								{	return m_beShape;				}
	void						setRoundCorners(point vpoint)				{	m_roundness = vpoint;			}
	point						getRoundCorners(void)						{	return m_roundness;				}

	virtual int32_t getHandleCount();
	virtual point getHandle(int32_t vhandle);
	//virtual HCURSOR GetHandleCursor(int vhandle);
	//virtual void MoveHandleTo(int vhandle, CPoint vpoint, CView* vpview);
	virtual bool intersects(const rect& vrect);

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobj>(*this);
		ar & m_beShape;  
		ar & m_roundness;
	}
#endif
protected:
	BEShape	m_beShape;
	point m_roundness; // for roundRect corners

	//friend class CNToolRect;

};


}
#endif