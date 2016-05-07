#include "StdAfx.h"
#include "baseUI.h"

namespace wui {

/////////////////////////////////////////////////////////////////////////////////////
//
//

void WINUI_API __trace(PCWSTR pstrFormat, ...)
{
#ifdef _DEBUG
	wchar_t szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	std::vswprintf(szBuffer, _countof(szBuffer) - 2, pstrFormat, args);
	std::wcscat(szBuffer, L"\n");
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}
void WINUI_API __traceA(PCSTR pstrFormat, ...)
{
#ifdef _DEBUG
	char szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	std::vsprintf(szBuffer, pstrFormat, args);
	std::strcat(szBuffer, "\n");

	va_end(args);
	::OutputDebugStringA(szBuffer);
#endif
}
PCWSTR __traceMsg(UINT uMsg)
{
#define MSGDEF(x) if(uMsg==x) return _T(#x)
	MSGDEF(WM_SETCURSOR);
	MSGDEF(WM_NCHITTEST);
	MSGDEF(WM_NCPAINT);
	MSGDEF(WM_PAINT);
	MSGDEF(WM_ERASEBKGND);
	MSGDEF(WM_NCMOUSEMOVE);  
	MSGDEF(WM_MOUSEMOVE);
	MSGDEF(WM_MOUSELEAVE);
	MSGDEF(WM_MOUSEHOVER);   
	MSGDEF(WM_NOTIFY);
	MSGDEF(WM_COMMAND);
	MSGDEF(WM_MEASUREITEM);
	MSGDEF(WM_DRAWITEM);   
	MSGDEF(WM_LBUTTONDOWN);
	MSGDEF(WM_LBUTTONUP);
	MSGDEF(WM_LBUTTONDBLCLK);
	MSGDEF(WM_RBUTTONDOWN);
	MSGDEF(WM_RBUTTONUP);
	MSGDEF(WM_RBUTTONDBLCLK);
	MSGDEF(WM_SETFOCUS);
	MSGDEF(WM_KILLFOCUS);  
	MSGDEF(WM_MOVE);
	MSGDEF(WM_SIZE);
	MSGDEF(WM_SIZING);
	MSGDEF(WM_MOVING);
	MSGDEF(WM_GETMINMAXINFO);
	MSGDEF(WM_CAPTURECHANGED);
	MSGDEF(WM_WINDOWPOSCHANGED);
	MSGDEF(WM_WINDOWPOSCHANGING);   
	MSGDEF(WM_NCCALCSIZE);
	MSGDEF(WM_NCCREATE);
	MSGDEF(WM_NCDESTROY);
	MSGDEF(WM_TIMER);
	MSGDEF(WM_KEYDOWN);
	MSGDEF(WM_KEYUP);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SYSKEYDOWN);
	MSGDEF(WM_SYSKEYUP);
	MSGDEF(WM_SYSCOMMAND);
	MSGDEF(WM_SYSCHAR);
	MSGDEF(WM_VSCROLL);
	MSGDEF(WM_HSCROLL);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SHOWWINDOW);
	MSGDEF(WM_PARENTNOTIFY);
	MSGDEF(WM_CREATE);
	MSGDEF(WM_NCACTIVATE);
	MSGDEF(WM_ACTIVATE);
	MSGDEF(WM_ACTIVATEAPP);   
	MSGDEF(WM_CLOSE);
	MSGDEF(WM_DESTROY);
	MSGDEF(WM_GETICON);   
	MSGDEF(WM_GETTEXT);
	MSGDEF(WM_GETTEXTLENGTH);   
	static wchar_t szMsg[10];
	::wsprintf(szMsg, L"0x%04X", uMsg);
	return szMsg;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
rect& rect::operator=(const RECT &rt)
{
	left = rt.left;
	top = rt.top;
	right = rt.right;
	bottom = rt.bottom;
	return *this;
}

int rect::width() const
{
	return right - left;
}

int rect::height() const
{
	return bottom - top;
}

void rect::clear()
{
	left = top = right = bottom = 0;
}

bool rect::empty() const
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0); 
}

void rect::join(const RECT& rc)
{
	if( rc.left < left ) left = rc.left;
	if( rc.top < top ) top = rc.top;
	if( rc.right > right ) right = rc.right;
	if( rc.bottom > bottom ) bottom = rc.bottom;
}

