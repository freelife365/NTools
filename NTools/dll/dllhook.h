#ifndef __DLLHOOK_H__
#define __DLLHOOK_H__
#pragma once

#include <string>
#include <vector>
#include <cwchar>
#if defined(_AFXDLL)
	#include <afxwin.h>         // MFC ��������ͱ�׼���
	#include <afxext.h>         // MFC ��չ
	#include <afxframewndex.h>
	#include <ATLComTime.h>
	#include <comutil.h>
	#include <afxdisp.h>
	#include <comdef.h>
	#include <assert.h>
#endif
#if defined(_WIN32) && !defined(_AFXDLL)
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <olectl.h>
#endif
namespace acm {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- dllhook	2014-05-20 --- >>
//	dllע����ز���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class dllhook {
private:
	dllhook(const dllhook &hook)
	{
		m_vecDllFullPath = hook.m_vecDllFullPath;
		m_dwProcessID = hook.m_dwProcessID;
	}
	dllhook &operator=(const dllhook &hook)
	{
		return *this;
	}
public:
	dllhook() : m_dwProcessID(0)
	{

	}
	dllhook(DWORD processID)
		: m_dwProcessID(processID)
	{

	}
	virtual ~dllhook()
	{
		//remoteUnhook();
	}
public:
	/*****************************************************************************************
		<< --- dllhook::remoteHook		2014-05-20 --- >>
		˵��������Զ��ע�뷽ʽע�뵽������
		������
		����ֵ��
		= 0 �ɹ�
		> 0 ʧ��
	*****************************************************************************************/
	bool remoteHook();

	/*****************************************************************************************
		<< --- dllhook::remoteUnhook		2014-05-20 --- >>
		˵������dll��Զ�˽�����ж��
		������
		����ֵ��
		= 0 �ɹ�
		> 0 ʧ��
	*****************************************************************************************/
	bool remoteUnhook();
public:
	//std::wstring	getDllFullPath() const {						return m_wsDllFullPath;									}
	//void			setDllFullPath(const wchar_t *pfullpath) {		m_wsDllFullPath = (pfullpath==nullptr?L"":pfullpath);	}
	//void			setDllFullPath(const std::wstring &fullpath) {	m_wsDllFullPath = fullpath;								}
	void			addDllFullPath(PCWSTR pdllpath) {
		assert(pdllpath != nullptr && std::wcslen(pdllpath) > 0);
		m_vecDllFullPath.push_back(pdllpath);
	}
	void			addDllFullPath(const std::wstring &dllpath) {
		assert(dllpath.length() > 0);
		m_vecDllFullPath.push_back(dllpath);
	}
	const std::vector<std::wstring>& getDllFullPath() const {		return m_vecDllFullPath;				}
	DWORD			getProcessID() const {							return m_dwProcessID;									}
	void			setProcessID(DWORD processID) {					m_dwProcessID = processID;								}
public:
	// 
	/*****************************************************************************************
		<< --- dllhook::findWindow		2014-09-02 --- >>
		˵��������ָ�����ھ��
		������
		pparentClasssName	= ���Ҵ�������������ע���������磺WorkerW��ע������ж��Ŷ����
		pclassName			= ���Ҵ���ע������
		����ֵ�����ھ�������û�ҵ�����NULL
	*****************************************************************************************/
	static HWND findWindow(PCWSTR pparentClasssName, PCWSTR pclassName);
	// ��findWindowʹ�õĻص�����
	static BOOL CALLBACK enumWindowsProc( HWND hWnd, LPARAM lParam );
private:
	// ��ע��dll��ȫ·��
	std::vector<std::wstring>	m_vecDllFullPath;
	// ��ע����̵�ID
	DWORD			m_dwProcessID;
};


}
#endif