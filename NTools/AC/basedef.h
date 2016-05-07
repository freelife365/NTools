#ifndef __BASEDEF_H__
#define __BASEDEF_H__
#pragma once
#pragma warning(disable:4005)

#include <cstdint>
#include <cstddef>
#include <assert.h>
#include <cctype>
#include <cwctype>
#include <cwchar>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <memory>
#if defined(_ATL)
	#include <atltypes.h>
	#include <ATLComTime.h>
#endif
#if defined(_AFXDLL)
	#include <afxwin.h>
	#include <afxdisp.h>
	#include <afxext.h>         // MFC ��չ
	#include <afxframewndex.h>
#else
#endif
#if defined(_WIN32) && !defined(_AFXDLL)
	#include <windows.h>
	#include <windowsx.h>
#endif
#if defined(_WIN32)
	#include <process.h>
	#include <comutil.h>
	#include <comdef.h>
	#include <commctrl.h>
	#include <stddef.h>
	#include <tchar.h>
	#include <assert.h>
	#include <crtdbg.h>
	#include <malloc.h>
	#include <olectl.h>
	#include <ShlObj.h>

#endif
#include "debugger.h"
#include "baseEnum.h"
#if defined(_DEBUG)
#define DEBUG_LOG	gtc::debugger::message
#define WRITE_LOG	gtc::debugger::write
//#define SEND_LOG	gtc::debugger::write/*_tick*/
#define SEND_LOG	gtc::debugger::send/*_tick*/
#define SEND_LOGA	gtc::debugger::sendA
#define SEND_LASTERROR()	SEND_LOG(L"ERROR %s �쳣���񣬴����룺%u", __FUNCTIONW__, ::GetLastError())

#else
#define DEBUG_LOG	
#define WRITE_LOG	
#define SEND_LOG	
#define SEND_LOGA
#define SEND_LASTERROR()
//
//#define DEBUG_LOG	gtc::debugger::message
//#define WRITE_LOG	gtc::debugger::write
//#define SEND_LOG	gtc::debugger::write
//#define SEND_LOGA	gtc::debugger::sendA
//#define SEND_LASTERROR()	SEND_LOG(L"ERROR %s �쳣���񣬴����룺%u", __FUNCTIONW__, ::GetLastError())

//#define DEBUG_LOG	gtc::debugger::write
//#define WRITE_LOG	gtc::debugger::write
//#define SEND_LOG	gtc::debugger::write
//#define SEND_LOGA
//#define SEND_LASTERROR()

#endif
#ifndef UINT_PTR
	typedef unsigned int UINT_PTR;
#endif
#ifndef INT_PTR
	typedef int INT_PTR;
#endif

#ifndef MAX_PATH
	#define MAX_PATH 260
#endif

//////////////////////////////////////////////////////////////////////////
// �����������Ͷ���
// �����û�ʹ�õ�SOCKET���ͽ�����������
typedef std::vector<char>		SCDataType;
typedef std::vector<wchar_t>	SWDataType;
// �ֽ����������ͣ������洢����������
typedef std::vector<uint8_t>	SBDataType;
// 
typedef std::map<std::wstring, std::wstring>	StringMapType;
typedef std::map<std::string, std::string>		SStringMapType;
typedef std::map<INT_PTR, std::wstring>			IStringMapType;
typedef std::vector<std::wstring>				StringArrayType;
typedef std::vector<std::string>				SStringArrayType;
typedef std::vector<int>						IntArrayType;
typedef std::set<int>							IntSetType;
#if defined(_WIN32)
	typedef std::vector<LPITEMIDLIST>			IIDLArrayType;
