#ifndef __KPBASEDEF_H__
#define __KPBASEDEF_H__
#pragma once

/****************************************************************************************
	��"�������湤����"�������Ϣ�����ļ�
	��[deskset]��
		HideIcon��int �Ƿ�֧��˫���������湦�ܣ�=1֧�֣�����֧��
	��[SHARE]��
		ShowDesk		��int �����Ƿ�����ʾ״̬��=1��ʾ
		KPDESK			��int ������������Ƿ��������У�=1������
		rundesktop		��int �Ƿ������������������=1����
		againkpdttool	��int KPDTTool.exe�˳����Ƿ��ٴ�����
*****************************************************************************************/
//#define KPDESK_SHARE_INI	L"kpzm.ini"
#define KPDESK_SHARE_INI	L"jzzm.ini"

// ���������ע����Ϣ
// KPDesktop.exe
static UINT gc_uiKPDesktopExit			= RegisterWindowMessageW(L"KPDesktopExit");			// ������������������յ��������˳���Ϣ���磺KPDTTool�����������ġ��ر���������
static UINT gc_uiKPDesktopRun			= RegisterWindowMessageW(L"KPDesktopRun");			// �����������������LPARAM=1�����˳���LPARAM=0��ʱ���͵���Ϣ�磺����������������KPDTTool.exe���͵���Ϣ
// KPDesktop.exe
static UINT gc_uiKPDTToolRunAgain		= RegisterWindowMessageW(L"KPDTToolRunAgain");		// ���յ���������
static UINT gc_uiKPDTToolExit			= RegisterWindowMessageW(L"KPDTToolExit");			// ���յ��˳�����
// ͬ������
// KPSglw.exe
#define KPGSLW_MUTEX_ONLY				L"KPSGLWMutexRunning"			// ��ֻ֤��һ��������
// KPDesktop.exe
#define KPDESKTOP_MUTEX_ONLY			L"KPDesktopMutexRunning"		// ��ֻ֤��һ��������
// KPDTTool.exe
#define KPDTTOOL_MUTEX_ONLY				L"KPDTToolMutexRunning"			// ��ֻ֤��һ��������
// favoriter.exe
#define KPFAVORITER_MUTEX_ONLY			L"KPFavoriterMutexRunning"		// ��ֻ֤��һ��������
// KPDown.exe
#define KPDOWN_MUTEX_ONLY				L"KPDownMutexRunning"			// ��ֻ֤��һ��������

// ������
// KPSglw.exe
#define	KPSGLW_WND_MAIN_CAPTION			L"KPSglwCaption"				// ������
#define	KPSGLW_WND_MAIN_CLASS			L"KPSglwClass"
#define	KPSGLW_WND_BKGD_CAPTION			L"KPSglwBackgroundCaption"		// �����ڵĸ�����
#define	KPSGLW_WND_BKGD_CLASS			L"KPSglwBackgroundClass"
// KPSglwEx.exe
#define	SGLWEX_WND_MAIN_CAPTION			L"KPSglwCaption"				// ������
#define	SGLWEX_WND_MAIN_CLASS			L"KPSglwClass"

// KPDesktop.exe
#define	KPDESKTOP_WND_MAIN_CAPTION		L"KPDesktopCaption"				// ������
#define	KPDESKTOP_WND_MAIN_CLASS		L"KPDesktopClass"
#define	KPDESKTOP_WND_BKGD_CAPTION		L"KPDesktopBackgroundCaption"	// �����ڵĸ�����
#define	KPDESKTOP_WND_BKGD_CLASS		L"KPDesktopBackgroundClass"
// KPDTTool.exe
#define	KPDTTOOL_WND_MAIN_CAPTION		L"KPDTToolCaption"				// ������
#define	KPDTTOOL_WND_MAIN_CLASS			L"KPDTToolClass"
#define	KPDTTOOL_WND_BKGD_CAPTION		L"KPDTToolBackgroundCaption"	// �����ڵĸ�����
#define	KPDTTOOL_WND_BKGD_CLASS			L"KPDTToolBackgroundClass"
// jzzm.exe
#define KPJZZM_WND_CAPTION				L"��������"
#define KPJZZM_WND_CLASS				L"JuziDesktopDui"
// favoriter.exe
#define KPFAVORITER_WND_MAIN_CAPTION	L"KPFavoriterCaption"
#define KPFAVORITER_WND_MAIN_CLASS		L"KPFavoriterClass"

// KPDown.exe
#define KPDOWN_WND_MAIN_CAPTION			L"KPDownCaption"
#define KPDOWN_WND_MAIN_CLASS			L"KPDownClass"
#endif