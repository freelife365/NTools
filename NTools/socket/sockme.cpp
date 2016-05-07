#include "sockme.h"
#include <cstdio>
#include <assert.h>
#include <process.h>
namespace gtc {
// ��ǰsockets��Ϣ
WSADATA socket::mc_wsData = {0};
// �����˿ں�
uint16_t socket::mc_unBaseport = 0;

socket::socket(void)
	: m_scMain(INVALID_SOCKET)
	, m_beType(BESocketTypeStream)
{
}


socket::~socket(void)
{
}

/*****************************************************************************************
	<< --- static sockme::init	2014-04-08 --- >>
	˵������ʼ��socket���õĻ�����������ֻ��ִ��һ�Ρ�
	������
	major	= �׽��ֵ����屾��
	minor	= �׽��ֵĴΰ屾��
	����ֵ���Ƿ�ɹ�
*****************************************************************************************/
bool socket::init(uint8_t major, uint8_t minor)
{
	return WSAStartup(MAKEWORD(major, minor), &mc_wsData)==0;
}

/*****************************************************************************************
	<< --- static sockme::clean	2014-04-08 --- >>
	˵�����ͷ�socket���õĻ�����������ֻ��ִ��һ�Ρ�
	������
	����ֵ��
*****************************************************************************************/
void socket::clean()
{
	WSACleanup();
}
/*****************************************************************************************
	<< --- static socket::threadECHO	2014-04-10 --- >>
	˵����������ͻ��˽��й�ͨ���̡߳�
	������
	parg	= ��ͻ������ӵ��׽���
	����ֵ��0=�ɹ�
*****************************************************************************************/
unsigned __stdcall socket::threadECHO(void *parg)
{
	const int len = 4096;
	char buf[len];
	SOCKET scSub = *(SOCKET*)parg;
	int rc = recv(scSub, buf, len, 0);
	while (rc != SOCKET_ERROR && rc > 0)
	{
		buf[rc] = '\0';
		if(::send(scSub, buf, rc, 0) == SOCKET_ERROR)
		{
			//errorInfo("echo����������ʧ�ܣ�");
#ifdef _DEBUG
			std::cout << "echo����������ʧ�ܣ�" << std::endl;
#endif
			return -1;
		}
		std::cout << int(scSub) << ":" << buf << std::endl;
		rc = recv(scSub, buf, len, 0);
	}
	if(rc == SOCKET_ERROR)
		return -2;
#ifdef _DEBUG
	std::cout << scSub << "����������" << std::endl;
#endif
	closesocket(scSub);
	return 0;
}

/*****************************************************************************************
	<< --- static socket::threadHeartBeat	2014-05-04 --- >>
	˵�������������˽��й�ͨ���̡߳�
		�Թ̶�ʱ��SOCKET_HEARTBEAT_INTERVAL������˷�����������
	������
	parg	= ���������ӵ��׽���
	����ֵ��0=�ɹ�
*****************************************************************************************/
unsigned __stdcall socket::threadHeartBeat(void *parg)
{
	char buf[] = "heart beat";
	SCDataType data(std::begin(buf), std::end(buf));
	SOCKET scSub = *(SOCKET*)parg;
	while(true)
	{
#if defined(_DEBUG)
		std::cout << "start send heart beat..." << std::endl;
#endif
		if(!send(scSub, data))
		{
#if defined(_DEBUG)
			char buf[40] = {0};
			std::sprintf(buf, "����������ʧ�ܣ�%d", ::WSAGetLastError());
			std::cout << buf << std::endl;
#endif
			closesocket(scSub);
			break;
		}
		Sleep(SOCKET_HEARTBEAT_INTERVAL);
	}
	return 0;
}
/*****************************************************************************************
	<< --- socket::connect	2014-04-08 --- >>
	˵�����������׽��֣�һ�㱻�ͻ��˵���
	������
	phost		= �����ӵ��豸�������ƻ�IP��ַ���磺www.163.com ��192.168.1.55
	pservice	= ���Ӳ��õķ�����������˿ںţ��磺daytime ��3328
	transport	= ����Э�飬�磺tcp��udp
	����ֵ���Ƿ����ӳɹ�
*****************************************************************************************/
bool socket::connect(const char *phost, const char *pservice, const char* ptransport)
{
	m_csHost = phost?phost:"";
	m_csService = pservice? pservice: "";
	m_csTransport = ptransport ? ptransport : "";
	return connect();
}
bool socket::connect()
{
	sockaddr_in scaddr = {0};
	scaddr.sin_family = AF_INET;
	// ����˿ں�
	if(servent *pse = getservbyname(m_csService.c_str(), m_csTransport.c_str()))
		scaddr.sin_port = pse->s_port;
	else if((scaddr.sin_port = htons((u_short)std::atoi(m_csService.c_str()))) == 0)
	{
		errorInfo("�޷��õ�����%s������ڵ㣡", m_csService.c_str());
		return false;
	}
	// ����IP��ַ
	if(hostent *phe = gethostbyname(m_csHost.c_str()))
		memcpy_s(&scaddr.sin_addr, sizeof(scaddr.sin_addr), phe->h_addr, phe->h_length);
	else if((scaddr.sin_addr.s_addr = inet_addr(m_csHost.c_str())) == INADDR_ANY)
	{
		errorInfo("�޷�ʶ���������ַ��%s����", m_csHost.c_str());
		return false;
	}
	// ����Э��
	protoent *ppe = getprotobyname(m_csTransport.c_str());
	if(!ppe)
	{
		errorInfo("�޷��õ���Э�顰%s����ص���Ϣ��", m_csTransport.c_str());
		return false;
	}
	if(_stricmp(m_csTransport.c_str(), "udp") == 0)
		m_beType = BESocketTypeDgram;
	else
		m_beType = BESocketTypeStream;
	// create socket
	m_scMain = ::socket(PF_INET, int(m_beType), ppe->p_proto);
	if(m_scMain == INVALID_SOCKET)
	{
		errorInfo("�޷������׽��֣�");
		return false;
	}
	// connect socket
	if(::connect(m_scMain, (sockaddr *)&scaddr, sizeof(scaddr)) == SOCKET_ERROR)
	{
		closesocket(m_scMain);
		m_scMain = INVALID_SOCKET;
		errorInfo("�޷��򿪵�%s.%s�����ӣ�", m_csHost.c_str(), m_csService.c_str());
		return false;
	}
	//#ifdef _DEBUG
	std::cout << "���ӵ�����:" << m_csHost.c_str() << ":" << m_csService.c_str() << "(" << scaddr.sin_port << ") " << m_csTransport.c_str() << std::endl;
	//#endif

	return true;
}
/*****************************************************************************************
	<< --- socket::daytime	2014-04-10 --- >>
	˵������daytime����ȡ��ʱ��
	������
	now		= ����ȡ�õķ�����ʱ��
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
 bool socket::daytime(std::time_t &now)
{
	memset(&now, 0, sizeof(now));
	std::string msg("what time is it?");
	::send(m_scMain, msg.data(), msg.length(), 0);
	// 
	SCDataType data;
	if(receive(data) && data.size() >= sizeof(now))
	{
		memcpy_s(&now, sizeof(now), data.data(), sizeof(now));
		now = ntohl((u_long)now);
		now -= SOCKET_WINEPOCH;
	}
	else
	{
		errorInfo("����daytime��������ʧ�ܣ�");
		return false;
	}
	return true;

}


/*****************************************************************************************
	<< --- socket::passive	2014-04-09 --- >>
	˵�������������׽��֣�һ�㱻����˵���
	������
	pservice	= ���Ӳ��õķ�����������˿ںţ��磺daytime ��3328
	ptransport	= ����Э�飬�磺tcp��udp
	queueLen	= ��ϣ��������������еĳ��ȣ�����UDPʱ�ò�����Ч
	����ֵ���Ƿ����ӳɹ�
*****************************************************************************************/
bool socket::passive(const char *pservice, const char* ptransport, int32_t queueLen)
{
	sockaddr_in scaddr = {0};
	scaddr.sin_family = AF_INET;
	scaddr.sin_addr.s_addr = INADDR_ANY;
	// ��������˿ں�
	if(servent *pse = getservbyname(pservice, ptransport))
		scaddr.sin_port = htons(ntohs(pse->s_port)+mc_unBaseport);
	else if((scaddr.sin_port = htons((u_short)atoi(pservice))) == 0)
	{
		errorInfo("�޷��õ�����%s������ڵ㣡", pservice);
		return false;
	}
	// ����Э��
	protoent *ppe = getprotobyname(ptransport);
	if(!ppe)
	{
		errorInfo("�޷��õ���Э�顰%s����ص���Ϣ��", ptransport);
		return false;
	}
	if(_stricmp(ptransport, "udp") == 0)
		m_beType = BESocketTypeDgram;
	else
		m_beType = BESocketTypeStream;
	// create socket
	m_scMain = ::socket(PF_INET, int(m_beType), ppe->p_proto);
	if(m_scMain == INVALID_SOCKET)
	{
		errorInfo("�޷������׽��֣�");
		return false;
	}
	// bind socket
	if(bind(m_scMain, (sockaddr *)&scaddr, sizeof(scaddr)) == SOCKET_ERROR)
	{
		errorInfo("�޷������%s���˿��ϣ�", pservice);
		return false;
	}
	if(m_beType == BESocketTypeStream && listen(m_scMain, queueLen) == SOCKET_ERROR)
	{
		errorInfo("�޷������ԡ�%s���˿ڵ�������", pservice);
		return false;
	}
#ifdef _DEBUG
	std::cout << "����������:" << pservice << "(" << scaddr.sin_port << ") " << ptransport << std::endl;
#endif
	return true;
}

/*****************************************************************************************
	<< --- socket::serviceDaytimeUDP	2014-04-10 --- >>
	˵������������daytime����ı����׽���
	������
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::serviceDaytimeUDP()
{
	const int len = 2048;
	char buf[len] = {0};
	sockaddr_in clientAddr = {0};
	int clientAddrLen = sizeof(clientAddr);
	std::time_t now = {0};
	if(passive("daytime", "udp", 0))
	{
		while(true)
		{
			if(recvfrom(m_scMain, buf, len, 0, (sockaddr *)&clientAddr, &clientAddrLen) == SOCKET_ERROR)
			{
				errorInfo("ִ��daytime����Ľ�������ʧ�ܣ�");
				return false;
			}
#ifdef _DEBUG
			std::cout << "���յ�" << inet_ntoa(clientAddr.sin_addr) << " ������ ..." << std::endl;
#endif
			std::time(&now);
			now = htonl((u_long)(now + SOCKET_WINEPOCH));
			sendto(m_scMain, (char *)&now, sizeof(now), 0, (sockaddr *)&clientAddr, clientAddrLen);
		}

	}
	else
	{
		errorInfo("����daytime����ʧ�ܣ�");
		return false;
	}
	return true;
}

/*****************************************************************************************
	<< --- socket::serviceECHOTCP	2014-04-10 --- >>
	˵������������echo����ı����׽��֣����ò�����ʽ��
	������
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::serviceECHOTCP()
{
	const int len = 2048;
	char buf[len] = {0};
	sockaddr_in clientAddr = {0};
	int clientAddrLen = sizeof(clientAddr);
	SOCKET scClient;
	if(passive("echo", "tcp", 10))
	{
		while(true)
		{
			if((scClient = accept(m_scMain, (sockaddr *)&clientAddr, &clientAddrLen)) == INVALID_SOCKET)
			{
				errorInfo("ִ��accept����ʧ�ܣ�");
				return false;
			}
#ifdef _DEBUG
			std::cout << "���յ�" << inet_ntoa(clientAddr.sin_addr) << " ������ ..." << std::endl;
#endif
			if(_beginthread((void (*)(void*))threadECHO, SOCKET_STKSIZE, (void*)scClient) < 0)
			{
				errorInfo("����echo�������߳�ʧ�ܣ�%d", strerror(errno));
			}
		}

	}
	else
	{
		errorInfo("����daytime����ʧ�ܣ�");
		return false;
	}
	return true;
}

/*****************************************************************************************
	˵������������port�˿ڵ������������񣻲��ò�����ʽ��
		�Խ��յ����ݼ���һ��ͷ��ǽ��лط���
	������
	port = �˿ں�
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::serviceHeartBeat(uint16_t port)
{
	std::string hbHead = "HB";

	const int len = 2048;
	char buf[len] = {0};
	sockaddr_in clientAddr = {0};
	int clientAddrLen = sizeof(clientAddr);
	SOCKET scaccept;
	char pport[10] = {0};
	std::sprintf(pport, "%u", port);
	u_long nonblock = 1;
	FD_SET fdread;
	timeval tv = {2, 0};	// 2��
	std::list<BSSocketInfo *>::iterator iter;
	if(passive(pport, "tcp", 10))
	{
		if(ioctlsocket(m_scMain, FIONBIO, &nonblock) == SOCKET_ERROR)
		{
			errorInfo("ִ��ioctlsocket����ʧ�ܣ�");
			return false;
		}
		while(true)
		{
			FD_ZERO(&fdread);
			FD_SET(m_scMain, &fdread);
			// ���Խ��������Ӽ���
			for (iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++iter)
			{
				FD_SET((*iter)->scObject, &fdread);
			}
			int rt = select(0, &fdread, nullptr, nullptr, &tv);
			if (rt == 0)
			{
				continue;	// ��ʱ��������
			}
			else if(rt == SOCKET_ERROR)
			{
				errorInfo("����ʧ�ܣ�");
				break;
			}
			// �Ƿ���������������
			if (FD_ISSET(m_scMain, &fdread))
			{
				if (m_lstSocket.size() >= FD_SETSIZE)
				{
					errorInfo("������������������%d", FD_SETSIZE);
					continue;
				}
				if((scaccept = accept(m_scMain, (sockaddr *)&clientAddr, &clientAddrLen)) != INVALID_SOCKET)
				{
					if(ioctlsocket(scaccept, FIONBIO, &nonblock) == SOCKET_ERROR)
					{
						errorInfo("ִ��ioctlsocket����ʧ��2��");
						return false;
					}
					m_lstSocket.push_front(new BSSocketInfo(scaccept));
				}			
			}
			// ��������
			for(iter = m_lstSocket.begin(); iter != m_lstSocket.end(); ++iter)
			{
				if (FD_ISSET((*iter)->scObject, &fdread))
				{
					if (receive(*iter))
					{
#if defined(_DEBUG)
						char buf[SOCKET_RECVBUFF_LEN] = {0};
						//std::strncpy (buf, (*iter)->dataRecv.data(), SOCKET_RECVBUFF_LEN);
						//std::copy((*iter)->dataRecv.begin(), (*iter)->dataRecv.end(), buf);
						std::sprintf(buf, "%d %d %s", (*iter)->scObject, (long)(*iter)->tmLastRecv, (*iter)->dataRecv.data());
						std::cout << buf << std::endl;
#endif
						SCDataType data(std::begin(SOCKET_HEARTBEAT_HEADER), std::end(SOCKET_HEARTBEAT_HEADER));
						data.insert(data.end(), (*iter)->dataRecv.begin(), (*iter)->dataRecv.end());
						this->send((*iter)->scObject, data);
					}
					else
					{	// �ͷŹرյ�����
						std::cout << "����ʧ�ܣ�" << WSAGetLastError() << std::endl;
						//errorInfo("����ʧ�ܣ�"); 
						closesocket((*iter)->scObject);
						FD_CLR((*iter)->scObject, &fdread);
						delete (*iter);
						m_lstSocket.erase(iter);
						break;
					}
				}
			}
		}
		// ����ѽ���������

	}
	else
	{
		errorInfo("����HeartBeat����ʧ�ܣ�");
		return false;
	}
	return true;
}

/*****************************************************************************************
	<< --- socket::receive	2014-04-09 --- >>
	˵������������
	������
	sc		= �׽���
	vecData	= ��ű����յ�����
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::receive(SOCKET sc, SCDataType &vecData)
{
	uint8_t buf[SOCKET_RECVBUFF_LEN] = {0};
	int rc = 0;
	vecData.clear();
	// ��������
	//switch (m_beType)
	//{
	//case socket::BESocketTypeStream:
	//	{
			rc = recv(sc, (char *)buf, SOCKET_RECVBUFF_LEN, 0);
			if (rc == SOCKET_ERROR )
			{
				return false;
			}
			while (rc > 0)
			{
				if(rc == SOCKET_ERROR)
				{
					vecData.clear();
					return false;
				}
				vecData.insert(vecData.cend(), std::begin(buf), std::begin(buf)+rc);
				// 
				if(rc < SOCKET_RECVBUFF_LEN)
					break;
				rc = recv(sc, (char *)buf, SOCKET_RECVBUFF_LEN, 0);
			}
	//	}

	//	break;
	//case socket::BESocketTypeDgram:
	//	{
	//		rc = recv(sc, (char *)buf, SOCKET_RECVBUFF_LEN, 0);
	//		if(rc == SOCKET_ERROR)
	//		{
	//			vecData.clear();
	//			return false;
	//		}
	//		vecData.insert(vecData.cend(), std::begin(buf), std::begin(buf)+rc);
	//	}

	//	break;
	//default:
	//	break;
	//}	
	return true;
}
bool socket::receive(BSSocketInfo *pscinfo)
{
	assert(pscinfo && pscinfo->scObject != INVALID_SOCKET);
	bool bl = false;
	if(bl = receive(pscinfo->scObject, pscinfo->dataRecv))
	{
		std::time(&pscinfo->tmLastRecv);
	}
	return bl;
}
bool socket::receive(SCDataType &vecData)
{
	return receive(m_scMain, vecData);
}

/*****************************************************************************************
	<< --- socket::send	2014-04-10 --- >>
	˵������������
	������
	sc		= �׽���
	vecData	= �����͵�����
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::send(SOCKET sc, const SCDataType &vecData)
{
	//char buf[SOCKET_RECVBUFF_LEN] = {0};
	//std::copy(vecData.begin(), vecData.end(), buf);
	//return ::send(sc, buf, vecData.size(), 0) != SOCKET_ERROR;
	return ::send(sc, vecData.data(), vecData.size(), 0) != SOCKET_ERROR;
}
bool socket::send(const SCDataType &vecData)
{
	//assert(!vecData.empty());
	return this->send(m_scMain, vecData);// ::send(m_scMain, vecData.data(), vecData.size(), 0) != SOCKET_ERROR;
}


/*****************************************************************************************
	<< --- socket::clientEcho	2014-04-09 --- >>
	˵����ִ��echo����Ŀͻ���
	������
	sendData	= �����͵�����
	recvData	= ���յ�����
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::clientEcho(const std::string &sendData, std::string &recvData)
{
	// ��������
	::send(m_scMain, sendData.c_str(), sendData.length(), 0);

	return true;
}

/*****************************************************************************************
	<< --- socket::clientHeartBeat	2014-04-09 --- >>
	˵����ִ������������Ŀͻ���
	�ڲ�����һ�����߳�threadHeartBeat���Թ̶���������������������
	������
		phostip	= Ŀ����������IP
		port	= ͨѶ�˿ں�
	����ֵ���Ƿ�ִ�гɹ�
*****************************************************************************************/
bool socket::clientHeartBeat(const char *phostip, const uint16_t port)
{
	char sport[10] = {0};
	std::sprintf(sport, "%u", port);
	if (connect(phostip, sport, "tcp"))
	{
		_beginthreadex(nullptr, 0, socket::threadHeartBeat, &m_scMain, 0, nullptr);
	}
	else
	{
		errorInfo("�򿪵�%s:%u����������ʧ�ܣ�", phostip, port);
	}
	return true;
}

}