#pragma once
#include "wucdef.h"
#include "wucContainer.h"
namespace wuc {

///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-27	listUC������Ϣ��״̬�����������Ա���ͼ���ȣ�������ɫ=0ʱ���û��Ʊ���ɫ
//					���ṹ��Ӱ��listElementUC����
#define UCLIST_MAX_COLUMNS	32		// �������
typedef struct BSListItemInfo {
	int			iColumns;				// ��ǰ����
	RECT		rcColumn[UCLIST_MAX_COLUMNS];		// 
	int			iFont;					// 
	UINT		uTextStyle;				// �ı�������ʽ*
	RECT		rcTextPadding;			// Ԫ������
	DWORD		clrText;				// ������ʾʱ���ı�ɫ
	DWORD		clrBk;					//		�ı���ɫ
	bool		bAlternateBk;
	//BYTE		nAlpha;					//		
	DWORD		clrSelectedText;		// ��ѡ��ʱ���ı�ɫ
	DWORD		clrSelectedBk;			//		�ı���ɫ
	//BYTE		nSelectedAlpha;			// 
	DWORD		clrHotText;				// ������ͣʱ���ı�ɫ
	DWORD		clrHotBk;				//		�ı���ɫ
	//BYTE		nHotAlpha;				// 
	DWORD		clrCutedText;			// ���ڼ���ʱ���ı�ɫ
	DWORD		clrCutedBk;				//		�ı���ɫ
	//BYTE		nCutedAlpha;			// 
	DWORD		clrDisabledText;		// ������Ч�����ʱ���ı�ɫ
	DWORD		clrDisabledBk;			//		�ı���ɫ
	//BYTE		nDisabledAlpha;			// 
	DWORD		clrLine;				// �����ߵ���ɫ��=0ʱ���û���������
	bool		bShowHtml;				// 
	bool		bMultiExpandable;		// 
	gtc::image		*pImage;			// ͼƬ
	ImageCellType	mapImage;			// key=BEImageState value=RECT����m_pImage�е�λ��

}BSListItemInfo;
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-29	�б�ͷ�ĵ�Ԫ����Ϣ
typedef struct BSHeaderItemInfo {
	UINT		uiTextStyle;
	bool		bShadowText;			// ���ƴ�����Ӱ���ı�
	int			iFont;					// 
	RECT		rcTextPadding;			// �ı�����
	DWORD		clrText;				// ������ʾʱ���ı�ɫ
	DWORD		clrBk;					//		�ı���ɫ
	DWORD		clrHotText;				// ������ͣʱ���ı�ɫ
	DWORD		clrHotBk;				//		�ı���ɫ
	DWORD		clrPushedText;			// ������ͣʱ���ı�ɫ
	DWORD		clrPushedBk;			//		�ı���ɫ

	gtc::image		*pImage;			// ͼƬ
	ImageCellType	mapImage;			// key=BEImageState value=RECT����m_pImage�е�λ��
}BSHeaderItemInfo;

class listBodyUC;
class listHeaderUC;
class listUC;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IFolderViewCallbackUC	��	2015-1-27
//	�ڵ����ݻص���������ࣻlistUC��ʵ����ʵ������ӿ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class IListCallbackUC
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-27	�õ��ڵ�ͼ������Ƶȵ�Ԫ����ϢpControl = listElementUC��������
	virtual bool getItemData(controlUC *pControl, int iItem, int iSubItem) = 0;
	//	���ͻ�����
	virtual bool drawItem(HDC hdc, RECT rcNode, int iItem, int iSubItem, controlUC *pControl) = 0;
	//	�õ�����������Ĭ�ϣ������ң����ϵ�������
	virtual int getSubItemIndex(int iItem, int iSubItem) = 0;
	virtual int getSubItemIndex(POINT pt) = 0;
	//	2015-2-12	�õ����ӿڹ�ͬӵ�еĿؼ�
	virtual controlUC* getSelf() = 0;
	//	2015-3-2  �б�������С�����ı�
	virtual void scrollRangeChanged(SIZE szOld, SIZE szNew) {}
	virtual void scrollPosChanged(SIZE szOld, SIZE szNew) {}
	virtual int getScrollLineStep()=0;// {	return 0;	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-27	listUC�ĸ�����֮һ
class IListOwnerUC
{
public:
	virtual BSListItemInfo* getListInfo() = 0;
	virtual int getCurSel() const = 0;
	virtual bool selectItem(int iIndex) = 0;
	//virtual void doEvent(BSEventUC& event) = 0;
	virtual void Event(BSEventUC& event) = 0;
};
class IListUC : public IListOwnerUC
{
public:
	virtual listHeaderUC* getHeader() const = 0;
	virtual containerUC* getList() const = 0;
	virtual IListCallbackUC* getItemCallback() const = 0;
	virtual void setItemCallback(IListCallbackUC* pCallback) = 0;
	virtual bool expandItem(int iIndex, bool bExpand = true) = 0;
	virtual int getExpandedItem() const = 0;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-27	
class IListItemUC
{
public:
	virtual int getIndex() const = 0;
	virtual void setIndex(int iIndex) = 0;
	virtual IListOwnerUC* getOwner() = 0;
	virtual void setOwner(controlUC* pOwner) = 0;
	virtual bool isSelected() const = 0;
	virtual bool select(bool bSelect = true) = 0;
	virtual bool isCuted() const = 0;
	virtual bool cut(bool bCut = true) = 0;
	virtual bool isHot() const = 0;
	virtual bool hot(bool bHot = true) = 0;
	virtual bool isExpanded() const = 0;
	virtual bool expand(bool bExpand = true) = 0;
	virtual void drawItem(HDC hdc, const RECT& rcItem) = 0;
	virtual void drawItemBk(HDC hdc, const RECT& rcItem) = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listElementUC	��	2015-1-27
//	�������һ�������ݵ�α���ϣ�ÿ����������drawItem�����ֳ���
//	�����ڵ�Ԫ�أ�������Ĭ����������BSListItemInfo��������������listHeaderItemUC
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listElementUC : public controlUC, public IListItemUC
{
public:
	listElementUC(void);
	~listElementUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void Event(BSEventUC &event);
	virtual void paint(HDC hdc, const RECT &rcPaint);

	virtual void setVisible(bool bVisible = true);
	virtual void setEnabled(bool bEnable = true);
	virtual void invalidate();
	virtual bool activate();
	// �����б���
	//virtual void drawBk(HDC hdc, const RECT& rcItem);
public:
	RECT getSubItemPosition(int iSubItem);
public:
	// IListItemUC
	virtual int getIndex() const;
	virtual void setIndex(int iIndex);
	virtual IListOwnerUC* getOwner();
	virtual void setOwner(controlUC* pOwner);
	virtual bool isSelected() const;
	virtual bool select(bool bSelect = true);
	virtual bool isCuted() const;
	virtual bool cut(bool bCut = true);
	virtual bool isHot() const;
	virtual bool hot(bool bHot = true);
	virtual bool isExpanded() const;
	virtual bool expand(bool bExpand = true);
	//virtual void drawItem(HDC hdc, const RECT& rcItem) = 0;
	virtual void drawItemBk(HDC hdc, const RECT& rcItem);
protected:
	int				m_iIndex;		// ����Ϊ�к���
	bool			m_bSelected;
	UINT			m_uButtonState;
	IListOwnerUC	*m_pOwner;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listLabelElementUC  ��		2015-8-7
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listLabelElementUC : public listElementUC
{
public:
	listLabelElementUC();
	~listLabelElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void Event(BSEventUC &event);
	//virtual void paint(HDC hdc, const RECT &rcPaint);
	virtual SIZE estimateSize(SIZE szAvailable);
	// IListItemUC
	virtual void drawItem(HDC hdc, const RECT& rcItem);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listContainerElementUC  ��		2015-7-30
//	�����ӿؼ����б�Ԫ�ػ���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listContainerElementUC : public containerUC, public IListItemUC
{
public:
	listContainerElementUC();
	~listContainerElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void setVisible(bool bVisible = true);
	virtual void setEnabled(bool bEnable = true);
	virtual bool activate();
	virtual void invalidate();

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void Event(BSEventUC &event);
	virtual void paint(HDC hdc, const RECT &rcPaint);

public:
	// IListItemUC
	virtual int getIndex() const;
	virtual void setIndex(int iIndex);
	virtual IListOwnerUC* getOwner();
	virtual void setOwner(controlUC* pOwner);
	virtual bool isSelected() const;
	virtual bool select(bool bSelect = true);
	virtual bool isCuted() const;
	virtual bool cut(bool bCut = true);
	virtual bool isHot() const;
	virtual bool hot(bool bHot = true);
	virtual bool isExpanded() const;
	virtual bool expand(bool bExpand = true);
	virtual void drawItem(HDC hdc, const RECT& rcItem);
	virtual void drawItemBk(HDC hdc, const RECT& rcItem);



	//void DoPaint(HDC hDC, const RECT& rcPaint);

	//void DrawItemText(HDC hDC, const RECT& rcItem);    
	//void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
	int				m_iIndex;
	bool			m_bSelected;
	UINT			m_uiButtonState;
	IListOwnerUC	*m_pOwner;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listEmptyElementUC  ��		2015-8-3
//	Ϊ������Ч��ͼ����˿���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listEmptyElementUC : public listContainerElementUC
{
public:
	listEmptyElementUC();
	~listEmptyElementUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
public:
	// IListItemUC
	virtual void drawItem(HDC hdc, const RECT& rcItem);
	virtual void drawItemBk(HDC hdc, const RECT& rcItem);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listUC��	2015-1-22
//	�ļ��ڵ���ʾ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listUC : public horizontalLayoutUC, public IListUC
{
public:
	listUC(void);
	~listUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void setPosition(RECT pos);
	//virtual void Event(BSEventUC &event);
public:
	RECT getSubItemPosition(int iItem, int iSubItem);
public:
	// IListOwnerUC
	virtual BSListItemInfo* getListInfo();
	virtual int getCurSel() const;
	virtual bool selectItem(int iIndex);
	virtual void Event(BSEventUC& event);
	// IListUC
	virtual listHeaderUC* getHeader() const;
	virtual containerUC* getList() const;
	virtual IListCallbackUC* getItemCallback() const;
	virtual void setItemCallback(IListCallbackUC* pCallback);
	virtual bool expandItem(int iIndex, bool bExpand = true);
	virtual int getExpandedItem() const;
	// 
	bool getScrollSelect();
	void setScrollSelect(bool bScrollSelect);
	void setMultiExpanding(bool bMultiExpandable); 
	//virtual void doEvent(BSEventUC& event);
	// containerUC
	virtual int getChildPadding() const;
	virtual void setChildPadding(int iPadding);

	virtual controlUC* getItemAt(int iIndex) const;
	virtual int getItemIndex(controlUC* pControl) const ;
	virtual bool setItemIndex(controlUC* pControl, int iIndex);
	virtual int getCount() const;
	virtual bool add(controlUC* pControl);
	virtual bool addAt(controlUC* pControl, int iIndex);
	virtual bool remove(controlUC* pControl);
	virtual bool removeAt(int iIndex) ;
	virtual void removeAll();

	// 
	void ensureVisible(int iIndex);
	void scroll(int dx, int dy);
	// 
	virtual SIZE getScrollPos() const;
	virtual SIZE getScrollRange() const;
	virtual void setScrollPos(SIZE szPos);
	virtual void lineUp(int step = 0);
	virtual void lineDown(int step = 0);
	virtual void pageUp();
	virtual void pageDown();
	virtual void homeUp();
	virtual void endDown();
	virtual void lineLeft();
	virtual void lineRight();
	virtual void pageLeft();
	virtual void pageRight();
	virtual void homeLeft();
	virtual void endRight();
	//virtual void buildScrollBar(scrollBarUC *pScrollbarVertical, scrollBarUC *pScrollbarHorizontal = nullptr);
	virtual void enableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
	virtual scrollBarUC* getVerticalScrollBar() const;
	virtual scrollBarUC* getHorizontalScrollBar() const;

	// BSFolderViewInfo
	int getColumns() const;
	int getItemFont() const;
	void setItemFont(int iFont);
	DWORD getItemLineColor() const;
	void setItemLineColor(DWORD clrLine);
	bool isItemShowHtml() const;
	void setItemShowHtml(bool bShowHtml = true);
	bool isAlternateBk() const;
	void setAlternateBk(bool bAlternateBk);
	UINT getItemTextStyle() const;
	void setItemTextStyle(UINT style);
	RECT getItemTextPadding() const;
	void setItemTextPadding(RECT rcPadding);
	DWORD getItemTextColor() const;
	void setItemTextColor(DWORD clrText);
	DWORD getItemBkColor() const;
	void setItemBkColor(DWORD clrBk);
	DWORD getItemSelectedTextColor() const;
	void setItemSelectedTextColor(DWORD clrText);
	DWORD getItemSelectedBkColor() const;
	void setItemSelectedBkColor(DWORD clrBk);
	DWORD getItemHotTextColor() const;
	void setItemHotTextColor(DWORD clrText);
	DWORD getItemHotBkColor() const;
	void setItemHotBkColor(DWORD clrBk);
	DWORD getItemCutTextColor() const;
	void setItemCutTextColor(DWORD clrText);
	DWORD getItemCutBkColor() const;
	void setItemCutBkColor(DWORD clrBk);
	DWORD getItemDisabledTextColor() const;
	void setItemDisabledTextColor(DWORD clrText);
	DWORD getItemDisabledBkColor() const;
	void setItemDisabledBkColor(DWORD clrBk);

	gtc::image* getItemImage() const;	// ��ͼƬ
	void setItemImage(PCWSTR pwsImage);
	void setItemNormalImage(RECT rcImgCell);
	bool getItemNormalImage(RECT &rc) const;
	void setItemSelectedImage(RECT rcImgCell);
	bool getItemSelectedImage(RECT &rc) const;
	void setItemCutedImage(RECT rcImgCell);
	bool getItemCutedImage(RECT &rc) const;
	void setItemHotImage(RECT rcImgCell);
	bool getItemHotImage(RECT &rc) const;
	void setItemDisabledImage(RECT rcImgCell);
	bool getItemDisabledImage(RECT &rc) const;
protected:
	int 				m_iCurSel;			// ��ǰѡ�нڵ�����
	int					m_iExpandedItem;	// 
	bool				m_bScrollSelect;	// �������Ƿ�ѡ��
	BSListItemInfo		m_bsInfo;			// �ڵ���Ϣ
	listBodyUC			*m_pList;			// ��Žڵ�����
	listHeaderUC		*m_pHeader;			// �б�ͷ
	IListCallbackUC		*m_pCallback;		// �õ��ڵ����ݵĻص��ӿ�
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listBodyUC ��	2015-1-27
//	��Žڵ��������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listBodyUC : public verticalLayoutUC
{
private:
	listBodyUC();
public:
	explicit listBodyUC(listUC *pOwner);
	~listBodyUC();

	virtual void Event(BSEventUC &event);
	virtual void setPosition(RECT pos);
	virtual void setScrollPos(SIZE szPos);

	listUC* getOwner() const;
protected:
	listUC	*m_pOwner;
};
inline listUC* listBodyUC::getOwner() const
{
	return m_pOwner;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listHeaderUC	��	2015-1-27
//	�б�ؼ���ͷ���������е�����Ӱ��listHeaderItemUC��Ԫ��
//	���Բ��ɼ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listHeaderUC : public horizontalLayoutUC
{
public:
	listHeaderUC();
	~listHeaderUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual SIZE estimateSize(SIZE szAvailable);
public:
	bool isDragable() const;
	void setDragable(bool bDragable);
	gtc::image* getSliderImage();		// �ı��п�Ļ���
	void setSliderImage(PCWSTR pImage);

	BSHeaderItemInfo* getHeaderInfo();

	void setItemTextStyle(UINT uStyle);
	UINT getItemTextStyle() const;
	void setItemTextColor(DWORD dwTextColor);
	DWORD getItemTextColor() const;
	void setItemBkColor(DWORD clrBk);
	DWORD getItemBkColor() const;
	void setItemFont(int index);
	int getItemFont() const;
	bool isItemShadowText() const;
	void setItemShadowText(bool bShadow = true);
	void setItemTextPadding(RECT rc);
	RECT getItemTextPadding() const;

	gtc::image* getItemImage() const;
	void setItemImage(PCWSTR pImage);
	bool getItemNormalImage(RECT &rc);
	void setItemNormalImage(RECT rcImgCell);
	bool getItemHotImage(RECT &rc);
	void setItemHotImage(RECT rcImgCell);
	bool getItemPushedImage(RECT &rc);
	void setItemPushedImage(RECT rcImgCell);
protected:
	BSHeaderItemInfo	m_bsInfo;
	//DWORD	m_clrText;
	////DWORD	m_clrBk;	// controlUC
	//UINT	m_uiTextStyle;
	//int		m_iFont;
	//bool	m_bShadowText;	// ���ƴ�����Ӱ���ı�
	//RECT	m_rcTextPadding;

	bool	m_bDragable;
	gtc::image *m_pimgSlider;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	listHeaderItemUC	��	2015-1-27
//	�б�ؼ���ͷԪ�ص�Ԫ�񣻱�����Ӱ�����Ӧ�е�listElementUC��������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class listHeaderItemUC : public controlUC
{
public:
	listHeaderItemUC();
	~listHeaderItemUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual void Event(BSEventUC &event);
	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void paintStatusImage(HDC hdc);
	virtual void paintText(HDC hdc);
public:
	listHeaderUC* getOwner() const;
	void setOwner(listHeaderUC *pHeader);
	int getSepWidth() const;
	void setSepWidth(int iWidth);

	RECT getThumbRect() const;
protected:
	listHeaderUC	*m_pOwner;

	POINT			m_ptLastMouse;
	UINT			m_uiButtonState;
	int				m_iSepWidth;		// �ָ�����
};


}