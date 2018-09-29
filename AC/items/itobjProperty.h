#ifndef __ITOBJPROPERTY_H__
#define __ITOBJPROPERTY_H__
#pragma once
#include "AC/generalobj.h"
#include "AC/baseobj.h"
#include "AC/sysupport.h"
#if defined(_AFXDLL)
	#include <afxpropertygridctrl.h>
#endif

//// ���������л�
//#if defined(BOOST_SERIALIZE)
//	BOOST_CLASS_EXPORT(gtc::itobjPropSection)
//	BOOST_CLASS_EXPORT(gtc::itobjPropSpin)
//	BOOST_CLASS_EXPORT(gtc::itobjPropBoolean)
//	BOOST_CLASS_EXPORT(gtc::itobjPropEdit)
//	BOOST_CLASS_EXPORT(gtc::itobjPropColor)
//	BOOST_CLASS_EXPORT(gtc::itobjPropCombo)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFont)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFolder)
//	BOOST_CLASS_EXPORT(gtc::itobjPropFile)
//#endif


namespace gtc {
//////////////////////////////////////////////////////////////////////////
//	2015-8-2	������С�����λ��
static entry* const gc_tbDecimalPlaces[] = {
	new entry(1, L"1"), 
	new entry(2, L"2"), 
	new entry(3, L"3"), 
	new entry(4, L"4"), 
	new entry(5, L"5"), 
	new entry(6, L"6")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjProp	2013-12-05 --- >>
//	������Ŀ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_wsCaption;  
		ar & m_wsDescription;
		ar & m_iTypekey;
		ar & m_iItemkey;
		ar & m_bEnable;
	}
#endif
public:
	typedef std::vector<itobjProp*> PtrArrayType;
//protected:
	itobjProp(void) : m_wsCaption(L""), m_wsDescription(L""), m_iTypekey(-1), m_iItemkey(-1), m_bEnable(true)
	{
		itobjProp::addMapProperty(this);
	}
public:
	itobjProp(const wchar_t *pcaption, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey, bool enable = true)
		: m_wsCaption(pcaption?pcaption:L""), m_wsDescription(pdescription?pdescription:L"")
		, m_iTypekey(typeKey), m_iItemkey(itemKey)
		, m_bEnable(enable)
	{
		itobjProp::addMapProperty(this);
	}
	itobjProp(const itobjProp &prop)
		: m_wsCaption(prop.m_wsCaption), m_wsDescription(prop.m_wsDescription)
		, m_iTypekey(prop.m_iTypekey), m_iItemkey(prop.m_iItemkey)
		, m_bEnable(prop.m_bEnable)
	{
		itobjProp::addMapProperty(this);
	}
	itobjProp& operator=(const itobjProp &prop)
	{
		if(this != &prop)
		{
			m_wsCaption = prop.m_wsCaption;
			m_wsDescription = prop.m_wsDescription;
			m_iTypekey = prop.m_iTypekey;
			m_iItemkey = prop.m_iItemkey;
			m_bEnable = prop.m_bEnable;
		}
		return *this;
	}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjProp(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		gtc::outClassBegin(os, L"itobjProp", indent);
		gtc::outClassItem(os, L"����", m_wsCaption, indent << 1);
		gtc::outClassItem(os, L"����", m_wsDescription, indent << 1);
		gtc::outClassItem(os, L"���ͼ�", m_iTypekey, indent << 1);
		gtc::outClassItem(os, L"��Ŀ��", m_iItemkey, indent << 1);
		gtc::outClassEnd(os, L"itobjProp", indent);
		return os.str();
	}


