#ifndef __HIGHLEVELAPI_H__
#define __HIGHLEVELAPI_H__
/****************************************************************************************
	˵����������ֻΪ��ЩXP�²�֧�ֵ�API�ṩ��̬��������
*****************************************************************************************/

#include <Shobjidl.h>		// Shell32.dll
#include "singleton.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-4-10 Shell32.dll API �������	
typedef HRESULT (WINAPI* fn_SHCreateItemFromParsingName)(PCWSTR, IBindCtx *, REFIID, void **);
typedef HRESULT (WINAPI* fn_SHCreateItemFromRelativeName)(IShellItem *, PCWSTR, IBindCtx *, REFIID, void **ppv);
typedef HRESULT (WINAPI* fn_SHCreateShellItemArrayFromIDLists)(UINT, PCIDLIST_ABSOLUTE_ARRAY, IShellItemArray **);



extern fn_SHCreateItemFromParsingName			_fnSHCreateItemFromParsingName;
extern fn_SHCreateItemFromRelativeName			_fnSHCreateItemFromRelativeName;
extern fn_SHCreateShellItemArrayFromIDLists		_fnSHCreateShellItemArrayFromIDLists;

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-6-9 User32.dll API �������
typedef BOOL (WINAPI* fn_ChangeWindowMessageFilter)(UINT  /*message*/, DWORD /*dwFlag*/);
typedef BOOL (WINAPI* fn_ChangeWindowMessageFilterEx )(HWND /*hWnd*/, UINT /*message*/, DWORD /*action*/, PCHANGEFILTERSTRUCT /*pChangeFilterStruct*/);

extern fn_ChangeWindowMessageFilter				_fnChangeWindowMessageFilter;
extern fn_ChangeWindowMessageFilterEx			_fnChangeWindowMessageFilterEx;

namespace acm {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	higtLevelAPI  ��		2015-4-10
//	������ṩһ�����غ�ж��dll������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class highLevelAPI : public gtc::singleton<highLevelAPI>
{
public:
	highLevelAPI();
	~highLevelAPI();
public:
	static highLevelAPI& getSingleton()
	{
		assert(m_pSingleton);
		return *m_pSingleton;
	}

	static highLevelAPI* getSingletonPtr()
	{
		return m_pSingleton;
	}
private:
	HMODULE	m_hmdlShell32;	// Shell32.dll
	HMODULE m_hmdlUser32;	// User32.dll
};

}	// end acm
#endif	// end __HIGHLEVELAPI_H__