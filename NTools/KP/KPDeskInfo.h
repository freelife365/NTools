#ifndef __KPDESKINFO_H__
#define __KPDESKINFO_H__
#pragma once

#include <string>
#include <vector>
#include <map>
#include <cwchar>
#include <sstream>
#include <cstdint>
#include <ctime>
#include <queue>
#include <deque>
#include <assert.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <ShellAPI.h>
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"shell32.lib")

#include <Msi.h>
#pragma comment(lib,"Msi.lib")

#include "AC/basedef.h"
#include "KP/KPDeskDef.h"
#include "AC/sysupport.h"

#define IDLNODE_INIT_INDEX		-1000		
namespace gtc {
//////////////////////////////////////////////////////////////////////////
// lnk�ļ�ͷ
typedef struct BSLinkHead
{
	DWORD		dwID;
	DWORD		dwGUID[4];
	DWORD		dwFlags;
	DWORD		dwFileAttributes;
	FILETIME	dwCreationTime;
	FILETIME	dwModificationTime;
	FILETIME	dwLastaccessTime;
	DWORD		dwFileLen;
	DWORD		dwIconNum;
	DWORD		dwWinStyle;
	DWORD		dwHotkey;
	DWORD		dwReserved1;
	DWORD		dwReserved2;
}BSLinkHead;

#define LNK_HASIDLIST	0x1
#define LNK_FILEDIR		0x2
#define LNK_HASDES		0x4
#define LNK_HASPATH		0x8
#define LNK_HASWORKDIR	0x10
#define LNK_HASCMD		0x20

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BSNodeMoveInfo moveNodeCache��	2014-12-25
//	û�гɹ��ƶ��ڵ�Ļ��棻�磺�ڵ㱻������������
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct BSNodeMoveInfo {
	wchar_t		pkeyName[MAX_PATH];
	POINT		pos;
	BSNodeMoveInfo()
	{
		pkeyName[0] = L'\0';
		pos.x = pos.y = 0;
	}
	BSNodeMoveInfo(PCWSTR pName, POINT pt)
	{
		if(pName)
			std::wcsncpy(pkeyName, pName, _countof(pkeyName));
		else
			pkeyName[0] = L'\0';
		memcpy(&pos, &pt, sizeof(pos));
	}
	bool operator==(const BSNodeMoveInfo &data) const
	{
		return std::wcsncmp(pkeyName, data.pkeyName, _countof(pkeyName)) == 0;
	}
	bool operator==(PCWSTR pName) const
	{
		return std::wcsncmp(pkeyName, pName, _countof(pkeyName)) == 0;
	}
}BSNodeMoveInfo;
class moveNodeCache {
public:
	moveNodeCache();
	~moveNodeCache();

	bool push(PCWSTR pkeyName, POINT pt);
	bool pop(std::wstring &keyName, POINT &pt);
	bool exists(PCWSTR pkeyName);
	bool isEmpty() const;
	std::size_t size() const;
protected:
	CRITICAL_SECTION			m_csLock;	// ���ݱ�����
	std::deque<BSNodeMoveInfo>	m_queCache;	// ���ݶ���
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	monitorDirCache	2014-12-24
//	windowsĿ¼(�ļ���)״̬������ݻ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct BSMonitorData {
	LONG	lEvent;
	wchar_t	pFullPath1[MAX_PATH];
	wchar_t	pFullPath2[MAX_PATH];
	BSMonitorData()
		: lEvent(0)
	{
		pFullPath1[0] = pFullPath2[0] = L'\0';
	}
	BSMonitorData(LONG Event, PCWSTR pfullPath1, PCWSTR pfullPath2)
		: lEvent(Event)
	{
		if (pfullPath1)
			std::wcsncpy(pFullPath1, pfullPath1, _countof(pFullPath1));
		else
			pFullPath1[0] = L'\0';
		if (pfullPath2)
			std::wcsncpy(pFullPath2, pfullPath2, _countof(pFullPath2));
		else
			pFullPath2[0] = L'\0';
	}
	
}BSMonitorData;
class monitorDirCache {
public:
	monitorDirCache();
	~monitorDirCache();