	virtual ~itobjProp(void) {
		itobjProp::removeMapProperty(this);
	}

public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjProp::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjProp::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		return COleVariant();
	}
	/*****************************************************************************************
		<< --- itobjProp::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){

	}

#endif
public:
	// static 
	/*****************************************************************************************
		<< --- static itobjProp::cloneArray	2013-12-06 --- >>
		˵�������ƶ���
		������
		����ֵ��
	*****************************************************************************************/
	static PtrArrayType cloneArray(const PtrArrayType &arrProp)
	{
		PtrArrayType arr;
		//for(PtrArrayType::size_type idx = 0; idx < arrProp.size(); ++idx)
		//	arr.push_back(arrProp[idx]->clone());
		for(PtrArrayType::const_iterator citer = arrProp.begin(), cend = arrProp.end(); citer != cend; ++citer)
			arr.push_back((*citer)->clone());
		return arr;
	}

	/*****************************************************************************************
		<< --- static itobjProp::findProperty	2013-12-06 --- >>
		˵��������ָ������Ӧ��������
		������
		����ֵ���ҵ���������
	*****************************************************************************************/
	static itobjProp * findProperty(int32_t typeKey, int32_t itemKey, PtrArrayType arrPropertys)
	{
		//for(PtrArrayType::size_type idx = 0; idx < arrPropertys.size(); ++idx)
		//	if(arrPropertys[idx]->getTypekey() == typeKey && arrPropertys[idx]->getItemkey() == itemKey)
		//		return arrPropertys[idx];
		for(PtrArrayType::const_iterator iter = arrPropertys.cbegin(), end = arrPropertys.cend(); iter != end; ++iter)
			if((*iter)->getTypekey() == typeKey && (*iter)->getItemkey() == itemKey)
				return *iter;
		return nullptr;
	}
public:
	// ����
	std::wstring	getCaption() const {								return m_wsCaption;								}
	void			setCaption(const wchar_t *pcaption) {				m_wsCaption = pcaption?pcaption:L"";			}
	std::wstring	getDescription() const {							return m_wsDescription;							}
	void			setDescription(const wchar_t *pdesc) {				m_wsDescription = pdesc?pdesc:L"";				}
	int32_t			getTypekey() const {								return m_iTypekey;								}
	void			setTypekey(int32_t typekey) {						m_iTypekey = typekey;							}
	int32_t			getItemkey() const {								return m_iItemkey;								}
	void			setItemkey(int32_t itemkey) {						m_iItemkey = itemkey;							}
	bool			isEnable() const {									return m_bEnable;								}
	void			enable(bool isEnable) {								m_bEnable = isEnable;							}

public:
	// �����������itobjProp::allocProperty��ʱ CMFCPropertyGridProperty::Data=itobjProp*
	// Ϊ��ͨ��CMFCPropertyGridProperty::Data�����ҵ��������������map
	static std::map<int64_t, itobjProp *>	mc_mapProperty;
	static void addMapProperty(itobjProp *pprop);
	static void removeMapProperty(itobjProp *pprop);
	static itobjProp* findPropertyItem(int32_t typeKey, int32_t itemKey);
protected:
	std::wstring	m_wsCaption;		// ����
	std::wstring	m_wsDescription;	// ��ע
	int32_t			m_iTypekey;			// ����key,BEDrawShapeType
	int32_t			m_iItemkey;			// �����ڲ�������Ŀkey��ÿ��itobj���������඼Ҫ����һ��
	bool			m_bEnable;			// CMFCPropertyGridProperty::Enable
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropSection	2013-12-05 --- >>
//	���Է���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropSection : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_bValueList;
		ar & m_bExpand;
		ar & m_vecPropertys;
	}
#endif
public:
	typedef std::vector<itobjPropSection *> PtrArrayType;
