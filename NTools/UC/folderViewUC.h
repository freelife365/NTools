#pragma once
#include "UC/wucList.h"

namespace wuc {

class folderViewUC;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IFolderCallbackUC ��		2015-2-5
//	�ýṹ��folderViewUC��ӵ�����ṩ����ʹ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class IFolderCallbackUC : public IListCallbackUC
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-10 �б�ؼ���ʵ�ʰ����Ľڵ��� >=0 and < iItem*cols	
	virtual std::size_t getNodeCount() = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-10 �õ��ڵ����������ڵ㼯�ϵ�����	
	virtual int getNodeIndex(int iItem, int iSubItem) = 0;
	virtual std::wstring getNodeKeyName(int nodeIndex) = 0;
	virtual std::wstring getNodeTipInfo(int nodeIndex) = 0;
	virtual int getSubItemIndex(int nodeIndex) const = 0;	// ���ݽڵ������õ����Ӧλ������
	virtual bool getNodePosition(int nodeIndex, RECT *prcNode) const = 0;
	virtual bool ptInNode(int nodeIndex, POINT pt) = 0;		// ���������Ƿ����ͼ����ı���
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-10 �趨�ڵ�״̬ nodeIndex���������ؼ��ڽڵ���
	virtual bool setNodeState(int nodeIndex, DWORD dwAdd, DWORD dwRemove) = 0;
	virtual bool setNodeState(std::wstring wsKeyName, DWORD dwAdd, DWORD dwRemove) = 0;
	virtual DWORD getNodeState(int nodeIndex) = 0;
	virtual bool reverseNodeState(int nodeIndex, DWORD dwReverseState) = 0;
	virtual void setAllNodeState(DWORD dwAdd, DWORD dwRemove) = 0;
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-5 ʹĳ���ڵ㴦��ѡ��+����״̬�������ڵ��״̬���	
	virtual bool selectNodeOnly(int nodeIndex) = 0;
	virtual folderViewUC* getFocusFolderView() = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-5 �ؼ�λ�øı�
	virtual void positionChanged() = 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-6 ���еĿ������Ľڵ���ֻ����һ���ڵ㴦�ڽ���״̬
struct BSFocusedNodeInfo {
	folderViewUC	*pfolderView;	// ����ڵ������������
	//int				iNodeIndex;		// ����ڿ�������ڲ��ڵ㼯�ϵ�����
	std::wstring	wsKeyName;		// �ڵ�Key���ƣ�����ǰ�������뱾�ṹ��һ��ʱֻ����key��������
	//bool			bDoneSelect;	// �Ƿ�ִ�й�ѡ�в����������ظ�����dialogMain::setAllNodeState���ѡ��״̬
	// 
	BSFocusedNodeInfo()
		: pfolderView(nullptr)
		//, iNodeIndex(-1)
		//, bDoneSelect(false)
	{}