	void push(LONG Event, PCWSTR pfullPath1, PCWSTR pfullPath2);
	bool pop(BSMonitorData &data);
	bool isEmpty() const;
	std::size_t size() const;
protected:
	CRITICAL_SECTION			m_csLock;	// ���ݱ�����
	std::queue<BSMonitorData>	m_queCache;	// ���ݶ���
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	nodesCache��	2014-10-30
//	�ڵ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class nodesCache {
public:
	nodesCache();
	~nodesCache();

	bool push(std::wstring fullPath);
	// �õ����Ƴ����ж�β��һ���ڵ�
	bool pop(std::wstring &fullPath);
	// �õ�һ���ڶ�β�㣬�����Ƴ�
	bool getNode(std::wstring &fullPath);
	// �����β�ڵ����fullPath�����Ƴ���β�ڵ�
	bool popNode(const std::wstring &fullPath);
	bool isEmpty();
	bool exists(std::wstring fullPath);
	std::size_t size();
protected:
	CRITICAL_SECTION			m_csLock;	// ���ݱ�����
	std::deque<std::wstring>	m_queCache;	// ���ݶ���
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	idlNode��	2014-04-30
//	������Ϣ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class idlNode {
public:
	// �ĵ����ļ��нڵ���Ϣ���飻key=���ƣ��������������������������ֵ�ᷢ���仯�� value=idlNode*
	typedef std::map<std::wstring, gtc::idlNode *>	IDINodeMap;
	typedef std::vector<idlNode *>	IDINodeArray;
public:
	typedef enum BENodeShowFlag {
		BENodeShowFlagNone		= 0,
		BENodeShowFlagHide		,	// �ڵ���������
		BENodeShowFlagShow		,		// �ڵ���������
		BENodeShowFlagCut		,		// �ڵ��������߽���
	};
private:
	idlNode(){}
public:
	idlNode(std::wstring wsname, std::wstring wssize, std::wstring wstype, std::wstring wsmodifydt);
//public:
//	idlNode(PCWSTR pname, int32_t idx, const SIZE &sz);
//	idlNode(PCWSTR pname, PCWSTR ptypeName, PCWSTR pfullpath, SFGAOF attributs);
//	idlNode(PCWSTR pname, PCWSTR ptypeName, PCWSTR pfullpath, SFGAOF attributs, LPITEMIDLIST pidlst);
	virtual ~idlNode();
public:
	// free
	/*****************************************************************************************
		<< --- static kpdeskAdmin::free		2014-06-03 --- >>
		˵�����Ƿ�ڵ���Ϣ
		������
		vecdata		= ���ͷŵĽڵ����ݼ���
		����ֵ��
	*****************************************************************************************/
	static void free(IDINodeMap &mapdata);
	static void free(IDINodeArray &arrdata);
	/****************************************************************************************
		<< --- createNodeIIDL		2014-10-28 --- >>         
		˵�������������ָ��ȫ·���ڵ��IIDLS*
		������
			wsFullPath = �����ҽڵ�ȫ·��
		����ֵ���ڵ�IIDLSָ�룬�ڵ㲻���ڷ��ؿա���������Ҫ�������ͷš�����
	*****************************************************************************************/
	static LPITEMIDLIST createNodeIIDL(const std::wstring &wsFullPath);
	/****************************************************************************************
		<< --- createNodeIIDL		2014-10-31 --- >>         
		˵�������ݽڵ����ԣ���ʾ�������͡���С���޸����ڣ���shell�в��ҽڵ�
		������
			displayName	= �����ҽڵ���ʾ��
			lvType		= �����ҽڵ�����
			lvSize		= �����ҽڵ��С
			lvModifyDT	= �����ҽڵ��޸�����
		����ֵ���ڵ�IIDLSָ�룬�ڵ㲻���ڷ��ؿա���������Ҫ�������ͷš�����
	*****************************************************************************************/
	static LPITEMIDLIST createNodeIIDL(const std::wstring &displayName, const std::wstring &lvType, const std::wstring &lvSize, const std::wstring &lvModifyDT);

