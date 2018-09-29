#pragma once

#include "wucdef.h"
#include "wucControl.h"
#include "wucContainer.h"
namespace wuc
{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	labelUC��	2014-06-23
//	��ǩ���޷��õ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////
class labelUC : public controlUC
{
public:
	labelUC(void);
	virtual ~labelUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void Event(BSEventUC &event);
	virtual void paintText(HDC hdc);
	virtual SIZE estimateSize(SIZE szAvailable);
	virtual SIZE autoCalculateSize();
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	gtc::image* getTextImage() const;
	void setTextImage(PCWSTR pImage);
	void setTextImage(gtc::image *pImage);
	SIZE getTextImageOffsetXY() const;
	void setTextImageOffsetXY(SIZE szXY);
	void setTextImageOffsetXY(LONG x, LONG y);

protected:
	void drawTextImage(HDC hdc, RECT rcPaint);
public:
	void setTextStyle(UINT uStyle);
	UINT getTextStyle() const;
	void setTextColor(DWORD dwTextColor);
	DWORD getTextColor() const;
	void setDisabledTextColor(DWORD dwTextColor);
	DWORD getDisabledTextColor() const;
	void setFont(int index);
	int getFont() const;
	RECT getTextPadding() const;
	void setTextPadding(RECT rc);
	void setTextPadding(LONG left, LONG top, LONG right, LONG bottom);
	bool isShowHtml();
	void setShowHtml(bool bShowHtml = true);
	bool isShadowText() const;
	void setShadowText(bool bShadow = true);
protected:
	DWORD		m_dwTextColor;
	DWORD		m_dwDisabledTextColor;
	int			m_iFont;
	UINT		m_uiTextStyle;
	RECT		m_rcTextPadding;
	bool		m_bShowHtml;
	bool		m_bShadowText;			// ���ƴ�����Ӱ���ı�

	gtc::image	*m_pimgText;			// ���ֲ���ͼƬ
	SIZE		m_szOffsetXY;			// ����ͼƬ��ԭ�л�������ƫ�ƣ� >0��������ƫ�� <0��������ƫ��
};

/////////////////////////////////////////////////////////////////////////////////////
// buttonUC

class buttonUC : public labelUC
{
public:
	// ��ťͼ������ı���λ��
	typedef enum {
		iconBeLeft,
		iconBeRight,
		iconBeTop,
		iconBeBottom
	}_BEIconPosition;
public:
	buttonUC();
	virtual ~buttonUC();
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;

	virtual bool activate();
	virtual void setEnabled(bool bEnable = true);
	virtual void Event(BSEventUC& event);

	virtual SIZE estimateSize(SIZE szAvailable);
	virtual SIZE autoCalculateSize();

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	virtual void paintText(HDC hdc);
	virtual void paintStatusImage(HDC hdc);

	bool isCheckBox() const;
	void setCheckBox(bool bCheckbox = true);
	bool isSelected() const;
	virtual void selected(bool bSelected = true);
	bool switchSelected();	// �л�ѡ��״̬���������л����״̬
	//PCWSTR getSelectedImage();
	//void setSelectedImage(PCWSTR pimgKeyName);

	//PCWSTR getImage();
	//void setImage(PCWSTR pimgKeyName);

	bool getNormalImage(RECT &rc);
	void setNormalImage(RECT rcImgCell);
	void setNormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotImage(RECT &rc);
	void setHotImage(RECT rcImgCell);
	void setHotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedImage(RECT &rc);
	void setPushedImage(RECT rcImgCell);
	void setPushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getFocusedImage(RECT &rc);
	void setFocusedImage(RECT rcImgCell);
	void setFocusedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledImage(RECT &rc);
	void setDisabledImage(RECT rcImgCell);
	void setDisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getNormalImageCheck(RECT &rc);
	void setNormalImageCheck(RECT rcImgCell);
	void setNormalImageCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotImageCheck(RECT &rc);
	void setHotImageCheck(RECT rcImgCell);
	void setHotImageCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedImageCheck(RECT &rc);
	void setPushedImageCheck(RECT rcImgCell);
	void setPushedImageCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getFocusedImageCheck(RECT &rc);
	void setFocusedImageCheck(RECT rcImgCell);
	void setFocusedImageCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledImageCheck(RECT &rc);
	void setDisabledImageCheck(RECT rcImgCell);
	void setDisabledImageCheck(LONG left, LONG top, LONG right, LONG bottom);

