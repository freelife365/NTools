#ifndef __DEV_DEVDEF_H__
#define __DEV_DEVDEF_H__
#pragma once
#include "AC/sysupport.h"

namespace dev {

//////////////////////////////////////////////////////////////////////////
//	2016-1-11	�豸����
enum BEDeviceModel {
	BEDeviceModelNone		= 0,
	BEDeviceModelPort		,
};

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-1-28		xutong	���ذ���Ϣ
struct BSPackInfo {
	long	lMaterialID;		// ��Ӧ������
	float	flgWeightPack;		// Ŀ����ء��߼���λ��������ϵ��λһ��
	float	flgMarginError;		// Ŀ����ص���Χ���߼���λ��������ϵ��λһ��
	int		iPackCount;			// ���������
	int		iRealCount;			// ʵ�ʺϸ�Ĵ������
	int		iFailedCount;		// ���ϸ������
};

}	// dev

#endif	// __DEV_DEVDEF_H__
