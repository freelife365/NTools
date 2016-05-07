#include "StdAfx.h"
#include "kpdeskDialog.h"


kpdeskDialog::kpdeskDialog(void)
	: wuc::windowWnd()
{
}


kpdeskDialog::~kpdeskDialog(void)
{
}

void kpdeskDialog::onFinalMessage(HWND hWnd)
{
	//m_paintManager.removePreMessageFilter(this);
	m_paintManager.removeNotifier(this);
	m_paintManager.reapObjects(m_paintManager.getRoot());

}
void kpdeskDialog::notify(wuc::BSNotifyUC& msg)
{
	if (msg.wsType == L"click")
	{
		
	}
	if (msg.wsType == L"killfocus")
	{
		
	}
}


LRESULT kpdeskDialog::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = S_OK;
	BOOL bHandled = TRUE;
	switch(uMsg)
	{
	case WM_CREATE:
		{
			m_paintManager.init(m_hWnd);	// ׼����ͼ����
			RECT rc = {0};
			GetClientRect(m_hWnd, &rc);
			wuc::controlUC *pRoot = new wuc::controlUC();
			pRoot->setPos(rc);
			//dialogBuilder builder;			//duilib.xml
			//controlUI* pRoot = builder.create(L"kpsk\\desk.xml", (UINT)0, NULL, &m_paintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
			assert(pRoot && L"���ֿؼ�ʧ�ܣ�");

			m_paintManager.attachDialog(pRoot);
			m_paintManager.addNotifier(this);   // ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����

			return lRes;
		}
		break;
	case WM_DESTROY:
		{
			::PostQuitMessage(0L);
		}
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_NCACTIVATE: // ����3����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT��������ϵͳ������
		{
			if( !::IsIconic(m_hWnd) ) 
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		break;
	case WM_NCCALCSIZE:
		return 0;
		break;
	case WM_NCPAINT:
		return 0;
		break;
	case WM_NCHITTEST:
		//lRes = onNcHitTest(uMsg, wParam, lParam, bHandled);
		//MessageBox(NULL, L"WM_NCHITTEST", L"msg", 0);
		break;
	case WM_GETMINMAXINFO:
		//lRes = onGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SIZE:
		//lRes = onSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SYSCOMMAND:
		//lRes = onSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	default:
		bHandled = FALSE;
		break;
	}

	if (bHandled) return lRes;

	//lRes = handleCustomMessage(uMsg, wParam, lParam, bHandled);
	//if (bHandled) return lRes;

	if (m_paintManager.messageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return wuc::windowWnd::handleMessage(uMsg, wParam, lParam);


}