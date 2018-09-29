#include "logManager.h"
#include "AC/baseException.h"
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	logManager��	2015-1-15
//	���õ�ʵ������־������
////////////////////////////////////////////////////////////////////////////////////////////////////////
template<> logManager* singleton<logManager>::m_pSingleton = NULL;

logManager& logManager::getSingleton()
{
	assert(m_pSingleton);
	return *m_pSingleton;
}
logManager* logManager::getSingletonPtr()
{
	return m_pSingleton;
}







} // end gtc