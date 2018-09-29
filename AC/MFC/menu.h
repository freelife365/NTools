#pragma once
#include "AC/basedef.h"
//////////////////////////////////////////////////////////////////////////
// �˵����Ӧ�Ĳ���
typedef enum BEMenuAction {
	BEMenuActionNone	= 0,
	BEMenuActionAdd		,		// �����˵�
	BEMenuActionReplace	,		// �������滻�˵���
	BEMenuActionDelete			// ���Ƴ��˵�������Ʋ��ң�
}BEMenuAction;

namespace acm {
// �滻ϵͳ�˵��� ���鿴��������ʽ����ˢ�¡�
class menuItem {
public:
	typedef std::vector<menuItem *> PtrArrayType;
protected:
	menuItem();
public:
	menuItem(PCWSTR pName, BEMenuAction active);
	//menuItem(PCWSTR pName, bool isEnabled);
	menuItem(PCWSTR pName, UINT itemID, BEMenuAction active);
	menuItem(PCWSTR pName, UINT itemID, /*UINT pos, PCWSTR pVerb, PCWSTR pHelp, */HBITMAP hbitmap, BEMenuAction active);
	virtual ~menuItem();
public:
	// �����˵���Ϣ
	void buildMenuItemInfo(MENUITEMINFO &mii);
	// ����Ӳ˵�
	menuItem* addSubMenu(PCWSTR pName, UINT itemID = 0xFFFFFFFF, /*PCWSTR pVerb, PCWSTR pHelp, UINT pos = 0xFFFFFFFF, */HBITMAP hbitmap = NULL);
	//menuItem* addSubMenu(PCWSTR pName, UINT pos = 0xFFFFFFFF, HBITMAP hbitmap = NULL);
	// �����Ӳ˵�����Ӳ˵�
	HMENU createSubMenu();
	// ����ƥ����Ҳ˵�������������Ϣ
	bool refreshItemInfo(HMENU hMenu);

	std::wstring toString() const;
public:
	const std::wstring& getName() const;
	void setName(PCWSTR pName);
	BEMenuAction getAction() const;
	void setAction(BEMenuAction action);
	UINT getPosition() const;
	void setPosition(UINT pos);
	UINT getItemID() const;
	void setItemID(UINT uiID);
	bool isEnabled() const;
	void setEnabled(bool bEnabled);
	HBITMAP getHBMPChecked() const;
	void setHBMPChecked(HBITMAP hbitmap);
	HBITMAP getHBMPUnchecked() const;
	void setHBMPUnchecked(HBITMAP hbitmap);
	const std::wstring& getHelpInfo() const;
	void setHelpInfo(PCWSTR pHelp);
	const std::wstring& getVerb() const;
	void setVerb(PCWSTR pVerb);
	bool isSeparator() const;
	bool haveSubMenu() const;
	bool isCmdMenuItem() const;
	PtrArrayType& getSubMenu();
	bool isRadioCheck() const;
	void setRadioCheck(bool bRadio = true);
	bool isChecked() const;
	void setCheckState(bool bChecked = true);
public:
	static void init(UINT itemBaseID);
	static UINT getItemBaseID();

	static void free(PtrArrayType &arrayMenuItem);
	static menuItem* find(PtrArrayType &arrayMenuItem, UINT cmdID);
protected:
	static UINT		mc_uiBaseID;		// ��һ���˵���ID
	std::wstring	m_wsName;			// �˵����ƣ�Ϊ""��"-"ʱΪ�˵��еķָ���
	BEMenuAction	m_beAction;			// �Բ˵���Ķ���
	UINT			m_uiPosition;		// �˵�λ������
	UINT			m_uiID;				// �˵�ID������0��SEPARATOR�ͺ����Ӳ˵�������������ID
	HBITMAP			m_hbmpChecked;		// ѡ��ʱ��ͼ��
	HBITMAP			m_hbmpUnchecked;	// δѡ��ʱ��ͼ��
	std::wstring	m_wsHelp;			// ��ʾ��Ϣ
	std::wstring	m_wsVerb;			// �������

	HMENU			m_hSubMenu;			// 
	PtrArrayType	m_vecSubMenu;		// �Ӳ˵���

	UINT			m_uiType;			// ���ͣ�Ŀǰ֧����MFT_SEPARATOR��MFT_RADIOCHECK
	UINT			m_uiState;			// ״̬MFS_CHECKED��MFS_UNCHECKED��MFS_ENABLED

};
}