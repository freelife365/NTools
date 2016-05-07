#pragma once
#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__
#include "AC/basedef.h"
#include "AC/singleton.h"

namespace gtc
{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	logManager��	2015-1-15
//	���õ�ʵ������־������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class logManager : public singleton<logManager>
{
public:
	// ������singleton�Ķ���
	static logManager& getSingleton();
	static logManager* getSingletonPtr();

};

}

#endif