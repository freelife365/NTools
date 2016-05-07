#pragma once
#include <unordered_set>
#include "AC/sysupport.h"
#include <ostream>
//#include "AC/MFC/menu.h"
// ���û����͵�����Ϣ
#if defined(_DEBUG)
#define SEND_LOG		__sendtoUser
#define WRITE_LOG		gtc::app::writeVSLogs
#else
#define SEND_LOG		//gtc::app::writeVSLogs//__sendtoUser
#define WRITE_LOG		//gtc::app::writeVSLogs
#endif
/****************************************************************************************
	��"�������湤����"�������Ϣ�����ļ�
	��[deskset]��
		HideIcon��int �Ƿ�֧��˫���������湦�ܣ�=1֧�֣�����֧��
	��[SHARE]��
		ShowDesk��int �����Ƿ�����ʾ״̬��=1��ʾ
		KPDESK��int ������������Ƿ��������У�=1������
*****************************************************************************************/
#define KPDESK_SHARE_INI	L"kpzm.ini"

//////////////////////////////////////////////////////////////////////////
// �����Ҽ��˵��Զ�����͵���Ϣ
// WPARAM = �˵�ID��LPARAM = ��Ϣ��־
static UINT gc_uiKPDeskRegMsg			= RegisterWindowMessageW(L"KPDeskInvokeCmd");	// ע����Ϣ
// 
static UINT gc_uiKPDeskHookRun			= RegisterWindowMessageW(L"KPDeskHookRun");	// ע����Ϣ
static UINT gc_uiKPDeskHookStop			= RegisterWindowMessageW(L"KPDeskHookStop");	// ע����Ϣ
// �������洰����
#define KPDESK_DESKTOP_CLASS			L"KpDeskDui"	// �������洰����
#define KPDESK_DESKTOP_CAPTION			L"�������湤����"	// �������洰�ڱ���
// ����������񴰿�
#define KPDESK_DESKHOST_CLASS			L"KPHostClass"	// hostWnd��������
#define KPDESK_DESKHOST_CAPTION			L"KPWork"		// hostWnd���ڱ���
// �����ػ淽ʽ���
#define KPDESK_REDRAW_HIDEDESK			0x001			// �Ƿ��������洰��
#define KPDESK_REDRAW_MOVEGROUP			0x002			// �Ƿ������ƶ��鴰��
#define KPDESK_REDRAW_ALL				(KPDESK_REDRAW_HIDEDESK|KPDESK_REDRAW_MOVEGROUP)

#define KPDESK_WMME_RUNORSTOP			WM_USER+201		// �����Ҽ��˵����������������������͸�KPDESK_DESKTOP_CLASS����Ϣ
#define KPDESK_WMME_ADDNODE				WM_USER+202		// ������ӽڵ� WPARAM=�ڵ�key���ƣ�wstring*
#define KPDESK_WMME_DELETENODE			WM_USER+203		// ����ɾ���ڵ� WPARAM=�ڵ�key���ƣ�wstring*��LPARAM=0ɾ�����Ƴ��ڵ� 1���Ƴ��ڵ�
#define KPDESK_WMME_UPDATENODE			WM_USER+204		// ���Ը��½ڵ� WPARAM=�ڵ�key���ƣ�wstring*

// ͬ����������
#define KPDESK_MUTEX_NAME				L"KPDeskRunning"
#define KPDESK_EVENT_WAITFORSTART		L"KPDeskWaitforStart"
#define KPDESK_EVENT_WAITFOREXIT		L"KPDeskWaitforExit"
// 
#define KPDESK_GROUP_DEFAULT_TITLE		L"�½���"		// �½����Ĭ������
#define KPDESK_GROUP_TITLE_MAXLEN		20			// �鴰����������ַ���
#define KPDESK_GROUP_NEXTROW_SPACE		20			// ��������ڵ��������±߿��������ʱ-->����
#define KPDESK_GROUP_NEXTROW_TIMERID	101			// ��ԡ����С���ʱ��ID
#define KPDESK_GROUP_MOVESIZE_TIMERID	102			// ����鴰���ƶ���ı��Сʱ�ƶ��ڵ��ʱ��ID

