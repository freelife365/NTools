#ifndef __SHELLEXT_H__
#define __SHELLEXT_H__
#pragma once
#include "AC/basedef.h"
//#include "dragDrop.h"
//#include <ShlObj.h>

// shell�˵��Χ
#define SHELL_MENU_MIN_ID		1
#define SHELL_MENU_MAX_ID		1000

namespace acm {
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-12 shell��չ�˵�������	
struct BSContextMenuHandle {
	HMODULE			hDLL;			// shell��չ�ļ�����̬����
	IUnknown		*pUnknown;		// 
	std::wstring	wsKeyName;		// ע�������ؼ�������
	std::wstring	wsClsid;		// ���CLSID

	BSContextMenuHandle()
		: hDLL(NULL), pUnknown(NULL)
	{}
};
typedef std::vector<BSContextMenuHandle> ContextMenuHandleArrayType;

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-15 STRINGRESOURCEIMAGE
struct BSStringResourceImage {
	WORD	nLength;	
	wchar_t	chString[1];
};
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-12 	
// shell32.dll SHCreateItemFromParsingName Vista�����Ժ�汾��֧��
typedef HRESULT (__stdcall * fnSHCreateItemFromParsingName)(PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv);


//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-25 	�ڵ�ϵͳ��������
enum BEFileSyscmdFlag {
	BEFileSyscmdFlagOpen			= 1,	// �򿪣����ֱ�ǣ�&o &O
	BEFileSyscmdFlagCopy			,		// ���ƣ����ֱ�ǣ�&c &C
	BEFileSyscmdFlagCut				,		// ���У����ֱ�ǣ�&t &T
	BEFileSyscmdFlagPaste			,		// ճ�������ֱ�ǣ�&p &P
	BEFileSyscmdFlagDelete			,		// ɾ�������ֱ�ǣ�&d &D
	BEFileSyscmdFlagUndoDelete		,		// ����վ��ԭ�����ֱ�ǣ�&E
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	shell��	2014-12-26
//	windows shell ��չ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class shell {
private:
	shell() {}
public:
	/****************************************************************************************
		<< --- 	findDeskHandle	2015-5-18 --- >>         
		˵�����������洰�ھ��
		������
			refProgman		= ���ء�Program Manager�����ھ��
			refShelldll		= ���ء�ShellDLL_DefView�����ھ��
			refDeskListView	= ���ء�SysListView32�����ھ��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool findDeskHandle(HWND &refProgman, HWND &refShelldll, HWND &refDeskListView);
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-18 �����ļ���Ŀ¼���ļ����ļ������ƴ���IShellItem�ӿ�
	static bool createShellItem(PCWSTR pRootName, PCWSTR pName, REFIID riid, void **ppv);
	static bool createShellItem(PCWSTR pFullPath, REFIID riid, void **ppv);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-17 ����ȫ·���ļ����ϴ���IShellItemArray�ӿ�
	static bool createShellItemArray(const StringArrayType &vecFiles, REFIID riid, void **ppv);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-4-16 �ͷ�
	static void free(IIDLArrayType &vecIIDLs);
public:
	static UINT mc_uiSmallIconSize;		// Сͼ��Ĵ�С
	/****************************************************************************************
		<< --- createThumbnail		2014-12-26 --- >>         
		˵���������ļ�������ͼ��֧��Windows Vista [desktop apps only]
		�ο���http://blog.163.com/lvan100@yeah/blog/static/6811721420118375054348/
		������
			pfullPath	= ��Ҫ�õ�����ͼ�ļ���ȫ·��
			imgCXCY		= ����ͼ��С����=�ߣ��磺16 32 48 96 256 ...����Ȼ 300x300 100x300Ҳ֧��
			prioritizeOnlyThumbnail	= ���ļ�Ϊϵͳ�ڵ㡢Ӧ��������ļ��С�ͼƬʱ���ȴ�������ͼ
		����ֵ��= NULLʧ�ܣ�����ɹ�
	*****************************************************************************************/
	static HBITMAP createThumbnail(PCWSTR pfullPath, int imgCXCY);
	/****************************************************************************************
		<< --- 	parseLinkTarget	2015-2-3 --- >>         
		˵����ͨ������.lnk�ļ��õ���Ŀ������ȫ·��
		������
			plnkPath		= ���������ļ�
			wsTargetPath	= lnk�ļ���Ӧ��Ŀ�����ȫ·��
			pwsTargetParam	= ���Ӳ���
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool parseLinkTarget(PCWSTR plnkPath, std::wstring &wsTargetPath, std::wstring *pwsTargetParam = NULL);

	/****************************************************************************************
		<< --- 		2015-2-3 --- >>         
		˵����ͨ������.lnk�ļ��õ�ͼ���ļ�·����������ͼ���ļ��е�λ������
		������
			plnkPath		= ���������ļ�
			wsIconPath		= �ļ�ͼ�����ڵ��ļ�
			iIconIndex		= ͼ�����ļ��е�����
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool parseLinkIconFile(PCWSTR plnkPath, std::wstring &wsIconPath, int &iIconIndex);

	/****************************************************************************************
		<< --- 		2015-2-3 --- >>         
		˵������imageres.dll���ע����в������ͼ��
		������
		pfullPath		= ���������ļ�
		wsIconPath		= �ļ�ͼ�����ڵ��ļ�
		iIconIndex		= ͼ�����ļ��е�����
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool parseAssociateIconFile(PCWSTR pfullPath, std::wstring &wsIconPath, int &iIconIndex);

	/****************************************************************************************
		<< --- 	parseFileForIconFile	2015-2-3 --- >>         
		˵����ͨ�������ļ��õ����Ӧ��ͼ���ļ�·����������ͼ���ļ��е�λ������
		������
			pfullPath		= ���������ļ�
			wsIconPath		= �ļ�ͼ�����ڵ��ļ�
			iIconIndex		= ͼ�����ļ��е�����
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool parseFileForIconFile(PCWSTR pfullPath, std::wstring &wsIconPath, int &iIconIndex);
	static bool parseFileForIconFile(PCWSTR pfullPath, PWSTR pIconPath, DWORD dwIconPathLen, int &iIconIndex);
	/****************************************************************************************
		<< --- 	extractFileIcon	2015-2-3 --- >>         
		˵������ȡ�ļ���Ӧ��ͼ�ꣻ��ͨ�ļ���ͨ��ע���õ���ͼƬͨ������ͼ������ͨ��IExtractIcon�õ�
		������
			pfullPath	= ����ȡ�ļ�
			iconCXCY	= ͼ��Ĵ�С
			hIcon		= ����iconCXCY�ߴ�ͼ��
			hSmallIcon	= ����Сͼ��16x16
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool extractFileIcon(PCWSTR pfullPath, int iconCXCY, HICON &hIcon, HICON &hSmallIcon);
	/****************************************************************************************
		<< ---	extractFolderIcon		2015-2-4 --- >>         
		˵������ͨ�����Ե��ļ��е�ͼ�궼һ��������ֱ�Ӵ�ע�������ȡһ�ξͿ�����
		������
			iconCXCY	= ͼ��Ĵ�С
			hIcon		= ����iconCXCY�ߴ�ͼ��
			hSmallIcon	= ����Сͼ��16x16
		����ֵ���Ƿ�����ɹ�
	*****************************************************************************************/
	static bool extractFolderIcon(int iconCXCY, HICON &hIcon, HICON &hSmallIcon);

