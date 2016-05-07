#ifndef __ANORMAL_H__
#define __ANORMAL_H__
#pragma once
#include <string>
#include <vector>
#include <cstdint>



//#include <boost/serialization/string.hpp>
//#include <boost/serialization/vector.hpp>
////

#if defined(_WIN32)

	#include <windows.h>
	#include <windowsx.h>
	#include <commctrl.h>
	#include <stddef.h>
	#include <richedit.h>
	#include <tchar.h>
	#include <assert.h>
	#include <crtdbg.h>
	#include <malloc.h>
	#include <olectl.h>
	#include <gdiplus.h>
#endif
#if defined(_AFXDLL)
	#include <afxwin.h>
	#include <atltypes.h>
	#include <gdiplus.h>
#endif


namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ĵ���Ҫ����acg��ö���͡��ṹ�͡�����
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAX_PATH
	#define MAX_PATH 260
#endif
// ���ݿ������ļ��е�key
#define DBKEY_LOGIN				L"LOGIN"
#define DBKEY_PASSWORD			L"PW"
#define DBKEY_IP				L"IP"
#define DBKEY_NAMEORPATH		L"DB"
#define DBKEY_TYPE				L"TYPE"
// ��Ϣ�Ի���ı���
#define _GTDEF_MESSAGEBOX_TITLE		L"��Ϣ��ʾ"
// ��־�ļ���·��
#define _GTDEF_DIR_GTDATA			L"GTDATA"
#define _GTDEF_DIR_GTLOG			L"LOG"

// �����û�ʹ�õ�SOCKET���ͽ�����������
typedef std::vector<char> SCDataType;


// ӳ��ģʽ
//#define WORK_AXIS_NORMAL	// ��������ϵ�����¡���Ϊ�������߼���λΪ����
//#define WORK_AXIS_DOC		// �ĵ�����ϵ�����¡���Ϊ�������߼���λΪ0.1����
//#define WORK_AXIS_CAD		// ��ͼ����ϵ�����ϡ���Ϊ�������߼���λΪ0.1����
typedef enum BEWorkAxis {
	BEWorkAxisNormal	= 1,	// ��������ϵ�����¡���Ϊ�������߼���λΪ����
	BEWorkAxisDOC		,		// �ĵ�����ϵ�����¡���Ϊ�������߼���λΪ0.1����
	BEWorkAxisCAD				// ��ͼ����ϵ�����ϡ���Ϊ�������߼���λΪ0.1����
}BEWorkAxis;

///////////////////////////////////////////////////////////////////////////////////////////////////
// �Զ���ͨ����������
typedef enum BEDataType{
							// VT_TYPE			STL						ADO.DataType			sqlite3
	BEDataTypeNone = 0,		// VT_EMPTY			��						adEmpty					SQLITE_NULL
	//BEDataTypeNULL = 1,		// VT_NULL
	BEDataTypeByte,			// VT_UI1			uint8_t					adUnsignedTinyInt		SQLITE_INTEGER
	BEDataTypeBoolean,		// VT_BOOL			bool					adBoolean				SQLITE_INTEGER
	BEDataTypeString,		// VT_BSTR			wstring					adVarWChar				SQLITE_TEXT
	BEDataTypeInt16,		// VT_I2			int16_t					adSmallInt				SQLITE_INTEGER
	BEDataTypeInt32,		// VT_I4			int32_t					adInteger				SQLITE_INTEGER
	BEDataTypeInt64,		// VT_I8			int64_t					adBigInt				SQLITE_INTEGER
	BEDataTypeFloat,		// VT_R4			float					adSingle				SQLITE_FLOAT
	BEDataTypeDouble,		// VT_R8			double					adDouble				SQLITE_FLOAT
	BEDataTypeDate,			// VT_DATE			tm						adDate					SQLITE_FLOAT
	BEDataTypeTime,			// VT_DATE			tm						adDate					SQLITE_FLOAT
	BEDataTypeDatetime,		// VT_DATE			tm						adDate					SQLITE_FLOAT
	BEDataTypeImage,		// VT_ARRAY|VT_UI1	std::vector<uint8_t>	adLongVarBinary			SQLITE_BLOB
	BEDataTypeText			// VT_BSTR			std::vector<wchar_t>	adLongVarWChar			SQLITE_TEXT
}BEDataType;

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
///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ͼ���������
typedef enum BEItemAction {
	BEItemActionAdd		= 1,	// ����
	BEItemActionModify	,		// �޸�
	BEItemActionDelete	,		// ɾ��
	BEItemActionSelect			// ѡ��
}BEItemAction;
///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ķ��ͷ�
typedef enum BESenderFlag {
	BESenderFlagApp			= 1,	// ϵͳApp
	BESenderFlagDoc			,		// �ĵ�����CDocument
	BESenderFlagMainFrame	,		// �����CMainFrame
	BESenderFlagView		,		// ��ͼCView
	BESenderFlagTreeDock	,		// ��״��������
	BESenderFlagPropDock			// ���Ը�������
}BESenderFlag;

///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ͼ���������𷽱��
//	int32_t			iSender;		// ���������߱��
//	BEItemAction	beAction;		// �������
typedef struct BSActionSender {
	BESenderFlag	beSender;		// ���������߱��
	BEItemAction	beAction;		// �������
	BSActionSender(BESenderFlag sender, BEItemAction action)
		: beSender(sender), beAction(action)
	{}
}BSActionSender;

