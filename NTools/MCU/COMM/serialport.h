/******************************************************************************
-- ��Ƭ����صĴ���ͨѶ���� 
-- 1. ����λ������8λ
-- 2. PCON��λΪ1�������ñ�Ƶ��ʽ
-- 3. 1λֹͣλ	
--	xu
-- 2013-06-12
*******************************************************************************/
#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__
#include "..\MCUDefine.h"
// ����ͨѶ������ bit/s
#define CBR_600             600
#define CBR_1200            1200
#define CBR_2400            2400
#define CBR_4800            4800
#define CBR_9600            9600
#define CBR_14400           14400
#define CBR_19200           19200
#define CBR_38400           38400
#define CBR_56000           56000
#define CBR_57600           57600

// ��żУ��λ
#define NOPARITY			0			// ��У��
#define ODDPARITY			1			// ��У��
#define EVENPARITY			2			// żУ��

// ���ջ�������С
#define PY_BUF_SIZE			16			// 
/*********************************************************************
-- ���ܣ�����ͨѶ�ĳ�ʼ��������1λֹͣλ��8λ����λ������żУ��
--		����������ݣ����շ��жϣ��ж��ܿ��ز��������ã�
-- ������baudrate = ������
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
extern BOOL SerialOpen(UINT baudrate);
/*********************************************************************
-- ���ܣ���������ͨѶ���ر��жϵ�
-- ������
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
extern void SerialClose(void);
/*********************************************************************
-- ���ܣ�������Ϣ
-- ������pmsg=�����͵���Ϣ
--		 msglen=��Ϣ����
-- ���أ��ɹ����͵����ݳ���
-- ������xu,2013-06-14
**********************************************************************/
extern char SendMessage(BYTE *pmsg, char msglen);

//////////////////////////////////////////////////////////////////////////////////////////////////////
// MODBUS

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1
/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_RW_WRITE_REGISTERS      121

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 12

#define _REPORT_SLAVE_ID 180

#define _MODBUS_EXCEPTION_RSP_LENGTH 5
/* Max between RTU and TCP max adu length (so TCP) */
#ifdef __ARM__
	#define MAX_MESSAGE_LENGTH 260
#else
	#define MAX_MESSAGE_LENGTH 16
#endif
/* Timeouts in microsecond (0.5 s) */
#define _RESPONSE_TIMEOUT    500000
#define _BYTE_TIMEOUT        500000

/* Function codes */
#define _FC_READ_COILS                0x01
#define _FC_READ_DISCRETE_INPUTS      0x02
#define _FC_READ_HOLDING_REGISTERS    0x03
#define _FC_READ_INPUT_REGISTERS      0x04
#define _FC_WRITE_SINGLE_COIL         0x05
#define _FC_WRITE_SINGLE_REGISTER     0x06
#define _FC_READ_EXCEPTION_STATUS     0x07
#define _FC_WRITE_MULTIPLE_COILS      0x0F
#define _FC_WRITE_MULTIPLE_REGISTERS  0x10
#define _FC_REPORT_SLAVE_ID           0x11
#define _FC_WRITE_AND_READ_REGISTERS  0x17

// modbus-rtuר��
#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2
// modbus-tcpר��
#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0
/*********************************************************************
-- ���ܣ�������ȡ���ּĴ������ݵı���ADU
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ������ı����ֽ���
-- ������xu,2013-06-14
**********************************************************************/
extern BYTE RTU_BuildRequestByReadRegisters(BYTE slave, BYTE addr, BYTE regnums, BYTE *preqbuf, BYTE reqbuflen);
/*********************************************************************
-- ���ܣ�������ȡ����Ĵ������ݵı���ADU
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ������ı����ֽ���
-- ������xu,2013-06-14
**********************************************************************/
extern BYTE RTU_BuildRequestByInputRegisters(BYTE slave, BYTE addr, BYTE regnums, BYTE *preqbuf, BYTE reqbuflen);
/*********************************************************************
-- ���ܣ�����Ӧ����ADU�н������Ĵ�����ֵ
-- ������req=�Ѿ����͵�������
--		 rsp=�յ�����Ӧ����
--		 rsplen=��Ӧ���ĳ���
--		 pregval=��żĴ���ֵ��ÿ���Ĵ���ռ��2Byte
--		 regvallen=������pregval��ά��
-- ���أ�pregval��Ч���ݳ���
-- ������xu,2013-06-14
**********************************************************************/
extern BYTE RTU_ResolveDataFromResponseRegisters(BYTE *req, BYTE *rsp, BYTE rsplen, UINT *pregval, BYTE regvallen);
/*********************************************************************
-- ���ܣ�����Ӧ����ADU�н���������ֵ
-- ������rsp=�յ�����Ӧ����
--		 rsplen=��Ӧ���ĳ���
--		 addr=�豸��ַ
-- ���أ���ų��صĲ���ֵ��ռ��4Byte
-- ������xu,2013-10-14
**********************************************************************/
extern long RTU_ADS_ResolveData(BYTE *rsp, BYTE rsplen, BYTE *addr);
#endif