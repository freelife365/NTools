#ifndef __DELEGATEBASE_H__
#define __DELEGATEBASE_H__
#pragma once
#include "sysupport.h"
#include "AC/SyncObject.h"
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	delegateBase  ��		2015-8-26
//	���������
//	��Ϣ�����ĸ�ʽΪ��LRESULT func(WPARAM wParam, LPARAM lParam)
////////////////////////////////////////////////////////////////////////////////////////////////////////
class delegateBase
{
public:
	typedef std::vector<delegateBase *> PtrArrayType;
	//friend bool operator== (const delegateBase &dl, const delegateBase &dr);
private:
	delegateBase();
	delegateBase& operator=(const delegateBase&); 
public:
	delegateBase(void *pobj, void *pfunc)
		: m_pObject(pobj), m_pFunc(pfunc)
	{
		assert(m_pObject && m_pFunc);
	}
	virtual ~delegateBase(){}
	delegateBase(const delegateBase &dbase)
		:  m_pObject(dbase.m_pObject), m_pFunc(dbase.m_pFunc)
	{}
	virtual delegateBase* copy() const = 0;

	virtual bool operator ==(const delegateBase &dbase) const
	{
		return m_pFunc == dbase.m_pFunc && m_pObject == dbase.m_pObject;
	}
	virtual bool operator !=(const delegateBase &dbase) const
	{
		return !operator==(dbase);
	}
	void operator()(WPARAM wParam, LPARAM lParam)
	{
		invoke(wParam, lParam);
	}
protected:
	void* getObject() const {				return m_pObject;									}
	void* getFunction() const {				return m_pFunc;										}
	virtual void invoke(WPARAM wParam, LPARAM lParam) = 0; 
private:
	void *m_pObject;	// �����
	void *m_pFunc;		// ����ָ��
};
inline bool operator== (const delegateBase *dl, const delegateBase &dr)
{
	return dl->operator==(dr);
	//return dl.m_pFunc == dr.m_pFunc && dl.m_pObject == dr.m_pObject;
	//return dl.getObject() == dr.getObject() && dl.getFunction() == dr.getFunction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	delegateFunc ��		2015-8-27
//	�����ͨ�����Ĵ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class delegateFunc : public delegateBase
{
	typedef void (*FUNC_TYPE)(WPARAM wParam, LPARAM lParam);
public:
	delegateFunc(FUNC_TYPE pfunc)
		: delegateBase(nullptr, pfunc)
	{}
	virtual delegateBase* copy() const
	{
		return new delegateFunc(*this);
	}
protected:
	virtual void invoke(WPARAM wParam, LPARAM lParam)
	{
		FUNC_TYPE pfunc = (FUNC_TYPE)getFunction();
		assert(pfunc);
		(*pfunc)(wParam, lParam);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	delegateObject  ��		2015-8-27
//	������Ա�����Ĵ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename O, typename T>
class delegateObject : public delegateBase
{
	typedef void (T::* MEMBER_TYPE)(WPARAM wParam, LPARAM lParam);
public:
	delegateObject(O *pobj, MEMBER_TYPE pfunc)
		: delegateBase(pobj, &pfunc)
		, m_pMemberFunc(pfunc)
	{
		assert(m_pMemberFunc);
	}
	virtual delegateBase* copy() const
	{
		return new delegateObject(*this);
	}
protected:
	virtual void invoke(WPARAM wParam, LPARAM lParam)
	{
		O *pobj = (O*)getObject();
		
		(pobj->*m_pMemberFunc)(wParam, lParam);
	}
private:
	MEMBER_TYPE		m_pMemberFunc;
};


template <class O, class T>
delegateObject<O, T> makeDelegate(O* pObject, void (T::* pMemberFunc)(WPARAM wParam, LPARAM lParam))
{
	return delegateObject<O, T>(pObject, pMemberFunc);
}

inline delegateFunc makeDelegate(void (*pFunc)(WPARAM wParam, LPARAM lParam))
{
	return delegateFunc(pFunc); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	eventSource  ��		2015-8-27
//	�¼�Դ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class eventSource
{
	typedef void (*FUNC_TYPE)(WPARAM wParam, LPARAM lParam);
public:
	eventSource();
	eventSource(const eventSource &es);
	~eventSource();
	operator bool();
	eventSource& operator= (const eventSource &es);
	eventSource& operator= (const delegateBase& dbase);
	void operator+= (const delegateBase& dbase); // add const for gcc
	void operator+= (FUNC_TYPE pfunc);
	void operator-= (const delegateBase& dbase);
	void operator-= (FUNC_TYPE pfunc);
	void operator()(WPARAM wParam, LPARAM lParam);


public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-8 �����m_vecDelegate�ı�����	
#if defined(GTC_BOOST_THREAD)
protected:
	mutable boost::shared_mutex m_rwMutex;
	mutable bool				m_bReadLocked;
	mutable bool				m_bWriteLocked;
public:
#define READ_LOCK	gtc::readLock	rdLock(m_rwMutex, m_bReadLocked, __FILEW__, __FUNCTIONW__);
#define WRITE_LOCK	gtc::writeLock	wtLock(m_rwMutex, m_bWriteLocked, __FILEW__, __FUNCTIONW__);
#else
#define READ_LOCK
#define WRITE_LOCK
#endif

protected:
	delegateBase::PtrArrayType	m_vecDelegate;
};

}	// gtc

#endif	// __DELEGATEBASE_H__