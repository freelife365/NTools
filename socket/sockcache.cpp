#include "sockcache.h"
#include "gt/gtdcsdata.h"
namespace gtc {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	sockcache��	2014-04-29
//	GT socket���ա����ͻ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////

///*****************************************************************************************
//	<< --- sockcache::open		2014-04-29 --- >>
//	˵���������ӣ�׼�����ա��������ݣ����������ա����ʹ����߳�
//	������
//	wndUser		= �����ߴ��ھ��
//	����ֵ���Ƿ�ִ�гɹ�
//*****************************************************************************************/
//bool sockcache::open(HWND wndUser, SOCKET scsend)
//{
//	m_wndUser = wndUser;
//	m_scMain = scsend;
//	// �������������߳�
//	m_hdlSenderThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadSendGTCmd, this, CREATE_SUSPENDED, &m_uiSenderThreadID);
//	if(m_hdlSenderThread)
//	{
//		InterlockedCompareExchange(&m_lSenderThreadSwitch, 1, 0);
//		ResumeThread(m_hdlSenderThread);
//	}
//	else
//	{
//		return false;
//	}
//	// �����������ݷ����߳�
//	m_hdlRecverThread = (HANDLE)_beginthreadex(nullptr, 0, doThreadAnalysisRecvData, this, CREATE_SUSPENDED, &m_uiRecverThreadID);
//	if(m_hdlRecverThread)
//	{
//		InterlockedCompareExchange(&m_lRecverThreadSwitch, 1, 0);
//		ResumeThread(m_hdlRecverThread);
//	}
//	else
//	{
//		return false;
//	}
//
//
//	return true;
//}
//
///*****************************************************************************************
//	<< --- sockcache::close		2014-04-29 --- >>
//	˵�����ر����ӣ�׼�����ա��������ݣ���ֹͣ���ա����ʹ����߳�
//	������
//	����ֵ���Ƿ�ִ�гɹ�
//*****************************************************************************************/
//bool sockcache::close()
//{
//	// �����������ݣ���������������
//
//	// �رշ��������߳�
//	InterlockedCompareExchange(&m_lSenderThreadSwitch, 0, 1);
//	// �رս������ݷ����߳�
//	InterlockedCompareExchange(&m_lRecverThreadSwitch, 0, 1);
//
//	return true;
//}
///*****************************************************************************************
//	<< --- sockcache::pushRecvData		2014-04-29 --- >>
//	˵����
//	������
//	data	= �����յ����ݣ�������ճ��
//	����ֵ��
//*****************************************************************************************/
//void sockcache::pushRecvData(const SCDataType &data)
//{
//	if(data.size() <= 0)
//		return;
//	// 
//	wchar_t buf[200] = {0};
//	std::swprintf(buf, 200, L"%s(%d)", gtc::convert::toStringHex(data));
//	gtc::app::writeLogs(buf,  L"pushRecvData Hex");
//	// 
//	m_lockRecver.Lock();
//	m_queRecver.push(data);
//	//m_queRecver.push(std::vector<char>(pdata, pdata+dataLen/*/sizeof(char)*/));
//	m_lockRecver.Unlock();
//}
//
///*****************************************************************************************
//	<< --- sockcache::pushGTCmd		2014-05-07 --- >>
//	˵�������û�����ֱ��ѹ�뷢�ͻ�����У��Ա�doThreadSendGTCmd�̷߳���
//	������
//	pcmd	= 
//	����ֵ��
//*****************************************************************************************/
//void sockcache::pushGTCmd(gtcmd *pcmd)
//{
//
//}

/*****************************************************************************************
	<< --- static sockcache::analysisRecvData		2014-04-30 --- >>
	˵�������յ���Ӧ�𰴷ָ���0Dh�ָ����0Dh��
	������
	data	= Ӧ����������
	results	= �ָ���Ӧ������
	����ֵ���Ƿ���Ҫ�ָ�
*****************************************************************************************/
bool sockcache::analysisRecvData(const std::vector<char> &data, std::vector<std::vector<char>> &results)
{
	results.clear();
	std::vector<char>::const_iterator itbegin = data.cbegin(), itf;
	while((itf = std::find(itbegin, data.cend(), _GTCMD_RECVTYPE_EOF)) != data.cend())
	{
		results.push_back(std::vector<char>(itbegin, ++itf));
		itbegin = itf;
	}
	return results.size() > 0;
}
/*****************************************************************************************
	<< --- sockcache::popRecvData		2014-05-09 --- >>
	˵������socket���ն�ȡ�������Ӧ������
	������
	vecRecv	= ����Ӧ������Ӧ�������ճ�������Բ�������
	����ֵ���Ƿ���Ӧ������
*****************************************************************************************/
bool sockcache::popRecvData(std::vector<SCDataType> &vecRecv)
{
	vecRecv.clear();
	SCDataType data;
	bool isvalid = false;
	std::vector<SCDataType> cmds;
	std::vector<SCDataType>::iterator iter;
	m_lockRecver.Lock();
	while(!m_queRecver.empty())
	{
		data = m_queRecver.front();
		m_queRecver.pop();
		if (!analysisRecvData(data, cmds))	// �鿴�Ƿ���ճ�����еĻ��ͷֽ⵽cmds��
		{
			cmds.push_back(data);
		}
		// 
		for(iter = cmds.begin(); iter != cmds.end(); ++iter)
		{
			switch(iter->front())
			{
			case _GTCMD_RECVTYPE_G:
			case _GTCMD_RECVTYPE_Pn:
			case _GTCMD_RECVTYPE_H:
			case _GTCMD_RECVTYPE_Ac:
				data.assign(iter->begin(), iter->end());
				isvalid = true;
				break;
			default:
				if(isvalid)
				{
					data.insert(data.end(), iter->begin(), iter->end());
				}
				break;
			}
			// Ӧ�����
			if(data.size() > 0 && data.back() == _GTCMD_RECVTYPE_EOF && isvalid)
			{
				vecRecv.push_back(data);
				isvalid = false;
			}
		}
	}
	m_lockRecver.Unlock();

	return !vecRecv.empty();
}
///*****************************************************************************************
//	<< --- static sockcache::doThreadSendGTCmd		2014-04-30 --- >>
//	˵������������������в�ͣȡ���������
//	������
//	pdata	= sockcache*
//	����ֵ��
//*****************************************************************************************/
//unsigned __stdcall sockcache::doThreadSendGTCmd(void *pdata)
//{
//	sockcache *pcatch = (sockcache*)pdata;
//	assert(pcatch != nullptr);
//	SCDataType data;
//	gtc::app::writeLogs(L"begin", L"doThreadSendGTCmd", L"sock");
//	while(pcatch->m_lSenderThreadSwitch == 1)
//	{	// 
//		pcatch->m_lockSender.Lock();
//		while(!pcatch->m_queSender.empty())
//		{
//			// ����һ������
//			gtcmd *pcmd = pcatch->m_queSender.front();	// swap
//			pcatch->m_queSender.pop();
//			if (pcmd->getCmd(data))
//			{
//				if (socket::send(pcatch->m_scMain, data))
//				{
//					if(gtcmdH *ph = dynamic_cast<gtcmdH *>(pcmd))
//						ph->refreshTime();
//				}
//				else
//				{
//#if defined(_DEBUG)
//					SendMessage(pcatch->m_wndUser, WM_Pn, 8, (LPARAM)"���ͳ���");
//#endif
//				}
//			}
//			if(pcmd && pcmd->getCmdType() != BEGTSocketCmdH)	// �����������ظ�new
//			{
//				delete pcmd;
//			}
//		}
//		pcatch->m_lockSender.Unlock();
//
//	}
//	gtc::app::writeLogs(L"end", L"doThreadSendGTCmd", L"sock");
//	return 0;
//}
//
///*****************************************************************************************
//	<< --- static sockcache::doThreadSendGTCmd		2014-04-30 --- >>
//	˵��������Ϣ�����������в�ͣȡ��Ϣ���������������windows��Ϣ��ʽ�͸��û�
//	������
//	pdata	= sockcache*
//	����ֵ��
//*****************************************************************************************/
//unsigned __stdcall sockcache::doThreadAnalysisRecvData(void *pdata)
//{
//	sockcache *pcatch = (sockcache*)pdata;
//	assert(pcatch);
//	std::vector<char> vecdata;
//	UINT cmdflag = 0;
//	//int linkIndex = pcatch->getLinkIndex();
//	bool isvalid = false;
//	uint32_t gidx = 0;
//	wchar_t buf[1000] = {0};
//	std::vector<std::vector<char>> vecCmds;
//	std::vector<std::vector<char>>::iterator itbegin;
//	gtc::app::writeLogs(L"begin", L"doThreadAnalysisRecvData", L"sock");
//	while(pcatch->m_lRecverThreadSwitch == 1 /* || !m_queRecver.empty()*/)
//	{	// G74 Pn64 H68
//		++gidx;
//		while(!pcatch->m_queRecver.empty())
//		{
//			// ����һ������
//			pcatch->m_lockRecver.Lock();
//			std::vector<char> data = pcatch->m_queRecver.front();	// swap
//			std::swprintf (buf, _countof(buf), L"Thread(%02d %04u)", pcatch->m_queRecver.size(), gidx%1000);
//			gtc::app::writeLogs(gtc::convert::toStringHex(data.data(),data.size()).c_str(), buf);
//			pcatch->m_queRecver.pop();
//			pcatch->m_lockRecver.Unlock();
//			if(!analysisRecvData(data, vecCmds))
//				vecCmds.push_back(data);
//			for(itbegin = vecCmds.begin(); itbegin != vecCmds.end(); ++itbegin)
//			{
//				switch(itbegin->front())
//				{
//				case _GTCMD_RECVTYPE_G:
//					vecdata.assign(itbegin->begin(), itbegin->end());
//					cmdflag = WM_G;
//					isvalid = true;
//					break;
//				case _GTCMD_RECVTYPE_Pn:
//					vecdata.assign(itbegin->begin(), itbegin->end());
//					cmdflag = WM_Pn;
//					isvalid = true;
//					break;
//				case _GTCMD_RECVTYPE_H:
//					vecdata.assign(itbegin->begin(), itbegin->end());
//					cmdflag = WM_H;
//					isvalid = true;
//					break;
//				default:
//					if(isvalid)
//					{
//						vecdata.insert(vecdata.end(), itbegin->begin(), itbegin->end());
//					}
//					break;
//				}
//				if(vecdata.size() > 0 && vecdata.back() == _GTCMD_RECVTYPE_EOF && isvalid)
//				{
//					// ������Ϣ���û�
//					//if(vecdata.size() > 0)
//					//{
//						std::swprintf (buf, _countof(buf), L"MSG:(%02d %04u)  ", vecdata.size(), gidx);
//						gtc::app::writeLogs(gtc::convert::toStringHex(vecdata.data(), vecdata.size()).c_str(), buf);
//						::PostMessage(pcatch->m_wndUser, cmdflag, (WPARAM)vecdata.size(), (LPARAM)vecdata.data());//pdata);
//						isvalid = false;
//						cmdflag = 0;
//						Sleep(10);
//					//}
//				}
//			}
//		}
//		
//	}
//	gtc::app::writeLogs(L"end", L"doThreadAnalysisRecvData", L"sock");
//	return 0;
//}


}