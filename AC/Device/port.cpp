#include "port.h"

#if defined(_WIN32)
	#include <afxwin.h>
	#include "AC/MFC/NTool.h"
#endif

namespace gtc{



////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- serialPort	2013-12-27 --- >>
//	����ͨѶ
////////////////////////////////////////////////////////////////////////////////////////////////////////
_IMPLEMENT_ITOBJ_(serialPort, device, BEDrawShapeTypeDeviceSerialPort, 0x1)

serialPort::serialPort()
	: device()
	, m_nDatabit(8), m_cStopbit(1), m_cParity('N')
	, m_iBaud(9600), m_iPort(0)
	, m_pctxPort(nullptr)
{

}

serialPort::serialPort(const wchar_t *pname, int32_t port, int32_t baud, BEDeviceModel model)
	: device(pname, model)
	, m_nDatabit(8), m_cStopbit(1), m_cParity('N')
	, m_iBaud(baud), m_iPort(port)
	, m_pctxPort(nullptr)
{

}

serialPort::serialPort(const serialPort &sport)
	: device(sport)
	, m_nDatabit(sport.m_nDatabit), m_cStopbit(sport.m_cStopbit), m_cParity(sport.m_cParity)
	, m_iBaud(sport.m_iBaud), m_iPort(sport.m_iPort)
	, m_pctxPort(nullptr)
{

}

serialPort& serialPort::operator=(const serialPort &sport)
{
	if(this != &sport)
	{
		m_nDatabit = sport.m_nDatabit;
		m_cStopbit = sport.m_cStopbit;
		m_cParity = sport.m_cParity;
		m_iBaud = sport.m_iBaud;
		m_iPort = sport.m_iPort;
		m_pctxPort = nullptr;
	}
	return *this;
}

serialPort::~serialPort(void)
{
	DEBUG_LOG(L"free %s", description().c_str());

	close();
}

// ����
std::wstring serialPort::description(int indent) const
{
	std::wostringstream os;
	gtc::outClassBegin(os, L"serialPort", indent);
	gtc::outClassItem(os, L"�˿ں�", m_iPort, indent << 1);
	gtc::outClassItem(os, L"������", m_iBaud, indent << 1);
	gtc::outClassItem(os, L"��żУ��", m_cParity, indent << 1);
	gtc::outClassItem(os, L"����λ", m_nDatabit, indent << 1);
	gtc::outClassItem(os, L"ֹͣλ", m_cStopbit, indent << 1);
	gtc::outBaseClass(os, *(device*)this, indent << 1);
	gtc::outClassEnd(os, L"serialPort", indent);
	return os.str();

}
/*****************************************************************************************
	<< --- serialPort::open	2013-12-27 --- >>
	˵���������豸��ͨѶ��·
	������
	����ֵ�������Ƿ�ִ�гɹ�
*****************************************************************************************/
bool serialPort::open()
{
	if(m_pctxPort)
	{
		++m_uiCounter;
		return true;
	}
	// 
	if(!m_pctxPort)
	{
		char comm[10] = {0};
#if defined(_WIN32)
		::sprintf_s(comm, sizeof(comm), "com%d", m_iPort+1);
#else
		std::snprintf(comm, sizeof(comm), "com%d", m_iPort+1);
#endif
		// 
		m_pctxPort = modbus_new_rtu(comm, m_iBaud, m_cParity, m_nDatabit, m_cStopbit);
		if(!m_pctxPort || modbus_connect(m_pctxPort) == -1)
		{
			app::messageEx(L"��ͨ�Ŷ˿ڡ�%s��ʧ�ܣ�error��%s", convert::toWChar(comm).c_str(), convert::toWChar(modbus_strerror(errno)).c_str());
			modbus_free(m_pctxPort);
			m_pctxPort = nullptr;
			return false;
		}
		// 
		m_uiCounter = 1;
	}
	return m_pctxPort != nullptr;
}

/*****************************************************************************************
	<< --- serialPort::close	2013-12-27 --- >>
	˵�����ر����豸��ͨѶ��·
	������
	����ֵ�������Ƿ�ִ�гɹ�
*****************************************************************************************/
void serialPort::close()
{
	//assert(m_pctxPort);
	if(m_pctxPort)
	{
		--m_uiCounter;
		if (m_uiCounter == 0)
		{
			modbus_close(m_pctxPort);
			modbus_free(m_pctxPort);
			m_pctxPort = nullptr;
		}
	}	
}
bool serialPort::isOpened() const 
{				
	return m_pctxPort != nullptr;				
}
void* serialPort::getChannelHandle() 
{
	return m_pctxPort;
}
PCWSTR serialPort::getName() const 
{
	return m_wsName.c_str();
}
int32_t serialPort::getChannelPort() const 
{	
	return getPort();
}

#if defined(_WIN32)
// 
void serialPort::draw(CDC *pdc)
{
}

/*****************************************************************************************
	<< ---static serialPort::allocPropertySectionArray	2013-12-06 --- >>
	˵���������������
	������
	����ֵ��
*****************************************************************************************/
std::size_t serialPort::allocPropertySectionArray(itobj* pitem, bool isShare)
{
	itobjPropSection::PtrArrayType &vecSection = pitem->getPropertys();
	if(vecSection.size() > 0) pitem->freePropertys();

	device::allocPropertySectionArray(pitem, isShare);
	itobjPropSection *psec = nullptr, *psubsec = nullptr;
	entry::PtrArrayType vec;
	serialPort *psport = dynamic_cast<serialPort *>(pitem);
	// 
	if(psport && (psec = new itobjPropSection(L"ͨѶ����")) != nullptr)
	{
		// ͨ�Ŷ˿�
		//const std::size_t len = 10;
		//wchar_t comm[len] = {0};
		//for(int32_t idx = 1; idx <=32; ++idx)
		//{
		//	std::swprintf(comm, 10, L"COM%d", idx);
		//	vec.push_back(entry(idx-1, comm));
		//}
		vec.assign(std::begin(gc_tbSerialPort), std::end(gc_tbSerialPort));
		psec->addProperty(new itobjPropCombo(L"�˿ں�", psport->getPort(), vec, false, L"",		BEDrawShapeTypeDeviceSerialPort, BESPortPropPort));
		// 
		//vec.clear();
		//vec.push_back(entry(110,		L"110"));
		//vec.push_back(entry(300,		L"300"));
		//vec.push_back(entry(600,		L"600"));
		//vec.push_back(entry(1200,		L"1200"));
		//vec.push_back(entry(2400,		L"2400"));
		//vec.push_back(entry(4800,		L"4800"));
		//vec.push_back(entry(9600,		L"9600"));
		//vec.push_back(entry(19200,	L"19200"));
		//vec.push_back(entry(38400,	L"38400"));
		//vec.push_back(entry(57600,	L"57600"));
		//vec.push_back(entry(115200,	L"115200"));
		vec.assign(std::begin(gc_tbSerialBaudrate), std::end(gc_tbSerialBaudrate));
		psec->addProperty(new itobjPropCombo(L"������", psport->getBaudrate(), vec, false, L"",		BEDrawShapeTypeDeviceSerialPort, BESPortPropBaud));
		// 
		//vec.clear();
		//vec.push_back(entry(5, L"5"));
		//vec.push_back(entry(6, L"6"));
		//vec.push_back(entry(7, L"7"));
		//vec.push_back(entry(8, L"8"));
		vec.assign(std::begin(gc_tbSerialDatabit), std::end(gc_tbSerialDatabit));
		psec->addProperty(new itobjPropCombo(L"����λ", psport->getDatabit(), vec, false, L"", BEDrawShapeTypeDeviceSerialPort, BESPortPropDatabit));
		// 
		//vec.clear();
		//vec.push_back(entry(1, L"1"));
		//vec.push_back(entry(2, L"2"));
		vec.assign(std::begin(gc_tbSerialStopbit), std::end(gc_tbSerialStopbit));
		psec->addProperty(new itobjPropCombo(L"ֹͣλ", psport->getStopbit(), vec, false, L"", BEDrawShapeTypeDeviceSerialPort, BESPortPropStopbit));
		// 
		//vec.clear();
		//vec.push_back(entry('N', L"��У��"));
		//vec.push_back(entry('E', L"żУ��"));
		//vec.push_back(entry('O', L"��У��"));
		vec.assign(std::begin(gc_tbSerialParity), std::end(gc_tbSerialParity));
		psec->addProperty(new itobjPropCombo(L"��żУ��", psport->getParity(), vec, false, L"", BEDrawShapeTypeDeviceSerialPort, BESPortPropParity));

		// 
		vecSection.push_back(psec);

	}

	return vecSection.size();	
}

/*****************************************************************************************
	<< --- serialPort::updatePropertyValue	2013-12-06 --- >>
	˵�����������ԶԻ���ֵ���¶���ֵ�������ĸ�����ֵͨ��typeKey��itemKeyȷ��
	������
	pprop	= ����ֵ�������µ����Ը���pprop�����ͼ�����Ŀ��ȷ��
	����ֵ�����ĳɹ�
*****************************************************************************************/
bool serialPort::updatePropertyValue(itobjProp *pprop)
{
	if(pprop && device::updatePropertyValue(pprop))
		return true;
	if(!pprop || pprop->getTypekey() != BEDrawShapeTypeDeviceSerialPort)
		return false;
	bool exists = true;
	COleVariant var = pprop->getValue();
	switch (BESPortProp( pprop->getItemkey()))
	{
	case gtc::serialPort::BESPortPropPort:
		var.ChangeType(VT_I4);
		setPort(var.lVal);
		break;
	case gtc::serialPort::BESPortPropBaud:
		var.ChangeType(VT_I4);
		setBaudrate(var.lVal);
		break;
	case gtc::serialPort::BESPortPropDatabit:
		var.ChangeType(VT_UI1);
		setDatabit(var.bVal);
		break;
	case gtc::serialPort::BESPortPropStopbit:
		var.ChangeType(VT_I1);
		setStopbit(var.cVal);
		break;
	case gtc::serialPort::BESPortPropParity:
		var.ChangeType(VT_I1);
		setParity(var.cVal);
		break;
	default:
		exists = false;
		break;
	}

	return exists;
}

/*****************************************************************************************
	<< --- serialPort::messageProperty	2013-12-24 --- >>
	˵��������ָ�����Ա�ǵ�����ֵ��������ϢWM_ME_ITEM_PROPERTY_CHANGED���͸����Ա༭����
	������
	typeKey		= �������ͣ��磺BEDrawShapeType
	vecItemKey	= ��Ҫ���µ����Ա��
	����ֵ��
*****************************************************************************************/
void serialPort::messageProperty(int32_t typeKey, std::vector<int32_t> &vecItemKey)
{
	if(typeKey != BEDrawShapeTypeDeviceSerialPort)
	{
		__super::messageProperty(typeKey, vecItemKey);
		return ;
	}
	gtc::itobjProp::PtrArrayType *pvecProp = new gtc::itobjProp::PtrArrayType();
	itobjProp *pitem = nullptr;
	COleVariant var;
	for(std::vector<int32_t>::iterator iter = vecItemKey.begin(), end = vecItemKey.end(); iter != end; ++iter)
	{
		pitem = itobjProp::findPropertyItem(typeKey, *iter);
		if(!pitem)
			break;
		var.Clear();
		bool exists = true;
		switch (BESPortProp(*iter))
		{
		case gtc::serialPort::BESPortPropPort:
			var.ChangeType(VT_I4);
			var.lVal = getPort();
			break;
		case gtc::serialPort::BESPortPropBaud:
			var.ChangeType(VT_I4);
			var.lVal = getBaudrate();
			break;
		case gtc::serialPort::BESPortPropDatabit:
			var.ChangeType(VT_UI1);
			var.bVal = getDatabit();
			break;
		case gtc::serialPort::BESPortPropStopbit:
			var.ChangeType(VT_I1);
			var.cVal = getStopbit();
			break;
		case gtc::serialPort::BESPortPropParity:
			var.ChangeType(VT_I1);
			var.cVal = getParity();
			break;
		default:
			exists = false;
			break;
		}
		if(exists)
		{
			pitem->setValue(var);
			pvecProp->push_back(pitem);
		}
	}
	if(pvecProp && pvecProp->size() < 1)
		delete pvecProp;
	if(pvecProp && pvecProp->size() > 0)
	{
		app::getMainWnd()->PostMessage(WM_ME_ITEM_PROPERTY_CHANGED, 0L, (LPARAM)pvecProp);
	}

}
#endif



}