public:
	itobjPropSection() 
		: itobjProp()
		, m_bValueList(false)
		, m_bExpand(true)
	{
	}
	itobjPropSection(const wchar_t *pcaption, const wchar_t *pdescription = nullptr, bool valueList = false, bool isExpand = true)
		: itobjProp(pcaption, pdescription, 0, 0), m_bValueList(valueList), m_bExpand(isExpand)
	{
	}
	itobjPropSection(const itobjPropSection &section)
		: itobjProp(section)
		, m_bValueList(section.m_bValueList)
		, m_bExpand(section.m_bExpand)
	{
		m_vecPropertys = itobjProp::cloneArray(section.m_vecPropertys);
	}
	itobjPropSection& operator=(const itobjPropSection &section)
	{
		if(this != &section)
		{
			itobjProp::operator=(section);
			m_bValueList = section.m_bValueList;
			m_bExpand = section.m_bExpand;
			m_vecPropertys = itobjProp::cloneArray(section.m_vecPropertys);
		}
		return *this;
	}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropSection(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		gtc::outClassBegin(os, L"itobjPropSection", indent);
		gtc::outClassItem(os, L"ֵ��", m_bValueList, indent << 1);
		itobjProp::PtrArrayType::size_type size = m_vecPropertys.size();
		gtc::outClassItem(os, L"������", size, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropSection", indent);
		return os.str();
	}
	virtual ~itobjPropSection() {
		//for(itobjProp::PtrArrayType::size_type idx = 0; idx < m_vecPropertys.size(); ++idx)
		//{
		//	delete m_vecPropertys[idx];
		//}
		for(itobjProp::PtrArrayType::iterator iter = m_vecPropertys.begin(), end = m_vecPropertys.end(); iter != end; ++iter)
			delete *iter;
		m_vecPropertys.clear();

	}
public:
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropSection::allocProperty	2013-12-05 --- >>
		˵�������������飬������������
		������
		����ֵ�������������
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty(){
		CMFCPropertyGridProperty* pgroup = new CMFCPropertyGridProperty(m_wsCaption.c_str(), 0, m_bValueList);
		pgroup->Enable(m_bEnable);
		pgroup->SetData(DWORD_PTR(this));
		CMFCPropertyGridProperty* psub = nullptr;
		//for(itobjProp::PtrArrayType::size_type idx = 0; idx < m_vecPropertys.size(); ++idx)
		//{
		//	if((psub = m_vecPropertys[idx]->allocProperty()) != nullptr)
		//	{
		//		pgroup->AddSubItem(psub);
		//	}
		//}
		for(itobjProp::PtrArrayType::iterator iter = m_vecPropertys.begin(), end = m_vecPropertys.end(); iter != end; ++iter)
		{
			if((psub = (*iter)->allocProperty()) != nullptr)
			{
				pgroup->AddSubItem(psub);
			}

		}

		pgroup->Expand(m_bExpand);
		return pgroup;
	}

#endif
public:
	/*****************************************************************************************
		<< --- itobjPropSection::addProperty	2013-12-05 --- >>
		˵��������������һ������
		������
		pproperty	= ������ָ��
		����ֵ��
	*****************************************************************************************/
	void addProperty(itobjProp* pproperty) {
		if(pproperty)
			m_vecPropertys.push_back(pproperty);
	}
	/*****************************************************************************************
		<< --- itobjPropSection::existsProperty	2014-09-06 --- >>
		˵��������ָ�������Ƿ����
		������
		pProp		= �����ҵ�����
		����ֵ���Ƿ����
	*****************************************************************************************/
	bool existsProperty(const itobjProp *pProp) const;
public:
	/*****************************************************************************************
		<< --- static itobjPropSection::findProperty	2013-12-05 --- >>
		˵��������������һ������
		������
		pproperty	= ������ָ��
		����ֵ��
	*****************************************************************************************/
	static itobjProp* findProperty(int32_t typeKey, int32_t itemKey, PtrArrayType &arrSections) {
		itobjProp *pitem = nullptr;
		//for(PtrArrayType::size_type idx = 0; idx < arrSections.size(); ++idx)
		//{
		//	pitem = itobjProp::findProperty(typeKey, itemKey, arrSections[idx]->getPropertys());
		//	if(pitem)
		//		return pitem;
		//}
		for(PtrArrayType::iterator iter = arrSections.begin(), end = arrSections.end(); iter != end; ++iter)
		{
			if((pitem = itobjProp::findProperty(typeKey, itemKey, (*iter)->getPropertys())) != nullptr)
				return pitem;
		}

		return nullptr;
	}
public:
	itobjProp::PtrArrayType& getPropertys() {
		return m_vecPropertys;
	}

protected:
	bool			m_bValueList;	// 
	bool			m_bExpand;		// �Ƿ���չ�Ա���ʾ����
	itobjProp::PtrArrayType	m_vecPropertys;	// ������
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropSpin	2013-12-05 --- >>
//	������Ŀ������ȡֵ��Χ��int������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropSpin : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		//boost::serialization::void_cast_register<itobjPropSpin, itobjProp>();
		ar & m_iValue;  
		ar & m_iMin;
		ar & m_iMax;
	}