#define KPDESK_MENU_START				L"����ͼ������"
#define KPDESK_MENU_EXIT				L"�ر�ͼ������"
//
// ��չ�˵� ��Ϣ��־��Ҫ���Ҽ��˵���չ��˳��һ��Ŷ
#define KPDESK_MENUID_MAIN				0		// ������رտ����������
#define KPDESK_MENUID_ICONLARGE			1		// ��ͼ��
#define KPDESK_MENUID_ICONNORMAL		2		// ��ͼ��
#define KPDESK_MENUID_ICONSMALL			3		// Сͼ��
#define KPDESK_MENUID_ALGINAUTO			4		// �Զ�����ͼ��
#define KPDESK_MENUID_ALGINGRID			5		// ��ͼ�����������
#define KPDESK_MENUID_SHOWDESK			6		// ��ʾ����ͼ��
#define KPDESK_MENUID_SORTNAME			7		// ��������
#define KPDESK_MENUID_SORTFILESIZE		8		// �����ļ���С
#define KPDESK_MENUID_SORTTYPE			9		// ��������
#define KPDESK_MENUID_SORTDTMODIFY		10		// �����޸�����
#define KPDESK_MENUID_REFRESH			11		// ˢ��


//// �˵�������ʽ������
//static acm::menuItem gc_menuItemSort[] = {
//	acm::menuItem(L"����"), 
//	acm::menuItem(L"��С"), 
//	acm::menuItem(L"��Ŀ����"), 
//	acm::menuItem(L"�޸�����")
//};
// �����Ҽ��˵�����ID��ע�⣺��ͬ�Ĳ���ϵͳ���ܲ�һ��
// ȫѡ��Ctrl+A��
#define KPDESK_CMDID_SELECTALL_W732			28705		// ȫѡ��Ctrl+A��
// ˢ�²˵�
#define KPDESK_CMDID_REFRESH_W732			28931		
#define KPDESK_ITEMID_REFRESH_W732			30981		// ˢ�²˵���ID		
// ����-����
#define KPDESK_CMDID_SORT_NAME_W732			31492		
#define KPDESK_ITEMID_SORT_NAME_W732		30985		// ����������˵���ID
// ����-��С
#define KPDESK_CMDID_SORT_FILESIZE_W732		31493		
#define KPDESK_ITEMID_SORT_FILESIZE_W732	30984		// ����С����˵���ID
// ����-��Ŀ����
#define KPDESK_CMDID_SORT_TYPE_W732			31494		
#define KPDESK_ITEMID_SORT_TYPE_W732		30983		// ����Ŀ��������˵���ID
// ����-���޸�����
#define KPDESK_CMDID_SORT_DTMODIFY_W732		31495		
#define KPDESK_ITEMID_SORT_DTMODIFY_W732	30982		// ���޸���������˵���ID

// �鿴-��ͼ��256*256
#define KPDESK_CMDID_VIEW_ICONLARGE_W732	28751		
#define KPDESK_ITEMID_VIEW_ICONLARGE_W732	30993		// �鿴-��ͼ��˵���ID	
// �鿴-�е�ͼ��48*48
#define KPDESK_CMDID_VIEW_ICONNORMAL_W732	28750		
#define KPDESK_ITEMID_VIEW_ICONNORMAL_W732	30992		// �鿴-��ͼ��˵���ID			
// �鿴-Сͼ��32*32
#define KPDESK_CMDID_VIEW_ICONSMALL_W732	28752		
#define KPDESK_ITEMID_VIEW_ICONSMALL_W732	30991		// �鿴-Сͼ��˵���ID	
// �鿴-�Զ�����ͼ��
#define KPDESK_CMDID_VIEW_ALIGNAUTO_W732	28785		
#define KPDESK_ITEMID_VIEW_ALIGNAUTO_W732	30990		// �鿴-�Զ�����ͼ��˵���ID	
// �鿴-��ͼ�����������
#define KPDESK_CMDID_VIEW_ALIGNGRID_W732	28788		
#define KPDESK_ITEMID_VIEW_ALIGNGRID_W732	30989		// �鿴-��ͼ�����������˵���ID	
// �鿴-��ʾ����
#define KPDESK_CMDID_VIEW_SHOWDESK_W732		29698		
#define KPDESK_ITEMID_VIEW_SHOWDESK_W732	30987		// �鿴-��ʾ����˵���ID	


