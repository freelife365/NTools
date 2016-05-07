#pragma once

#include <cwchar>

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
//#include <richedit.h>
//#include <tchar.h>
#include <assert.h>

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include "AC/image.h"
#include "AC/draw/drawer.h"
#include "AC/theme.h"
#include "KP/KPDeskInfo.h"
#include "KP/KPDeskDef.h"
//#include "KP/uc/windowWnd.h"
//#include "KP/uc/controlUC.h"
#define WMME_DESK_CHANGENOTIFY			WM_USER + 109	// �������¼�
#define WMME_WALLPAPER_CHANGENOTIFY		WM_USER + 110	// ǽֽ����¼�


#define SYSTEM_CLSID_COMPUTE	L"{20D04FE0-3AEA-1069-A2D8-08002B30309D}"		// �ҵĵ���
#define SYSTEM_CLSID_NETWORK	L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}"		// ����
#define SYSTEM_CLSID_CONTROL	L"{5399E694-6CE5-4D6C-8FCE-1D8870FDCBA0}"		// �������
#define SYSTEM_CLSID_USER		L"{59031a47-3f72-44a7-89c5-5595fe6b30ee}"		// �û�
#define SYSTEM_CLSID_RECYCLE	L"{645FF040-5081-101B-9F08-00AA002F954E}"		// ����վ

// ����״̬���
#define IS_KEYDOWN(vk_code) ((GetKeyState(vk_code) & 0x8000) ? true : false) 
#define IS_KEYUP(vk_code)	((GetKeyState(vk_code) & 0x1) ? true : false) 


//static UINT keyState()
//{
//	UINT uState = 0;
//	if( ::GetKeyState(VK_CONTROL) < 0 ) uState |= MK_CONTROL;
//	if( ::GetKeyState(VK_RBUTTON) < 0 ) uState |= MK_RBUTTON;
//	if( ::GetKeyState(VK_LBUTTON) < 0 ) uState |= MK_LBUTTON;
//	if( ::GetKeyState(VK_SHIFT) < 0 ) uState |= MK_SHIFT;
//	if( ::GetKeyState(VK_MENU) < 0 ) uState |= MK_ALT;
//	return uState;
//}

namespace gtc {
// ��ϵͳԤ������ɫ
#define DEF_COLOR_DRAPBAR		0x00C0A070			// ��קλ��������ɫ
// Ŀ¼��صõ���Ϣ
typedef struct SHNotifyInfo{
	DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
	DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNotifyInfo;


//
///*****************************************************************************************
//	<< ---  global::operator==RECT		2014-06-25 --- >>
//	˵�����ж��������������Ƿ�һ��
//	������
//	rt1		= ���Ƚϵ�����1
//	rt2		= ���Ƚϵ�����2
//	����ֵ���Ƿ�һ��
//*****************************************************************************************/
//static bool operator==(const RECT &rt1, const RECT &rt2)
//{
//	return rt1.left == rt2.left && rt1.top == rt2.top && rt1.right == rt2.right && rt1.bottom == rt2.bottom;
//}
//
//static bool operator!=(const RECT &rt1, const RECT &rt2)
//{
//	return !(rt1 == rt2);
//}
//
//static bool isEmpty(const RECT &rt)
//{
//	return rt.left == rt.right && rt.top == rt.bottom;
//}
//
//static bool rcInRect(const RECT &rtArea, const RECT &rt)
//{
//	POINT pt1 = {rt.left,rt.top};
//	POINT pt2 = {rt.right,rt.bottom};
//	return ::PtInRect(&rtArea, pt1) && ::PtInRect(&rtArea, pt2);
//}
//
//static int rectWidth(const RECT &rt)
//{
//	return rt.right-rt.left;
//}
//
//static int rectHeight(const RECT &rt)
//{
//	return rt.bottom - rt.top;
//}
//
//static Gdiplus::Rect toGdiplus(const RECT &rt)
//{
//	Gdiplus::Rect rc(rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top);
//	return rc;
//}
//static SIZE rectSize(const RECT &rt)
//{
//	SIZE sz = {rt.right-rt.left, rt.bottom-rt.top};
//	return sz;
//}
//
//
///*****************************************************************************************
//	<< ---  global::operator==POINT		2014-06-25 --- >>
//	˵�����ж������������Ƿ�һ��
//	������
//	pt1		= ���Ƚϵĵ�1
//	pt2		= ���Ƚϵĵ�2
//	����ֵ���Ƿ�һ��
//*****************************************************************************************/
//static bool operator==(const POINT &pt1, const POINT &pt2)
//{
//	return pt1.x == pt2.x && pt1.y == pt2.y;
//}
//static bool operator!=(const POINT &pt1, const POINT &pt2)
//{
//	return !(pt1==pt2);
//}

}