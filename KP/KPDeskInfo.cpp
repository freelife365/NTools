#include "KP/KPDeskInfo.h"
#include "AC/sysupport.h"
#include "AC/baseReg.h"
//#include "kpdeskListView.h"
#include <stgprop.h>

#define DEBUG_CS_LOG	//gtc::app::writeVSLogs

namespace gtc{

static const IID IID_IImageList = { //"{46EB5926-582E-4017-9FDF-E8998DAA0950}";
	0x46EB5926, 0x582E, 0x4017,
	{ 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x09, 0x50}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BSNodeMoveInfo moveNodeCache��	2014-12-25
//	û�гɹ��ƶ��ڵ�Ļ��棻�磺�ڵ㱻������������
////////////////////////////////////////////////////////////////////////////////////////////////////////

moveNodeCache::moveNodeCache()
{
	::InitializeCriticalSection(&m_csLock);
}
moveNodeCache::~moveNodeCache()
{
	::DeleteCriticalSection(&m_csLock);
}

bool moveNodeCache::push(PCWSTR pkeyName, POINT pt)
{
	bool bOk = false;
	if(!pkeyName || std::wcslen(pkeyName) <= 0) return bOk;
	::EnterCriticalSection(&m_csLock);
	BSNodeMoveInfo data(pkeyName, pt);
	if(std::find(m_queCache.cbegin(), m_queCache.cend(), data) == m_queCache.cend())
	{
		m_queCache.push_front(data);
		bOk = true;
	}
	else
	{
		SEND_LOG(L"moveNodeCache::push �Ѿ����� name:<%s>", pkeyName);
	}
	::LeaveCriticalSection(&m_csLock);
	return bOk;
}
bool moveNodeCache::pop(std::wstring &keyName, POINT &pt)
{
	bool bOk = false;
	pt.x = pt.y = 0;
	keyName.clear();
	::EnterCriticalSection(&m_csLock);
	if (!m_queCache.empty())
	{
		pt = m_queCache.back().pos;
		keyName = m_queCache.back().pkeyName;
		m_queCache.pop_back();
		bOk = true;
	}
	::LeaveCriticalSection(&m_csLock);

	return bOk;
}
bool moveNodeCache::exists(PCWSTR pkeyName)
{
	bool bOk = false;
	::EnterCriticalSection(&m_csLock);
	bOk = std::find(m_queCache.cbegin(), m_queCache.cend(), pkeyName) != m_queCache.cend();
	::LeaveCriticalSection(&m_csLock);
	return bOk;
}
bool moveNodeCache::isEmpty() const
{
	return m_queCache.empty();
}
std::size_t moveNodeCache::size() const
{
	return m_queCache.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	monitorDirCache	2014-12-24
//	windowsĿ¼(�ļ���)״̬������ݻ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
monitorDirCache::monitorDirCache()
{
	::InitializeCriticalSection(&m_csLock);
}
monitorDirCache::~monitorDirCache()
{
	::DeleteCriticalSection(&m_csLock);
}

void monitorDirCache::push(LONG Event, PCWSTR pfullPath1, PCWSTR pfullPath2)
{
	::EnterCriticalSection(&m_csLock);
	m_queCache.push(BSMonitorData(Event, pfullPath1, pfullPath2));
	::LeaveCriticalSection(&m_csLock);
}
bool monitorDirCache::pop(BSMonitorData &data)
{
	bool bOk = false;
	data.lEvent = 0;
	data.pFullPath1[0] = data.pFullPath2[0] = L'\0';
	::EnterCriticalSection(&m_csLock);
	if (!m_queCache.empty())
	{
		data.lEvent = m_queCache.front().lEvent;
		std::wcsncpy(data.pFullPath1, m_queCache.front().pFullPath1, _countof(data.pFullPath1));
		std::wcsncpy(data.pFullPath2, m_queCache.front().pFullPath2, _countof(data.pFullPath2));
		m_queCache.pop();
		bOk = true;
	}
	::LeaveCriticalSection(&m_csLock);

	return bOk;
}
bool monitorDirCache::isEmpty() const
{
	return m_queCache.empty();
}
std::size_t monitorDirCache::size() const
{
	return m_queCache.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	nodesCache��	2014-10-30
//	�ڵ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
nodesCache::nodesCache()
{
	::InitializeCriticalSectionAndSpinCount(&m_csLock, 4000);
}
nodesCache::~nodesCache()
{
	::DeleteCriticalSection(&m_csLock);
}

bool nodesCache::push(std::wstring fullPath)
{
	bool bOk = false;
	if(fullPath.empty()) return bOk;
	::EnterCriticalSection(&m_csLock);
	if(std::find(m_queCache.cbegin(), m_queCache.cend(), fullPath) == m_queCache.cend())
	{
		m_queCache.push_front(fullPath);
		bOk = true;
	}
	else
	{
		SEND_LOG(L"nodesCache::push �Ѿ����� name:<%s>", fullPath.c_str());
	}
	::LeaveCriticalSection(&m_csLock);
	return bOk;
}

bool nodesCache::pop(std::wstring &fullPath)
{
	fullPath.clear();
	::EnterCriticalSection(&m_csLock);
	if (!m_queCache.empty())
	{
		fullPath = m_queCache.back();
		m_queCache.pop_back();
	}
	::LeaveCriticalSection(&m_csLock);
	return !fullPath.empty();
}

bool nodesCache::getNode(std::wstring &fullPath)
{
	fullPath.clear();
	::EnterCriticalSection(&m_csLock);
	if (!m_queCache.empty())
	{
		fullPath = m_queCache.back();
	}
	::LeaveCriticalSection(&m_csLock);
	return !fullPath.empty();
}

bool nodesCache::popNode(const std::wstring &fullPath)
{
	bool bOk = false;
	::EnterCriticalSection(&m_csLock);
	if (!m_queCache.empty())
	{
		if(m_queCache.back() == fullPath)
		{
			m_queCache.pop_back();
			bOk = true;
		}
	}
	::LeaveCriticalSection(&m_csLock);
	return bOk;
}
bool nodesCache::isEmpty()
{
	bool bl = false;
	::EnterCriticalSection(&m_csLock);
	bl = m_queCache.empty();
	::LeaveCriticalSection(&m_csLock);
	return bl;
}

bool nodesCache::exists(std::wstring fullPath)
{
	bool bOk = false;
	::EnterCriticalSection(&m_csLock);
	bOk = std::find(m_queCache.cbegin(), m_queCache.cend(), fullPath) != m_queCache.cend();
	::LeaveCriticalSection(&m_csLock);
	return bOk;
}

std::size_t nodesCache::size()
{
	return m_queCache.size();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	idlNode��	2014-04-30
//	������Ϣ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
std::multimap<std::wstring, std::wstring> idlNode::mc_mapFileTypeExtension;	

idlNode::idlNode(std::wstring wsname, std::wstring wssize, std::wstring wstype, std::wstring wsmodifydt)
	: m_wsName(wsname), m_wsLVSize(wssize), m_wsLVTypeName(wstype), m_wsLVModifyDT(wsmodifydt)
	, m_iIndex(IDLNODE_INIT_INDEX), m_wsFullPath(L""), m_wsDirectory(L"")
	, m_ulAttributes(0x0)
	//, m_pIDLST(NULL)
	, m_iSmallIcon(-1), m_iSmallOpenIcon(-1), m_iLagerIcon(-1), m_iLagerOpenIcon(-1), m_hIcon(NULL)
	, m_beGroupFlag(BEDeskGroupFlagNone)
	, m_uSize(0), m_dtCreated(0), m_dtLastModify(0)
	, m_beShow(BENodeShowFlagHide), m_bTemporary(false)
	, m_iOptFlag(1)
	, m_iPOSIndex(-1)
{
	m_szNode.cx = m_szNode.cy = 0;
}
//
//idlNode::idlNode(PCWSTR pname, int32_t idx, const SIZE &sz)
//	: m_wsName(pname?pname:L""), m_iIndex(idx), m_wsTypeName(L""), m_wsFullPath(L"")
//	, m_ulAttributes(0x0)
//	, m_pIDLST(nullptr)
//	, m_iSmallIcon(-1), m_iSmallOpenIcon(-1), m_iLagerIcon(-1), m_iLagerOpenIcon(-1), m_hIcon(NULL)
//	, m_beGroupFlag(BEDeskGroupFlagNone)
//	, m_uSize(0), m_dtCreated(0), m_dtLastModify(0)
//	, m_beShow(BENodeShowFlagHide), m_bTemporary(false)
//	, m_szNode(sz)
//{
//
//}
//
//idlNode::idlNode(PCWSTR pname, PCWSTR ptypeName, PCWSTR pfullpath, SFGAOF attributs)
//	: m_wsName(pname?pname:L""), m_iIndex(-1), m_wsTypeName(ptypeName?ptypeName:L""), m_wsFullPath(pfullpath?pfullpath:L"")
//	, m_ulAttributes(attributs)
//	, m_pIDLST(nullptr)
//	, m_iSmallIcon(-1), m_iSmallOpenIcon(-1), m_iLagerIcon(-1), m_iLagerOpenIcon(-1), m_hIcon(NULL)
//	, m_beGroupFlag(BEDeskGroupFlagNone)
//	, m_uSize(0), m_dtCreated(0), m_dtLastModify(0)
//	, m_beShow(BENodeShowFlagHide), m_bTemporary(false)
//{
//}
//
//idlNode::idlNode(PCWSTR pname, PCWSTR ptypeName, PCWSTR pfullpath, SFGAOF attributs, LPITEMIDLIST pidlst)
//	: m_wsName(pname?pname:L""), m_iIndex(-1), m_wsTypeName(ptypeName?ptypeName:L""), m_wsFullPath(pfullpath?pfullpath:L"")
//	, m_ulAttributes(attributs)
//	, m_pIDLST(pidlst)
//	, m_iSmallIcon(-1), m_iSmallOpenIcon(-1), m_iLagerIcon(-1), m_iLagerOpenIcon(-1), m_hIcon(NULL)
//	, m_beGroupFlag(BEDeskGroupFlagNone)
//	, m_uSize(0), m_dtCreated(0), m_dtLastModify(0)
//	, m_beShow(BENodeShowFlagHide), m_bTemporary(false)
//{
//}

idlNode::~idlNode()
{
	//gtc::app::writeVSLogs(L"idlNode::~idlNode %s", m_wsName.c_str());
	if (m_hIcon)
	{
		::DestroyIcon(m_hIcon);
	}
	//if (m_pIDLST)
	//{
	//	CoTaskMemFree(m_pIDLST);
	//}
}

void idlNode::free(IDINodeMap &mapdata)
{
	for (IDINodeMap::iterator iter = mapdata.begin(); iter != mapdata.end(); ++iter)
	{
		delete iter->second;
	}
	mapdata.clear();
}

void idlNode::free(IDINodeArray &arrdata)
{
	for (IDINodeArray::iterator iter = arrdata.begin(); iter != arrdata.end(); ++iter)
	{
		delete *iter;
	}
	arrdata.clear();
}

std::wstring idlNode::getKeyName() const
{
	// �Խڵ���2�����ƣ�1��ϵͳ�ڵ�䲻��������2���û��ļ��в�������ͨ�ļ�������
	// �����ڵ�keyҪ����һ��
	// key�ĸ�ʽΪ��ϵͳ�ڵ���á�::{CLSID}������ͨ�ڵ������ȫ·��
	std::wstring wskey = getFileName();
	if(wskey.length() >= 3 && wskey.substr(0, 3) == L"::{")
		return wskey;
	return getFullPath();
	//std::wstring wskey = m_wsName;
	//if (m_wsLVTypeName == L"Internet ��ݷ�ʽ")
	//{
	//	wskey += L".url";
	//}
	//else if((m_wsLVTypeName.length() >= 4 && m_wsLVTypeName.substr(m_wsLVTypeName.length()-4) == L"��ݷ�ʽ")/* || 
	//	(m_wsTypeName.length() >= 4 && m_wsTypeName.substr(m_wsTypeName.length()-4) == L"��ݷ�ʽ")*/)
	//{
	//	wskey += L".lnk";
	//}
	//if(m_wsLVTypeName.empty() && m_wsLVCreated.empty())
	//{	// ϵͳͼ�꣺���ҵĵ���
	//	//return m_wsName;
	//}
}

//int idlNode::getIndex()
//{
//	if (m_iIndex < 0)
//	{
//		m_iIndex = kpdeskListView::findNodeIndex(getKeyName().c_str());
//	}
//	return m_iIndex;
//}

bool idlNode::isSystemGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	//if (wsLVType == L"ϵͳ�ļ���" || wsTargetType == L"ϵͳ�ļ���")
	//{
	//	return true;
	//}
	//if (m_wsLVSize.empty() && m_wsLVTypeName.empty() && m_wsLVModifyDT.empty())
	//{
	//	return true;
	//}
	//return false;
	return m_wsFileName.length() > 1 && m_wsFileName.substr(0, 2) == L"::";
}
bool idlNode::isFolderGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	if (wsLVType == L"�ļ���" || wsTargetType == L"�ļ���")
	{
		return true;
	}
	return false;
}
bool idlNode::isSoftGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	// exe�ļ�Ϊ��������
	if(m_wsFullPath.length() > 4 && m_wsFullPath.substr(m_wsFullPath.length()-4) == L".exe")
		return false;
	if (wsLVType == L"Ӧ�ó���" || wsTargetType == L"Ӧ�ó���")
	{
		return true;
	}
	return false;
}
bool idlNode::isCompressGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	std::wstring::size_type pos = m_wsFileName.rfind(L'.');
	if (pos != std::wstring::npos)
	{
		std::wstring wsext = m_wsFileName.substr(pos);
		gtc::convert::toLower(wsext);
		if(gc_mapExtnamePackeg.find(wsext) != gc_mapExtnamePackeg.end())
			return true;
	}

	if ((wsLVType.length() >= 5 && wsLVType.substr(wsLVType.length()-5).compare(L" ѹ���ļ�") == 0) ||
		(wsTargetType.length() >= 5 && wsTargetType.substr(wsTargetType.length()-5).compare(L" ѹ���ļ�") == 0))
	{
		return true;
	}
	return false;
}
bool idlNode::isDocumentGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	std::wstring::size_type pos = m_wsFileName.rfind(L'.');
	if (pos != std::wstring::npos)
	{
		std::wstring wsext = m_wsFileName.substr(pos);
		gtc::convert::toLower(wsext);
		if(gc_mapExtnameDocument.find(wsext) != gc_mapExtnameDocument.end())
		{
			//WRITE_LOG(L"idlNode::isDocumentGroup %s=%s", wsLVType.c_str(), wsext.c_str());
			return true;
		}
	}
	if ((wsLVType.length() >= 3 && (wsLVType.substr(wsLVType.length()-3) == L" �ĵ�" || wsLVType.substr(wsLVType.length()-3) == L"������")) 
		|| (wsTargetType.length() >= 3 && (wsTargetType.substr(wsTargetType.length()-3) == L" �ĵ�" || wsTargetType.substr(wsTargetType.length()-3) == L"������")) 
		|| (wsLVType.length() >= 9 && wsLVType.substr(wsLVType.length()-9) == L" Document") 
		|| (wsTargetType.length() >= 9 && wsTargetType.substr(wsTargetType.length()-9) == L" Document"))
	{
		return true;
	}


	return false;
}
bool idlNode::isPictureGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType)
{
	std::wstring::size_type pos = m_wsFileName.rfind(L'.');
	if (pos != std::wstring::npos)
	{
		std::wstring wsext = m_wsFileName.substr(pos);
		gtc::convert::toLower(wsext);
		if(gc_mapExtnameImage.find(wsext) != gc_mapExtnameImage.end())
			return true;
	}
	if ((wsLVType.length() >= 3 && wsLVType.substr(wsLVType.length()-3) == L" ͼ��") || 
		(wsTargetType.length() >= 3 && wsTargetType.substr(wsTargetType.length()-3) == L" ͼ��"))
	{
		return true;
	}
	if ((wsLVType.length() >= 5 && wsLVType.substr(wsLVType.length()-5) == L" ͼƬ�ļ�") || 
		(wsTargetType.length() >= 5 && wsTargetType.substr(wsTargetType.length()-5) == L" ͼƬ�ļ�"))
	{
		return true;
	}
	return false;
}

