#ifndef __BASEENUM_H__
#define __BASEENUM_H__

///////////////////////////////////////////////////////////////////////////////////////////////////
// �Զ���ͨ����������
typedef enum BEDataType{
	//							VT_TYPE			STL						ADO.DataType			sqlite3
	BEDataTypeNone = 0,		// VT_EMPTY			��						adEmpty					SQLITE_NULL
	//BEDataTypeNULL,			// VT_NULL
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


//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-12 ������
typedef enum BEOrientationFlag {
	BEOrientationFlagNone			= 0,		// 
	BEOrientationFlagCenter			,			// ����
	BEOrientationFlagTop			,			// 
	BEOrientationFlagBottom			,			// 
	BEOrientationFlagLeft			,			// 
	BEOrientationFlagRight			,			// 
	BEOrientationFlagLeftTop		,			// 
	BEOrientationFlagLeftBottom		,			// 
	BEOrientationFlagRightTop		,			// 
	BEOrientationFlagRightBottom				// 

};


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


//////////////////////////////////////////////////////////////////////////
// ����ϵͳ�汾
typedef enum BEOSVersion {
	BEOSVersionNone				= 0,	// δ֪ϵͳ
	BEOSVersionXPHome			,		// Windows XP Home Edition
	BEOSVersionXPProfessional	,		// Windows XP Professional
	BEOSVersionXP64				,		// XP x64
	BEOSVersionServer2003		,		// Server 2003
	BEOSVersionServer2003R2		,		// Server 2003R2
	BEOSVersionVista			,		// Vista
	BEOSVersionWin7HomeBasic	,		// Windows 7 Home Basic
	BEOSVersionServer2008		,		// Server 2008
	BEOSVersionServer2008R2		,		// Server 2008 R2
	BEOSVersionWin8				,		// Windows 8
	BEOSVersionWin81			,		// Windows 8.1
	BEOSVersionServer2012		,		// Server 2012
	BEOSVersionServer2012R2		,		// Server 2012 R2
};

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
// �ҵļ��㷽ʽ
enum BEComputeMethod{
	BEComputeMethodNone			= 0,		// �� ϵͳ����CM_NONE = 0
	BEComputeMethodCount		= 1,		// �Ǵ�
	BEComputeMethodSum			= 2,		// ���
	BEComputeMethodAvg			= 3,		// ��ƽ��
	BEComputeMethodMax			= 4,		// ���ֵ
	BEComputeMethodMin			= 5			// ��Сֵ
};

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

///////////////////////////////////////////////////////////////////////////////////////////////////
// �߼������
enum BELogicOptor{
	BELogicOptorNone			= 0,		// δ�����
	BELogicOptorAnd				= 1,		// �߼���
	BELogicOptorOr				= 2,		// ��
	BELogicOptorNot				= 3			// ��
};

#endif	// __BASEENUM_H__