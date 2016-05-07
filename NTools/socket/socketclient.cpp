#include "socketclient.h"
#include <cstdio>
#include <map>

#include "ac/sysupport.h"
#include "gt/gtdcsdata.h"
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- sockclient	2014-05-06 --- >>
//	�ͻ����׽��ֵĲ���
////////////////////////////////////////////////////////////////////////////////////////////////////////

//// ���ӹ�������ÿ�������һ����¼
//std::map<unsigned int, gtc::sockclient *> g_mapDllServer;

/*****************************************************************************************
	<< --- socket::startup	2014-05-06 --- >>
	˵����������Զ�˵�TCPͨѶ����
		1�������ȴ�������Ϣ�߳�
		2��������Ϣ�����̣߳�m_sckCache����
		3������������̣߳�m_sckCache����
		4������m_bHeartbeat�����Ƿ��������̣߳�gtc::socket����
	������
	hwnd	= ������Ϣ�Ĵ��ھ��
	����ֵ��ͨѶ���
*****************************************************************************************/
uint32_t sockclient::startup(HWND hwnd)
{
	char port[10] = {0};
	std::sprintf(port, "%u", m_unPort);
	m_hWnd = hwnd;
	if (m_scMain != INVALID_SOCKET)
	{
		shutdown();
	}

	if (connect(m_csHost.c_str(), port, "tcp"))
	{
		// �ȴ��߳�
		m_hdlWaitforThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadWaitforData, this, CREATE_SUSPENDED, &m_uiWaitforThreadID);
		if(!m_hdlWaitforThread)
		{
			shutdown();
			return 0;
		}
		// �������͡����ջ���
		m_hdlSenderThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadSendGTCmd, this, CREATE_SUSPENDED, &m_uiSenderThreadID);
		m_hdlRecverThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadAnalysisRecvData, this, CREATE_SUSPENDED, &m_uiRecverThreadID);
		if(m_hdlSenderThread && m_hdlRecverThread)
		{
			InterlockedCompareExchange(&m_lCacheThreadSwitch, 1, 0);
			ResumeThread(m_hdlSenderThread);
			ResumeThread(m_hdlRecverThread);
		}
		else
		{
			shutdown();
			return 0;
		}

		// ����
		if (m_bHeartbeat)
		{
			m_hdlHeartbeatThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadHeartbeat, this, CREATE_SUSPENDED, &m_uiHeartbeatThreadID);
			if(m_hdlHeartbeatThread)
			{
				SetThreadPriority(m_hdlHeartbeatThread, THREAD_PRIORITY_BELOW_NORMAL);
				InterlockedCompareExchange(&m_lHeartbeatThreadSwitch, 1, 0);
				ResumeThread(m_hdlHeartbeatThread);
			}
			else
			{
				shutdown();
				return 0;
			}
		}
		// �����ȴ��߳�
		InterlockedCompareExchange(&m_lWaitforThreadSwitch, 1, 0);
		ResumeThread(m_hdlWaitforThread);
	}



	return m_scMain == INVALID_SOCKET?0:m_scMain;
}

/*****************************************************************************************
	<< --- socket::startup	2014-05-06 --- >>
	˵�����ر�Զ�����ӣ���ֹͣstartup()�������߳�
	������
	����ֵ��
*****************************************************************************************/
void sockclient::shutdown()
{
	// �ر�����
	InterlockedCompareExchange(&m_lHeartbeatThreadSwitch, 0, 1);
	// �رջ���
	InterlockedCompareExchange(&m_lCacheThreadSwitch, 0, 1);
	// �رյȴ��߳�
	InterlockedCompareExchange(&m_lWaitforThreadSwitch, 0, 1);
	SetEvent(m_heventWaitforSender);
	SetEvent(m_heventWaitforRecver);
	//ResumeThread(m_hdlRecverThread);
	//ResumeThread(m_hdlSenderThread);
	// �����̹߳رպ���ܹر�����
	HANDLE hdls[] = {m_hdlWaitforThread, m_hdlHeartbeatThread, m_hdlSenderThread, m_hdlRecverThread};
	WaitForMultipleObjects(4, hdls, true, INFINITE);
	close();
	// 
	CloseHandle(m_hdlRecverThread);
	CloseHandle(m_hdlSenderThread);
	CloseHandle(m_hdlHeartbeatThread);
	CloseHandle(m_hdlWaitforThread);
}

