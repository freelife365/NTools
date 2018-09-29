// dbger.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "dbger.h"

int g_ifontMSYH11 = 0;				// ΢���ź�11
int g_ifontMSYH12 = 0;				// ΢���ź�12
int g_ifontMSYH14 = 0;				// ΢���ź�14
int g_ifontMSYH15 = 0;				// ΢���ź�15
int g_ifontMSYH16 = 0;				// ΢���ź�16
int g_ifontMSYH18 = 0;				// ΢���ź�18
int g_ifontMSYH22 = 0;				// ΢���ź�22
int g_ifontMSYH24 = 0;				// ΢���ź�24
int g_ifontMSYH25 = 0;				// ΢���ź�25
int g_ifontMSYH38 = 0;				// ΢���ź�38

dialogMain*	_pdialogMainPtr = dialogMain::getSingletonPtr();

int Run(int nCmdShow = SW_SHOWDEFAULT)
{
	if (_pdialogMainPtr == NULL)
	{
		_pdialogMainPtr = new dialogMain();
		if(!_pdialogMainPtr->create())
		{
			delete _pdialogMainPtr;
			_pdialogMainPtr = NULL;
			return - 1;
		}
	}
	assert(_pdialogMainPtr);

	_pdialogMainPtr->createTray();

	_pdialogMainPtr->showWindow();

	wuc::drawManager::messageLoop();
	_pdialogMainPtr->destroyTray();

	delete _pdialogMainPtr;
	return 0;
}

bool loadResource()
{
	int fails = 0;
	gtc::resourceManager &resManager = wuc::drawManager::getResource();
	// ����ͼƬ
	if(!resManager.addImage(L"main_bk", IDB_PNG_MAIN_BK, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<main_bk>��");
	}	
	if(!resManager.addImage(L"main_icon", IDB_PNG_MAIN_ICON, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<main_icon>��");
	}	
	if(!resManager.addImage(L"button_minbox", IDB_PNG_BUTTON_MINBOX, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_minbox>��");
	}	
	if(!resManager.addImage(L"button_close", IDB_PNG_BUTTON_CLOSE, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<button_close>��");
	}	

	// �˵�
	if(!resManager.addImage(L"menu_bk", IDB_PNG_MENU_BK, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<menu_bk>��");
	}
	if(!resManager.addImage(L"menu_item_hot", IDB_PNG_MENU_ITEM_HOT, L"PNG"))
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ��PNG:<menu_item_hot>��");
	}
	// ����
	if ((g_ifontMSYH11 = resManager.addFont(L"΢���ź�", 11/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�11>��");
	}
	if ((g_ifontMSYH12 = resManager.addFont(L"΢���ź�", 12/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�12>��");
	}
	if ((g_ifontMSYH14 = resManager.addFont(L"΢���ź�", 14/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�14>��");
	}
	if ((g_ifontMSYH15 = resManager.addFont(L"΢���ź�", 15/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�15>��");
	}
	if ((g_ifontMSYH16 = resManager.addFont(L"΢���ź�", 16/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�16>��");
	}
	if ((g_ifontMSYH18 = resManager.addFont(L"΢���ź�", 18/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�18>��");
	}
	if ((g_ifontMSYH24 = resManager.addFont(L"΢���ź�", 24/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�24>��");
	}
	if ((g_ifontMSYH22 = resManager.addFont(L"΢���ź�", 22/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�22>��");
	}
	if ((g_ifontMSYH25 = resManager.addFont(L"΢���ź�", 25/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�25>��");
	}
	if ((g_ifontMSYH38 = resManager.addFont(L"΢���ź�", 38/*, true*/)) < 0)
	{
		++fails;
		SEND_LOG(L"loadResource ������Դʧ�� ����:<΢���ź�38>��");
	}
	// �����ؼ�����
	std::wstring wsAttribute;
	wchar_t pbuf[MAX_PATH] = {0};

	//		�˵�����
	wsAttribute = L"image=\"name='menu_bk' corner='2,2,2,2'\" defaultitemheight=\"35\" separatorcolor=\"#FFF7F7F7\"" \
		L" bkimage=\"0,0,35,35\"" \
		L" itemimage=\"menu_item_hot\" itemhotimage=\"0,0,34,34\"";
	std::swprintf(pbuf, _countof(pbuf), L" itemfont=\"%d\" itemtextcolor=\"#FF5E5E5E\" itemhottextcolor=\"#FF5E5E5E\"", g_ifontMSYH11);
	wsAttribute += pbuf;
	resManager.addAttributeList(L"Menu", wsAttribute);
	return fails == 0;
}
void freeResource()
{
	__try
	{
		wuc::drawManager::getResource().free();
	}
	__except(gtc::app::sehUnhandleFilterDump(GetExceptionInformation()))
	{
		//bSEH = true;
	}
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hRes = S_FALSE;
	//hRes = ::CoInitialize(NULL);
	hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hRes));
	hRes = ::OleInitialize(NULL);
	assert(SUCCEEDED(hRes));
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"DBGER_MUTEX_ONLY");
	if(hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		SEND_LOG(L"KPSglw ��������ͻ����Ѿ������У�%08X", hMutex);
		::OleUninitialize();
		::CoUninitialize();
		return 0;
	}
	::SetUnhandledExceptionFilter(gtc::app::sehUnhandleFilterDump);
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	//::DefWindowProc(NULL, 0, 0, 0L);

	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES | ICC_TAB_CLASSES|ICC_WIN95_CLASSES };
	BOOL bRet = ::InitCommonControlsEx(&iccx);

	// 
	gtc::app::init(L"dbger.ini", L"dbg_app", L"dbg_dmp", L"dbger");
	wuc::drawManager::setInstance(hInstance);
	// GDI+��ʼ��
	if (!gtc::resourceManager::initEnvironment((HMODULE)hInstance))
	{
		//SEND_LOG(L"ERROR KPSglw GDI+ ��ʼ��ʧ�ܣ�");
		return 0;
	}
	// 
	if (!loadResource())
	{
		//SEND_LOG(L"ERROR KPSglw ������Դʧ�ܣ�");
		return 0;
	}
	wuc::wkeWebkitUC::wkeWebkit_Init();
	int nRet = Run(nCmdShow);

	wuc::wkeWebkitUC::wkeWebkit_Shutdown();
	// �ͷ�ͼƬ����Դ
	freeResource();
	// �˳�GDI+
	gtc::resourceManager::releaseEnvironment();

	::OleUninitialize();
	::CoUninitialize();
	//SEND_LOG(L"WARNING KPSglw.exe �˳�����������������������������");
	return nRet;
}

