#pragma once

#include "device.h"

namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- �봮��ͨѶ��ض��� entry[]	2014-02-12 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
// RS485��ַ
static entry* gc_tbRS485Address[] = {
	new entry(0,	L"�㲥��ַ"), 
	new entry(1,	L"1"), 
	new entry(2,	L"2"), 
	new entry(3,	L"3"), 
	new entry(4,	L"4"), 
	new entry(5,	L"5"), 
	new entry(6,	L"6"), 
	new entry(7,	L"7"), 
	new entry(8,	L"8"), 
	new entry(9,	L"9"), 
	new entry(10,	L"10"), 
	new entry(11,	L"11"), 
	new entry(12,	L"12"), 
	new entry(13,	L"13"), 
	new entry(14,	L"14"), 
	new entry(15,	L"15"), 
	new entry(16,	L"16"), 
	new entry(17,	L"17"), 
	new entry(18,	L"18"), 
	new entry(19,	L"19"), 
	new entry(20,	L"20"), 
	new entry(21,	L"21"), 
	new entry(22,	L"22"), 
	new entry(23,	L"23"), 
	new entry(24,	L"24"), 
	new entry(25,	L"25"), 
	new entry(26,	L"26"), 
	new entry(27,	L"27"), 
	new entry(28,	L"28"), 
	new entry(29,	L"29"), 
	new entry(30,	L"30"), 
	new entry(31,	L"31")
};