#endif
public:
	itobjPropSpin()
		: itobjProp(), m_iValue(0), m_iMin(0), m_iMax(0)
	{
	}
	itobjPropSpin(const wchar_t *pcaption, int32_t value, int32_t min, int32_t max, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_iValue(value), m_iMin(min), m_iMax(max)
	{
	}
	itobjPropSpin(const itobjPropSpin &spin)
		: itobjProp(spin)
		, m_iValue(spin.m_iValue), m_iMin(spin.m_iMin), m_iMax(spin.m_iMax)
	{
	}
	itobjPropSpin& operator=(const itobjPropSpin &spin)
	{
		if(this != &spin)
		{
			itobjProp::operator=(spin);
			m_iValue = spin.m_iValue;
			m_iMin = spin.m_iMin;
			m_iMax = spin.m_iMax;
		}
		return *this;
	}
	virtual ~itobjPropSpin() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropSpin(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%d[%d,%d]", m_iValue, m_iMin, m_iMax);
		gtc::outClassBegin(os, L"itobjPropSpin", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropSpin", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropSpin::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropSpin::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		COleVariant var;
		if(isMFCProp)
		{	// // CMFCPropertyGridProperty�е�Value����VT_INT
			var.ChangeType(VT_INT);
			var.intVal = m_iValue;
		}
		else
		{
			var.ChangeType(VT_I4);
			var.lVal = m_iValue;
		}

		return var;
	}
	/*****************************************************************************************
		<< --- itobjPropSpin::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_I4);
			if(value.lVal >= m_iMin && value.lVal <= m_iMax)
				m_iValue = value.lVal;
		}
	}

#endif


protected:
	int32_t	m_iValue;	// ����ֵ
	int32_t	m_iMax;		// ��������[min��max]
	int32_t	m_iMin;		// ��������[min��max]
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropBoolean	2013-12-05 --- >>
//	������Ŀ��bool������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropBoolean : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		//boost::serialization::void_cast_register<itobjPropBoolean, itobjProp>();
		ar & m_bValue;
	}
#endif
protected:
public:
	itobjPropBoolean()
		: itobjProp(), m_bValue(true)
	{
	}
	itobjPropBoolean(const wchar_t *pcaption, bool value, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription, typeKey, itemKey)
		, m_bValue(value)
	{
	}
	itobjPropBoolean(const itobjPropBoolean &prop)
		: itobjProp(prop)
		, m_bValue(prop.m_bValue)
	{
	}
	itobjPropBoolean& operator=(const itobjPropBoolean &prop)
	{
		if(this != &prop)
		{
			itobjProp::operator=(prop);
			m_bValue = prop.m_bValue;
		}
		return *this;
	}
	virtual ~itobjPropBoolean() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropBoolean(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%s", m_bValue?L"true":L"false");
		gtc::outClassBegin(os, L"itobjPropBoolean", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropBoolean", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropBoolean::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropBoolean::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		COleVariant var;
		var.ChangeType(VT_BOOL);
		var.boolVal = m_bValue?VARIANT_TRUE:VARIANT_FALSE;
		return var;
	}
	/*****************************************************************************************
		<< --- itobjPropBoolean::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_BOOL);
			m_bValue = value.boolVal == VARIANT_TRUE;
		}
	}

#endif

protected:
	bool	m_bValue;	// ����ֵ
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropEdit	2013-12-05 --- >>
//	������Ŀ���ı�������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropEdit : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_wsText;
		ar & m_bEnable;
	}
#endif
protected:
public:
	itobjPropEdit()
		: itobjProp(), m_wsText(L""), m_bEnable(false)
	{
	}
	itobjPropEdit(const wchar_t *pcaption, const wchar_t *ptext, bool enable, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_wsText(ptext?ptext:L""), m_bEnable(enable)
	{
	}
	itobjPropEdit(const itobjPropEdit &edit)
		: itobjProp(edit)
		, m_wsText(edit.m_wsText), m_bEnable(edit.m_bEnable)
	{
	}
	itobjPropEdit& operator=(const itobjPropEdit &edit)
	{
		if(this != &edit)
		{
			itobjProp::operator=(edit);
			m_wsText = edit.m_wsText;
			m_bEnable = edit.m_bEnable;
		}
		return *this;
	}
	virtual ~itobjPropEdit() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropEdit(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%s enable:%s", m_wsText.c_str(), m_bEnable?L"true":L"false");
		gtc::outClassBegin(os, L"itobjPropEdit", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropEdit", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropEdit::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropEdit::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		return COleVariant(m_wsText.c_str(), VT_BSTR);
	}
	/*****************************************************************************************
		<< --- itobjPropEdit::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_BSTR);
			m_wsText = _bstr_t(value.bstrVal);
		}
	}

#endif


protected:
	std::wstring	m_wsText;	// ����ֵ
	bool			m_bEnable;	// ����editbox��enable����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropColor	2013-12-05 --- >>
//	������Ŀ����ɫ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropColor : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_uiColor;
		ar & m_uiDefault;
	}
#endif
protected:
public:
	itobjPropColor()
		: itobjProp(), m_uiColor(0xFF000000), m_uiDefault(0x0)
	{
	}
	itobjPropColor(const wchar_t *pcaption, uint32_t color, uint32_t defaultColor, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_uiColor(color), m_uiDefault(defaultColor)
	{
	}
	itobjPropColor(const itobjPropColor &color)
		: itobjProp(color)
		, m_uiColor(color.m_uiColor), m_uiDefault(color.m_uiDefault)
	{
	}
	itobjPropColor& operator=(const itobjPropColor &color)
	{
		if(this != &color)
		{
			itobjProp::operator=(color);
			m_uiColor = color.m_uiColor;
			m_uiDefault = color.m_uiDefault;
		}
		return *this;
	}
	virtual ~itobjPropColor() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropColor(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%08X Ĭ��ɫ��%08X", m_uiColor, m_uiDefault);
		gtc::outClassBegin(os, L"itobjPropColor", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropColor", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropColor::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropColor::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		COleVariant var;
		if(isMFCProp)
		{
			// CMFCPropertyGridProperty�е�Value����VT_I4
			var.ChangeType(VT_I4);	
			var.lVal = int32_t(m_uiColor);
		}
		else
		{
			var.ChangeType(VT_UI4);
			var.ulVal = m_uiColor | 0xFF000000;
		}
		return var;
	}
	/*****************************************************************************************
		<< --- itobjPropColor::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_UI4);
			m_uiColor = value.ulVal;
		}
	}

#endif

protected:
	uint32_t	m_uiColor;		// ����ֵ
	uint32_t	m_uiDefault;	// Ĭ��ɫ
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropCombo	2013-12-05 --- >>
//	������Ŀ���б������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropCombo : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_iID;
		ar & m_bInterFree;
		ar & m_vecItems;
	}
#endif
protected:
public:
	itobjPropCombo()
		: itobjProp(), m_iID(-1), m_bInterFree(false)
	{
	}
	itobjPropCombo(const wchar_t *pcaption, INT_PTR value, const entry::PtrArrayType &items, bool binterFree, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_iID(value), m_vecItems(items)
		, m_bInterFree(binterFree)
	{
	}
	itobjPropCombo(const itobjPropCombo &combo)
		: itobjProp(combo)
		, m_iID(combo.m_iID), m_vecItems(combo.m_vecItems)
		, m_bInterFree(combo.m_bInterFree)
	{
		if(combo.m_bInterFree) m_vecItems = entry::clone(combo.m_vecItems);
		else m_vecItems = combo.m_vecItems;
	}
	itobjPropCombo& operator=(const itobjPropCombo &combo)
	{
		if(this != &combo)
		{
			itobjProp::operator=(combo);
			m_iID = combo.m_iID;
			setItemDatas(combo.m_vecItems, combo.m_bInterFree);
		}
		return *this;
	}
	virtual ~itobjPropCombo() 
	{
		if(m_bInterFree) entry::freeEntry(m_vecItems);
	}
	virtual itobjProp* clone()
	{	
		itobjProp *pitem = new itobjPropCombo(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%d", m_iID);
		gtc::outClassBegin(os, L"itobjPropCombo", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropCombo", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropCombo::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropCombo::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		COleVariant var;
		if(isMFCProp)
		{
			if(const entry *pentry = entry::findEntry(m_iID, m_vecItems))
			{
				var = pentry->getName().c_str();
			}
		}
		else
		{
			var.ChangeType(VT_INT);
			var.intVal = m_iID;
		}
		return var;
	}
	/*****************************************************************************************
		<< --- itobjPropCombox::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			if(value.vt == VT_BSTR)
			{
				const entry *pentry = entry::findEntry((wchar_t*)_bstr_t(value.bstrVal), m_vecItems);
				if(pentry)
					m_iID = pentry->getID();
			}
			else
			{
				value.ChangeType(VT_INT);
				const entry *pentry = entry::findEntry(value.intVal, m_vecItems);
				if(pentry)
					m_iID = value.intVal;
			}
		}
	}

#endif
public:
	void setItemDatas(const entry::PtrArrayType &vecItems, bool binterFree)
	{
		if(m_bInterFree) {
			entry::freeEntry(m_vecItems);
		}
		m_bInterFree = binterFree;
		/*if(binterFree) m_vecItems = entry::clone(vecItems);
		else */m_vecItems = vecItems;
	}

protected:
	INT_PTR		m_iID;				// ����ֵ
	entry::PtrArrayType m_vecItems;	// combobox��Ŀ����
	bool		m_bInterFree;		// �Ƿ����ڲ��ͷ�m_vecItems
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropFont	2013-12-06 --- >>
//	������Ŀ���б������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropFont : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_bsFont;
	}
