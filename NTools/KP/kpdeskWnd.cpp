#include "StdAfx.h"
#include "kpdeskWnd.h"
#include "KP/kpdeskListView.h"
//#include "KP/KPDeskInfo.h"



//HMODULE		kpdeskWnd::mc_hModule = nullptr;		// ��dll��HMODULE
//HWND		kpdeskWnd::mc_hProgman = nullptr;		// Program Manager���ھ��
//HWND		kpdeskWnd::mc_hShelldll = nullptr;		// ShellDLL_DefView���ھ��
//HWND		kpdeskWnd::mc_hDeskListView = nullptr;	// ����ؼ�SysListVew32���



kpdeskWnd::kpdeskWnd(void)
{
}


kpdeskWnd::~kpdeskWnd(void)
{
	wui::TRACE_CLASS_BEGIN(this)
}

void kpdeskWnd::onFinalMessage(HWND hWnd)
{
	//m_paintManager.removePreMessageFilter(this);
	m_paintManager.removeNotifier(this);
	m_paintManager.reapObjects(m_paintManager.getRoot());

}
void kpdeskWnd::notify(wui::TNotifyUI& msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender->getName() == kpButtonLock)
		{
			MessageBox(NULL, L"lock", L"hello", 0);
		}
		else if (msg.pSender->getName() == kpButtonMenu)
		{
			MessageBox(NULL, L"menu", L"hello", 0);
		}
		else if(msg.pSender->getName() == kpTextTitle/*kpButtonTitle signaturetip*/)
		{
			msg.pSender->setVisible(false);
			wui::richEditUI* redit = static_cast<wui::richEditUI*>(m_paintManager.findControl(L"reditTitle"/*signature*/)); 
			if (redit != NULL)
			{
				wui::rect rt = msg.pSender->getPos();
				rt.deflate(rt.width()/4, 0);
				rt.offset(0, 2);
				redit->setPos(rt);
				redit->setFixedWidth(rt.width());
				redit->setText(msg.pSender->getText().c_str());
				redit->setVisible(true);
				redit->setFocus();
				redit->setSelAll();
			}

		}
	}
	if (msg.sType == L"killfocus")
	{
		if (msg.pSender->getName() == kpREditTitle)
		{
			msg.pSender->setVisible(false);
			wui::controlUI* txtTitle = m_paintManager.findControl(kpTextTitle/*kpButtonTitle*/);
			if (txtTitle != NULL)
			{
				wui::richEditUI* reditTitle = static_cast<wui::richEditUI*>(msg.pSender);
				if (reditTitle != NULL)
					txtTitle->setText(reditTitle->getText().c_str());
				txtTitle->setVisible(true);
			}
		}
	}
}
LRESULT kpdeskWnd::onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = m_paintManager.getSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_paintManager.getCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			wui::controlUI* pControl = static_cast<wui::controlUI*>(m_paintManager.findControl(pt));
			if( pControl && _tcsicmp(pControl->getClass(), L"ButtonUI") != 0 && 
				_tcsicmp(pControl->getClass(), L"OptionUI") != 0 /*&&
				_tcsicmp(pControl->GetClass(), L"TextUI")) != 0 */)
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT kpdeskWnd::onGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	wui::rect rcWork = oMonitor.rcWork;
	rcWork.offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return S_OK;

}

LRESULT kpdeskWnd::onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_paintManager.getRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		wui::rect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return S_OK;

}

LRESULT kpdeskWnd::onSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE){
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = wui::windowWnd::handleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ){
	}
	return lRes;

}

