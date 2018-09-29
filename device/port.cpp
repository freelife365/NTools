#include "port.h"
#include <Setupapi.h>

#pragma comment(lib, "Setupapi.lib")
namespace dev {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	serialPort  ��    2016-1-12
//	ͨѶ�˿ڻ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
serialPort::serialPort()
	: m_hPort(nullptr)
	, m_nDatabit(8), m_cStopbit(1), m_cParity('N'), m_iBaud(19200), m_iPort(0)
	, m_threadRead(nullptr), m_switchRead(0)
{

}
serialPort::~serialPort()
{
	if (m_hPort)
	{
		close();
	}
}

bool serialPort::open()
{
	if (m_hPort)
	{
		return true;
	}

	char comm[10] = {0};

	//std::snprintf(comm, sizeof(comm), "com%d", m_iPort+1);
	std::sprintf(comm, "com%d", m_iPort + 1);
	// 
	m_hPort = modbus_new_rtu(comm, m_iBaud, m_cParity, m_nDatabit, m_cStopbit);
	if(!m_hPort || modbus_connect((modbus_t *)m_hPort) == -1)
	{
		modbus_free((modbus_t *)m_hPort);
		m_hPort = nullptr;
	}
	if (m_hPort)
	{
		start();
	}
	return m_hPort != nullptr;
}
void serialPort::close()
{
	if(!m_hPort) return;
	stop();

	modbus_close((modbus_t *)m_hPort);
	modbus_free((modbus_t *)m_hPort);
	m_hPort = nullptr;
}
bool serialPort::isOpened() const
{
	return m_hPort != nullptr;
}

bool serialPort::write(uint8_t *pdata, uint32_t datalen)
{
	assert(pdata);
	assert(datalen > 0);
	int rc = modbus_send_straight((modbus_t *)m_hPort, pdata, datalen);
	return rc > 0;
}

int serialPort::read(uint8_t *pbuf, uint32_t buflen)
{
	assert(pbuf);
	assert(buflen > 0);
	int rc = modbus_recv_straight((modbus_t *)m_hPort, pbuf, buflen);

	return rc;
}

void serialPort::start()
{
	if (m_threadRead)
	{
		stop();
	}
	m_threadRead = (HANDLE)::_beginthreadex(NULL, 0, threadReadData, this, CREATE_SUSPENDED, NULL);
	if (m_threadRead)
	{
		InterlockedCompareExchange(&m_switchRead, 1, 0);
		::ResumeThread(m_threadRead);
	}

}

void serialPort::stop()
{
	if(!m_threadRead) return ;
	InterlockedCompareExchange(&m_switchRead, 0, 1);
	switch(WaitForSingleObject(m_threadRead, 5000))
	{
	case WAIT_OBJECT_0:
		SEND_LOG(L"ERROR serialPort::stop  ֹͣ����  WAIT_OBJECT_0");
		break;
	case WAIT_TIMEOUT:
		SEND_LOG(L"ERROR serialPort::stop  ֹͣ����  WAIT_TIMEOUT");
		break;
	case WAIT_FAILED:
		SEND_LOG(L"ERROR serialPort::stop  ֹͣ����  WAIT_FAILED");
		break;
	default:
		break;
	}
	CloseHandle(m_threadRead);
	m_threadRead = NULL;

}

unsigned int WINAPI serialPort::threadReadData(LPVOID pParam)
{
	serialPort *port = (serialPort *)pParam;
	assert(port);
	uint8_t pbuf[255] = {0};
	int rc = 0;
	while(port->m_switchRead == 1)
	{
		rc = port->read(pbuf, sizeof(pbuf));
		if (rc >= 5)
		{
			SEND_LOG(L"threadReadData rc:%d %02X %02X  %d", rc, pbuf[0], pbuf[1], gtc::app::MAIN_THREAD_ID);
			uint8_t *pdata = new uint8_t[rc];
			memcpy_s(pdata, rc, pbuf, rc);
			PostMessage(gtc::app::MAIN_WINDOW, WME_GOTOMAIN, (WPARAM)pdata, (LPARAM)rc);
			//PostThreadMessage(gtc::app::MAIN_THREAD_ID, WME_GOTOMAIN, (WPARAM)pdata, (LPARAM)rc);
		}
		Sleep(50);
	}
	return 0;
}

// The following define is from ntddser.h in the DDK. It is also
// needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, \
	0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif
/*static*/ IntSetType serialPort::enumPort(bool bIgnoreBusyPorts /*= true*/)
{
	IntSetType setPort;
	GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;

	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;
	wchar_t pbuf[MAX_PATH];
	try
	{
		hDevInfo = SetupDiGetClassDevs( guidDev,
			NULL,
			NULL,
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
			);

		if(hDevInfo == INVALID_HANDLE_VALUE) GTC_EXCEPT_DEFAULT(L"SetupDiGetClassDevs ʧ��", 0);

		// Enumerate the serial ports
		BOOL bOk = TRUE;
		SP_DEVICE_INTERFACE_DATA ifcData;
		DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
		pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
		// This is required, according to the documentation. Yes,
		// it's weird.
		ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		for (DWORD ii=0; bOk; ii++) {
			bOk = SetupDiEnumDeviceInterfaces(hDevInfo,
				NULL, guidDev, ii, &ifcData);
			if (bOk) {
				// Got a device. Get the details.
				SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
				bOk = SetupDiGetDeviceInterfaceDetail(hDevInfo,
					&ifcData, pDetData, dwDetDataSize, NULL, &devdata);
				if (bOk) {
					// Got a path to the device. Try to get some more info.
					wchar_t fname[256];
					wchar_t desc[256];
					BOOL bSuccess = SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL,
						(PBYTE)fname, sizeof(fname), NULL);
					bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,
						(PBYTE)desc, sizeof(desc), NULL);

					//DWORD addr = 0;
					//wchar_t pbuf[256];
					//bSuccess = /*bSuccess &&*/ SetupDiGetDeviceRegistryProperty(
					//	hDevInfo, &devdata, SPDRP_BUSNUMBER, NULL,
					//	(PBYTE)&addr, sizeof(addr), NULL);
					//bSuccess = /*bSuccess &&*/ SetupDiGetDeviceRegistryProperty(
					//	hDevInfo, &devdata, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, NULL,
					//	(PBYTE)pbuf, sizeof(pbuf), NULL);

					BOOL bUsbDevice = FALSE;
					wchar_t locinfo[256];
					if (SetupDiGetDeviceRegistryProperty(
						hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL,
						(PBYTE)locinfo, sizeof(locinfo), NULL))
					{
						// Just check the first three characters to determine
						// if the port is connected to the USB bus. This isn't
						// an infallible method; it would be better to use the
						// BUS GUID. Currently, Windows doesn't let you query
						// that though (SPDRP_BUSTYPEGUID seems to exist in
						// documentation only).

						SEND_LOG(L"serialPort::enumPort locinfo:<%s>", locinfo);
						bUsbDevice = gtc::stringUtil::startsWith(locinfo, L"usb");// (strncmp(locinfo, "USB", 3)==0);
					}
					if (bSuccess) {
						// Add an entry to the array
						//std::wstring wstr = pDetData->DevicePath;
						//wstr = fname;
						//wstr = desc;
						//si.strFriendlyName = fname;
						//si.strPortDesc = desc;
						//si.bUsbDevice = bUsbDevice;
						//asi.Add(si);
						SEND_LOG(L"serialPort::enumPort friendly:<%s> port:<%s>", fname, desc);

						// �������ں�
						std::wstring wstr = fname;
						std::wstring::size_type pos = wstr.rfind(L"(COM");
						if (pos != std::wstring::npos)
						{
							pos += 4;
							std::wstring::size_type posEnd = wstr.find(L')', pos);
							if (posEnd != std::wstring::npos)
							{
								wstr = wstr.substr(pos, posEnd - pos);
								setPort.insert((int)std::wcstol(wstr.c_str(), nullptr, 10) - 1);	// ���ڻ���0
							}
						}

					}
				}
				else
				{
					GTC_EXCEPT_DEFAULT(L"SetupDiGetDeviceInterfaceDetail failed", GetLastError());
				}
			}
			else {
				GTC_EXCEPT_DEFAULT(L"SetupDiEnumDeviceInterfaces failed", GetLastError());
			}
		}

	}
	catch(gtc::baseException &e)
	{
		SEND_LOG(L"ERROR serialPort::enumPort error:<%s>", e.description().c_str());
	}
	// 
	if (pDetData != NULL)
		delete [] (char*)pDetData;
	if (hDevInfo != INVALID_HANDLE_VALUE)
		SetupDiDestroyDeviceInfoList(hDevInfo);

	return setPort;
}



}	// dev