#ifndef __THEME_H__
#define __THEME_H__
#pragma once
#include "AC/basedef.h"
#include <Uxtheme.h>
#include <tmschema.h>
namespace acm {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	theme��	2014-12-29
//	windows������ز�����
//  �ο���
//		��Ӱ�֣�http://msdn.microsoft.com/en-us/library/bb688195(VS.85).aspx
//		��չ����������������http://www.codeproject.com/Articles/14141/Band-Objects-NET-Redux
//		windows���ÿؼ���http://www.codeproject.com/Articles/1688/Add-XP-Visual-Style-Support-to-OWNERDRAW-Controls
//		Theme Explorer��http://www.codeproject.com/Articles/620045/Custom-Controls-in-Win-API-Visual-Styles
////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define DRAWTEXT_FLAG_NODE		(DT_CENTER|DT_MODIFYSTRING|DT_END_ELLIPSIS|DT_WORDBREAK|DT_WORD_ELLIPSIS)
// Uxtheme API����
typedef HTHEME  (WINAPI* fn_OpenThemeData)(HWND, PCWSTR);
typedef HRESULT (WINAPI* fn_CloseThemeData)(HTHEME);
typedef HRESULT (WINAPI* fn_DrawThemeTextEx)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwFlags, LPRECT pRect, const DTTOPTS *pOptions);
typedef HRESULT (WINAPI* fn_GetThemeSysFont)(HTHEME hTheme, int iFontID, LOGFONTW *plf);
typedef HRESULT (WINAPI* fn_DrawThemeIcon)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, HIMAGELIST himl, int iImageIndex);
typedef HRESULT (WINAPI* fn_DrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
typedef HRESULT (WINAPI* fn_DrawThemeParentBackground)(HWND hwnd, HDC hdc, const RECT *prc);
typedef HRESULT (WINAPI* fn_GetThemeColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor);
class theme {
public:
	theme();
	~theme();

	/****************************************************************************************
		<< --- open		2014-12-29 --- >>         
		˵����������
		������
			pClassList	= ����������
			hWnd		= ���ھ��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool open(HWND hWnd, PCWSTR pClassList);
	/****************************************************************************************
		<< --- close		2014-12-29 --- >>         
		˵����
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool close();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-5 �Ƿ��Ѵ�
	bool isOpen() const;
	/****************************************************************************************
		<< --- drawShadowText		2014-12-29 --- >>         
		˵����������������ڵ��ǩ������Ӱ
		������
			hdc			= ��ͼ����
			pstrText	= �����Ƶ��ı�
			prcArea		= ��������
			dwFlags		= ���Ʒ�ʽ
			iFontID		= ��������
			clrText		= �ı�ɫ
			clrShadow	= ��Ӱɫ
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool drawShadowText(HDC hDC, PCWSTR pstrText, LPRECT prcArea, COLORREF clrText = 0x0FFFFFF, COLORREF clrShadow = 0x00, DWORD dwFlags = DRAWTEXT_FLAG_TITLE, int iFontID = TMT_ICONTITLEFONT);
	
	/****************************************************************************************
		<< --- drawBackground		2014-12-30 --- >>         
		˵�������Ʊ���
		������
			hdc			= ��ͼ����
			prcArea		= ��������
			iPartID		= ���ƿؼ��Ĳ���
			iStateID	= �����������״̬
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool drawBackground(HDC hDC, LPRECT prcArea, int iPartID, int iStateID);

	/****************************************************************************************
		<< --- drawParentBackground		2014-12-30 --- >>         
		˵�������Ƹ����ڱ���
		������
		hdc			= ��ͼ����
		prcArea		= ��������
		hwndChild	= �Ӵ���
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool drawParentBackground(HDC hDC, LPRECT prcArea, HWND hwndChild = NULL);

	/****************************************************************************************
		<< --- drawIcon		2014-12-30 --- >>         
		˵��������ͼ��
		������
			hdc			= ��ͼ����
			iImageIndex	= ��ͼ���б�ؼ��е�����
			hImageList	= ���ͼ�꼯�ϣ�ͼ���б�ؼ�
			prcArea		= ��������
			iPartID		= ���ƿؼ��Ĳ���
			iStateID	= �����������״̬
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool drawIcon(HDC hDC, int iImageIndex, HIMAGELIST hImageList, LPRECT prcArea, int iPartID, int iStateID);
public:
	/****************************************************************************************
		<< --- initEnvironment		2014-12-29 --- >>         
		˵������������API������ϵͳֻ��ִ��һ��
		������
			pdllPath	= �����·��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool initEnvironment(PCWSTR pdllPath = L"UxTheme.dll");
	/****************************************************************************************
		<< --- releaseEnvironment		2014-12-29 --- >>         
		˵�����ͷ�initEnvironment���ص���Դ
		������
		����ֵ��
	*****************************************************************************************/
	static void releaseEnvironment();
protected:
	static HMODULE	mc_hmdlTheme;	// UxTheme.dll
	HWND	m_hwndTheme;	// ���ⴰ��
	HTHEME	m_hTheme;		// 
};

}
#endif