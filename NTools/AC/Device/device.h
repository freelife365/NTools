#pragma once
#include "AC/items/items.h"
#include "libmodbus/modbus.h"

#if defined(BOOST_SERIALIZE)
#include "../../boost/serialization/map.hpp"

#endif

namespace gtc{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	deviceDataImpl  ��    2015-8-2
//	���豸�õ�������
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- IChannel	2014-06-22 --- >>
//	����ͨѶ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class IChannel
{
public:
	typedef std::vector<IChannel *> PtrArrayType;
	/*****************************************************************************************
		<< --- serialPort::open	2013-12-27 --- >>
		˵���������豸��ͨѶ��·������Ѿ��򿪾������ڲ�������
		������
		����ֵ�������Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual bool open() = 0;//{	return false;	}
	/*****************************************************************************************
		<< --- serialPort::close	2013-12-27 --- >>
		˵�����ر����豸��ͨѶ��·���ڲ���һ��������������Ϊ0ʱ�������رն˿�
		������
		����ֵ�������Ƿ�ִ�гɹ�
	*****************************************************************************************/
	virtual void close() = 0;//{}
	virtual bool isOpened() const = 0;//{			return false;					}
	virtual void* getChannelHandle() = 0;//{		return nullptr;					}
	virtual PCWSTR getName() const = 0;//{		return nullptr;					}
	virtual int32_t getChannelPort() const = 0;
protected:
	uint32_t	m_uiCounter;	// ���ü�����
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- reger	 2014-06-22 --- >>
//	�Ĵ�����Ԫ����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class reger
{
public:
	typedef std::map<int32_t, reger*> MapType;
protected:
	reger();
public:
	reger(int32_t addr, const wchar_t *pname);
	reger(const reger &reg);
	reger& operator=(const reger &reg);
	virtual ~reger(void);
	/*****************************************************************************************
		<< --- reger::clone	2013-12-31 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual reger* clone() = 0;
	// ����
	std::wstring description(int indent = 4) const;

	/*****************************************************************************************
		<< --- reger::read	2013-12-31 --- >>
		˵������ȡ�Ĵ�����ֵ
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool read(modbus_t *phandle) = 0;
	/*****************************************************************************************
		<< --- reger::write	2013-12-31 --- >>
		˵������ֵ���浽�Ĵ���
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool write(modbus_t *phandle) = 0;
public:
	// ����
	std::wstring	getName() const {					return m_wsName;					}
	void			setName(const wchar_t *pname) {		m_wsName = pname?pname:L"";			}
	void			setName(std::wstring &name) {		m_wsName = name;					}
	int32_t			getAddress() const {				return m_iAddress;					}
	void			setAddress(int32_t addr) {			m_iAddress = addr;					}
	bool			isChanged() const {					return m_isChanged != 0;			}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_wsName;
		ar & m_iAddress;
		ar & m_isChanged;
	}
#endif

protected:
	std::wstring	m_wsName;		// �Ĵ�������
	int32_t			m_iAddress;		// �Ĵ�����ַ
	//int32_t			iLenByte;	// �Ĵ�����ռ�ֽ���
	//int32_t			m_iKeyFlag;		// �Ĵ��������ֱ��
	int32_t			m_isChanged;	// �Ĵ���ֵ�Ƿ��иı�
};
//
class regerI4: public reger
{
protected:
	regerI4();
public:
	regerI4(int32_t addr, const wchar_t *pname, int32_t value);
	regerI4(const regerI4 &reg);
	regerI4& operator=(const regerI4 &reg);
	virtual ~regerI4(void);
	/*****************************************************************************************
		<< --- regerI4::clone	2013-12-31 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual reger* clone();
	// ����
	std::wstring description(int indent = 4) const;
public:
	/*****************************************************************************************
		<< --- regerI4::read	2013-12-31 --- >>
		˵������ȡ�Ĵ�����ֵ
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool read(modbus_t *phandle);
	/*****************************************************************************************
		<< --- regerI4::write	2013-12-31 --- >>
		˵������ֵ���浽�Ĵ���
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool write(modbus_t *phandle);

public:
	int32_t getValue() const {					return m_iValue;							}
	void setValue(int32_t val) {				m_iValue = val; m_isChanged = true;			}


#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<reger>(*this);
		ar & m_iValue;
	}
#endif

protected:
	int32_t		m_iValue;
};
//
class regerUI2: public reger
{
protected:
	regerUI2();
public:
	regerUI2(int32_t addr, const wchar_t *pname, uint16_t value);
	regerUI2(const regerUI2 &reg);
	regerUI2& operator=(const regerUI2 &reg);
	virtual ~regerUI2(void);
	/*****************************************************************************************
		<< --- regerUI2::clone	2013-12-31 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual reger* clone();
	// ����
	std::wstring description(int indent = 4) const;
public:
	/*****************************************************************************************
		<< --- regerUI2::read	2013-12-31 --- >>
		˵������ȡ�Ĵ�����ֵ
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool read(modbus_t *phandle);
	/*****************************************************************************************
		<< --- regerUI2::write	2013-12-31 --- >>
		˵������ֵ���浽�Ĵ���
		������
		����ֵ������ִ���Ƿ�ɹ�
	*****************************************************************************************/
	virtual bool write(modbus_t *phandle);

public:

	uint16_t getValue() const {					return m_usValue;							}
	void setValue(uint16_t val) {				m_usValue = val; m_isChanged = true;		}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<reger>(*this);
		ar & m_usValue;
	}
#endif

protected:
	int32_t		m_usValue;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- device	2013-12-27 --- >>
//	�����豸��Ԫ�Ļ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class device : public itobj
{
	_DECLARE_ITOBJ_(device)
public:
	typedef std::vector<device *> PtrArrayType;
public:
	typedef enum BEDeviceProp {
		BEDevicePropName=1,BEDevicePropModelName,BEDevicePropAddress
	}BEDeviceProp;

public:
	// �豸�Ĺ���״̬
	typedef enum BEDeviceStatus{
		BEDeviceStatusNone		= 0,	// Ĭ��
		BEDeviceStatusReady		= 1,	// ����
		BEDeviceStatusOpened	,		// �Ѿ���
		BEDeviceStatusClosed	,		// �Ѿ��ر�
		BEDeviceStatusReading	,		// ���ڶ�ȡ����
		BEDeviceStatusWriting			// ����д����
	}BEDeviceStatus;
	// 
	typedef enum BEDeviceModel {
		BEDeviceModelNone				= 0x0,		// δ֪�ͺ�
		BEDeviceModelWeigherADS321		= 0x1000,	// ���ش����� ɽ����̩���������޹�˾SeTAQ AD-S321
		BEDeviceModelSCRPLC				= 0x2000,	// ������ ������ΰ����PLC V2.2
		BEDeviceModelMotor				= 0x4000,	// �������
		BEDeviceModelScale				= 0xF000	// ���ӳ�
	}BEDeviceModel;
	/*****************************************************************************************
		<< --- toStringBEDeviceModel		2014-02-12 --- >>
		˵������ö��ֵת���ɶ�Ӧ���ַ�����
		������
		model = ��ת����ö��ֵ
		����ֵ������ö���͵��ַ�����
	*****************************************************************************************/
	static std::wstring toStringBEDeviceModel(BEDeviceModel model)
	{
		switch (model)
		{
		case BEDeviceModelNone:
			return L"δ����";
			break;
		case BEDeviceModelWeigherADS321:
			return L"SeTAQ AD-S321";
			break;
		case BEDeviceModelSCRPLC:
			return L"�ɿع������PLC";
			break;
		case BEDeviceModelMotor://				= 0x4000,	// �������
			return L"�������";
			break;
		case BEDeviceModelScale://				= 0xF000	// ���ӳ�
			return L"���ӳ�";
			break;
		default:
			break;
		}
		return L"";
	}

	std::wstring	getModelName() const {
		switch (m_beModel)
		{
		case BEDeviceModelNone:
			return L"δ����";
			break;
		case BEDeviceModelWeigherADS321:
			return L"SeTAQ AD-S321";
			break;
		case BEDeviceModelSCRPLC:
			return L"�ɿع������PLC";
			break;
		case BEDeviceModelMotor://				= 0x4000,	// �������
			return L"�������";
			break;
		case BEDeviceModelScale://				= 0xF000	// ���ӳ�
			return L"���ӳ�";
			break;
		default:
			break;
		}
		return L"";
	}
public:
	//device();
	device(BEDeviceModel model = BEDeviceModelNone);
	device(const wchar_t *pname, BEDeviceModel model = BEDeviceModelNone);
	device(const device &dev);
	device& operator=(const device &dev);
	virtual ~device(void);
	// ����
	std::wstring description(int indent = 4) const;
	/*****************************************************************************************
		<< --- device::clone	2013-11-29 --- >>
		˵������¡�Լ�
		������
		����ֵ����¡����
	*****************************************************************************************/
	virtual itobj* clone() {	
		itobj *pdevice = new device(*this);
		return pdevice;
	}

public:
	// �豸���ƹ�������
	///*****************************************************************************************
	//	<< --- device::initEnvironment	2013-12-27 --- >>
	//	˵�������豸���г�ʼ�����Ա��豸������ʹ�ã��磺����洢���䣬�����������ݴ����̵߳�
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//virtual bool initEnvironment() {					assert(!L"δʵ��");return true;					}
	///*****************************************************************************************
	//	<< --  device::releaseEnvironment		2015-1-3 --- >>
	//	˵�����ͷ�initEnvironment׼���Ļ������磺�ͷŷ���Ĵ洢�����ر����ݴ����̵߳�
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//virtual bool releaseEnvironment() {					assert(!L"δʵ��");return true;						}
	///*****************************************************************************************
	//	<< --- device::changeBaudrate	2014-02-10 --- >>
	//	˵�����ı��豸��ͨѶ������
	//	������
	//	newBaudrate	= �µĲ�����
	//	����ֵ���Ƿ��޸ĳɹ�
	//*****************************************************************************************/
	//virtual bool changeBaudrate(int32_t	newBaudrate) {			assert(!L"δʵ��");return true;				}
	///*****************************************************************************************
	//	<< --- device::switchOff	2013-12-27 --- >>
	//	˵�����ж��豸�����Դ��ʹ�豸���ܹ���
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//virtual bool switchOff() {				m_bOnline = false;	return true;				}
	///*****************************************************************************************
	//	<< --- device::switchOn	2013-12-27 --- >>
	//	˵�������豸�����Դ��ʹ�豸���ܹ���
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	//virtual bool switchOn() {				m_bOnline = true;	return true;				}
	///*****************************************************************************************
	//	<< --- device::readParameter	2013-12-27 --- >>
	//	˵������ȡ�豸����
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	virtual bool readParameter(UINT_PTR ptrData) {		assert(!L"δʵ��");return true;					}
	///*****************************************************************************************
	//	<< --- device::readParameter	2013-12-27 --- >>
	//	˵�������µ��豸����д���豸
	//	������
	//	����ֵ�������Ƿ�ִ�гɹ�
	//*****************************************************************************************/
	virtual bool writeParameter(UINT_PTR ptrData) {		assert(!L"δʵ��");return true;					}

	///*****************************************************************************************
	//	<< --- device::setChannels	2014-07-21 --- >>
	//	˵�����趨ϵͳ�п��Ե�ͨѶ�˿ڼ���
	//	������
	//	pvecChannels	= ����ͨ�ж˿ڼ���
	//	����ֵ��
	//*****************************************************************************************/
	//virtual void setChannels(const IChannel::PtrArrayType &vecChannels);
	//
	/*****************************************************************************************
		<< --- device::changeAddress	2014-01-26 --- >>
		˵������Щ�豸��ͨ�ŵ�ַ������EEPROM�У����ñ�����ɸı��ַ
		������
		newAddress	= �µ�ͨѶ��ַ
		����ֵ���Ƿ��޸ĳɹ�
	*****************************************************************************************/
	virtual bool changeAddress(uint32_t newAddress) {	assert(!L"δʵ��");return true;					}
	//////////////////////////////////////////////////////////////////////////
	//	2015-9-16	��ʼ���豸���л���
	virtual bool init(UINT_PTR ptrData) {				assert(!L"δʵ��");return true;					}
	//////////////////////////////////////////////////////////////////////////
	//	2015-9-16	�����豸
	virtual bool startup(UINT_PTR ptrData) {			assert(!L"δʵ��");return true;					}
	//////////////////////////////////////////////////////////////////////////
	//	2015-9-16	ֹͣ�豸
	virtual bool stop(UINT_PTR ptrData) {				assert(!L"δʵ��");return true;					}
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
	std::wstring	getName() const {						return m_wsName;										}
	void			setName(const std::wstring &name) {		m_wsName = name;										}
	void			setName(const wchar_t *pname) {			m_wsName = pname?pname:L"";								}	
	uint32_t		getAddress() const {					return m_uiAddress;										}
	void			setAddress(uint32_t address) {			m_uiAddress = address;									}

	bool			online() const {						return m_bOnline;										}
	BEDeviceModel	getModel() const {						return m_beModel;										}


	void			setModel(BEDeviceModel model) {			m_beModel = model;										}
	int32_t			getPlace() const {						return m_iPlace;										}
	void			setPlace(int32_t place) {				m_iPlace = place;										}
	BEDeviceStatus	getStatus() const {						return m_beStatus;										}
	void			setStatus(BEDeviceStatus status) {		m_beStatus = status;									}
	int32_t			getOptCode() const {					return m_iOptCode;										}
	void			setOptCode(int32_t optCode) {			m_iOptCode = optCode;									}

#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<itobj>(*this);
		ar & m_wsName;  
		ar & m_uiAddress;
		ar & m_bOnline;
		ar & m_beModel;
		ar & m_iPlace;
		ar & m_beStatus;
		ar & m_iOptCode;
		//ar & m_vecChannels;
	}
#endif
protected:
	std::wstring	m_wsName;			// �豸����
	uint32_t		m_uiAddress;		// RS485�豸ͨѶ��ַ��IP��ַ
	//IChannel::PtrArrayType	m_vecChannels;	// ϵͳ����ͨѶ�˿ڼ���
	bool			m_bOnline;			// �Ƿ���������
	BEDeviceModel	m_beModel;			// �豸����

	int32_t			m_iPlace;			// �豸����λ��
	BEDeviceStatus	m_beStatus;			// ��ǰ�豸��״̬
	int32_t			m_iOptCode;			// ��ǰ�����������������

};


}