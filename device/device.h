#ifndef __DEV_DEVICE_H__
#define __DEV_DEVICE_H__
#pragma once
#include "libmodbus/modbus.h"
#include "devdef.h"
namespace dev {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	device  ��    2016-1-11
//	�����豸����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class device
{
public:
	device();
	virtual ~device();

public:
	void setName(std::wstring wsname) {				m_wsName = wsname;						}
	const std::wstring& getName() const {			return m_wsName;						}
	void setAddress(int32_t addr) {					m_iAddress = addr;						}
	int32_t getAddress() const {					return m_iAddress;						}
	void setModel(BEDeviceModel model) {			m_beModel = model;						}
	BEDeviceModel getModel() const {				return m_beModel;						}
protected:
	std::wstring	m_wsName;		// �豸����
	int32_t			m_iAddress;		// �豸��ַ
	BEDeviceModel	m_beModel;		// �豸�ͺ�
};


}	// dev

#endif	// __DEV_DEVICE_H__