#endif
//////////////////////////////////////////////////////////////////////////
//	2015-9-20	
#define RGB_FLIP(color)		(((color) & 0xFF00FF00) | (((color) >> 16) & 0x0FF) | (((color) << 16) & 0x0FF0000))
// ���ݿ������ļ��е�key
#define DBKEY_LOGIN					L"LOGIN"
#define DBKEY_PASSWORD				L"PW"
#define DBKEY_IP					L"IP"
#define DBKEY_NAMEORPATH			L"DB"
#define DBKEY_TYPE					L"TYPE"
// ��Ϣ�Ի���ı���
#define _NDEF_MESSAGEBOX_TITLE		L"��Ϣ��ʾ"
// ��־�ļ���·��
#define _NDEF_DIR_GTDATA			L"kpzm"
#define _NDEF_DIR_GTLOG				L"LOG"

// �����ļ��Ի�����
static wchar_t gc_wsImageFilter[] = L"ͼ���ļ�(*.ico)|*.ico|ͼ���ļ�(*.jpg)|*.jpg|bmp�ļ�(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||";
// ʱ��ת����
#define EPOCH_OFFSET 116444736000000000LL	// 1601��1��1�յ�1970��1��1�յ�ʱ����λ100����
// ����float�����ݵ������Сֵ
#define FLOAT_MAX	(3000000000.0f)
#define FLOAT_MIN	(-3000000000.0f)
//namespace gtc{
//#if defined(_DEBUG)
//	#define DEBUG_LOG __debug
//#else
//	#define DEBUG_LOG
//#endif
//
//static void __debug(const wchar_t *vpformat, ...) {
//	/*if(vhaveLine)
//		OutputDebugString(_T("------------------------------------------------------\n"));*/
//	wchar_t pinfo[512] = {0};
//	va_list argList;
//	va_start(argList, vpformat);
//	std::vswprintf(pinfo, _countof(pinfo)-sizeof(wchar_t), vpformat, argList);
//	va_end(argList);
//#if defined(_WIN32)
//	std::wcscat(pinfo, L"\n");
//	OutputDebugStringW(pinfo);
//#endif
//
//}
#define TIME_TYPE_YEAR		1900		// ����������Ϊʱ������ʱ��Ӧ��Ĭ���꣬��������ȽϵȲ���
#define TIME_TYPE_MONTH		1			// ����������Ϊʱ������ʱ��Ӧ��Ĭ���£���������ȽϵȲ���
#define TIME_TYPE_DAY		1			// ����������Ϊʱ������ʱ��Ӧ��Ĭ���գ���������ȽϵȲ���


