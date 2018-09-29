#include "dllhook.h"
#include <cstring>
#include <cwchar>

#include <TlHelp32.h>

#include "AC/sysupport.h"
namespace acm {

typedef struct BSEnumProcData {
	PCWSTR	pParentClassName;	// ����������
	PCWSTR	pClassName;			// Ҫ�Ҵ�������
	HWND	hWnd;				// ���ر��ҵ��Ĵ��ھ��

	BSEnumProcData(PCWSTR pparentClassName, PCWSTR pclassName)
		: pParentClassName(pparentClassName), pClassName(pclassName)
		, hWnd(NULL)
	{}
}BSEnumProcData;
/*****************************************************************************************
	<< --- dllhook::remoteHook		2014-05-20 --- >>
	˵��������Զ��ע�뷽ʽע�뵽������
	������
	����ֵ��
	= 0 �ɹ�
	> 0 ʧ��
*****************************************************************************************/
bool dllhook::remoteHook()
{
	if (m_vecDllFullPath.size() <= 0)
	{
		return false;
	}
	bool bret = false;
	HANDLE hProcess = nullptr;
	HANDLE hThread = nullptr;
	wchar_t *phkdll = nullptr;
	try
	{
		if (m_dwProcessID == 0)
		{
			m_dwProcessID = GetCurrentProcessId();
		}
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
			PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
			PROCESS_VM_WRITE,             // For WriteProcessMemory
			FALSE, m_dwProcessID);
		if (hProcess == nullptr) throw;
		// �õ�������ַ
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		if (pfnThreadRtn == nullptr) throw;
		// Զ�˽����ڷ���ռ�
		int dlllen = MAX_PATH;
		dlllen = dlllen * sizeof(wchar_t);
		phkdll = (wchar_t *)VirtualAllocEx(hProcess, nullptr, dlllen, MEM_COMMIT, PAGE_READWRITE);
		if (phkdll == nullptr) throw;
		// ע��
		for (std::vector<std::wstring>::const_iterator iter = m_vecDllFullPath.cbegin(); iter != m_vecDllFullPath.cend(); ++iter)
		{
			// ��dll·��������̿ռ�
			if(!WriteProcessMemory(hProcess, phkdll, iter->c_str(), (iter->length()+1)*sizeof(wchar_t), nullptr)) throw;
			// ����Զ���̣߳�ע��dll
			hThread = CreateRemoteThread(hProcess, nullptr, 0, pfnThreadRtn, phkdll, 0, nullptr);
			if (!hThread ) 
			{
				gtc::app::lastError(true);
				throw(L"ddd");
			}
			// 
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = nullptr;
		}

		bret = true;
	}
	catch(...)
	{
		bret = false;
	}

	if (!phkdll)
	{
		VirtualFreeEx(hProcess, phkdll, 0, MEM_RELEASE);
	}
	if(!hThread)
		CloseHandle(hThread);
	if(!hProcess)
		CloseHandle(hProcess);

	return bret;
}
/*****************************************************************************************
	<< --- dllhook::remoteUnhook		2014-05-20 --- >>
	˵������dll��Զ�˽�����ж��
	������
	����ֵ��
	= 0 �ɹ�
	> 0 ʧ��
*****************************************************************************************/
bool dllhook::remoteUnhook()
{
	if(m_dwProcessID == 0 && m_vecDllFullPath.size() <= 0)
		return true;
	bool bret = false;
	HANDLE hSnapshot = nullptr;
	HANDLE hProcess = nullptr, hThread = nullptr;
	try
	{
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessID);
		if(hSnapshot == INVALID_HANDLE_VALUE) throw;
		// 
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |   
			PROCESS_CREATE_THREAD     | 
			PROCESS_VM_OPERATION,  // For CreateRemoteThread
			FALSE, m_dwProcessID);
		if (hProcess == nullptr) throw;
		// Get the real address of FreeLibrary in Kernel32.dll
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
		if (pfnThreadRtn == nullptr) throw;

		// ����dllģ���Ƿ��Ѿ�����
		MODULEENTRY32W me = { sizeof(me) };
		// ע��
		for (std::vector<std::wstring>::const_reverse_iterator iter = m_vecDllFullPath.crbegin(); iter != m_vecDllFullPath.crend(); ++iter)
		{
			bool bFound = FALSE;
			bool bMoreMods = Module32FirstW(hSnapshot, &me);
			for (; bMoreMods; bMoreMods = Module32NextW(hSnapshot, &me)) {
				bFound = (_wcsicmp(me.szModule,  iter->c_str()) == 0) || 
					(_wcsicmp(me.szExePath, iter->c_str()) == 0);
				if (bFound) break;
			}
			if (!bFound) throw;
			// Create a remote thread that calls FreeLibrary()
			hThread = CreateRemoteThread(hProcess, nullptr, 0, pfnThreadRtn, me.hModule /*me.modBaseAddr*/, 0, nullptr);
			if (hThread == nullptr) throw;

			// Wait for the remote thread to terminate
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = nullptr;
			
		}
		m_vecDllFullPath.clear();
		bret = true;
	}
	catch(...)
	{
		bret = false;
	}

	if(hSnapshot)
		CloseHandle(hSnapshot);
	if(hThread)
		CloseHandle(hThread);
	if(hProcess)
		CloseHandle(hProcess);
	return bret;
}

BOOL CALLBACK dllhook::enumWindowsProc( HWND hWnd, LPARAM lParam )
{
	BSEnumProcData *pData = (BSEnumProcData *)lParam;
	if(!pData) return FALSE;
	wchar_t pclassName[MAX_PATH] = {0};
	::GetClassNameW(hWnd, pclassName, MAX_PATH);
	if (std::wcsncmp(pclassName, pData->pParentClassName, _countof(pclassName)) == 0)
	{
		if(HWND hw = ::FindWindowExW(hWnd, NULL, pData->pClassName, NULL))
		{
			pData->hWnd = hw;
			return FALSE;
		}
	}
	//gtc::app::writeVSLogs(L"���������� %s %08X", pclassName, hWnd);
	return TRUE;
}

HWND dllhook::findWindow(PCWSTR pparentClasssName, PCWSTR pclassName)
{
	assert(pparentClasssName && pclassName);
	HWND hWnd = NULL;
	BSEnumProcData *pparam = new BSEnumProcData(pparentClasssName, pclassName);
	assert(pparam);
	
	EnumWindows(dllhook::enumWindowsProc, (LPARAM)pparam);

	hWnd = pparam->hWnd;
	delete pparam;
	return hWnd;
}


}