//////////////////////////////////////////////////////////////////////////
// ������Ϣ���
static void __sendtoUser(PCWSTR pstrFormat, ...)
{
//#ifdef _DEBUG
	wchar_t szBuffer[1024] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	std::vswprintf(szBuffer, _countof(szBuffer) - 2, pstrFormat, args);
	va_end(args);
	gtc::app::sendtoUser(szBuffer, L"Untitled - testDeskHost" /*L"�ޱ��� - testDeskHost"*/);
	//gtc::app::writeLogs(szBuffer, L"SEND_LOG");
	std::wcscat(szBuffer, L"\n");
	::OutputDebugStringW(szBuffer);
//#endif
}


//////////////////////////////////////////////////////////////////////////
// ϵͳ��ɫ
#define KPDESK_COLOR_GROUP_TITLE			0x20000000	// �鴰�ڱ���������ɫ
#define KPDESK_COLOR_GROUP_TITLE_HOT		0x40000000	// �鴰��ѡ��ʱ����������ɫ
#define KPDESK_COLOR_GROUP_TITLE_TEXT		0xFFFFFFFF	// �鴰�ڱ������ı���ɫ
#define KPDESK_COLOR_GROUP_TITLE_TEXT_HOT	0xFFFFFFFF	// �鴰��ѡ��ʱ�������ı���ɫ
#define KPDESK_COLOR_GROUP_CLIENT			0x20000000	// �鴰�ڿͻ�������ɫ
#define KPDESK_COLOR_GROUP_CLIENT_HOT		0x40000000	// �鴰��ѡ��ʱ�ͻ�������ɫ
#define KPDESK_COLOR_GROUP_BORDER			0x60FFFFFF	// �鴰�ڱ߿���ɫ
#define KPDESK_COLOR_GROUP_BORDER_HOT		0x60FFFFFF	// �鴰��ѡ��ʱ�߿���ɫ
#define KPDESK_COLOR_GROUP_CUTLINE			0x40FFFFFF	// �鴰�ڱ�������ͻ�����ָ�����ɫ
#define KPDESK_COLOR_GROUP_CUTLINE_HOT		0x40FFFFFF	// �鴰��ѡ��ʱ��������ͻ�����ָ�����ɫ

#define KPDESK_COLOR_SCROLL_BK				0x40FFFFFF	// ����������ɫ
#define KPDESK_COLOR_SCROLL_BK_HOT			0x80FFFFFF	// ������ѡ��ʱ����ɫ

#define KPDESK_COLOR_DRAPBAR				0x602DDBC6	// �ڵ����ʱ���鱳��ɫ
//////////////////////////////////////////////////////////////////////////
// ����ϵͳͼ���Ƿ����ر��
#define KPDESK_HIDE_COMPUTE		0x00000001		// �ҵĵ���
#define KPDESK_HIDE_NETWORK		0x00000002		// ����
#define KPDESK_HIDE_CONTROL		0x00000004		// �������
#define KPDESK_HIDE_USER		0x00000008		// �û�
#define KPDESK_HIDE_RECYCLE		0x00000010		// ����վ


//////////////////////////////////////////////////////////////////////////
// �鴰�ڸ��±��
#define KPDESK_SCROLL_FIRST		0	// 
#define KPDESK_SCROLL_UPDATE	1	// 
#define KPDESK_SCROLL_LAST		2	// 
#define KPDESK_SCROLL_TOPKEY	3	// 
#define KPDESK_SCROLL_MOVENODE	4	// �����ƶ�show��cut�ڵ�

///////////////////////////////////////////////////////////////////////////////////////////////////
//	���ɼ��ڵ��ƶ���λ��	2015-1-7
#define  KPDESK_HIDENODE_POSX	-1000	// x������
#define  KPDESK_HIDENODE_POSY	-1000	// x������

//////////////////////////////////////////////////////////////////////////
// �߳���Ϣ
typedef struct BSThreadInfo {
	HANDLE			hThread;		// �߳̾��
	std::wstring	wsDesc;			// ������Ϣ
	void			*pParameter;	// 
	BSThreadInfo(HANDLE hthread, PCWSTR pdesc)
		: hThread(hthread), wsDesc(pdesc?pdesc:L""), pParameter(NULL)
	{}
	~BSThreadInfo()
	{
		WRITE_LOG(L"~BSThreadInfo free ...");
	}
}BSThreadInfo;

//////////////////////////////////////////////////////////////////////////
// �ڵ�����λ�ñ��
typedef enum BENodeDrapPos
{
	BENodeDrapPosNone	= 0,	// 
	BENodeDrapPosFirst	,		// ��λ��
	BENodeDrapPosLeft	,		// ����ڽڵ����
	BENodeDrapPosRight	,		// ����ڽڵ��Ҳ�
	//BENodeDrapPosEnd	,		// ��β��׷��
	BENodeDrapPosOn		,		// �ڽڵ���
}BENodeDrapPos;

//////////////////////////////////////////////////////////////////////////
// ͼ��������
typedef enum BEDeskSortFlag {
	BEDeskSortFlagNone			= 0,	// û������
	BEDeskSortFlagName			,		// ����������
	BEDeskSortFlagFileSize		,		// �ļ���С
	BEDeskSortFlagType			,		// ����
	BEDeskSortFlagDTModify				// �޸�����
};
// ͼ����Դ��key���
typedef enum BEDeskImageFlag {
	BEDeskImageFlagLock			= 1,	// IDB_PNG_LOCK
	BEDeskImageFlagUnlock		,		// IDB_PNG_UNLOCK
	BEDeskImageFlagMenu					// IDB_PNG_MENU
}BEDeskImageFlag;
// �˵���Դ��key���
typedef enum BEDeskMenuFlag {
	BEDeskMenuFlagMain			= 1		// IDR_MENU_MAIN
};
// ����ͼ��Ĵ�С
typedef enum BEDeskNodeiconSize{
	BEDeskNodeiconSizeSmall		= 0,	// 32*32
	BEDeskNodeiconSizeNormal	,		// 48*48��Ĭ�ϣ�
	BEDeskNodeiconSizeLarge				// 256*256
}BEDeskNodeiconSize;
//namespace gtc{
// Ĭ�ϴ��ڷ���
typedef enum BEDeskGroupFlag {
	BEDeskGroupFlagNone			= 0,	// δ֪
	BEDeskGroupFlagSystem		,		// ϵͳ
	BEDeskGroupFlagSoft			,		// Ӧ�ó���
	BEDeskGroupFlagFolder		,		// �ļ���
	BEDeskGroupFlagDocument		,		// �ļ�
	//BEDeskGroupFlagCompress		,		// �ĵ���
	BEDeskGroupFlagPicture		,		// ͼƬ
	BEDeskGroupFlagOther		,		// ����
	BEDeskGroupFlag_Last		= 100	// �Զ����鴰���ڴ��Ժ�
}BEDeskGroupFlag;

static bool inDeskGroupFlag(int groupFlag) {
	return groupFlag > BEDeskGroupFlagNone && groupFlag < BEDeskGroupFlag_Last;
}

