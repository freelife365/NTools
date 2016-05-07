#pragma once

#include <cwchar>

#include <windows.h>
#include <windowsx.h>

#include <commctrl.h>
#include <stddef.h>
//#include <richedit.h>
//#include <tchar.h>
#include <assert.h>

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include "AC/sysupport.h"
#include "AC/baseobj.h"
#include "AC/resourceManager.h"
#include "AC/image.h"
#include "AC/draw/drawer.h"
#include "AC/delegateBase.h"
namespace wuc {
// �Ի��������ť����
#define UC_DIALOG_BUTTON_OK			L"_buttonOk"				// ȷ��
#define UC_DIALOG_BUTTON_CANCEL		L"_buttonCancel"			// ȡ��
#define UC_DIALOG_BUTTON_CLOSE		L"_buttonClose"				// �رգ�һ��Ϊ��x����ť
#define UC_DIALOG_BUTTON_MIN		L"_buttonMin"				// ��С��

///////////////////////////////////////////////////////////////////////////////////////////////////
//	������ʽ	2015-1-19
#define UC_WNDSTYLE_CONTAINER  (0)
#define UC_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UC_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UC_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UC_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UC_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UC_CLASSSTYLE_CONTAINER  (0)
#define UC_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UC_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UC_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-20	��WTL��Ϣ���ݸ�WUC���Ա����Ԥ����
#define WTL_TO_WUC_MSG_HANDLE()				\
	if (true)														\
	{																\
		bHandled = FALSE;											\
		lResult = handleMessage(uMsg, wParam, lParam, bHandled);	\
		if(bHandled) return TRUE;									\
	}
//	2015-1-20	��ӦWM_NCDESTROY��Ϣ��Ĭ�ϴ�����
#define WTL_TO_WUC_MSG_FINAL()				\
	if (uMsg == WM_NCDESTROY)							\
	{													\
		onFinalMessage(hWnd);							\
		return TRUE;									\
	}
//	2015-1-20	��ӦWM_INITDIALOG��Ϣ��Ĭ�ϴ�����
#define WTL_TO_WUC_MSG_INITDIALOG()			\
	if (uMsg == WM_INITDIALOG)							\
	{													\
		return onInitDialog();							\
	}
//	2015-1-20	��ӦWM_DESTROY��Ϣ��Ĭ�ϴ�����
#define WTL_TO_WUC_MSG_DESTROY()			\
	if (uMsg == WM_DESTROY)								\
	{													\
		onDestroy();									\
		return TRUE;									\
	}
/////////////////////////////////////////////////////////////////////////////////////
// UC�ؼ��¼�
typedef enum BEUCEvent
{
	BEUCEventFirst = 1,
	BEUCEventKeyBegin,
	BEUCEventKeyDown,
	BEUCEventKeyUp,
	BEUCEventChar,
	BEUCEventSysKey,
	BEUCEventKeyEnd,
	BEUCEventMouseBegin,
	BEUCEventMouseMove,
	BEUCEventMouseLeave,
	BEUCEventMouseEnter,
	BEUCEventMouseHover,
	BEUCEventButtonDown,
	BEUCEventButtonUp,
	BEUCEventRButtonDown,
	BEUCEventDBLClick,
	BEUCEventContextMenu,
	BEUCEventScrollWheel,
	BEUCEventMouseEnd,
	BEUCEventKillFocus,
	BEUCEventSetFocus,
	BEUCEventWindowSize,
	BEUCEventSetCursor,
	BEUCEventTimer,
	BEUCEventNotify,
	BEUCEventCommand,
	BEUCEventLast,
}BEUCEvent;
static std::wstring toString(BEUCEvent eventFlag)
{
	switch(eventFlag)
	{
	case BEUCEventFirst: //= 1,
		return L"BEUCEventFirst";
		break;
	case BEUCEventKeyBegin:
		return L"BEUCEventKeyBegin";
		break;
	case BEUCEventKeyDown:
		return L"BEUCEventKeyDown";
		break;
	case BEUCEventKeyUp:
		return L"BEUCEventKeyUp";
		break;
	case BEUCEventChar:
		return L"BEUCEventChar";
		break;
	case BEUCEventSysKey:
		return L"BEUCEventSysKey";
		break;
	case BEUCEventKeyEnd:
		return L"BEUCEventKeyEnd";
		break;
	case BEUCEventMouseBegin:
		return L"BEUCEventMouseBegin";
		break;
	case BEUCEventMouseMove:
		return L"BEUCEventMouseMove";
		break;
	case BEUCEventMouseLeave:
		return L"BEUCEventMouseLeave";
		break;
	case BEUCEventMouseEnter:
		return L"BEUCEventMouseEnter";
		break;
	case BEUCEventMouseHover:
		return L"BEUCEventMouseHover";
		break;
	case BEUCEventButtonDown:
		return L"BEUCEventButtonDown";
		break;
	case BEUCEventButtonUp:
		return L"BEUCEventButtonUp";
		break;
	case BEUCEventRButtonDown:
		return L"BEUCEventRButtonDown";
		break;
	case BEUCEventDBLClick:
		return L"BEUCEventDBLClick";
		break;
	case BEUCEventContextMenu:
		return L"BEUCEventContextMenu";
		break;
	case BEUCEventScrollWheel:
		return L"BEUCEventScrollWheel";
		break;
	case BEUCEventMouseEnd:
		return L"BEUCEventMouseEnd";
		break;
	case BEUCEventKillFocus:
		return L"BEUCEventKillFocus";
		break;
	case BEUCEventSetFocus:
		return L"BEUCEventSetFocus";
		break;
	case BEUCEventWindowSize:
		return L"BEUCEventWindowSize";
		break;
	case BEUCEventSetCursor:
		return L"BEUCEventSetCursor";
		break;
	case BEUCEventTimer:
		return L"BEUCEventTimer";
		break;
	case BEUCEventNotify:
		return L"BEUCEventNotify";
		break;
	case BEUCEventCommand:
		return L"BEUCEventCommand";
		break;
	case BEUCEventLast:
		return L"BEUCEventLast";
		break;
	}
	return L"";
}
// �Ի�����Ӧ״̬
typedef enum BEAnswerState 
{
	BEAnswerStateNone		= 0,	// û��Ӧ��
	BEAnswerStateOK			,		
	BEAnswerStateCancel		,
	BEAnswerStateAbort		, 
	BEAnswerStateRetry		,
	BEAnswerStateIgnore		, 
	BEAnswerStateYes		, 
	BEAnswerStateNo			, 
	BEAnswerStateClose		,
	BEAnswerStateHelp		, 
	BEAnswerStateTryAgain	, 
	BEAnswerStateContinue	, 
	BEAnswerStateTimeout
}BEAnswerState;
// Flags for controlUC::GetControlFlags()
#define UCFLAG_TABSTOP       0x00000001
#define UCFLAG_SETCURSOR     0x00000002
#define UCFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UCFIND_ALL           0x00000000
#define UCFIND_VISIBLE       0x00000001
#define UCFIND_ENABLED       0x00000002
#define UCFIND_HITTEST       0x00000004
#define UCFIND_TOP_FIRST     0x00000008
#define UCFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UCSTRETCH_NEWGROUP   0x00000001
#define UCSTRETCH_NEWLINE    0x00000002
#define UCSTRETCH_MOVE_X     0x00000004
#define UCSTRETCH_MOVE_Y     0x00000008
#define UCSTRETCH_SIZE_X     0x00000010
#define UCSTRETCH_SIZE_Y     0x00000020
// Flags used for controlling the paint
#define UCSTATE_NONE		0x00000000
#define UCSTATE_FOCUSED		0x00000001
#define UCSTATE_SELECTED	0x00000002
#define UCSTATE_DISABLED	0x00000004
#define UCSTATE_HOT			0x00000008
#define UCSTATE_PUSHED		0x00000010
#define UCSTATE_READONLY	0x00000020
#define UCSTATE_CAPTURED	0x00000040
#define UCSTATE_CUTED		0x00000080		//	2015-1-29	����״̬

///////////////////////////////////////////////////////////////////////////////////////////////////
//	2015-1-23	ΪͼƬ�����ӵı��
#define UCIMAGE_STATE_BACKGROUND	0x00000001		// �ؼ�����ͼ
#define UCIMAGE_STATE_NORMAL		0x00000002		// ������ʾʱ
#define UCIMAGE_STATE_FOCUSED		0x00000004		// �õ�����ʱ
#define UCIMAGE_STATE_SELECTED		0x00000008		// ѡ��ʱ
#define UCIMAGE_STATE_DISABLED		0x00000010		// �ؼ���Чʱ
#define UCIMAGE_STATE_HOT			0x00000020		// �õ����ʱ
#define UCIMAGE_STATE_PUSHED		0x00000040		// ����ʱ
#define UCIMAGE_STATE_READONLY		0x00000080		// �ؼ�����ֻ��ʱ
#define UCIMAGE_STATE_CAPTURED		0x00000100		// �������ʱ
#define UCIMAGE_STATE_CUTED			0x00000200		// ������ʱ
#define UCIMAGE_STATE_CHECKED		0x00000400		// check��ťѡ��ʱ

#define UCIMAGE_SCROLL_BUTTON1		0x00010000		// ��������button1����UCIMAGE_STATE_xxx���ʹ��
#define UCIMAGE_SCROLL_BUTTON2		0x00020000		// ��������button2����UCIMAGE_STATE_xxx���ʹ��
//#define UCIMAGE_SCROLL_BK			0x00040000		// ��ֱ��ʹ��UCIMAGE_STATE_xxx��
#define UCIMAGE_SCROLL_THUMB		0x00040000		// ����������ı�������UCIMAGE_STATE_xxx���ʹ��
#define UCIMAGE_SCROLL_RAIL			0x00080000		// �����������ͼ������UCIMAGE_STATE_xxx���ʹ��

#define UCIMAGE_PROGRESS_FORE		0x00100000		// ������������ͼ��



}