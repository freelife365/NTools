/******************************************************************************
-- ��Ƭ����صĴ���ͨѶ���� 
-- 
--	
--	xu
-- 2013-06-12
*******************************************************************************/
#include "serialport.h"
#include <reg52.h>	
/* Table of CRC values for high-order byte */
static const BYTE code  table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const BYTE code table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
// modbus-rtu����ͨ�ŵ�ѭ��У��
static UINT crc16(BYTE *buffer, BYTE buffer_length)
{
    BYTE crc_hi = 0xFF; /* high CRC byte initialized */
    BYTE crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}
// 
/*********************************************************************
-- ���ܣ�������Э�����ݵ�λPDU������RTU������Ϣ����ͷ
-- ������slave=�������豸��ַ
--		 function=������
--		 addr=�豸�ڼĴ�����ַ
--		 regnums=Ҫ���ʵļĴ�����
--		 req=����ͷ
-- ���أ����ĳ���
-- ������xu,2013-06-10
**********************************************************************/
static BYTE RTU_BuildRequestBasis(BYTE slave, BYTE function, BYTE addr, BYTE regnums, BYTE *req)
{
	if(slave < 0)
		return 0;
	req[0] = slave;
    req[1] = function;
    req[2] = addr >> 8;
    req[3] = addr & 0x00ff;
    req[4] = regnums >> 8;
    req[5] = regnums & 0x00ff;

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}
/*********************************************************************
-- ���ܣ�����Ӧ�����ݵ�λADU�еĲ��У�鲿�֣�����CRC��
-- ������req=�������ݣ�������ַ�򡢹����롢���ݣ�
--		 reqlength=�������ݳ���
-- ���أ�����ADU���ֽڳ���
-- ������xu,2013-06-17
**********************************************************************/
static BYTE RTU_SendMsgPre(BYTE *req, BYTE reqlength)
{
    UINT crc = crc16(req, reqlength);
    req[reqlength++] = crc >> 8;
    req[reqlength++] = crc & 0x00FF;

    return reqlength;
}
/*********************************************************************
-- ���ܣ�У��Ӧ�����ݵ�λADU������CRC��
-- ������pmsg=��У�������
--		 msglength=��Ϣ����
-- ���أ����ĳ���
-- ������xu,2013-06-17
**********************************************************************/
static BYTE RTU_CheckIntegrity(BYTE *pmsg, BYTE msglength)
{
	UINT crc_calculated;
    UINT crc_received;

    crc_calculated = crc16(pmsg, msglength - 2);
    crc_received = (pmsg[msglength - 2] << 8) | pmsg[msglength - 1];

    /* Check CRC of msg */
    if (crc_calculated == crc_received) {
        return msglength;
    } else {
        return -1;
    }
}