///////////////////////////////////////////////////////////////////////////////////////////////////
// �����ᵥλ���
typedef enum BEUnitType {
	BEUnitTypeWeight	= 1,		// ����
	BEUnitTypeTime					// ʱ��
}BEUnitType;


///////////////////////////////////////////////////////////////////////////////////////////////////
// ������λ
typedef enum BEUnitWeight{
	 BEUnitWeightMicrogram	= 1,		// ΢��
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
	case BEUnitWeightMicrogram:
		return L"΢��";
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
		break;
	}
	return L"";
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
		break;
	}
	return L"";
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
// ���ݿ���������뷽��
enum BEParameterDirection{
    BEParameterDirectionUnknown			= 0,//adParamUnknown,// = 0,
    BEParameterDirectionInput			= 1,//adParamInput,// = 1,
    BEParameterDirectionOutput			= 2,//adParamOutput,// = 2,
    BEParameterDirectionInputOUtput		= 3,//adParamInputOutput,// = 3,
    BEParameterDirectionReturnValue		= 4//adParamReturnValue// = 4
};
/*****************************************************************************************
	<< --- toStringBEParameterDirection		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	dir = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toString(BEParameterDirection dir)
{
	switch (dir)
	{
	case BEParameterDirectionUnknown:			//= 0,		// �� ϵͳ����CM_NONE = 0
		return L"BEParameterDirectionUnknown";
		break;
	case BEParameterDirectionInput:			//= 1,		// �Ǵ�
		return L"BEParameterDirectionInput";
		break;
	case BEParameterDirectionOutput:			//= 2,		// ���
		return L"BEParameterDirectionOutput";
		break;
	case BEParameterDirectionInputOUtput:			//= 3,		// ��ƽ��
		return L"BEParameterDirectionInputOUtput";
		break;
	case BEParameterDirectionReturnValue:			//= 4,		// ���ֵ
		return L"BEParameterDirectionReturnValue";
		break;
	default:
		break;
	}
	return L"";
}
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
typedef enum BEAlignMode{
	BEAlignModeAllLeft			= DT_LEFT|DT_SINGLELINE|DT_VCENTER,
	BEAlignModeAllRight			= DT_RIGHT|DT_SINGLELINE|DT_VCENTER,
	BEAlignModeAllCenter		= DT_CENTER|DT_SINGLELINE|DT_VCENTER,
	BEAlignModeLeftTop			= DT_LEFT|DT_TOP,			// ͼƬ���������
	BEAlignModeLeftCenter		= DT_LEFT|DT_VCENTER,		// ͼƬ�м������
	BEAlignModeLeftBottom		= DT_LEFT|DT_BOTTOM,		// ͼƬ�ײ������
	BEAlignModeCenterTop		= DT_CENTER|DT_TOP,			// ͼƬ��������
	BEAlignModeCenterCenter		= DT_CENTER|DT_VCENTER,		// ͼƬ�м����
	BEAlignModeCenterBottom		= DT_CENTER|DT_BOTTOM,		// ͼƬ�ײ�����
	BEAlignModeRightTop			= DT_RIGHT|DT_TOP,			// ͼƬ�����Ҷ���
	BEAlignModeRightCenter		= DT_RIGHT|DT_VCENTER,		// ͼƬ�м��Ҷ���
	BEAlignModeRightBottom		= DT_RIGHT|DT_BOTTOM		// ͼƬ�ײ��Ҷ���
}BEAlignMode;
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// ��Ԫ��ı߿���ʽ
typedef enum BEBorderStyle {
	BEBorderStyleNone			= 0x0,			// �ޱ߽��
	BEBorderStyleTop			= 0x1,			// ���ϱ߿�
	BEBorderStyleBottom			= 0x2,			// ���±߿�
	BEBorderStyleLeft			= 0x4,			// ����߿�
	BEBorderStyleRight			= 0x8,			// ���ұ߿�
	BEBorderStyleTopBottom		= 0x3,			// ���±߿�
	BEBorderStyleLeftRight		= 0xC,			// ���ұ߿�
	BEBorderStyleRectangle		= 0xF			// ���α߿�

}BEBorderStyle;

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
// �ҵļ��㷽ʽ
enum BEComputeMethod{
	BEComputeMethodNone			= 0,		// �� ϵͳ����CM_NONE = 0
	BEComputeMethodCount		= 1,		// �Ǵ�
	BEComputeMethodSum			= 2,		// ���
	BEComputeMethodAvg			= 3,		// ��ƽ��
	BEComputeMethodMax			= 4,		// ���ֵ
	BEComputeMethodMin			= 5			// ��Сֵ
};
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// ��ϵ�����
enum BERelationalOptor{
	BERelationalOptorNone				= 0,		// δ�����
	BERelationalOptorEquals				= 1,		// ����
	BERelationalOptorNotEqual			= 2,		// ������
	BERelationalOptorLessthan			= 3,		// С��
	BERelationalOptorLessthanOrEqual	= 4,		// С�ڵ���
	BERelationalOptorGreaterthan		= 5,		// ����
	BERelationalOptorGreaterthanOrEqual = 6			// ���ڵ���
};
/*****************************************************************************************
	<< --- BERelationalOptor		2013-11-22 --- >>
	˵������ö��ֵת���ɶ�Ӧ���ַ�����
	������
	relational = ��ת����ö��ֵ
	����ֵ������ö���͵��ַ�����
*****************************************************************************************/
static std::wstring toStirng(BERelationalOptor relational)
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// �߼������
enum BELogicOptor{
	BELogicOptorNone			= 0,		// δ�����
	BELogicOptorAnd				= 1,		// �߼���
	BELogicOptorOr				= 2,		// ��
	BELogicOptorNot				= 3			// ��
};

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
////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// ϵͳ֧�ֵ����ݿ�����
enum BEDBType{
	BEDBTypeMSSql = 1,		// MSSQL
	BEDBTypeOracle,			// 
	BEDBTypeAccess,			// 
	BEDBTypeSqlite3			// 
};
/*****************************************************************************************
	<< --- toStringBEDBType		2013-11-18 --- >>
	˵��������BEDBType�����ַ�������ʽ
	������
	dbtype	= ���ݿ�����
	����ֵ�� BEDBType�����ַ�������ʽ
*****************************************************************************************/
static std::wstring toString(BEDBType dbtype)
{
	switch(dbtype)
	{
	case BEDBTypeMSSql: //= 1,		// MSSQL
		return L"BEDBTypeMSSql";
		break;
	case BEDBTypeOracle: //,			// 
		return L"BEDBTypeOracle";
		break;
	case BEDBTypeAccess: //,			// 
		return L"BEDBTypeAccess";
		break;
	case BEDBTypeSqlite3: //			// 
		return L"BEDBTypeSqlite3";
		break;
	}
	return L"";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ģʽ
enum BEDBCmdType{
	BEDBCmdTypeText = 1,//adCmdText,	// 
	BEDBCmdTypeProc = 4	//adCmdStoredProc
};
/*****************************************************************************************
	<< --- toStringBEDBCmdType		2013-11-18 --- >>
	˵��������BEDBType�����ַ�������ʽ
	������
	cmdtype	= ���ݿ�����
	����ֵ�� BEDBCmdType�����ַ�������ʽ
*****************************************************************************************/
static std::wstring toString(BEDBCmdType cmdtype)
{
	switch(cmdtype)
	{
	case BEDBCmdTypeText: //= 1,		// MSSQL
		return L"BEDBCmdTypeText";
		break;
	case BEDBCmdTypeProc: //,			// 
		return L"BEDBCmdTypeProc";
		break;
	}
	return L"";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// һ��������CComboBox CListBox�ؼ���Ӹ�����
typedef enum BEAuxiliaryItem
{
	BEAuxiliaryItemNone		= 0x0,	// �޸�����
	BEAuxiliaryItemAll		= 0x1,	// �����ȫ��������Ӧ��ItemData=-1
	BEAuxiliaryItemWu		= 0x2,	// ������ޡ�����Ӧ��ItemData=-2
	BEAuxiliaryItemNull		= 0x4	// �����Ϊ�ա�����Ӧ��ItemData=-3
}BEAuxiliaryItem;

// ��
typedef struct BSPoint
{
	int32_t x;	// ָ����� X ����
	int32_t y;	// ָ����� Y ����
public:
	BSPoint()
		: x(0), y(0)
	{}
	BSPoint(int32_t vx, int32_t vy)
		: x(vx), y(vy)
	{
	}
	BSPoint(const BSPoint &point)
		: x(point.x), y(point.y)
	{
	}
#if defined(_AFXDLL)
	BSPoint(const CPoint &point)
		: x(point.x), y(point.y)
	{
	}
#endif
	BSPoint& operator=(const BSPoint &point)
	{
		if(this != &point)
		{
			x = point.x;
			y = point.y;
		}
		return *this;
	}
	~BSPoint() 
	{
	}
public:
#if defined(_AFXDLL)
	CPoint toMSPoint() const {
		CPoint pt;
		pt.x = x;
		pt.y = y;
		return pt;
	}
	Gdiplus::Point toGdiplus() const {
		return Gdiplus::Point(x, y);
	}
	Gdiplus::PointF toGdiplusF() const {
		return Gdiplus::PointF(x, y);
	}
	// ƫ�ƶ���vx>0���ң�vy>0����
	void offset(int32_t offx, int32_t offy)
	{
#if defined(WORK_AXIS_CAD)
		x += offx;
		y -= offy;
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		x += offx;
		y += offy;
#endif
	}
#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & x;
//		ar & y;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & x;
//		ar & y;
//    }
//    BOOST_SERIALIZATION_SPLIT_MEMBER()	

}BSPoint;
// �����С
typedef struct BSSize
{
	int32_t cx;		// �����
	int32_t cy;		// �����
public:
	BSSize() : cx(0), cy(0)
	{
	}
	BSSize(int32_t width, int32_t height)
		: cx(width), cy(height)
	{
	}
	BSSize(const BSSize &size)
		: cx(size.cx), cy(size.cy)
	{
	}
	BSSize& operator=(const BSSize &size)
	{
		if(this != &size)
		{
			cx = size.cx;
			cy = size.cy;
		}
		return *this;
	}
	~BSSize()
	{
	}
public:
#if defined(_AFXDLL)
	CSize toMSSize() const {
		CSize sz;
		sz.cx = cx;
		sz.cy = cy;
		return sz;
	}
	Gdiplus::Size toGdiplus() const {
		return Gdiplus::Size(cx, cy);
	}
	Gdiplus::SizeF toGdiplusF() const {
		return Gdiplus::SizeF(cx, cy);
	}

	BSSize(const CSize &size)
		: cx(size.cx), cy(size.cy)
	{
	}
	BSSize& operator=(const CSize &size)
	{
		cx = size.cx;
		cy = size.cy;
		return *this;
	}
#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & cx;
//		ar & cy;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & cx;
//		ar & cy;
//    }
//    BOOST_SERIALIZATION_SPLIT_MEMBER()	


}BSSize;

// ������ε����ϽǺ����½ǵ�����
typedef struct BSRect
{
	int32_t left;		// ָ�����ε����Ͻǵ� x ����
	int32_t top;		// ָ�����ε����Ͻǵ� y ����
	int32_t right;		// ָ�����ε����½ǵ� x ����
	int32_t bottom;	// ָ�����ε����½ǵ� y ����
public:
	BSRect()
		: left(0), top(0), right(0), bottom(0)
	{
	}
	BSRect(BSPoint point, BSSize size)
		: left(point.x), top(point.y), right(left+size.cx)
	{
#if defined(WORK_AXIS_CAD)
		bottom = top-size.cy;
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		bottom = top+size.cy;
#endif
	}
	BSRect(int32_t vleft, int32_t vtop, int32_t vright, int32_t vbottom)
		: left(vleft), top(vtop), right(vright), bottom(vbottom)
	{
	}
	BSRect(const BSRect &rect)
		: left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
	{
	}
	BSRect& operator=(const BSRect &rect)
	{
		if(this != &rect)
		{
			left = rect.left;
			top = rect.top;
			right = rect.right;
			bottom = rect.bottom;
		}
		return *this;
	}
	~BSRect() {}
public:
	void setRect(int32_t vleft, int32_t vtop, int32_t vright, int32_t vbottom)
	{
		left = vleft;
		top = vtop;
		right = vright;
		bottom = vbottom;
	}
	void setRect(BSPoint point, BSSize size)
	{
		left = point.x;
		top = point.y;
		right = left + size.cx;
		bottom = top + size.cy;
		//#if defined(WORK_AXIS_CAD)
//		right = left + size.cx;
//		bottom = top - size.cy;
//#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
//		right = left + size.cx;
//		bottom = top + size.cy;
//#else
//		right = left + size.cx;
//		bottom = top + size.cy;
//#endif
	}
	BSPoint topLeft() const
	{
		return BSPoint(left, top);
	}
	BSPoint bottomRight() const
	{
		return BSPoint(right, bottom);
	}
	BSPoint center() const
	{
#if defined(WORK_AXIS_CAD)
		return BSPoint(left+width()/2, top-height()/2);
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		return BSPoint(left+width()/2, top+height()/2);
#else
		return BSPoint(left+width()/2, top+height()/2);
#endif
	}
	int32_t width() const
	{
		return std::abs(right-left);
	}
	int32_t height() const
	{
		return std::abs(bottom-top);
	}
	// �Ƿ�Ϊ��
	bool isEmpty() const 
	{
		return left==right && top==bottom;
	}
	// ��ת90��
	void rotate90() {
		BSPoint pt = center();
		BSRect rt;
		rt.left = rt.right = pt.x;
		rt.top = rt.bottom = pt.y;
		rt.inflateRect(height()/2, width()/2);
		operator=(rt);
	}

	// �淶�� 
	void normalizeRect()
	{
#if defined(WORK_AXIS_CAD)
		if(left > right)
			std::swap(left, right);
		if(bottom > top)
			std::swap(bottom, top);
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		if(left > right)
			std::swap(left, right);
		if(top > bottom)
			std::swap(top, bottom);
#else
		if(left > right)
			std::swap(left, right);
		if(top > bottom)
			std::swap(top, bottom);
#endif
	}
	// �鿴���Ƿ��ڶ�����
	bool ptInRect(const BSPoint &pt)
	{
		normalizeRect();
#if defined(WORK_AXIS_CAD)
		return (pt.x >= left && pt.x <= right) && (pt.y >= bottom && pt.y <= top);
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		return (pt.x >= left && pt.x <= right) && (pt.y >= top && pt.y <= bottom);
#else
		return (pt.x >= left && pt.x <= right) && (pt.y >= top && pt.y <= bottom);
#endif
	}
	bool rtInRect(BSRect &rt)
	{
		rt.normalizeRect();
		return ptInRect(rt.topLeft()) && ptInRect(rt.bottomRight());
	}
	// ƫ�ƶ���vx>0���ң�vy>0����
	void offsetRect(int32_t vx, int32_t vy)
	{
#if defined(WORK_AXIS_CAD)
		left += vx;
		right += vx;
		top -= vy;
		bottom -= vy;
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		left += vx;
		right += vx;
		top += vy;
		bottom += vy;
#else
		left += vx;
		right += vx;
		top += vy;
		bottom += vy;
#endif
	}
	// ��չ���� >0���� <0��С
	void inflateRect(int32_t vleft, int32_t vtop, int32_t vright, int32_t vbottom)
	{
#if defined(WORK_AXIS_CAD)
		left -= vleft;
		right += vright;
		top += vtop;
		bottom -= vbottom;
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		left -= vleft;
		right += vright;
		top -= vtop;
		bottom += vbottom;
#else
		left -= vleft;
		right += vright;
		top -= vtop;
		bottom += vbottom;
#endif
	}
	void inflateRect(int32_t vx, int32_t vy){
		inflateRect(vx, vy, vx, vy);
	}
	// ��չ���� >0��С <0����
	void deflateRect(int32_t vleft, int32_t vtop, int32_t vright, int32_t vbottom)
	{
#if defined(WORK_AXIS_CAD)
		left += vleft;
		right -= vright;
		top -= vtop;
		bottom += vbottom;
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		left += vleft;
		right -= vright;
		top += vtop;
		bottom -= vbottom;
#else
		left += vleft;
		right -= vright;
		top += vtop;
		bottom -= vbottom;
#endif
	}
	void deflateRect(int32_t vx, int32_t vy){
		deflateRect(vx, vy, vx, vy);
	}
public:
	// ����
	BSRect operator&(const BSRect &rect) 
	{
		BSRect rt, rt1(rect);
		rt1.normalizeRect();
		BSRect rt2(*this);
		rt2.normalizeRect();
#if defined(WORK_AXIS_CAD)
		rt.setRect(max(rt1.left,rt2.left), min(rt1.top,rt2.top),
			min(rt1.right,rt2.right), max(rt1.bottom,rt2.bottom));
		if(rt.left > rt.right || rt.top < rt.bottom)
			rt.setRect(0,0,0,0);
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		rt.setRect(max(rt1.left,rt2.left), max(rt1.top,rt2.top),
			min(rt1.right,rt2.right), min(rt1.bottom,rt2.bottom));
		if(rt.left > rt.right || rt.top > rt.bottom)
			rt.setRect(0,0,0,0);
#else
		rt.setRect(max(rt1.left,rt2.left), max(rt1.top,rt2.top),
			min(rt1.right,rt2.right), min(rt1.bottom,rt2.bottom));
		if(rt.left > rt.right || rt.top > rt.bottom)
			rt.setRect(0,0,0,0);
#endif
		return rt;
	}
	void operator&=(const BSRect &rect) 
	{
		*this = *this & rect;
	}
public:
#if defined(_AFXDLL)
	CRect toMSRect() const {
		CRect rt;
		rt.SetRect(left, top, right, bottom);
		return rt;
	}
	Gdiplus::Rect toGdiplus() const {
#if defined(WORK_AXIS_CAD)
		return Gdiplus::Rect(left, bottom, width(), height());
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		return Gdiplus::Rect(left, top, width(), height());
#endif
	}
	Gdiplus::RectF toGdiplusF() const {
#if defined(WORK_AXIS_CAD)
		return Gdiplus::RectF(left, bottom, width(), height());
#elif defined(WORK_AXIS_DOC) || defined(WORK_AXIS_NORMAL)
		return Gdiplus::RectF(left, top, width(), height());
#endif
	}
	void setRect(RECT rt) {
		left = rt.left;
		right = rt.right;
		top = rt.top;
		bottom = rt.bottom;
		//normalizeRect();
	}

#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & left;
//		ar & top;
//		ar & right;
//		ar & bottom;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & left;
//		ar & top;
//		ar & right;
//		ar & bottom;
//    }
//    BOOST_SERIALIZATION_SPLIT_MEMBER()	

}BSRect;

// ��������
typedef struct BSPen
{

	BEPenStyle	style;	// ������ʽ
	uint32_t	width;	// �߼���λ,���
	uint32_t	color;	// ������ɫ
public:
	BSPen() : style(BEPenStyleSolid), width(3), color(0xFF000000) {}
	BSPen(BEPenStyle vstyle, uint32_t vwidth, uint32_t vcolor)
		: style(vstyle), width(vwidth), color(vcolor)
	{
	}
	BSPen(const BSPen &pen)
		: style(pen.style), width(pen.width), color(pen.color)
	{
	}
	BSPen& operator=(const BSPen &pen)
	{
		if(this != &pen)
		{
			style = pen.style;
			width = pen.width;
			color = pen.color;
		}
		return *this;
	}
	~BSPen()
	{
	}
public:
#if defined(_WIN32)
	LOGPEN toMSPen() const {
		LOGPEN lgpen;
		lgpen.lopnColor = color;
		lgpen.lopnStyle = style;
		lgpen.lopnWidth.x = lgpen.lopnWidth.y = width;
		return lgpen;
	}
	/*****************************************************************************************
		<< --- BSPen::toGdiplus	2014-01-09 --- >>
		˵���������Զ��廭�ʽṹ���ɻ���Gdiplus::Pen*
		������
		����ֵ�����ݻ�����ʽ����Gdiplus::Pen*�������߸����ͷ�
	*****************************************************************************************/
	Gdiplus::Pen* toGdiplus() const {
		Gdiplus::Pen *ppen = new Gdiplus::Pen(color, width);
		if(style == BEPenStyleNull)
			ppen->SetColor(color & 0x00FFFFFF);	// ͸��
		ppen->SetDashStyle(Gdiplus::DashStyle(style));
		return ppen;
	}

#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & style;
//		ar & width;
//		ar & color;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & style;
//		ar & width;
//		ar & color;
//    }
    //BOOST_SERIALIZATION_SPLIT_MEMBER()	
}BSPen;

// ������һ��ʵ�ʻ�ˢ����ʽ����ɫ��ģʽ
typedef struct BSBrush
{
	BEBrushStyle	style;		// ��ˢ��ʽ
	uint32_t		backColor;	// ��ˢ���������ɫ
	uint32_t		foreColor;	// ��ˢǰ��ɫ
	BEHatchStyle	hatch;		// ��Ӱ��ʽ
public:
	BSBrush() : style(BEBrushStyleNull), backColor(0xFFFFFF), foreColor(0xFF000000), hatch(BEHatchStyleHorizontal)
	{
	}
	BSBrush(BEBrushStyle vstyle, uint32_t vforeColor, uint32_t vbackColor, BEHatchStyle vhatch)
		: style(vstyle), foreColor(vforeColor), backColor(vbackColor), hatch(vhatch)
	{
	}
	BSBrush(uint32_t vbackColor)
		: style(BEBrushStyleSolid), foreColor(vbackColor), backColor(vbackColor), hatch(BEHatchStyleHorizontal)
	{
	}
	BSBrush(const BSBrush &brush)
		: style(brush.style), foreColor(brush.foreColor), backColor(brush.backColor), hatch(brush.hatch)
	{
	}
	BSBrush& operator=(const BSBrush &brush)
	{
		if(this != &brush)
		{
			style = brush.style;
			foreColor = brush.foreColor;
			backColor = brush.backColor;
			hatch = brush.hatch;
		}
		return *this;
	}
	~BSBrush()
	{
	}
public:
#if defined(_WIN32)
	LOGBRUSH toMSBrush() const {
		LOGBRUSH lgbrush;
		lgbrush.lbColor = foreColor;
		lgbrush.lbHatch = hatch;
		lgbrush.lbStyle = style;
		return lgbrush;
	}
	/*****************************************************************************************
		<< --- BSBrush::toGdiplus	2014-01-09 --- >>
		˵���������Զ��廭ˢ�ṹ���ɻ���Gdiplus::Brush
		������
		����ֵ�����ݻ�ˢ��ʽ����SolidBrush��HatchBrush�λ�ˢ�������߸����ͷ�
	*****************************************************************************************/
	Gdiplus::Brush* toGdiplus() const {
		Gdiplus::Brush *pbrush = nullptr;
		switch (style)
		{
		case BEBrushStyleSolid:
			pbrush = new Gdiplus::SolidBrush(backColor);
			break;
		case BEBrushStyleNull:
			pbrush = new Gdiplus::SolidBrush(Gdiplus::Color::Transparent);// backColor & 0x00FFFFFF);
			break;
		case BEBrushStyleHatch:
			pbrush = new Gdiplus::HatchBrush(Gdiplus::HatchStyle(hatch), foreColor, backColor);
			break;
		default:
			break;
		}
		return pbrush;
	}

#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & style;
//		ar & foreColor;
//		ar & backColor;
//		ar & hatch;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & style;
//		ar & foreColor;
//		ar & backColor;
//		ar & hatch;
//    }
//    BOOST_SERIALIZATION_SPLIT_MEMBER()	
}BSBrush;
// �������
typedef enum BEFontSize {
	BEFontSizeZero=0,BEFontSizeZeroSmall,BEFontSizeOne,BEFontSizeOneSmall,BEFontSizeTwo,BEFontSizeTwoSmall,
	BEFontSizeThree,BEFontSizeThreeSmall,BEFontSizeFour,BEFontSizeFourSmall,BEFontSizeFive,BEFontSizeFiveSmall,
	BEFontSizeSix,BEFontSizeSixSmall,BEFontSizeSeven,BEFontSizeEight
}BEFontSize;

// ����ṹ
typedef struct BSFont
{
#define FACENAME_MAX 32
	int32_t	iHeight;		// ���߼���λָ�������ַ�Ԫ(character cell)���ַ��ĸ߶�
	int32_t	iWidth;			// ���߼���λָ�������ַ���ƽ�����
	int32_t	iEscapement;	// ��ʮ��֮һ��Ϊ��λָ��ÿһ���ı����ʱ�����ҳ��׶˵ĽǶ�
	int32_t	iOrientation;	// ��ʮ��֮һ��Ϊ��λָ���ַ����������ҳ��׶˵ĽǶ�
	int32_t	iWeight;		// ָ��������������Windows�У��������������������ָ������Ĵ�ϸ�̶ȡ�
							//     lfWeight�ķ�ΧΪ0��1000����������µ���������Ϊ400������Ϊ700�����lfWeightΪ0����ʹ��Ĭ�ϵ���������
	bool	bItalic;		// ΪTRUEʱʹ��б��
	bool	bUnderline;	// ΪTRUEʱ����������»���
	bool	bStrikeOut;	// ΪTRUEʱ���������ɾ����
	uint8_t nCharSet;		// ָ���ַ���
	uint8_t nOutPrecision;	// ָ���������
	uint8_t nClipPrecision;	// ָ����������
	uint8_t nQuality;		// �����������
	uint8_t nPitchAndFamily;// ָ��������ַ�������
	wchar_t wcFaceName[FACENAME_MAX];// ��������	
public:
	BSFont()
		: iHeight(-20), iWidth(0), iEscapement(0), iOrientation(0), iWeight(400)
		, bItalic(false), bUnderline(false), bStrikeOut(false)
		, nCharSet(0/*ANSI_CHARSET*/), nOutPrecision(0/*OUT_DEFAULT_PRECIS*/), nClipPrecision(0/*CLIP_DEFAULT_PRECIS*/)
		, nQuality(0/*DEFAULT_QUALITY*/), nPitchAndFamily(0/*DEFAULT_PITCH*/)
	{
		std::swprintf(wcFaceName, FACENAME_MAX, L"%s", L"����");
	}
	BSFont(BEFontSize fontSize)
		: iWidth(0), iEscapement(0), iOrientation(0), iWeight(400)
		, bItalic(false), bUnderline(false), bStrikeOut(false)
		, nCharSet(0/*ANSI_CHARSET*/), nOutPrecision(3/*OUT_STROKE_PRECIS*/), nClipPrecision(2/*CLIP_STROKE_PRECIS*/)
		, nQuality(1/*DRAFT_QUALITY*/), nPitchAndFamily(2/*VARIABLE_PITCH*/)
	{
		std::swprintf(wcFaceName, FACENAME_MAX, L"%s", L"����");
		setFontSize(fontSize);
	}

	BSFont(const std::wstring &fontname, BEFontSize fontSize = BEFontSizeFourSmall)
		: iWidth(0), iEscapement(0), iOrientation(0), iWeight(400)
		, bItalic(false), bUnderline(false), bStrikeOut(false)
		, nCharSet(0/*ANSI_CHARSET*/), nOutPrecision(0/*OUT_DEFAULT_PRECIS*/), nClipPrecision(0/*CLIP_DEFAULT_PRECIS*/)
		, nQuality(0/*DEFAULT_QUALITY*/), nPitchAndFamily(0/*DEFAULT_PITCH*/)
	{
		std::swprintf(wcFaceName, FACENAME_MAX, L"%s", fontname.c_str());
		setFontSize(fontSize);
	}
	BSFont(const BSFont &font)
		: iHeight(font.iHeight), iWidth(font.iWidth), iEscapement(font.iEscapement), iOrientation(font.iOrientation), iWeight(font.iWeight)
		, bItalic(font.bItalic), bUnderline(font.bUnderline), bStrikeOut(font.bStrikeOut)
		, nCharSet(font.nCharSet), nOutPrecision(font.nOutPrecision), nClipPrecision(font.nClipPrecision)
		, nQuality(font.nQuality), nPitchAndFamily(font.nPitchAndFamily)
	{
		std::swprintf(wcFaceName, FACENAME_MAX, L"%s", font.wcFaceName);
	}
	BSFont& operator=(const BSFont &font)
	{
		if(this != &font)
		{
			iHeight = font.iHeight;
			iWidth = font.iWidth;
			iEscapement = font.iEscapement;
			iOrientation = font.iOrientation;
			iWeight = font.iWeight;
			bItalic = font.bItalic;
			bUnderline = font.bUnderline;
			bStrikeOut = font.bStrikeOut;
			nCharSet = font.nCharSet;
			nOutPrecision = font.nOutPrecision;
			nClipPrecision = font.nClipPrecision;
			nQuality = font.nQuality;
			nPitchAndFamily = font.nPitchAndFamily;
			std::swprintf(wcFaceName, FACENAME_MAX, L"%s", font.wcFaceName);
		}
		return *this;
	}
	~BSFont()
	{
	}
public:
	std::vector<uint8_t> toData() {
		uint8_t *pbuf = (uint8_t*)this;
		std::vector<uint8_t> vec(pbuf, pbuf+sizeof(BSFont));
		return vec;
	}
	bool fromData(std::vector<uint8_t> vecData) {
		if(vecData.size() != sizeof(BSFont))
			return false;
		BSFont font;
		std::memcpy((void*)&font, vecData.data(), sizeof(BSFont));
		*this = font;
		return true;
	}
	void setFontSize(BEFontSize fontSize) {
		switch (fontSize)
		{
		case BEFontSizeZero:
			iHeight = -70;
			break;
		case BEFontSizeZeroSmall:
			iHeight = -60;
			break;
		case BEFontSizeOne:
			iHeight = -43;
			break;
		case BEFontSizeOneSmall:
			iHeight = -40;
			break;
		case BEFontSizeTwo:
			iHeight = -37;
			break;
		case BEFontSizeTwoSmall:
			iHeight = -30;
			break;
		case BEFontSizeThree:
			iHeight = -27;
			break;
		case BEFontSizeThreeSmall:
			iHeight = -25;
			break;
		case BEFontSizeFour:
			iHeight = -23;
			break;
		case BEFontSizeFourSmall:
			iHeight = -20;
			break;
		case BEFontSizeFive:
			iHeight = -18;
			break;
		case BEFontSizeFiveSmall:
			iHeight = -15;
			break;
		case BEFontSizeSix:
			iHeight = -13;
			break;
		case BEFontSizeSixSmall:
			iHeight = -11;
			break;
		case BEFontSizeSeven:
			iHeight = -9;
			break;
		case BEFontSizeEight:
			iHeight = -8;
			break;
		default:
			iHeight = -20;
			break;
		}
	}

#if defined(_AFXDLL)
	void toMSFont(LOGFONT *plgfont, CDC *pdc = nullptr) const {
		if(plgfont){
			plgfont->lfHeight = -iHeight;
			plgfont->lfWidth = iWidth;
			plgfont->lfEscapement = iEscapement;
			plgfont->lfWeight = iWeight;
			plgfont->lfItalic = bItalic;
			plgfont->lfUnderline = bUnderline;
			plgfont->lfStrikeOut = bStrikeOut;
			plgfont->lfCharSet	= nCharSet;
			plgfont->lfOutPrecision = nOutPrecision;
			plgfont->lfClipPrecision = nClipPrecision;
			plgfont->lfQuality = nQuality;
			plgfont->lfPitchAndFamily = nPitchAndFamily;
			std::swprintf(plgfont->lfFaceName, LF_FACESIZE, L"%s", wcFaceName);
#if defined(WORK_AXIS_CAD) || defined(WORK_AXIS_DOC)	// ���ҡ��ϵ�����1�߼���λ����0.1����
			plgfont->lfHeight *=254/72; 
#else
			if(pdc)
				plgfont->lfHeight = -MulDiv(plgfont->lfHeight, pdc->GetDeviceCaps(LOGPIXELSY), 72);
#endif

		}

	}

#endif
//private:
//	friend class boost::serialization::access;
//    template<class Archive>
//    void save(Archive & ar, const unsigned int version) const
//    {
//        // ע�⣬����ʱ���Ǳ������°汾
//		ar & iHeight;
//		ar & iWidth;
//		ar & iEscapement;
//		ar & iOrientation;
//		ar & iWeight;
//		ar & bItalic;
//		ar & bUnderline;
//		ar & bStrikeOut;
//		ar & nCharSet;
//		ar & nOutPrecision;
//		ar & nClipPrecision;
//		ar & nQuality;
//		ar & nPitchAndFamily;
//		ar & wcFaceName;
//    }
//    template<class Archive>
//    void load(Archive & ar, const unsigned int version)
//    {
//		ar & iHeight;
//		ar & iWidth;
//		ar & iEscapement;
//		ar & iOrientation;
//		ar & iWeight;
//		ar & bItalic;
//		ar & bUnderline;
//		ar & bStrikeOut;
//		ar & nCharSet;
//		ar & nOutPrecision;
//		ar & nClipPrecision;
//		ar & nQuality;
//		ar & nPitchAndFamily;
//		ar & wcFaceName;
//    }
//    BOOST_SERIALIZATION_SPLIT_MEMBER()	


}BSFont;
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- BSBTClassNode	2013-12-12 --- >>
//	����һ����̳й�ϵ��B�������£�
//		depth:0				classA
//							/    \
//		depth:1			classB	 classC
//						/		  /	  |	\	\
//		depth:2		classD	   classE F  G   H
//						|		|
//		depth:3		classI	   classJ
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct BSBTClassNode {
	std::string						className;
	int32_t							classVersion;
	int32_t							classType;		// �����ͣ�����������Ψһkey
	int32_t							classDepth;		// ��̳����
	BSBTClassNode					*pparentClass;	// ����
	std::vector<BSBTClassNode *>	vecSubClass;	// ����
	BSBTClassNode(const char *pname, int32_t type, int32_t depth, BSBTClassNode *pnode, BSBTClassNode *pparentNode)
		: className(pname?pname:""), classType(type), classDepth(depth), pparentClass(pparentNode)
	{
		if(pnode && pparentClass)
			pparentClass->vecSubClass.push_back(pnode);
	}
	//// ��������
	//BSBTClassNode *getNodeByDepth(int32_t depth) {
	//	if(depth > classDepth)
	//		return nullptr;
	//	BSBTClassNode *pnode = this;
	//	while(pnode)
	//	{
	//		if(pnode->classDepth == depth)
	//			return pnode;
	//		pnode = pnode->pparentClass;
	//	}
	//	return pnode;
	//}
	///*****************************************************************************************
	//	<< --- static findCommunityBasecalss	2013-12-12 --- >>
	//	˵��������ָ��������Ĺ�����������
	//	������
	//	pnode1	= ���ȽϽڵ�1
	//	pnode2	= ���ȽϽڵ�2
	//	����ֵ����������������
	//*****************************************************************************************/
	//static BSBTClassNode* findCommunityBasecalss(BSBTClassNode* pnode1, BSBTClassNode *pnode2)
	//{
	//	// �ڵ����ྲ̬�����ģ�����ͬһ����Ľڵ��ַֻ��һ��
	//	if(pnode1 == pnode2)
	//		return pnode1;
	//	if(pnode1 && pnode2)
	//	{
	//		// ��λ��ͬһ���
	//		if(pnode1->classDepth < pnode2->classDepth)
	//			pnode2 = pnode2->getNodeByDepth(pnode1->classDepth);
	//		else if(pnode1->classDepth > pnode2->classDepth)
	//			pnode1 = pnode1->getNodeByDepth(pnode2->classDepth);
	//		while(pnode1 && pnode2)
	//		{
	//			if(pnode1 == pnode2)
	//				return pnode1;
	//			pnode1 = pnode1->pparentClass;
	//			pnode2 = pnode2->pparentClass;
	//		}
	//	}
	//	return nullptr;
	//}
}BSBTClassNode;
// �ඨ���ļ���ʹ��
#define _DECLARE_BTNODE_(class_name) \
public: \
	static BSBTClassNode class##class_name; \
public: \
	virtual BSBTClassNode* getBTClassNode() const { \
		return &class##class_name; \
	} \
	virtual int32_t getType(int32_t depth) const { /*������ȵõ����������*/\
		BSBTClassNode *pnode = &class##class_name; \
		while(pnode) \
		{ \
			if(pnode->classDepth == depth) \
				return pnode->classType; \
			pnode = pnode->pparentClass; \
		} \
		return -1; \
	} \
	virtual int32_t getType() const { \
		return class##class_name.classType; \
	}
// ��ʵ���ļ���ʹ�ã�������ʹ��
#define _IMPLEMENT_ROOT_BTNODE_(class_root, class_type) \
	BSBTClassNode class_root::class##class_root = BSBTClassNode(#class_root, class_type, 0, nullptr, nullptr, class_root::allocSharePropertySectionArray); 
// ��ʵ���ļ���ʹ�ã���������ʹ��
#define _IMPLEMENT_BTNODE_(class_name, base_class_name, class_type) \
	BSBTClassNode class_name::class##class_name = BSBTClassNode( \
	#class_name, class_type, base_class_name::class##base_class_name.classDepth+1, \
	&class_name::class##class_name, &base_class_name::class##base_class_name, class_name::allocSharePropertySectionArray); 



} // end gtc


#endif