/*****************************************************************************************
	<< --- static sockclient::doThreadWaitforData		2014-05-06 --- >>
	˵�����ȴ���������������Ϣ��������Ϣѹ����Ϣ�������
	������
	pdata	= sockclient*
	����ֵ��
*****************************************************************************************/
unsigned __stdcall sockclient::doThreadWaitforData(void *pdata)
{
	int	retrys = 0;
	sockclient *pclient = (sockclient*)pdata;
	assert(pclient);
	timeval tv = {2, 0};	// 2��
	fd_set fdread;
	int ret;
	SCDataType data;
	bool scerr = false;
	gtc::app::writeLogs(L"begin", L"doThreadWaitforData", L"sock");
	while(pclient->m_lWaitforThreadSwitch == 1)
	{
		scerr = false;
		FD_ZERO(&fdread);
		FD_SET(pclient->m_scMain, &fdread);
		// �ȴ���Ϣ
		ret = select(0, &fdread, nullptr, nullptr, &tv);
		// ����
		if (ret == SOCKET_ERROR)
		{
			scerr = true;
		}
		// ������
		if (ret > 0)
		{
			if(pclient->receive(data))
			{
				pclient->pushGTCmdResponse(data);
			}
			else
			{
				scerr = true;
			}
		}
		// socket������������
		if (scerr)
		{
			if (pclient->connect())
			{
				retrys = 0;
			}
			else
			{
				if (retrys < SOCKET_CONNECTION_RETRYS)
				{
					retrys++;
#if defined(_DEBUG)
					char buf[40] = {0};
					std::sprintf(buf, "������%d��...", retrys);
					SendMessage(pclient->m_hWnd, WM_Log, strlen(buf), (LPARAM)buf);
#endif
					Sleep(retrys*1000);
					continue;
				}
				else
				{
					pclient->shutdown();
				}
			}
		}
	}
	gtc::app::writeLogs(L"end", L"doThreadWaitforData", L"sock");
	return 0;
}

