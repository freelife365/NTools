#pragma once
#include "wucdef.h"
#include "drawManager.h"
namespace wuc{

typedef controlUC* (CALLBACK* FINDCONTROLPROC)(controlUC*, LPVOID);

typedef std::map<UINT, RECT>	ImageCellType;	//	key:UCIMAGE_STATE_xxx value:ͼƬ�еĲ�������λ������

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	controlUC ��	2015-1-23
//	�ؼ�����
//	ͼƬ��ʾ��ʽ����ͬ״̬ͼƬ�ϳ�һ�Ų�����m_pImage�У�������ʾ���ִ���m_mapImage��
//				����һ���Ÿ�Ķ�λ����Ҫһ����m_rcPatch
////////////////////////////////////////////////////////////////////////////////////////////////////////
class controlUC
{
public:
	controlUC(void);
	virtual ~controlUC(void);
public:
	virtual const std::wstring& getName() const;
	virtual void setName(PCWSTR pstrName);
	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);
	virtual UINT getControlFlags() const;
	virtual void Event(BSEventUC &event);

	virtual void paint(HDC hdc, const RECT &rcPaint);
	virtual void paintBkColor(HDC hdc);
	virtual void paintBkImage(HDC hdc);
	virtual void paintStatusImage(HDC hdc);
	virtual void paintText(HDC hdc);
	virtual void paintBorder(HDC hdc);

	virtual bool activate();
	virtual drawManager* getManager();
	virtual void onInit();
	virtual void setManager(drawManager *pManager, controlUC *pParent, bool bInit = true);
	virtual controlUC* getParent() const;
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	controlUC* applyAttributeList(PCWSTR pstrList);
	virtual controlUC* findControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	virtual bool existsControl(controlUC *pControl);
	// �ı����
	virtual PCWSTR getText() const;
	virtual void setText(PCWSTR pstrText);
	// 
	DWORD getBkColor() const;
	void setBkColor(DWORD dwBackColor);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-23	
	//void setImagePatch(SIZE szPatch);
	//void setImagePatch(int cx, int cy);
	//SIZE getImagePatch() const;
	void setImage(PCWSTR pImage, bool bRefImg = true);		// ͼ�����������ñ�����[from��to]
	void setImage(gtc::image *pImage, bool bRefImg = true);
	void clearImage();
	gtc::image* getImage() const;
	bool getBkImage(RECT &rc) const;
	void setBkImage(RECT rcImgCell);			// rcImgCell������getImage��ĳһ����
	void setBkImage(LONG left, LONG top, LONG right, LONG bottom);
	bool existsImage(UINT keyFlag, RECT *prcCell = NULL) const;	// �鿴ָ��key��ͼƬ�Ƿ����
	DWORD getBorderColor() const;
	void setBorderColor(DWORD dwBorderColor);
	DWORD getFocusBorderColor() const;
	void setFocusBorderColor(DWORD dwBorderColor);
	int getBorderSize() const;
	void setBorderSize(int nSize);
	SIZE getBorderRound() const;
	void setBorderRound(SIZE cxyRound);
	int getFont() const;
	void setFont(int fontIndex);
	// 
	virtual SIZE estimateSize(SIZE szAvailable);
	virtual SIZE autoCalculateSize();
	virtual RECT getPadding() const;
	virtual void setPadding(RECT rcPadding);	// ������߾࣬���ϲ㴰�ڻ���
	virtual void setPadding(LONG left, LONG top, LONG right, LONG bottom);	// ������߾࣬���ϲ㴰�ڻ���
	virtual SIZE getFixedXY() const;			// ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedXY(SIZE szXY);			// ��floatΪtrueʱ��Ч
	virtual void setFixedXY(int fixedX, int fixedY);
	virtual void setFixedSize(SIZE sz);
	virtual void setFixedSize(LONG cx, LONG cy);
	virtual int getFixedWidth() const;			// ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedWidth(int cx);			// Ԥ��Ĳο�ֵ
	virtual int getFixedHeight() const;			// ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
	virtual void setFixedHeight(int cy);		// Ԥ��Ĳο�ֵ
	virtual int getMinWidth() const;
	virtual void setMinWidth(int cx);
	virtual int getMaxWidth() const;
	virtual void setMaxWidth(int cx);
	virtual int getMinHeight() const;
	virtual void setMinHeight(int cy);
	virtual int getMaxHeight() const;
	virtual void setMaxHeight(int cy);
	// һЩ��Ҫ������
	virtual bool isVisible() const;
	virtual void setVisible(bool bVisible = true);
	virtual void setInternVisible(bool bVisible = true); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���
	virtual bool isEnabled() const;
	virtual void setEnabled(bool bEnable = true);
	virtual bool isMouseEnabled() const;
	virtual void setMouseEnabled(bool bEnable = true);
	virtual bool isFocused() const;
	virtual void setFocus(bool bFocus = true);
	virtual bool isFloat() const;
	virtual void setFloat(bool bFloat = true);
	virtual void setRelativePos(SIZE szMove,SIZE szZoom);
	virtual void setRelativeParentSize(SIZE sz);
	virtual BSRelativePosUC getRelativePos() const;
	virtual bool isRelativePos() const;