// �ڵ�����
BEDeskGroupFlag idlNode::getGroupType(){
	if(m_beGroupFlag != BEDeskGroupFlagNone) return m_beGroupFlag;
	
	std::wstring wsLVType = getLVType();
	//std::wstring wsType = getTypeName();
	WRITE_LOG(L"idlNode::getGroupType AAA name:<%s> fullPath:<%s> fileName:<%s>", m_wsName.c_str(), m_wsFullPath.c_str(), m_wsFileName.c_str());
	std::wstring wsTargetType = getTargetType();
	WRITE_LOG(L"idlNode::getGroupType  lvtype:%s   tartype:%", wsLVType.c_str(), wsTargetType.c_str());
	if (isSystemGroup(wsLVType, wsTargetType))
	{
		m_beGroupFlag = BEDeskGroupFlagSystem;
	}
	else if (isFolderGroup(wsLVType, wsTargetType))
	{
		m_beGroupFlag = BEDeskGroupFlagFolder;
	}
	else if (isSoftGroup(wsLVType, wsTargetType))
	{
		m_beGroupFlag = BEDeskGroupFlagSoft;
	}
	//else if (isCompressGroup(wsType, wsLVType, wsTargetType))
	//{
	//	m_beGroupFlag = BEDeskGroupFlagCompress;
	//}
	else if (isDocumentGroup(wsLVType, wsTargetType))
	{
		m_beGroupFlag = BEDeskGroupFlagDocument;
	}
	else if (isPictureGroup(wsLVType, wsTargetType))
	{
		m_beGroupFlag = BEDeskGroupFlagPicture;
	}
	else
	{
		if (m_wsFullPath.empty())
		{
			WRITE_LOG(L"idlNode::getGroupType name:<%s> lvtype:<%s> targetType:<%s> targetPath:<%s>", m_wsName.c_str(), wsLVType.c_str(), wsTargetType.c_str(), m_wsTargetPath.c_str());
		}
		m_beGroupFlag = getGroupFlagByExtname(m_wsFileName.c_str());
	}
	WRITE_LOG(L"idlNode::getGroupType TYPE:%d", m_beGroupFlag);
	return m_beGroupFlag;
}
// Сͼ������
int32_t idlNode::getSmallIcon() {
	__try
	{
		if (m_iSmallIcon < 0 && m_wsFullPath.length() > 0)
		{
			SHFILEINFO fileinfo = {0};
			DWORD_PTR dw = ::SHGetFileInfo(m_wsFullPath.c_str(), -1, &fileinfo, sizeof(SHFILEINFO), 
				SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
			m_iSmallIcon = fileinfo.iIcon;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::getSmallIcon"))
	{
		m_iSmallIcon = -1;
		WRITE_LOG(L"idlNode::getSmallIcon SEH�쳣 ");
	}
	return m_iSmallIcon;
}
// ��ͼ������
int32_t idlNode::getLagerIcon() {
	__try
	{
		if (m_iLagerIcon < 0 && m_wsFullPath.length() > 0)
		{
			SHFILEINFO fileinfo = {0};
			DWORD_PTR dw = ::SHGetFileInfo(m_wsFullPath.c_str(), -1, &fileinfo, sizeof(SHFILEINFO), 
				SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
			m_iLagerIcon = fileinfo.iIcon;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::getLagerIcon"))
	{
		m_iLagerIcon = -1;
		WRITE_LOG(L"idlNode::getLagerIcon SEH�쳣 ");
	}
	return m_iLagerIcon;
}
// ѡ��Сͼ������
int32_t idlNode::getSmallOpenIcon() {
	__try
	{
		if (m_iSmallOpenIcon < 0 && m_wsFullPath.length() > 0)
		{
			SHFILEINFO fileinfo = {0};
			DWORD_PTR dw = ::SHGetFileInfo(m_wsFullPath.c_str(), -1, &fileinfo, sizeof(SHFILEINFO), 
				SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
			m_iSmallOpenIcon = fileinfo.iIcon;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::getSmallOpenIcon"))
	{
		m_iSmallOpenIcon = -1;
		WRITE_LOG(L"idlNode::getSmallOpenIcon SEH�쳣 ");
	}
	return m_iSmallOpenIcon;
}
// ѡ�д�ͼ������
int32_t idlNode::getLagerOpenIcon() {
	__try
	{
		if (m_iLagerOpenIcon < 0 && m_wsFullPath.length() > 0)
		{
			SHFILEINFO fileinfo = {0};
			DWORD_PTR dw = ::SHGetFileInfo(m_wsFullPath.c_str(), -1, &fileinfo, sizeof(SHFILEINFO), 
				SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_OPENICON);
			m_iLagerOpenIcon = fileinfo.iIcon;
		}

	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::getLagerOpenIcon"))
	{
		m_iLagerOpenIcon = -1;
		WRITE_LOG(L"idlNode::getLagerOpenIcon SEH�쳣 ");
	}
	return m_iLagerOpenIcon;
}
HICON idlNode::getIcon() {
	__try
	{
		if (m_hIcon == NULL && m_wsFullPath.length() > 0)
		{
			SHFILEINFO fileinfo = {0};
			DWORD_PTR dw = ::SHGetFileInfo(m_wsFullPath.c_str(), -1, &fileinfo, sizeof(SHFILEINFO), 
				SHGFI_ICON);
			m_hIcon = fileinfo.hIcon;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::getIcon"))
	{
		m_hIcon = NULL;
		WRITE_LOG(L"idlNode::getIcon SEH�쳣 ");
	}
	return m_hIcon;
}
std::wstring idlNode::getTargetPath() {
	if (m_wsTargetPath.empty() && 
		((m_wsLVTypeName.length()>= 4 && m_wsLVTypeName.substr(m_wsLVTypeName.length()-4).compare(L"��ݷ�ʽ") == 0)))
	{
		WRITE_LOG(L"idlNode::getTargetPath lvType:<%s> fullPath:<%s>", m_wsLVTypeName.c_str(), m_wsFullPath.c_str());
		if(m_wsLVTypeName == L"Internet ��ݷ�ʽ") 
			return m_wsTargetPath;
		if(!readShortcutFile(m_wsFullPath.c_str(), m_wsTargetPath))
			WRITE_LOG(L"ERROR idlNode::getTargetPath ����Ŀ��·�� ʧ�ܣ����� name:<%s> targetPath:<%s>", m_wsFullPath.c_str(), m_wsTargetPath.c_str());

		//wchar_t pProductCode[39] = {0};
		//wchar_t pFeatureId[MAX_FEATURE_CHARS+1] = {0};
		//wchar_t pComponentCode[39] = {0};
		//DWORD len = MAX_PATH;
		//wchar_t path[MAX_PATH] = {0};
		//UINT target = MsiGetShortcutTarget(m_wsFullPath.c_str(), pProductCode, pFeatureId, pComponentCode);
		//INSTALLSTATE inst = MsiGetComponentPath(pProductCode, pComponentCode, path, &len);
		//if (target == ERROR_FUNCTION_FAILED || len == 0)
		//{
		//	WRITE_LOG(L"idlNode::getTargetPath ����Ŀ��·�� fullPath:<%s>", m_wsFullPath.c_str());
		//	//m_wsTargetPath = readShortcutFile(m_wsFullPath.c_str());// readShortcut(m_wsFullPath.c_str());
		//	if(!readShortcutFile(m_wsFullPath.c_str(), m_wsTargetPath))
		//		SEND_LOG(L"ERROR idlNode::getTargetPath ����Ŀ��·�� ʧ�ܣ�����");
		//}
		//else
		//	m_wsTargetPath = path;

		WRITE_LOG(L"idlNode::getTargetPath <%s>", m_wsTargetPath.c_str());

	}
	
	return m_wsTargetPath;
}
std::wstring idlNode::getTargetType() {
	if(m_wsTargetPath.length() <= 0)
		getTargetPath();
	if (m_wsTargetType.length() <= 0 && m_wsTargetPath.length()> 0)
	{
		if(!parseFileType(m_wsTargetPath.c_str(), m_wsTargetType))
			SEND_LOG(L"ERROR idlNode::getTargetType �����ļ�����ʧ�ܣ� targetPath:<%s>", m_wsTargetPath.c_str());
		//SHFILEINFO fileinfo = {0};
		//if(::SHGetFileInfo(m_wsTargetPath.c_str(), 0, &fileinfo, sizeof(SHFILEINFO), SHGFI_TYPENAME))
		//{
		//	m_wsTargetType = fileinfo.szTypeName;

		//}
	}

	//WRITE_LOG(L"idlNode::getTargetType <%s> <%s> <%s>", m_wsTargetPath.c_str(), m_wsLVTypeName.c_str(), m_wsTargetType.c_str());
	return m_wsTargetType;
}

//void idlNode::copyIIDList(LPITEMIDLIST piidl) {
//	assert(piidl);
//	if(!piidl) return;
//	__try
//	{
//		if (m_pIDLST)
//		{
//			CoTaskMemFree(m_pIDLST);
//			m_pIDLST = NULL;
//		}
//		int len = piidl->mkid.cb + sizeof(piidl->mkid.cb);
//		m_pIDLST = (LPITEMIDLIST)CoTaskMemAlloc(len);
//		::memcpy_s(m_pIDLST, len, piidl, len);
//	}
//	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::copyIIDList"))
//	{
//		WRITE_LOG(L"idlNode::copyIIDList SEH�쳣 piidl:%08X", m_pIDLST);
//		if(m_pIDLST)
//		{
//			CoTaskMemFree(m_pIDLST);
//			m_pIDLST = NULL;
//		}
//	}
//}

bool idlNode::update(LPITEMIDLIST piidl)
{
	assert(piidl);
	if(!piidl) return false;
	IShellFolder *ishellFolder = NULL;
	IShellFolder2 *ishell2 = NULL;
	HRESULT hr = S_OK;
	//wchar_t pstr[MAX_PATH] = {0};
	SHELLDETAILS shds = {0};
	wchar_t pbuf[MAX_PATH] = {0};
	bool bOk = false;
	__try
	{
		hr = ::SHGetDesktopFolder(&ishellFolder);
		if(FAILED(hr)) __leave;
		hr = ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2);
		if(FAILED(hr)) __leave;
		//hr = ishell2->BindToObject(m_pIDLST, NULL, IID_IShellFolder, (void **)&ishellFolder);
		//SEND_LOG(L"��ȡ %08X", hr);
		//if(FAILED(hr)) return false;

		//if(piidl) copyIIDList(piidl);
		// ע�⣺������ҲҪ���ñ�����������Ҫ�������Ƶ���Ϣ
		// ��ʾ��
		if (SUCCEEDED(hr = ishell2->GetDetailsOf(piidl, 0, &shds)))
		{
			if(SUCCEEDED(StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf)))) setName(pbuf);
		}
		// ��С
		if (SUCCEEDED(hr = ishell2->GetDetailsOf(piidl, 1, &shds)))
		{
			if(SUCCEEDED(StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf)))) setLVSize(pbuf);
		}
		// ����
		if (SUCCEEDED(ishell2->GetDetailsOf(piidl, 2, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf))))
		{
			setLVTypeName(pbuf);
		}
		// �޸�����
		if (SUCCEEDED(ishell2->GetDetailsOf(piidl, 3, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf))))
		{
			setLVModifyDT(pbuf);
		}

		// ȫ·�� �� ::{CLSID}
		STRRET strr = {0};
		if (SUCCEEDED(ishell2->GetDisplayNameOf(piidl, SHGDN_NORMAL | SHGDN_FORPARSING, &strr)))
		{
			if(SUCCEEDED(StrRetToBuf(&strr, piidl, pbuf, _countof(pbuf)))) setFullPath(pbuf);
			if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
		}
		// �ļ���������չ������ ::{CLSID}
		if (SUCCEEDED(ishell2->GetDisplayNameOf(piidl, SHGDN_INFOLDER | SHGDN_FORPARSING, &strr)))
		{
			if(SUCCEEDED(StrRetToBuf(&strr, piidl, pbuf, _countof(pbuf)))) setFileName(pbuf);
			if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
		}
		bOk = true;
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::update"))
	{
		bOk = false;
		WRITE_LOG(L"idlNode::update SEH�쳣 buf:<%s>", pbuf);
	}
	// 
	if(ishellFolder) ishellFolder->Release();
	if(ishell2) ishell2->Release();
	return bOk;
}
bool idlNode::update(const idlNode *pNode)
{
	if(!pNode) return false;
	bool bOk = false;
	__try
	{

		//copyIIDList(pNode->getIIDL());
		setName(pNode->getName().c_str());
		setLVSize(pNode->getLVSize().c_str());
		setLVTypeName(pNode->getLVType().c_str());
		setLVModifyDT(pNode->getLVModifyDT().c_str());

		setFullPath(pNode->getFullPath().c_str());
		setFileName(pNode->getFileName().c_str());
		bOk = true;
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::update"))
	{
		bOk = false;
		WRITE_LOG(L"idlNode::update SEH�쳣 ");
	}
	return bOk;
}
//std::wstring idlNode::readShortcut(PCWSTR pLnk)
//{
//	assert(pLnk);
//	std::wstring wsTarget;
//	IShellLink *pshLink = NULL;
//	IPersistFile *pshPersist = NULL;
//	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pshLink)))
//	{
//		if (SUCCEEDED(pshLink->QueryInterface	(IID_IPersistFile, (void **)&pshPersist)))
//		{
//			if(SUCCEEDED(pshPersist->Load(pLnk, STGM_READ)) && SUCCEEDED(pshLink->Resolve(NULL, SLR_ANY_MATCH)))
//			{
//				WIN32_FIND_DATA wfd = {0};
//				wchar_t ptarget[MAX_PATH] = {0};
//				if(SUCCEEDED(pshLink->GetPath(ptarget, _countof(ptarget), &wfd, SLGP_RAWPATH)))
//					wsTarget = ptarget;
//			}
//		}
//	}
//	if(pshPersist) pshPersist->Release();
//	if(pshLink) pshLink->Release();
//	return wsTarget;
//}

bool idlNode::readShortcutFile(PCWSTR pLnk, std::wstring &wsTargetPath)
{
	assert(pLnk);
	std::FILE *pfile = NULL;
	char *pfilePath = NULL;
	bool bOk = false;
	char *pbuf = NULL;
	LPITEMIDLIST pids = NULL;
	wsTargetPath.clear();
	if(!pLnk) return false;
	if(!gtc::convert::toChar(pLnk, &pfilePath)) return false;
	//std::string sfile = gtc::convert::toChar(pLnk);
	__try
	{
		wchar_t pProductCode[39] = {0};
		wchar_t pFeatureId[MAX_FEATURE_CHARS+1] = {0};
		wchar_t pComponentCode[39] = {0};
		DWORD len = MAX_PATH;
		wchar_t path[MAX_PATH] = {0};
		UINT target = MsiGetShortcutTarget(pLnk, pProductCode, pFeatureId, pComponentCode);
		INSTALLSTATE inst = MsiGetComponentPath(pProductCode, pComponentCode, path, &len);
		if (target == ERROR_FUNCTION_FAILED || len == 0)
		{
			WRITE_LOG(L"idlNode::readShortcutFile ����Ŀ��·�� ʧ��A fullPath:<%s>", pLnk);
		}
		else
		{
			wsTargetPath = path;
			__leave;
		}
		//////////////////////////////////////////////////////////////////////////
		if ((pfile = std::fopen(pfilePath, "rb")) == NULL)
			__leave;
		// ���ļ�ͷ
		BSLinkHead lnkHead = {0};
		if (std::fread(&lnkHead, sizeof(BSLinkHead), 1, pfile) != 1)
			__leave;
		// ����Ŀ��·��
		if (lnkHead.dwFlags & LNK_HASIDLIST)
		{
			short nidlLen = 0;
			if (std::fread(&nidlLen, sizeof(short), 1, pfile) != 1)
				__leave;
			if (nidlLen > 0)
			{
				pids = (LPITEMIDLIST)CoTaskMemAlloc(nidlLen);
				pbuf = new char[nidlLen];
				fread(pbuf, nidlLen, 1, pfile);
				::memcpy_s(pids, nidlLen, pbuf, nidlLen);
				wchar_t ppath[MAX_PATH] = {0};
				SHGetPathFromIDList(pids, ppath);
				wsTargetPath = ppath;
				if(pbuf) delete [] pbuf;
				if(pids) CoTaskMemFree(pids);
				pbuf = NULL;
				pids = NULL;
			}
		}
		bOk = true;
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::readShortcutFile"))
	{
		bOk = false;
		if(pbuf) delete [] pbuf;
		if(pids) CoTaskMemFree(pids);
		WRITE_LOG(L"idlNode::readShortcutFile SEH�쳣 lnk:<%s>", pLnk);
	}

	if(pfilePath) delete[] pfilePath;
	if(pfile) std::fclose(pfile);

	return bOk;
}

BEDeskGroupFlag idlNode::getGroupFlagByExtname(const std::wstring &fullpath) {
	//if(fullpath.empty()) return BEDeskGroupFlagSystem;
	assert(fullpath.length() > 0);
	std::wstring::size_type pos = fullpath.rfind(L".");
	std::wstring wsext;
	if(pos != std::wstring::npos)
	{
		wsext = fullpath.substr(pos);
		gtc::convert::toLower(wsext);
	}
	else
		return BEDeskGroupFlagOther;
	// 
	if(gc_mapExtnameDocument.find(wsext) != gc_mapExtnameDocument.end())
		return BEDeskGroupFlagDocument;
	// 
	if(gc_mapExtnameImage.find(wsext) != gc_mapExtnameImage.end())
		return BEDeskGroupFlagPicture;
	//// 
	//if(gc_mapExtnamePackeg.find(wsext) != gc_mapExtnamePackeg.end())
	//	return BEDeskGroupFlagCompress;
	return BEDeskGroupFlagOther;
}

LPITEMIDLIST idlNode::createNodeIIDL(const std::wstring &wsFullPath)
{
	assert(!wsFullPath.empty());
	if(wsFullPath.empty()) return NULL;

	IShellFolder *ishellFolder = NULL;
	IShellFolder2 *ishell2 = NULL;
	HRESULT hr = S_OK;
	ULONG celt = 0;
	LPENUMIDLIST peids = NULL;
	LPITEMIDLIST pidlItems = NULL;
	STRRET strr = {0};
	wchar_t pbuf[MAX_PATH] = {0};

	__try
	{
		// 
		if(FAILED(::SHGetDesktopFolder(&ishellFolder))) __leave;
		if(FAILED(ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2))) __leave;
		hr = ishell2->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &peids);
		assert(hr == S_OK);
		if(FAILED(hr)) __leave;


		while(peids->Next(1, &pidlItems, &celt) == S_OK && celt == 1)
		{
			if (SUCCEEDED(hr = ishell2->GetDisplayNameOf(pidlItems, SHGDN_NORMAL | SHGDN_FORPARSING, &strr)) &&
				SUCCEEDED(StrRetToBuf(&strr, pidlItems, pbuf, _countof(pbuf))))
			{
				//WRITE_LOG(L"idlNode::createNodeIIDL target:<%s> cur:<%s>", wsFullPath.c_str(), pbuf);
				if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
				if(wsFullPath == pbuf)
				{
					break;
				}
			}
			CoTaskMemFree(pidlItems);
			pidlItems = NULL;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::createNodeIIDL"))
	{
		if (pidlItems)
		{
			CoTaskMemFree(pidlItems);
			pidlItems = NULL;
		}
		WRITE_LOG(L"idlNode::createNodeIIDL_name SEH�쳣 name:<%s> buf:<%s>", wsFullPath.c_str(), pbuf);
	}
	if(peids) peids->Release();
	if(ishell2) ishell2->Release();
	if(ishellFolder) ishellFolder->Release();
	return pidlItems;
}

LPITEMIDLIST idlNode::createNodeIIDL(const std::wstring &displayName, const std::wstring &lvType, const std::wstring &lvSize, const std::wstring &lvModifyDT)
{
	assert(!displayName.empty());
	if(displayName.empty()) return NULL;

	IShellFolder *ishellFolder = NULL;
	IShellFolder2 *ishell2 = NULL;
	HRESULT hr = S_OK;
	ULONG celt = 0;
	LPENUMIDLIST peids = NULL;
	LPITEMIDLIST pidl = NULL;
	SHELLDETAILS shds = {0};
	STRRET strr = {0};
	wchar_t pbuf[MAX_PATH] = {0};

	__try
	{
		// 
		if(FAILED(::SHGetDesktopFolder(&ishellFolder))) __leave;
		if(FAILED(ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2))) __leave;
		hr = ishell2->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &peids);
		assert(hr == S_OK);
		if(FAILED(hr)) __leave;

		//SEND_LOG(L"idlNode::createNodeIIDL name:<%s> size:<%s> type:<%s> mdt:<%s>", displayName.c_str(), lvSize.c_str(), lvType.c_str(), lvModifyDT.c_str());
		//bool exedb = displayName == L"999.docx";
		bool bFind = false;
		bool bSysNode = lvType.empty() && lvSize.empty() && lvModifyDT.empty();
		while(peids->Next(1, &pidl, &celt) == S_OK && celt == 1)
		{
			//bFind = false;
			if (SUCCEEDED(ishell2->GetDetailsOf(pidl, 0, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
			{
				bFind = displayName == pbuf;
			}
			if (bFind)
			{	// ��ʾ��һ�£�������������
				//bFind = false;
				//if(exedb) SEND_LOG(L"idlNode::createNodeIIDL AAA name:<%s><%s>", displayName.c_str(), pbuf);
				if (bSysNode)
				{	// ��֤�ڵ��Ƿ�Ϊϵͳ�ڵ�
					// �ļ���������չ������ ::{CLSID}
					hr = ishell2->GetDisplayNameOf(pidl, SHGDN_INFOLDER | SHGDN_FORPARSING, &strr);
					if (SUCCEEDED(hr))
					{
						memset(pbuf, 0, sizeof(pbuf));
						if(SUCCEEDED(StrRetToBuf(&strr, pidl, pbuf, _countof(pbuf))))
						{
							SEND_LOG(L"idlNode::createNodeIIDL ϵͳ�ڵ� name:<%s>", pbuf);
							if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
							if(std::wcslen(pbuf) > 2 && pbuf[0] == L':' && pbuf[1] == L':' /*wstr.substr(0, 2) == L"::"*/) break;
						}
					}
				}
				else
				{
					// ����
					pbuf[0] = L'\0';
					hr = ishell2->GetDetailsOf(pidl, 2, &shds);
					if (bFind && SUCCEEDED(hr) && SUCCEEDED(hr = StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
					{
						bFind = lvType == pbuf;
					}
					//else
					//{
					//	if(exedb) SEND_LOG(L"idlNode::createNodeIIDL ���Ͳ�һ�£�����lv:<%s> shell:<%s>", lvType.c_str(), pbuf);
					//}
					//if(exedb) SEND_LOG(L"idlNode::createNodeIIDL ���� %d hr:%d lv:<%s> shell:<%s>", bFind, hr, lvType.c_str(), pbuf);
					// ��ͬ��ʾ�������͵Ľڵ��ڹ���������Ҳ���ھͱȽ����������Ƿ�һ��
					if (inPublicDesktop(displayName, lvType))
					{
						// ��С
						pbuf[0] = L'\0';
						hr = ishell2->GetDetailsOf(pidl, 1, &shds);
						if (SUCCEEDED(hr) && SUCCEEDED(hr = StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
						{
							bFind = lvSize == pbuf;
						}
						//else
						//{
						//	if(exedb) SEND_LOG(L"idlNode::createNodeIIDL ��С��һ�£�����lv:<%s> shell:<%s>", lvSize.c_str(), pbuf);
						//}
						//if(exedb) SEND_LOG(L"idlNode::createNodeIIDL ��С %d hr:%d lv:<%s> shell:<%s>", bFind, hr, lvSize.c_str(), pbuf);
				
						// �޸�����
						pbuf[0] = L'\0';
						hr = ishell2->GetDetailsOf(pidl, 3, &shds);
						if (bFind && SUCCEEDED(hr) && SUCCEEDED(hr = StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
						{
							bFind = lvModifyDT == pbuf;
						}
						//else
						//{
						//	if(exedb) SEND_LOG(L"idlNode::createNodeIIDL �޸����ڲ�һ�£�����lv:<%s> shell:<%s>", lvModifyDT.c_str(), pbuf);
						//}
						//if(exedb) SEND_LOG(L"idlNode::createNodeIIDL �޸����� %d hr:%d lv:<%s> shell:<%s>", bFind, hr, lvModifyDT.c_str(), pbuf);
						
					}
				
				}
			
			}
			//else
			//{
			//	SEND_LOG(L"idlNode::createNodeIIDL ��ʾ���Ʋ�һ�£�����lv:<%s> shell:<%s>", displayName.c_str(), pbuf);
			//}
			if(bFind) break;	// ���������ֵ��������Ŷ������
			CoTaskMemFree(pidl);
			pidl = NULL;
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::createNodeIIDL"))
	{
		if(pidl)
		{
			CoTaskMemFree(pidl);
			pidl = NULL;
		}
		WRITE_LOG(L"idlNode::createNodeIIDL_lvSubs SEH�쳣 lvName:<%s> lvType:<%s> buf:<%s>", displayName.c_str(), lvType.c_str(), pbuf);
	}
	if(peids) peids->Release();
	if(ishell2) ishell2->Release();
	if(ishellFolder) ishellFolder->Release();
	return pidl;
}

bool idlNode::inPublicDesktop(const std::wstring &displayName, const std::wstring &lvType)
{
	IShellFolder *pshellDesk = NULL;
	IShellFolder2 *ishell2 = NULL;
	HRESULT hr = S_OK;
	ULONG celt = 0;
	LPENUMIDLIST peids = NULL;
	LPITEMIDLIST piidl = NULL;
	bool bHad = false;
	wchar_t pbuf[MAX_PATH] = {0};
	__try
	{
		if(FAILED(::SHGetDesktopFolder(&pshellDesk))) __leave;
		ITEMIDLIST *pidldir = NULL;
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL,CSIDL_COMMON_DESKTOPDIRECTORY, &pidldir)))
		{
			pshellDesk->BindToObject(pidldir, NULL, IID_IShellFolder2, (void **)&ishell2);
			CoTaskMemFree(pidldir);
		}
		if(!ishell2)
		{
			__leave;
			//if(pshellDesk) pshellDesk->Release();
			//return false;
		}
		if (SUCCEEDED(ishell2->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &peids)))
		{
			
			SHELLDETAILS shds = {0};
			while(peids->Next(1, &piidl, &celt) == S_OK && celt == 1)
			{
				if (SUCCEEDED(ishell2->GetDetailsOf(piidl, 0, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf))))
				{
					bHad = displayName == pbuf;
				}
				hr = ishell2->GetDetailsOf(piidl, 2, &shds);
				if (bHad && SUCCEEDED(hr) && SUCCEEDED(hr = StrRetToBuf(&shds.str, piidl, pbuf, _countof(pbuf))))
				{
					bHad = lvType == pbuf;
				}
				CoTaskMemFree(piidl);
				piidl = NULL;
				if(bHad) break;
			}
		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::inPublicDesktop"))
	{
		bHad = false;
		if(piidl) CoTaskMemFree(piidl);
		WRITE_LOG(L"idlNode::inPublicDesktop SEH�쳣 name:<%s> type:<%s> buf:<%s>", displayName.c_str(), lvType.c_str(), pbuf);
	}
	// 
	if(peids) peids->Release();
	if(ishell2) ishell2->Release();
	if(pshellDesk) pshellDesk->Release();

	return bHad;
}
//void idlNode::updateNode(LPITEMIDLIST pidl, idlNode *pNode)
//{
//	assert(pidl && pNode);
//	if(!pidl || !pNode) return;
//
//	std::wstring wstr;
//	wchar_t pstr[MAX_PATH] = {0};
//
//	pNode->copyIIDList(pidl);
//	// ��ʾ��
//	SHFILEINFO fileinfo = {0};
//	if (::SHGetFileInfo((LPCTSTR)pidl, -1, &fileinfo, sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_DISPLAYNAME))
//	{
//		pNode->setName(fileinfo.szDisplayName);
//	}
//	// ����ȫ·���õ��ļ���
//	if(SHGetPathFromIDList(pidl, pstr))
//	{
//		pNode->setFullPath(pstr);
//		wstr = pstr;
//		if (wstr.length() > 0)
//		{	
//			// �����ļ���
//			std::wstring::size_type pos = wstr.rfind(L'\\');
//			if(pos != std::wstring::npos)
//				pNode->setFileName(wstr.substr(pos+1).c_str());
//			else
//				pNode->setFileName(wstr.c_str());
//		}
//		else
//		{	// ����ʾ����Ϊ�ļ������磺�ҵĵ��ԡ�����վ��
//			pNode->setFileName(pNode->getName().c_str());
//		}
//	}
//	else
//		pNode->setFileName(pNode->getName().c_str());
//}
bool idlNode::parseKeyName(LPITEMIDLIST pidl, std::wstring &wsKeyName)
{
	//std::wstring wsKey;
	//idlNode *pNode = createNode(pidl);
	//if(pNode)
	//{
	//	wsKey = pNode->getKeyName();
	//	delete pNode;
	//}
	//return wsKey;
	// �����ڵ�keyҪ����һ��
	// key�ĸ�ʽΪ��ȫ·������ʾ��
	//assert(pidl);
	wsKeyName.clear();
	if(!pidl) return false;
	IShellFolder *ishellFolder = nullptr;
	IShellFolder2 *ishell2 = nullptr;
	HRESULT hr = S_OK;
	bool bOk = false;
	wchar_t pstr[MAX_PATH] = {0};
	__try
	{

		hr = ::SHGetDesktopFolder(&ishellFolder);
		if(FAILED(hr)) __leave;
		hr = ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2);
		if(FAILED(hr)) __leave;

		// �ļ���������չ������ ::{CLSID}
		STRRET strr = {0};
		hr = ishell2->GetDisplayNameOf(pidl, SHGDN_INFOLDER | SHGDN_FORPARSING, &strr);
		if (SUCCEEDED(hr))
		{
			hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
			if(SUCCEEDED(hr))
			{
				//if(std::wcslen(pstr) >= 3 && std::wcsncmp(pstr, L"::{", 3) == 0)
				//	memset(pstr, 0, sizeof(pstr));
				wsKeyName = pstr;
				if(!(wsKeyName.length() >= 3 && std::wcsncmp(pstr, L"::{", 3) == 0 /*wsKeyName.substr(0, 3) == L"::{"*/))
					wsKeyName.clear();
			}
			if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
		}
		// ��ʾ��
		if (wsKeyName.empty())
		{
			hr = ishell2->GetDisplayNameOf(pidl, SHGDN_NORMAL | SHGDN_FORPARSING, &strr);
			if (SUCCEEDED(hr))
			{
				hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
				if(SUCCEEDED(hr))
				{
					wsKeyName = pstr;
				}
				if(strr.uType == STRRET_WSTR && strr.pOleStr) CoTaskMemFree(strr.pOleStr);
			}	
		}
		bOk = true;
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::parseKeyName"))
	{
		bOk = false;
		wsKeyName.clear();
		WRITE_LOG(L"idlNode::parseKeyName SEH�쳣 buf:<%s>", pstr);
	}

	if(ishell2) ishell2->Release();
	if(ishellFolder) ishellFolder->Release();
	return bOk;
}

bool idlNode::parseFileType(PCWSTR pfullPath, std::wstring &wsFileType)
{
	assert(pfullPath);
	if(!pfullPath) return false;
	bool bOk = true;
	__try
	{
		wsFileType.clear();
		SHFILEINFO fileinfo = {0};
		if(::SHGetFileInfo(pfullPath, 0, &fileinfo, sizeof(SHFILEINFO), SHGFI_TYPENAME))
		{
			wsFileType = fileinfo.szTypeName;
		}

	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"idlNode::parseFileType"))
	{
		bOk = false;
		WRITE_LOG(L"idlNode::parseFileType SEH�쳣 fullPath:<%s>", pfullPath);
	}
	return bOk;
}

idlNode* idlNode::createNode(LPITEMIDLIST pidl)
{
	assert(pidl);
	if(!pidl) return nullptr;
	IShellFolder *ishellFolder = nullptr;
	IShellFolder2 *ishell2 = nullptr;
	HRESULT hr = S_OK;
	wchar_t pstr[MAX_PATH] = {0};
	hr = ::SHGetDesktopFolder(&ishellFolder);
	if(FAILED(hr)) return nullptr;
	hr = ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2);
	if(FAILED(hr)) return nullptr;

	// �õ���ʾ������С�����͡��޸�����
	std::wstring wsDisplay, wsSize, wsType, wsModifyDT, wstr;
	SHELLDETAILS shds = {0};
	wchar_t pbuf[MAX_PATH] = {0};
	if (SUCCEEDED(ishell2->GetDetailsOf(pidl, 0, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
	{
		wsDisplay = pbuf;
	}
	else
	{
		WRITE_LOG(L"��ȡ ��ʾ�� ʧ�� %d", GetLastError());
		return nullptr;
	}
	if (SUCCEEDED(ishell2->GetDetailsOf(pidl, 1, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
	{
		wsSize = pbuf;
	}
	//else
	//{
	//	WRITE_LOG(L"��ȡ ��С ʧ�� %d", GetLastError());// �ļ����޴�С
	//	return nullptr;
	//}
	if (SUCCEEDED(ishell2->GetDetailsOf(pidl, 2, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
	{
		wsType = pbuf;
	}
	//else
	//{
	//	WRITE_LOG(L"��ȡ ���� ʧ�� %d", GetLastError()); // ϵͳ�ڵ�������
	//	return nullptr;
	//}
	if (SUCCEEDED(ishell2->GetDetailsOf(pidl, 3, &shds)) && SUCCEEDED(StrRetToBuf(&shds.str, pidl, pbuf, _countof(pbuf))))
	{
		wsModifyDT = pbuf;
	}
	//else
	//{
	//	WRITE_LOG(L"��ȡ �޸����� ʧ��");
	//	return nullptr;
	//}
	//WRITE_LOG(L"idlNode::createNode 0:<%s> 1:<%s> 2:<%s> 3:<%s>", wsDisplay.c_str(), wsSize.c_str(), wsType.c_str(), wsModifyDT.c_str());
	idlNode *pNode = new idlNode(wsDisplay, wsSize, wsType, wsModifyDT);
	assert(pNode);
	if(!pNode) return nullptr;

	// ����
	SFGAOF attribute = -1;
	if (SUCCEEDED(ishell2->GetAttributesOf(1, (LPCITEMIDLIST*)&pidl, &attribute)))
	{
		//WRITE_LOG(L"idlNode::createNode ���� %08X %s", attribute, wsDisplay.c_str());
		pNode->setAttributes(attribute);
	}
	// 
	// ȫ·�� �� ::{CLSID}
	STRRET strr = {0};
	//WRITE_LOG(L"sssssssssssssssssssssssssssssssss %s", wsDisplay.c_str());
	hr = ishell2->GetDisplayNameOf(pidl, SHGDN_NORMAL | SHGDN_FORPARSING, &strr);
	if (SUCCEEDED(hr))
	{
		hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
		if(SUCCEEDED(hr))
		{
			//WRITE_LOG(L"1 ȫ·����%s", pstr);
			pNode->setFullPath(pstr);
		}
		if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
	}
	// �ļ���������չ������ ::{CLSID}
	hr = ishell2->GetDisplayNameOf(pidl, SHGDN_INFOLDER | SHGDN_FORPARSING, &strr);
	if (SUCCEEDED(hr))
	{
		hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
		if(SUCCEEDED(hr))
		{
			//WRITE_LOG(L"2 �ļ�����%s", pstr);
			pNode->setFileName(pstr);
		}
		if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
	}
	//hr = ishell2->GetDisplayNameOf(pidl, SHGDN_INFOLDER | SHGDN_FOREDITING, &strr);
	//if (SUCCEEDED(hr))
	//{
	//	hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
	//	if(SUCCEEDED(hr)) WRITE_LOG(L"3 %s", pstr);
	//	if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
	//}
	//hr = ishell2->GetDisplayNameOf(pidl, SHGDN_INFOLDER, &strr);
	//if (SUCCEEDED(hr))
	//{
	//	hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
	//	if(SUCCEEDED(hr)) WRITE_LOG(L"4 %s", pstr);
	//	if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
	//}
	//hr = ishell2->GetDisplayNameOf(pidl, SHGDN_NORMAL, &strr);
	//if (SUCCEEDED(hr))
	//{
	//	hr = StrRetToBuf(&strr, pidl, pstr, _countof(pstr));
	//	if(SUCCEEDED(hr)) WRITE_LOG(L"5 %s", pstr);
	//	if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
	//}

	//WRITE_LOG(L"idlNode::createNode %s=%s=%s=%s *** %s=%s", wsDisplay.c_str(), wsSize.c_str(), wsType.c_str(), wsModifyDT.c_str(), pNode->getFileName().c_str(), pstr);
	// �ļ���С
	VARIANT var;
	SHCOLUMNID column={PSGUID_STORAGE,PID_STG_SIZE};
	if (SUCCEEDED(ishell2->GetDetailsEx(pidl, &column, &var)))
	{
		pNode->setFileSize(std::size_t(var.ullVal));
	}
	// ����Ŀ¼
	column.pid = PID_STG_DIRECTORY;
	if(SUCCEEDED(ishell2->GetDetailsEx(pidl, &column, &var)))
	{
		pNode->setDirectory(var.bstrVal);
	}
	// ��������
	column.pid = PID_STG_WRITETIME;
	if(SUCCEEDED(ishell2->GetDetailsEx(pidl, &column, &var)))
	{
		pNode->setCreatedTime(var.date);
	}
	//column.pid = PID_STG_NAME;
	//if(SUCCEEDED(ishell2->GetDetailsEx(pidl, &column, &var)))
	//{	// xp�²�����չ�������в�����
	//	WRITE_LOG(L"idlNode::createNode  PID_STG_NAME %s", var.bstrVal);
	//}
	// ���� ITEMIDLIST
	//pNode->copyIIDList(pidl);

	if(ishellFolder) ishellFolder->Release();
	if(ishell2) ishell2->Release();
	return pNode;

}

bool idlNode::isTemporaryFile(LPITEMIDLIST pidl)
{
	if(!pidl) return false;
	wchar_t ppath[MAX_PATH] = {0};
	if (SHGetPathFromIDList(pidl, ppath))
	{
		return isTemporaryFile(ppath);
	}
	return false;
}
bool idlNode::isTemporaryFile(PCWSTR pfullPath)
{
	if(!pfullPath && std::wcslen(pfullPath) <= 0) return false;
	std::wstring fullPath = pfullPath;
	return isTemporaryFile(fullPath);
}
bool idlNode::isTemporaryFile(std::wstring fullPath)
{
	if(fullPath.empty()) return false;
	bool isTemp = false;
	std::wstring wsstr;

	std::wstring::size_type pos = std::wstring::npos;
	pos = fullPath.rfind(L'\\');
	//SEND_LOG(L"WARNING idlNode::isTemporaryFile AAA  attr:%08X path:%s", GetFileAttributes(ppath), wsPath.c_str());
	if (pos != std::wstring::npos)
	{
		++pos;	// ȥ��L'\\'
		//SEND_LOG(L"WARNING idlNode::isTemporaryFile BBB %s", wsPath.substr(pos, 1).c_str());
		if(fullPath.substr(pos, 2) == L"~$")
		{
			pos = fullPath.rfind(L'.');
			//SEND_LOG(L"WARNING idlNode::isTemporaryFile DDD %s", wsPath.substr(pos).c_str());
			if (pos != std::wstring::npos)
			{
				wsstr = gtc::convert::toLower(fullPath.substr(pos).c_str());

				if(wsstr == L".docx" || wsstr == L".doc" ||
					wsstr == L".xlsx" || wsstr == L".xls" ||
					wsstr == L".pptx" || wsstr == L".ppt") isTemp = true;
			}
		}
		else if(fullPath.substr(pos, 1) == L"~")
		{
			pos = fullPath.rfind(L'.');
			//SEND_LOG(L"WARNING idlNode::isTemporaryFile CCC %s", wsPath.substr(pos).c_str());
			if (pos != std::wstring::npos && gtc::convert::toLower(fullPath.substr(pos).c_str()) == L".tmp")
			{
				isTemp = true;
			}

		}
		else if (fullPath.substr(pos, 4) == L"δȷ�� ")
		{
			pos = fullPath.rfind(L'.');
			//SEND_LOG(L"WARNING idlNode::isTemporaryFile CCC %s", wsPath.substr(pos).c_str());
			if (pos != std::wstring::npos && gtc::convert::toLower(fullPath.substr(pos).c_str()) == L".crdownload")
			{
				isTemp = true;
			}
		}

		if(!isTemp)
		{
			pos = fullPath.rfind(L'.');
			if (pos != std::wstring::npos)
			{
				std::wstring wsext = gtc::convert::toLower(fullPath.substr(pos).c_str());
				// .crdownload ��������Ϊ�����ļ�ʱ��������ʱ�ļ�
				// .tdl .tdlex QQ�����ļ�ʱ��������ʱ�ļ�
				// .td .td.cfg
				if (wsext == L".tmp" || wsext == L".crdownload" || wsext == L".partial" ||
					wsext == L".tdl" || wsext == L".tdlex" || 
					wsext == L".td")
				{
					isTemp = true;
				}
				else if (wsext == L".cfg")
				{	// .td.cfg
					wsext = fullPath.substr(0, pos);
					pos = wsext.rfind(L'.');
					if(pos != std::wstring::npos)
						wsext = gtc::convert::toLower(wsext.substr(pos).c_str());
					isTemp = wsext == L".td";
				}
			}

		}
	}

	return isTemp;
}

bool idlNode::exists(std::wstring fullPath)
{
	PIDLIST_ABSOLUTE piidl = NULL;
	bool bExists = false;
	if (SUCCEEDED(::SHParseDisplayName(fullPath.c_str(), NULL, &piidl, 0, NULL)))
	{
		bExists = piidl != NULL;
		CoTaskMemFree(piidl);
	}
	return bExists;
}
void idlNode::enumFileTypeExtensionInfo()
{
	mc_mapFileTypeExtension.clear();
	acm::regKey reg;
	if (reg.open(HKEY_CLASSES_ROOT, L"", KEY_ENUMERATE_SUB_KEYS))
	{
		bool have = false;
		DWORD idx = 0, count = 0;
		std::wstring wsExt;
		SHFILEINFO shfi = {0};
		//WRITE_LOG(L"---- enumFileTypeExtensionInfo A --------------------------------------------------------");
		while(reg.enumKey(idx++, wsExt))
		{
			if(wsExt.empty() || wsExt[0] != L'.')
			{
				if(count > 0) break;
				continue;
			}
			//if(wsExt == L".wtx" || wsExt == L".log" || wsExt == L".scp" ||
			//	wsExt == L".dib") continue;
			if (::SHGetFileInfo(wsExt.c_str(),FILE_ATTRIBUTE_NORMAL, &shfi,sizeof(shfi), SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME))
			{
				mc_mapFileTypeExtension.insert(std::pair<std::wstring, std::wstring>(shfi.szTypeName,wsExt));
				//WRITE_LOG(L"%s\t\t%s", wsExt.c_str(), shfi.szTypeName);
				++count;
			}
			else
			{
				WRITE_LOG(L"enumFileTypeExtensionInfo ʧ�� %s", wsExt.c_str());
			}
		}
		//WRITE_LOG(L"---- enumFileTypeExtensionInfo B --------------------------------------------------------");
	}
}

void idlNode::enumDeskFileTypeExtInfo()
{
	IShellFolder *ishellFolder = nullptr;
	IShellFolder2 *ishell2 = nullptr;
	HRESULT hr = S_OK;
	ULONG celt = 0;
	LPENUMIDLIST peids = nullptr;
	LPITEMIDLIST pidlItems = nullptr;
	//WRITE_LOG(L"---- enumFileTypeExtensionInfo A --------------------------------------------------------");
	mc_mapFileTypeExtension.clear();
	// 
	if(FAILED(::SHGetDesktopFolder(&ishellFolder))) return;
	if(FAILED(ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&ishell2))) return;
	hr = ishell2->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &peids);
	assert(hr == S_OK);
	if(FAILED(hr)) return;
	SHELLDETAILS shds = {0};
	STRRET strr = {0};
	std::wstring wsType, wsExt;
	PWSTR pbuf = NULL;
	wchar_t pstr[MAX_PATH];
	while(peids->Next(1, &pidlItems, &celt) == S_OK && celt == 1)
	{
		wsType.clear();
		wsExt.clear();
		// ����
		if (SUCCEEDED(ishell2->GetDetailsOf(pidlItems, 2, &shds)) && SUCCEEDED(StrRetToStr(&shds.str, pidlItems, &pbuf)))
		{
			wsType = pbuf;
			CoTaskMemFree(pbuf);
		}
		// ��չ��
		// �ļ���������չ������ ::{CLSID}
		hr = ishell2->GetDisplayNameOf(pidlItems, SHGDN_INFOLDER | SHGDN_FORPARSING, &strr);
		if (SUCCEEDED(hr))
		{
			hr = StrRetToBuf(&strr, pidlItems, pstr, _countof(pstr));
			if(SUCCEEDED(hr))
			{
				wsExt = pstr;
				std::wstring::size_type pos = wsExt.rfind(L'.');
				if(pos != std::wstring::npos)
					wsExt = wsExt.substr(pos);
				else
					wsExt = L"";
			}
			if(strr.uType == STRRET_WSTR) CoTaskMemFree(strr.pOleStr);
		}
		if(!wsExt.empty() && !wsType.empty())
			addFileTypeExtInfo(wsType, wsExt);
		CoTaskMemFree(pidlItems);
	}
	//WRITE_LOG(L"---- enumFileTypeExtensionInfo B --------------------------------------------------------");
}

void idlNode::addFileTypeExtInfo(std::wstring &wsType, std::wstring &wsExt)
{
	std::multimap<std::wstring, std::wstring>::const_iterator iter = mc_mapFileTypeExtension.find(wsType);
	if (iter != mc_mapFileTypeExtension.cend())
	{	// �鿴��չ���Ƿ���ڣ������ж��ͬ�������Ƶ���չ��
		bool bExt = false;
		for (iter = mc_mapFileTypeExtension.equal_range(wsType).first; iter != mc_mapFileTypeExtension.equal_range(wsType).second; ++iter)
		{
			if (iter->second == wsExt)
			{
				bExt = true;
				break;
			}
		}
		if(!bExt)
			mc_mapFileTypeExtension.insert(std::pair<std::wstring, std::wstring>(wsType, wsExt));
	}
	else
	{
		mc_mapFileTypeExtension.insert(std::pair<std::wstring, std::wstring>(wsType, wsExt));
	}
}
// 
std::multimap<std::wstring, std::wstring>& idlNode::getFileTypeExtensionInfo()
{
	return mc_mapFileTypeExtension;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	kpdeskAdmin��	2014-04-30
//	������Ϣ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
static wchar_t nc_pstrLockDesc[MAX_PATH] = {0};
static std::wstring stringRight(const std::wstring &wstr, int right = 10)
{
	if(wstr.empty()) return L"";
	if (wstr.length() > right)
	{
		return wstr.substr(wstr.length()-right);
	}
	return wstr;
}

kpdeskAdmin::kpdeskAdmin()
	: m_hDeskLV(NULL)
	//, m_pshFolder(NULL)
{
	::InitializeCriticalSection(&m_csLockNodes);
	//::InitializeCriticalSectionAndSpinCount(&m_csLockNodes, 2000);
	memset(&m_szOffset, 0, sizeof(m_szOffset));
	memset(&m_szMaxNode, 0, sizeof(m_szMaxNode));
}
kpdeskAdmin::kpdeskAdmin(const kpdeskAdmin &admin)
{
}
kpdeskAdmin& kpdeskAdmin::operator =(const kpdeskAdmin &admin)
{
	return *this;
}

kpdeskAdmin::~kpdeskAdmin()
{
	close();
	::DeleteCriticalSection(&m_csLockNodes);
}

const gtc::idlNode::IDINodeMap& kpdeskAdmin::getNodes() const 
{		
	return m_mapNodes;
}

SIZE kpdeskAdmin::getNodeMaxSize() const
{
	return m_szMaxNode;
}
SIZE kpdeskAdmin::getNodeOffset() const
{
	return m_szOffset;
}
void kpdeskAdmin::saveNodePosition()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;

	wchar_t pstr[MAX_PATH] = {0};
	int nodeIndex = -1;
	POINT pt = {0};
	int nodes = 0;
	std::wstring wsv;
	for (gtc::idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); )
	{
		if ((nodeIndex = findNodeIndex(iter->second)) >= 0)
		{
			// ��¼�ڵ��������ߴ�
			setNodeListViewInfo(iter->second, nodeIndex, &pt);
			// �ڵ�ԭʼλ�����걣��
			std::swprintf(pstr, _countof(pstr), L"NODE%d", nodes++);
			wsv = iter->first;
			wsv	+= L"*";
			wsv += gtc::format::point(pt);
			gtc::app::writeProfile(L"DESKPOS", pstr, wsv.c_str());

			++iter;
		}
		else
		{
			DEBUG_CS_LOG(L"saveItemPosition �޷��������ҵ��ڵ㡰%s��=%s", iter->first.c_str(), iter->second->toString().c_str());
			delete iter->second;
			iter = m_mapNodes.erase(iter);
		}
	}
	// �����ܽڵ���
	gtc::app::writeProfile(L"DESKPOS", L"NODES", nodes);

	
}

void kpdeskAdmin::makeNode(idlNode *pNode)
{
	if(!pNode) return;
	m_mapNodes[pNode->getKeyName()] = pNode;
	std::wstring wstr = pNode->getFileName();
	if (wstr.length() > 1 && wstr.substr(0, 2) == L"::")
	{
		m_mapSystemNodes[pNode->getName()] = wstr;
	}
}

inline idlNode* kpdeskAdmin::wrapExistsNode(int nodeIndex)
{
	idlNode *pNode = nullptr;
	idlNode::IDINodeMap::const_iterator iter;
	// �鿴�����Ƿ����
	for (iter = m_mapNodes.cbegin(); iter != m_mapNodes.cend(); ++iter)
	{
		if (iter->second->getIndex() == nodeIndex)
		{
			if (isEqualNode(nodeIndex, iter->second))
			{
				pNode = iter->second;
			}
			break;
		}
	}
	return pNode;
}

inline idlNode* kpdeskAdmin::wrapExistsNode(const std::wstring &keyName)
{
	idlNode *pNode = nullptr;
	idlNode::IDINodeMap::const_iterator iter;

	iter = m_mapNodes.find(keyName);
	if(iter != m_mapNodes.cend())
		pNode = iter->second;

	return pNode;
}

int kpdeskAdmin::wrapMoveto(const std::wstring &keyName, POINT pt)
{
	idlNode::IDINodeMap::const_iterator iter;
	idlNode *pNode = nullptr;
	int nodeIndex = -1;
	RECT rc = {0};
	iter = m_mapNodes.find(keyName);
	if (iter != m_mapNodes.cend())
	{
		pNode = iter->second;
		nodeIndex = pNode->getIndex();
		if (isEqualNode(nodeIndex, pNode))
		{
			if (pNode->isHide())
			{
				ListView_GetItemRect(m_hDeskLV, nodeIndex, &rc, LVIR_BOUNDS);
				if(rc.left >= 0 ||rc.top >= 0)
					ListView_SetItemPosition(m_hDeskLV, nodeIndex, KPDESK_HIDENODE_POSX, KPDESK_HIDENODE_POSY);
			}
			else
			{
				pt.x += m_szOffset.cx;
				pt.y += m_szOffset.cy;
				//WRITE_LOG(L"kpdeskAdmin::moveto idx:%d pos:<%d,%d> name:<%s>", nodeIndex, pt.x, pt.y, keyName.c_str());
				ListView_SetItemPosition(m_hDeskLV, nodeIndex, pt.x, pt.y);
			}
			return 1;
		}
	}

	return 0;
}
void kpdeskAdmin::restoreNodePosition()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;
	//// go
	//int nodes = gtc::app::readProfileInt(L"DESKPOS", L"NODES");
	int nodes = 0;
	gtc::app::readProfileInt(L"DESKPOS", L"NODES", nodes);
	wchar_t pkey[20];
	std::wstring wsvalue;
	std::vector<std::wstring> vecSub;
	int idx;
	DEBUG_CS_LOG(L"kpdeskAdmin::restoreNodePosition EnterCriticalSection 1");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::restoreNodePosition EnterCriticalSection 2 %u", GetTickCount());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::restoreNodePosition");
	// ʹ�ڵ�ɼ�
	for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		iter->second->setShowFlag(idlNode::BENodeShowFlagShow);
	}
	//idlNode::IDINodeMap::const_iterator iter;
	//POINT pt;
	//int nodeIndex;
	for (idx = 0; idx < nodes; ++idx)
	{
		std::swprintf(pkey, _countof(pkey), L"NODE%d", idx);
		//// go
		gtc::app::readProfile(L"DESKPOS", pkey, wsvalue);
		//wsvalue = gtc::app::readProfile(L"DESKPOS", pkey);
		if (wsvalue.find(L'*') != std::wstring::npos && gtc::convert::splitString(wsvalue, vecSub, L'*'))
		{
			assert(vecSub.size() == 2);
			//if(vecSub.size() == 2 && (iter = m_mapNodes.find(vecSub[0])) != m_mapNodes.cend())
			//{
			//	nodeIndex = iter->second->getIndex();
			//	pt = gtc::format::point(vecSub[1].c_str());
			//	WRITE_LOG(L"kpdeskAdmin::restoreNodePosition idx:%d pos:<%s><%d,%d> name:<%s>", nodeIndex, vecSub[1].c_str(), pt.x, pt.y, vecSub[0].c_str());
			//	if (nodeIndex >= 0)
			//	{
			//		ListView_SetItemPosition(m_hDeskLV, nodeIndex, pt.x, pt.y);
			//	}
			//	else
			//		moveto(vecSub[0], pt);
			//}
			if(vecSub.size() == 2 && existsNode(vecSub[0]) != NULL)
			{
				//WRITE_LOG(L"kpdeskAdmin::restoreNodePosition pos:<%s> name:<%s>", vecSub[1].c_str(), vecSub[0].c_str());
				moveto(vecSub[0], gtc::format::point(vecSub[1].c_str()));
			}
			//////////////////////////////////////////////////////////////////////////
			////if(vecSub.size() != 2)
			////	SEND_LOG(L"restoreItemPosition %d %s", vecSub.size(), wsvalue.c_str());
			//gtc::idlNode *pNode = findNode(vecSub[0]);
			////DEBUG_CS_LOG(L"restoreItemPosition node:%08X keyName:%s pos:%s", pNode, vecSub[0].c_str(), vecSub[1].c_str());
			//if(pNode)
			//{
			//	//POINT pt = gtc::format::point(vecSub[1].c_str());
			//	//PSTMSG((kpdeskListView::mc_hDeskListView), LVM_SETITEMPOSITION32, (WPARAM)pNode->getIndex(), (LPARAM)&pt);
			//	moveto(pNode, gtc::format::point(vecSub[1].c_str()));
			//}
			////pt = gtc::format::point(vecSub[1].c_str());
			////if((idx = findNodeIndex(vecSub[0])) >= 0)
			////{
			////	moveto(idx, pt);
			////}
		}
	}
	// �����صĽڵ���ʾ����
	RECT rc = {0};
	for (idx = 0; idx < ListView_GetItemCount(m_hDeskLV); ++idx)
	{
		ListView_GetItemRect(m_hDeskLV, idx, &rc, LVIR_BOUNDS);
		if (rc.left < 0 || rc.top < 0)
		{
			//WRITE_LOG(L"kpdeskAdmin::restoreNodePosition �����صĽڵ���ʾ���� idx:%d pos:<%d,%d>", idx, rc.left, rc.top);
			//if(rc.left < 0)
			//	rc.left = m_szOffset.cx;
			//if(rc.top < 0)
			//	rc.top = m_szOffset.cy;
			//ListView_SetItemPosition(m_hDeskLV, idx, rc.left, rc.top);
			ListView_SetItemPosition(m_hDeskLV, idx, m_szOffset.cx, m_szOffset.cy);
		}
	}

	::LeaveCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::restoreNodePosition LeaveCriticalSection %u", GetTickCount());
}
/*****************************************************************************************
	<< --- kpdeskAdmin::open		2014-05-30 --- >>
	˵�����õ�����ӿ�ָ�룬�ڴ����ָ��
	������
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool kpdeskAdmin::open(HWND hLVWnd)
{
	assert(hLVWnd);
	if(m_hDeskLV) return true;
	HRESULT hr = S_OK;
	//IShellFolder *ishellFolder = nullptr;
	try
	{
		//hr = ::SHGetDesktopFolder(&ishellFolder);
		//if(FAILED(hr)) return false;
		//hr = ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&m_pshFolder);
		//if(FAILED(hr)) return false;
		m_hDeskLV = hLVWnd;
		// �ڵ�ƫ����
		refreshNodeOffset();
		// ͼ��ڵ��С
		refreshNodeMaxSize();
		
	}
	catch (...)
	{
		gtc::app::lastError();
		close();
	}
	return hLVWnd != NULL;
}

bool kpdeskAdmin::isOpen() const
{
	return /*m_pshFolder != NULL &&*/ m_hDeskLV != NULL;
}
/*****************************************************************************************
	<< --- kpdeskAdmin::close		2014-05-30 --- >>
	˵�����Ƿ�open�����ָ��
	������
	����ֵ��
*****************************************************************************************/
void kpdeskAdmin::close()
{
	//if (m_pshFolder)
	//{
	//	m_pshFolder->Release();
	//	m_pshFolder = NULL;
	//}
	free();
}

/*****************************************************************************************
	<< --- kpdeskAdmin::enumNodes		2014-05-30 --- >>
	˵����ö������ڵ�
	������
	����ֵ��
		>= 0	�ɹ����ڵ���
		< 0		ʧ��
*****************************************************************************************/
int kpdeskAdmin::enumNodes()
{
	//assert(m_pshFolder);
	HRESULT hr = S_OK;
	ULONG celt = 0;
	LPENUMIDLIST peids = nullptr;
	LPITEMIDLIST pidlItems = nullptr;
	gtc::idlNode *pNode = nullptr;
	//std::wstring wstr;
	IShellFolder *ishellFolder = nullptr;
	IShellFolder2 *pshFolder = nullptr;
	bool bEnter = false;
	__try
	{
		hr = ::SHGetDesktopFolder(&ishellFolder);
		if(FAILED(hr)) __leave;
		hr = ishellFolder->QueryInterface(IID_IShellFolder2, (void **)&pshFolder);
		if(FAILED(hr)) __leave;
		// 
		hr = pshFolder->EnumObjects(nullptr, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN, &peids);
		if(FAILED(hr)) __leave;

		WRITE_LOG(L"kpdeskAdmin::enumNodes EnterCriticalSection 1");
		::EnterCriticalSection(&m_csLockNodes);
		WRITE_LOG(L"kpdeskAdmin::enumNodes EnterCriticalSection 2 %u", GetTickCount());
		bEnter = true;
		std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::enumNodes");
		// 
		m_mapSystemNodes.clear();
		free();
		// ��shell��ö�ٽڵ�
		while(SUCCEEDED(peids->Next(1, &pidlItems, &celt)) && celt == 1)
		{
			pNode = gtc::idlNode::createNode(pidlItems);
			if(pNode)
			{
				if (pNode->getName().empty() || idlNode::isTemporaryFile(pidlItems))
				{
					WRITE_LOG(L"kpdeskAdmin::enumNodes ��ʾ��Ϊ�գ�=<%s>", pNode->getName().c_str());
					delete pNode;
					CoTaskMemFree(pidlItems);
					continue;
				}
				makeNode(pNode);
				//m_mapNodes[pNode->getKeyName()] = pNode;
				//wstr = pNode->getFileName();
				//if (wstr.length() > 1 && wstr.substr(0, 2) == L"::")
				//{
				//	m_mapSystemNodes[pNode->getName()] = wstr;
				//}
			}
			else
			{
				WRITE_LOG(L"kpdeskAdmin::enumNodes createNodeʧ��");
			}
			CoTaskMemFree(pidlItems);
		}

		// 
		saveNodePosition();
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"kpdeskAdmin::enumNodes"))
	{
		free();
		WRITE_LOG(L"kpdeskAdmin::enumNodes SEH�쳣 ");
	}
	if(bEnter)
	{
		::LeaveCriticalSection(&m_csLockNodes);
		WRITE_LOG(L"kpdeskAdmin::enumNodes LeaveCriticalSection %u", GetTickCount());
	}
	if(peids) peids->Release();
	if(pshFolder) pshFolder->Release();
	if(ishellFolder) ishellFolder->Release();
	return m_mapNodes.size();
}

int kpdeskAdmin::size() const
{
	return int(m_mapNodes.size());
}

/*****************************************************************************************
	<< --- kpdeskAdmin::freeNodes		2014-05-30 --- >>
	˵�����ͷŲ���սڵ�
	������
	����ֵ��
*****************************************************************************************/
void kpdeskAdmin::free()
{
	idlNode::free(m_mapNodes);
}

void kpdeskAdmin::freeNone()
{
	DEBUG_CS_LOG(L"kpdeskAdmin::freeNone EnterCriticalSection 1 ");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::freeNone EnterCriticalSection 2 ");
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::freeNone");
	for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		if (iter->second->getOptFlag() == 0)
		{
			SEND_LOG(L"kpdeskAdmin::freeNone KPDESK_WMME_DELETENODE ����ɾ�� name:<%s>", iter->first.c_str());
			SendMessage(m_hDeskLV, KPDESK_WMME_DELETENODE, (WPARAM)(new std::wstring(iter->first)), 0);
		}
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::freeNone LeaveCriticalSection   ");
	::LeaveCriticalSection(&m_csLockNodes);
}

void kpdeskAdmin::clearOptFlag()
{
	DEBUG_CS_LOG(L"kpdeskAdmin::clearOptFlag EnterCriticalSection 1 ");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::clearOptFlag EnterCriticalSection 2 ");
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::clearOptFlag");
	for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		iter->second->setOptFlag(0);
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::clearOptFlag LeaveCriticalSection   ");
	::LeaveCriticalSection(&m_csLockNodes);
}

std::vector<std::wstring> kpdeskAdmin::getNodeKeyNames(BEDeskGroupFlag groupFlag)
{
	std::vector<std::wstring> vecKeys;
	DEBUG_CS_LOG(L"kpdeskAdmin::getNodeKeyNames EnterCriticalSection 1 ");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::getNodeKeyNames EnterCriticalSection 2 ");
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::getNodeKeyNames_gpflag(%d)", groupFlag);
	WRITE_LOG(L"kpdeskAdmin::getNodeKeyNames groupFlag:%d", groupFlag);
	for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		if(iter->second->getGroupType() == groupFlag) vecKeys.push_back(iter->first);
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::getNodeKeyNames LeaveCriticalSection   ");
	::LeaveCriticalSection(&m_csLockNodes);

	return vecKeys;

}
idlNode* kpdeskAdmin::existsNode(const std::wstring &keyName)
{
	idlNode *pNode = nullptr;
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode EnterCriticalSection 1 <%s>", keyName.c_str());
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::existsNode_(%s)", stringRight(keyName).c_str());
	idlNode::IDINodeMap::iterator iter = m_mapNodes.find(keyName);
	if (iter != m_mapNodes.end())
	{
		pNode = iter->second;
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode LeaveCriticalSection   %u <%s>", GetTickCount(), keyName.c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	return pNode;

}

inline idlNode* kpdeskAdmin::existsNodeSEH(const std::wstring &keyName)
{
	idlNode *pNode = nullptr;
	__try
	{
		pNode = wrapExistsNode(keyName);
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"kpdeskAdmin::existsNodeSEH_name"))
	{
		pNode = nullptr;
		WRITE_LOG(L"kpdeskAdmin::existsNodeSEH_name SEH�쳣  name:<%s>", keyName.c_str());
	}

	return pNode;
}
idlNode* kpdeskAdmin::existsNode(const int nodeIndex)
{
	assert(m_hDeskLV && nodeIndex >= 0);
	if(nodeIndex < 0 || nodeIndex >= ListView_GetItemCount(m_hDeskLV) || !m_hDeskLV) return nullptr;
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode EnterCriticalSection 1 <%d>", nodeIndex);
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode EnterCriticalSection 2 %u <%d>", GetTickCount(), nodeIndex);
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::existsNode_idx(%d)", nodeIndex);
	idlNode *pNode = nullptr;
	idlNode::IDINodeMap::iterator iter;
	// �鿴�����Ƿ����
	for (iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		if (iter->second->getIndex() == nodeIndex)
		{
			if (isEqualNode(nodeIndex, iter->second))
			{
				pNode = iter->second;
				break;
			}
			else
			{
				// ԭ���ڵ���������
				int idx = findNodeIndex(iter->second);
				if(idx >= 0)
					setNodeListViewInfo(iter->second, idx);
				else
				{
					iter->second->setIndex(-1);
					if (!gtc::idlNode::exists(iter->second->getFullPath()))
					{
						SEND_LOG(L"ERROR kpdeskAdmin::existsNode KPDESK_WMME_DELETENODE �ڵ㲻��shell�� name:<%s> fullPath:<%s>", iter->second->getName().c_str(), iter->second->getFullPath().c_str());
						SendMessage(m_hDeskLV, KPDESK_WMME_DELETENODE, (WPARAM)(new std::wstring(iter->second->getKeyName())), 0);
					}
				}
				//SEND_LOG(L"kpdeskAdmin::existsNode ���ýڵ����� idx��%d newIdx:%d ԭ���ڵ��������� name:<%s>", nodeIndex, iter->second->getIndex(), iter->first.c_str());

			}
		}
	}
	// �ڼ����в���
	if (!pNode)
	{
		wchar_t pstr[MAX_PATH] = {0};
		std::wstring wsName, wsType, wsSize, wsDT;
		// ��ʾ��
		ListView_GetItemText(m_hDeskLV, nodeIndex, 0, pstr, _countof(pstr));
		wsName = pstr;
		// ����
		ListView_GetItemText(m_hDeskLV, nodeIndex, 2, pstr, _countof(pstr));
		wsType = pstr;
		// ��С��������Ϊ�ˣ�1�����֡��������桱�롰�û����桱��ͬ���ڵ㣬�磺��ѶQQ.lnk��2�����ֲ�ͬ��չ������ʾ������ͬ�Ľڵ㣬�磺abc.txt, abc.wtx��
		ListView_GetItemText(m_hDeskLV, nodeIndex, 1, pstr, _countof(pstr));
		wsSize = pstr;
		// �޸�ʱ�䣬������3
		ListView_GetItemText(m_hDeskLV, nodeIndex, 3, pstr, _countof(pstr));
		wsDT = pstr;
		wsDT = gtc::convert::deleteNoGraph(wsDT);
		// 
		for (iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
		{
			if (isEqualNode(wsName, wsSize, wsType, wsDT, iter->second))
			{
				//SEND_LOG(L"kpdeskAdmin::existsNode ���ýڵ����� idx:%d LVName:<%s> LVType:<%s> name:%d<%s>", nodeIndex, wsName.c_str(), wsType.c_str(), iter->second->getIndex(), iter->second->getName().c_str());
				setNodeListViewInfo(iter->second, nodeIndex);
				//iter->second->setIndex(nodeIndex);
				pNode = iter->second;
				break;
			}
		}
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::existsNode LeaveCriticalSection   %u <%d>", GetTickCount(), nodeIndex);
	::LeaveCriticalSection(&m_csLockNodes);

	return pNode;

}

inline idlNode* kpdeskAdmin::existsNodeSEH(const int nodeIndex)
{
	idlNode *pNode = nullptr;
	__try
	{
		pNode = wrapExistsNode(nodeIndex);
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"kpdeskAdmin::existsNodeSEH"))
	{
		pNode = nullptr;
		WRITE_LOG(L"kpdeskAdmin::existsNodeSEH SEH�쳣 idx:%d ", nodeIndex);
	}

	return pNode;
}
gtc::idlNode* kpdeskAdmin::findNode(const std::wstring &keyName, PCWSTR pDesc)
{
	if(keyName.empty()) return nullptr;
	gtc::idlNode *pNode = nullptr;
	idlNode::IDINodeMap::const_iterator iter;
	//SEND_LOG(L"kpdeskAdmin::findNode_name desc:<%s> name<%s>", pDesc?pDesc:L"", keyName.c_str());
	//bool bbug = false;
	//try
	//{
	//	iter = m_mapNodes.find(keyName);
	//	if(iter != m_mapNodes.end())
	//		pNode = iter->second;
	//}
	//catch (...)
	//{
	//	SEND_LOG(L"ERROR kpdeskAdmin::findNode_name name �쳣 �쳣 �쳣������");
	//	bbug;
	//}
	//if(pNode) return pNode;
	//if(!bbug) return nullptr;
	pNode = existsNodeSEH(keyName);
	if(pNode) return pNode;

	DEBUG_CS_LOG(L"kpdeskAdmin::findNode_name keyName EnterCriticalSection 1 <%s>", keyName.c_str());
	//::EnterCriticalSection(&m_csLockNodes);
	//DEBUG_CS_LOG(L"kpdeskAdmin::findNode keyName EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::findNode TryEnterCriticalSection 1 idx:<%d> ���ݼ��Ѿ�������������", nodeIndex);
		SEND_LOG(L"WARNING kpdeskAdmin::findNode_name TryEnterCriticalSection 1 name:<%s> lock:<%s> ���ݼ��Ѿ�������������", keyName.c_str(), nc_pstrLockDesc);
		//PostMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		return nullptr;
	}
	//const int tryCount = 2;
	//int atry = 0;
	//bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//while(!bEnter)
	//{
	//	if(atry < tryCount)
	//	{
	//		++atry;
	//		Sleep(30);
	//		SEND_LOG(L"WARNING kpdeskAdmin::findNode keyName TryEnterCriticalSection 1 trys:%d name:<%s> lock:<%s> ���ݼ��Ѿ�������������", atry, keyName.c_str(), nc_pstrLockDesc);
	//		bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//	}
	//	else
	//		break;
	//	//return nullptr;
	//}
	//if(!bEnter) return nullptr;
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::findNode_name(%s)", stringRight(keyName).c_str());
	iter = m_mapNodes.find(keyName);
	if(iter != m_mapNodes.cend())
		pNode = iter->second;

	DEBUG_CS_LOG(L"kpdeskAdmin::findNode_name keyName LeaveCriticalSection   %u <%s>", GetTickCount(), keyName.c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	return pNode;
}
int kpdeskAdmin::findNode(const int nodeIndex, gtc::idlNode *&pNode)
{
	static int c_ioldNodeIndex = -1;
	assert(m_hDeskLV);
	if(!m_hDeskLV || nodeIndex < 0) return 0;
	//idlNode::IDINodeMap::const_iterator iter;
	pNode = existsNodeSEH(nodeIndex);
	//try
	//{
	//	// �鿴�����Ƿ����
	//	for (iter = m_mapNodes.cbegin(); iter != m_mapNodes.cend(); ++iter)
	//	{
	//		if (iter->second->getIndex() == nodeIndex)
	//		{
	//			if (isEqualNode(nodeIndex, iter->second))
	//			{
	//				pNode = iter->second;
	//			}
	//			break;
	//		}
	//	}
	//}
	//catch (...)
	//{
	//	gtc::app::writeVSLogs(L"kpdeskAdmin::findNode index �쳣 �쳣 �쳣������");
	//	SEND_LOG(L"ERROR kpdeskAdmin::findNode index �쳣 �쳣 �쳣������");
	//}
	if(pNode) return 1;
	DEBUG_CS_LOG(L"kpdeskAdmin::findNode_index EnterCriticalSection 1 idx:<%d>", nodeIndex);
	//::EnterCriticalSection(&m_csLockNodes);
	//DEBUG_CS_LOG(L"kpdeskAdmin::findNode EnterCriticalSection 2 %u idx:<%d>", GetTickCount(), nodeIndex);

	//const int tryCount = 10;
	//int atry = 0;
	//bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//while(!bEnter)
	//{
	//	if(atry < tryCount)
	//	{
	//		++atry;
	//		Sleep(20);
	//		SEND_LOG(L"WARNING kpdeskAdmin::findNode TryEnterCriticalSection 1 trys:%d idx:<%d> ���ݼ��Ѿ�������������", atry, nodeIndex);
	//		bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//	}
	//	else
	//		break;
	//}
	//if(!bEnter) return nullptr;

	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::findNode TryEnterCriticalSection 1 idx:<%d> ���ݼ��Ѿ�������������", nodeIndex);
		//WRITE_LOG(L"WARNING kpdeskAdmin::findNode_index TryEnterCriticalSection 1 idx:<%d> lock:<%s> ���ݼ��Ѿ�������������", nodeIndex, nc_pstrLockDesc);
		SEND_LOG(L"WARNING kpdeskAdmin::findNode_index TryEnterCriticalSection 1 idx:<%d> lock:<%s> ���ݼ��Ѿ�������������", nodeIndex, nc_pstrLockDesc);
		//PostMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		//redrawNode(nodeIndex);
		return -1;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::findNode_index(%d)", nodeIndex);
	pNode = existsNode(nodeIndex);
	//for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	//{
	//	if (iter->second->getIndex() == nodeIndex)
	//	{
	//		if (isEqualNode(nodeIndex, iter->second))
	//		{
	//			pNode = iter->second;
	//			break;
	//		}
	//		else
	//		{
	//			// ԭ���ڵ���������
	//			iter->second->setIndex(findNodeIndex(iter->second));
	//			DEBUG_CS_LOG(L"kpdeskAdmin::findNode idx��%d newIdx:%d ԭ���ڵ��������� name:<%s>", nodeIndex, iter->second->getIndex(), iter->first.c_str());
	//			
	//		}
	//	}
	//}

	if (!pNode)
	{
		//::SendMessage(m_hDeskLV, WM_SETREDRAW, FALSE, NULL);
		// ���ڵ����������ڼ���������shell�в��ҽڵ�
		ITEMIDLIST *piidl = getNodeIIDLs(nodeIndex);
		if (piidl)
		{
			std::wstring keyName;
			gtc::idlNode::parseKeyName(piidl, keyName);
			pNode = findNode(keyName, L"kpdeskAdmin::findNode_Index");
			if (pNode)
			{
				SEND_LOG(L"kpdeskAdmin::findNode_index idx��%d ���½ڵ� name:<%s", nodeIndex, keyName.c_str());
				pNode->update(piidl);
				setNodeListViewInfo(pNode, nodeIndex);
			}
			else 
			{
				if (c_ioldNodeIndex != nodeIndex)
				{
					SEND_LOG(L"kpdeskAdmin::findNode_index KPDESK_WMME_ADDNODE oldidx:%d idx��%d ׼�����ڵ�׷�ӵ��������� name:<%s>", c_ioldNodeIndex, nodeIndex, keyName.c_str());
					c_ioldNodeIndex = nodeIndex;
					SendMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(keyName)), NULL);
				}
			}
			CoTaskMemFree(piidl);
			piidl = NULL;
		}
		else
		{
			wchar_t pbuf[MAX_PATH] = {0};
			ListView_GetItemText(m_hDeskLV, nodeIndex, 0, pbuf, _countof(pbuf));
			SEND_LOG(L"ERROR kpdeskAdmin::findNode_index �޷���shell���ҵ��ڵ�����Ϊ%d�Ľڵ㣡���� lvName:<%s>", nodeIndex, pbuf);
			//// ˢ������
			//::PostMessage(::GetParent(m_hDeskLV), WM_COMMAND, MAKEWPARAM(KPDESK_CMDID_REFRESH_W732, 0), 0);	// ˢ��
		}
		//::SendMessage(m_hDeskLV, WM_SETREDRAW, TRUE, NULL);
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::findNode_index index LeaveCriticalSection   %u idx:%d", GetTickCount(), nodeIndex);
	::LeaveCriticalSection(&m_csLockNodes);

	return 1;
}
int kpdeskAdmin::addNode(const std::wstring &keyName, idlNode *& prefNode)
{
	// �鿴�ڵ��Ƿ��Ѿ��ڽڵ㼯����
	ITEMIDLIST *piidl = NULL;
	gtc::idlNode *pNode = nullptr;
	prefNode = nullptr;
	DEBUG_CS_LOG(L"kpdeskAdmin::addNode EnterCriticalSection 1 node:%s", keyName.c_str());
	//::EnterCriticalSection(&m_csLockNodes);
	//DEBUG_CS_LOG(L"kpdeskAdmin::addNode EnterCriticalSection 2 %u node:%s", GetTickCount(), pNode->getName().c_str());
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::addNode TryEnterCriticalSection 1 keyName:%s ���ݼ��Ѿ�������������", keyName.c_str());
		SEND_LOG(L"WARNING kpdeskAdmin::addNode TryEnterCriticalSection 1 keyName:<%s> lock:<%s> ���ݼ��Ѿ�������������", keyName.c_str(), nc_pstrLockDesc);
		//PostMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		return -1;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::addNode_(%s)", keyName.substr(0,10).c_str());
	// �ڵ㲻���ھʹ����ڵ����
	pNode = findNode(keyName, L"kpdeskAdmin::addNode");
	if (!pNode)
	{
		if (piidl = gtc::idlNode::createNodeIIDL(keyName))
		{
			pNode = gtc::idlNode::createNode(piidl);
			CoTaskMemFree(piidl);
			piidl = NULL;
		}
		else
		{
			SEND_LOG(L"ERROR kpdeskAdmin::addNode �޷���shell�ҵ��ڵ� keyName:<%s>", keyName.c_str());
		}
	}
	else
	{
		SEND_LOG(L"WARNING kpdeskAdmin::addNode �ڵ��Ѿ����ڡ�keyName:<%s>", keyName.c_str());
		DEBUG_CS_LOG(L"kpdeskAdmin::addNode LeaveCriticalSection   %u keyName:%s", GetTickCount(), keyName.c_str());
		::LeaveCriticalSection(&m_csLockNodes);
		prefNode = pNode;
		return 2;
	}
	//assert(pNode);
	if(!pNode)
	{
		SEND_LOG(L"ERROR kpdeskAdmin::addNode �����ڵ�createNode ʧ�ܣ����� keyName:<%s>", keyName.c_str());
		DEBUG_CS_LOG(L"kpdeskAdmin::addNode LeaveCriticalSection   %u node:%s", GetTickCount(), keyName.c_str());
		::LeaveCriticalSection(&m_csLockNodes);
		return 0;
	}
	// �趨�ڵ�����ֵ
	int nodeIndex = findNodeIndex(pNode);
	if (nodeIndex < 0)
	{
		SEND_LOG(L"WARNING kpdeskAdmin::addNode �ڵ����������ڣ������������Ҽ������ڵ㣡name:<%s> Nodes:%d", pNode->getKeyName().c_str(), ListView_GetItemCount(m_hDeskLV));
		// Ҳ�п����������ڵ����ϵͳ������������ڵ��������磺QQ�������ն���ļ���
		//SendMessage(m_hDeskLV, KPDESK_WMME_UPDATENODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
	}
	else
	{
		// ���ýڵ��syslistview32��Ϣ���ꡢ�ߴ��
		setNodeListViewInfo(pNode, nodeIndex);
	}
	idlNode::IDINodeMap::iterator iter = m_mapNodes.find(pNode->getKeyName());
	if(iter != m_mapNodes.end())
	{
		delete iter->second;
		m_mapNodes.erase(iter);
	}
	m_mapNodes[pNode->getKeyName()] = pNode;
	prefNode = pNode;

	DEBUG_CS_LOG(L"kpdeskAdmin::addNode LeaveCriticalSection   %u node:%s", GetTickCount(), pNode->getName().c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	return 1;
}

int kpdeskAdmin::deleteNode(const std::wstring &keyName)
{
	assert(keyName.length() > 0 && m_hDeskLV);
	if(!m_hDeskLV || keyName.length() <= 0) return 0;
	DWORD tick = GetTickCount();
	//bool bRet = false;
	//SEND_LOG(L"kpdeskAdmin::deleteNode EnterCriticalSection 1 <%s>", keyName.c_str());
	//::EnterCriticalSection(&m_csLockNodes);
	//SEND_LOG(L"kpdeskAdmin::deleteNode EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::deleteNode TryEnterCriticalSection 1 keyName:%s ���ݼ��Ѿ�������������", keyName.c_str());
		SEND_LOG(L"WARNING kpdeskAdmin::deleteNode TryEnterCriticalSection 1 keyName:<%s> lock:<%s> ���ݼ��Ѿ�������������", keyName.c_str(), nc_pstrLockDesc);
		//PostMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		return -1;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::deleteNode_(%s)", stringRight(keyName).c_str());
	idlNode::IDINodeMap::iterator iter = m_mapNodes.find(keyName);
	bool bExists = iter != m_mapNodes.end();
	if(bExists)
	{
		int oldIdx = iter->second->getIndex(), idx;
		delete iter->second;
		m_mapNodes.erase(iter);
		// ����ʣ��ڵ�����ֵ
		if (oldIdx >= 0)
		{
			for (iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
			{
				idx = iter->second->getIndex();
				if (idx > oldIdx)
				{
					iter->second->setIndex(idx-1);	// �������ע��1��Ƚ������ر���ɾ���������Ƚ�Сʱ
					///1///////////////////////////////////////////////////////////////////////
					//if (isEqualNode(idx-1, iter->second))
					//{
					//	iter->second->setIndex(idx-1);
					//}
					//else
					//{
					//	SEND_LOG(L"----------------------->%d", idx);
					//	iter->second->setIndex(findNodeIndex(iter->second));
					//}
				}
				//SEND_LOG(L"deleteNode idx:%03d name:<%s>", iter->second->getIndex(), iter->second->getName().c_str());
			}
		}
		//bRet = true;
	}

	SEND_LOG(L"kpdeskAdmin::deleteNode LeaveCriticalSection   ��ʱ��%u name:<%s>", GetTickCount()-tick, keyName.c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	return bExists ? 1 : 2;
}

bool kpdeskAdmin::renameNode(std::wstring oldKeyName, std::wstring newKeyName)
{
	assert(oldKeyName.length() > 0 && newKeyName.length() > 0);
	if(oldKeyName.length() <= 0 || newKeyName.length() <= 0) return false;

	bool bRet = false;
	ITEMIDLIST *piidl = gtc::idlNode::createNodeIIDL(newKeyName);
	if(!piidl)
	{
		SEND_LOG(L"ERROR kpdeskAdmin::renameNode �½ڵ㲻���� name:<%s>", newKeyName.c_str());
		return false;
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::renameNode EnterCriticalSection 1");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::renameNode EnterCriticalSection 2 %u", GetTickCount());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::renameNode_(%s)", stringRight(oldKeyName).c_str());
	gtc::idlNode *pNode = nullptr;
	idlNode::IDINodeMap::iterator iter = m_mapNodes.find(oldKeyName);
	if(iter != m_mapNodes.end())
	{
		// ����ɽڵ�
		pNode = iter->second;
		m_mapNodes.erase(iter);
		// ���½ڵ�
		pNode->update(piidl);
		m_mapNodes[newKeyName] = pNode;
		bRet = true;
	}
	// 
	DEBUG_CS_LOG(L"kpdeskAdmin::renameNode LeaveCriticalSection %u", GetTickCount());
	::LeaveCriticalSection(&m_csLockNodes);
	// 
	if(piidl) CoTaskMemFree(piidl);

	return bRet;
}

int kpdeskAdmin::updateNode(std::wstring keyName)
{
	assert(keyName.length() > 0 && m_hDeskLV);
	if(!m_hDeskLV || keyName.length() <= 0) return false;
	int ret = 0;
	
	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode EnterCriticalSection 1 <%s>", keyName.c_str());
	//::EnterCriticalSection(&m_csLockNodes);
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::updateNode index TryEnterCriticalSection 1 name:<%s> ���ݼ��Ѿ�������������", keyName.c_str());
		SEND_LOG(L"WARNING kpdeskAdmin::updateNode index TryEnterCriticalSection 1 name:<%s> lock:<%s> ���ݼ��Ѿ�������������", keyName.c_str(), nc_pstrLockDesc);
		return -1;
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::updateNode_(%s)", stringRight(keyName).c_str());
	//SEND_LOG(L"kpdeskAdmin::updateNode AAA");
	gtc::idlNode *pNode = findNode(keyName, L"kpdeskAdmin::updateNode");
	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode LeaveCriticalSection   %u <%s>", GetTickCount(), keyName.c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	if(pNode)
	{
		ITEMIDLIST *piidls = gtc::idlNode::createNodeIIDL(keyName);
		if (piidls)
		{
			//SEND_LOG(L"kpdeskAdmin::updateNode BBB node:<%s>", pNode->toString().c_str());
			if (pNode->update(piidls))
			{
				// ���½ڵ�����������������ʱ��ı�����ֵ����word�ļ����޸ģ�
				int idx = findNodeIndex(pNode);
				if(idx >= 0) setNodeListViewInfo(pNode, idx)/* pNode->setIndex(idx)*/;
				//SEND_LOG(L"kpdeskAdmin::updateNode CCC idx:%d node:<%s>", idx, pNode->toString().c_str());
				ret = 1;
			
			}
			CoTaskMemFree(piidls);
		}
		else
		{
			SEND_LOG(L"ERROR kpdeskAdmin::updateNode �޷���shell�ҵ��ڵ㣡name��<%s>", keyName.c_str());
		}
	}
	else
	{
		SEND_LOG(L"ERROR kpdeskAdmin::updateNode �ڵ㲻���ڣ�name��<%s>", keyName.c_str());
		ret = 2;
	}


	return ret;
}
//bool kpdeskAdmin::updateNode(std::wstring keyName, std::wstring tmpKeyName)
//{
//	assert(keyName.length() > 0 && m_hDeskLV);
//	if(!m_hDeskLV || keyName.length() <= 0) return false;
//	bool bRet = false;
//	gtc::idlNode *pNode = findNode(keyName);
//	if(!pNode) return false;
//
//	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode EnterCriticalSection 1 <%s>", keyName.c_str());
//	::EnterCriticalSection(&m_csLockNodes);
//	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
//	SEND_LOG(L"kpdeskAdmin::updateNode AAA");
//	ITEMIDLIST *piidls = gtc::idlNode::createNodeIIDL(tmpKeyName);
//	if (piidls)
//	{
//		gtc::idlNode *pTmp = gtc::idlNode::createNode(piidls);
//		if (pTmp)
//		{
//			SEND_LOG(L"kpdeskAdmin::updateNode BBB");
//			pNode->setLVSize(pTmp->getLVSize().c_str());
//			pNode->setLVModifyDT(pTmp->getLVModifyDT().c_str());
//			// ���½ڵ�����������������ʱ��ı�����ֵ����word�ļ����޸ģ�
//			int idx = findNodeIndex(pNode);
//			SEND_LOG(L"kpdeskAdmin::updateNode CCC idx:%d", idx);
//			if(idx >= 0) pNode->setIndex(idx);
//		}
//		CoTaskMemFree(piidls);
//	}
//
//	DEBUG_CS_LOG(L"kpdeskAdmin::updateNode LeaveCriticalSection   %u <%s>", GetTickCount(), keyName.c_str());
//	::LeaveCriticalSection(&m_csLockNodes);
//
//	return bRet;
//}
LPITEMIDLIST kpdeskAdmin::getNodeIIDLs(int nodeIndex)
{
	assert(m_hDeskLV);
	if(!m_hDeskLV || nodeIndex < 0) return NULL;

	wchar_t pstr[MAX_PATH] = {0};
	std::wstring wsName, wsType, wsSize, wsDT;
	// ��ʾ��
	ListView_GetItemText(m_hDeskLV, nodeIndex, 0, pstr, _countof(pstr));
	wsName = pstr;
	if(wsName.empty()) return NULL;
	// ����
	ListView_GetItemText(m_hDeskLV, nodeIndex, 2, pstr, _countof(pstr));
	wsType = pstr;
	// ��С
	ListView_GetItemText(m_hDeskLV, nodeIndex, 1, pstr, _countof(pstr));
	wsSize = pstr;
	// �޸�ʱ�䣬������3
	ListView_GetItemText(m_hDeskLV, nodeIndex, 3, pstr, _countof(pstr));
	wsDT = pstr;
	wsDT = gtc::convert::deleteNoGraph(wsDT);
	//SEND_LOG(L"kpdeskAdmin::getNodeIIDLs idx:%d name:<%s> size:<%s> type:<%s> mdt:<%s>", nodeIndex, wsName.c_str(), wsSize.c_str(), wsType.c_str(), wsDT.c_str());
	return gtc::idlNode::createNodeIIDL(wsName, wsType, wsSize, wsDT);
}
void kpdeskAdmin::refreshNodeOffset()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;
	if (ListView_GetItemCount(m_hDeskLV) <= 0) return;
	RECT rc = {0};
	RECT rcOld = {0};
	ListView_GetItemRect(m_hDeskLV, 0, &rcOld, LVIR_BOUNDS);
	ListView_SetItemPosition(m_hDeskLV, 0, 0, 0);
	ListView_GetItemRect(m_hDeskLV, 0, &rc, LVIR_BOUNDS);
	m_szOffset.cx = -rc.left;
	m_szOffset.cy = -rc.top;
	ListView_SetItemPosition(m_hDeskLV, 0, rcOld.left, rcOld.top);
}

void kpdeskAdmin::refreshNodeMaxSize()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;
	DWORD dwspace = ListView_GetItemSpacing(m_hDeskLV, FALSE);
	m_szMaxNode.cx = LOWORD(dwspace);
	m_szMaxNode.cy = HIWORD(dwspace);
}

int kpdeskAdmin::findNodeIndex(const gtc::idlNode *pNode)
{
	assert(m_hDeskLV);
	if(!m_hDeskLV || !pNode) return -1;
	int nodeIndex = -1;
	wchar_t pstr[MAX_PATH] = {0};
	LVFINDINFO lvfind = {0};

	lvfind.flags = LVFI_STRING;
	lvfind.psz = pNode->getName().c_str();
	// �����ж����ͬ��ʾ���Ľڵ�
	while((nodeIndex = ListView_FindItem(m_hDeskLV, nodeIndex, &lvfind)) >= 0)
	{
		if(isEqualNode(nodeIndex, pNode)) 
			return nodeIndex;
	}

	return -1;
}

bool kpdeskAdmin::isEqualNode(int nodeIndex, const gtc::idlNode *pNode)
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return false;
	// ���ڵ���ȵ������ǣ���ʾ�������ͣ���С��Ҫ���
	if(nodeIndex < 0 || !pNode) return false;
	wchar_t pstr[MAX_PATH] = {0};
	std::wstring wsName, wsType, wsSize, wsDT;
	// ��ʾ��
	ListView_GetItemText(m_hDeskLV, nodeIndex, 0, pstr, _countof(pstr));
	wsName = pstr;
	// ����
	ListView_GetItemText(m_hDeskLV, nodeIndex, 2, pstr, _countof(pstr));
	wsType = pstr;
	// ��С��������Ϊ�ˣ�1�����֡��������桱�롰�û����桱��ͬ���ڵ㣬�磺��ѶQQ.lnk��2�����ֲ�ͬ��չ������ʾ������ͬ�Ľڵ㣬�磺abc.txt, abc.wtx��
	ListView_GetItemText(m_hDeskLV, nodeIndex, 1, pstr, _countof(pstr));
	wsSize = pstr;
	// �޸�ʱ�䣬������3
	ListView_GetItemText(m_hDeskLV, nodeIndex, 3, pstr, _countof(pstr));
	wsDT = pstr;
	wsDT = gtc::convert::deleteNoGraph(wsDT);

	return isEqualNode(wsName, wsSize, wsType, wsDT, pNode);
}

bool kpdeskAdmin::isEqualNode(const std::wstring &wsName, const std::wstring &wsSize, const std::wstring &wsType, const std::wstring &wsDT, const gtc::idlNode *pNode)
{
	//assert(!wsName.empty() && pNode);
	if(wsName.empty() || !pNode) return false;

	// ��ʾ��
	if(pNode->getName() != wsName) return false;
	// ����
	//SEND_LOG(L"isEqualNode ����");
	if (wsType.empty())
	{	// 1��ϵͳ�ڵ�����Ϊ�� 2��ע�⣺WIN8.1ϵͳ���û��ڵ������Ϊ���ļ��С�
		StringMapType::iterator iter = m_mapSystemNodes.find(wsName);
		if (iter != m_mapSystemNodes.end())
		{
			//if(pNode->getName() == L"1111") SEND_LOG(L"isEqualNode disname:%s  keyname:%s", iter->first.c_str(), iter->second.c_str());
			if(iter->second != pNode->getFileName())
			{
				//SEND_LOG(L"ERROR isEqualNode ϵͳ�ڵ㲻һ�� A %s=%s*****%s=%s", iter->first.c_str(), iter->second.c_str(), pNode->getKeyName().c_str(), pNode->getFullPath().c_str());
				return false;
			}
			else
			{
				//SEND_LOG(L"WARNING ϵͳ�ڵ� ���ü����Ա��� A name:%s  clsid:%s", pNode->getName().c_str(), iter->second.c_str());
				return true;
			}
		}
		else
		{
			//SEND_LOG(L"ERROR isEqualNode �޷��ҵ�ϵͳ�ڵ� A %s", pNode->getName().c_str());
			return false;
		}
	}
	else if (wsType == L"�ļ���")
	{	
		if (pNode->getLVType() != wsType)
		{	
			if (pNode->getLVType().empty())
			{	// ע�⣺WIN8.1ϵͳ���û��ڵ������Ϊ���ļ��С�
				StringMapType::iterator iter = m_mapSystemNodes.find(pNode->getName());
				if (iter != m_mapSystemNodes.end())
				{
					if(iter->second != pNode->getFileName())
					{
						//SEND_LOG(L"ERROR isEqualNode ϵͳ�ڵ㲻һ�� B %s=%s*****%s=%s", iter->first.c_str(), iter->second.c_str(), pNode->getKeyName().c_str(), pNode->getFullPath().c_str());
						return false;
					}
					else
					{
						//SEND_LOG(L"WARNING ϵͳ�ڵ� ���ü����Ա��� B name:%s  clsid:%s", pNode->getName().c_str(), iter->second.c_str());
						return true;
					}
				}
				else
				{
					//SEND_LOG(L"ERROR isEqualNode �޷��ҵ�ϵͳ�ڵ� B %d name:%s type:%s size:%s dt:%s Node:%s<%s>", nodeIndex, wsName.c_str(), wsType.c_str(), wsSize.c_str(), wsDT.c_str(), pNode->getName().c_str(), pNode->getLVType().c_str());
					return false;
				}			
			}
			else
			{
				//SEND_LOG(L"ERROR �Ƚ������� %s=%s", wsType.c_str(), pNode->getLVType().c_str());
				return false;
			}
		}
	}
	else if(wsType != pNode->getLVType())
	{
		//SEND_LOG(L"ERROR ���Ͳ�һ�� lv:%s   node:%s", wsType.c_str(), pNode->getLVType().c_str());
		return false;
	}
	// ��ͬ��ʾ�������͵Ľڵ��ڹ���������Ҳ���ھͱȽ����������Ƿ�һ��
	if (gtc::idlNode::inPublicDesktop(wsName, wsType))
	{
		// ��С��������Ϊ�ˣ�1�����֡��������桱�롰�û����桱��ͬ���ڵ㣬�磺��ѶQQ.lnk��2�����ֲ�ͬ��չ������ʾ������ͬ�Ľڵ㣬�磺abc.txt, abc.wtx��
		//SEND_LOG(L"isEqualNode ��С");
		if(pNode->getLVSize() != wsSize) return false;

		// �޸�ʱ�䣬������3
		//SEND_LOG(L"isEqualNode �޸�ʱ��");
		if(pNode->getLVModifyDT() != wsDT) return false;
	}

	return true;
}
void kpdeskAdmin::setNodeListViewInfo(gtc::idlNode *pNode, int nodeIndex, POINT *ptpos /*= NULL*/)
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;
	assert(pNode && nodeIndex >= 0);
	if(!pNode || nodeIndex < 0) return;
	RECT rc = {0};
	SIZE sz = {0};
	ListView_GetItemRect(m_hDeskLV, nodeIndex, &rc, LVIR_BOUNDS);
	if (ptpos)
	{
		ptpos->x = rc.left;
		ptpos->y	 = rc.top;
	}
	sz.cx = rc.right-rc.left;
	sz.cy = rc.bottom-rc.top;
	// ��¼�ڵ��������ߴ�
	pNode->setIndex(nodeIndex);
	pNode->setSizeBox(sz);
}

inline int kpdeskAdmin::movetoSEH(const std::wstring &keyName, POINT pt)
{
	int ret = 0;
	__try
	{
		ret = wrapMoveto(keyName, pt);
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"kpdeskAdmin::movetoSEH"))
	{
		ret = 0;
		WRITE_LOG(L"kpdeskAdmin::movetoSEH SEH�쳣 pos:<%d,%d> name:<%s>", pt.x, pt.y, keyName.c_str());
	}
	return ret;
}
int kpdeskAdmin::moveto(const std::wstring &keyName, POINT pt)
{
	//WRITE_LOG(L"kpdeskAdmin::moveto pos:<%d,%d> name:<%s>", pt.x, pt.y, keyName.c_str());
	static POINT c_ptHide = {KPDESK_HIDENODE_POSX, KPDESK_HIDENODE_POSY};
	assert(m_hDeskLV && keyName.length() > 0);
	if(!m_hDeskLV || keyName.length() <= 0) return 0;
	idlNode::IDINodeMap::const_iterator iter;
	idlNode *pNode = nullptr;
	int ret = 0;
	int nodeIndex;
	RECT rc = {0};
	//try
	//{
	//	iter = m_mapNodes.find(keyName);
	//	if (iter != m_mapNodes.end())
	//	{
	//		pNode = iter->second;
	//		nodeIndex = pNode->getIndex();
	//		if (isEqualNode(nodeIndex, pNode))
	//		{
	//			if (pNode->isHide())
	//			{
	//				ListView_GetItemRect(m_hDeskLV, nodeIndex, &rc, LVIR_BOUNDS);
	//				if(rc.left >= 0 ||rc.top >= 0)
	//					ListView_SetItemPosition(m_hDeskLV, nodeIndex, c_ptHide.x, c_ptHide.y);
	//			}
	//			else
	//			{
	//				pt.x += m_szOffset.cx;
	//				pt.y += m_szOffset.cy;
	//				//WRITE_LOG(L"kpdeskAdmin::moveto idx:%d pos:<%d,%d> name:<%s>", nodeIndex, pt.x, pt.y, keyName.c_str());
	//				ListView_SetItemPosition(m_hDeskLV, nodeIndex, pt.x, pt.y);
	//			}
	//			return 1;
	//		}
	//	}
	//}
	//catch (...)
	//{
	//	SEND_LOG(L"ERROR kpdeskAdmin::moveto �쳣 �쳣 �쳣������");
	//}
	ret = movetoSEH(keyName, pt);
	// �ɹ�
	if(ret == 1) return 1;	

	DEBUG_CS_LOG(L"kpdeskAdmin::moveto EnterCriticalSection 1 <%s>", keyName.c_str());
	//::EnterCriticalSection(&m_csLockNodes);
	//DEBUG_CS_LOG(L"kpdeskAdmin::moveto EnterCriticalSection 2 %u <%s>", GetTickCount(), keyName.c_str());
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if (!bEnter)
	{
		SEND_LOG(L"WARNING kpdeskAdmin::moveto TryEnterCriticalSection 1 name:<%s> lock:<%s> ���ݼ��Ѿ�������������", keyName.c_str(), nc_pstrLockDesc);
		return -1;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::moveto");
	//const int tryCount = 2;
	//int atry = 0;
	//bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//while(!bEnter)
	//{
	//	if(atry < tryCount)
	//	{
	//		++atry;
	//		Sleep(30);
	//		SEND_LOG(L"WARNING kpdeskAdmin::moveto TryEnterCriticalSection 1 trys:%d <%s> ���ݼ��Ѿ�������������", atry, keyName.c_str());
	//		bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	//	}
	//	else
	//		break;
	//}
	//if(!bEnter) return ;

	iter = m_mapNodes.find(keyName);
	if (iter == m_mapNodes.end())
	{
		SEND_LOG(L"WARNING kpdeskAdmin::moveto KPDESK_WMME_DELETENODE �ڵ�����δ�ҵ�����Ҫ���Ƴ��� name��<%s>", keyName.c_str());
		SendMessage(m_hDeskLV, KPDESK_WMME_DELETENODE, (WPARAM)(new std::wstring(keyName)), 1);
		::LeaveCriticalSection(&m_csLockNodes);
		return 2;
	}
	pNode = iter->second;
	//idlNode *pNode = findNode(keyName);
	//if (!pNode)
	//{
	//	SEND_LOG(L"WARNING kpdeskAdmin::moveto �ڵ�����δ�ҵ��� name��<%s>", keyName.c_str());
	//	//PostMessage(m_hDeskLV, KPDESK_WMME_DELETENODE, (WPARAM)(new std::wstring(keyName)), NULL);
	//	//DEBUG_CS_LOG(L"kpdeskAdmin::moveto LeaveCriticalSection   %u node:%s", GetTickCount(), keyName.c_str());
	//	::LeaveCriticalSection(&m_csLockNodes);
	//	return;
	//}
	nodeIndex = pNode->getIndex();

	if (!isEqualNode(nodeIndex, pNode))
	{
		SEND_LOG(L"WARNING kpdeskAdmin::moveto �ڵ����������ı�->ˢ���鴰�� idx:%d name:<%s>", nodeIndex, pNode->getName().c_str());
		redrawNode(nodeIndex);
		//if(!updateNode(pNode->getKeyName()))
		//	PostMessage(m_hDeskLV, KPDESK_WMME_UPDATENODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		//else
		//	nodeIndex = pNode->getIndex();
		//if(nodeIndex >= 0)
		//{
		//	if(!updateNode(pNode->getKeyName()))
		//		PostMessage(m_hDeskLV, KPDESK_WMME_UPDATENODE, (WPARAM)(new std::wstring(pNode->getKeyName())), NULL);
		//}
		nodeIndex = findNodeIndex(pNode);
		if(nodeIndex >= 0)
		{
			setNodeListViewInfo(pNode, nodeIndex);
		}
	}

	//	
	if (nodeIndex < 0)
	{
		// DAI
		ret = 2;
		SEND_LOG(L"WARNING kpdeskAdmin::moveto KPDESK_WMME_DELETENODE ��Ч�ڵ� ����Ϊ-1 ��Ҫ��ɾ���� name��<%s>", keyName.c_str());
		//mc_pwndHost->pushDelNode(pNode->getKeyName());
		// ������ɾ���ļ�ʱ���ᴥ������ɾ����Ϣ��ֻ���п���������ɾ����
		SendMessage(m_hDeskLV, KPDESK_WMME_DELETENODE, (WPARAM)(new std::wstring(keyName)), 0);
	}
	else
	{
		if (pNode->isHide())
		{
			ListView_GetItemRect(m_hDeskLV, nodeIndex, &rc, LVIR_BOUNDS);
			if(rc.left >= 0 ||rc.top >= 0)
				ListView_SetItemPosition(m_hDeskLV, nodeIndex, c_ptHide.x, c_ptHide.y);
		}
		else
		{
			pt.x += m_szOffset.cx;
			pt.y += m_szOffset.cy;
			ListView_SetItemPosition(m_hDeskLV, nodeIndex, pt.x, pt.y);
		}
		ret = 1;
	}
	DEBUG_CS_LOG(L"kpdeskAdmin::moveto LeaveCriticalSection   %u node:%s", GetTickCount(), keyName.c_str());
	::LeaveCriticalSection(&m_csLockNodes);

	return ret;
}

bool kpdeskAdmin::refreshAllNode()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return false;
	idlNode *pNode = nullptr;
	idlNode::IDINodeMap::iterator iter;
	LPITEMIDLIST piidl = NULL;
	std::wstring wsKey;
	DEBUG_CS_LOG(L"kpdeskAdmin::refreshAllNode index EnterCriticalSection 1 ");
	//::EnterCriticalSection(&m_csLockNodes);
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		//DEBUG_CS_LOG(L"WARNING kpdeskAdmin::refreshAllNode index TryEnterCriticalSection 1 ���ݼ��Ѿ�������������");
		SEND_LOG(L"WARNING kpdeskAdmin::refreshAllNode index TryEnterCriticalSection 1 lock:<%s> ���ݼ��Ѿ�������������", nc_pstrLockDesc);
		return false;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::refreshAllNode");
	DEBUG_CS_LOG(L"kpdeskAdmin::refreshAllNode index EnterCriticalSection 2 %u ", GetTickCount());

	// 1������ڵ�����
	SEND_LOG(L"kpdeskAdmin::refreshAllNode ׼�� ����ڵ�����");
	clearOptFlag();

	// 2���������нڵ㼯��������û�з����ı�Ľڵ�
	std::vector<int> vecOkNodes;	// �ڵ㲻��Ҫˢ�µĽڵ�����
	int nodeIndex;
	for (iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		nodeIndex = iter->second->getIndex();
		if (nodeIndex >= 0 && isEqualNode(nodeIndex, iter->second))
		{
			iter->second->setOptFlag(1);
			vecOkNodes.push_back(nodeIndex);
		}
	}
	// 3����������ڵ��趨������ֵ
	SEND_LOG(L"kpdeskAdmin::refreshAllNode ׼�� ��������ڵ��趨������ֵ");
	int lvCount = ListView_GetItemCount(m_hDeskLV);
	for (int idx = 0; idx < lvCount; ++idx)
	{
		if(std::find(vecOkNodes.cbegin(), vecOkNodes.cend(), idx) != vecOkNodes.cend()) continue;
		piidl = getNodeIIDLs(idx);
		if (piidl)
		{
			/*wsKey = */gtc::idlNode::parseKeyName(piidl, wsKey);
			// :<::{26EE0668-A00A-44D7-9371-BEB064C98683}> ��<::{5399E694-6CE5-4D6C-8FCE-1D8870FDCBA0}>
			if(wsKey == L"::{26EE0668-A00A-44D7-9371-BEB064C98683}")
				wsKey = L"::{5399E694-6CE5-4D6C-8FCE-1D8870FDCBA0}";

			wchar_t pbuf[MAX_PATH] = {0};
			ListView_GetItemText(m_hDeskLV, idx, 0, pbuf, _countof(pbuf));
			//SEND_LOG(L"kpdeskAdmin::refreshAllNode lvname:<%s> key:<%s> ", pbuf, wsKey.c_str());
			pNode = findNode(wsKey, L"kpdeskAdmin::refreshAllNode");
			if (pNode)
			{
				//SEND_LOG(L"kpdeskAdmin::refreshAllNode ���ýڵ�����idx:%d name:<%s>", idx, pNode->getName().c_str());
				//pNode->setIndex(idx);
				if(pNode->getIndex() != idx)
					setNodeListViewInfo(pNode, idx);
				pNode->setOptFlag(1);
			}
			else
			{
				//SEND_LOG(L"kpdeskAdmin::refreshAllNode KPDESK_WMME_ADDNODE �ڵ㲻�ڼ����� name:<%s>", wsKey.c_str());
				SendMessage(m_hDeskLV, KPDESK_WMME_ADDNODE, (WPARAM)(new std::wstring(wsKey)), NULL);
			}
			// 
			CoTaskMemFree(piidl);
			piidl = NULL;
		}
	}
	// 3����������ڵĽڵ�
	SEND_LOG(L"kpdeskAdmin::refreshAllNode ׼�� ��������ڵĽڵ�");
	freeNone();

	DEBUG_CS_LOG(L"kpdeskAdmin::refreshAllNode index LeaveCriticalSection   %u ", GetTickCount());
	::LeaveCriticalSection(&m_csLockNodes);

	return true;
}

void kpdeskAdmin::refreshAllNodeSize()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;

	DEBUG_CS_LOG(L"kpdeskAdmin::refreshNodeSize EnterCriticalSection 1");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::refreshNodeSize EnterCriticalSection 2 %u", GetTickCount());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::refreshNodeSize");
	RECT rc = {0};
	SIZE sz = {0};
	for (idlNode::IDINodeMap::iterator iter = m_mapNodes.begin(); iter != m_mapNodes.end(); ++iter)
	{
		ListView_GetItemRect(m_hDeskLV, iter->second->getIndex(), &rc, LVIR_BOUNDS);

		sz.cx = rc.right-rc.left;
		sz.cy = rc.bottom-rc.top;
		// ��¼�ڵ��������ߴ�
		iter->second->setSizeBox(sz);
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::refreshNodeSize LeaveCriticalSection %u", GetTickCount());
	::LeaveCriticalSection(&m_csLockNodes);
}
void kpdeskAdmin::redrawNode(int nodeIndex)
{
	assert(m_hDeskLV);
	if(!m_hDeskLV || nodeIndex < 0) return;
	ListView_RedrawItems(m_hDeskLV, nodeIndex, nodeIndex);
	//ListView_Update(m_hDeskLV, nodeIndex);

}
void kpdeskAdmin::refreshHideNode()
{
	static POINT c_ptHide = {KPDESK_HIDENODE_POSX, KPDESK_HIDENODE_POSY};
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;

	DEBUG_CS_LOG(L"kpdeskAdmin::refreshHideNode EnterCriticalSection 1");
	//::EnterCriticalSection(&m_csLockNodes);
	bool bEnter = ::TryEnterCriticalSection(&m_csLockNodes) == TRUE;
	if(!bEnter)
	{
		DEBUG_CS_LOG(L"WARNING kpdeskAdmin::refreshHideNode TryEnterCriticalSection 1 lock:<%s> ���ݼ��Ѿ�������������", nc_pstrLockDesc);
		return;
	}
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::refreshHideNode");
	DEBUG_CS_LOG(L"kpdeskAdmin::refreshHideNode EnterCriticalSection 2 %u", GetTickCount());

	for (gtc::idlNode::IDINodeMap::const_iterator iter = m_mapNodes.cbegin(); iter != m_mapNodes.cend(); ++iter)
	{
		if (iter->second->isHide())
		{
			//SEND_LOG(L"moveHideNodes %s", iter->first.c_str());
			moveto(iter->first, c_ptHide);
		}
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::refreshHideNode LeaveCriticalSection %u", GetTickCount());
	::LeaveCriticalSection(&m_csLockNodes);
}
void kpdeskAdmin::redrawCutNode()
{
	assert(m_hDeskLV);
	if(!m_hDeskLV) return;
	DEBUG_CS_LOG(L"kpdeskAdmin::redrawCutNode EnterCriticalSection 1");
	::EnterCriticalSection(&m_csLockNodes);
	DEBUG_CS_LOG(L"kpdeskAdmin::redrawCutNode EnterCriticalSection 2 %u", GetTickCount());
	std::swprintf(nc_pstrLockDesc, _countof(nc_pstrLockDesc), L"kpdeskAdmin::redrawCutNode");
	for (gtc::idlNode::IDINodeMap::const_iterator iter = m_mapNodes.cbegin(); iter != m_mapNodes.cend(); ++iter)
	{
		if (iter->second->isCut())
		{
			redrawNode(iter->second->getIndex());
		}
	}

	DEBUG_CS_LOG(L"kpdeskAdmin::redrawCutNode LeaveCriticalSection %u", GetTickCount());
	::LeaveCriticalSection(&m_csLockNodes);
}
}