	void setHotTextColor(DWORD dwColor);
	DWORD getHotTextColor() const;
	void setPushedTextColor(DWORD dwColor);
	DWORD getPushedTextColor() const;
	void setFocusedTextColor(DWORD dwColor);
	DWORD getFocusedTextColor() const;

	void setTextColorCheck(DWORD dwColor);
	DWORD getTextColorCheck() const;
	void setHotTextColorCheck(DWORD dwColor);
	DWORD getHotTextColorCheck() const;
	void setPushedTextColorCheck(DWORD dwColor);
	DWORD getPushedTextColorCheck() const;
	void setFocusedTextColorCheck(DWORD dwColor);
	DWORD getFocusedTextColorCheck() const;
	void setDisabledTextColorCheck(DWORD dwColor);
	DWORD getDisabledTextColorCheck() const;

public:
	// icon
	void setIconFixedSize(int cxcy);
	void setIconFixedSize(SIZE sz);
	void setIconFixedSize(int cx, int cy);
	SIZE getIconFixedSize() const;
	void setIconSpace(int space);
	int getIconSpace() const;
	//void setIconFixedWidth(int width);
	//int getIconFixedWidth() const;
	RECT getIconArea(RECT *prcText = NULL) const;
	//RECT getIconArea(const RECT *prcSrc = NULL) const;
	void setIconPosition(_BEIconPosition pos);
	_BEIconPosition getIconPosition() const;
	//void setIconFront(bool bBefore);
	//bool isFrontIcon() const;
	void setIcon(PCWSTR pImage);		// ͼ�����������ñ�����[from��to]
	void setIcon(gtc::image *pImage);
	void clearIcon();
	gtc::image* getIcon() const;

