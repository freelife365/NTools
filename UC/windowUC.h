#pragma once
#include "wucdef.h"
#include "wucContainer.h"

namespace wuc {
//////////////////////////////////////////////////////////////////////////
// �߽���
typedef enum BEUCBorderFlag {
	BEUCBorderFlagLeft		= 1,	
	BEUCBorderFlagTop		,		
	BEUCBorderFlagRight		,		
	BEUCBorderFlagBottom				
}BEUCBorderFlag;
//////////////////////////////////////////////////////////////////////////
// �Զ�����̲����ӿ�
class IKeyEvent
{	// nFlag(shift ctrl alt���Ƿ񱻰���
	virtual void onKeyDown(UINT nChar, UINT nFlag, UINT nRepCnt, bool& bHandled) = 0;
	virtual void onKeyUp(UINT nChar, UINT nFlag, UINT nRepCnt, bool& bHandled) = 0;
	virtual void onChar(UINT nChar, UINT nFlag, UINT nRepCnt, bool& bHandled) = 0;
};
//////////////////////////////////////////////////////////////////////////
// �Զ����������ӿ�
class IMouseEvent
{
	virtual void onLButtonDown(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onLButtonUp(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onLButtonDBClick(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onRButtonDown(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onRButtonUp(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onMouseMove(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual void onMouseWheel(UINT nFlags, short zDelta, POINT pt, bool& bHandled) = 0;
	virtual void onMouseEntry(bool& bHandled) = 0;
	virtual void onMouseLeave(bool& bHandled) = 0;
	virtual void onMouseHover(UINT nFlags, POINT pt, bool& bHandled) = 0;
	virtual LRESULT onNcHitTest(POINT pt) = 0;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////	windowTitleUC��	2015-1-22
////	windowUC���ڵı��������ڿɷ��������С���رյȰ�ť
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//class windowTitleUC : public horizontalLayoutUC
//{
//public:
//	windowTitleUC();
//	~windowTitleUC();
//	virtual PCWSTR getClass() const;
//	virtual LPVOID getInterface(PCWSTR pstrName);
//
//protected:
//	int getHeight() const;
//	void setHeight(int height);
//	PCWSTR getTitle() const;
//	void setTitle(std::wstring wsTitle);
//protected:
//	std::wstring	m_wsTitle;
//};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-22	���ڲ���
enum BESelectMode {
	BESelectModeNone	= 0,	// 
	BESelectModeSize	,		// �߿�˴θı�
	BESelectModeMove			// �����ƶ�
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-23	��ǰ����
enum BEUCActiveFlag
{
	BEUCActiveFlagNone			= 0,	// 
	BEUCActiveFlagSize			,		// �ı䴰��λ�û�ߴ�
	//BEUCActiveFlagSysMenu		,		// �˵�
	BEUCActiveFlagEditTitle		,		// ����������
	BEUCActiveFlagScroll		,		// ����������
	BEUCActiveFlagUserMenu		,		// �鴰���Ҽ��˵�
	BEUCActiveFlagDrapDrop				// ��קͼ��
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	windowUC��	2015-1-22
//	ģ��Ĵ��ڻ���
//	���ڹ���������3���֣����⣨�������أ�����������ˮƽ����ֱ���������أ����ͻ���
//	ע�⣺
//		1���������������屾�����������ɿͻ�������
//		2���ͻ����Ĵ�С���ݱ��⡢�������Ŀɼ��Խ��в��֣�������ͻ����ڶ��ı�ߴ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class windowUC : public verticalLayoutUC, public IMouseEvent
{
public:
public:
	windowUC(void);
	~windowUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	void Event(BSEventUC &event);
	virtual void setPosition(RECT pos);
	virtual void paintStatusImage(HDC hdc);

public:
	// IMouseEvent
	virtual void onLButtonDown(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onLButtonUp(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onLButtonDBClick(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onRButtonDown(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onRButtonUp(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onMouseMove(UINT nFlags, POINT pt, bool& bHandled);
	virtual void onMouseWheel(UINT nFlags, short zDelta, POINT pt, bool& bHandled);
	virtual void onMouseEntry(bool& bHandled);
	virtual void onMouseLeave(bool& bHandled);
	virtual void onMouseHover(UINT nFlags, POINT pt, bool& bHandled);
	virtual LRESULT onNcHitTest(POINT pt);
	// 
	/*****************************************************************************************
		<< --- windowUC::ptInSizeBox		2014-06-23 --- >>
		˵�����鿴����Ƿ��ڴ��ڱ߽�ĸı�ߴ���϶������������Ӧ�ı���״̬
		������
		pt	= ��������
		����ֵ���õ����Ե��ǣ��磺HTNOWHERE��HTCLIENT...
	*****************************************************************************************/
	//virtual int hitTest(const POINT &pt);
	/*****************************************************************************************
		<< --- windowUC::moveTo		2014-06-24 --- >>
		˵�����������ƶ����µ�λ��
		������
		position	= �µ�λ�ã���Ļ���꣩
		����ֵ��
	*****************************************************************************************/
	virtual void moveTo(const RECT &position, bool mustMove = false);
	/*****************************************************************************************
		<< --- windowUC::moveHandleTo		2014-06-24 --- >>
		˵����ͨ���߽�ı䴰�ڵĳߴ�
		������
		handle	= �߽�λ��
		pt		= �߽��µ�λ�ã���Ļ���꣩
		����ֵ��
	*****************************************************************************************/
	virtual void moveHandleTo(int handle, const POINT &pt);

	/*****************************************************************************************
		<< --- windowUC::close		2014-07-17 --- >>
		˵�����˳����ڣ����ͷ���Դ
		������
		����ֵ��
	*****************************************************************************************/
	//virtual void close();
	/*****************************************************************************************
		<< --- windowUC::getCursor		2014-06-23 --- >>
		˵�����õ����Ե�����λ�õĹ��
		������
		handle	= ���Ե��ǣ�һ����hitTest�õ�
		����ֵ�������
	*****************************************************************************************/
	virtual HCURSOR getCursor(int handle);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-26	�����ǿͻ����Ĳ���
	bool ptIn(const POINT &pt, bool inclusionNc);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-19 �����(��Ļ����)�Ƿ����ڿͻ���
	bool ptInClient(POINT pt);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-19 �����(��Ļ����)�Ƿ����ڱ�����
	bool ptInCaption(POINT pt);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-19 	
	void clientToScreen(RECT &rc);
	void clientToScreen(POINT &pt);
	void screenToClient(RECT &rc);
	void screenToClient(POINT &pt);
	// �����ƶ����Ĵ���λ��
	RECT getWindowNcPos() const;
public:
	horizontalLayoutUC* getTitleBar() const;
	horizontalLayoutUC* getClientUC() const;
	horizontalLayoutUC* getTaskBar() const;
	virtual void setLockPos(bool lock);
	bool isLockPos() const;
	BESelectMode getSelectMode() const;
	bool isActivated() const;
	void setActivated(bool activate);
	bool isSelected() const;
	void selected(bool bSelected = true);
	BEUCActiveFlag getActiveFlag() const;
	void clearActiveFlag();
	bool isInitial() const;
	int getOperateHandle() const;
	PCWSTR getTitle() const;
	void setTitle(const std::wstring &wsTitle);
	int getTitlebarHeight() const;
	void setTitlebarHeight(int height);
	RECT getTitlebarPos() const;
	int getTaskbarHeight() const;
	void setTaskbarHeight(int height);
	RECT getTaskbarPos() const;
protected:
	horizontalLayoutUC	*m_pTitleBar;		// ������
	horizontalLayoutUC	*m_pClientUC;		// �ͻ���
	horizontalLayoutUC	*m_pTaskBar;		// ��������״̬��

	BEUCActiveFlag	m_beActive;			// ��ǰ�������
	int				m_iOperateHandle;	// ��ǰ�����������
	POINT			m_lastMousePos;		// ��굱ǰ����
	RECT			m_rcSizeBox;		// ���ڳߴ���Ըı䣬��һ�߲����϶�������Ϊ0
	bool			m_bInitial;			// ����û�в���ǰ��״̬�������entry���false
	bool			m_bActivated;		// ��ǰ�Ƿ��ڻ״̬����������뿪ʱ�ı��ֵ
	bool			m_bSelected;		// ��ǰ�����Ƿ�ѡ�У��磺������ô���
	bool			m_bLockPos;			// �����Ĵ��ڲ�����λҲ���ܸı��С
	BESelectMode	m_beSelectMode;		// ��ǰ�Ĳ���ģʽ
	std::wstring	m_wsTitle;
};

}