	/****************************************************************************************
		<< --- inPublicDesktop		2014-11-27 --- >>         
		˵�����鿴ָ����ʾ�������͵Ľڵ��Ƿ��ڹ���������
		������
			displayName	= �����ҽڵ���ʾ��
			lvType		= �����ҽڵ�����
		����ֵ���Ƿ��ڹ���������
	*****************************************************************************************/
	static bool inPublicDesktop(const std::wstring &displayName, const std::wstring &lvType);
public:
	// 
	// �ڵ�����
	BEDeskGroupFlag getGroupType();
	// Сͼ������
	int32_t getSmallIcon();
	// ��ͼ������
	int32_t getLagerIcon();
	// ѡ��Сͼ������
	int32_t getSmallOpenIcon();
	// ѡ�д�ͼ������
	int32_t getLagerOpenIcon();
	HICON getIcon();
	std::wstring getTargetPath();
	std::wstring getTargetType();
	//void copyIIDList(LPITEMIDLIST piidl);
	/****************************************************************************************
		<< --- update		2014-10-27 --- >>         
		˵�������ݽڵ�ITEMIDLISTֵ�����������ԣ���С���޸�����
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool update(LPITEMIDLIST piidl);
	bool update(const idlNode *pNode);
private:
	bool isSystemGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
	bool isFolderGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
	bool isSoftGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
	bool isCompressGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
	bool isDocumentGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
	bool isPictureGroup(const std::wstring &wsLVType, const std::wstring &wsTargetType);
private:
	//static std::wstring readShortcut(PCWSTR pLnk);
	static bool readShortcutFile(PCWSTR pLnk, std::wstring &wsTargetPath);
	static BEDeskGroupFlag getGroupFlagByExtname(const std::wstring &fullpath);
public:
	// ����LPITEMIDLIST����һ���ڵ�
	static idlNode* createNode(LPITEMIDLIST pidl);
	// �õ�key��
	static bool parseKeyName(LPITEMIDLIST pidl, std::wstring &wsKeyName);
	// �����ļ�����
	static bool parseFileType(PCWSTR pfullPath, std::wstring &wsFileType);
	//// ���½ڵ���Ϣ
	//static void updateNode(LPITEMIDLIST pidl, idlNode *pNode);
	// �Ƿ�Ϊ��ʱ�ļ�������Զ�ɾ����
	static bool isTemporaryFile(LPITEMIDLIST pidl);
	static bool isTemporaryFile(PCWSTR pfullPath);
	static bool isTemporaryFile(std::wstring fullPath);
	// ȫ·���ļ�����ϵͳ�ڵ� ::{GUID}���Ƿ���shell��
	static bool exists(std::wstring fullPath);
	// ö���ļ�������Ϣ�����Ӧ��չ��
	static void enumFileTypeExtensionInfo();
	// ��ö���������ļ�������Ϣ�����Ӧ��չ��
	static void enumDeskFileTypeExtInfo();
	static void addFileTypeExtInfo(std::wstring &wsType, std::wstring &wsExt);
	// �õ�ö�ٵ��ļ�����-��չ������
	static std::multimap<std::wstring, std::wstring>& getFileTypeExtensionInfo();
private:
	// key=�ļ����ͣ��磺�ı��ļ��� value=��չ�����磺.txt����ע�����������ظ�Ŷ���磺.txt��.wtx���С��ı��ĵ���
	static std::multimap<std::wstring, std::wstring> mc_mapFileTypeExtension;	
	
public:
	std::wstring getKeyName() const;
	const std::wstring& getName() const {			return m_wsName;								}
	void setName(PCWSTR pName) {					m_wsName = pName ? pName : L"";					}
	const std::wstring& getFileName() const {				return m_wsFileName;							}
	void setFileName(PCWSTR pfilename) {			m_wsFileName = pfilename ?pfilename :L"";		}
	int getIndex() {								return m_iIndex;								}
	void setIndex(int idx) {						m_iIndex = idx;									}
	const std::wstring& getFullPath() const {		return m_wsFullPath;							}
	void setFullPath(PCWSTR pfullpath) {			m_wsFullPath = pfullpath?pfullpath:L"";			}
	const std::wstring& getDirectory() const {		return m_wsDirectory;							}
	void setDirectory(PCWSTR pdir) {				m_wsDirectory = pdir ?pdir :L"";				}
	const std::wstring& getLVSize() const {			return m_wsLVSize;								}
	void setLVSize(PCWSTR psize) {					m_wsLVSize = psize ?psize :L"";					}
	const std::wstring& getLVType() const {			return m_wsLVTypeName;							}
	void setLVTypeName(PCWSTR ptype) {				m_wsLVTypeName = ptype?ptype:L"";				}
	const std::wstring& getLVModifyDT() const {		return m_wsLVModifyDT;							}
	void setLVModifyDT(PCWSTR pmodifydt) {			m_wsLVModifyDT = pmodifydt?pmodifydt:L"";		}

	SFGAOF getAttributes() const {					return m_ulAttributes;							}
	void setAttributes(DWORD attributes) {			m_ulAttributes = attributes;					}
	bool isHide() const {							return m_beShow == BENodeShowFlagHide;			}
	bool isCut() const {							return m_beShow == BENodeShowFlagCut;			}
	bool isShow() const {							return m_beShow == BENodeShowFlagShow;			}
	BENodeShowFlag getShowFlag() const {			return m_beShow;								}
	void setShowFlag(BENodeShowFlag showFlag) {		m_beShow = showFlag;							}
	bool isTemporary() const {						return m_bTemporary;							}
	void setTemporary(bool temp = true) {			m_bTemporary = temp;							}
	SIZE getSizeBox() const {						return m_szNode;								}
	void setSizeBox(const SIZE &sz) {				m_szNode = sz;									}
	std::size_t getFileSize() const {				return m_uSize;									}
	void setFileSize(std::size_t filesize) {		m_uSize = filesize;								}
	DATE getCreatedTime() const {					return m_dtCreated;								}
	void setCreatedTime(DATE dtCreated) {			m_dtCreated = dtCreated;						}
	int32_t getOptFlag() const {					return m_iOptFlag;								}
	void setOptFlag(int32_t flag) {					m_iOptFlag = flag;								}
	//LPITEMIDLIST getIIDL() const {					return m_pIDLST;								}
	int32_t getPOSIndex() const {					return m_iPOSIndex;								}
	void setPOSIndex(int32_t posIndex) {			m_iPOSIndex = posIndex;							}
	std::wstring toString() {
		std::wostringstream out;
		out << L"���ͣ�" << ::toString(getGroupType()) 
			<< L"\t���ԣ�0x" << std::hex << std::uppercase << m_ulAttributes << std::dec << std::nouppercase
			<< L"\t��ǣ�" << (m_beShow == BENodeShowFlagHide ? L"hide" : (m_beShow== BENodeShowFlagCut ?L"cut" : L"show")) << L" " << (m_bTemporary ? L"temp" : L"" )
			<< L"\tLV���ͣ�" << getLVType() << L"  LV��С��" << getLVSize() << L"  LV���ڣ�" << getLVModifyDT()
			<< L"\t�ڵ㣺" << getIndex() << L"(" << m_szNode.cx << L"," << m_szNode.cy << L")"
			//<< L"\t��������" << getTypeName() << L"=" << getLVType()
			<< L"\t��ʾ����" << getName()
			<< L"\tKey����<" << getKeyName() << L">"
			//<< L"\tLV���ԣ�" << m_wsLVTypeName << L" " << m_uSize << L" " << m_wsLVCreated
			//<< L"\tȫ·����" << getFullPath() 
			//<< L"\tĿ�����ͣ�" << getTargetType()
			//<< L"\tĿ��·����" << getTargetPath()
			<< std::ends;
		return out.str();
	}
private:
	BEDeskGroupFlag	m_beGroupFlag;		// 
	//LPITEMIDLIST	m_pIDLST;			// ��ö�ٵĽڵ㸴�ƶ���

	std::wstring	m_wsName;			// ��ʾ��(���ܲ�����չ��)
	std::wstring	m_wsFileName;		// ����չ�����ļ�����
	std::wstring	m_wsLVSize;			// 8
	std::wstring	m_wsLVTypeName;		// ��������SysListView32�õ��Ľڵ�����	// 
	std::wstring	m_wsLVModifyDT;		// 8

	SFGAOF			m_ulAttributes;		// ���Ա�ǣ���λbit����
	//DWORD			m_dwFileAttributes;	// �ļ�����
	int32_t			m_iIndex;			// �ڵ���SysListView�е�������ע�⣺����������ͼ��ʱ������ı�
	//std::wstring	m_wsTypeName;		// ������
	std::wstring	m_wsFullPath;		// ȫ·����
	std::wstring	m_wsTargetPath;		// ��ݼ���Ӧ��Ŀ���ļ�·��
	std::wstring	m_wsTargetType;		// Ŀ���ļ�������
	std::wstring	m_wsDirectory;		// �ڵ�����Ŀ¼���磺���桢��������ȣ�
	int32_t			m_iSmallIcon;		// Сͼ������16*16
	int32_t			m_iLagerIcon;		// ��ͼ������
	int32_t			m_iSmallOpenIcon;	// ѡ��Сͼ������
	int32_t			m_iLagerOpenIcon;	// ѡ�д�ͼ������
	HICON			m_hIcon;			// �ļ�ͼ����

	std::size_t		m_uSize;			// �ļ���С		// ����ͨ��SysListView32�õ�
	DATE			m_dtCreated;		// ������ʱ��ʱ��	
	//std::wstring	m_wsLVCreated;		// ������ʱ��ʱ��	// ����ͨ��SysListView32�õ�
	std::time_t		m_dtLastModify;		// ����޸�ʱ��ʱ��

	BENodeShowFlag	m_beShow;			// ͼ���Ƿ���Ҫ���أ��磺ͼ�겻�ڴ��ڵĿ���������Ҫ���أ������ص�ͼ�걻�Ƶ�������
	bool			m_bTemporary;		// ͼ��ʱ��ʱ�ģ��磺���ҵĵ��ԡ�����ϵͳ�˳�ʱҪɾ��

	int32_t			m_iPOSIndex;		// ����λ���������鴰���ڣ������ҡ����ϵ��¡������ϣ����ϵ��¡������ң�
	int32_t			m_iOptFlag;			// �������
	SIZE			m_szNode;			// �ڵ�ĳߴ�
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	kpdeskAdmin��	2014-04-30
//	������Ϣ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class kpdeskAdmin {
private:
	kpdeskAdmin(const kpdeskAdmin &admin);
	kpdeskAdmin& operator =(const kpdeskAdmin &admin);
public:
	kpdeskAdmin();
	virtual ~kpdeskAdmin();
public:
	/*****************************************************************************************
		<< --- kpdeskAdmin::open		2014-05-30 --- >>
		˵�����õ�����ӿ�ָ�룬�ڴ����ָ��
		������
			hLVWnd	= ����SysListView32���ھ��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool open(HWND hLVWnd);
	bool isOpen() const;
	/*****************************************************************************************
		<< --- kpdeskAdmin::close		2014-05-30 --- >>
		˵�����Ƿ�open�����ָ��
		������
		����ֵ��
	*****************************************************************************************/
	void close();

