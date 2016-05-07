#ifndef __ANORMAL_H__
#define __ANORMAL_H__
#pragma once

//#include "AC/basedef.h"
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/vector.hpp>
////
#if defined(_AFXDLL)

#elif defined(_WIN32)


#endif

namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ĵ���Ҫ����acg��ö���͡��ṹ�͡�����
////////////////////////////////////////////////////////////////////////////////////////////////////////


//// ���ݿ������ļ��е�key
//#define DBKEY_LOGIN				L"LOGIN"
//#define DBKEY_PASSWORD			L"PW"
//#define DBKEY_IP				L"IP"
//#define DBKEY_NAMEORPATH		L"DB"
//#define DBKEY_TYPE				L"TYPE"
//// ��Ϣ�Ի���ı���
//#define _GTDEF_MESSAGEBOX_TITLE		L"��Ϣ��ʾ"
//// ��־�ļ���·��
//#define _GTDEF_DIR_GTDATA			L"GTDATA"
//#define _GTDEF_DIR_GTLOG			L"LOG"
//
//// �����û�ʹ�õ�SOCKET���ͽ�����������
//typedef std::vector<char> SCDataType;
//// �ֽ����������ͣ������洢����������
//typedef std::vector<uint8_t> SBDataType;

// ӳ��ģʽ
// 1������WORK_AXIS_CAD����ϵʱ����ͼ���ڵ�һ���ޣ����ң�����Ϊ����
//		 ^
//		 |
// height|
//		 |
//		 ----------->
//	   (0,0)   width
//		
// 2������WORK_AXIS_DOC || WORK_AXIS_NORMAL����ϵʱ
// 
//	   (0,0)   width
//		 ----------->
//		 |
// height|
//		 |
//       V
//	
typedef enum BEWorkAxis {
	BEWorkAxisNormal	= 1,	// ��������ϵ�����¡���Ϊ�������߼���λΪ����
	BEWorkAxisDOC		,		// �ĵ�����ϵ�����¡���Ϊ�������߼���λΪ0.1����
	BEWorkAxisCAD				// ��ͼ����ϵ�����ϡ���Ϊ�������߼���λΪ0.1����
}BEWorkAxis;

///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ᵥλ���
typedef enum BEUnitType {
	BEUnitTypeWeight	= 1,		// ����
	BEUnitTypeTime					// ʱ��
}BEUnitType;