void rect::resetOffset()
{
	::OffsetRect(this, -left, -top);
}

void rect::normalize()
{
	if( left > right ) { int iTemp = left; left = right; right = iTemp; }
	if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
}

void rect::offset(int cx, int cy)
{
	::OffsetRect(this, cx, cy);
}

void rect::inflate(int cx, int cy)
{
	::InflateRect(this, cx, cy);
}

void rect::deflate(int cx, int cy)
{
	::InflateRect(this, -cx, -cy);
}

void rect::Union(rect& rc)
{
	::UnionRect(this, this, &rc);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- stringex	2014-05-21 --- >>
//	��չstd::wstring
////////////////////////////////////////////////////////////////////////////////////////////////////////
stringex::stringex(const char *pstr)
{
	*this = toWChar(pstr);
}
stringex::stringex(const wchar_t *pstr)
	: std::wstring(pstr)
{
}
stringex::stringex(const std::wstring &ws)
	: std::wstring(ws)
{
}
stringex::stringex(const stringex &sx)
	: std::wstring(sx.data())
{
}
stringex &stringex::operator=(const stringex & sx)
{
	std::wstring::operator=(sx.data());
	//assign(sx.begin(), sx.end());
	return *this;
}
stringex &stringex::operator=(PCWSTR pstr)
{
	std::wstring::operator=(pstr);
	return *this;
}
stringex stringex::RES(UINT nRes)
{
	wchar_t szBuffer[256];
	int cchLen = ::LoadString(paintManagerUI::getResourceDll(), nRes, szBuffer, _countof(szBuffer) - 1);
	assert(cchLen>0);
	szBuffer[cchLen] = '\0';
	return szBuffer;
}
/*****************************************************************************************
	<< --- static convert::toWChar	2014-04-25 --- >>
	˵���������ַ�ת���ɿ��ַ���(UTF-8δ����)
	������
	pdata		= ��ת�����ַ�
	isAnsi	= true:��ת�����ַ�ΪANSI�룻false:����ΪUTF-8
	����ֵ��ת������ַ���
*****************************************************************************************/
std::wstring stringex::toWChar(const char *pdata, bool isAnsi)
{
	if(pdata == nullptr || std::strlen(pdata) <= 0)
		return L"";
	std::wstring wstr;
	wchar_t *buf = nullptr;

	int len = ::MultiByteToWideChar(isAnsi? CP_ACP: CP_UTF8, 0, pdata, -1, NULL, 0);
	if(len > 0)
	{
		buf = new wchar_t[len+1];
		memset(buf, 0, (len+1)*sizeof(wchar_t));
		len = ::MultiByteToWideChar(isAnsi? CP_ACP: CP_UTF8, 0, pdata, -1, buf, len);
	}

	if(buf)
	{
		wstr = buf;
		delete buf;
	}
	return wstr;
}
/*****************************************************************************************
	<< --- static convert::toWChar	2014-04-25 --- >>
	˵���������ַ�ת���ɵ��ֽ��ַ���(UTF-8δ����)
	������
	pdata	= ��ת�����ַ�
	toAnsi	= true:�ַ���ת��ΪANSI�룻false:����ΪUTF-8
	����ֵ��ת������ַ���
*****************************************************************************************/
std::string stringex::toChar(const wchar_t *pdata, bool toAnsi)
{
	assert(pdata != nullptr && std::wcslen(pdata) > 0);
	std::string cstr;
	char *buf = nullptr;
	int len = ::WideCharToMultiByte(toAnsi? CP_ACP: CP_UTF8, 0, pdata, -1, NULL,0, NULL, NULL);
	if(len > 0)
	{
		buf = new char[len+1];
		memset(buf, 0, (len+1)*sizeof(char));
		len = ::WideCharToMultiByte(toAnsi? CP_ACP: CP_UTF8, 0, pdata, -1, buf, len, NULL, NULL);
	}

	if(buf)
	{
		cstr = buf;
		delete buf;
	}
	return cstr;
}
int stringex::compareNoCase(const wchar_t *pstr) const
{
	return _tcsicmp(data(), pstr); 
}

void stringex::makeUpper()
{
	std::string cs = toChar(data());
	for (std::string::iterator iter = cs.begin(); iter != cs.end(); ++iter)
	{
		if(!(*iter & 0x80) && std::isalnum(*iter) && !std::isupper(*iter))
		{
			*iter = std::toupper(*iter);
		}
	}

	assign(toWChar(cs.c_str()));

}
void stringex::makeLower()
{
	std::string cs = toChar(data());
	for (std::string::iterator iter = cs.begin(); iter != cs.end(); ++iter)
	{
		if(!(*iter & 0x80) && std::isalnum(*iter) && std::isupper(*iter))
		{
			*iter = std::tolower(*iter);
		}
	}

	assign(toWChar(cs.c_str()));
}

stringex stringex::left(int nLength) const
{
	if( nLength < 0 ) nLength = 0;
	if( nLength > length() ) nLength = length();
	return std::wstring(data(), nLength);
}
stringex stringex::mid(int iPos, int nLength) const
{
	if( nLength < 0 ) nLength = length() - iPos;
	if( iPos + nLength > length() ) nLength = length() - iPos;
	if( nLength <= 0 ) return std::wstring();
	return substr(iPos, nLength);// std::wstring(data() + iPos, nLength);
}
stringex stringex::right(int nLength) const
{
	int iPos = length() - nLength;
	if( iPos < 0 ) {
		iPos = 0;
		nLength = length();
	}
	return substr(iPos, nLength);//std::wstring(data() + iPos, nLength);

}
int stringex::replace(const wchar_t *pstrFrom, const wchar_t *pstrTo)
{
	stringex sTemp;
	int nCount = 0;
	int iPos = find(pstrFrom);
	if( iPos < 0 ) return 0;
	int cchFrom = (int) std::wcslen(pstrFrom);
	int cchTo = (int) std::wcslen(pstrTo);
	while( iPos >= 0 ) {
		sTemp = left(iPos);
		sTemp += pstrTo;
		sTemp += mid(iPos + cchFrom);
		assign(sTemp);
		iPos = find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}
void stringex::processResourceTokens()
{
	int iPos = find(L'%');
	while( iPos >= 0 ) {
		if( at(iPos + 1) == L'{' ) {
			int iEndPos = iPos + 2;
			while( std::isdigit(at(iEndPos)) ) iEndPos++;
			if( at(iEndPos) == L'}' ) {
				stringex sTemp = stringex::RES((UINT)_ttoi(data() + iPos + 2));
				replace(mid(iPos, iEndPos - iPos + 1).c_str(), sTemp.c_str());
			}
		}
		iPos = find(L'%', iPos + 1);
	}
}

int stringex::format(PCWSTR pstrFormat, ...)
{
	//CStdString sFormat = pstrFormat;
	// Do ordinary printf replacements
	// NOTE: Documented max-length of wvsprintf() is 1024
	wchar_t szBuffer[1025] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	//int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	int iRet = std::vswprintf(szBuffer, _countof(szBuffer), pstrFormat, argList);
	va_end(argList);
	assign(szBuffer);
	return iRet;
}
int stringex::smallFormat(PCWSTR pstrFormat, ...)
{
	//CStdString sFormat = pstrFormat;
	TCHAR szBuffer[64] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	//int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	int iRet = std::vswprintf(szBuffer, _countof(szBuffer), pstrFormat, argList);
	va_end(argList);
	assign(szBuffer);
	return iRet;
}



/////////////////////////////////////////////////////////////////////////////////////
//
//

windowWnd::windowWnd() : m_hWnd(NULL), m_OldWndProc(::DefWindowProc), m_bSubclassed(false)
{
	////TRACE_CLASS_BEGIN(this)
}

HWND windowWnd::getHWND() const 
{ 
	return m_hWnd; 
}

UINT windowWnd::getClassStyle() const
{
	return 0;
}

PCWSTR windowWnd::getSuperClassName() const
{
	return NULL;
}

windowWnd::operator HWND() const
{
	return m_hWnd;
}

HWND windowWnd::create(HWND hwndParent, PCWSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
	return create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND windowWnd::create(HWND hwndParent, PCWSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	if( getSuperClassName() != NULL && !registerSuperclass() ) return NULL;
	if( getSuperClassName() == NULL && !registerWindowClass() ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, getWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, paintManagerUI::getInstance(), this);
	assert(m_hWnd!=NULL);
	return m_hWnd;
}

HWND windowWnd::subclass(HWND hWnd)
{
	assert(::IsWindow(hWnd));
	assert(m_hWnd==NULL);
	m_OldWndProc = SubclassWindow(hWnd, __wndProc);
	if( m_OldWndProc == NULL ) return NULL;
	m_bSubclassed = true;
	m_hWnd = hWnd;
	return m_hWnd;
}

void windowWnd::unsubclass()
{
	assert(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	if( !m_bSubclassed ) return;
	SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = false;
}

void windowWnd::showWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	assert(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

bool windowWnd::showModal()
{
	assert(::IsWindow(m_hWnd));
	HWND hWndParent = GetWindowOwner(m_hWnd);
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::EnableWindow(hWndParent, FALSE);
	MSG msg = { 0 };
	while( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) ) {
		if( msg.message == WM_CLOSE && msg.hwnd == m_hWnd ) {
			::EnableWindow(hWndParent, TRUE);
			::SetFocus(hWndParent);
		}
		if( !paintManagerUI::translateMessage(&msg) ) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if( msg.message == WM_QUIT ) break;
	}
	::EnableWindow(hWndParent, TRUE);
	::SetFocus(hWndParent);
	if( msg.message == WM_QUIT ) ::PostQuitMessage(msg.wParam);
	return true;
}

void windowWnd::close()
{
	assert(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	postMessage(WM_CLOSE);
}

void windowWnd::centerWindow()
{
	assert(::IsWindow(m_hWnd));
	assert((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	if( hWndCenter == NULL ) rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left ) xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void windowWnd::setIcon(UINT nRes)
{
	HICON hIcon = (HICON)::LoadImage(paintManagerUI::getInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	assert(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
	hIcon = (HICON)::LoadImage(paintManagerUI::getInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	assert(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

bool windowWnd::registerWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = getClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = windowWnd::__wndProc;
	wc.hInstance = paintManagerUI::getInstance();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = getWindowClassName();
	ATOM ret = ::RegisterClass(&wc);
	assert(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool windowWnd::registerSuperclass()
{
	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, getSuperClassName(), &wc) ) {
		if( !::GetClassInfoEx(paintManagerUI::getInstance(), getSuperClassName(), &wc) ) {
			assert(!"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = windowWnd::__controlProc;
	wc.hInstance = paintManagerUI::getInstance();
	wc.lpszClassName = getWindowClassName();
	ATOM ret = ::RegisterClassEx(&wc);
	assert(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK windowWnd::__wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	windowWnd* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<windowWnd*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else {
		pThis = reinterpret_cast<windowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			if( pThis->m_bSubclassed ) pThis->unsubclass();
			pThis->m_hWnd = NULL;
			pThis->onFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		return pThis->handleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK windowWnd::__controlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	windowWnd* pThis = NULL;
	if( uMsg == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<windowWnd*>(lpcs->lpCreateParams);
		::SetProp(hWnd, L"WndX", (HANDLE) pThis);
		pThis->m_hWnd = hWnd;
	} 
	else {
		pThis = reinterpret_cast<windowWnd*>(::GetProp(hWnd, L"WndX"));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			if( pThis->m_bSubclassed ) pThis->unsubclass();
			::SetProp(hWnd, L"WndX", NULL);
			pThis->m_hWnd = NULL;
			pThis->onFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) {
		return pThis->handleMessage(uMsg, wParam, lParam);
	} 
	else {
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT windowWnd::sendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	assert(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 

LRESULT windowWnd::postMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	assert(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

void windowWnd::resizeClient(int cx /*= -1*/, int cy /*= -1*/)
{
	assert(::IsWindow(m_hWnd));
	RECT rc = { 0 };
	if( !::GetClientRect(m_hWnd, &rc) ) return;
	if( cx != -1 ) rc.right = cx;
	if( cy != -1 ) rc.bottom = cy;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) return;
	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
}

LRESULT windowWnd::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
}

void windowWnd::onFinalMessage(HWND /*hWnd*/)
{
}
}