	/*****************************************************************************************
		<< --- kpdeskAdmin::enumNodes		2014-05-30 --- >>
		˵����ö������ڵ�
		������
		����ֵ��
			>= 0	�ɹ����ڵ���
			< 0		ʧ��
	*****************************************************************************************/
	int enumNodes();

	/****************************************************************************************
		<< --- size		2014-11-6 --- >>         
		˵��������ö�ٵ�����ڵ�����
		������
		����ֵ��>=0
	*****************************************************************************************/
	int size() const;

	/*****************************************************************************************
		<< --- kpdeskAdmin::freeNodes		2014-05-30 --- >>
		˵�����ͷŲ���սڵ�
		������
		����ֵ��
	*****************************************************************************************/
	void free();
	/****************************************************************************************
		<< --- freeNone		2014-11-6 --- >>         
		˵�����ͷ�����Ϊ-1�Ľڵ�
		������
		����ֵ��
	*****************************************************************************************/
	void freeNone();

	/****************************************************************************************
		<< --- clearOptFlag		2014-11-6 --- >>         
		˵�����������ڵ������
		������
		����ֵ��
	*****************************************************************************************/
	void clearOptFlag();

	/****************************************************************************************
		<< --- getNodeKeyNames		2014-12-8 --- >>         
		˵�����õ�����ָ�����ǵĽڵ㼯��
		������
			groupFlag	= �����ҽڵ������
		����ֵ��ָ�����ǽڵ㼯��
	*****************************************************************************************/
	std::vector<std::wstring> getNodeKeyNames(BEDeskGroupFlag groupFlag);
public:
	/****************************************************************************************
		<< --- existsNode		2014-11-3 --- >>         
		˵��������ָ���ڵ��Ƿ���ڣ�������ڷ��ؽڵ�����
		������
			keyName	= ������Ľڵ�����
		����ֵ���ڵ�ָ�룬�����ڷ��ؿ�
	*****************************************************************************************/
	idlNode* existsNode(const std::wstring &keyName);
	idlNode* existsNodeSEH(const std::wstring &keyName);
	/****************************************************************************************
		<< --- existsNode		2014-11-21 --- >>         
		˵�������ҽڵ�������Ӧ����
		������
			nodeIndex	= �����ҽڵ�����
		����ֵ���ڵ�ָ�룬�����ڷ��ؿ�
	*****************************************************************************************/
	idlNode* existsNode(const int nodeIndex);
	idlNode* existsNodeSEH(const int nodeIndex);