/*****************************************************************************************
	<< --- toStringBEDataType		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	optor = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BEDataType optor)
{
	switch (optor)
	{
	case BEDataTypeNone: //= 0,		// VT_EMPTY
		return L"BEDataTypeNone";
		break;
	//case BEDataTypeNULL: // = 1,		// VT_NULL
	//	return L"BEDataTypeNULL";
	//	break;
	case BEDataTypeByte: //,			// VT_UI1
		return L"BEDataTypeByte";
		break;
	case BEDataTypeBoolean: //,		// VT_BOOL
		return L"BEDataTypeBoolean";
		break;
	case BEDataTypeString: //,		// VT_BSTR
		return L"BEDataTypeString";
		break;
	case BEDataTypeInt16:
		return L"BEDataTypeInt16";
		break;
	case BEDataTypeInt32: //,		// VT_I4
		return L"BEDataTypeInt32";
		break;
	case BEDataTypeInt64: //,		// VT_I8
		return L"BEDataTypeInt64";
		break;
	case BEDataTypeFloat: //,		// VT_R4
		return L"BEDataTypeFloat";
		break;
	case BEDataTypeDouble: //,		// VT_R8
		return L"BEDataTypeDouble";
		break;
	case BEDataTypeDate: //,			// VT_DATE
		return L"BEDataTypeDate";
		break;
	case BEDataTypeTime: //,			// VT_DATE
		return L"BEDataTypeTime";
		break;
	case BEDataTypeDatetime: //,		// VT_DATE
		return L"BEDataTypeDatetime";
		break;
	case BEDataTypeImage: //,		// �ֽ��� VT_ARRAY | VT_UI1��2011-07-12
		return L"BEDataTypeImage";
		break;
	case BEDataTypeText: //			// �����ַ���VT_BSTR
		return L"BEDataTypeText";
		break;
	default:
		break;
	}
	return L"";
}



static std::wstring toString(BEOrientationFlag orientate)
{
	switch(orientate)
	{
	case BEOrientationFlagNone:				return L"BEOrientationFlagNone";		break;
	case BEOrientationFlagCenter:			return L"BEOrientationFlagCenter";		break;
	case BEOrientationFlagTop:				return L"BEOrientationFlagTop";			break;
	case BEOrientationFlagBottom:			return L"BEOrientationFlagBottom";		break;
	case BEOrientationFlagLeft:				return L"BEOrientationFlagLeft";		break;
	case BEOrientationFlagRight:			return L"BEOrientationFlagRight";		break;
	case BEOrientationFlagLeftTop:			return L"BEOrientationFlagLeftTop";		break;
	case BEOrientationFlagLeftBottom:		return L"BEOrientationFlagLeftBottom";	break;
	case BEOrientationFlagRightTop:			return L"BEOrientationFlagRightTop";	break;
	case BEOrientationFlagRightBottom:		return L"BEOrientationFlagRightBottom";	break;
	default:								return L"δ֪����";		break;
	}
	return L"δ֪����";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ͼ���������𷽱��
//	int32_t			iSender;		// ���������߱��
//	BEItemAction	beAction;		// �������
typedef struct BSActionSender {
	BESenderFlag	beSender;		// ���������߱��
	BEItemAction	beAction;		// �������
	INT_PTR			itemObject;		// ���ڸı����
	BSActionSender(BESenderFlag sender, BEItemAction action, INT_PTR itObj = 0)
		: beSender(sender), beAction(action), itemObject(itObj)
	{}
}BSActionSender;

///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-5 ���嶨��	
struct BSFontInfo
{
	std::wstring wsFontName;
	int iSize;
	bool bBold;
	bool bUnderline;
	bool bItalic;

	BSFontInfo()
		: iSize(12), bBold(false), bUnderline(false), bItalic(false)
	{}
	BSFontInfo(PCWSTR pfontName, int fontSize = 12) 
		: wsFontName(pfontName?pfontName:L""), iSize(fontSize)
		, bBold(false), bUnderline(false), bItalic(false)
	{}

};
struct BSFontInfoUC : public BSFontInfo
{
	HFONT hFont;
	//std::wstring wsFontName;
	//int iSize;
	//bool bBold;
	//bool bUnderline;
	//bool bItalic;
	TEXTMETRIC tm;

	BSFontInfoUC()
		: BSFontInfo(), hFont(NULL)
	{
		memset(&tm, 0, sizeof(tm));
	}
	~BSFontInfoUC()
	{
		SEND_LOG(L"BSFontInfoUC free ...");
	}
};
//////////////////////////////////////////////////////////////////////////
// �ļ�������Ϣ
typedef struct BSFileAttribute
{
	uint32_t		uiAttribute;	// ����
	std::time_t		dtCreate;		// ��������
	std::time_t		dtLastAccess;	// 
	std::time_t		dtLastWrite;	// �޸�����
	int64_t			llSize;			// ��С
}BSFileAttribute;

#define FILE_ATTRIBUTE_CREATE_TIME			0x0001		// �ļ����Կ��ر�ǣ�����ʱ��
#define FILE_ATTRIBUTE_LASTACCESS_TIME		0x0002		// �ļ����Կ��ر�ǣ�������ʱ��
#define FILE_ATTRIBUTE_LASTWRITE_TIME		0x0004		// �ļ����Կ��ر�ǣ����дʱ��
#define FILE_ATTRIBUTE_ALL_TIME				(FILE_ATTRIBUTE_CREATE_TIME|FILE_ATTRIBUTE_LASTACCESS_TIME|FILE_ATTRIBUTE_LASTWRITE_TIME)

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ҵ�ˮƽ���ж��뷽ʽ
#ifndef DT_TOP
	#define DT_TOP                      0x00000000
#endif
#ifndef DT_LEFT
	#define DT_LEFT                     0x00000000
#endif
#ifndef DT_CENTER
	#define DT_CENTER                   0x00000001
#endif
#ifndef DT_RIGHT
	#define DT_RIGHT                    0x00000002
#endif
#ifndef DT_VCENTER
	#define DT_VCENTER                  0x00000004
#endif
#ifndef DT_BOTTOM
	#define DT_BOTTOM                   0x00000008
#endif
#ifndef DT_SINGLELINE
	#define DT_SINGLELINE               0x00000020
#endif

// �����ı��ĸ�ʽ
// ���С���ֱˮƽ���У��������򲿷���3��ʡ�ԺŴ���
#define DRAWTEXT_FLAG_TITLE		(DT_CENTER|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EDITCONTROL|DT_VCENTER|DT_SINGLELINE)
// �ı����˾��л��ƣ����ʳ�����������Ŀ��ʱ�Զ�����(���ʷָ����)���������ױ�����ʱ��3��ʡ�ԺŴ��泬������
#define DRAWTEXT_FLAG_EDIT		(DT_CENTER|DT_MODIFYSTRING|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EDITCONTROL|DT_WORDBREAK)
// ��������ڵ��ı�������ʽ���ı����˾��л��ƣ����ʳ�����������Ŀ��ʱ��3��ʡ�ԺŴ��泬������(���ʲ��ָ�)���������ױ�����ʱ��3��ʡ�ԺŴ��泬������
#define DRAWTEXT_FLAG_NODE		(DT_CENTER|DT_MODIFYSTRING|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EDITCONTROL|DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_NOFULLWIDTHCHARBREAK)
#define DRAWTEXT_FLAG_LIST		(DT_LEFT  |DT_MODIFYSTRING|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EDITCONTROL|DT_VCENTER|DT_SINGLELINE)
// �ı�ȫ�������Զ�����
#define DRAWTEXT_FLAG_SHOWALL	(DT_CENTER|DT_NOPREFIX|DT_EDITCONTROL|DT_WORDBREAK)
//#define DRAWTEXT_FLAG_SHOWALL	(DT_CENTER|DT_NOPREFIX|DT_EDITCONTROL|DT_WORDBREAK)
//DT_WORDBREAK | DT_END_ELLIPSIS | DT_NOPREFIX | DT_EDITCONTROL

/*****************************************************************************************
	<< --- toStringBEAlignMode		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	optor = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BEAlignMode optor)
{
	switch (optor)
	{
	case BEAlignModeAllLeft:			//= DT_LEFT|DT_SINGLELINE|DT_VCENTER,
		return L"BEAlignModeAllLeft";
		break;
	case BEAlignModeAllRight:			//= DT_RIGHT|DT_SINGLELINE|DT_VCENTER,
		return L"BEAlignModeAllRight";
		break;
	case BEAlignModeAllCenter:			//= DT_CENTER|DT_SINGLELINE|DT_VCENTER,
		return L"BEAlignModeAllCenter";
		break;
	case BEAlignModeLeftTop:			//= DT_LEFT|DT_TOP,			// ͼƬ���������
		return L"BEAlignModeLeftTop";
		break;
	case BEAlignModeLeftCenter:			//= DT_LEFT|DT_VCENTER,		// ͼƬ�м������
		return L"BEAlignModeLeftCenter";
		break;
	case BEAlignModeLeftBottom:			//= DT_LEFT|DT_BOTTOM,		// ͼƬ�ײ������
		return L"BEAlignModeLeftBottom";
		break;
	case BEAlignModeCenterTop:			//= DT_CENTER|DT_TOP,		// ͼƬ��������
		return L"BEAlignModeCenterTop";
		break;
	case BEAlignModeCenterCenter:			//= DT_CENTER,				// ͼƬ�м����
		return L"BEAlignModeCenterCenter";
		break;
	case BEAlignModeCenterBottom:			//= DT_CENTER|DT_BOTTOM,	// ͼƬ�ײ�����
		return L"BEAlignModeCenterBottom";
		break;
	case BEAlignModeRightTop:			//= DT_RIGHT|DT_TOP,			// ͼƬ�����Ҷ���
		return L"BEAlignModeRightTop";
		break;
	case BEAlignModeRightCenter:			//= DT_RIGHT|DT_VCENTER,	// ͼƬ�м��Ҷ���
		return L"BEAlignModeRightCenter";
		break;
	case BEAlignModeRightBottom:			//= DT_RIGHT					// ͼƬ�ײ��Ҷ���
		return L"BEAlignModeRightBottom";
		break;
	default:
		break;
	}
	return L"";
}

/*****************************************************************************************
	<< --- toStringBELogicOptor		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	cmethod = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BEComputeMethod cmethod)
{
	switch (cmethod)
	{
	case BEComputeMethodNone:			//= 0,		// �� ϵͳ����CM_NONE = 0
		return L"BEComputeMethodNone";
		break;
	case BEComputeMethodCount:			//= 1,		// �Ǵ�
		return L"BEComputeMethodCount";
		break;
	case BEComputeMethodSum:			//= 2,		// ���
		return L"BEComputeMethodSum";
		break;
	case BEComputeMethodAvg:			//= 3,		// ��ƽ��
		return L"BEComputeMethodAvg";
		break;
	case BEComputeMethodMax:			//= 4,		// ���ֵ
		return L"BEComputeMethodMax";
		break;
	case BEComputeMethodMin:			//= 5			// ��Сֵ
		return L"BEComputeMethodMin";
		break;
	default:
		break;
	}
	return L"";
}

/*****************************************************************************************
	<< --- BERelationalOptor		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	relational = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BERelationalOptor relational)
{
	switch (relational)
	{
	case BERelationalOptorNone:					//= 0,		// δ�����
		return L"BERelationalOptorNone";
		break;
	case BERelationalOptorEquals:				//= 1,		// ����
		return L"BERelationalOptorEquals";
		break;
	case BERelationalOptorNotEqual:				//= 2,		// ������
		return L"BERelationalOptorNotEqual";
		break;
	case BERelationalOptorLessthan:				//= 3,		// С��
		return L"BERelationalOptorLessthan";
		break;
	case BERelationalOptorLessthanOrEqual:		//= 4,		// С�ڵ���
		return L"BERelationalOptorLessthanOrEqual";
		break;
	case BERelationalOptorGreaterthan:			//= 5,		// ����
		return L"BERelationalOptorGreaterthan";
		break;
	case BERelationalOptorGreaterthanOrEqual:	//= 6			// ���ڵ���
		return L"BERelationalOptorGreaterthanOrEqual";
		break;
	default:
		break;
	}
	return L"";
}

/*****************************************************************************************
	<< --- toStringBELogicOptor		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	optor = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BELogicOptor optor)
{
	switch (optor)
	{
	case BELogicOptorNone:			//= 0,		// δ�����
		return L"BELogicOptorNone";
		break;
	case BELogicOptorAnd:			//= 1,		// �߼���
		return L"BELogicOptorAnd";
		break;
	case BELogicOptorOr:			//= 2,		// ��
		return L"BELogicOptorOr";
		break;
	case BELogicOptorNot:			//= 3		// ��
		return L"BELogicOptorNot";
		break;
	default:
		break;
	}
	return L"";
}

static std::wostream& operator<<(std::wostream &wos, const std::wstring &wstr)
{
	return wos << wstr.c_str();
}

static UINT keyState()
{
	UINT uState = 0;
	if( ::GetKeyState(VK_CONTROL) < 0 ) uState |= MK_CONTROL;
	if( ::GetKeyState(VK_RBUTTON) < 0 ) uState |= MK_RBUTTON;
	if( ::GetKeyState(VK_LBUTTON) < 0 ) uState |= MK_LBUTTON;
	if( ::GetKeyState(VK_SHIFT) < 0 ) uState |= MK_SHIFT;
	if( ::GetKeyState(VK_MENU) < 0 ) uState |= MK_ALT;
	return uState;
}

/*****************************************************************************************
	<< ---  global::operator==RECT		2014-06-25 --- >>
	˵�����ж��������������Ƿ�һ��
	������
	rt1		= ���Ƚϵ�����1
	rt2		= ���Ƚϵ�����2
	����ֵ���Ƿ�һ��
*****************************************************************************************/
static bool operator==(const RECT &rt1, const RECT &rt2)
{
	return rt1.left == rt2.left && rt1.top == rt2.top && rt1.right == rt2.right && rt1.bottom == rt2.bottom;
}