/*********************************************************************
-- ���ܣ��������ĵõ���Ӧ����ADU���ֽ���
-- ������req=������ADU
-- ���أ���Ӧ����ADU���ֽ���
-- ������xu,2013-06-17
**********************************************************************/
static UINT RTU_ComputeResponseLengthFromRequest(BYTE *req)
{
    int length;
    const BYTE offset = 1;	  // RTU����ͷΪ1�ֽ�
	const BYTE checksum_length = 2;	// ���ĵ�У��ͳ���
    switch (req[offset]) {
    case _FC_READ_COILS:
    case _FC_READ_DISCRETE_INPUTS: {
        /* Header + nb values (code from write_bits) */
        int nb = (req[offset + 3] << 8) | req[offset + 4];
        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
        break;
    case _FC_WRITE_AND_READ_REGISTERS:
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case _FC_READ_EXCEPTION_STATUS:
        length = 3;
        break;
    case _FC_REPORT_SLAVE_ID:
        /* The response is device specific (the header provides the
           length) */
        return MSG_LENGTH_UNDEFINED;
    default:
        length = 5;
    }

    return offset + length + checksum_length;
}
/*********************************************************************
-- ���ܣ�ͨ���������ĺ���Ӧ���ıȽϷ����õ��Ĵ�������Ȧ���ֽ���
-- ������req=���͵�������ADU��ʽ
--		 rsp=�õ���Ӧ����ADU��ʽ
--		 rsp_length=��Ӧ���ĵ��ֽ���
-- ���أ������мĴ�������Ȧ���ֽ���
-- ������xu,2013-06-17
**********************************************************************/
static BYTE RTU_CheckConfirmation(BYTE *req, BYTE *rsp, BYTE rsp_length)
{
    char rc;
    char rsp_length_computed;
    const BYTE offset = 1, checksum_length = 2;

    rsp_length_computed = RTU_ComputeResponseLengthFromRequest(req);

    /* Check length */
    if (rsp_length == rsp_length_computed ||
        rsp_length_computed == MSG_LENGTH_UNDEFINED) {
        char req_nb_value;
        char rsp_nb_value;
        const BYTE function = rsp[offset];

        /* Check function code */
        if (function != req[offset]) {
            return -1;
        }

		// �ֱ���������Ӧ�����еõ��Ĵ�������Ȧ���ֽ���
        /* Check the number of values is corresponding to the request */
        switch (function) {
        case _FC_READ_COILS:
        case _FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
             * of values in the request and byte count in
             * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case _FC_WRITE_AND_READ_REGISTERS:
        case _FC_READ_HOLDING_REGISTERS:
        case _FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case _FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        if (req_nb_value == rsp_nb_value) {
            rc = rsp_nb_value;
        } else {
            
            rc = -1;
        }
    } else if (rsp_length == (offset + 2 + checksum_length) &&
               req[offset] == (rsp[offset] - 0x80)) {
        /* EXCEPTION CODE RECEIVED */

        //int exception_code = rsp[offset + 1];
        
        rc = -1;
    } else {

        rc = -1;
    }

    return rc;
}
/*********************************************************************
-- ���ܣ����ݴ��ڲ����ʵõ�T1��ʱ���ĳ�ֵ���ο�����Ƶ��
-- ������
-- ���أ�T1��ʼֵ
-- ������xu,2013-06-10
**********************************************************************/
static BYTE GetSerialRate(UINT baudrate)
{
	BYTE t1 = 0;
#ifdef MCU_CLOCK_11MHZ
	switch(baudrate)
	{
	case CBR_600:
		t1 = 0xA0;
		break;
	case CBR_1200:
		t1 = 0xD0;
		break;
	case CBR_2400:
		t1 = 0xE8;
		break;
	case CBR_4800:
		t1 = 0xF4;
		break;
	case CBR_9600:
		t1 = 0xFA; 
		break;
	case CBR_14400:
		t1 = 0xFC;
		break;
	case CBR_19200:
		t1 = 0xFD;
		break;
	case CBR_38400:
		t1 = 0xFE;	 // ���-25%
		break;
	case CBR_56000:
		t1 = 0xFF;	 // ���2.86%
		break;
	case CBR_57600:
		t1 = 0xFF;
		break;
	default:	  // 9600
		t1 = 0xFA;
		break;
	}
#endif
#ifdef MCU_CLOCK_22MHZ
	switch(baudrate)
	{
	case CBR_600:
		t1 = 0x40;
		break;
	case CBR_1200:
		t1 = 0xA0;
		break;
	case CBR_2400:
		t1 = 0xD0;
		break;
	case CBR_4800:
		t1 = 0xE8;
		break;
	case CBR_9600:
		t1 = 0xF4; 
		break;
	case CBR_14400:
		t1 = 0xF8;
		break;
	case CBR_19200:
		t1 = 0xFA;
		break;
	case CBR_38400:
		t1 = 0xFD;
		break;
	case CBR_56000:
		t1 = 0xFE;	 // ���2.86%
		break;
	case CBR_57600:
		t1 = 0xFE;
		break;
	default:	  // 9600
		t1 = 0xF4;
		break;
	}
#endif
	return t1;
}
/*********************************************************************
-- ���ܣ�����ͨѶ�ĳ�ʼ��������1λֹͣλ��8λ����λ������żУ��
--		����������ݣ����շ��жϣ��ж��ܿ��ز��������ã�
-- ������baudrate = ������
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
BOOL SerialOpen(UINT baudrate)
{
	PCON |= 0x80;	//�����ʼӱ�
	// ����żУ�飺��ʽ1,8λ�첽�շ�
	SM0 = 0;
	SM1 = 1;
	SM2 = 1; 	// ֻ�н��յ���Чֹͣλʱ�Ų����ж�
	PS = 1;		// �����ж����ȼ���
	TI = 1;
	// �趨T1��ʱ��
	TMOD &= 0x0F;  // T1��ʽ2��T0���ֲ���
	TMOD |= 0x20;
	TH1 = GetSerialRate(baudrate);
	TL1 = TH1;								  
	
	REN = 1; 	// �����������
	ES = 1;		// �������ж�
	TR1 = 1;	// ����T1
	return MCU_OK;
}
/*********************************************************************
-- ���ܣ���������ͨѶ���ر��жϵ�
-- ������
-- ���أ��ɹ�=MCU_OK,ʧ��=MCU_ERROR
-- ������xu,2013-06-10
**********************************************************************/
void SerialClose(void)
{
	// �رն�ʱ��T1
	TR1 = 0;	// ֹͣ��ʱ��T1
	TMOD &= 0x0F;
	// ֹͣ����
	REN = 0;
	// �رմ����ж�
	ES = 0;
}

/*********************************************************************
-- ���ܣ�������Ϣ
-- ������pmsg=�����͵���Ϣ
--		 msglen=��Ϣ����
-- ���أ��ɹ����͵����ݳ���
-- ������xu,2013-06-14
**********************************************************************/
char SendMessage(BYTE *pmsg, char msglen)
{
	char idx = 0;
	if(pmsg == NULL)
		return -1;
	//while(!TI);
	for(idx = 0; idx < msglen; idx++)
	{
		SBUF = pmsg[idx];
		while(!TI);
		TI = 0;
	}
	return idx;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************
-- ���ܣ�������ȡ���ּĴ������ݵı���ADU
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ��ɹ����͵����ݳ���
-- ������xu,2013-06-14
**********************************************************************/
BYTE RTU_BuildRequestByReadRegisters(BYTE slave, BYTE addr, BYTE regnums, BYTE *preqbuf, BYTE reqbuflen)
{
    BYTE req_length;

   	if(regnums > MODBUS_MAX_READ_REGISTERS || preqbuf == NULL || reqbuflen > _MIN_REQ_LENGTH)
		return -1;

	req_length = RTU_BuildRequestBasis(slave, _FC_READ_HOLDING_REGISTERS, addr, regnums, preqbuf);
	req_length = RTU_SendMsgPre(preqbuf, req_length);
	req_length = SendMessage(preqbuf, req_length);

	return req_length;
}
/*********************************************************************
-- ���ܣ���ȡ����Ĵ�������
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ������ı����ֽ���
-- ������xu,2013-06-14
**********************************************************************/
BYTE RTU_BuildRequestByInputRegisters(BYTE slave, BYTE addr, BYTE regnums, BYTE *preqbuf, BYTE reqbuflen)
{	
    BYTE req_length;

	if(regnums > MODBUS_MAX_READ_REGISTERS || preqbuf == NULL || reqbuflen < _MIN_REQ_LENGTH)
		return -1;

	req_length = RTU_BuildRequestBasis(slave, _FC_READ_INPUT_REGISTERS, addr, regnums, preqbuf);
	req_length = SendMessage(preqbuf, req_length);

	return req_length;
}

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
BYTE RTU_ResolveDataFromResponseRegisters(BYTE *req, BYTE *rsp, BYTE rsplen, UINT *pregval, BYTE regvallen)
{
	BYTE rc = 0, idx = 0;
	const BYTE offset = 1;	// ADU����ͷ
	if(req == NULL || rsp == NULL || rsplen <= 0 || pregval == NULL || regvallen <=0)
		return 99;

	rc = RTU_CheckConfirmation(req, rsp, rsplen);
	for (idx = 0; idx < rc; idx++) 
	{
        // shift reg hi_byte to temp OR with lo_byte 
        pregval[idx] = (rsp[offset + 2 + (idx << 1)] << 8) |
            rsp[offset + 3 + (idx << 1)];
    }
	 
	return rc;
}

/*********************************************************************
-- ���ܣ�����Ӧ����ADU�н���������ֵ
-- ������rsp=�յ�����Ӧ����
--		 rsplen=��Ӧ���ĳ���
--		 addr=�豸��ַ
-- ���أ���ų��صĲ���ֵ��ռ��4Byte
-- ������xu,2013-10-14
**********************************************************************/
long RTU_ADS_ResolveData(BYTE *rsp, BYTE rsplen, BYTE *addr)
{
	BYTE rc = 0;
	if(rsp == NULL)
	{
		return -1;
	}
	if(RTU_CheckIntegrity(rsp, rsplen) == rsplen)
	{
		if(addr != NULL)
			*addr = rsp[0];
		return (((long)(rsp[3] << 8) | rsp[4]) << 16) | ((rsp[5] << 8) | rsp[6]);
	}
	return -1;
}