	/****************************************************************************************
		<< --- findNode		2014-11-3 --- >>         
		˵�������ݽڵ�key���Ʋ��ҽڵ����
		������
			keyName	= ������Ľڵ�
		����ֵ���ڵ�ָ�룬�����ڷ���NULL
	*****************************************************************************************/
	gtc::idlNode* findNode(const std::wstring &keyName, PCWSTR pDesc);
	/****************************************************************************************
		<< --- findNode		2014-11-3 --- >>         
		˵�������ݽڵ��������ҽڵ����
		������
			nodeIndex	= ������Ľڵ�����
		����ֵ���ڵ�ָ�룬�����ڷ���NULL
		= -1���޷�������
		= 0��ʧ��
		= 1���ɹ�
	*****************************************************************************************/
	int findNode(const int nodeIndex, gtc::idlNode *&pNode);

	/****************************************************************************************
		<< --- addNode		2014-11-3 --- >>         
		˵������ӽڵ�
		������
			keyName		= ��ɾ���ڵ�key����
			prefNode	= ���������ڵ�ָ��
		����ֵ��
		= -1���޷�������
		= 0��ʧ��
		= 1���ɹ�
		= 2���ڵ��Ѿ�����
	*****************************************************************************************/
	int addNode(const std::wstring &keyName, idlNode *& prefNode);

