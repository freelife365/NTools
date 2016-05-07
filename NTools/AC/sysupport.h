#ifndef __SYSUPPORT_H__
#define __SYSUPPORT_H__
#pragma once
#include <fstream>
#include <cstdarg>
#include <clocale>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <cwctype>
#include <algorithm>
#include "AC/basedef.h"		// ���� debugger.h
#include "AC/singleton.h"
#include "AC/baseException.h"
#include "AC/stringUtil.h"
#include "AC/converter.h"
#include "AC/datetime.h"
//#include "AC/debugger.h"
#if defined(BOOST_SERIALIZE)
#include "../boost/serialization/export.hpp"
#include "../boost/serialization/base_object.hpp"
#include "../boost/serialization/string.hpp"
#include "../boost/serialization/vector.hpp"
#include "../boost/serialization/map.hpp"
#endif


#if defined(_WIN32)
	#include <comutil.h>
	#include <Shellapi.h>     // Included for shell constants such as FO_* values
	#include <ShlObj.h>
	#include <stgprop.h>
	#include <Shlwapi.h>
	#pragma comment( lib, "shlwapi.lib") 
	#include <dbghelp.h>
	#pragma comment(lib, "dbghelp.lib")
#endif
#if defined(_AFXDLL )
	#include <afxMDIFrameWndEx.h>
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// 2015-12-18		kp007	������Ϣ
struct BSAdapterInfo
{
	std::wstring	wsIP;
	std::wstring	wsMAC;

	BSAdapterInfo(std::wstring ip, std::wstring mac)
		: wsIP(ip), wsMAC(mac)
	{}
};
typedef std::vector<BSAdapterInfo> AdapterInfoArrayType;

namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	format��	2014-06-27
//	��ʽ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class format
{
public:
	/*****************************************************************************************
		<< --- format::rect		2014-06-27 --- >>
		˵������RECTת���ɡ�left,top,right,bottom width,height����ʽ
	*****************************************************************************************/
	static std::wstring rect(const RECT &rc, bool showSize = false);
	static RECT rect(PCWSTR prect);
	/*****************************************************************************************
		<< --- format::point		2014-06-27 --- >>
		˵������POINTת���ɡ�x,y����ʽ
	*****************************************************************************************/
	static std::wstring point(const POINT &pt);
	static POINT point(PCWSTR ppoint);
	/*****************************************************************************************
		<< --- format::size		2014-06-27 --- >>
		˵������SIZEת���ɡ�cx,cy����ʽ
	*****************************************************************************************/
	static std::wstring size(const SIZE &sz);

	/****************************************************************************************
		<< --- 	color	2015-8-19 --- >>         
		˵������ɫת������ʽ:#HHHHHHHH
		������
		����ֵ��
	*****************************************************************************************/
	static std::wstring color(DWORD clr);
	static DWORD color(PCWSTR pclr);
	/*****************************************************************************************
		<< --  floatString		2015-1-4 --- >>
		˵������ʽ�����������
		������
			fltValue	= ����ʽ���ĸ���ֵ
			decimals	= ������С������
		����ֵ����ʽ������ַ���
	*****************************************************************************************/
	static std::wstring floatString(float fltValue, int32_t decimals = 2);

};