	/****************************************************************************************
		<< --- 	parseShellMenu	2015-2-12 --- >>         
		˵��������Ŀ¼������˵����
		������
			vecIIDLs		= Ҫ����Ŀ¼���ļ��У����� >= 1
			poutContextMenu	= ��Ϊ��ʱ���ز˵�IContextMenu*
		����ֵ��Ŀ¼���ļ��ж�Ӧ�Ĳ˵���ʧ�ܷ���NULL
	*****************************************************************************************/
	static HMENU parseShellMenu(IShellFolder *pshFolder, IIDLArrayType &vecIIDLs, bool bRename = false, IContextMenu **poutContextMenu = NULL, HWND hwndOwner = NULL);


	/****************************************************************************************
		<< --- 		2015-2-13 --- >>         
		˵������ע������λ����ȡ�����Ĳ˵�
		������
			pregKey			= ����ȡ�˵���λ��
			vecContextMenu	= ����ȡ�������Ĳ˵���Ϣ
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool extractContextMenuHandle(PCWSTR pregKey, ContextMenuHandleArrayType &vecContextMenu);

	static bool parseShellMenuFromContextMenuHandle(const BSContextMenuHandle &contextMenu, IContextMenu **pcontextMenu = NULL, 
		IContextMenu2 **pcontextMenu2 = NULL, IContextMenu3 **pcontextMenu3 = NULL);
	/****************************************************************************************
		<< --- 	extractStringFromDLL	2015-2-15 --- >>         
		˵��������Դ�ļ�����ȡ�ַ���
		������
			pregValue	= ��ע�������ȡ�ġ�@����ͷ���ַ���ֵ
			pdllpath	= �洢����ȡ�ַ�������Դ�ļ�·��
			uiResID		= ��ԴID
			wsOutValue	= ��ȡ���ַ���
			wLanguageID	= ���Ի���ID
		����ֵ���Ƿ�ɹ���ȡ
	*****************************************************************************************/
	static bool extractStringFromDLL(PCWSTR pdllpath, UINT uiResID, std::wstring &wsOutValue, WORD wLanguageID = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
	static bool extractStringFromDLL(const std::wstring &wsregValue, std::wstring &wsOutValue, WORD wLanguageID = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

	///****************************************************************************************
	//	<< --- 	createNewFolder	2015-2-26 --- >>         
	//	˵������ָ��Ŀ¼�´������½��ļ��С�
	//	������
	//		wsParentPath	= ���ļ��еĸ�Ŀ¼
	//	����ֵ�������½��ļ��е�ȫ·��
	//*****************************************************************************************/
	// �÷����Ѿ��Ƶ�gtc::file��
	//static std::wstring createNewFolder(const std::wstring &wsParentPath);
	/****************************************************************************************
		<< --- 		2015-2-26 --- >>         
		˵������ָ��Ŀ¼�´������¿�ݷ�ʽ��
		������
			wsParentPath	= �½���ݷ�ʽ�ĸ�Ŀ¼
		����ֵ��
	*****************************************************************************************/
	static void createNewLink(const std::wstring &wsParentPath);
	/****************************************************************************************
		<< --- 	execSystemMenuCommand	2015-3-23 --- >>         
		˵����ִ�нڵ��Ҽ��˵��е�ϵͳ����磺���򿪡��������ơ�����ճ����
		������
			vecIIDLs	= Ҫ�������ļ����ļ��м���
			wsFullPath	= Ҫ�������ļ����ļ���
			cmdFlag		= ������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool execSystemMenuCommand(IShellFolder *pshFolder, IIDLArrayType &vecIIDLs, BEFileSyscmdFlag cmdFlag, HWND hWnd = NULL, PCSTR pParameters = NULL);
	static bool execSystemMenuCommand(std::wstring wsFullPath, BEFileSyscmdFlag cmdFlag, HWND hWnd = NULL, PCSTR pParameters = NULL);
	static bool execSystemMenuCommand(const StringArrayType &vecFullPath, BEFileSyscmdFlag cmdFlag, HWND hWnd = NULL, PCSTR pParameters = NULL);
	/****************************************************************************************
		<< --- execSystemCommand		2015-4-8 --- >>         
		˵����ͨ��ShellExecuteEx ��ָ���ļ�ִ�в���
		������
			pcmdFile	= ��ִ�е��ļ�
			pCmdVerb	= �����ǣ��磺open��edit��explore��find��print��properties
			pParameters	= ����
		����ֵ��
	*****************************************************************************************/
	static bool execSystemCommand(PCWSTR pcmdFile, PCWSTR pCmdVerb = L"open", PCWSTR pParameters = NULL, HWND hWnd = NULL);
	/****************************************************************************************
		<< --- 	extractTipInfo	2015-3-27 --- >>         
		˵�����õ��ļ����ļ��еȽڵ����ʾ��Ϣ
		������
			piidl		= ����ȡ���ļ����ļ���
			refwsTip	= ���ص���ʾ��Ϣ
		����ֵ��
	*****************************************************************************************/
	static bool extractTipInfo(LPCITEMIDLIST piidl, std::wstring &refwsTip);
	static bool extractTipInfo(std::wstring wsFullPath, std::wstring &refwsTip);
protected:
	/****************************************************************************************
		<< --- 	loadContextMenuHandleFromCLSID	2015-2-13 --- >>         
		˵����ͨ��CLSID�õ�shell��չ�˵���Ϣ
		������
			pCLSID		= ָ����չ���CLSID
			menuHandle	= ���صĲ˵���Ϣ
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool loadContextMenuHandleFromCLSID(PCWSTR pCLSID, BSContextMenuHandle &menuHandle);

	/****************************************************************************************
		<< --- 		2015-2-15 --- >>         
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	static const BSStringResourceImage* getStringResouceImage(HINSTANCE hInstance, HRSRC hResource, UINT id);

	
};


}	// end acm
#endif	// end __SHELLEXT_H__