	/****************************************************************************************
		<< --- deleteNode		2014-11-3 --- >>         
		˵����ɾ���ڵ�
		������
			keyName	= ��ɾ���ڵ�key����
		����ֵ��
			= -1���޷�������
			= 0��ʧ��
			= 1���ɹ�
	*****************************************************************************************/
	int deleteNode(const std::wstring &keyName);

	/****************************************************************************************
		<< --- renameNode		2014-11-3 --- >>         
		˵�����������ڵ�
		������
			oldKeyName	= ԭ�ڵ�key����
			newKeyName	= ��������ڵ��key����
		����ֵ��
	*****************************************************************************************/
	bool renameNode(std::wstring oldKeyName, std::wstring newKeyName);

	/****************************************************************************************
		<< --- updateNode		2014-11-17 --- >>         
		˵�������½ڵ���Ϣ������
		������
			keyName	= �����½ڵ�key����
		����ֵ��
			= -1���޷�������
			= 0��ʧ��
			= 1���ɹ�
			= 2���ڵ㲻���ڣ�����Ϊ��ʱ�ļ���δ��ӣ�
	*****************************************************************************************/
	int updateNode(std::wstring keyName);
	//bool updateNode(std::wstring keyName, std::wstring tmpKeyName);
	/****************************************************************************************
		<< --- getNodeIIDLs		2014-11-3 --- >>         
		˵������������ڵ�������shell���ҽڵ��Ӧ��ITEMIDLIST*
		������
			nodeIndex = ����ڵ�����
		����ֵ���ڵ�ITEMIDLISTָ�룬ʧ�ܷ���NULL��������Ҫ�������ͷš�����
	*****************************************************************************************/
	LPITEMIDLIST getNodeIIDLs(int nodeIndex);

