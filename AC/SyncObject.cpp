#include "SyncObject.h"

namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	criticalSection  ��		2015-7-14
//	��CRITICAL_SECTION��װ
////////////////////////////////////////////////////////////////////////////////////////////////////////
criticalSection::criticalSection()
{
	::InitializeCriticalSection(&m_cs);
	//::InitializeCriticalSectionAndSpinCount(&m_cs, 4000);

}
criticalSection::~criticalSection()
{
	::DeleteCriticalSection(&m_cs);
}

bool criticalSection::tryEnter(PCWSTR pdesc /*= NULL*/)
{
	bool bl = ::TryEnterCriticalSection(&m_cs) == TRUE;
	if(bl && pdesc) m_strPrevEntry = pdesc;

	//SEND_LOG(L"criticalSection::tryEnter ���Խ���ؼ��Ρ�%s�� desc:<%s> prev:<%s>", bl ?L"�ɹ�" :L"ʧ��", pdesc ?pdesc :L"", m_strPrevEntry.c_str());
	return bl;
}
void criticalSection::enter(PCWSTR pdesc /*= NULL*/)
{
	//SEND_LOG(L"criticalSection::enter ׼������ؼ��� desc:<%s> prev:<%s>", pdesc ?pdesc :L"", m_strPrevEntry.c_str());
	::EnterCriticalSection(&m_cs);
	if(pdesc) m_strPrevEntry = pdesc;
	//SEND_LOG(L"criticalSection::enter �ɹ�����ؼ��� desc:<%s> prev:<%s>", pdesc ?pdesc :L"", m_strPrevEntry.c_str());

}
void criticalSection::leave(PCWSTR pdesc /*= NULL*/)
{
	::LeaveCriticalSection(&m_cs);
	//SEND_LOG(L"criticalSection::enter �뿪�ؼ��� desc:<%s> prev:<%s>", pdesc ?pdesc :L"", m_strPrevEntry.c_str());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	optex  ��		2015-5-26
//	����ͬ�����࣬�ɿ����(������)
////////////////////////////////////////////////////////////////////////////////////////////////////////
int optex::mc_bUniprocessorHost = -1;

void optex::commonConstructor(DWORD dwSpinCount, PCWSTR pName)
{
	if (mc_bUniprocessorHost == -1) {
		// This is the 1st object constructed, get the number of CPUs
		SYSTEM_INFO sinf;
		GetSystemInfo(&sinf);
		mc_bUniprocessorHost = (sinf.dwNumberOfProcessors == 1);
	}


	m_hEvent = m_hFileMap = NULL;
	m_psi = NULL;

	if (pName == NULL) {  // Creating a single-process optex

		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(m_hEvent != NULL);

		m_psi = new SHAREDINFO;
		assert(m_psi != NULL);
		ZeroMemory(m_psi, sizeof(*m_psi));

	} else {                // Creating a cross-process optex

		// Always use ANSI so that this works on Win9x and Windows 2000
		wchar_t pObjName[MAX_PATH] = {0};
		std::swprintf(pObjName, _countof(pObjName), L"opter_event_%s", pName);
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, pObjName);
		assert(m_hEvent != NULL);

		std::swprintf(pObjName, _countof(pObjName), L"opter_mmf_%s", pName);
		m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
			PAGE_READWRITE, 0, sizeof(*m_psi), pName);
		assert(m_hFileMap != NULL);

		m_psi = (PSHAREDINFO) MapViewOfFile(m_hFileMap, 
			FILE_MAP_WRITE, 0, 0, 0);
		assert(m_psi != NULL);

		// Note: SHAREDINFO's m_lLockCount, m_dwThreadId, and m_lRecurseCount
		// members need to be initialized to 0. Fortunately, a new pagefile 
		// MMF sets all of its data to 0 when created. This saves use from 
		// some thread synchronization work.
	}

	setSpinCount(dwSpinCount);
}

optex::~optex()
{
#ifdef _DEBUG
	if (isSingleProcessOptex() && (m_psi->dwThreadId != 0)) {
		// A single-process optex shouldn't be destroyed if any thread owns it
		DebugBreak();
	}

	if (!isSingleProcessOptex() && 
		(m_psi->dwThreadId == GetCurrentThreadId())) {

			// A cross-process optex shouldn't be destroyed if our thread owns it
			DebugBreak();
	}
#endif

	CloseHandle(m_hEvent);

	if (isSingleProcessOptex()) {
		delete m_psi;
	} else {
		UnmapViewOfFile(m_psi);
		CloseHandle(m_hFileMap);
	}
}

