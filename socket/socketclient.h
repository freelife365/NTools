#ifndef __SOCKETCLIENT_H__
#define __SOCKETCLIENT_H__
#pragma once
#include <cstring>
#include <cstdint>
#include <map>

#include "ac/sysupport.h"
#include "socket/sockme.h"
#include "socket/sockcache.h"
#include "gt/gtdcsdata.h"

// GTָ���Ӧ��״̬
typedef enum BEGTCMDRespondResult {
	BEGTCMDRespondResultOK			= 0,		// ָ��ִ�гɹ�
	BEGTCMDRespondResultFailure		= 1000,		// ָ��ִ��ʧ��
	BEGTCMDRespondResultTimeout		= 1001		// ��ʱ
};

namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- sockclient	2014-05-06 --- >>
//	�ͻ����׽��ֵĲ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class sockclient : public gtc::socket
{
private:
	sockclient()
		: m_bHeartbeat(false), m_unPort(0)
		, m_hdlWaitforThread(NULL), m_uiWaitforThreadID(0), m_lWaitforThreadSwitch(0)
		, m_hdlHeartbeatThread(NULL), m_uiHeartbeatThreadID(0), m_lHeartbeatThreadSwitch(0)
		, m_lCacheThreadSwitch(0)
		, m_uiRecverThreadID(0), m_hdlRecverThread(NULL)
		, m_uiSenderThreadID(0), m_hdlSenderThread(NULL)
		, m_heventWaitforSender(NULL), m_heventWaitforRecver(NULL)
	{
	}
public:
	sockclient(const char *phost, uint16_t port, bool supportheartbeat = true)
		: m_csHost(phost?phost:""), m_unPort(port), m_bHeartbeat(supportheartbeat)
		, m_hdlWaitforThread(NULL), m_uiWaitforThreadID(0), m_lWaitforThreadSwitch(0)
		, m_hdlHeartbeatThread(NULL), m_uiHeartbeatThreadID(0), m_lHeartbeatThreadSwitch(0)
		, m_lCacheThreadSwitch(0)
		, m_uiRecverThreadID(0), m_hdlRecverThread(NULL)
		, m_uiSenderThreadID(0), m_hdlSenderThread(NULL)
		, m_heventWaitforSender(NULL), m_heventWaitforRecver(NULL)
	{
		m_heventWaitforSender = CreateEvent(NULL, true, false, NULL);
		m_heventWaitforRecver = CreateEvent(NULL, true, false, NULL);
	}
public:
	/*****************************************************************************************
		<< --- sockclient::startup	2014-05-06 --- >>
		˵����������Զ�˵�TCPͨѶ����
			1�������ȴ�������Ϣ�߳�
			2��������Ϣ�����̣߳�m_sckCache����
			3������������̣߳�m_sckCache����
			4������m_bHeartbeat�����Ƿ��������̣߳�gtc::socket����
		������
		hwnd	= ������Ϣ�Ĵ��ھ��
		����ֵ��ͨѶ���
	*****************************************************************************************/
	uint32_t startup(HWND hwnd);

	/*****************************************************************************************
		<< --- sockclient::startup	2014-05-06 --- >>
		˵�����ر�Զ�����ӣ���ֹͣstartup()�������߳�
		������
		����ֵ��
	*****************************************************************************************/
	void shutdown();
	/*****************************************************************************************
	<< --- sockclient::pushGTCmd		2014-05-07 --- >>
	˵�������û�����ֱ��ѹ�뷢�ͻ������
		������
		pcmd	= 
		����ֵ��
	*****************************************************************************************/
	void pushGTCmd(gtcmd *pcmd)
	{
		m_sckCache.pushGTCmd(pcmd);
		SetEvent(m_heventWaitforSender);
	}
	void pushGTCmdResponse(const SCDataType &data)
	{
		m_sckCache.pushRecvData(data);
		SetEvent(m_heventWaitforRecver);
	}
public:
	/*****************************************************************************************
		<< --- static sockclient::doThreadWaitforData		2014-05-06 --- >>
		˵�����ȴ���������������Ϣ��������Ϣѹ����Ϣ�������
		������
		pdata	= sockclient*
		����ֵ��
	*****************************************************************************************/
	static unsigned __stdcall doThreadWaitforData(void *pdata);
	/*****************************************************************************************
		<< --- static sockclient::doThreadHeartbeat		2014-05-06 --- >>
		˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
		������
		pdata	= sockclient*
		����ֵ��
	*****************************************************************************************/
	static unsigned __stdcall doThreadHeartbeat(void *pdata);
	/*****************************************************************************************
		<< --- static sockclient::doThreadSendGTCmd		2014-05-09 --- >>
		˵������������������в�ͣȡ���������
		������
		pdata	= sockclient*
		����ֵ��
	*****************************************************************************************/
	static unsigned __stdcall doThreadSendGTCmd(void *pdata);
	/*****************************************************************************************
		<< --- static sockclient::doThreadSendGTCmd		2014-05-09 --- >>
		˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
		������
		pdata	= sockclient*
		����ֵ��
	*****************************************************************************************/
	static unsigned __stdcall doThreadAnalysisRecvData(void *pdata);
public:
	// ����
	std::string	getHost() const {			return m_csHost;				}
	uint16_t	getPort() const {			return m_unPort;				}
public:
	// ָ��Ӧ����
	//BEGTCMDRespondResult	m_beRespondResult;
	std::map<HANDLE, BEGTCMDRespondResult> m_mapRespondResult;	// ������߳�
private:
	HWND			m_hWnd;
	// �Ƿ�֧���������
	bool			m_bHeartbeat;
	// �������������ƻ�IP
	std::string		m_csHost;
	// ͨѶ�˿ں�
	uint16_t		m_unPort;
	// �ȴ�������Ϣ�߳̿���
	LONG volatile	m_lWaitforThreadSwitch;
	unsigned int	m_uiWaitforThreadID;
	HANDLE			m_hdlWaitforThread;
	// �����߳̿���
	LONG volatile	m_lHeartbeatThreadSwitch;
	unsigned int	m_uiHeartbeatThreadID;
	HANDLE			m_hdlHeartbeatThread;
	// ���ý��ա����ͻ�����
	sockcache		m_sckCache;
	// ����������͡������߳̿���
	LONG volatile	m_lCacheThreadSwitch;
	unsigned int	m_uiSenderThreadID;
	HANDLE			m_hdlSenderThread;
	unsigned int	m_uiRecverThreadID;
	HANDLE			m_hdlRecverThread;
	// ���ͽ����̵߳ĵȴ��¼�
	// ���ͻ�����û����������ʱ�ʹ���˯�ߵȴ���
	HANDLE			m_heventWaitforSender;
	// ���ջ��������û������ʱ�ʹ���˯�ߵȴ���
	HANDLE			m_heventWaitforRecver;
};



}
#endif