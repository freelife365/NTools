#ifndef __SOCKCACHE_H__
#define __SOCKCACHE_H__
#pragma once
#include <cstdint>
#include <deque>
#include <vector>
#include <queue>
#include <iostream>
#include <cstdio>
#include <algorithm>

#include "GT/gtcmd.h"
#include "gt/gtdcsdata.h"
#include "AC/sysupport.h"

#if defined(_WIN32)
	#include <afxwin.h>         // MFC ��������ͱ�׼���
	#include <afxext.h>         // MFC ��չ
	#include <afxframewndex.h>
	#include <ATLComTime.h>
	#include <comutil.h>
	#include <afxdisp.h>
	#include <comdef.h>
#endif

namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	sockcache��	2014-04-29
//	GT socket���ա����ͻ�����
//	1.����TCPͨѶ���ٶ������ݣ��ְ����͵ģ���˳�򵽴�
//	2.�������ֽ��ա����������������棻�����ٽ��ͬ������
//	3.�����߳�
//	4.���մ����߳�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class sockcache
{
//public:
//	typedef struct BSGTRecvData{
//		uint32_t uiAddr;				// �������ݵ��豸��ַ
//		std::vector<char>	vecData;	// ���յ�ԭʼ���ݣ�����������'0D'��
//		bool	isEnd;					// �Ƿ������ɣ������Ƿ���յ�������'0D')
//		// 
//		BSGTRecvData()
//			: uiAddr(0), vecData(40)
//		{
//		}
//	}BSGTRecvData;
public:
	sockcache()
		//: m_wndUser(NULL), m_scMain(NULL)
		//, m_hdlSenderThread(NULL), m_uiSenderThreadID(0), m_lSenderThreadSwitch(0)
		//, m_hdlRecverThread(NULL), m_uiRecverThreadID(0), m_lRecverThreadSwitch(0)
	{

	}
public:
	///*****************************************************************************************
	//	<< --- sockcache::open		2014-04-29 --- >>
	//	˵���������ӣ�׼�����ա��������ݣ����������ա����ʹ����߳�
	//	������
	//	wndUser		= �����ߴ��ھ��
	//	����ֵ���Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//bool open(HWND wndUser, SOCKET scsend);
	///*****************************************************************************************
	//	<< --- sockcache::close		2014-04-29 --- >>
	//	˵�����ر����ӣ�׼�����ա��������ݣ���ֹͣ���ա����ʹ����߳�
	//	������
	//	����ֵ���Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//bool close();

	/*****************************************************************************************
		<< --- sockcache::pushRecvData		2014-04-29 --- >>
		˵������socket���յ�������ֱ��ѹ����ջ�����У��Ա�doThreadAnalysisRecvData�߳̽���
		������
		data	= �����յ����ݣ�������ճ��
		����ֵ��
	*****************************************************************************************/
	void pushRecvData(const SCDataType &data)
	{
		if(data.size() <= 0)
			return;
		// 
		wchar_t buf[200] = {0};
		std::swprintf(buf, 200, L"%s(%d)", gtc::convert::toStringHex(data));
		gtc::app::writeLogs(buf,  L"pushRecvData Hex");
		// 
		m_lockRecver.Lock();
		m_queRecver.push(data);
		m_lockRecver.Unlock();
	}
	/*****************************************************************************************
		<< --- sockcache::popRecvData		2014-05-09 --- >>
		˵������socket���ն�ȡ�������Ӧ������
		������
		vecRecv	= ����Ӧ������Ӧ�������ճ�������Բ�������
		����ֵ���Ƿ���Ӧ������
	*****************************************************************************************/
	bool popRecvData(std::vector<SCDataType> &vecRecv);

	/*****************************************************************************************
		<< --- sockcache::pushGTCmd		2014-05-07 --- >>
		˵�������û�����ֱ��ѹ�뷢�ͻ�����У��Ա�doThreadSendGTCmd�̷߳���
		������
		pcmd	= 
		����ֵ��
	*****************************************************************************************/
	void pushGTCmd(gtcmd *pcmd)
	{
		assert(pcmd != NULL);
		m_lockSender.Lock();
		m_queSender.push(pcmd);
		m_lockSender.Unlock();
	}
	gtcmd* popGTCmd()
	{
		gtcmd *pcmd = nullptr;
		m_lockSender.Lock();
		if (!m_queSender.empty())
		{
			pcmd = m_queSender.front();
			m_queSender.pop();
		}
		m_lockSender.Unlock();
		return pcmd;
	}
	bool isEmptySender()  
	{
		bool bl = false;
		m_lockSender.Lock();
		bl = m_queSender.empty();
		m_lockSender.Unlock();
		return bl;
	}
	bool isEmptyRecver()  
	{
		bool bl = false;
		m_lockRecver.Lock();
		bl = m_queRecver.empty();
		m_lockRecver.Unlock();
		return bl;
	}
public:
	/*****************************************************************************************
		<< --- static sockcache::analysisRecvData		2014-04-30 --- >>
		˵������ճ�����ݷָ�����յ���Ӧ�𰴷ָ���0Dh�ָ����0Dh��
		������
		data	= Ӧ����������
		results	= �ָ���Ӧ������
		����ֵ���Ƿ���Ҫ�ָ�
	*****************************************************************************************/
	static bool analysisRecvData(const std::vector<char> &data, std::vector<std::vector<char>> &results);
	//// �߳�
	///*****************************************************************************************
	//	<< --- static sockcache::doThreadSendGTCmd		2014-04-30 --- >>
	//	˵������������������в�ͣȡ���������
	//	������
	//	pdata	= sockcache*
	//	����ֵ��
	//*****************************************************************************************/
	//static unsigned __stdcall doThreadSendGTCmd(void *pdata);
	///*****************************************************************************************
	//	<< --- static sockcache::doThreadSendGTCmd		2014-04-30 --- >>
	//	˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
	//	������
	//	pdata	= sockcache*
	//	����ֵ��
	//*****************************************************************************************/
	//static unsigned __stdcall doThreadAnalysisRecvData(void *pdata);
public:
	////int getLinkIndex() {		return m_iLinkIndex;			}
	//HWND getWndUser() {		return m_wndUser;				}
	//void setMainSocket(SOCKET sc) {				m_scMain = sc;				}
private:
	// ���ͻ�����
	std::queue<gtc::gtcmd *>	m_queSender;
	CCriticalSection			m_lockSender;			// ���ͻ���ͬ����
	std::queue<SCDataType>		m_queRecver;
	CCriticalSection			m_lockRecver;			// ���ջ���ͬ����
	// ��������
	//int		m_iLinkIndex;
	// �����ߴ��ھ��������������Ϣ
	//HWND	m_wndUser;
	//SOCKET	m_scMain;	// ������������
	//HANDLE						m_hdlSenderThread;
	//unsigned int				m_uiSenderThreadID;
	//LONG						m_lSenderThreadSwitch;	// �̵߳Ŀ���
	//// ���ջ�����
	//HANDLE						m_hdlRecverThread;
	//unsigned int				m_uiRecverThreadID;
	//LONG						m_lRecverThreadSwitch;	// �̵߳Ŀ���


};




}

#endif