static bool operator!=(const RECT &rt1, const RECT &rt2)
{
	return !(rt1 == rt2);
}

static bool isEmpty(const RECT &rt)
{
	return rt.left == rt.right && rt.top == rt.bottom;
}

static bool rcInRect(const RECT &rtIn, const RECT &rt)
{
	POINT pt1 = {rt.left,rt.top};
	POINT pt2 = {rt.right,rt.bottom};
	return ::PtInRect(&rtIn, pt1) && ::PtInRect(&rtIn, pt2);
}

static int rectWidth(const RECT &rt)
{
	return rt.right-rt.left;
}

static int rectHeight(const RECT &rt)
{
	return rt.bottom - rt.top;
}


static SIZE rectSize(const RECT &rt)
{
	SIZE sz = {rt.right-rt.left, rt.bottom-rt.top};
	return sz;
}

static RECT rectJoin(const RECT &rt1, const RECT &rt2)
{
	RECT rt = {0};
	rt.left = min(rt1.left, rt2.left);
	rt.top = min(rt1.top, rt2.top);
	rt.right = max(rt1.right, rt2.right);
	rt.bottom = max(rt1.bottom, rt2.bottom);
	return rt;
}
static SIZE operator+(const SIZE &sz1, const SIZE &sz2)
{
	SIZE sz = {sz1.cx + sz2.cx, sz1.cy + sz2.cy};
	return sz;
}

