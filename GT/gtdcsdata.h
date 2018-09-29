#ifndef __GTDCSDATA_H__
#define __GTDCSDATA_H__
#pragma once


// ��ʾ������
#pragma pack(push)
#pragma pack(1)
struct pn_infor{
	unsigned short addr;	// ��ʾ����ַ
	unsigned char modem_no;	// ģ���
	char msg[400];			// ��ʾ����
	unsigned int len;		// ��Ч���ݳ���
};
#pragma pack(pop)

// ָ���Ӧ���¼������͸��û������
#define WM_G                        WM_USER+114
#define WM_T                        WM_USER+115
#define WM_Ac                       WM_USER+116
#define WM_Z1                       WM_USER+117
#define WM_Pn                       WM_USER+118
#define WM_H						WM_USER+119
#define WM_Log						WM_USER+200		// ֪ͨ�û�����־��Ϣ����������socket����Ϣ
// G����Ӧ���־
#define _GTCMD_RECVTYPE_G		't'	// 74H
// Pn����Ӧ���־
#define _GTCMD_RECVTYPE_Pn		'd'	// 64H
// H����Ӧ���־
#define _GTCMD_RECVTYPE_H		'h'	// 68H
// Ac����Ӧ���־
#define _GTCMD_RECVTYPE_Ac		'a'	// 
// Ӧ�������ֹ��
#define _GTCMD_RECVTYPE_EOF		0x0D	// 0DH
// Pnָ����ģ��ָ��
#define _GTCMD_MODEL_SPLIT		0x1D
// Pnָ����ģ����������ָ��
#define _GTCMD_MODELAREA_SPLIT	0x1F// ','

#endif