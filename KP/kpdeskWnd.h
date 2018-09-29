#pragma once
#include "kpdeskGroupWnd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	kpdeskWnd��	2014-06-04
//	������Ϣ��
//	ǰ��������Ϊ���û������ã����ڿ���������Ϊǽֽ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class kpdeskWnd  : public wui::windowWnd, public wui::INotifyUI
{
public:

public:
	kpdeskWnd(void);
	~kpdeskWnd(void);
public:
	// wui
	virtual PCWSTR getWindowClassName() const { return L"WUIDeskWnd";							}
	virtual UINT getClassStyle() const {		return CS_DBLCLKS;								}
	UINT getWindowStyle() const {				return WS_THICKFRAME;							}
	UINT getWindowExStyle() const {				return WS_EX_WINDOWEDGE|WS_EX_CONTROLPARENT|WS_EX_TOOLWINDOW;							}
	virtual void onFinalMessage(HWND hWnd);
	virtual void notify(wui::TNotifyUI& msg);
	virtual LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:

	/*****************************************************************************************
		<< ---  kpdeskWnd::createGroupWindow		2014-06-04 --- >>
		˵������������ͼ������鴰�ڣ��磺ϵͳ���ߣ�Ӧ�ó���ͼƬ���ļ��У��ĵ��ļ��������ļ�
		������
		����ֵ��
	*****************************************************************************************/
	void createGroupWindow();

	/*****************************************************************************************
		<< ---  kpdeskWnd::loadGroupWindow	2014-06-06 --- >>
		˵�����������ļ��м��ط��鴰��
		������
		����ֵ��
	*****************************************************************************************/
	void loadGroupWindow();

	/*****************************************************************************************
		<< ---  kpdeskWnd::closeGroupWindow	2014-06-04 --- >>
		˵�����ر���createGroupWindow�����Ĵ���
		������
		����ֵ��
	*****************************************************************************************/
	void closeGroupWindow();

	/*****************************************************************************************
		<< ---  kpdeskWnd::classifyNodes		2014-06-09 --- >>
		˵����������ͼ����з���
		������
		mapGroup	= ͼ�������鴰���ֵ�
		����ֵ��
	*****************************************************************************************/
	bool classifyNodes(std::map<BEDeskGroupFlag, kpdeskGroupWnd *> &mapGroup);

	/*****************************************************************************************
		<< ---  kpdeskWnd::classifyNodes		2014-06-14 --- >>
		˵������������ͼ�������ҵ�ͼ�������Ĵ���
		������
		nodeName	= ͼ������
		����ֵ��
			��= null	��������
			= null	������
	*****************************************************************************************/
	kpdeskGroupWnd* findGroupWnd(const std::wstring &nodeName);
	
	/*****************************************************************************************
		<< ---  kpdeskWnd::getNodeContainScreenPOS		2014-06-14 --- >>
		˵������������ͼ�����Ƶõ�������������������Ļ����
		������
		nodeName	= ͼ������
		����ֵ��
			��= empty()	��������
			= empty()	������
	*****************************************************************************************/
	RECT getNodeContainScreenPOS(const std::wstring &nodeName);
public:	// staic
	
	
	/*****************************************************************************************
		<< --- static kpdeskWnd::initHost		2014-06-04 --- >>
		˵����������������
			1������һ�����ɼ����壨�Ժ������ǽֽ���壩�������û�������������û����͵��˳�����
			2���õ�������Ϣ
			3���������鴰��
		������
		pparam	= ��dll��HMODULE
		����ֵ��
		= 0 �ɹ�
		> 0 ʧ��
	*****************************************************************************************/
	static unsigned int WINAPI initHost(void *pparam);

	/*****************************************************************************************
		<< --- static kpdeskWnd::stopHost		2014-06-05 --- >>
		˵�����ָ�Ĭ�ϻ��������ر����д���
		������
		pparam	= ��dll��HMODULE
		����ֵ��
		= 0 �ɹ�
		> 0 ʧ��
	*****************************************************************************************/
	static unsigned int WINAPI stopHost(void *pparam);

public:
	std::vector<kpdeskGroupWnd *>& getGroupWnd() {				return m_vecGroupWnd;		}
protected:
	wui::paintManagerUI m_paintManager;
public:
	//static HMODULE		mc_hModule;			// ��dll��HMODULE
	//static HWND			mc_hProgman;		// Program Manager���ھ��
	//static HWND			mc_hShelldll;		// ShellDLL_DefView���ھ��
	//static HWND			mc_hDeskListView;	// ����ؼ�SysListVew32���
protected:
	bool	m_bFirstExec;				// �����װ���һ������
	//gtc::kpdeskAdmin	m_deskAdmin;	// 
	std::vector<kpdeskGroupWnd *>	m_vecGroupWnd;	// ����ͼ����鴰��
	gtc::idlNode::IDINodeMap		m_mapDeskNodes;	// �����ϵ�ͼ��
};

