#ifndef __NETWRAP_H__
#define __NETWRAP_H__

#define   _WINSOCKAPI_   //   ��ֹwindows.h����winsock.h

#include "AC/basedef.h"
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define NET_DEF_PACKET_SIZE			32
#define NET_ECHO_REQUEST			8
#define NET_ECHO_REPLY				0

typedef struct {
	BYTE		byVerHLen;			//4λ�汾+4λ�ײ�����
	BYTE		byTOS;				//��������
	USHORT		usTotalLen;			//�ܳ���
	USHORT		usID;				//��ʶ
	USHORT		usFlagFragOffset;	//3λ��־+13λƬƫ��
	BYTE		byTTL;				//TTL
	BYTE		byProtocol;			//Э��
	USHORT		usHChecksum;		//�ײ������
	ULONG		ulSrcIP;			//ԴIP��ַ
	ULONG		ulDestIP;			//Ŀ��IP��ַ
}BSIPHeader;

typedef struct { 
	BYTE		byType;				//����
	BYTE		byCode;				//����
	USHORT		usChecksum;			//����� 
	USHORT		usID;				//��ʶ��
	USHORT		usSeq;				//���
	ULONG		ulTimeStamp;		//ʱ������Ǳ�׼ICMPͷ����
}BSICMPHeader;
typedef struct 
{
	USHORT		usSeq;
	DWORD		dwRoundTripTime;
	DWORD		dwBytes;
	DWORD		dwTTL;
}BSPingReply;
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	pingCmd  ��		2015-10-22
//	��װping����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class pingCmd 
{
public:
	pingCmd();
	~pingCmd();

	bool ping(DWORD dwDestIP, BSPingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
	bool ping(char *szDestIP, BSPingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
private:
	bool pingCore(DWORD dwDestIP, BSPingReply *pPingReply, DWORD dwTimeout);
	USHORT calCheckSum(USHORT *pBuffer, int nSize);
	ULONG getTickCountCalibrate();
private:
	SOCKET		m_sockRaw; 
	WSAEVENT	m_event;
	USHORT		m_usCurrentProcID;
	char		*m_szICMPData;
	bool		m_bIsInitSucc;
private:
	static USHORT mc_usPacketSeq;
};

}	// gtc
#endif	// __NETWRAP_H__