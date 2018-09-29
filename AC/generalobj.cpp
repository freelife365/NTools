#include "generalobj.h"
#include "AC/sysupport.h"

namespace gtc
{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gridColHeader��		2014-10-18
//	���ؼ����б���
////////////////////////////////////////////////////////////////////////////////////////////////////////
gridColHeader::gridColHeader()
	: m_strName(L"")
	, m_strParName(L"")
	, m_strFldName(L"")
	, m_lFldIndex(-1L)
	, m_strTableName(L"")
	, m_beDataType(BEDataTypeNone)
	, m_beAlign(BEAlignModeAllLeft)
	, m_iWidth(1000)
	, m_iDecimals(2)
	, m_bVisible(true)
	, m_bIsSaved(false)
	, m_beComputed(BEComputeMethodNone)
	, m_bIsLocked(false)
	, m_bIsMerged(false)
	, m_bIsMustWrited(false)
	, m_bIsFind(false)
	, m_bIsOnlySelected(false)
	, m_strIDFldName(L"")
	, m_lIDFldIndex(-1L)
{
}
gridColHeader::gridColHeader(const wchar_t *vname, const wchar_t *vparname, const wchar_t *vfldname, const wchar_t *vidfldname, 
	BEDataType vtype, BEAlignMode valign, int vwidth)
	: m_strName(vname?vname:L"")
	, m_strParName(vparname?vparname:L"")
	, m_strFldName(vfldname?vfldname:L"")
	, m_lFldIndex(-1L)
	, m_strTableName(L"")
	, m_beDataType(vtype)
	, m_beAlign(valign)
	, m_iWidth(vwidth)
	, m_iDecimals(2)
	, m_bVisible(true)
	, m_bIsSaved(false)
	, m_beComputed(BEComputeMethodNone)
	, m_bIsLocked(false)
	, m_bIsMerged(false)
	, m_bIsMustWrited(false)
	, m_bIsFind(false)
	, m_bIsOnlySelected(false)
	, m_strIDFldName(vidfldname?vidfldname:L"")
	, m_lIDFldIndex(-1L)
{
}
gridColHeader::gridColHeader(const wchar_t *vname, const wchar_t *vfldname,
	BEDataType vtype, BEAlignMode valign, int vwidth)
	: m_strName(vname?vname:L"")
	, m_strParName(L"")
	, m_strFldName(vfldname?vfldname:L"")
	, m_lFldIndex(-1L)
	, m_strTableName(L"")
	, m_beDataType(vtype)
	, m_beAlign(valign)
	, m_iWidth(vwidth)
	, m_iDecimals(2)
	, m_bVisible(true)
	, m_bIsSaved(false)
	, m_beComputed(BEComputeMethodNone)
	, m_bIsLocked(false)
	, m_bIsMerged(false)
	, m_bIsMustWrited(false)
	, m_bIsFind(false)
	, m_bIsOnlySelected(false)
	, m_strIDFldName(L"")
	, m_lIDFldIndex(-1L)
{
}
gridColHeader::gridColHeader(const gridColHeader& header)
	: m_strName(header.m_strName)
	, m_strParName(header.m_strParName)
	, m_strFldName(header.m_strFldName)
	, m_lFldIndex(header.m_lFldIndex)
	, m_strTableName(header.m_strTableName)
	, m_beDataType(header.m_beDataType)
	, m_beAlign(header.m_beAlign)
	, m_iWidth(header.m_iWidth)
	, m_iDecimals(header.m_iDecimals)
	, m_bVisible(header.m_bVisible)
	, m_bIsSaved(header.m_bIsSaved)
	, m_beComputed(header.m_beComputed)
	, m_bIsLocked(header.m_bIsLocked)
	, m_bIsMerged(header.m_bIsMerged)
	, m_bIsMustWrited(header.m_bIsMustWrited)
	, m_bIsFind(header.m_bIsFind)
	, m_bIsOnlySelected(header.m_bIsOnlySelected)
	, m_strIDFldName(header.m_strIDFldName)
	, m_lIDFldIndex(header.m_lIDFldIndex)
{
}
gridColHeader& gridColHeader::operator =(const gridColHeader& header)
{
	m_strName = header.m_strName;
	m_strParName = header.m_strParName;
	m_strFldName = header.m_strFldName;
	m_lFldIndex = header.m_lFldIndex;
	m_strTableName = header.m_strTableName;
	m_beDataType = header.m_beDataType;
	m_beAlign = header.m_beAlign;
	m_iWidth = header.m_iWidth;
	m_iDecimals = header.m_iDecimals;
	m_bVisible = header.m_bVisible;
	m_bIsSaved = header.m_bIsSaved;
	m_beComputed = header.m_beComputed;
	m_bIsLocked = header.m_bIsLocked;
	m_bIsMerged = header.m_bIsMerged;
	m_bIsMustWrited = header.m_bIsMustWrited;
	m_bIsFind = header.m_bIsFind;
	m_bIsOnlySelected = header.m_bIsOnlySelected;
	m_strIDFldName = header.m_strIDFldName;
	m_lIDFldIndex = header.m_lIDFldIndex;
	return *this;
}
// ����
std::wstring gridColHeader::toString() const
{
	std::wostringstream dsc;
	dsc << L"gridColHeader" << L"<"
		<< L"    ��ʾ����" << m_strName
		<< L"    ����ʾ����"<< m_strParName
		<< L"    �ֶ�����"<< m_strFldName
		<< L"    �ֶ�������"<< m_lFldIndex
		<< L"    ��ر�����"<< m_strTableName
		//<< L"    �������ͣ�"<< toString(m_beDataType)
		////<< L"    ���뷽ʽ��"<< toString(m_beAlign)
		<< L"    �п�"<< m_iWidth
		<< L"    С��λ����"<< m_iDecimals
		<< L"    �пɼ���"<< m_bVisible
		<< L"    ���Ƿ񱻱��棺"<< m_bIsSaved
		//<< L"    �в�ȡ����ֵ��ʽ��"<< toString(m_beComputed)
		<< L"    ���ɱ༭��"<< m_bIsLocked
		<< L"    ��ͬ�ϲ���"<< m_bIsMerged
		<< L"    �Ƿ���䣺"<< m_bIsMustWrited
		<< L"    �Ƿ�ͨ�����ҵõ�ֵ��"<< m_bIsFind
		<< L"    �����ֶ����Ƿ��ǵ�ѡ��"<< m_bIsOnlySelected
		<< L"    SQL�ֶ����Ĺؼ��ֶ�����"<< m_strIDFldName
		<< L"    SQL�ֶ����Ĺؼ��ֶ�����������"<< m_lIDFldIndex
		<< L"    �ֶζ�Ӧ��ֵ��"<< m_mapIDValue.size()
		<< L">" << std::endl;
	return dsc.str();
}
gridColHeader::~gridColHeader() 
{
}

void gridColHeader::free(PtrArrayType &headers)
{
	for(PtrArrayType::iterator iter = headers.begin(); iter != headers.end(); ++iter)
		delete *iter;
	headers.clear();
}

void gridColHeader::setIDFieldName(const wchar_t *vpidfield, std::map<std::wstring,int32_t> &mapHeadindex) {	
	m_strIDFldName = vpidfield?vpidfield:L"";
	// 
	std::wstring sname = m_strFldName;
	convert::toUpper(sname);
	std::map<std::wstring,int32_t>::iterator iter = mapHeadindex.find(sname);
	if(iter != mapHeadindex.end())
	{
		std::wstring sname = m_strIDFldName;
		convert::toUpper(sname);
		mapHeadindex[sname] = iter->second;
	}
}
std::wstring gridColHeader::getIDFieldValue(int32_t vrow) const {
	std::wstring value;
	std::map<int32_t, std::wstring>::const_iterator it = m_mapIDValue.find(vrow);
	if(it != m_mapIDValue.end())
	{
		value = it->second;
	}

	return value;
}
void gridColHeader::setIDFieldValue(int32_t vrow, const wchar_t *vpvalue) {
	if(vpvalue != nullptr && std::wcslen(vpvalue) > 0)
		m_mapIDValue[vrow] = vpvalue;
}

}