///////////////////////////////////////////////////////////////////////////////////////////////////
// ������λ
typedef enum BEUnitWeight{
	 BEUnitWeightMilligram	= 1,		// ����
	 BEUnitWeightGram,					// ��
	 BEUnitWeightLiang,					// ��
	 BEUnitWeightJing,					// ��
	 BEUnitWeightKilogram,				// ����
	 BEUnitWeightTon					// ��
}BEUnitWeight;
static std::wstring toString(BEUnitWeight unit)
{
	switch (unit)
	{
	case BEUnitWeightMilligram:
		return L"����";
		break;
	case BEUnitWeightGram:
		return L"��";
		break;
	case BEUnitWeightLiang:
		return L"��";
		break;
	case BEUnitWeightJing:
		return L"��";
		break;
	case BEUnitWeightKilogram:
		return L"����";
		break;
	case BEUnitWeightTon:
		return L"��";
		break;
	default:
		assert(false && L"���ص�λ");
		break;
	}
	return L"";
}
// ��Ӧ�ĺ�����
static int toMilligram(BEUnitWeight unit)
{
	switch (unit)
	{
	case BEUnitWeightMilligram:
		return 1;
		break;
	case BEUnitWeightGram:
		return 1000;
		break;
	case BEUnitWeightLiang:
		return 50000;
		break;
	case BEUnitWeightJing:
		return 500000;
		break;
	case BEUnitWeightKilogram:
		return 1000000;
		break;
	case BEUnitWeightTon:
		return 1000000000;
		break;
	default:
		assert(false && L"���ص�λ");
		break;
	}
	return 0;
}
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
// ʱ�䵥λ
typedef enum BEUnitTime {
	BEUnitTimeMillisecond = 1,			// ����
	BEUnitTimeSecond,					// ��
	BEUnitTimeMinute					// ��
}BEUnitTime;
static std::wstring toString(BEUnitTime unit) {
	switch (unit)
	{
	case BEUnitTimeMillisecond:
		return L"����";
		break;
	case BEUnitTimeSecond:
		return L"��";
		break;
	case BEUnitTimeMinute:
		return L"��";
		break;
	default:
		assert(false && L"��ʱ��λ");
		break;
	}
	return L"";
}
// ��Ӧ�ĺ�����
static int toMillisecond(BEUnitTime unit)
{
	switch (unit)
	{
	case BEUnitTimeMillisecond:
		return 1;
		break;
	case BEUnitTimeSecond:
		return 1000;
		break;
	case BEUnitTimeMinute:
		return 60000;
		break;
	default:
		assert(false && L"��ʱ��λ");
		break;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ݱ���б�����ʽ
typedef enum BEGridColCaptionStyle {
	BEGridColCaptionStyleNone		= 0,	// ��֧���б���
	BEGridColCaptionStyleSingle		,		// ���б���
	BEGridColCaptionStyleDouble				// ˫�б��⣬��һ�б���ʵ���кϲ�
}BEGridColCaptionStyle;
/*****************************************************************************************
	<< --- toStringBEGridColCaptionStyle		2013-12-16 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	optor = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BEGridColCaptionStyle colCaptionStyle)
{
	switch (colCaptionStyle)
	{
	case BEGridColCaptionStyleNone:
		return L"BEGridColCaptionStyleNone";
		break;
	case BEGridColCaptionStyleSingle:
		return L"BEGridColCaptionStyleSingle";
		break;
	case BEGridColCaptionStyleDouble:
		return L"BEGridColCaptionStyleDouble";
		break;
	default:
		break;
	}
	return L"";
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// ���ʵ���ʽ
typedef enum BEPenStyle {
	BEPenStyleSolid		= 0,	// 0
	BEPenStyleDash,				// 1/* -------  */
	BEPenStyleDot,				// 2/* .......  */
	BEPenStyleDashDot,			// 3/* _._._._  */
	BEPenStyleDashDotDot,		// 4/* _.._.._  */
	BEPenStyleCustom,			// 5
	BEPenStyleNull				// 6
}BEPenStyle;
///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ˢ����ʽ
typedef enum BEBrushStyle {
	BEBrushStyleSolid	= 0,	// ʵ��
	BEBrushStyleNull	,		// ��
	BEBrushStyleHatch			// ����ˢ
}BEBrushStyle;
///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ˢ��������ʽ
typedef enum BEHatchStyle {
	BEHatchStyleHorizontal		= 0,	/* ----- */
	BEHatchStyleVertical		,		/* ||||| */
	BEHatchStyleForwardDiagonal	,		/* \\\\\ */
	BEHatchStyleBackwardDiagonal,		/* ///// */
	BEHatchStyleCross			,		/* +++++ */
	BEHatchStyleDiagonalCross			/* xxxxx */

}BEHatchStyle;

///////////////////////////////////////////////////////////////////////////////////////////////////
// һ��������CComboBox CListBox�ؼ���Ӹ�����
typedef enum BEAuxiliaryItem
{
	BEAuxiliaryItemNone		= 0x0,	// �޸�����
	BEAuxiliaryItemAll		= 0x1,	// �����ȫ��������Ӧ��ItemData=-1
	BEAuxiliaryItemWu		= 0x2,	// ������ޡ�����Ӧ��ItemData=-2
	BEAuxiliaryItemNull		= 0x4	// �����Ϊ�ա�����Ӧ��ItemData=-3
}BEAuxiliaryItem;


// 


} // end gtc


#endif