	/****************************************************************************************
		<< --- refreshNodeOffset		2014-11-3 --- >>         
		˵�����ƶ��ڵ�ʱҪ׷�ӵ�ƫ���������������л�ͼ���СʱҪˢ��ƫ����
		������
		����ֵ��
	*****************************************************************************************/
	void refreshNodeOffset();

	/****************************************************************************************
		<< --- refreshNodeMaxSize		2014-11-3 --- >>         
		˵�������µõ��ڵ�����ߴ�
		������
		����ֵ��
	*****************************************************************************************/
	void refreshNodeMaxSize();

	/****************************************************************************************
		<< --- findNodeIndex		2014-11-3 --- >>         
		˵������������������ָ���ڵ�����Ӧ����ڵ�����
		������
			pNode	= �����Ҷ���
		����ֵ������ڵ��������ڵ㲻���ڷ���-1
	*****************************************************************************************/
	int findNodeIndex(const gtc::idlNode *pNode);

	/****************************************************************************************
		<< --- isEqualNode		2014-11-3 --- >>         
		˵�����������������������Ͻڵ���shell�еĽڵ��Ƿ�һ��
		������
			nodeIndex	= ���ȽϽڵ㣬��������ڵ�����
			pNode		= ���ȽϽڵ㣬����shellö�ٵĽڵ����
		����ֵ���Ƿ�һ��
	*****************************************************************************************/
	bool isEqualNode(int nodeIndex, const gtc::idlNode *pNode);
	bool isEqualNode(const std::wstring &wsName, const std::wstring &wsSize, const std::wstring &wsType, const std::wstring &wsDT, const gtc::idlNode *pNode);