/*****************************************************************************************
	<< --- static sockclient::doThreadHeartbeat		2014-05-06 --- >>
	˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
	������
	pdata	= sockclient*
	����ֵ��
*****************************************************************************************/
unsigned __stdcall sockclient::doThreadHeartbeat(void *pdata)
{
	gtcmdH *ph = new gtcmdH();
	sockclient *pclient = (sockclient*)pdata;
	assert(pclient && ph);
	char buf[80] = {0};
	gtc::app::writeLogs(L"begin", L"doThreadHeartbeat", L"sock");
	while(pclient->m_lHeartbeatThreadSwitch == 1)
	{
		pclient->pushGTCmd(ph);
//#if defined(_DEBUG)
//		std::sprintf(buf, "Hbs %0.2f", ph->differenceTime());
//		SendMessage(pclient->m_hWnd, WM_H, std::strlen(buf), (LPARAM)buf);
//#endif
		// ��Ϣ��
		if (pclient->m_lHeartbeatThreadSwitch == 1)
		{
			Sleep(SOCKET_HEARTBEAT_INTERVAL);
		}
	}
	delete ph;
	gtc::app::writeLogs(L"end", L"doThreadHeartbeat", L"sock");
	return 0;
}
/*****************************************************************************************
	<< --- static sockclient::doThreadSendGTCmd		2014-05-09 --- >>
	˵������������������в�ͣȡ���������
	������
	pdata	= sockcache*
	����ֵ��
*****************************************************************************************/
unsigned __stdcall sockclient::doThreadSendGTCmd(void *pdata)
{
	sockclient *pclient = (sockclient *)pdata;
	assert(pclient != nullptr);
	gtcmd *pcmd = nullptr;
	SCDataType data;
	gtc::app::writeLogs(L"begin", L"doThreadSendGTCmd", L"sock");
	while(pclient->m_lCacheThreadSwitch == 1)
	{	// 
		switch(WaitForSingleObject(pclient->m_heventWaitforSender, INFINITE))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			break;
		}
		if (pclient->m_lCacheThreadSwitch == 0)
		{
			break;
		}
//#if defined(_DEBUG)
//		SendMessage(pclient->m_hWnd, WM_Pn, 80, (LPARAM)"event waitfor sender");
//#endif
		if ((pcmd = pclient->m_sckCache.popGTCmd()) != nullptr && pcmd->getCmd(data))
		{
			if (socket::send(pclient->m_scMain, data))
			{
				if(gtcmdH *ph = dynamic_cast<gtcmdH *>(pcmd))
					ph->refreshTime();
			}
			else
			{
#if defined(_DEBUG)
				char buf[100] = {0};
				std::sprintf(buf, "���͡�%s��ʧ�ܣ�", gtc::convert::toChar(toString(pcmd->getCmdType()).c_str()).c_str());
				SendMessage(pclient->m_hWnd, WM_Log, lstrlenA(buf), (LPARAM)buf);
#endif
			}

		}
		if(pcmd && pcmd->getCmdType() != BEGTSocketCmdH)	// �����������ظ�new
		{
			delete pcmd;
			pcmd = nullptr;
		}
		if (pclient->m_sckCache.isEmptySender() && pclient->m_lCacheThreadSwitch == 1)
		{
			ResetEvent(pclient->m_heventWaitforSender);
		}
	}
	gtc::app::writeLogs(L"end", L"doThreadSendGTCmd", L"sock");
	return 0;
}
/*****************************************************************************************
	<< --- static sockclient::doThreadSendGTCmd		2014-05-09 --- >>
	˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
	������
	pdata	= sockclient*
	����ֵ��
*****************************************************************************************/
unsigned __stdcall sockclient::doThreadAnalysisRecvData(void *pdata)
{
	sockclient *pclient = (sockclient*)pdata;
	assert(pclient);
	SCDataType data;
	UINT cmdflag = 0;
	std::vector<SCDataType> cmds;
	std::vector<SCDataType>::iterator iter;
	gtc::app::writeLogs(L"begin", L"doThreadAnalysisRecvData", L"sock");
	while(pclient->m_lCacheThreadSwitch == 1 /* || !m_queRecver.empty()*/)
	{	// G74 Pn64 H68
		switch(WaitForSingleObject(pclient->m_heventWaitforRecver, INFINITE))
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			break;
		}
		if (pclient->m_lCacheThreadSwitch == 0)
		{
			break;
		}
//#if defined(_DEBUG)
//		SendMessage(pclient->m_hWnd, WM_Pn, 80, (LPARAM)"event waitfor recver");
//#endif
		if (pclient->m_sckCache.popRecvData(cmds))
		{
			for(iter = cmds.begin(); iter != cmds.end(); ++iter)
			{
				switch(iter->front())
				{
				case _GTCMD_RECVTYPE_G:
					{
						if(HANDLE hevent = OpenEvent(EVENT_ALL_ACCESS, false, _T("GTDCS_EVENT_G")))
						{
							pclient->m_mapRespondResult[hevent] = BEGTCMDRespondResultOK;
							SetEvent(hevent);
							continue;
						}
					}
					break;
				case _GTCMD_RECVTYPE_Pn:
					cmdflag = WM_Pn;
					break;
				case _GTCMD_RECVTYPE_H:
					cmdflag = WM_H;
					break;
				case _GTCMD_RECVTYPE_Ac:
					{
						if(HANDLE hevent = OpenEvent(EVENT_ALL_ACCESS, false, _T("GTDCS_EVENT_AC")))
						{
							pclient->m_mapRespondResult[hevent] = BEGTCMDRespondResultOK;
							SetEvent(hevent);
							continue;
						}
					}
					break;
				default:
					cmdflag = 0;
					break;
				}
				::SendMessage(pclient->m_hWnd, cmdflag, (WPARAM)iter->size(), (LPARAM)iter->data());
			}
		}
		if (pclient->m_sckCache.isEmptyRecver() && pclient->m_lCacheThreadSwitch == 1)
		{
			ResetEvent(pclient->m_heventWaitforRecver);
		}
	}
	gtc::app::writeLogs(L"end", L"doThreadAnalysisRecvData", L"sock");
	return 0;
}


}
