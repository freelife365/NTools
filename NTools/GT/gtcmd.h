#ifndef __GTCMD_H__
#define __GTCMD_H__
#pragma once

#include <cstdint>
#include <vector>
#include <ctime>
#include "socket/sockme.h"
#include "gt/gtdcsdata.h"

namespace gtc {
// dll����������ͨ��ָ������
enum BEGTSocketCmd{
	BEGTSocketCmdNone	= 1,
	BEGTSocketCmdZ		,	// Zָ��
	BEGTSocketCmdZ1		,		// Z1ָ���ʼ��������ʾ��
	BEGTSocketCmdAc		,
	BEGTSocketCmdG		,
	BEGTSocketCmdH		,
	BEGTSocketCmdPn		,
	BEGTSocketCmdT
};

static std::wstring toString(BEGTSocketCmd cmd)
{
	switch(cmd)
	{
	case BEGTSocketCmdNone:	//= 1,
		return L"δ����ָ��";
		break;
	case BEGTSocketCmdZ:		//,	// Zָ��
		return L"Zָ��";
		break;
	case BEGTSocketCmdZ1:		//,		// Z1ָ���ʼ��������ʾ��
		return L"Z1ָ��";
		break;
	case BEGTSocketCmdAc:		//,
		return L"Acָ��";
		break;
	case BEGTSocketCmdG:		//,
		return L"Gָ��";
		break;
	case BEGTSocketCmdH:		//,
		return L"Hָ��";
		break;
	case BEGTSocketCmdPn:		//,
		return L"Pnָ��";
		break;
	case BEGTSocketCmdT:
		return L"Tָ��";
		break;
	}
	return L"";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmd��	2014-04-26
//	GT����ָ�����
//	Ϊʲô�������������װ��һ�����ṹ�У���Ϊ��ָ������̫��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmd
{
protected:
	gtcmd() : m_csCmdType(""), m_beCmdType(BEGTSocketCmdNone) {}
public:
	gtcmd(const char *pcmd, BEGTSocketCmd cmd) : m_csCmdType(pcmd ? pcmd : ""), m_beCmdType(cmd) {}
	virtual ~gtcmd() {}
public:
	/*****************************************************************************************
		<< --- gtcmd::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		//if(m_csCmdType.length() <= 0)
		//	return false;
		cmdData.assign(m_csCmdType.begin(), m_csCmdType.end());
		return cmdData.size() > 0;
	}
public:
	/*****************************************************************************************
		<< --- static gtcmd::appendAddr		2014-04-26 --- >>
		˵��������ַ����ascii��ʽ׷�ӵ�cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ��
	*****************************************************************************************/
	static void appendAddr(std::vector<char> &cmdData, uint32_t addr)
	{
		cmdData.push_back(addr/1000%10+'0');
		cmdData.push_back(addr/100%10+'0');
		cmdData.push_back(addr/10%10+'0');
		cmdData.push_back(addr%10+'0');
	}
	/*****************************************************************************************
		<< --- static gtcmd::appendCRC		2014-04-28 --- >>
		˵��������������cmdData����CRCУ�飬������У����
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ��
	*****************************************************************************************/
	static void appendCRC(std::vector<char> &cmdData)
	{
	}
	/*****************************************************************************************
		<< --- static gtcmd::appendEOF		2014-04-26 --- >>
		˵������ָ���������ascii��ʽ׷�ӵ�cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ��
	*****************************************************************************************/
	static void appendEOF(std::vector<char> &cmdData)
	{
		cmdData.push_back(0);
		cmdData.push_back(0xD);
	}
public:
	BEGTSocketCmd	getCmdType() const {			return m_beCmdType;			}
private:
	// ���������
	std::string		m_csCmdType;	
	BEGTSocketCmd	m_beCmdType;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdZ��	2014-04-26
//	GT Zָ�ϵͳ��ʼ����ʱ���ͣ������в�����Ԫ�ĳ�ʼ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdZ : public gtcmd
{
public:
	gtcmdZ()
		: gtcmd("Z", BEGTSocketCmdZ)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdZ::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdZ1��	2014-04-26
//	GT Z1ָ���ʼ��ָ����ʾ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdZ1 : public gtcmd
{
protected:
	gtcmdZ1() : gtcmd() {}
public:
	gtcmdZ1(uint32_t addr)
		: gtcmd("Z1", BEGTSocketCmdZ1) , m_uiAddr(addr)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdZ1::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// ���ַ
		gtcmd::appendAddr(cmdData, m_uiAddr);
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
private:
	// ����ʼ����ʾ���ĵ�ַ
	uint32_t	m_uiAddr;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdAc��	2014-04-26
//	GT Acָ����ñ�ʾ����ַ
//	1.��ַ��������Ҫһ��һ����ִ��
//	2.���ȴ���Zָ��
//	3.Acָ����ڷ���Aָ��֮��ִ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdAc : public gtcmd
{
protected:
	gtcmdAc() : gtcmd() {}
public:
	gtcmdAc(uint32_t oldAddr, uint32_t newAddr)
		: gtcmd("Ac", BEGTSocketCmdAc) , m_uiOldAddr(oldAddr), m_uiNewAddr(newAddr)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdAc::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// ���ַ
		gtcmd::appendAddr(cmdData, m_uiOldAddr);
		gtcmd::appendAddr(cmdData, m_uiNewAddr);
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
private:
	// ��ʾ��ԭ��ַ
	uint32_t	m_uiOldAddr;
	// ��ʾ���µ�ַ
	uint32_t	m_uiNewAddr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	BSGTCMDDataPn�ṹ	2014-04-28
//	GT Pnָ������ݲ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct BSGTCMDDataPn{
	uint16_t	unAddr;		// ��ǩ��ַ
	uint8_t		btModel;	// ��ʾģ���
	std::vector<char>	vecData;	// ģ������
	// 
	BSGTCMDDataPn(uint16_t addr, uint8_t model, const char *pdata, const int dataLen)
		: unAddr(addr), btModel(model), vecData(pdata, pdata+dataLen)
	{
	}
	void buildCmd(std::vector<char> &cmdData)
	{
		// ���ַ
		gtcmd::appendAddr(cmdData, unAddr);
		cmdData.push_back(btModel+'0');
		cmdData.insert(cmdData.end(), vecData.begin(), vecData.end());
		cmdData.push_back(_GTCMD_MODEL_SPLIT);
	}
}BSGTCMDDataPn;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdPn��	2014-04-28
//	GT Pnָ�ˢ�±�ʾ������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdPn : public gtcmd
{
protected:
	gtcmdPn() : gtcmd() {}
public:
	gtcmdPn(uint8_t blkNo)
		: gtcmd("Pn", BEGTSocketCmdPn) , m_btBlkNo(blkNo)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdPn::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// ����
		cmdData.push_back(m_btBlkNo/10%10+'0');
		cmdData.push_back(m_btBlkNo%10+'0');
		//cmdData.push_back(m_btBlkNo);
		// ��ʾ������
		std::vector<BSGTCMDDataPn>::iterator it;
		for(it = m_vecData.begin(); it != m_vecData.end(); ++it)
			it->buildCmd(cmdData);
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
public:
	/*****************************************************************************************
		<< --- gtcmdPn::pushSELData		2014-05-12 --- >>
		˵����������ǩ����
		������
		addr	= ��ǩ��ַ
		model	= ��ʾģ���
		pdata	= ��ʾ����
		dataLen	= ��ʾ���ݳ���
		����ֵ��
	*****************************************************************************************/
	void pushSELData(uint16_t addr, uint8_t model, const char *pdata, const int dataLen)
	{
		m_vecData.push_back(BSGTCMDDataPn(addr, model, pdata, dataLen));
	}
private:
	// �����
	uint8_t		m_btBlkNo;
	// ��ʾ������
	std::vector<BSGTCMDDataPn>	m_vecData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdG��	2014-04-28
//	GT Gָ����������ȡ���Ĺ���״̬����ʼ���գ�ֹͣ���գ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdG : public gtcmd
{
protected:
	gtcmdG() : gtcmd() {}
public:
	gtcmdG(uint32_t addr, bool execOpen)
		: gtcmd("G", BEGTSocketCmdG) , m_uiAddr(addr), m_execOpenOrClose(execOpen)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdG::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// ���ַ
		gtcmd::appendAddr(cmdData, m_uiAddr);
		// ����ָ��
		cmdData.push_back(m_execOpenOrClose?'O':'C');
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
private:
	// �����ȡ����ַ
	uint32_t	m_uiAddr;
	// �����ȡ����ִ�ж�����true����ʼ����'O''o'��false��ֹͣ����'C''c'��
	bool		m_execOpenOrClose;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdT��	2014-04-28
//	GT Tָ�����ָʾ�ƺ͵���I/Oָ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdT : public gtcmd
{
protected:
	gtcmdT() : gtcmd() {}
public:
	gtcmdT(uint32_t addr, uint8_t ioData = 0)
		: gtcmd("T", BEGTSocketCmdT) , m_uiAddr(addr), m_btIOData(ioData)
	{}
public:	
	/*****************************************************************************************
		<< --- gtcmdT::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// ���ַ
		gtcmd::appendAddr(cmdData, m_uiAddr);
		// ����ָ��
		cmdData.push_back(m_btIOData);
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
public:
	void openRedLight() {				m_btIOData |= 0x4;					}
	void closeRedLight() {				m_btIOData &= ~0x4;					}
	void openGreenLight() {				m_btIOData |= 0x2;					}
	void closeGreenLight() {			m_btIOData &= ~0x2;					}
private:
	// �����ȡ����ַ
	uint32_t	m_uiAddr;
	// ����I/O�ı������
	uint8_t		m_btIOData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtcmdH��	2014-04-28
//	GT Hָ�����ָʾ�ƺ͵���I/Oָ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class gtcmdH : public gtcmd
{
public:
	gtcmdH()
		: gtcmd("H", BEGTSocketCmdH)
	{
		std::time(&m_tmLast);
	}
public:	
	/*****************************************************************************************
		<< --- gtcmdT::getCmd		2014-04-26 --- >>
		˵���������ָ����ʽ��֯�ú����cmdData
		������
		cmdData		= ������Ÿ�ʽ���������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool getCmd(std::vector<char> &cmdData)
	{
		if(!gtcmd::getCmd(cmdData))
			return false;
		// CRC
		gtcmd::appendCRC(cmdData);
		// ָ�������
		gtcmd::appendEOF(cmdData);
		return cmdData.size() > 0;
	}
public:
	/*****************************************************************************************
		<< --- gtcmdT::refreshTime		2014-05-08 --- >>
		˵�������¡����һ�γɹ�����ʱ�䡱Ϊ��ǰʱ��
		������
		����ֵ��
	*****************************************************************************************/
	void	refreshTime() {			std::time(&m_tmLast);			}
	/*****************************************************************************************
		<< --- gtcmdT::differenceTime		2014-05-08 --- >>
		˵�������ص�ǰʱ��ࡰ���һ�γɹ�����ʱ�䡱��ʱ����λ�룩
		������
		����ֵ��ʱ����λ�룩
	*****************************************************************************************/
	double	differenceTime() const {
		std::time_t now;
		std::time(&now);
		return std::difftime(now, m_tmLast);
	}
private:
	std::time_t  m_tmLast;	// ���һ�γɹ�����ʱ��
};

}
#endif