	/****************************************************************************************
		<< --- setNodeListViewInfo		2014-11-3 --- >>         
		˵����������������������ڵ����Ը��½ڵ����
		������
			pNode		= �����¶���
			nodeIndex	= ����Դ�ڵ�����
			ptpos		= ��Ϊ��ʱ������ڵ�λ������
		����ֵ��
	*****************************************************************************************/
	void setNodeListViewInfo(gtc::idlNode *pNode, int nodeIndex, POINT *ptpos = NULL);
	/****************************************************************************************
		<< --- moveto		2014-11-3 --- >>         
		˵�����ƶ��ڵ�
		������
			keyName	= ���ƶ��ڵ�key����
			pt		= Ŀ��λ��
		����ֵ��
			= -1���޷�������
			= 0��ʧ��
			= 1���ɹ�
			= 2���ڵ㲻���ڣ�����Ϊ��ʱ�ļ���δ��ӣ�
	*****************************************************************************************/
	int moveto(const std::wstring &keyName, POINT pt);
	int movetoSEH(const std::wstring &keyName, POINT pt);
	/****************************************************************************************
		<< --- refreshAllNode		2014-11-19 --- >>         
		˵����ˢ�����нڵ�
		������
		����ֵ��
	*****************************************************************************************/
	bool refreshAllNode();

	/****************************************************************************************
		<< --- refreshNodeSize		2014-11-3 --- >>         
		˵����ˢ�����нڵ�ĳߴ�
		������
		����ֵ��
	*****************************************************************************************/
	void refreshAllNodeSize();

	/****************************************************************************************
		<< --- redrawNode		2014-11-3 --- >>         
		˵�������»��ƽڵ�
		������
			nodeIndex	= �����ƽڵ�����
		����ֵ��
	*****************************************************************************************/
	void redrawNode(int nodeIndex);

	/****************************************************************************************
		<< --- refreshHideNode		2014-11-3 --- >>         
		˵���������ƶ��������صĽڵ�
		������
		����ֵ��
	*****************************************************************************************/
	void refreshHideNode();
	/****************************************************************************************
		<< --- redrawCutNode		2014-11-3 --- >>         
		˵�������»���cut�ڵ�
		������
		����ֵ��
	*****************************************************************************************/
	void redrawCutNode();

	/****************************************************************************************
		<< --- restoreNodePosition		2014-11-3 --- >>         
		˵�����ָ�����ڵ㵽ԭ��λ��
		������
		����ֵ��
	*****************************************************************************************/
	void restoreNodePosition();
	///****************************************************************************************
	//	<< --- decrementNodeIndex		2014-11-3 --- >>         
	//	˵����
	//	������
	//	����ֵ��
	//*****************************************************************************************/
	//void decrementNodeIndex(std::wstring keyName);
public:
	const gtc::idlNode::IDINodeMap& getNodes() const;
	SIZE getNodeMaxSize() const;
	SIZE getNodeOffset() const;
private:
	/****************************************************************************************
		<< --- saveNodePosition		2014-11-3 --- >>         
		˵��������ڵ�ԭ��λ�ã������ݽڵ������������½ڵ����
		������
		����ֵ��
	*****************************************************************************************/
	void saveNodePosition();
	/****************************************************************************************
		<< --- makeNode		2014-12-18 --- >>         
		˵�������ڵ���뼯����
		������
		����ֵ��
	*****************************************************************************************/
	void makeNode(idlNode *pNode);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	����Ϊ��SEH	2015-1-7
	idlNode* wrapExistsNode(int nodeIndex);
	idlNode* wrapExistsNode(const std::wstring &keyName);
	int wrapMoveto(const std::wstring &keyName, POINT pt);
protected:
	gtc::idlNode::IDINodeMap	m_mapNodes;			// ����ڵ���Ϣ
	StringMapType			m_mapSystemNodes;	// ϵͳ�ڵ㼯�ϡ�key=��ʾ�����磺������壻value=::{CLSID}
	CRITICAL_SECTION			m_csLockNodes;		// m_mapNodes������
	
	SIZE						m_szMaxNode;		// �ڵ�����ߴ�
	SIZE						m_szOffset;			// �ƶ��ڵ�ʱ���ϵ�ƫ���������ڵ��Ƶ�0��0ʱ�����겢��Ϊ0��0����������

	HWND						m_hDeskLV;			// ����SysListView32���ھ��
	//IShellFolder2				*m_pshFolder;		// 
};


}
#endif
