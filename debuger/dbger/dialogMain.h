#pragma once
class dialogMain : public wuc::windowWnd, public wuc::INotifyUC, public gtc::singleton<dialogMain>
{
public:
	dialogMain(void);
	~dialogMain(void);

	// gtc::singleton
	static dialogMain& getSingleton();
	static dialogMain* getSingletonPtr();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	������ʽ	2015-1-19
	virtual PCWSTR getWindowClassName() const {		return DBGER_WND_MAIN_CLASS;					}
	virtual PCWSTR getWindowCaption() const {		return DBGER_WND_MAIN_CAPTION;					}
	virtual UINT getClassStyle() const {			return CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;		}
	virtual UINT getWindowStyle() const {			return WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;}
	virtual UINT getWindowExStyle() const {			return WS_EX_TOPMOST|WS_EX_TOOLWINDOW;			}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	2015-1-20	���WUC::handleMessage����bHandled�������������ж��û��Ƿ�����˶���Ϣ����
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	virtual void onFinalMessage(HWND hWnd);
	virtual void notify(wuc::BSNotifyUC& msg);
public:
	LRESULT onCreate();
	LRESULT onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void onInit();
	LRESULT onMessageTray(WPARAM wParam, LPARAM lParam);
	// �˵�����
	void onMenuCommand(WPARAM wParam, LPARAM lParam);
	void onClickMinbox();
	void onClickClose();
	void onClickSave();
	void onClickClear();

public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-24 ����������
	bool create();
	// ������ͼ��
	bool createTray();
	void destroyTray();
	void modifyTray();
	// ���һ����־
	void addLog(gtc::BELogType logType, PCWSTR pInfo);
public:
	void setCaption(PCWSTR pTitle);
	void setStateText(PCWSTR pState);
protected:
	wuc::drawManager		m_drawManager;

	wuc::horizontalLayoutUC	*m_pTitleBar;		// ������
	wuc::horizontalLayoutUC *m_pToolBar;		// ������
	wuc::horizontalLayoutUC	*m_pClientUC;		// �ͻ���
	wuc::horizontalLayoutUC	*m_pTaskBar;		// ��������״̬��

	wuc::labelUC			*m_labelCaption;	// ���ڱ����ı�
	wuc::labelUC			*m_labelState;		// ״̬�ı�
	wuc::buttonUC			*m_buttonMinBox;	// ��С��
	wuc::buttonUC			*m_buttonClose;		// �ر�
	wuc::buttonUC			*m_buttonClear;		// ���
	wuc::buttonUC			*m_buttonSave;		// ����
	wuc::wkeWebkitUC		*m_web;				// 
	// ������ͼ��ṹ����
	NOTIFYICONDATA			m_nid;

};

