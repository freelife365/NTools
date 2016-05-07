#ifndef __SOCKME_H__
#define __SOCKME_H__

#pragma once
#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <ctime>
#include <iostream>
#include <cstdint>
#include <list>

#include <afxsock.h> 
//#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "ac/sysupport.h"
#include "gt/gtdcsdata.h"
namespace gtc {
#define SOCKET_RECVBUFF_LEN			1024		// ���ջ�������С
#define SOCKET_SENDBUFF_LEN			1024		// ���ͻ�������С
#define SOCKET_WINEPOCH				2208988800	// ����������Ԫ��windows��Ԫ֮��Ĳ�ֵ
#define SOCKET_STKSIZE				16536
#define SOCKET_HEARTBEAT_HEADER		"HB:"		// �������Ļظ�ͷ
#define SOCKET_HEARTBEAT_INTERVAL	3000		// ���������ͼ�������룩
#define SOCKET_CONNECTION_RETRYS	10			// ��������
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- sockme	2014-04-08 --- >>
//	�����׽��ֵĲ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class socket
{
public:
	// ����Э��
	typedef enum BESocketType {
		BESocketTypeStream	= SOCK_STREAM,		// TCP
		BESocketTypeDgram	= SOCK_DGRAM		// UDP
	};
	/* ��¼socket��Ϣ */
	typedef struct BSSocketInfo{
		SOCKET		scObject;		// 
		SCDataType	dataRecv;		// ������Ž��յ�����
		SCDataType	dataSend;		// ������ŷ��͵�����
		std::time_t	tmLastSend;		// ���һ�η������ݵ�ʱ��
		std::time_t tmLastRecv;		// ���һ�ν������ݵ�ʱ��
		// 
		BSSocketInfo()
			: scObject(INVALID_SOCKET)
			//, dataRecv(512,0), dataSend(512, 0)
			, tmLastSend(0), tmLastRecv(0)
		{

		}
		BSSocketInfo(SOCKET sct)
			: scObject(sct)
			//, dataRecv(512,0), dataSend(512, 0)
			, tmLastSend(0), tmLastRecv(0)
		{
		}
	}BSSocketInfo;
public:
	socket(void);
	virtual ~socket(void);
public:
	// static 
	/*****************************************************************************************
		<< --- static socket::init	2014-04-08 --- >>
		˵������ʼ��socket���õĻ�����������ֻ��ִ��һ�Ρ�
		������
		major	= �׽��ֵ����屾��
		minor	= �׽��ֵĴΰ屾��
		����ֵ���Ƿ�ɹ�
	*****************************************************************************************/
	static bool init(uint8_t major = 2, uint8_t minor = 2);
	/*****************************************************************************************
		<< --- static socket::clean	2014-04-08 --- >>
		˵�����ͷ�socket���õĻ�����������ֻ��ִ��һ�Ρ�
		������
		����ֵ��
	*****************************************************************************************/
	static void clean();
	/*****************************************************************************************
		<< --- socket::receive	2014-04-09 --- >>
		˵������������
		������
		sc		= �׽���
		vecData	= ��ű����յ�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool receive(SOCKET sc, SCDataType &vecData);
	static bool receive(BSSocketInfo *pscinfo);
	/*****************************************************************************************
		<< --- socket::send	2014-04-10 --- >>
		˵������������
		������
		sc		= �׽���
		vecData	= �����͵�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool send(SOCKET sc, const SCDataType &vecData);

	/*****************************************************************************************
		<< --- static socket::threadECHO	2014-04-10 --- >>
		˵����������ͻ��˽��й�ͨ���̡߳�
		������
		parg	= ��ͻ������ӵ��׽���
		����ֵ��0=�ɹ�
	*****************************************************************************************/
	static unsigned __stdcall threadECHO(void *parg);

	/*****************************************************************************************
		<< --- static socket::threadHeartBeat	2014-05-04 --- >>
		˵�������������˽��й�ͨ���̡߳�
			�Թ̶�ʱ��SOCKET_HEARTBEAT_INTERVAL������˷�����������
		������
		parg	= ���������ӵ��׽���
		����ֵ��0=�ɹ�
	*****************************************************************************************/
	static unsigned __stdcall threadHeartBeat(void *parg);
public:
	/*****************************************************************************************
		<< --- socket::connect	2014-04-08 --- >>
		˵�����������׽��֣�һ�㱻�ͻ��˵���
		������
		phost		= �����ӵ��豸�������ƻ�IP��ַ���磺www.163.com ��192.168.1.55
		pservice	= ���Ӳ��õķ�����������˿ںţ��磺daytime ��3328
		ptransport	= ����Э�飬�磺tcp��udp
		����ֵ���Ƿ����ӳɹ�
	*****************************************************************************************/
	bool connect(const char *phost, const char *pservice, const char* transport);
	bool connect();
	/*****************************************************************************************
		<< --- socket::daytime	2014-04-10 --- >>
		˵�������Թر�connect�򿪵��׽���
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool close() {
		bool bret = closesocket(m_scMain);
		if(!bret)
			errorInfo("closesocketʧ�ܣ�");
		return bret;
	}

	/*****************************************************************************************
		<< --- socket::daytime	2014-04-10 --- >>
		˵������daytime����ȡ��ʱ��
		������
		now		= ����ȡ�õķ�����ʱ��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool daytime(std::time_t &now);
	/*****************************************************************************************
		<< --- socket::passive	2014-04-09 --- >>
		˵�������������׽��֣�һ�㱻����˵���
		������
		pservice	= ���Ӳ��õķ�����������˿ںţ��磺daytime ��3328
		ptransport	= ����Э�飬�磺tcp��udp
		queueLen	= ��ϣ��������������еĳ��ȣ�����UDPʱ�ò�����Ч
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool passive(const char *pservice, const char* ptransport, int32_t queueLen);
	/*****************************************************************************************
		<< --- socket::serviceDaytimeUDP	2014-04-10 --- >>
		˵������������daytime����ı����׽��֣�����ѭ����ʽ��
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool serviceDaytimeUDP();
	/*****************************************************************************************
		<< --- socket::serviceECHOTCP	2014-04-10 --- >>
		˵������������echo����ı����׽��֣����ò�����ʽ��
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool serviceECHOTCP();

	/*****************************************************************************************
		<< --- socket::serviceHeartBeat	2014-05-04 --- >>
		˵������������port�˿ڵ������������񣻲��ò�����ʽ��
			�Խ��յ����ݼ���һ��ͷ��ǽ��лط���
		������
		port = �˿ں�
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool serviceHeartBeat(uint16_t port);

	/*****************************************************************************************
		<< --- socket::receive	2014-04-09 --- >>
		˵������������
		������
		vecData	= ��ű����յ�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool receive(SCDataType &vecData);
	/*****************************************************************************************
		<< --- socket::send	2014-04-10 --- >>
		˵������������
		������
		vecData	= �����͵�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool send(const SCDataType &vecData);
	/*****************************************************************************************
		<< --- socket::clientEcho	2014-04-09 --- >>
		˵����ִ��echo����Ŀͻ���
		������
		sendData	= �����͵�����
		recvData	= ���յ�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool clientEcho(const std::string &sendData, std::string &recvData);

	/*****************************************************************************************
		<< --- socket::clientHeartBeat	2014-04-09 --- >>
		˵����ִ������������Ŀͻ���
		�ڲ�����һ�����߳�threadHeartBeat���Թ̶���������������������
		������
		phostip	= Ŀ����������IP
		port	= ͨѶ�˿ں�
		����ֵ���Ƿ�ִ�гɹ�
		*****************************************************************************************/
	bool clientHeartBeat(const char *phostip, const uint16_t port);
private:
	// ֧�ֺ���
	void errorInfo(const char *pformat, ...)
	{
		const int32_t len = 2048;
		char pinfo[len] = {0};
		va_list argList;
		va_start(argList, pformat);
		std::vsprintf(pinfo, pformat, argList);
		va_end(argList);
		m_csErrinfo = pinfo;
		// 
		int err = WSAGetLastError();
		if(err != 0)
		{
			std::sprintf(pinfo, "SEC��%d", err);
			m_csErrinfo += pinfo;
		}
#if defined(_DEBUG)
		std::cout << m_csErrinfo.c_str() << std::endl;
#endif
	}

public:
	// ����
	std::string	getErrorInfo() const {				return m_csErrinfo;						}
protected:
	// ��ǰsockets��Ϣ
	static WSADATA		mc_wsData;
	// �����˿ں�
	static uint16_t		mc_unBaseport;
protected:
	std::string		m_csHost;		// �������ƻ�IP
	std::string		m_csService;	// �������ƻ�˿ں� echo ��6780
	std::string		m_csTransport;	// ����Э��tcp/udp
	// ���׽��֣��ͻ��˵�����socket�����˵ĵȴ�socket��
	SOCKET			m_scMain;
	// ����Э�����ͣ������������ݱ���
	BESocketType	m_beType;
	// ������Ϣ
	std::string		m_csErrinfo;
	// �첽IO select;��С���ܳ���FD_SETSIZE
	std::list<BSSocketInfo *>	m_lstSocket;
};





}
#endif