// ͨѶ�˿�
static entry* gc_tbSerialPort[] = {
	new entry(0,	L"COM1"), 
	new entry(1,	L"COM2"), 
	new entry(2,	L"COM3"), 
	new entry(3,	L"COM4"), 
	new entry(4,	L"COM5"), 
	new entry(5,	L"COM6"), 
	new entry(6,	L"COM7"), 
	new entry(7,	L"COM8"), 
	new entry(8,	L"COM9"), 
	new entry(9,	L"COM10"), 
	new entry(10,	L"COM11"), 
	new entry(11,	L"COM12"), 
	new entry(12,	L"COM13"), 
	new entry(13,	L"COM14"), 
	new entry(14,	L"COM15"), 
	new entry(15,	L"COM16"), 
	new entry(16,	L"COM17"), 
	new entry(17,	L"COM18"), 
	new entry(18,	L"COM19"), 
	new entry(19,	L"COM20"), 
	new entry(20,	L"COM21"), 
	new entry(21,	L"COM22"), 
	new entry(22,	L"COM23"), 
	new entry(23,	L"COM24"), 
	new entry(24,	L"COM25"), 
	new entry(25,	L"COM26"), 
	new entry(26,	L"COM27"), 
	new entry(27,	L"COM28"), 
	new entry(28,	L"COM29"), 
	new entry(29,	L"COM30"), 
	new entry(30,	L"COM31"), 
	new entry(31,	L"COM32")
};
// ������
static entry* gc_tbSerialBaudrate[] = {
	new entry(110,	L"110"),
	new entry(300,	L"300"),
	new entry(600,	L"600"),
	new entry(1200,	L"1200"),
	new entry(2400,	L"2400"),
	new entry(4800,	L"4800"),
	new entry(9600,	L"9600"),
	new entry(19200,	L"19200"),
	new entry(38400,	L"38400"),
	new entry(57600,	L"57600"),
	new entry(115200,	L"115200")
};
// ����λ
static entry* gc_tbSerialDatabit[] = {
	new entry(5, L"5"),
	new entry(6, L"6"),
	new entry(7, L"7"),
	new entry(8, L"8")
};
// ֹͣλ
static entry* gc_tbSerialStopbit[] = {
	new entry(1, L"1"),
	new entry(2, L"2")
};
// ��żУ��
static entry* gc_tbSerialParity[] = {
	new entry('N', L"��У�� N"),
	new entry('E', L"żУ�� E"),
	new entry('O', L"��У�� O")
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- serialPort	2013-12-27 --- >>
//	����ͨѶ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class serialPort : public device , public IChannel
{
	_DECLARE_ITOBJ_(serialPort)
public:
	typedef std::vector<serialPort*> PtrArrayType;
public:
	typedef enum BESPortProp {
		BESPortPropPort=1,BESPortPropBaud,BESPortPropDatabit,BESPortPropStopbit,BESPortPropParity
	}BESPortProp;

public:
	serialPort();
	serialPort(const wchar_t *pname, int32_t port, int32_t baud, BEDeviceModel model = BEDeviceModelNone);
	serialPort(const serialPort &sport);
	serialPort& operator=(const serialPort &sport);
	virtual ~serialPort(void);
	// ����
	std::wstring description(int indent = 4) const;
	/*****************************************************************************************
		<< --- device::clone	2013-11-29 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual itobj* clone() {	
		itobj* pdevice = new serialPort(*this);
		return pdevice;
	}
public:
	// IChannel
	/*****************************************************************************************
		<< --- serialPort::open	2013-12-27 --- >>
		˵���������豸��ͨѶ��·
		������
		����ֵ�������Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool open();
	/*****************************************************************************************
		<< --- serialPort::close	2013-12-27 --- >>
		˵�����ر����豸��ͨѶ��·
		������
		����ֵ�������Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual void close();
	virtual bool isOpened() const;
	virtual void* getChannelHandle();
	virtual PCWSTR getName() const;
	virtual int32_t getChannelPort() const;
public:
	//////////////////////////////////////////////////////////////////////////////////////////////
	// WIN32 ����̳�
#if defined(_WIN32)
	// 
	virtual void draw(CDC *pdc);

	/*****************************************************************************************
		<< --- static device::allocPropertySectionArray	2013-12-06 --- >>
		˵���������������
		������
		����ֵ��
	*****************************************************************************************/
	static std::size_t allocPropertySectionArray(itobj* pitem, bool isShare);
	virtual std::size_t allocPropertySectionArray(bool isShare) {
		return allocPropertySectionArray(this, isShare);
	}
	/*****************************************************************************************
		<< --- device::updatePropertyValue	2013-12-06 --- >>
		˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
		������
		pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
		����ֵ�����ĳɹ�
	*****************************************************************************************/
	virtual bool updatePropertyValue(itobjProp *pprop);
	/*****************************************************************************************
		<< --- device::messageProperty	2013-12-24 --- >>
		˵��������ָ�����Ա�ǵ�����ֵ��������ϢWM_ME_ITEM_PROPERTY_CHANGED���͸����Ա༭����
		������
		typeKey		= �������ͣ��磺BEDrawShapeType
		vecItemKey	= ��Ҫ���µ����Ա��
		����ֵ��
	*****************************************************************************************/
	virtual void messageProperty(int32_t typeKey, std::vector<int32_t> &vecItemKey);
#endif

public:
	// ����
	uint8_t	getDatabit() const {				return m_nDatabit;							}
	void	setDatabit(uint8_t databit) {
		if(databit >= 5 && databit <= 8)
			m_nDatabit = databit;
		else
			m_nDatabit = 8;
	}
	int8_t	getStopbit() const {				return m_cStopbit;							}
	void	setStopbit(int8_t stopbit) {
		m_cStopbit = stopbit==1?1:2;
	}
	int8_t	getParity() const {					return m_cParity;							}
	void	setParity(int8_t parity) {
		if(parity == 'O' || parity == 'o')
			parity = 'O';
		else if(parity == 'E' || parity == 'e')
			parity = 'E';
		else
			parity = 'N';
		m_cParity = parity;
	}
	int32_t	getBaudrate() const {					return m_iBaud;							}
	void	setBaudrate(int32_t baud) {
		switch(baud)
		{
		case 110:
		case 300:
		case 600:
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 57600:
		case 115200:
			m_iBaud = baud;
			break;
		default:
			m_iBaud = 9600;
			break;
		}
	}
	int32_t	getPort() const {					return m_iPort;								}
	void	setPort(int32_t port) {				m_iPort = port < 0?0:port;					}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<device>(*this);
		ar & m_nDatabit;  
		ar & m_cStopbit;
		ar & m_cParity;
		ar & m_iBaud;
		ar & m_iPort;
		//ar & m_vecChannels;
	}
#endif
protected:
	uint8_t		m_nDatabit;		// ����λ�ĸ���
	int8_t		m_cStopbit;		// ֹͣλ�ĸ������磺	1=ONESTOPBIT��2=TWOSTOPBITS
	int8_t		m_cParity;		// ��żУ��λ���磺'N', 'O', 'E'
	int32_t		m_iBaud;		// �����ʣ��磺9600, 19200, 57600, 115200, etc
	int32_t		m_iPort;		// �˿ںţ��磺0=com1,1=com2...

	// �˿ھ��
	modbus_t*	m_pctxPort;

};
}