void optex::setDescription(PCWSTR pDesc)
{
	m_wsDesc = pDesc ? pDesc : L"";
}
std::wstring optex::getDescription() const
{
	return m_wsDesc;
}

void optex::setSpinCount(DWORD dwSpinCount)
{
	if (mc_bUniprocessorHost == 0)
	{
		InterlockedExchangePointer((PVOID *)&m_psi->dwSpinCount, (PVOID)(DWORD_PTR)dwSpinCount);
	}
}
void optex::enter(PCWSTR pDesc /*= NULL*/)
{
	// Spin, trying to get the optex
	if (tryEnter())
	{
		if(pDesc) m_wsPrevEntry = pDesc;
		return;  // We got it, return
	}
	// We couldn't get the optex, wait for it.
	DWORD dwThreadId = GetCurrentThreadId();

	if (InterlockedIncrement(&m_psi->lLockCount) == 1) {

		// Optex is unowned, let this thread own it once
		m_psi->dwThreadId = dwThreadId;
		m_psi->dwProcessID = GetCurrentProcessId();
		m_psi->lRecurseCount = 1;

	} else {

		if (m_psi->dwThreadId == dwThreadId) {

			// If optex is owned by this thread, own it again
			++m_psi->lRecurseCount;

		} else {

			// Optex is owned by another thread, wait for it
			if(pDesc)
				SEND_LOG(L"ERROR optex::enter ��ʼ�ȴ�thread:%4d�ͷ� ���߳�id:%4d flag:<%s> me:<%s>", m_psi->dwThreadId, dwThreadId, pDesc, toString().c_str());
			else
				SEND_LOG(L"ERROR optex::enter ��ʼ�ȴ�thread:%4d�ͷ� ���߳�id:%4d <%s>", m_psi->dwThreadId, dwThreadId, toString().c_str());
			WaitForSingleObject(m_hEvent, INFINITE);
			if(pDesc)
				SEND_LOG(L"ERROR optex::enter �ȴ�����                      ���߳�id:%4d        flag:<%s> me:<%s>", m_psi->dwThreadId, dwThreadId, pDesc, toString().c_str());
			else
				SEND_LOG(L"ERROR optex::enter �ȴ�����                      ���߳�id:%4d        <%s>", m_psi->dwThreadId, dwThreadId, toString().c_str());
			// Optex is unowned, let this thread own it once
			m_psi->dwThreadId = dwThreadId;
			m_psi->dwProcessID = GetCurrentProcessId();
			m_psi->lRecurseCount = 1;
		}
	}
	
}
bool optex::tryEnter(PCWSTR pDesc /*= NULL*/)
{
	DWORD dwThreadId = GetCurrentThreadId();

	bool bThisThreadOwnsTheOptex = false;     // Assume a thread owns the optex
	DWORD dwSpinCount = m_psi->dwSpinCount; // How many times to spin

	do {
		// If lock count = 0, optex is unowned, we can own it
		bThisThreadOwnsTheOptex = (0 == 
			InterlockedCompareExchange(&m_psi->lLockCount, 1, 0)); 

		if (bThisThreadOwnsTheOptex) {

			// Optex is unowned, let this thread own it once
			m_psi->dwThreadId = dwThreadId;
			m_psi->dwProcessID = GetCurrentProcessId();
			m_psi->lRecurseCount = 1;

		} else {

			if (m_psi->dwThreadId == dwThreadId) {

				// If optex is owned by this thread, own it again
				InterlockedIncrement(&m_psi->lLockCount);
				++m_psi->lRecurseCount;
				bThisThreadOwnsTheOptex = true;
			}
		}

	} while (!bThisThreadOwnsTheOptex && (dwSpinCount-- > 0));

	// Return whether or not this thread owns the optex
	if(!bThisThreadOwnsTheOptex)
	{
		if(pDesc)
			SEND_LOG(L"ERROR optex::tryEnter ���Խ����߳�ʧ�ܣ�����thread:%4d�ͷ� ���߳�id:%4d flag:<%s> me:<%s>", pDesc, m_psi->dwThreadId, dwThreadId, toString().c_str());
		else
			SEND_LOG(L"ERROR optex::tryEnter ���Խ����߳�ʧ�ܣ�����thread:%4d�ͷ� ���߳�id:%4d <%s>", m_psi->dwThreadId, dwThreadId, toString().c_str());

	}
	return(bThisThreadOwnsTheOptex);
}
void optex::leave(PCWSTR /*pDesc = NULL*/)
{
#ifdef _DEBUG
	// Only the owning thread can leave the optex
	if (m_psi->dwThreadId != GetCurrentThreadId()) 
		DebugBreak();
#endif

	// Reduce this thread's ownership of the optex
	if (--m_psi->lRecurseCount > 0) {

		// We still own the optex
		InterlockedDecrement(&m_psi->lLockCount);

	} else {

		// We don't own the optex anymore
		m_psi->dwThreadId = 0;

		if (InterlockedDecrement(&m_psi->lLockCount) > 0) {

			// Other threads are waiting, the auto-reset event wakes one of them
			SetEvent(m_hEvent);
		}
	}
}