////////////////////////////////////////////////////////////////////////////////////////////////////////
//	app��	2014-04-25
//	����Ӧ�ò�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class app
{
public:
	static std::wstring mc_wsLogPath;	// ��־�ļ�·��
	static std::wstring mc_wsIniPath;	// �����ļ�·��
	static std::wstring mc_wsIniName;	// �����ļ�����
	static std::wstring mc_wsLogFileName;	// ��־�ļ����Ŀ�ͷ����
	static std::wstring mc_wsDumpFileName;	// dump�ļ��Ŀ�ͷ����

	static std::wstring getConfigPath() {
		return mc_wsIniPath;
	}
	static std::wstring getLogPath() {
		return mc_wsLogPath;
	}
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// 2015-12-18		kp007	�ĵ�������Ϣ
	static AdapterInfoArrayType getAdapterInfo(PCWSTR pmacSeparator = NULL);
	// ���������С��bDeskTaskbar���Ƿ��������������
	static RECT getDeskArea(bool containTaskbar = true);
	/****************************************************************************************
		<< --- addFilterWindowMessage		2015-6-9 --- >>         
		˵���������Ҫȡ�����ص���Ϣ������ͨ�������ڡ���Ȩ�޳�������Ȩ�޽��̷����Զ�����Ϣ��
		������
			hWnd	= ������Ϣ�Ĵ���
			uMsg	= �Զ�����Ϣ
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool addFilterWindowMessage(HWND hWnd, UINT uMsg);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-11-4 ���񴰿ڣ���ʹ��õ���꽹��	
	static void captureWindow(HWND hWnd);
	/****************************************************************************************
		<< --- exec		2015-6-9 --- >>         
		˵����������ִ���ļ�
		������
			pexeFile	= ��ִ���ļ�ȫ·��
			pParameter	= ���в���
			showFlag	= ��ʾ���
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool exec(PCWSTR pexeFile, PCWSTR pParameter = NULL, int showFlag = SW_SHOW);
	/****************************************************************************************
		<< --- 	getProductVersion	2015-5-14 --- >>         
		˵�����õ�����İ汾��
		������
			pFileName	= Ҫ�õ��汾��Ϣ���ļ�
			poutFileVersion	= ��Ϊ��ʱ�����ļ��汾��
		����ֵ��= 0 ʧ�ܣ����򷵻ز�Ʒ�汾��
	*****************************************************************************************/
	static uint64_t getProductVersion(PCWSTR pFileName = NULL, uint64_t *poutFileVersion = NULL);
	/****************************************************************************************
		<< --- 	expandEnvironmentString	2015-2-15 --- >>         
		˵������չ���л����������ַ������磺"%ProgramFiles%"-->"C:\Program Files"
		������
			pSrc = ����Դ
		����ֵ����չ����ַ���
	*****************************************************************************************/
	static std::wstring expandEnvironmentString(PCWSTR pSrc);
	/*****************************************************************************************
		<< --- static app::getOSVersion		2014-04-25 --- >>
		˵�����õ�����ϵͳ�İ汾��
		������
		posVersion		= ����ϵͳ�汾�ṹ
		����ֵ������ϵͳ�İ汾��
	*****************************************************************************************/
	static BEOSVersion getOSVersion(OSVERSIONINFOEX *posVersion = NULL);
	static bool isWindowsXP();
	static bool isWindowsVistaLater();
	static bool isWindows7Later();
	static bool isWindows8Later();
	static bool isWindowsOS64();
	/****************************************************************************************
		<< --- standardisePath		2015-1-15 --- >>         
		˵������·��ת���ɱ�׼��ʽ���磺c:/folder1/folder2/
		������
			initPath	= ԭ·��
		����ֵ��ת����ı�׼·��
	*****************************************************************************************/
	static std::wstring standardisePath(const std::wstring &initPath)
	{
		std::wstring path = initPath;

		std::replace(path.begin(), path.end(), L'\\', L'/');
		if(path.back() != L'/')
			path += L'/';

		return path;
	}
	/*****************************************************************************************
		<< --- static app::message		2014-04-25 --- >>
		˵�������ѡ���ļ�
		������
		vpinfo		= Ҫ��ʾ����Ϣ
		vptitle		= ��Ϣ��ı���
		vtype		= ��Ϣ�������ť����
		viswriter	= �Ƿ���Ϣд����־�ļ�
		����ֵ��ѡ�е����ť���
	*****************************************************************************************/
	static std::wstring getAppPath(void)
	{
		wchar_t str[MAX_PATH];
		std::wstring path;
//#if defined(_UNICODE)
//		path = _wpgmptr;
//#else
//		path = convert::toWChar(_pgmptr);
//#endif
		uint32_t len = 0;
		// �õ�ϵͳ����
	#if defined(_WIN32)
		len = ::GetModuleFileNameW(nullptr, str, MAX_PATH);
		str[len] = '\0';
	#endif
		path = str;
		path = path.substr(0, path.rfind('\\'));//.Left(path.ReverseFind('\\'));
		return path;
	}
#if defined(_AFXDLL)
	/*****************************************************************************************
		<< --- CAApp::getMainWnd	2014-06-14 --- >>
		˵�����õ�Ӧ�ó��������
		������
		����ֵ�������CWnd*
	*****************************************************************************************/
	static CWnd* getMainWnd()
	{
		CWinApp* app = ::AfxGetApp();
		if(app)
			return app->GetMainWnd();
		return nullptr;
	}
	static CView* getActiveView()
	{
		CWnd *pWnd = getMainWnd();
		
		if(CMDIFrameWnd *pMDIWnd = dynamic_cast<CMDIFrameWnd *>(pWnd))
		{
			if(pMDIWnd->MDIGetActive()) return pMDIWnd->MDIGetActive()->GetActiveView();
			return NULL;
		}
		else if(CFrameWnd *pframe = dynamic_cast<CFrameWnd *>(pWnd))
			return pframe->GetActiveView();
		assert(!"û�л����ͼ��");
		return NULL;
	}
#endif
	/*****************************************************************************************
		<< --- static app::init		2014-04-26 --- >>
		˵������ʼ��ϵͳ����
		1. ����Ŀ¼\\GTDATA\\LOG
		������
		isAppPath	=	true  Ӧ�ó���·��
						false ϵͳ��¼�û�Application Data��
		����ֵ��
	*****************************************************************************************/
	static void init(PCWSTR pinifile = L"kpzm.ini", PCWSTR plogfileName = L"app", PCWSTR pdumpfileName = L"du", PCWSTR pfolder = _NDEF_DIR_GTDATA, bool isAppPath = false)
	{
		std::setlocale(LC_ALL, "");

		mc_wsIniName = pinifile ? pinifile : L"";
		mc_wsLogFileName = plogfileName ? plogfileName : L"";
		mc_wsDumpFileName = pdumpfileName ? pdumpfileName : L"";
		if(isAppPath)
			mc_wsLogPath = getAppPath();
		else
		{
#if defined(_WIN32)
			wchar_t path[MAX_PATH] = {0};
			if(SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, path)))
			{
				mc_wsLogPath = path;
			}
#else
#endif
		}