static std::wstring toString(BEDeskGroupFlag flag) {
	switch(flag)
	{
	case BEDeskGroupFlagNone:		//	= 0,// δ֪
		return L"δ֪";
		break;
	case BEDeskGroupFlagSystem:		//,		// ϵͳ
		return L"ϵͳ";
		break;
	case BEDeskGroupFlagSoft:		//,		// Ӧ�ó���
		return L"���";
		break;
	case BEDeskGroupFlagFolder:		//,		// �ļ���
		return L"�ļ���";
		break;
	case BEDeskGroupFlagDocument:	//,		// �ĵ��ļ�
		return L"�ĵ�";
		break;
	//case BEDeskGroupFlagCompress:	//,		// �ĵ���
	//	return L"�ĵ���";
	//	break;
	case BEDeskGroupFlagPicture:	//,		// ͼƬ
		return L"ͼƬ";
		break;
	case BEDeskGroupFlagOther:		//		// ����
		return L"����";
		break;
	}
	return L"";
}
// ͼ���ļ�����չ����ҪСдŶ
static const wchar_t gc_pswExtnameImage[][20] = {
	L".png",	L".bmp",	L".jpg",	L".jpeg",	L".gif",
	L".tiff",	L".pcx",	L".tga",	L".exif",	L".fpx",
	L".svg",	L".psd",	L".cdr",	L".pcd",	L".dxf",
	L".ufo",	L".eps",	L".ai",		L".raw",	L".ico"
};
static const std::unordered_set<std::wstring> gc_mapExtnameImage(std::begin(gc_pswExtnameImage), std::end(gc_pswExtnameImage));

// ѹ�����ļ���ҪСдŶ
static const wchar_t gc_pswExtnamePackeg[][20] = {
	L".exe",	L".msi",	L".zip",	L".7z",		L".lhaozip",
	L".rar",	L".iso"
};
static const std::unordered_set<std::wstring> gc_mapExtnamePackeg(std::begin(gc_pswExtnamePackeg), std::end(gc_pswExtnamePackeg));
// �ĵ��ļ���ҪСдŶ
static const wchar_t gc_pswExtnameDocument[][20] = {
	L".rtf",	L".doc",	L".docx",	L".wps",	L".xls",
	L".xlsx",	L".et",		L".txt",	L".ppt",	L".pptx",
	L".dps",	L".pdf",	L".pdg",	L".chm",	L".chw",
	L".ini",	L".xml",	L".vsd"
};
static const std::unordered_set<std::wstring> gc_mapExtnameDocument(std::begin(gc_pswExtnameDocument), std::end(gc_pswExtnameDocument));


static std::wstring lvItemState(UINT state)
{
	std::wstring ws;
	if(state & LVIS_ACTIVATING)
		ws += L"LVIS_ACTIVATING ";
	else if(state & LVIS_CUT)
		ws += L"LVIS_CUT";
	else if(state & LVIS_DROPHILITED)
		ws += L"LVIS_DROPHILITED";
	else if(state & LVIS_FOCUSED)
		ws += L"LVIS_FOCUSED";
	else if(state & LVIS_OVERLAYMASK)
		ws += L"LVIS_OVERLAYMASK";
	else if(state & LVIS_SELECTED)
		ws += L"LVIS_SELECTED";
	else if(state & LVIS_STATEIMAGEMASK)
		ws += L"LVIS_STATEIMAGEMASK";

	return ws;
}
static std::wstring toString(const NMLISTVIEW *pnmlv)
{
	std::wostringstream out;
	if (pnmlv)
	{
		out << L"NMHDR:<wndFrom:" << std::setiosflags(std::ios::uppercase|std::ios::hex) << std::setw(8) << pnmlv->hdr.hwndFrom
			<< L" idFrom:" << pnmlv->hdr.idFrom << L" code:" << (long)pnmlv->hdr.code 
			<< L" item:" << pnmlv->iItem << L" subItem:" << pnmlv->iSubItem
			<< L" changed:" << pnmlv->uChanged << L" action:<" << pnmlv->ptAction.x << L"," << pnmlv->ptAction.y << L">"
			<< L" oldState:<" << lvItemState(pnmlv->uOldState) << L">"
			<< L" newState:<" << lvItemState(pnmlv->uNewState) << L">"
			<< L" param:" << std::setiosflags(std::wios::uppercase|std::wios::hex|std::wios::showbase) << pnmlv->lParam
			<< std::endl;
	}

	return out.str();
}
//}