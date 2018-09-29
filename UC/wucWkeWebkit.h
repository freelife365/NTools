#ifndef __WUCWKEWEBKIT_H__
#define __WUCWKEWEBKIT_H__
#pragma once
#include "wucContainer.h"
#include "windowWnd.h"
#include "../wke/wke.h"
namespace wuc {
class wkeWebkitWnd;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	wkeWebkitUC  ��		2015-9-16
//	����wke�ķ�װ��
//	�ĵ��ļ���״̬�뵱ǰurl��أ���url�ı��ǰ״̬��Ҫ����
////////////////////////////////////////////////////////////////////////////////////////////////////////

class wkeWebkitUC : public containerUC
{
	friend class wkeWebkitWnd;
public:
	wkeWebkitUC();
	~wkeWebkitUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void Event(BSEventUC &event);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	virtual void setPosition(RECT pos);
	virtual void setScrollPos(SIZE szPos);
	virtual void onInit();
	virtual void setVisible(bool bVisible = true);
	virtual void setInternVisible(bool bVisible = true);

	void setURL(std::wstring strValue);
	void setFile(std::wstring strValue);
	std::wstring runJS(std::wstring strValue);
	void setClientHandler(const wkeClientHandler* handler);
	void stopLoading();

	void goBack();
	void goForward();
	/****************************************************************************************
		<< --- buildJSFunction	2016-1-6	kp007 --- >>         
		˵������js�����󶨵�C++������
		������
			jsfuncName	= ���󶨵�js��������
			exeFunc		= �󶨵�cpp�������������ִ��jsfuncName����ʱ���ñ�����
			argCount	= js�����Ĳ�������
		����ֵ��
	*****************************************************************************************/
	void buildJSFunction(const char *jsfuncName, jsNativeFunction exeFunc, int argCount);

public:
	windowWnd* getWindow();
	std::wstring getURL() const;		// �õ��״�setURLʱ���õ�����
	std::wstring getCurrentURL() const;	// �ڲ��䵼��������
protected:
	void initScrollBar(SIZE szScroll);
public:
	static void wkeWebkit_Init();
	static void wkeWebkit_Shutdown();

	static wkeWebkitUC* findWebkitUC(const wkeClientHandler *pHandler);
	static void removeClientHandler(const wkeClientHandler *pHandler);
	static std::map<const wkeClientHandler *, wkeWebkitUC *>	mc_mapClientHandler;
protected:
	wkeWebkitWnd	*m_pWindow;
};



}	// wuc
#endif	// __WUCWKEWEBKIT_H__