#if defined(_WIN32)
		mc_wsLogPath += L"\\";
		mc_wsLogPath += pfolder;
		if(!::PathIsDirectoryW(mc_wsLogPath.c_str()) /*file::existsDirectory(mc_wsLogPath.c_str())*/)
			::CreateDirectoryW(mc_wsLogPath.c_str(), nullptr);
		mc_wsIniPath = mc_wsLogPath;	// �趨�����ļ�·��
		mc_wsLogPath += L"\\";
		mc_wsLogPath += _NDEF_DIR_GTLOG;
		//MessageBoxW(NULL, mc_wsLogPath.c_str(), L"path", 0);
		if(!::PathIsDirectoryW(mc_wsLogPath.c_str())/*file::existsDirectory(mc_wsLogPath.c_str())*/)
			::CreateDirectoryW(mc_wsLogPath.c_str(), nullptr);
#else
#endif
	}

	static void sendtoUser(const std::wstring &wsLog, PCWSTR pWindowName, PCWSTR pClassName = NULL)
	{
		static HWND hwnd = NULL;
		if(!hwnd) hwnd = FindWindowW(pClassName, pWindowName);
		if (hwnd != NULL && IsWindow(hwnd))
		{
			COPYDATASTRUCT stCopy = {0};
			// stCopy.lpData = (void *)strLog.c_str();
			stCopy.cbData = wsLog.length() * sizeof(wchar_t);
			stCopy.lpData = (void *)wsLog.c_str();

			SendMessage(hwnd, WM_COPYDATA,(WPARAM)NULL, (LPARAM)&stCopy);
			//OutputDebugString(wsLog.c_str());
		}
	}
	static void sendtoUser(BELogType logType, const std::wstring &wsLog, PCWSTR pWindowName, PCWSTR pClassName = NULL)
	{
		static HWND hwnd = NULL;
		if(!hwnd) hwnd = FindWindowW(pClassName, pWindowName);
		if (hwnd != NULL && IsWindow(hwnd))
		{
			//BSLogData logdata = {logType};
			//std::wcsncpy(logdata.pLogDesc, wsLog.c_str(), _countof(logdata.pLogDesc));
			COPYDATASTRUCT stCopy = {0};
			stCopy.dwData = (ULONG_PTR(DBGER_LOG_TYPE) << 16) | ULONG_PTR(logType);
			stCopy.cbData = (wsLog.length()+1)*sizeof(wchar_t);
			stCopy.lpData = (void *)wsLog.c_str();

			SendMessage(hwnd, WM_COPYDATA,(WPARAM)NULL, (LPARAM)&stCopy);
			//OutputDebugString(wsLog.c_str());
		}
		//else
		//{
		//	writeLogs(wsLog.c_str());
		//}
	}

	/*****************************************************************************************
		<< --- static app::message		2014-04-25 --- >>
		˵�������ѡ���ļ�
		������
		vpinfo		= Ҫ��ʾ����Ϣ
		vptitle		= ��Ϣ��ı���
		vtype		= ��Ϣ�������ť����
		viswriter	= �Ƿ���Ϣд����־�ļ�
		����ֵ��ѡ�е����ť���
	*****************************************************************************************/
	static int message(const wchar_t *vpinfo, const wchar_t *vptitle = nullptr, uint32_t vtype = 0/*MB_OK*/, bool viswriter = false) {
		int ret = -1;
	#if defined(_WIN32)
		if(vptitle != NULL)
			ret = ::MessageBoxW(NULL, vpinfo, vptitle, MB_ICONINFORMATION|vtype);
		else
			ret = ::MessageBoxW(NULL, vpinfo, _NDEF_MESSAGEBOX_TITLE, MB_ICONINFORMATION|vtype);
	#else
	#endif
		if(viswriter)
		{
			writeLogs(vpinfo, vptitle);
		}

		return ret;
	}
	static void messageEx(const wchar_t *vpformat, ...) {
		static const int len = 4096;
		static wchar_t pinfo[len] = {0};
		va_list argList;
		va_start(argList, vpformat);
		std::vswprintf(pinfo, len, vpformat, argList);
		va_end(argList);
	#if defined(_WIN32)
		message(pinfo);
	#else
	#endif
	}

	/*****************************************************************************************
		<< --- static app::writeLogs		2014-04-25 --- >>
		˵�������ѡ���ļ�
		������
		pinfo	= ��д����Ϣ
		ptitle	= ��д��Ϣ�ı������
		showmsg	= �Ƿ���ʾ��Ϣ
		����ֵ��
	*****************************************************************************************/
	static void writeLogs(const wchar_t *pinfo, const wchar_t *ptitle = NULL, bool showmsg = false)
	{
		if(pinfo == NULL || std::wcslen(pinfo) <= 0)
			return;
		const int len = 4096;
		wchar_t str[len] = {0};
		std::wstring path(mc_wsLogPath.empty()?getAppPath():mc_wsLogPath);
		char stimer[40] = {0};
		char logname[40] = {0};
		std::time_t tm = {0};
		std::time(&tm);
		std::tm *ptminfo = std::localtime(&tm);
		assert(ptminfo);
		std::strftime(stimer, sizeof(stimer), "%Y-%m-%d %H:%M:%S", ptminfo);
		std::strftime(logname, sizeof(logname), "%Y%m%d.ino", ptminfo);
		//path += L"\\";
		//path += _NDEF_DIR_GTDATA;
		//path += L"\\";
		//path += _NDEF_DIR_GTLOG;
		//path += L"\\";
		//path = mc_wsLogPath;
		path += L"\\";
		path += mc_wsLogFileName;
		path += convert::toWChar(logname);
		// д�ļ�
		if(ptitle != NULL)
			std::swprintf(str, len, L"%s\t %s\t%s  ", convert::toWChar(stimer).c_str(), ptitle, pinfo);
		else
			std::swprintf(str, len, L"%s\t %s  ", convert::toWChar(stimer).c_str(), pinfo);
		
		//file::writeLine(path.c_str(), str);
		std::wofstream file(path.c_str(), std::wofstream::app | std::wofstream::out);
		if(file)
		{
			// ֧���������
			file.imbue(std::locale("", std::locale::all ^ std::locale::numeric));
			file << str << std::endl;
			file.close();
		}
		// 
		if(showmsg)
			message(str, ptitle);

	}
	static void writeLogs(const char *pinfo, const char *ptitle = NULL, bool showmsg = false)
	{
		std::wstring wsinfo = convert::toWChar(pinfo);
		std::wstring title = convert::toWChar(ptitle);
		//std::wstring nameflag = convert::toWChar(pnameflag);
		writeLogs(wsinfo.c_str(), title.c_str(), showmsg);
	}
	static void writeVSLogs(const wchar_t *vpformat, ...) {
		const int len = 4096;
		wchar_t pinfo[len] = {0};
		va_list argList;
		va_start(argList, vpformat);
		std::vswprintf(pinfo, len, vpformat, argList);
		va_end(argList);
		// 
		writeLogs(pinfo);
	}
	/*****************************************************************************************
		<< --- static app::writeProfile 	2014-06-05 --- >>
		˵����д�����ļ���Ϣ
		������
		psection	= ������
		pkey		= ������
		pValue		= ��ֵ
		pfileName	= �����ļ�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool writeProfile(PCWSTR psection, PCWSTR pkey, PCWSTR pValue, PCWSTR pfileName = nullptr);
	static bool writeProfile(PCWSTR psection, PCWSTR pkey, int32_t iValue, PCWSTR pfileName = nullptr);
	static bool writeProfile(PCWSTR psection, PCWSTR pkey, const POINT &pt, PCWSTR pfileName = nullptr);
	static bool writeProfile(PCWSTR psection, PCWSTR pkey, const RECT &rc, PCWSTR pfileName = nullptr);
	static bool writeProfile(PCWSTR psection, PCWSTR pkey, datetime &dt, PCWSTR pfileName = nullptr);
	/*****************************************************************************************
		<< --- static app::readProfile 	2014-06-05 --- >>
		˵������ȡ�����ļ���Ϣ
		������
		psection	= ������
		pkey		= ������
		pValue		= ��ֵ
		pfileName	= �����ļ�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool readProfile(PCWSTR psection, PCWSTR pkey, std::wstring &refoutData, PCWSTR pfileName = nullptr);
	static bool readProfileLarge(PCWSTR psection, PCWSTR pkey, std::wstring &refoutData, PCWSTR pfileName = nullptr);
	static bool readProfileInt(PCWSTR psection, PCWSTR pkey, int32_t &refoutData, PCWSTR pfileName = nullptr, int32_t dftValue = 0);
	static bool readProfilePoint(PCWSTR psection, PCWSTR pkey, POINT &refoutData, PCWSTR pfileName = nullptr);
	static bool readProfileRect(PCWSTR psection, PCWSTR pkey, RECT &refoutData, PCWSTR pfileName = nullptr);
	static bool readProfileDatetime(PCWSTR psection, PCWSTR pkey, datetime &refoutData, PCWSTR pfileName = nullptr);
	/*****************************************************************************************
		<< --- static app::lastError 	2014-05-30 --- >>
		˵�����õ���������Ϣ����
		������
		isMsgbox	= �Ƿ��ѵ�����Ϣ��ʽ��ʾ������Ϣ
		isWrite		= �Ƿ񽫴�����Ϣд����־�ļ�
		����ֵ��
	*****************************************************************************************/
	static void lastError(bool isMsgbox = false, bool isWrite = true)
	{
		// ������һ��Ϊ��
		assert(isWrite || isMsgbox);
#if defined(_WIN32)
		DWORD err = ::GetLastError();

		HLOCAL hlocal = NULL;   // Buffer that gets the error message string

		// Use the default system locale since we look for Windows messages.
		// Note: this MAKELANGID combination has 0 as value
		DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

		// Get the error code's textual description
		BOOL fOk = FormatMessageW(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER, 
			NULL, err, systemLocale, 
			(PWSTR) &hlocal, 0, NULL);

		if (!fOk) {
			// Is it a network-related error?
			HMODULE hDll = LoadLibraryExW(L"netmsg.dll", NULL, 
				DONT_RESOLVE_DLL_REFERENCES);

			if (hDll != NULL) {
				fOk = FormatMessageW(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll, err, systemLocale,
					(PWSTR) &hlocal, 0, NULL);
				FreeLibrary(hDll);
			}
		}
		if (fOk && hlocal)
		{
			if(isWrite)
				writeLogs((PCWSTR)LocalLock(hlocal), L"LAST_ERROR");
			if(isMsgbox)
			{
				wchar_t buf[40];
				std::swprintf(buf, _countof(buf), L"��Ϣ��ʾ(%u)", err);
				message((PCWSTR)LocalLock(hlocal), buf);
			}
			LocalFree(hlocal);
		}
#endif
	}