LRESULT kpdeskWnd::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = S_OK;
	BOOL bHandled = TRUE;
	switch(uMsg)
	{
	case WM_CREATE:
		{
			m_paintManager.init(m_hWnd);//);	kpdeskListView::mc_hShelldll// ׼����ͼ����

			// 
			wui::dialogBuilder builder;			//duilib.xml
			wui::controlUI* pRoot = builder.create(L"kpsk\\deskhost.xml", (UINT)0, NULL, &m_paintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
			assert(pRoot && L"Failed to parse XML");

			m_paintManager.attachDialog(pRoot);
			m_paintManager.addNotifier(this);   // ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����
			// DAI
			m_bFirstExec = gtc::app::readProfileInt(L"MAIN", L"IsFirst") == 0;
			// ����ͼ������鴰��
			if(m_bFirstExec)
				createGroupWindow();
			else
				loadGroupWindow();

			return lRes;
		}
		break;
	//case WM_KPDESK_CLOSE:
	//	{
	//		kpdeskListView::stopSubClass();
	//		sendMessage(WM_CLOSE);
	//	}
	//	break;
	case WM_DESTROY:
		{
			//// ��ɷ��鴰�ڶ�д����ʱ������ // DAI
			//if(m_bFirstExec)
			//	gtc::app::writeProfile(L"MAIN", L"IsFirst", 1);
			// �ָ�����ͼ��λ��
			//kpdeskListView::restoreItemPosition();

			//closeGroupWindow();
			// 
			::PostQuitMessage(0L);
		}
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_NCACTIVATE: // ����3����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT��������ϵͳ������
		{
			if( !::IsIconic(m_hWnd) ) 
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		break;
	case WM_NCCALCSIZE:
		return 0;
		break;
	case WM_NCPAINT:
		return 0;
		break;
	case WM_NCHITTEST:
		lRes = onNcHitTest(uMsg, wParam, lParam, bHandled);
		//MessageBox(NULL, L"WM_NCHITTEST", L"msg", 0);
		break;
	case WM_GETMINMAXINFO:
		lRes = onGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SIZE:
		lRes = onSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SYSCOMMAND:
		lRes = onSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	case WM_USER + 100:
		{
			//kpdeskListView::stopSubClass();
			////sendMessage(WM_CLOSE);
			//break;
			wchar_t pname[MAX_PATH] = {0};
			//wchar_t plog[256] = {0};
			POINT pt = {0};
			int nMaxItems = ListView_GetItemCount(kpdeskListView::mc_hDeskListView);


			for (int idx = 0; idx < nMaxItems; ++idx)
			{
				ListView_GetItemText(kpdeskListView::mc_hDeskListView, idx, 0, pname, _countof(pname));
				// ����ԭ����
				ListView_GetItemPosition(kpdeskListView::mc_hDeskListView, idx, &pt);
				SEND_LOG(L"user msg %d %s.......", idx, pname);
			}

		}
		break;
	default:
		bHandled = FALSE;
		break;
	}

	if (bHandled) return lRes;

	//lRes = handleCustomMessage(uMsg, wParam, lParam, bHandled);
	//if (bHandled) return lRes;

	if (m_paintManager.messageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return windowWnd::handleMessage(uMsg, wParam, lParam);


}

void kpdeskWnd::createGroupWindow()
{
	// ϵͳ��Ӧ�ó����ļ��У��ĵ��ļ����ĵ�����ͼƬ������
	std::map<BEDeskGroupFlag, kpdeskGroupWnd *> mapGroup;
	SEND_LOG(L"׼�������鴰��...");
	int32_t idx = 0;
	kpdeskGroupWnd* pgroup = new kpdeskGroupWnd();
	if( pgroup == NULL ) return ;
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"ϵͳ");
	pgroup->setWindowPos(idx++);
	
	mapGroup[BEDeskGroupFlagSystem] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"Ӧ�ó���");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagSoft] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"�ļ���");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagFolder] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"�ĵ��ļ�");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagDocument] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"�ļ���");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagCompress] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"ͼƬ");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagPicture] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	pgroup = new kpdeskGroupWnd();
	pgroup->create(kpdeskListView::mc_pwndHost->getHWND(), L"KPGroup", pgroup->getWindowStyle(), pgroup->getWindowExStyle());
	pgroup->setTitle(L"�����ļ�");
	pgroup->setWindowPos(idx++);
	mapGroup[BEDeskGroupFlagOther] = pgroup;
	m_vecGroupWnd.push_back(pgroup);

	// ��������ͼ��
	classifyNodes(mapGroup);
	
	// ��ʾ�鴰��
	for (std::vector<kpdeskGroupWnd *>::iterator iter = m_vecGroupWnd.begin(); iter != m_vecGroupWnd.end(); ++iter)
	{
		//::SetParent((*iter)->getHWND(), kpdeskListView::mc_hDeskListView); // ������Ϊ����
		//gtc::idlNode::IDINodeMap &mapNodes = (*iter)->getNodes();
		//for (gtc::idlNode::IDINodeMap::const_iterator it = mapNodes.cbegin(); it != mapNodes.cend(); ++it)
		//{
		//	gtc::app::writeLogs(it->second->toString().c_str());
		//}
		(*iter)->allocNodes();
		(*iter)->showWindow(true);
	}
	
}

void kpdeskWnd::loadGroupWindow()
{

}

void kpdeskWnd::closeGroupWindow()
{
	DWORD processID = 0;
	try
	{
		for (std::vector<kpdeskGroupWnd *>::const_iterator iter = m_vecGroupWnd.cbegin(); iter != m_vecGroupWnd.cend(); ++iter)
		{
			GetWindowThreadProcessId((*iter)->getHWND(), &processID);
			(*iter)->sendMessage(WM_CLOSE);

			HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
			WaitForSingleObject(hProcess, INFINITE);
		}
	}
	catch(...)
	{
		gtc::app::lastError(true);
	}
}