	virtual void invalidate();
	bool isUpdateNeeded() const;
	void needUpdate();
	void needParentUpdate();
	// �ͻ�����
	RECT getPosition() const;
	virtual void setPosition(RECT pos);
	/*****************************************************************************************
		<< ---  controlUC::ptIn		2014-06-24 --- >>
		˵�����鿴���Ƿ��ڿؼ���
		������
		pt = �ο��㣨�ͻ����꣩
		����ֵ���Ƿ���
	*****************************************************************************************/
	bool ptIn(const POINT &pt);

	// �û�����
	virtual const std::wstring& getUserData(); // �������������û�ʹ��
	virtual void setUserData(PCWSTR pstrText); // �������������û�ʹ��
	virtual UINT_PTR getTag() const; // �������������û�ʹ��
	virtual void setTag(UINT_PTR pTag); // �������������û�ʹ��

	// �˵�
	virtual bool isContextMenuUsed() const;
	virtual void setContextMenuUsed(bool bMenuUsed);
	// �����ʾ
	virtual std::wstring getToolTip() const;
	virtual void setToolTip(PCWSTR pstrText);
	// ��ݼ�
	virtual wchar_t getShortcut() const;
	virtual void setShortcut(wchar_t ch);
public:
	// ����ͼ��
	static gtc::image* analyzeImage(PCWSTR pstrImage);
	static bool existsImage(const ImageCellType &mapImages, UINT keyFlag, RECT *prcCell = NULL);	// �鿴ָ��key��ͼƬ�Ƿ����
	static bool setCellImage(ImageCellType &mapImages, UINT keyFlag, RECT rcCell, gtc::image *pImage = NULL);

protected:
	// �趨ͼ�����m_pImage�е�λ�ã�keyFlag=ͼ������
	bool setCellImage(UINT keyFlag, RECT rcCell);
	///*****************************************************************************************
	//	<< ---  controlUC::drawImage		2014-07-31 --- >>
	//	˵������ͼƬĳһ������Ƶ�Ŀ��λ��
	//	������
	//	grap	= ��ͼ����
	//	pImage	= ͼ��
	//	rcDest	= Ŀ��λ��
	//	prcSrc	= ͼ��Դ������Ϊ��ʱָͼ���ĳһ���򣬵����ܳ���ͼ���С
	//	����ֵ��ִ���Ƿ�ɹ�
	//*****************************************************************************************/
	//static bool drawImage(Gdiplus::Graphics &grap, gtc::image *pImage, const RECT &rcDest, RECT *prcSrc = nullptr);
protected:
	ImageCellType	m_mapImage;			// key=BEImageState value=RECT����m_pImage�е�λ�� // �������ֽڶ������⣬Ӱ���˵�����ʹ�ã����Է��ڶ���
	drawManager		*m_pManager;		// �ؼ���������
	std::wstring	m_wsName;			// �ؼ����ƣ�ͬһ�����ڵĿؼ����Ʊ���Ψһ
	controlUC		*m_pParent;			// �ؼ��ĸ��ؼ�

	RECT			m_rcItem;			// �ؼ���m_pWnd�ڵ��߼�����
	RECT			m_rcPadding;		// ��߾࣬���ϲ㴰�ڻ���
	SIZE			m_szXY;				// �ؼ���Ը�����λ�� >= 0�����ߡ��ϱ߾��룬< 0����ұߡ��±߾���
	SIZE			m_szxyFixed;		// �ؼ�Ԥ��ĸ߶ȡ����
	SIZE			m_szxyMin;			// �ؼ���С�ߴ�
	SIZE			m_szxyMax;			// �ؼ����ߴ�

	std::wstring	m_wsText;			// �ؼ�����
	DWORD			m_dwBackColor;		// 
	//std::wstring	m_wsBkImage;		// ����ͼkey����
	
	//SIZE			m_szPatch;			// ���þŹ������ʱ�Ľǳߴ磬�����˻���ʱ�˿�cx��˸�cy
	gtc::image		*m_pImage;			// ͼƬ������������ñ�����[from��to]
	bool			m_brefImage;		// m_pImage�Ƿ�������Դ���еĶ��󣬷��Լ���Ҫ�ͷţ�gdi+�����ObjectBusy״̬����
	int				m_iFont;			// ��������������0��-1ʱĬ������

	DWORD			m_dwBorderColor;
	DWORD			m_dwFocusBorderColor;
	bool			m_bVisible;			// 
	bool			m_bInternVisible;	// �ҵ��������Щ�����ؼ��еĲ��ֿؼ��Ƿ�ɼ�
	bool			m_bFocused;			// 
	bool			m_bEnabled;			// 
	bool			m_bMouseEnabled;
	bool			m_bUpdateNeeded;	// �Ƿ���Ҫ���»���
	bool			m_bMenuUsed;		// ֧�ֵ���ʽ�˵�		
	bool			m_bFloat;
	BSRelativePosUC m_bsRelativePos;

	std::wstring	m_wsUserData;
	UINT_PTR		m_pTag;
	std::wstring	m_wsToolTip;
	wchar_t			m_chShortcut;

	int				m_iBorderSize;		// �߿��С����(1)
	SIZE			m_szxyBorderRound;	// �߿�Բ�ǰ뾶,��(2,2)

	RECT			m_rcPaint;
};

}