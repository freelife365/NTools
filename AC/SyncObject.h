#ifndef __SYNCOBJECT_H__
#define __SYNCOBJECT_H__
#pragma once
#include "sysupport.h"

#if defined(GTC_BOOST_THREAD)
#include "boost/thread.hpp"

//D:/NTools/boost/lib/libboost_thread-vc100-mt-gd-1_59.lib
//D:/NTools/boost/lib/libboost_date_time-vc100-mt-gd-1_59.lib
//D:/NTools/boost/lib/libboost_system-vc100-mt-gd-1_59.lib
//D:/NTools/boost/lib/libboost_chrono-vc100-mt-gd-1_59.lib
//
//#endif
#endif
//#define SYNC_OBJECT_LOG_ON		// �Ƿ���ʾ��־����

namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	criticalSection  ��		2015-7-14
//	��CRITICAL_SECTION��װ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class criticalSection
{
public:
	criticalSection();
	~criticalSection();

	bool tryEnter(PCWSTR pdesc = NULL);
	void enter(PCWSTR pdesc = NULL);
	void leave(PCWSTR pdesc = NULL);
private:
	CRITICAL_SECTION		m_cs;
	std::wstring			m_strPrevEntry;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	optex  ��		2015-5-26
//	����ͬ�����࣬�ɿ����(������)
////////////////////////////////////////////////////////////////////////////////////////////////////////
class optex
{
public:
	optex(DWORD dwSpinCount = 4000) 
	{
		commonConstructor(dwSpinCount, NULL);
	}
	optex(PCWSTR pName, DWORD dwSpinCount = 4000)
	{
		commonConstructor(dwSpinCount, pName);
	}
	~optex();
	void setSpinCount(DWORD dwSpinCount);
	void enter(PCWSTR pDesc = NULL);
	bool tryEnter(PCWSTR pDesc = NULL);
	void leave(PCWSTR pDesc = NULL);
	bool isSingleProcessOptex() const {
		return m_hFileMap == NULL;
	}

	void setDescription(PCWSTR pDesc);
	std::wstring getDescription() const;

	std::wstring toString() const;
private:
	void commonConstructor(DWORD dwSpinCount, PCWSTR pName);
private:
	// ���̼乲�������
	typedef struct {
		DWORD	dwSpinCount;	// ָ����ͼ����optex���߳��ڵȴ��¼��ں˶���֮ǰӦ�ó��Խ���Ĵ������ڵ�
								// ������������ϣ����ֵ����0
		long	lLockCount;		// ָ���߳���ͼ����optex�Ĵ��������û���߳̽���optex����ô���ֵ��0
		DWORD	dwProcessID;	// ����ID
		DWORD	dwThreadId;		// ָ��ӵ��optex���̵߳�ΨһID�����û���߳�ӵ��optex����ô���ֵ��0
		long	lRecurseCount;	// ָ���߳�ӵ��optex�Ĵ��������optexû�б��߳���ӵ�У������ֵ��0
	} SHAREDINFO, *PSHAREDINFO;
private:
	HANDLE			m_hEvent;	// ���Ǹ��¼��ں˶���ľ����ֻ�е�һ���߳���ͼ����һ���߳�ӵ��optexʱ��
								// ���optex����ʹ�����������ں˶��������������صľ��������Ǹó�Ա
								// Ϊʲô��ʹ��SHAREDINFO�ṹ��ԭ��
	HANDLE			m_hFileMap;	// �����ļ�ӳ���ں˶���ľ����������̹���һ��optexʱ����ʹ����������
								// �ں˶����������������صľ���������Ϊʲô�ó�Ա����SHAREDINFO��
								// ����ԭ�򡣶��ڵ�����optex��˵�����ֵ����NULL
	PSHAREDINFO		m_psi;		// ����ָ��Ǳ�ڵĹ���optex���ݳ�Ա��ָ�롣�ڴ��ַ���������صĵ�ַ����
								// ����Ϊʲô�ó�Ա��ʹ��SHAREDINFO�ṹ��ԭ�򡣶��ڵ�����optex��˵����
								// ָ��һ����ջ������ڴ�顣���ڶ����optex��˵����ָ��һ���ڴ�ӳ���ļ�
	std::wstring	m_wsDesc;	// 

	std::wstring	m_wsPrevEntry;
	static int mc_bUniprocessorHost;	// �Ƿ�Ϊ�������� 1���������� 0���ദ���� -1����ֵδ����
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	swmrg  ��		2015-5-26
//	����д�����/����Ķ�����ı���
//	4��ԭ��
//	1) ��һ���߳�����д������ʱ�������κ��̲߳���д�����ݡ�
//	2) ��һ���߳�����д������ʱ�������κ��̲߳��ܶ�ȡ���ݡ�
//	3) ��һ���߳����ڶ�ȡ����ʱ�������κ��̲߳���д�����ݡ�
//	4) ��һ���߳����ڶ�ȡ����ʱ�������߳�Ҳ�ܹ���ȡ���ݡ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class swmrg
{
public:
	swmrg(PCWSTR pDesc = NULL);
	~swmrg();

	void waitToRead(PCWSTR pDesc = NULL);        // ִ�ж�ȡ������Դ
	void waitToWrite(PCWSTR pDesc = NULL);       // ִ�ж�ȡ��д�빲����Դ��
	void done(PCWSTR pDesc = NULL);              // ���ٷ��ʹ�����Դʱ

	void setDescription(PCWSTR pDesc);
	std::wstring getDescription() const;

	std::wstring toString() const;
private:
	CRITICAL_SECTION	m_cs;				// ���ڱ������е�������Ա�����������������ǵĲ������ܹ���ԭ�Ӳ�����ʽ�����
	HANDLE				m_hsemReaders;		// ������̵߳���waitToRead����������m_nActivee��-1�����ܾ�����ʱ��
											// �����Ķ��߳̾��ȴ����űꡣ�����һ�����ڵȴ����Ķ��̵߳���doneʱ��
											// ���ű걻�ͷţ���������m_nWaitingReaders���Ӷ������������ڵȴ����Ķ��߳�
	HANDLE				m_hsemWriters;		// ���̵߳���waitToWrite����������m_nActive����0�����ܾ�����ʱ����
											// ��д���߳̾��ȴ����űꡣ��һ���߳����ڵȴ�ʱ�����Ķ��߳̽����ܾ�
											// ���ʸ���Դ������Է�ֹ�Ķ��߳�¢�ϸ���Դ�������һ��ӵ����Դ����
											// Ȩ���Ķ��̵߳���doneʱ�����ű�ͱ��ͷţ���������1���Ӷ�����һ����
											// �ڵȴ���д���߳�
	int					m_nWaitingReaders;	// ��ʾ��Ҫ������Դ���Ķ��̵߳���������ֵ����ʼ��Ϊ0����m_nActive
											// ��-1ʱ��ÿ���̵߳���һ��waitToRead����ֵ�͵���1
	int					m_nWaitingWriters;	// ��ʾ��Ҫ������Դ��д���̵߳���������ֵ����ʼ��Ϊ0����m_nActive��
											// ��0ʱ��ÿ���̵߳���һ��waitToWrite����ֵ�͵���1
	int					m_nActive;			// ���ڷ�ӳ������Դ�ĵ�ǰ״̬�������ֵ��0����ôû���߳��ڷ�����Դ��
											// �����ֵ����0�����ֵ���ڱ�ʾ��ǰ��ȡ����Դ���̵߳�������������
											// �����Ǹ�ֵ����ôд��������ڽ�����д�����Դ��Ψһ��Ч�ĸ�ֵ��-1
	std::wstring		m_wsDesc;			// 
	std::wstring		m_wsWriter;			// ��ǰд������
	std::wstring		m_wsReader;			// ��ǰ��������

};

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-8 �����ǻ���boost��ͬ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	readLock  ��		2015-10-8
//	��ȡ��������boost::shared_lock<boost::shared_mutex>����֧��Ƕ�ף����Է�װ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class readLock 
{
private:
	readLock();
	readLock(const readLock &rlk);
	readLock& operator=(const readLock &rlk);
public:
#if defined(GTC_BOOST_THREAD)
	readLock(boost::shared_mutex &rdMutex, bool &bLocked, PCWSTR pfile = NULL, PCWSTR pfunc = NULL)
		: m_readLock(rdMutex, boost::defer_lock), m_brefLocked(bLocked)
	{
		if (!m_brefLocked)
		{
			m_readLock.lock();
			m_brefLocked = m_readLock.owns_lock();
#if defined(_DEBUG)
			wchar_t pbuf[MAX_PATH] = {0};
			std::swprintf(pbuf, _countof(pbuf), L"mutex:<%08X> threadID:<%d> file:<%s> func:<%s>", &rdMutex, GetCurrentThreadId(), pfile ?pfile :L"", pfunc ?pfunc :L"");
			mc_mapPreInfo[(INT_PTR)&rdMutex] = pbuf;

#endif
		}
	}
#endif
	~readLock() 
	{
#if defined(GTC_BOOST_THREAD)
		if(m_readLock.owns_lock())
			m_brefLocked = false;
#else
		assert(!L"�˴���δ���壡����");
#endif
		//m_brefLocked = m_readLock.owns_lock();
	}

private:
#if defined(GTC_BOOST_THREAD)

	boost::shared_lock<boost::shared_mutex>	m_readLock;
#endif
	bool	&m_brefLocked;
#if defined(_DEBUG)
	static IStringMapType	mc_mapPreInfo;		// key=��ͬ�����Ļ�������ַ value=ǰ�ζ�д��Ϣ

#endif

};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	writeLock  ��		2015-10-8
//	Ϊ��ͳһ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class writeLock
{
public:
#if defined(GTC_BOOST_THREAD)
	writeLock(boost::shared_mutex &wtMutex, bool &bLocked, PCWSTR pfile = NULL, PCWSTR pfunc = NULL)
		: m_writeLock(wtMutex, boost::defer_lock), m_brefLocked(bLocked)
	{
		//if (!m_brefLocked)
		{
			if(m_brefLocked) 
				Sleep(0);
			m_writeLock.lock();
			m_brefLocked = m_writeLock.owns_lock();
#if defined(_DEBUG)
			wchar_t pbuf[MAX_PATH] = {0};
			std::swprintf(pbuf, _countof(pbuf), L"mutex:<%08X> threadID:<%d> file:<%s> func:<%s>", &wtMutex, GetCurrentThreadId(), pfile ?pfile :L"", pfunc ?pfunc :L"");
			mc_mapPreInfo[(INT_PTR)&wtMutex] = pbuf;

#endif
		}
//		else
//		{
//#if defined(_DEBUG)
//			IStringMapType::const_iterator iter = mc_mapPreInfo.find((INT_PTR)&wtMutex);
//			if (iter != mc_mapPreInfo.cend())
//			{
//				SEND_LOG(L"NOTE writeLock �Ѿ�������������������%s", iter->second.c_str());
//			}
//
//#endif
//			//assert(!L"�Ѿ�������������");
//		}
	}
#endif

	~writeLock()
	{
#if defined(GTC_BOOST_THREAD)
		if(m_writeLock.owns_lock())
			m_brefLocked = false;
#else
		assert(!L"�˴���δ���壡����");
#endif
	}
private:
#if defined(GTC_BOOST_THREAD)

	boost::unique_lock<boost::shared_mutex>	m_writeLock;
#endif
	bool					&m_brefLocked;
#if defined(_DEBUG)
	static IStringMapType	mc_mapPreInfo;		// key=��ͬ�����Ļ�������ַ value=ǰ�ζ�д��Ϣ

#endif
};

}	// gtc
#endif	// end __SYNCOBJECT_H__