std::wstring optex::toString() const
{
	wchar_t pbuf[MAX_PATH] = {0};
	std::swprintf(pbuf, _countof(pbuf), L"prevFlag:<%s> ������<%s> ����������%s ProcessID:%-4d ThreadId:%-4d LockCount:%-2d RecurseCount:%-2d", 
		m_wsPrevEntry.c_str(), m_wsDesc.c_str(), isSingleProcessOptex()?L"��" :L"��", 
		m_psi->dwProcessID, m_psi->dwThreadId, m_psi->lLockCount, m_psi->lRecurseCount);

	return pbuf;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	swmrg  ��		2015-5-26
//	����д�����/����Ķ�����ı���
//	4��ԭ��
//	1) ��һ���߳�����д������ʱ�������κ��̲߳���д�����ݡ�
//	2) ��һ���߳�����д������ʱ�������κ��̲߳��ܶ�ȡ���ݡ�
//	3) ��һ���߳����ڶ�ȡ����ʱ�������κ��̲߳���д�����ݡ�
//	4) ��һ���߳����ڶ�ȡ����ʱ�������߳�Ҳ�ܹ���ȡ���ݡ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
swmrg::swmrg(PCWSTR pDesc /*= NULL*/)
	: m_wsDesc(pDesc ? pDesc : L"")
{
	m_nWaitingReaders = m_nWaitingWriters = m_nActive = 0;
	m_hsemReaders = CreateSemaphore(NULL, 0, MAXLONG, NULL);
	m_hsemWriters = CreateSemaphore(NULL, 0, MAXLONG, NULL);
	InitializeCriticalSection(&m_cs);
}
swmrg::~swmrg()
{
#ifdef _DEBUG
	// A SWMRG shouldn't be destroyed if any threads are using the resource
	if (m_nActive != 0)
		DebugBreak();
#endif

	m_nWaitingReaders = m_nWaitingWriters = m_nActive = 0;
	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hsemReaders);
	CloseHandle(m_hsemWriters);
}