#endif
protected:
public:
	itobjPropFont()
		: itobjProp(), m_bsFont(L"����")
	{
	}
	itobjPropFont(const wchar_t *pcaption, BSFont &font, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_bsFont(font)
	{
	}
	itobjPropFont(const itobjPropFont &font)
		: itobjProp(font)
		, m_bsFont(font.m_bsFont)
	{
	}
	itobjPropFont& operator=(const itobjPropFont &font)
	{
		if(this != &font)
		{
			itobjProp::operator=(font);
			m_bsFont = font.m_bsFont;
		}
		return *this;
	}
	virtual ~itobjPropFont() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropFont(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%s", m_bsFont.wcFaceName);
		gtc::outClassBegin(os, L"itobjPropFont", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropFont", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropFont::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropFont::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		COleVariant var;
		if(isMFCProp)
		{
			var.ChangeType(VT_BSTR);
			var.bstrVal = _bstr_t(m_bsFont.wcFaceName);
		}
		else
		{
			convert::toVariant(var, m_bsFont.toData());
		}

		return var;
	}
	/*****************************************************************************************
		<< --- itobjPropCombox::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			m_bsFont.fromData(convert::toData(value));
		}
	}

#endif


protected:
	BSFont	m_bsFont;	// 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropFolder	2013-12-06 --- >>
//	������Ŀ���ļ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropFolder : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_wsFolder;
	}
#endif
protected:
public:
	itobjPropFolder()
		: itobjProp(), m_wsFolder(L"")
	{
	}
	itobjPropFolder(const wchar_t *pcaption, const wchar_t *value, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_wsFolder(value?value:L"")
	{
	}
	itobjPropFolder(const itobjPropFolder &folder)
		: itobjProp(folder)
		, m_wsFolder(folder.m_wsFolder)
	{
	}
	itobjPropFolder& operator=(const itobjPropFolder &folder)
	{
		if(this != &folder)
		{
			itobjProp::operator=(folder);
			m_wsFolder = folder.m_wsFolder;
		}
		return *this;
	}
	virtual ~itobjPropFolder() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropFolder(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%s", m_wsFolder);
		gtc::outClassBegin(os, L"itobjPropFolder", indent);
		gtc::outClassItem(os, L"����ֵ", buf, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropFolder", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropFolder::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropFolder::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		return COleVariant(m_wsFolder.c_str(), VT_BSTR);
	}
	/*****************************************************************************************
		<< --- itobjPropFolder::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_BSTR);
			m_wsFolder = _bstr_t(value.bstrVal);
		}
	}

#endif


protected:
	std::wstring m_wsFolder; 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjPropFile	2013-12-06 --- >>
//	������Ŀ���ļ�ѡ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjPropFile : public itobjProp
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobjProp>(*this);
		ar & m_wsFile;
		ar & m_wsFilter;
		ar & m_wsDefaultExt;
		ar & m_bToData;
	}
#endif
protected:
public:
	itobjPropFile()
		: itobjProp(), m_wsFile(L""), m_wsFilter(L""), m_wsDefaultExt(L""), m_bToData(false)
	{
	}
	itobjPropFile(const wchar_t *pcaption, const wchar_t *pfile, const wchar_t *pdefext, const wchar_t *pfilter, bool istoData, const wchar_t *pdescription, int32_t typeKey, int32_t itemKey)
		: itobjProp(pcaption, pdescription,typeKey,itemKey)
		, m_wsFile(pfile?pfile:L""), m_wsFilter(pfilter?pfilter:L""), m_wsDefaultExt(pdefext?pdefext:L""), m_bToData(istoData)
	{
	}
	itobjPropFile(const itobjPropFile &file)
		: itobjProp(file)
		, m_wsFile(file.m_wsFile), m_wsFilter(file.m_wsFilter), m_wsDefaultExt(file.m_wsDefaultExt), m_bToData(file.m_bToData)
	{
	}
	itobjPropFile& operator=(const itobjPropFile &file)
	{
		if(this != &file)
		{
			itobjProp::operator=(file);
			m_wsFile = file.m_wsFile;
			m_wsFilter = file.m_wsFilter;
			m_wsDefaultExt = file.m_wsDefaultExt;
			m_bToData = file.m_bToData;
		}
		return *this;
	}
	virtual ~itobjPropFile() {}
	virtual itobjProp* clone() {	
		itobjProp *pitem = new itobjPropFile(*this);
		return pitem;
	}
	// ����
	std::wstring description(int indent = 4) const {
		std::wostringstream os;
		wchar_t buf[128] = {0};
		std::swprintf(buf, 128, L"%s %s", m_wsDefaultExt, m_wsFilter);
		gtc::outClassBegin(os, L"itobjPropFile", indent);
		gtc::outClassItem(os, L"����ֵ", m_wsFile, indent << 1);
		gtc::outBaseClass(os, *(itobjProp*)this, indent << 1);
		gtc::outClassEnd(os, L"itobjPropFile", indent);
		return os.str();
	}
public:
	// ��̬����
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjPropFile::allocProperty	2013-12-05 --- >>
		˵���������ض����͵�������Ŀ���磺�ı��༭�͡������͡��б��͡������͡������͡��ļ���...
		������
		����ֵ���ض����͵�������Ŀ
	*****************************************************************************************/
	virtual CMFCPropertyGridProperty* allocProperty();
	/*****************************************************************************************
		<< --- itobjPropFile::getValue	2013-12-05 --- >>
		˵�����õ�������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		isMFCProp	= ��CMFCPropertyGridProperty����������ֵ
		����ֵ����ǰ�����ʵ��ֵ
	*****************************************************************************************/
	virtual COleVariant getValue(bool isMFCProp = false) {
		return COleVariant(m_wsFile.c_str(), VT_BSTR);
	}
	/*****************************************************************************************
		<< --- itobjPropFile::setValue	2013-12-05 --- >>
		˵��������������Ŀ��Ӧ��ʵ��ֵ���磺�����б������ԣ�����ֵΪ�ַ�������ʵ��Ҫ�õ�����IDֵ
		������
		����ֵ��
	*****************************************************************************************/
	virtual void setValue(COleVariant &value){
		if(value.vt > 1)
		{
			value.ChangeType(VT_BSTR);
			m_wsFile = _bstr_t(value.bstrVal);
		}
	}

#endif

protected:
	std::wstring	m_wsFile;		// �ļ�����
	std::wstring	m_wsDefaultExt;	// ȱʡ�ļ�������չ��
	std::wstring	m_wsFilter;		// �ļ����͹���
	bool			m_bToData;		// true=�ļ����ֽ����飨std::vector<uint8_t>����ʽ���棬�������ļ���ȫ·������
};
//#if defined(BOOST_SERIALIZE)
//
//BOOST_CLASS_EXPORT_GUID(itobjPropSpin, "gtc::itobjPropSpin")
//
//#endif
}	// gtc

//BOOST_SERIALIZATION_ASSUME_ABSTRACT(gtc::itobjProp)
//BOOST_CLASS_EXPORT_GUID(gtc::itobjPropSection, "gtc::itobjPropSection")
//BOOST_CLASS_EXPORT_GUID(gtc::itobjPropSpin, "gtc::itobjPropSpin")
//BOOST_CLASS_EXPORT_GUID(gtc::itobjPropBoolean, "gtc::itobjPropBoolean")

#endif