/*****************************************************************************************
	<< ---  global::operator==POINT		2014-06-25 --- >>
	˵�����ж������������Ƿ�һ��
	������
	pt1		= ���Ƚϵĵ�1
	pt2		= ���Ƚϵĵ�2
	����ֵ���Ƿ�һ��
*****************************************************************************************/
static bool operator==(const POINT &pt1, const POINT &pt2)
{
	return pt1.x == pt2.x && pt1.y == pt2.y;
}
static bool operator!=(const POINT &pt1, const POINT &pt2)
{
	return !(pt1==pt2);
}

static bool operator==(const SIZE &sz1, const SIZE &sz2)
{
	return sz1.cx == sz2.cx && sz1.cy == sz2.cy;
}
static bool operator!=(const SIZE &sz1, const SIZE &sz2)
{
	return !(sz1 == sz2);
}

/*****************************************************************************************
	<< ---  global::calculateAreaCell		2014-08-01 --- >>
	˵����������鰴ָ����Ԫ���С���зָ��Ԫ���Ŀ�϶ΪspaceXY���������ı�û�п�϶��������˳��Ϊ�����ң����ϵ���
	������
	szArea		= ���ָ������1
	szCell		= ��Ԫ���С
	spaceXY		= ��Ԫ���Ŀ�϶
	����ֵ����Ԫ�����꼯��
*****************************************************************************************/
static std::vector<RECT> calculateAreaCell(SIZE szArea, SIZE szCell, int spaceXY = 0)
{
	std::vector<RECT> vecCell;
	int rows = (szArea.cy + spaceXY)/(szCell.cy + spaceXY);
	int cols = (szArea.cx + spaceXY)/(szCell.cx + spaceXY);
	RECT rc = {0};
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			rc.left = col * (szCell.cx + spaceXY);
			rc.top = row * (szCell.cy + spaceXY);
			rc.right = rc.left + szCell.cx;
			rc.bottom = rc.top + szCell.cy;
			vecCell.push_back(rc);
		}
	}
	return vecCell;
}
/****************************************************************************************
	<< --- sliceAreaCell		2015-7-27 --- >>         
	˵��������λ�������õ�����������λ��������isHorizontal=trueʱ��ˮƽ�����ң����ϵ��£��磺	0 1 2
																					3 4 5
																					6 7 8
										isHorizontal=falseʱ�����ϵ��£�ˮƽ�����ң��磺0 3 6
																					1 4 7
																					2 5 8
	������
		posIndex		= Ҫ��λ��λ������
		szCell			= ��λ��Ԫ���С
		isHorizontal	= ������������
		fixCount		= isHorizontal=true��ʾ�̶���������isHorizontal=false��ʾ�̶�������
	����ֵ��
*****************************************************************************************/
static RECT sliceAreaCell(int posIndex, SIZE szCell, bool isHorizontal = true, int fixCount = 3)
{
	assert(posIndex >= 0);
	int col, row;
	if (isHorizontal)
	{
		col = posIndex % fixCount;
		row = posIndex / fixCount;
	}
	else
	{
		col = posIndex / fixCount;
		row = posIndex % fixCount;
	}
	RECT rc = {0};
	rc.left = col * szCell.cx;
	rc.right = rc.left + szCell.cx;
	rc.top = row * szCell.cy;
	rc.bottom = rc.top + szCell.cy;
	return rc;
}
static RECT sliceAreaCell(int posIndex, int cxcy, bool isHorizontal = true, int fixCount = 3)
{
	SIZE sz = {cxcy, cxcy};
	return sliceAreaCell(posIndex, sz, isHorizontal, fixCount);
}
static std::wstring sliceAreaCellString(int posIndex, SIZE szCell, bool isHorizontal = true, int fixCount = 3)
{
	RECT rc = sliceAreaCell(posIndex, szCell, isHorizontal, fixCount);
	wchar_t pbuf[80] = {0};
	std::swprintf(pbuf, _countof(pbuf), L"%d,%d,%d,%d", rc.left, rc.top, rc.right, rc.bottom);
	return pbuf;
}
static std::wstring sliceAreaCellString(int posIndex, int cxcy, bool isHorizontal = true, int fixCount = 3)
{
	SIZE sz = {cxcy, cxcy};
	return sliceAreaCellString(posIndex, sz, isHorizontal, fixCount);
}
namespace gtc{

	// ����������̬��
	static std::wostream& outClassBegin(std::wostream &os, const std::wstring &cls, int indent)
	{
		os << std::setw(indent) << std::left << L"" << cls.c_str() << L"\n";
		os << std::setw(indent) << L"" << std::setw(2) << L"[" << L"\n";
		return os;
	}
	static std::wostream& outClassEnd(std::wostream &os, const std::wstring &cls, int indent)
	{
		os << std::setw(indent) << std::left << L"" << std::setw(2) << L"]" << cls.c_str() << L"\n";
		return os;
	}

	template<class Value>
	static std::wostream& outClassItem(std::wostream &os, const std::wstring &title, Value &val, int indent)
	{
		os << std::right << std::setw(indent+10) /*<< " *" << std::setw(16)*/ << title.c_str() /* << std::setw(16)*/ << "  >  " << val << std::endl;// "\n";
		return os;
	}
	template<class Type>
	static std::wostream& outBaseClass(std::wostream &os, Type &ty, int indent)
	{
		os << std::setw(indent) << std::left << L"" << L"base class:" << "\n" << ty.description(indent);
		return os;
	}

}
//}
#endif