	// �ͷŽڵ�Ľ���״̬
	void releaseState();
	
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-2-9 ֻ����һ���ڵ㴦��hot״̬
struct BSHotNodeInfo {
	folderViewUC	*pfolderView;	// hot�ڵ������������
	int				iNodeIndex;		// ����ڿ�������ڲ��ڵ㼯�ϵ�����
	//std::wstring	wsKeyName;		// �ڵ�Key����
	BSHotNodeInfo()
		: pfolderView(nullptr)
		, iNodeIndex(-1)
	{}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-22	listUC���õ�������ʾ��ʽ
enum BEFolderStyleFlag {
	BEFolderStyleFlagList			= 0,	//	���б��ʽ��ʾ��1�ж��У�
	BEFolderStyleFlagIcon					//	���е�ͼ���ı���ʽ��ʾ�����ж��У�
											//		1��ͼ���С���С���ߴ�ֱ�Ϊ32��48��96����С16�����256
											//		2���ڵ���С���Ϊ20�����Ϊ260����ǰ���ΪSystemParametersInfo<SPI_ICONHORIZONTALSPACING>
											//		3����ǩ����Ӧ���磺����19 ˫��36...
											//		4�����Ʊ�ǩ�����Ҹ�Ԥ��2�����¸�Ԥ��1
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	folderElementUC		2015-2-2
//	��Ԫ�����е�ͼ����б�ȷ�ʽ��ʾ�ڵ�
//	�ڵ�������4������ɣ��ڵ�����ͼ��������ǩ����ͼ�����ͱ�ǩ�������ڽڵ�����
//	�ڵ�������СΪ1��ͼ��+���2���ı���Ĭ�ϣ�ѡ��ʱ��ȫ��ʾ��
//	��ǩ�����ڱ�ǩ��ʾ��Χ�Ļ����ϣ���������չ��2����������չ1
//	ͼ��������СΪ8��������������������Ϊ�ڵ��ȣ��߶�Ϊͼ��߶���������չ2
//	��ǩ����ͼ�������1
////////////////////////////////////////////////////////////////////////////////////////////////////////
class folderElementUC : public listElementUC
{
public:
	folderElementUC();
	~folderElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void Event(BSEventUC& event);
	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void paint(HDC hdc, const RECT &rcPaint);
public:
	/****************************************************************************************
		<< --- 		2015-2-5 --- >>         
		˵�����ҵ���������λ�õ������������ܵĻ�������������λ��
		������
			pt			= ������ĵ�
			prcNodePos	= ��Ϊ��ʱ���ؽڵ�λ������
		����ֵ��= -1δ�ҵ� >=0Ϊ������
	*****************************************************************************************/
	int findColumn(POINT pt, RECT *prcNodePos = NULL);
	
public:
	// IListItemUC
	virtual void drawItem(HDC hdc, const RECT& rcItem);
	virtual void setOwner(controlUC* pOwner);
public:
	//PCWSTR getFullPath(int iSubItem);
	//void setFullPath(int iSubItem, PCWSTR pfullPath);
	//BEFolderStyleFlag getFolderStyle() const;
	//void setFolderStyle(BEFolderStyleFlag style);
protected:
	folderViewUC		*m_pOwner;
	BEFolderStyleFlag	m_beFolderStyle;
	//std::vector<std::wstring>	m_vecFullPathSubs;	// �����ļ��л��ļ���ȫ·�����ϣ��������������������һ���п��Բ�ȫ
};




////////////////////////////////////////////////////////////////////////////////////////////////////////
//	folderViewUC��		2015-1-29
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class folderViewUC : public listUC
{
public:
	folderViewUC();
	~folderViewUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
	virtual void Event(BSEventUC& event);
	virtual void setPosition(RECT pos);
	//virtual SIZE estimateSize(SIZE szAvailable);
public:
	/****************************************************************************************
		<< --- 	init	2015-2-4 --- >>         
		˵������ʼ�����������б�ͷ���б�Ԫ��
		������
			styleFlag	= �б��ı�����ʽ
			szNode		= ��BEFolderStyleFlagIconģʽ�µ���Ԫ�صĳߴ�
			szNodeSpace	= �ڵ��Ҳ�ĵײ�Ԥ���ռ�
		����ֵ��
	*****************************************************************************************/
	void init(BEFolderStyleFlag styleFlag, SIZE szNode, SIZE szNodeSpace);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-5 ˢ��ָ���ڵ�����subItemIndex�ڵ�λ������
	void invalidateEx(int subItemIndex);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-6 ��������λ�������õ���λ��
	bool getSubItemPosition(int subItemIndex, RECT &rcPos);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-23 ��������λ�������õ�����������ֵ
	bool getItemPositionIndex(int subItemIndex, int &refiItem, int &refiSubItem);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-4 �����б���ʽ�ͽڵ��С�Լ������С���²����б������Ԫ��	
	void refreshLayout(bool bforceUpdate = false);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-10 �趨�ڵ��hot״̬
	void hoverNode(int iItem, int iSubItem);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-2-10 �趨�ڵ��ѡ��״̬
	//void selectNode(int iItem, int iSubItem, UINT keyState);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-11 ֻѡ��ָ���Ľڵ㣬��ʹ�����ڵ㴦�ڷ�ѡ��״̬
	int selectNodeOnly(int iItem, int iSubItem);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-11 �õ�ָ���ڵ��״̬
	DWORD getNodeState(int iItem, int iSubItem);
	/****************************************************************************************
		<< --- setNodeFocus		2015-3-5 --- >>         
		˵�����趨�ڵ��focus״̬������ѡ��״̬�Ľڵ���봦��ѡ��״̬
		������
			nodeIndex	= �õ�����Ľڵ�����������0������ڿ��ڽڵ㼯�ϣ�
		����ֵ��ԭ�Ƚڵ����������ԭ�ڵ㲻���ڱ��򷵻�-1
	*****************************************************************************************/
	int setNodeFocus(int nodeIndex);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-11 ���ڷ�Χ�ڵ��ѡ�У������ؽ���ڵ�����
	int selectRangeNode(int iItem, int iSubItem);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-23 	
	int getCols() const;
	int getRows() const;
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-6-30 ���ҵ����ڵ��б���Ԫ��
	listElementUC* findElement(POINT pt) const;
protected:
	/****************************************************************************************
		<< --- 		2015-2-10 --- >>         
		˵���������б�ͷ������
		������
			targetCols	= �б�ͷ��Ŀ������
			fixedWidth	= �п��
			bforceUpdateWidth	= �Ƿ�ǿ��ˢ�������п��
		����ֵ���Ƿ�������������
	*****************************************************************************************/
	bool modifyHeaderCount(int targetCols, int fixedWidth = 0, bool bforceUpdateWidth = false);
	/****************************************************************************************
		<< --- 		2015-2-10 --- >>         
		˵���������б�Ԫ������
		������
		����ֵ��
	*****************************************************************************************/
	bool modifyElementCount(int targetRows, int fixedHeight = 0, bool bforceUpdateHeight = false);
public:
	BEFolderStyleFlag getFolderStyle() const;
	void setFolderStyle(BEFolderStyleFlag style);
	SIZE getNodeWorkSize() const;
	SIZE getNodeSize() const;
	void setNodeSize(SIZE szNode);
	SIZE getNodeSpaceSize() const;
	void setNodeSpaceSize(SIZE szNode);
	void setOwner(controlUC *pOwner);
	IFolderCallbackUC* getOwner() const;
	void setListItemHeight(int iHeight);
	int getListItemHeight() const;
	//BSFocusedNodeInfo& getFocusedNode();
	BSHotNodeInfo& getHotNode();
	bool isSetterHorizontal() const;
	void setSetterHorizontal(bool bHorizontal);
	bool isSetterAnywhere() const;
	void setSetterAnywhere(bool bAnywhere);
	int getCurrentNodeIndex() const;
	std::wstring toString() const;
protected:
	BEFolderStyleFlag	m_beFolderStyle;
	bool				m_bSetterHorizontal;		// ��BEFolderStyleFlagIconģʽ�£�=true �ڵ�ˮƽ�����ҡ����ϵ��£�=false ���ϵ��¡�������
	bool				m_bSetterAnywhere;			// �ڵ����������������λ��
	int					m_iListItemHeight;			// ��BEFolderStyleFlagListģʽ�µ���Ԫ���и�
	
	SIZE				m_szNode;					// �ڵ��������С
	SIZE				m_szNodeSpace;				// �ڵ���϶���ճ��ڵ��Ҳ�͵ײ�
	
	IFolderCallbackUC	*m_pOwner;
	int					m_iCurNodeIndex;			// ��ǰѡ�еĽڵ�����������ڱ���ڵ㼯�ϣ�
	//static BSFocusedNodeInfo	mc_bsFocusedNode;	// �þ�̬�ģ��Ա�֤�������乲��
	static BSHotNodeInfo		mc_bsHotNode;		// �þ�̬�ģ��Ա�֤�������乲��
};



}	// end wuc