#ifndef __DBDEF_H__
#define __DBDEF_H__
#pragma once

namespace gtc{

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

}
#endif