bool kpdeskWnd::classifyNodes(std::map<BEDeskGroupFlag, kpdeskGroupWnd *> &mapGroup)
{
	//gtc::idlNode::IDINodeArray &nodes = kpdeskListView::getNodes();
	//if (nodes.size() > 0)
	//{
	//	for(std::vector<gtc::idlNode *>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter)
	//	{
	//		mapGroup[(*iter)->getGroupType()]->getNodes()[(*iter)->getName()] = *iter;
	//	}
	//	return true;
	//}
	return false;

	
}

kpdeskGroupWnd* kpdeskWnd::findGroupWnd(const std::wstring &nodeName)
{
	if (m_mapDeskNodes.find(nodeName) != m_mapDeskNodes.end())
	{
		return nullptr;
	}
	for (std::vector<kpdeskGroupWnd *>::iterator iter = m_vecGroupWnd.begin(); iter != m_vecGroupWnd.end(); ++iter)
	{
		gtc::idlNode::IDINodeMap &mapNodes = (*iter)->getNodes();
		if (mapNodes.find(nodeName) != mapNodes.end())
		{
			return *iter;
		}
	}
	return nullptr;
}

RECT kpdeskWnd::getNodeContainScreenPOS(const std::wstring &nodeName)
{
	RECT rt = {0};
	if (m_mapDeskNodes.find(nodeName) != m_mapDeskNodes.end())
	{
		GetWindowRect(getHWND(), &rt);
		return rt;
	}
	for (std::vector<kpdeskGroupWnd *>::iterator iter = m_vecGroupWnd.begin(); iter != m_vecGroupWnd.end(); ++iter)
	{
		gtc::idlNode::IDINodeMap &mapNodes = (*iter)->getNodes();
		if (mapNodes.find(nodeName) != mapNodes.end())
		{
			return (*iter)->getContainScreenPOS();
		}
	}
	return rt;
}

unsigned int WINAPI kpdeskWnd::initHost(void *pparam)
{
	//kpdeskListView::mc_hModule = (HMODULE)pparam;
	//InitCommonControls();
	//OleInitialize(nullptr);

	//while(!kpdeskListView::findDeskHandle())
	//{
	//	Sleep(100);
	//}

	//gtc::app::init();
	//// GDI+��ʼ��
	//ULONG_PTR gdiToken = 0;
	//Gdiplus::GdiplusStartupInput gdiInput;
	//Gdiplus::GdiplusStartup(&gdiToken, &gdiInput, nullptr);
	//// ����Ĭ�ϻ���
	//kpdeskListView::saveDefaultAttributes();
	//// ö������ڵ㣬�����ڵ�ϵͳͼ��ʹ���
	//kpdeskListView::enumNodes();
	//// 
	////assert(mc_hModule && mc_hDeskListView);
	////DWORD processID;
	//wui::paintManagerUI::setInstance(kpdeskListView::mc_hModule);
	//wui::paintManagerUI::setResourcePath(paintManagerUI::getInstancePath().c_str());
	//

	//SEND_LOG(L"���ڴ���������...%08X %08X %08X %08X", GetDC(kpdeskListView::mc_hProgman), GetDC(kpdeskListView::mc_hShelldll), 
	//	GetDC(kpdeskListView::mc_hDeskListView), GetDC(nullptr));
	//kpdeskListView::mc_pwndHost = new kpdeskWnd();
	//if( kpdeskListView::mc_pwndHost == NULL ) return 0;

	//kpdeskListView::mc_pwndHost->create(kpdeskListView::mc_hShelldll, L"KPWork", kpdeskListView::mc_pwndHost->getWindowStyle(), kpdeskListView::mc_pwndHost->getWindowExStyle());
	//kpdeskListView::mc_pwndHost->centerWindow();
	//kpdeskListView::mc_pwndHost->showWindow(true);
	//// ���໯
	//if(!kpdeskListView::startSubClass())
	//	SEND_LOG(L"���໯ʧ�ܣ�");
	//// ˢ������
	//kpdeskListView::redrawDeskWindow();

	//SEND_LOG(L"������OK");

	//// ��ѭ��
	//wui::paintManagerUI::messageLoop();

	//// �˳����໯
	//kpdeskListView::stopSubClass();

	//// 
	//kpdeskListView::restoreDefaultAttributes();

	//SEND_LOG(L"�˳�������OK");
	//for (std::vector<kpdeskGroupWnd *>::const_iterator iter = kpdeskListView::mc_pwndHost->getGroupWnd().cbegin(); iter != kpdeskListView::mc_pwndHost->getGroupWnd().cend(); ++iter)
	//{
	//	delete *iter;
	//}
	//delete kpdeskListView::mc_pwndHost;
	//// ���»�������
	//kpdeskListView::refreshWindow();
	//// �˳�GDI+
	//Gdiplus::GdiplusShutdown(gdiToken);
	//// 
	//OleUninitialize();

	return 0;
}


unsigned int WINAPI kpdeskWnd::stopHost(void *pparam)
{
	return 0;
}