	bool getNormalIcon(RECT &rc);
	void setNormalIcon(RECT rcImgCell);
	void setNormalIcon(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotIcon(RECT &rc);
	void setHotIcon(RECT rcImgCell);
	void setHotIcon(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedIcon(RECT &rc);
	void setPushedIcon(RECT rcImgCell);
	void setPushedIcon(LONG left, LONG top, LONG right, LONG bottom);
	bool getFocusedIcon(RECT &rc);
	void setFocusedIcon(RECT rcImgCell);
	void setFocusedIcon(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledIcon(RECT &rc);
	void setDisabledIcon(RECT rcImgCell);
	void setDisabledIcon(LONG left, LONG top, LONG right, LONG bottom);

	bool getNormalIconCheck(RECT &rc);
	void setNormalIconCheck(RECT rcImgCell);
	void setNormalIconCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getHotIconCheck(RECT &rc);
	void setHotIconCheck(RECT rcImgCell);
	void setHotIconCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getPushedIconCheck(RECT &rc);
	void setPushedIconCheck(RECT rcImgCell);
	void setPushedIconCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getFocusedIconCheck(RECT &rc);
	void setFocusedIconCheck(RECT rcImgCell);
	void setFocusedIconCheck(LONG left, LONG top, LONG right, LONG bottom);
	bool getDisabledIconCheck(RECT &rc);
	void setDisabledIconCheck(RECT rcImgCell);
	void setDisabledIconCheck(LONG left, LONG top, LONG right, LONG bottom);

protected:
	UINT	m_uButtonState;	// ��겶��״̬

	bool	m_bCheckBox;	// ��checkbox�ؼ�������ͨbutton��ť
	bool	m_bSelected;	// ѡ��״̬
	//std::wstring m_wsSelectedImage;

	DWORD	m_dwHotTextColor;
	DWORD	m_dwPushedTextColor;
	DWORD	m_dwFocusedTextColor;
	//DWORD	m_dwDisabledTextColor;		// labelUC���Ѿ�������

	DWORD	m_dwTextColorCheck;
	DWORD	m_dwHotTextColorCheck;
	DWORD	m_dwPushedTextColorCheck;
	DWORD	m_dwFocusedTextColorCheck;
	DWORD	m_dwDisabledTextColorCheck;

	gtc::image		*m_pimgIcon;		// ͼ�꼯��
	ImageCellType	m_mapIcon;			// key=BEImageState value=RECT����m_pImage�е�λ��
	//bool			m_bFrontIcon;		// ͼ�����ı�ǰ��true�����Ǻ�
	//int				m_iIconFixedWidth;	// ͼ����ʾ����Ŀ�ȣ���<=0ʱ��ȵ��ڰ�ť�߶�
	_BEIconPosition	m_beIconPosition;	// ͼ������ı���λ��
	SIZE			m_szIconFixed;		// ͼ��Ĵ�С
	int				m_iIconSpace;		// ͼ����߿��࣬�Լ�ͼ�����ı��ļ�࣬�磺m_beIconPosition=iconBeLeftʱ��ʾ����߿����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	optionUC  ��		2015-7-27
//	��ť����ֻ�ܵ�ѡ�İ�ť
////////////////////////////////////////////////////////////////////////////////////////////////////////
class optionUC : public buttonUC
{
public:
	optionUC();
	~optionUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void setManager(drawManager *pManager, controlUC *pParent, bool bInit = true);

	virtual bool activate();
	virtual void setEnabled(bool bEnable = true);

	bool getSelectedImage(RECT &rc);
	void setSelectedImage(RECT rcImgCell);
	void setSelectedImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getSelectedIcon(RECT &rc);
	void setSelectedIcon(RECT rcImgCell);
	void setSelectedIcon(LONG left, LONG top, LONG right, LONG bottom);

	void setSelectedTextColor(DWORD dwTextColor);
	DWORD getSelectedTextColor();

	//LPCTSTR GetForeImage();
	//void SetForeImage(LPCTSTR pStrImage);

	std::wstring getGroup() const;
	void setGroup(PCWSTR pstrGroupName = NULL);
	//bool isSelected() const;
	virtual void selected(bool bSelected = true);

	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	virtual void paintStatusImage(HDC hdc);
	//virtual void paintText(HDC hdc);

protected:
	//bool			m_bSelected;
	std::wstring	m_wsGroupName;

	DWORD			m_dwSelectedTextColor;

	//CStdString m_sSelectedImage;
	//CStdString m_sForeImage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	scrollBarUC��	2014-07-17
//	��������
//	�ҵ�һ�����:
//		button1����Ӧ���ϣ���ֱ��������������ˣ�ˮƽ�������������п��ޣ���=��=��ֱ���������ˮƽ�������ĸ�
//		button2����Ӧ���£���ֱ�������������Ҷˣ�ˮƽ�������������п���
//		thumb����Ӧ����ı���
//		rail����Ӧ�����ڵ�ͼ��
//		bk����Ӧ�����������ı���
////////////////////////////////////////////////////////////////////////////////////////////////////////
class scrollBarUC : public controlUC
{
public:
	scrollBarUC();
	~scrollBarUC();

	PCWSTR getClass() const;
	LPVOID getInterface(PCWSTR pstrName);

	containerUC* getOwner() const;
	void setOwner(containerUC* pOwner);

	void setEnabled(bool bEnable = true);
	void setFocus();

	bool isHorizontal();
	void setHorizontal(bool bHorizontal = true);
	int getScrollRange() const;
	void setScrollRange(int nRange);
	int getScrollPos() const;
	void setScrollPos(int nPos);
	int getLineSize() const;
	void setLineSize(int nSize);

	bool getShowButton1();
	void setShowButton1(bool bShow);
	bool getButton1NormalImage(RECT &rc);
	void setButton1NormalImage(RECT rcImgCell);
	void setButton1NormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton1HotImage(RECT &rc);
	void setButton1HotImage(RECT rcImgCell);
	void setButton1HotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton1PushedImage(RECT &rc);
	void setButton1PushedImage(RECT rcImgCell);
	void setButton1PushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton1DisabledImage(RECT &rc);
	void setButton1DisabledImage(RECT rcImgCell);
	void setButton1DisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getShowButton2();
	void setShowButton2(bool bShow);
	bool getButton2NormalImage(RECT &rc);
	void setButton2NormalImage(RECT rcImgCell);
	void setButton2NormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton2HotImage(RECT &rc);
	void setButton2HotImage(RECT rcImgCell);
	void setButton2HotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton2PushedImage(RECT &rc);
	void setButton2PushedImage(RECT rcImgCell);
	void setButton2PushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getButton2DisabledImage(RECT &rc);
	void setButton2DisabledImage(RECT rcImgCell);
	void setButton2DisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getThumbNormalImage(RECT &rc);
	void setThumbNormalImage(RECT rcImgCell);
	void setThumbNormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getThumbHotImage(RECT &rc);
	void setThumbHotImage(RECT rcImgCell);
	void setThumbHotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getThumbPushedImage(RECT &rc);
	void setThumbPushedImage(RECT rcImgCell);
	void setThumbPushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getThumbDisabledImage(RECT &rc);
	void setThumbDisabledImage(RECT rcImgCell);
	void setThumbDisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getRailNormalImage(RECT &rc);
	void setRailNormalImage(RECT rcImgCell);
	void setRailNormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getRailHotImage(RECT &rc);
	void setRailHotImage(RECT rcImgCell);
	void setRailHotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getRailPushedImage(RECT &rc);
	void setRailPushedImage(RECT rcImgCell);
	void setRailPushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getRailDisabledImage(RECT &rc);
	void setRailDisabledImage(RECT rcImgCell);
	void setRailDisabledImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getBkNormalImage(RECT &rc);
	void setBkNormalImage(RECT rcImgCell);
	void setBkNormalImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getBkHotImage(RECT &rc);
	void setBkHotImage(RECT rcImgCell);
	void setBkHotImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getBkPushedImage(RECT &rc);
	void setBkPushedImage(RECT rcImgCell);
	void setBkPushedImage(LONG left, LONG top, LONG right, LONG bottom);
	bool getBkDisabledImage(RECT &rc);
	void setBkDisabledImage(RECT rcImgCell);
	void setBkDisabledImage(LONG left, LONG top, LONG right, LONG bottom);
	/****************************************************************************************
		<< --- setPosition		2015-1-30 --- >>         
		˵��������λ���趨2��ť�ͻ����λ��
	*****************************************************************************************/
	void setPosition(RECT rc);
	void Event(BSEventUC& event);
	void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);

	void paint(HDC hDC, const RECT& rcPaint);

	void paintBk(HDC hDC);
	void paintButton1(HDC hDC);
	void paintButton2(HDC hDC);
	void paintThumb(HDC hDC);
	void paintRail(HDC hDC);

	virtual void setVisible(bool bVisible)
	{
		//SEND_LOG(L"NOTE setVisible obj:%08X visible: %d", this, bVisible);
		__super::setVisible(bVisible);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-23 ��ǰ�������Ƿ��ڹ�����	
	bool isScrolling();
protected:

	enum { 
		DEFAULT_SCROLLBAR_SIZE = 16,
		DEFAULT_TIMERID = 10,
	};

	bool m_bHorizontal;
	int m_iRange;
	int m_iScrollPos;
	int m_iLineSize;
	containerUC* m_pOwner;
	POINT ptLastMouse;
	int m_iLastScrollPos;
	int m_iLastScrollOffset;
	int m_iScrollRepeatDelay;


	bool m_bShowButton1;
	RECT m_rcButton1;
	UINT m_uiButton1State;

	bool m_bShowButton2;
	RECT m_rcButton2;
	UINT m_uiButton2State;

	RECT m_rcThumb;
	UINT m_uiThumbState;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	trackerUC  ��		2015-3-5
//	��ͨ�ĸ��ٿ��磺���ѡ��һ������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class trackerUC : public controlUC
{
public:
	trackerUC();
	~trackerUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual void paintBkImage(HDC hdc);
	//virtual void setPosition(RECT pos);
public:
	POINT getStartPoint() const;
	void setStartPoint(POINT pt);
	POINT getEndPoint() const;
	void setEndPoint(POINT pt);
protected:
	POINT	m_ptStart;
	POINT	m_ptEnd;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	progressUC	�� 2016-3-3	kp007
//	������
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class progressUC : public labelUC
{
public:
	progressUC();
	~progressUC();

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void paintStatusImage(HDC hdc);

public:
	bool getNormalImage(RECT &rc);
	void setNormalImage(RECT rcImgCell);
	void setNormalImage(LONG left, LONG top, LONG right, LONG bottom);

	bool getProgressImage(RECT &rc);
	void setProgressImage(RECT rcImgCell);
	void setProgressImage(LONG left, LONG top, LONG right, LONG bottom);


	void setHorizontal(bool bHorizontal = true);
	bool isHorizontal() const;
	void setMaxValue(int _max);
	int getMaxValue() const;
	void setMinValue(int _min);
	int getMinValue() const;
	void setValue(int val);
	int getValue() const;
private:
	bool	m_bHorizontal;		// ��������ˮƽ��ʾ���Ǵ�ֱ��ʾ
	int		m_iMax;
	int		m_iMin;
	int		m_iValue;

};


}	// end wuc