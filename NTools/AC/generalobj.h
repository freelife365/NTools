#ifndef __GENERALOBJ_H__
#define __GENERALOBJ_H__
#pragma once
#include "AC/basedef.h"

#if defined(BOOST_SERIALIZE)
#include "../boost/serialization/base_object.hpp"

#endif
namespace gtc{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	entry
////////////////////////////////////////////////////////////////////////////////////////////////////////
class entry
{
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_lID;
		ar & m_wsName;
		ar & m_wsDesc;
	}
#endif
public:
	typedef std::vector<entry> ArrayType;
	typedef std::vector<entry*> PtrArrayType;
	//typedef std::vector<const entry*> CPtrArrayType;
public:
	entry(void)
		: m_lID(-1)
	{
	}
	virtual ~entry(void)
	{
	}
	entry(int32_t vid, const wchar_t *vpname, const wchar_t *pdesc = NULL)
		: m_lID(vid)
		, m_wsName(vpname?vpname:L""), m_wsDesc(pdesc?pdesc:L"")
	{
	}
	entry(const wchar_t *vpname, int32_t vid, const wchar_t *pdesc = NULL)
		: m_lID(vid)
		, m_wsName(vpname?vpname:L""), m_wsDesc(pdesc?pdesc:L"")
	{
	}
	entry(const entry &vitem)
		: m_lID(vitem.m_lID)
		, m_wsName(vitem.m_wsName), m_wsDesc(vitem.m_wsDesc)
	{
	}
	entry& operator=(const entry &vitem)
	{
		if (this != &vitem)
		{
			m_lID = vitem.m_lID;
			m_wsName = vitem.m_wsName;
			m_wsDesc = vitem.m_wsDesc;
		}
		return *this;
	}
	virtual entry* clone()
	{
		return new entry(*this);
	}
	// ����
	std::wstring toString() const
	{
		std::wostringstream out;
		out << L"ID��" << m_lID << L"  ���ƣ�" << m_wsName << L"  ������" << m_wsDesc << std::endl;
		return out.str();
	}
	virtual std::wstring getTitle() const {
		return m_wsName;
	}

public:
	/*****************************************************************************************
		<< --- static entry::findEntry	2013-12-05 --- >>
		˵��������id������fromEntrys�в���ָ������
		������
		id			= �����ҵ�id
		fromEntrys	= ����������Դ
		����ֵ��
			= �ա�Ϊ�ҵ�
			= �����ҵ�����ĵ�ַ
	*****************************************************************************************/
	static entry* findEntry(INT_PTR id, ArrayType &fromEntrys)
	{
		for(std::vector<entry>::size_type idx = 0; idx < fromEntrys.size(); ++idx)
			if(fromEntrys[idx].getID() == id)
				return &fromEntrys[idx];
		return nullptr;
	}
	static entry* findEntry(INT_PTR id, PtrArrayType &fromEntrys)
	{
		for(PtrArrayType::size_type idx = 0; idx < fromEntrys.size(); ++idx)
			if(fromEntrys[idx]->getID() == id)
				return fromEntrys[idx];
		return nullptr;
	}
	//static const entry* findEntry(uint32_t id, CPtrArrayType &fromEntrys)
	//{
	//	for(CPtrArrayType::size_type idx = 0; idx < fromEntrys.size(); ++idx)
	//		if(fromEntrys[idx]->getID() == id)
	//			return fromEntrys[idx];
	//	return nullptr;
	//}
	/*****************************************************************************************
		<< --- static entry::findEntry	2013-12-05 --- >>
		˵������������������fromEntrys�в���ָ������
		������
		id			= �����ҵ�id
		fromEntrys	= ����������Դ
		����ֵ��
			= �ա�Ϊ�ҵ�
			= �����ҵ�����ĵ�ַ
	*****************************************************************************************/
	static entry* findEntry(std::wstring sname, ArrayType &fromEntrys)
	{
		for(std::vector<entry>::size_type idx = 0; idx < fromEntrys.size(); ++idx)
			if(fromEntrys[idx].getName() == sname)
				return &fromEntrys[idx];
		return nullptr;
	}
	static entry* findEntry(std::wstring sname, PtrArrayType &fromEntrys)
	{
		for(PtrArrayType::size_type idx = 0; idx < fromEntrys.size(); ++idx)
			if(fromEntrys[idx]->getName() == sname)
				return fromEntrys[idx];
		return nullptr;
	}
	//static const entry* findEntry(std::wstring sname, CPtrArrayType &fromEntrys)
	//{
	//	for(CPtrArrayType::size_type idx = 0; idx < fromEntrys.size(); ++idx)
	//		if(fromEntrys[idx]->getName() == sname)
	//			return fromEntrys[idx];
	//	return nullptr;
	//}
	static void freeEntry(PtrArrayType &entrys)
	{
		for (PtrArrayType::iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
		{
			delete *iter;
		}
		entrys.clear();
	}
	static PtrArrayType clone(const PtrArrayType &entrys)
	{
		PtrArrayType vec;
		for (PtrArrayType::const_iterator iter = entrys.cbegin(); iter != entrys.cend(); ++iter)
		{
			vec.push_back((*iter)->clone());
		}
		return vec;
	}
public:
	// 
	INT_PTR getID(void) const {						return m_lID;							}
	void setID(INT_PTR vid) {						m_lID = vid;							}
	const std::wstring& getName(void) const {		return m_wsName;						}
	void setName(const std::wstring& vname) {		m_wsName = vname;						}
	void setName(const wchar_t *vpname) {			m_wsName = vpname?vpname:L"";			}
	const std::wstring& getDesc() const {			return m_wsDesc;						}
	void setDesc(const std::wstring &vdesc) {		m_wsDesc = vdesc;						}
protected:
	INT_PTR			m_lID;
	std::wstring	m_wsName;
	std::wstring	m_wsDesc;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gridColHeader��		2014-10-18
//	���ؼ����б���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gridColHeader
{
public:
	typedef std::vector<gridColHeader *>	PtrArrayType;
public:
	gridColHeader();
	gridColHeader(const wchar_t *vname, const wchar_t *vparname, const wchar_t *vfldname, const wchar_t *vidfldname, 
		BEDataType vtype, BEAlignMode valign, int vwidth);
	gridColHeader(const wchar_t *vname, const wchar_t *vfldname,
		BEDataType vtype, BEAlignMode valign, int vwidth);
	gridColHeader(const gridColHeader& header);
	gridColHeader& operator =(const gridColHeader& header);
	// ����
	std::wstring toString() const;
	virtual ~gridColHeader();
public:
	static void free(PtrArrayType &headers);

public:
	// ����
	std::wstring getName(void) const {				return m_strName;			}
	void setName(const wchar_t *vpname) {			m_strName = vpname?vpname:L"";			}
	std::wstring getParentName(void) const {		return m_strParName;		}
	void setParentName(const wchar_t *vpparent) {	m_strParName = vpparent?vpparent:L"";	}
	std::wstring getFieldName(void) const {			return m_strFldName;		}
	void setFieldName(const wchar_t *vpfieldname) {	m_strFldName = vpfieldname?vpfieldname:L"";	}
	int32_t getFieldIndex(void) const {				return m_lFldIndex;			}
	void setFieldIndex(int32_t vindex) {			m_lFldIndex = vindex;		}
	std::wstring getTableName(void) const {			return m_strTableName;		}
	void setTableName(const wchar_t *vptable) {		m_strTableName = vptable?vptable:L"";	}
	BEDataType getDataType(void) const {			return m_beDataType;			}
	void setDataType(BEDataType vtype) {			m_beDataType = vtype;		}
	BEAlignMode getAlign(void) const {				return m_beAlign;			}
	void setAlign(BEAlignMode valign) {				m_beAlign = valign;			}
	int32_t getWidth(void) const {					return m_iWidth;			}
	void setWidth(int32_t vwidth) {					m_iWidth = vwidth<0?0:vwidth;			}
	int32_t getDecimals(void) const {				return m_iDecimals;						}
	void setDecimals(int32_t vdecimals) {			m_iDecimals = vdecimals<0?0:vdecimals;	}
	bool isVisible(void) const {					return m_bVisible;			}
	void setVisible(bool vvisible) {				m_bVisible = vvisible;		}
	bool isSaved(void) const {						return m_bIsSaved;			}
	void setSaved(bool vsaved) {					m_bIsSaved = vsaved;		}
	BEComputeMethod getComputedFlag(void) const {		return m_beComputed;		}
	void setComputedFlag(BEComputeMethod vcomputed) {	m_beComputed = vcomputed;		}
	bool isLocked(void) const {						return m_bIsLocked;			}
	void setLocked(bool vlocked) {					m_bIsLocked = vlocked;		}
	bool isMerged(void) const {						return m_bIsMerged;			}
	void setMerged(bool vmerged) {					m_bIsMerged = vmerged;		}
	bool isMustWrited(void) const {					return m_bIsMustWrited;		}
	void setMustWrited(bool vwrited) {				m_bIsMustWrited = vwrited;	}
	bool isFind(void) const {						return m_bIsFind;			}
	void setFind(bool vfind) {						m_bIsFind = vfind;			}
	bool isOnlySelected(void) const {				return m_bIsOnlySelected;	}
	void setOnlySelected(bool visonly) {			m_bIsOnlySelected = visonly;}
	std::wstring getIDFieldName(void) const {		return m_strIDFldName;		}
	void setIDFieldName(const wchar_t *vpidfield, std::map<std::wstring,int32_t> &mapHeadindex);
	int32_t getIDFieldIndex(void) const {			return m_lIDFldIndex;		}
	void setIDFieldIndex(int32_t vindex) {			m_lIDFldIndex = vindex;		}
	std::wstring getIDFieldValue(int32_t vrow) const;
	void setIDFieldValue(int32_t vrow, const wchar_t *vpvalue);
	std::map<int32_t, std::wstring>& getIDValue(void) {		return m_mapIDValue;		}

protected:
	std::wstring		m_strName;			// ��ʾ��
	std::wstring		m_strParName;		// ����ʾ��
	std::wstring		m_strFldName;		// SQL�ֶ�����Ψһ�ԣ�����Ϊ��
	int32_t				m_lFldIndex;		// SQL�ֶ���m_strFldName��Ӧ������
	std::wstring		m_strTableName;		// SQL��ر���
	// 
	BEDataType			m_beDataType;		// ��������
	BEAlignMode			m_beAlign;			// ���뷽ʽ
	int32_t				m_iWidth;			// �п�
	int32_t				m_iDecimals;		// ������ʱ������С�������
	bool				m_bVisible;			// �пɼ���
	bool				m_bIsSaved;			// ���Ƿ񱻱���
	BEComputeMethod		m_beComputed;		// �в�ȡ����ֵ��ʽ
	bool				m_bIsLocked;		// ���ɱ༭
	bool				m_bIsMerged;		// ��ͬ�ϲ�
	bool				m_bIsMustWrited;	// �Ƿ����
	bool				m_bIsFind;			// �Ƿ�ͨ�����ҵõ�ֵ������ǣ�������֤���м���������m_bIsLocked=true���У���������ֵ
	bool				m_bIsOnlySelected;	// �����ֶ����Ƿ��ǵ�ѡ

	std::wstring		m_strIDFldName;		// SQL�ֶ����Ĺؼ��ֶ���
	int32_t				m_lIDFldIndex;		// SQL�ֶ����Ĺؼ��ֶ���������
	// SQL�ֶ����Ĺؼ��ֶ���(m_strIDFldName) ��SDTBoolean���ֶ� ��Ӧ��ֵ
	//CStringArray	m_arrIDValue;		// m_strIDFldName�ֶζ�Ӧ��ֵ�����鳤����������������һ��
	std::map<int32_t, std::wstring>	m_mapIDValue;	// m_strIDFldName�ֶζ�Ӧ��ֵ��key=�кţ�����0��
};



}

#endif