#if defined(_WIN32)
	/****************************************************************************************
		<< --- sehFilterDefault		2014-12-3 --- >>         
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	static int sehFilterDefault(LPEXCEPTION_POINTERS pExcept, PCWSTR pdesc, bool createDumpFile = true);
	/****************************************************************************************
		<< --- sehUnhandleFilterDump		2014-12-9 --- >>         
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	static LONG WINAPI sehUnhandleFilterDump(LPEXCEPTION_POINTERS pExcept);
#endif
	/*****************************************************************************************
		<< --- static app::createGUID 	2014-06-09 --- >>
		˵���������ַ���GUID
		������
		����ֵ���µ�guid
	*****************************************************************************************/
	static std::wstring createGUID() {
		wchar_t buf[64] = {0};
#if defined(_WIN32)
		GUID guid = {0};
		if (SUCCEEDED(::CoCreateGuid(&guid)))
		{
			std::swprintf(buf, _countof(buf), L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
				guid.Data1,
				guid.Data2,
				guid.Data3,
				guid.Data4[0], guid.Data4[1],
				guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
		}
#endif
		return buf;
	}
	/****************************************************************************************
		<< --- 	createGlobalHandle	2015-3-6 --- >>         
		˵��������HGLOBAL
		������
			ptr		= ����Դ��ptr = NULLʱֻ����ռ�
			size	= ����Դ��С����λ���ֽ�
			nFlag	= �ڴ������
		����ֵ��
			!= NULL �ɹ�
			= NLL ʧ��
	*****************************************************************************************/
	static HGLOBAL createGlobalHandle(const void* ptr, int size, UINT nFlag = GMEM_SHARE|GMEM_MOVEABLE);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-22 ����HGLOBAL���ɹ�ʱ����hDest�����򷵻�NULL	
	static HGLOBAL copyGlobalMemory(HGLOBAL hDest, HGLOBAL hSource);

	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-20 �趨���а�����
	static void setClipboard(const std::wstring &wstr);
	static void setClipboard(const std::string &str);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	thread  ��		2015-3-18
//	�̲߳�������ط�װ
////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define THREAD_INFO_MASK_AUTOFREE		0x0001		// _BSThreadInfo::bAutofree �Ƿ���Ч
//#define THREAD_INFO_MASK_LOOPLOCK		0x0002		// _BSThreadInfo::plLooplock �Ƿ���Ч
//#define THREAD_INFO_MASK_EVENT			0x0004		// _BSThreadInfo::hEvent �Ƿ���Ч
class thread : public gtc::singleton<thread>
{
private:
	thread(const thread &);
	thread& operator=(const thread &);
public:
	thread();
	~thread();
public:
	// gtc::singleton
	static thread& getSingleton();
	static thread* getSingletonPtr();
public:
	struct _BSThreadInfo {
		HANDLE			hThread;				// �߳̾��
		void			*pParameter;			// �̲߳���
		bool			bAutofree;				// �Ƿ��Զ��ͷţ�=trueʱ���˳��߳�ʱ�ͷű��ڵ㣬ͨ��free()����
												// һ���ڲ�ִֻ�м򵥵Ĳ���ʱ�ñ����趨Ϊ��true��
		volatile LONG	*plLooplock;			// �߳��ڲ�ѭ���������� =1����ѭ�� =0�˳�ѭ��
		HANDLE			hEvent;					// ���������󴥷��¼�
		wchar_t			pDescription[MAX_PATH];	// ������Ϣ
		unsigned int (WINAPI *pthreadFunc)(_BSThreadInfo *);	// �̺߳�����ַ������������ͬһʱ��ֻ��һ���̵߳���������
	};
	typedef std::vector<_BSThreadInfo *> PtrArrayType;
	typedef unsigned int (WINAPI *PTHREAD_START_ADDRESS)(gtc::thread::_BSThreadInfo *);
public:
	/****************************************************************************************
		<< --- 	start	2015-3-18 --- >>         
		˵��������һ���߳�
		������
			pstartAddress	= �̵߳�ַ
			pParam			= �̲߳���
			onlyOne			= ͬһʱ��ͬһ�̵߳�ַ�Ƿ�ֻ������һ���߳�
			isAutofree		= �Ƿ����߳��˳�ʱ�Զ��Ƿ���������ر��߳̾��
			pLooplock		= �߳��ڲ�ѭ������
			pDesc			= ������Ϣ
		����ֵ��
			!= NULL �������߳̾��
			= NULL	����ʧ��
	*****************************************************************************************/
	static HANDLE start(PTHREAD_START_ADDRESS pstartAddress, void *pParam, bool onlyOne = false, bool isAutofree = true, volatile LONG *pLooplock = NULL, HANDLE hEvent = NULL, PCWSTR pDesc = NULL);

	/****************************************************************************************
		<< --- 	free	2015-3-18 --- >>         
		˵�����ͷ��̲߳�����Ϣ��һ�����߳��˳������һ��ִ�б�������
		������
			pThreadInfo	= �̲߳�����Ϣ
		����ֵ��
	*****************************************************************************************/
	static void free(_BSThreadInfo *pThreadInfo);

	/****************************************************************************************
		<< --- 	waitfor	2015-3-25 --- >>         
		˵�����ȴ��߳��˳�
		������
			hThread		= Ҫ�˳����߳̾��
			timeout		= �ȴ���ʱ
		����ֵ��
	*****************************************************************************************/
	static DWORD waitfor(HANDLE hThread, DWORD timeout = INFINITE);
private:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-25 ����ָ���̺߳������߳��Ƿ��Ѿ�����
	static bool exists(PTHREAD_START_ADDRESS pstartAddress);
private:
	static CRITICAL_SECTION mc_csDataLock;	// ��mc_threadData����
	static PtrArrayType mc_threadData;
};
}
#endif