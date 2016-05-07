#ifndef __WALLPAPER_H__
#define __WALLPAPER_H__
#pragma once
#include "AC/sysupport.h"
#include "AC/baseReg.h"
namespace acm {
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-21	����ǽֽ�Ļ�����ʽ
enum BEWallpaperStyle
{
	BEWallpaperStyleCenter		,	//����
	BEWallpaperStyleTile		,	//ƽ��
	BEWallpaperStyleStretch		,	//����
	BEWallpaperStyleFit			,	//��Ӧ
	BEWallpaperStyleFill			//���
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	wallPaper��	2015-1-21
//	ǽֽ��Ϣ�ռ����Լ����ǽֽ�ĸı�
//	ע�⣺
//		1��WinXP�޷����յ�SPI_SETDESKWALLPAPER��Ϣ��
//		2���������±仯��ֽʱע����б�ֽ�ļ�ȫ·�����ᷢ���ı䣬��sogo��ֽ���Ըı����趨ֵ��
//	���������ͬʱ�������3��״̬�ĸı�
//		1��ͬʱ���ע���HKEY_CURRENT_USER\Control Panel\Desktop\Wallpaper��ֵ�ı䡣
//		2��ϵͳǽֽĿ¼�ļ���Win7_later����C:\Users\xxx\AppData\Roaming\Microsoft\Windows\Themes\TranscodedWallpaper.jpg����
//			WinXp����C:\Documents and Settings\xxx\Local Settings\Application Data\Microsoft\Wallpaper1.bmp����
//			�ļ����ԣ���С������޸����ڣ��ı仯��
//		3���������յ�SPI_SETDESKWALLPAPER��Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class wallPaper
{
public:
	wallPaper();
	~wallPaper();

public:
	/****************************************************************************************
		<< --- refresh		2015-1-21 --- >>         
		˵����ˢ��ǽֽ������Ϣ
		������
			bcopyWallfile	= �Ƿ����Ϊ��ֽ�ļ�
		����ֵ��
	*****************************************************************************************/
	void refresh(bool bcopyWallfile = true);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-22	����ǽֽ
	void draw(HDC hdc);

	/****************************************************************************************
		<< --- startMonitor		2015-1-21 --- >>         
		˵��������ǽֽ����߳�
		������
			hReceiver	= ����ǽֽ�ı���Ϣ�Ĵ���
			uMsg		= ��Ϣ���
		����ֵ���Ƿ������ɹ�
	*****************************************************************************************/
	bool startMonitor(HWND hReceiver, UINT uMsg);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-21	ֹͣ���
	void stopMonitor();
public:
	// ������Ļ��ͼ��ߴ��������Ҫ���Ƶ�ǽֽԴ���򣬷�����ĻԴ����������ĻĿ������
	static void estimateWallparperArea(BEWallpaperStyle style, SIZE szImage, SIZE szScreen, RECT &rcImage, RECT &rcScreen);

	/****************************************************************************************
		<< --- threadMonitor		2015-1-21 --- >>         
		˵�����������1��2�ļ���߳�
		������
			pparam	= wallPaper*
		����ֵ��
	*****************************************************************************************/
	static unsigned int WINAPI threadMonitor(void *pparam);
	/****************************************************************************************
		<< --- threadCopyWallfile		2015-6-16 --- >>         
		˵�������Ը���ǽֽ�ļ�
		������
			pparam	= _BSCopyWallfileData*
		����ֵ��
	*****************************************************************************************/
	static unsigned int WINAPI threadCopyWallfile(gtc::thread::_BSThreadInfo *pthr);
protected:
	struct _BSCopyWallfileData {
		std::wstring	wsSourceFile;	// ǽֽԴ�ļ�ȫ·��
		wallPaper		*pWallPaper;	// Ŀ���ļ�ȫ·��
	};
protected:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-21	�趨ϵͳǽֽ·��
	void init();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-22	�鿴ע���ǽֽ·���Ƿ��б仯���������ԣ���С������޸����ڣ����ޱ仯
	bool checkChanged();
public:
	BEWallpaperStyle getStyle() const {			return m_beStyle;						}
	PCWSTR getFullPath() const {				return m_wsFullPath.c_str();			}
protected:
	BEWallpaperStyle	m_beStyle;				// ǽֽ�Ļ�����ʽ
	std::wstring		m_wsFullPath;			// ������ǽֽͼƬȫ·��
	std::wstring		m_wsSysWallPath;		// ϵͳǽֽ·��
	std::wstring		m_wsSysWallFileFullPath;// ϵͳǽֽ�ļ�ȫ·��

	acm::regKey			m_regWall;
	HANDLE				m_eventReg;		// ��ע����� HKEY_CURRENT_USER\Control Panel\Desktop

	HANDLE				m_eventExit;	// �˳�����¼�
	HANDLE				m_hThread;		// ����߳�
	HWND				m_hReceiver;	// ����ǽֽ�ı���Ϣ�Ĵ���
	UINT				m_uiMsgFlag;	// ��Ϣ���
};
}	// end acm
#endif