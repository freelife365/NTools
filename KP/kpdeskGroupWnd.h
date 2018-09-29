#ifndef __KPDESKGROUPWND_H__
#define __KPDESKGROUPWND_H__
#pragma once

#include "UI/win/ctrl/deskListCtrl.h"

class dialogBuilderCallbackEx : public IDialogBuilderCallback
{

public:
	controlUI* createControl(PCWSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, L"deskListCtrl") == 0 ) return new gtc::deskListCtrl();
		return NULL;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	kpdeskWnd��	2014-06-04
//	������Ϣ��
//	ǰ��������Ϊ���û������ã����ڿ���������Ϊǽֽ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class kpdeskGroupWnd  : public wui::windowWnd, public wui::INotifyUI
{
public:
	kpdeskGroupWnd(void);
	~kpdeskGroupWnd(void);
public:
	// wui
	virtual PCWSTR getWindowClassName() const { return L"WUIDeskGroupWnd";							}
	virtual UINT getClassStyle() const {
		return CS_VREDRAW | CS_HREDRAW;
		return CS_DBLCLKS;
	}
	UINT getWindowStyle() const {				
		return WS_POPUP;
		return WS_THICKFRAME|WS_CLIPSIBLINGS;
	}
	UINT getWindowExStyle() const {				
		return WS_EX_LAYERED|WS_EX_NOACTIVATE|WS_EX_TRANSPARENT;
		return WS_EX_WINDOWEDGE|WS_EX_CONTROLPARENT|WS_EX_TOOLWINDOW;
	}
	virtual void onFinalMessage(HWND hWnd);
	virtual void notify(wui::TNotifyUI& msg);
	virtual LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT onSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool textTitleNotify(void *param)
	{
		TNotifyUI* pMsg = (TNotifyUI*)param;
		if (pMsg)
		{
			SEND_LOG(L"txt:%s", pMsg->sType.c_str());
		}
		else
			SEND_LOG(L"txt");
		return true;
	}
public:

	/*****************************************************************************************
		<< ---  kpdeskGroupWnd::setWindowPos		2014-06-04 --- >>
		˵�����趨�����������������������λ�ã�����Ԥ�������л���һЩ����
		������
		idx		= ����������������������0
		����ֵ��
	*****************************************************************************************/
	void setWindowPos(int32_t idx);

	/*****************************************************************************************
		<< ---  kpdeskGroupWnd::setTitle		2014-06-05 --- >>
		˵�������ô��ڱ���
		������
		ptitle	= ��������
		����ֵ��
	*****************************************************************************************/
	void setTitle(PCWSTR ptitle);

	/*****************************************************************************************
		<< ---  kpdeskGroupWnd::allocNodes		2014-06-09 --- >>
		˵��������m_mapDeskNodes�ڵ���������deskListCtrl�з��䲼�ֿռ�
		������
		����ֵ��
	*****************************************************************************************/
	void allocNodes();
	/*****************************************************************************************
		<< ---  kpdeskGroupWnd::redrawNodes		2014-06-09 --- >>
		˵�������»��ƽڵ���Ϣ
		������
		����ֵ��
	*****************************************************************************************/
	void redrawNodes();
	void redrawNode(const std::wstring &nodeName, HDC sourceDC);
	/*****************************************************************************************
		<< ---  kpdeskGroupWnd::getContainScreenPOS		2014-06-14 --- >>
		˵�����õ���������Ļ����
		������
		����ֵ��
	*****************************************************************************************/
	RECT getContainScreenPOS();
	bool getNodePos(const std::wstring &nodeName, RECT &rt);
public:
	std::wstring getGroupGuid() const {			return m_wsGuid;						}
	gtc::idlNode::IDINodeMap& getNodes() {		return m_mapDeskNodes;					}
	HDC getPaintDC() const {					return m_paintManager.getPaintDC();		}
	void setPaintDC(HDC hdc) {					
		m_paintManager.setPaintDC(hdc);
		m_paintManager.needUpdate();
		sendMessage(WM_PAINT);
	}
protected:
	wui::paintManagerUI m_paintManager;

	bool m_bWinInit;
	std::wstring	m_wsGuid;	// �������ֲ�ͬ�Ĵ���
	gtc::idlNode::IDINodeMap	m_mapDeskNodes;	// ���鴰���ڵ�ͼ�꣬�����ͷţ�ͬһ��kpdeskAdmin�ͷ�
};


#endif
