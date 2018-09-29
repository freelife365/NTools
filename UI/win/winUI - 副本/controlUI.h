#pragma once
namespace wui {

/////////////////////////////////////////////////////////////////////////////////////
//

typedef controlUI* (CALLBACK* FINDCONTROLPROC)(controlUI*, LPVOID);

class WINUI_API controlUI
{
public:
	controlUI();
	virtual ~controlUI();

public:
	virtual stringex getName() const;
	virtual void setName(PCWSTR pstrName);
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual bool activate();
	virtual paintManagerUI* getManager() const;
	virtual void setManager(paintManagerUI* pManager, controlUI* pParent, bool bInit = true);
	virtual controlUI* getParent() const;

	// �ı����
	virtual stringex getText() const;
	virtual void setText(PCWSTR pstrText);

	// ͼ�����
	DWORD getBkColor() const;
	void setBkColor(DWORD dwBackColor);
	DWORD getBkColor2() const;
	void setBkColor2(DWORD dwBackColor);
	DWORD getBkColor3() const;
	void setBkColor3(DWORD dwBackColor);
	PCWSTR getBkImage();
	void setBkImage(PCWSTR pStrImage);
	DWORD getBorderColor() const;
	void setBorderColor(DWORD dwBorderColor);
	DWORD getFocusBorderColor() const;
	void setFocusBorderColor(DWORD dwBorderColor);
	bool isColorHSL() const;
	void setColorHSL(bool bColorHSL);
	int getBorderSize() const;
	void setBorderSize(int nSize);
	SIZE getBorderRound() const;
	void setBorderRound(SIZE cxyRound);
	bool drawImage(HDC hDC, PCWSTR pStrImage, PCWSTR pStrModify = NULL);

	// λ�����
	virtual const RECT& getPos() const;
	virtual void setPos(RECT rc);
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual int getX() const;
	virtual int getY() const;
	virtual RECT getPadding() const;
	virtual void setPadding(RECT rcPadding); // ������߾࣬���ϲ㴰�ڻ���
	virtual SIZE getFixedXY() const;         // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedXY(SIZE szXY);      // ��floatΪtrueʱ��Ч
	virtual int getFixedWidth() const;       // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedWidth(int cx);      // Ԥ��Ĳο�ֵ
	virtual int getFixedHeight() const;      // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedHeight(int cy);     // Ԥ��Ĳο�ֵ
	virtual int getMinWidth() const;
	virtual void setMinWidth(int cx);
	virtual int getMaxWidth() const;
	virtual void setMaxWidth(int cx);
	virtual int getMinHeight() const;
	virtual void setMinHeight(int cy);
	virtual int getMaxHeight() const;
	virtual void setMaxHeight(int cy);
	virtual void setRelativePos(SIZE szMove,SIZE szZoom);
	virtual void setRelativeParentSize(SIZE sz);
	virtual TRelativePosUI getRelativePos() const;
	virtual bool isRelativePos() const;

	// �����ʾ
	virtual stringex getToolTip() const;
	virtual void setToolTip(PCWSTR pstrText);

	// ��ݼ�
	virtual TCHAR getShortcut() const;
	virtual void setShortcut(TCHAR ch);

	// �˵�
	virtual bool isContextMenuUsed() const;
	virtual void setContextMenuUsed(bool bMenuUsed);

	// �û�����
	virtual const stringex& getUserData(); // �������������û�ʹ��
	virtual void setUserData(PCWSTR pstrText); // �������������û�ʹ��
	virtual UINT_PTR getTag() const; // �������������û�ʹ��
	virtual void setTag(UINT_PTR pTag); // �������������û�ʹ��

	// һЩ��Ҫ������
	virtual bool isVisible() const;
	virtual void setVisible(bool bVisible = true);
	virtual void setInternVisible(bool bVisible = true); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���
	virtual bool isEnabled() const;
	virtual void setEnabled(bool bEnable = true);
	virtual bool isMouseEnabled() const;
	virtual void setMouseEnabled(bool bEnable = true);
	virtual bool isFocused() const;
	virtual void setFocus();
	virtual bool isFloat() const;
	virtual void setFloat(bool bFloat = true);

	virtual controlUI* findControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	void invalidate();
	bool isUpdateNeeded() const;
	void needUpdate();
	void needParentUpdate();
	DWORD getAdjustColor(DWORD dwColor);

	virtual void init();
	virtual void doInit();

	virtual void Event(TEventUI& event);
	virtual void doEvent(TEventUI& event);

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	controlUI* applyAttributeList(PCWSTR pstrList);

	virtual SIZE estimateSize(SIZE szAvailable);

	virtual void doPaint(HDC hDC, const RECT& rcPaint);
	virtual void paintBkColor(HDC hDC);
	virtual void paintBkImage(HDC hDC);
	virtual void paintStatusImage(HDC hDC);
	virtual void paintText(HDC hDC);
	virtual void paintBorder(HDC hDC);

	virtual void doPostPaint(HDC hDC, const RECT& rcPaint);

public:
	eventSource onInit;
	eventSource onDestroy;
	eventSource onSize;
	eventSource onEvent;
	eventSource onNotify;

protected:
	paintManagerUI* m_pManager;
	controlUI* m_pParent;
	stringex m_sName;
	bool m_bUpdateNeeded;
	bool m_bMenuUsed;
	RECT m_rcItem;
	RECT m_rcPadding;
	SIZE m_cXY;
	SIZE m_cxyFixed;
	SIZE m_cxyMin;
	SIZE m_cxyMax;
	bool m_bVisible;
	bool m_bInternVisible;
	bool m_bEnabled;
	bool m_bMouseEnabled;
	bool m_bFocused;
	bool m_bFloat;
	bool m_bSetPos; // ��ֹSetPosѭ������
	TRelativePosUI m_tRelativePos;

	stringex m_sText;
	stringex m_sToolTip;
	wchar_t m_chShortcut;
	stringex m_sUserData;
	UINT_PTR m_pTag;

	DWORD m_dwBackColor;
	DWORD m_dwBackColor2;
	DWORD m_dwBackColor3;
	stringex m_sBkImage;
	DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
	bool m_bColorHSL;
	int m_nBorderSize;
	SIZE m_cxyBorderRound;
	RECT m_rcPaint;
};
}