void swmrg::setDescription(PCWSTR pDesc)
{
	m_wsDesc = pDesc ? pDesc : L"";
}
std::wstring swmrg::getDescription() const
{
	return m_wsDesc;
}
void swmrg::waitToRead(PCWSTR pDesc /*= NULL*/)
{
#if defined(SYNC_OBJECT_LOG_ON)
	SEND_LOG(L"swmrg::waitToRead begin flag:<%s> desc:<%s>", m_wsDesc.c_str(), pDesc ?pDesc :L"");
#endif
	// Ensure exclusive access to the member variables
	EnterCriticalSection(&m_cs);

	// Are there writers waiting or is a writer writing?
	bool bResourceWritePending = (m_nWaitingWriters || (m_nActive < 0));

	if (bResourceWritePending) {

		// This reader must wait, increment the count of waiting readers
		m_nWaitingReaders++;
	} else {

		// This reader can read, increment the count of active readers
		++m_nActive;
		if(pDesc) m_wsReader = pDesc;
	}

	// Allow other threads to attempt reading/writing
	LeaveCriticalSection(&m_cs);

	if (bResourceWritePending) {

		// This thread must wait
		if(pDesc)
			SEND_LOG(L"ERROR swmrg::waitToRead  read  begin ׼������ȴ�����ȡ�� flag:<%s> me:<%s> ", pDesc, toString().c_str());
		else
			SEND_LOG(L"ERROR swmrg::waitToRead  read  begin ׼������ȴ�����ȡ�� <%s>", toString().c_str());
		WaitForSingleObject(m_hsemReaders, INFINITE);
		if(pDesc)
			SEND_LOG(L"ERROR swmrg::waitToRead  read  end                        flag:<%s> me:<%s> ", pDesc, toString().c_str());
		else
			SEND_LOG(L"ERROR swmrg::waitToRead  read  end                        <%s>", toString().c_str());
	}
}
void swmrg::waitToWrite(PCWSTR pDesc /*= NULL*/)
{
	//if(m_wsDesc != L"USII")
	//	SEND_LOG(L"swmrg::waitToWrite begin flag:<%s> desc:<%s>", m_wsDesc.c_str(), pDesc ?pDesc :L"");
	// Ensure exclusive access to the member variables
	EnterCriticalSection(&m_cs);

	// Are there any threads accessing the resource?
	bool bResourceOwned = (m_nActive != 0);

	if (bResourceOwned) {

		// This writer must wait, increment the count of waiting writers
		m_nWaitingWriters++;
	} else {

		// This writer can write, decrement the count of active writers
		m_nActive = -1;
		if(pDesc) m_wsWriter = pDesc;
	}

	// Allow other threads to attempt reading/writing
	LeaveCriticalSection(&m_cs);

	if (bResourceOwned) {

		// This thread must wait
		if(pDesc)
			SEND_LOG(L"ERROR swmrg::waitToWrite write begin ׼������ȴ���д��   flag:<%s> me:<%s> ", pDesc, toString().c_str());
		else
			SEND_LOG(L"ERROR swmrg::waitToWrite write begin ׼������ȴ���д��   <%s>", toString().c_str());
		WaitForSingleObject(m_hsemWriters, INFINITE);
		if(pDesc)
			SEND_LOG(L"ERROR swmrg::waitToWrite write end                        flag:<%s> me:<%s> ", pDesc, toString().c_str());
		else
			SEND_LOG(L"ERROR swmrg::waitToWrite write end                        <%s>", toString().c_str());
		
	}
}
void swmrg::done(PCWSTR pDesc /*= NULL*/)
{
#if defined(SYNC_OBJECT_LOG_ON)
	SEND_LOG(L"swmrg::done begin flag:<%s> desc:<%s>", m_wsDesc.c_str(), pDesc ?pDesc :L"");
#endif
	//SEND_LOG(L"swmrg::waitToWrite readers:%d writers:%d", m_nWaitingReaders, m_nWaitingWriters);
	// Ensure exclusive access to the member variables
	EnterCriticalSection(&m_cs);

	if (m_nActive > 0) {

		// Readers have control so a reader must be done
		--m_nActive;
	} else {

		// Writers have control so a writer must be done
		++m_nActive;
	}

	HANDLE hsem = NULL;  // Assume no threads are waiting
	LONG lCount = 1;     // Assume only 1 waiter wakes; always true for writers

	if (m_nActive == 0) {

		// No thread has access, who should wake up?
		// NOTE: It is possible that readers could never get access
		//       if there are always writers wanting to write

		if (m_nWaitingWriters > 0) {

			// Writers are waiting and they take priority over readers
			m_nActive = -1;         // A writer will get access
			m_nWaitingWriters--;    // One less writer will be waiting
			hsem = m_hsemWriters;   // Writers wait on this semaphore
			// NOTE: The semaphore will release only 1 writer thread
			m_wsWriter.clear();

		} else if (m_nWaitingReaders > 0) {

			// Readers are waiting and no writers are waiting
			m_nActive = m_nWaitingReaders;   // All readers will get access
			m_nWaitingReaders = 0;           // No readers will be waiting
			hsem = m_hsemReaders;            // Readers wait on this semaphore
			lCount = m_nActive;              // Semaphore releases all readers
			m_wsReader.clear();
		} else {

			// There are no threads waiting at all; no semaphore gets released
		}
	}
	else
	{
		// ע��Ŷ��������֧��Ƕ�ף�
		if(pDesc)
			SEND_LOG(L"ERROR swmrg::done        done  end                        desc:<%s> me:<%s> ", pDesc, toString().c_str());
		else
			SEND_LOG(L"ERROR swmrg::done        done  end                        <%s>", toString().c_str());

	}

	// Allow other threads to attempt reading/writing
	LeaveCriticalSection(&m_cs);

	if (hsem != NULL) {

		// Some threads are to be released
		ReleaseSemaphore(hsem, lCount, NULL);
	}
}
std::wstring swmrg::toString() const
{
	wchar_t pbuf[MAX_PATH] = {0};
	std::swprintf(pbuf, _countof(pbuf), L"������%s �߳�ID:%-4d Active:%-4d waitforReaders:%-4d<%s> waitforWriters:%-4d<%s>", 
		m_wsDesc.c_str(), GetCurrentThreadId(), 
		m_nActive, m_nWaitingReaders, m_wsReader.c_str(), m_nWaitingWriters, m_wsWriter.c_str());
	return pbuf;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	readLock  ��		2015-10-8
//	��ȡ��������boost::shared_lock<boost::shared_mutex>����֧��Ƕ�ף����Է�װ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)
IStringMapType readLock::mc_mapPreInfo;

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	writeLock  ��		2015-10-8
//	Ϊ��ͳһ
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)
IStringMapType writeLock::mc_mapPreInfo;

#endif

}	// end gtc