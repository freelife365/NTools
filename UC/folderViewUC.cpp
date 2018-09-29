#include "folderViewUC.h"
#include "wucBase.h"
#include "AC/shellExt.h"
namespace wuc {

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-25 	BSFocusedNodeInfo��ʵ��
void BSFocusedNodeInfo::releaseState() {
	if (pfolderView)
	{
		IFolderCallbackUC *pFolder = pfolderView->getOwner();
		if (pFolder)
		{
			pFolder->setNodeState(wsKeyName, 0, UCSTATE_FOCUSED);
		}
		pfolderView = nullptr;
		wsKeyName.clear();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	folderElementUC		2015-2-2
//	��Ԫ�����е�ͼ�귽ʽ��ʾ�ڵ�
////////////////////////////////////////////////////////////////////////////////////////////////////////
folderElementUC::folderElementUC()
	: m_pOwner(NULL)
	, m_beFolderStyle(BEFolderStyleFlagIcon)
{
	setBkColor(0x7FCC0033);
}
folderElementUC::~folderElementUC()
{
	SEND_LOG(L"folderElementUC idx:%d free ...", m_iIndex);
}
PCWSTR folderElementUC::getClass() const
{
	return L"folderElementUC";
}
LPVOID folderElementUC::getInterface(PCWSTR pstrName)
{
	if( _tcscmp(pstrName, L"FolderElement") == 0 ) return static_cast<folderElementUC*>(this);
	return __super::getInterface(pstrName);
}
UINT folderElementUC::getControlFlags() const
{
	return UCFLAG_TABSTOP | (isEnabled() ? UCFLAG_SETCURSOR : 0);
}
void folderElementUC::setOwner(controlUC* pOwner)
{
	__super::setOwner(pOwner);
	//m_pOwner = static_cast<IListUC *>(pOwner->getInterface(L"IList"));
	m_pOwner = static_cast<folderViewUC *>(pOwner);
}
//
//PCWSTR folderElementUC::getFullPath(int iSubItem)
//{
//	if (iSubItem >= 0 && iSubItem < m_vecFullPathSubs.size())
//		return m_vecFullPathSubs[iSubItem].c_str();
//	return NULL;
//}
//void folderElementUC::setFullPath(int iSubItem, PCWSTR pfullPath)
//{
//	assert(m_pOwner);
//	if(!m_pOwner) return;
//	BSListItemInfo *pListInfo = m_pOwner->getListInfo();
//	assert(pListInfo);
//	if(!pListInfo) return;
//	if(iSubItem < 0 || iSubItem >= pListInfo->iColumns) return;
//	while(m_vecFullPathSubs.size() < pListInfo->iColumns)
//	{
//		m_vecFullPathSubs.push_back(L"");
//	}
//	m_vecFullPathSubs[iSubItem] = pfullPath ? pfullPath : L"";
//	invalidate();
//}
//BEFolderStyleFlag folderElementUC::getFolderStyle() const
//{
//	return m_beFolderStyle;
//}
//void folderElementUC::setFolderStyle(BEFolderStyleFlag style)
//{
//	if(m_beFolderStyle == style) return;
//	m_beFolderStyle = style;
//	invalidate();
//}
void folderElementUC::Event(BSEventUC& event)
{
	if( !isMouseEnabled() && event.Type > BEUCEventMouseBegin && event.Type < BEUCEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else __super::Event(event);
		return;
	}
	RECT rcNode = {0};
	int col = -1, nodeIndex = -1;
	static DWORD c_dwPredownState = 0;		// ��갴��ǰ��״̬���ж���������
	static bool c_bLDownForDrop = false;	// �Ƿ�Ϊ���������£�׼�������Ϸ�
	static POINT c_ptLDown = {0};			// ����������ʱ������
	switch(event.Type)
	{
	case BEUCEventMouseHover:
		{
			col = findColumn(event.ptMouse, &rcNode);
			if(col < 0) break;
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			if(m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse))
			//if (nodeIndex >= 0)
			{
				SEND_LOG(L"folderElementUC::Event BEUCEventMouseHover xxxxxxxx");
				setToolTip(m_pOwner->getOwner()->getNodeTipInfo(nodeIndex).c_str());
			}
			else setToolTip(L"");
		}
		break;
	//case BEUCEventMouseEnter:
	//	{
	//		col = findColumn(event.ptMouse);
	//		nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
	//		std::wstring ws = m_pOwner->getOwner()->getNodeKeyName(nodeIndex);
	//		SEND_LOG(L"folderElementUC::Event BEUCEventMouseEnter row:%d col:%d name:<%s>", m_iIndex, col, ws.c_str());
	//		return;
	//	}
	//	break;
	case BEUCEventMouseLeave:
		{
			m_pOwner->hoverNode(m_iIndex, -1);
			//m_pOwner->getOwner()->hoverNode(m_iIndex, -1);
			//col = findColumn(event.ptMouse);
			//nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			//std::wstring ws = m_pOwner->getOwner()->getNodeKeyName(nodeIndex);
			//SEND_LOG(L"folderElementUC::Event BEUCEventMouseLeave row:%d col:%d name:<%s>", m_iIndex, col, ws.c_str());
			return;
		}
		break;
	case BEUCEventMouseMove:
		{
			//POINT pt = event.ptMouse;
			//GetCursorPos(&pt);
			col = findColumn(event.ptMouse, &rcNode);
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			bool bInNode = m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse);
			//SEND_LOG(L"folderElementUC::Event BEUCEventMouseMove tick:%u col:%d nodeIndex:%d inNode:%d  pt:<%d, %d>", GetTickCount(), col, nodeIndex, bInNode, event.ptMouse.x, event.ptMouse.y);
			//if(col < 0) break;
			if (event.wKeyState & MK_LBUTTON)
			{
				if (c_bLDownForDrop /*bInNode ����갴�º�����ƶ���bInNode�п���Ϊfalse*/ && c_ptLDown != event.ptMouse)
				{
					c_bLDownForDrop = false;
					//DWORD state = m_pOwner->getNodeState(m_iIndex, col);
					//if (state & UCSTATE_SELECTED)
					{
						SEND_LOG(L"folderElementUC::Event BEUCEventMouseMove �����Ϸ� xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx downPT:<%d,%d> tick:%u", c_ptLDown.x, c_ptLDown.y, GetTickCount());
						BSNotifyUC msg;
						msg.wsType = L"startdragdrop";
						msg.pSender = this;
						msg.dwTimestamp = GetTickCount();
						//msg.ptMouse = event.ptMouse;
						msg.ptMouse = c_ptLDown;
						msg.lParam = event.lParam;
						msg.wParam = event.wKeyState;
					
						m_pManager->sendNotify(msg);
						return;
					}
				}
			}
			else
			{
				if(bInNode)
					m_pOwner->hoverNode(m_iIndex, col);
				else m_pOwner->hoverNode(m_iIndex, -1);
				//return;
			}
			//m_pOwner->getOwner()->hoverNode(m_iIndex, col);
			//SEND_LOG(L"focus me:%08X   click:%08X hover:%08X", this, m_pManager->m_pEventClick, m_pManager->m_pEventHover);
			//SEND_LOG(L"folderElementUC::Event BEUCEventMouseMove row:%d col:%d inNode:%d keystate:%04X pt:<%d,%d>", m_iIndex, col, bInNode, event.wKeyState, event.ptMouse.x, event.ptMouse.y);
		}
		break;
	case BEUCEventDBLClick:
		{
			col = findColumn(event.ptMouse, &rcNode);
			int nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			if (nodeIndex >= 0)
			{
				std::wstring keyName = m_pOwner->getOwner()->getNodeKeyName(nodeIndex);
				SEND_LOG(L"folderElementUC::Event BEUCEventDBLClick ִ�д����� nodeIndex:%d name:<%s>", nodeIndex, keyName.c_str());
				acm::shell::execSystemMenuCommand(keyName, acm::BEFileSyscmdFlagOpen);
			}
			else
			{	// ˫���հ׽ڵ��л�������ʾ
				BSNotifyUC notify;
				notify.dwTimestamp = GetTickCount();
				notify.wsType = L"showdesktop";
				notify.ptMouse = event.ptMouse;
				if(m_pOwner && m_pOwner->getOwner())
					notify.pSender = m_pOwner->getOwner()->getSelf();
				if(m_pManager) m_pManager->sendNotify(notify);
			}
			return;
		}
		break;
	/********************************************************************************************
	������Ҽ�+��������shift��ctrl�������ڵ�ʱ�Ľڵ�״̬˵��
	1��������down+û�и�����ʱ�������ǰ�ڵ��Ѿ�ѡ����ʹ��õ����㣬����ֻѡ�е�ǰ�ڵ㲢ʹ��õ�����
	2������Ҽ�down+û�и�����ʱ�������ǰ�ڵ��Ѿ�ѡ����ʹ��õ����㣬����ֻѡ�е�ǰ�ڵ㲢ʹ��õ�����
	3��������ctrl����+������upʱ����ѡ��ǰ�ڵ㣬��ʹ��õ�����
	4��������upʱ+û�и�����ʱ��ֻѡ�е�ǰ�ڵ㲢ʹ��õ�����
	5��������shift����+������down��ѡ�б�����һ����Χ�ڵ㣬��ʹ��갴�µĽڵ�õ�����
	********************************************************************************************/
	case BEUCEventButtonDown:
		{
			c_bLDownForDrop = false;
			c_ptLDown = event.ptMouse;
			//POINT pt = event.ptMouse;
			//GetCursorPos(&pt);
			col = findColumn(event.ptMouse, &rcNode);
			if(col < 0) break;
			SEND_LOG(L"folderElementUC::Event BEUCEventButtonDown begin tick:%u", GetTickCount());
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			bool bInNode = m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse);
			if(!bInNode)
			{
				break;
				//SEND_LOG(L"ERROR folderElementUC::Event BEUCEventButtonDown<selectNone> >>>>>>>>??? nodeIndex:%d", nodeIndex);
				
				//if((event.wKeyState & MK_CONTROL) == 0)	// �˴������Ƶ�BEUCEventButtonUp
				//	m_pManager->sendNotify(m_pOwner, L"selectNone");
				//return;
			}
			c_dwPredownState = m_pOwner->getNodeState(m_iIndex, col);
			if (event.wKeyState & MK_SHIFT)
			{	// ����5
				m_pOwner->selectRangeNode(m_iIndex, col);
				return;
			}
			else if ((event.wKeyState & MK_CONTROL) == 0)
			{	// ����1
				//DWORD state = m_pOwner->getNodeState(m_iIndex, col);
				if(c_dwPredownState & UCSTATE_SELECTED) m_pOwner->setNodeFocus(nodeIndex);
				else
				{
					nodeIndex = m_pOwner->selectNodeOnly(m_iIndex, col);
					m_pOwner->setNodeFocus(nodeIndex);
					SEND_LOG(L"ERROR folderElementUC::Event BEUCEventButtonDown XXXXXXXXXXXXXXXXXXX tick:%u pt:<%d,%d> pos:<%s>", GetTickCount(), event.ptMouse.x, event.ptMouse.y, gtc::format::rect(m_rcItem, true).c_str());
				}
				c_bLDownForDrop = true;
				//nodeIndex = m_pOwner->selectNodeOnly(m_iIndex, col);
				////nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
				//m_pOwner->setNodeFocus(nodeIndex);
				return;
			}
		}
		break;
	case BEUCEventButtonUp:
		{
			c_bLDownForDrop = false;
			col = findColumn(event.ptMouse, &rcNode);
			if(col < 0) break;
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			if(!m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse)) 
			{
				if((event.wKeyState & MK_CONTROL) == 0)
					m_pManager->sendNotify(m_pOwner, L"selectNone");
				break;
			}
			SEND_LOG(L"NOTE folderElementUC::Event BEUCEventButtonUp xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx col:%d node:%d keyState:%04X", col, nodeIndex, event.wKeyState);
			if (event.wKeyState & MK_CONTROL)
			{	// ����3
				m_pOwner->getOwner()->reverseNodeState(nodeIndex, UCSTATE_SELECTED);
				m_pOwner->setNodeFocus(nodeIndex);
				return;
			}
			if ((event.wKeyState & (MK_CONTROL|MK_SHIFT)) == 0)
			{	// ����4
				nodeIndex = m_pOwner->selectNodeOnly(m_iIndex, col);
				//return;	// ��Ҫȡ��ע�ͣ����ڴ��ݸ������ڵ���freeTracker()
			}
		}
		break;
	case BEUCEventRButtonDown:
		{
			col = findColumn(event.ptMouse, &rcNode);
			if(col < 0) break;
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			if(!m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse)) break;
			if ((event.wKeyState & (MK_CONTROL|MK_SHIFT)) == 0)
			{	// ����2
				DWORD state = m_pOwner->getNodeState(m_iIndex, col);
				if(state & UCSTATE_SELECTED) m_pOwner->setNodeFocus(m_pOwner->getOwner()->getNodeIndex(m_iIndex, col));
				else
				{
					nodeIndex = m_pOwner->selectNodeOnly(m_iIndex, col);
					m_pOwner->setNodeFocus(nodeIndex);
				}
				return;
			}
			//m_pOwner->selectNode(m_iIndex, col, event.wKeyState);
			//
			////invalidate();
			//SEND_LOG(L"folderElementUC::Event BEUCEventButtonDown row:%d col:%d keystate:%04X", m_iIndex, col, event.wKeyState);
		}
		break;
	case BEUCEventContextMenu:
		{
			col = findColumn(event.ptMouse, &rcNode);
			if(col < 0) break;
			nodeIndex = m_pOwner->getOwner()->getNodeIndex(m_iIndex, col);
			//if(!m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse)) break;
			if(!m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse)) 
			{
				if((event.wKeyState & MK_CONTROL) == 0)
					m_pManager->sendNotify(m_pOwner, L"selectNone");
			}
			BSNotifyUC notify;
			notify.dwTimestamp = GetTickCount();
			notify.wsType = L"contextMenu";
			notify.ptMouse = event.ptMouse;
			if(m_pOwner && m_pOwner->getOwner())
				notify.pSender = m_pOwner->getOwner()->getSelf();
			
			notify.lParam = m_pOwner->getOwner()->ptInNode(nodeIndex, event.ptMouse);
			if(m_pManager) m_pManager->sendNotify(notify);
			return;
		}
		break;
	//case BEUCEventKeyDown:
	//	{
	//		SEND_LOG(L"folderElementUC::Event BEUCEventKeyDown row:%d col:%d", m_iIndex, col);
	//		return;
	//	}
	//	break;
	default:
		break;
	}

	__super::Event(event);
}
SIZE folderElementUC::estimateSize(SIZE szAvailable)
{
	SIZE szXY = m_szxyFixed;
	//szXY.cx = 0;
	//szXY.cy = 50;
	//assert(m_pOwner);
	//if(!m_pOwner) return szXY;
	//BSListItemInfo *pListInfo = m_pOwner->getListInfo();
	////assert(pListInfo);
	////if(!pListInfo) return szXY;
	//if (m_beFolderStyle == BEFolderStyleFlagIcon)
	//{	// ������ʾ1��2��
	//}
	//else if (m_beFolderStyle == BEFolderStyleFlagList)
	//{	// ������ʾ2�У��̶�Ϊ1�У�
	//}
	//if (szXY.cy == 0 && m_pManager)
	//{
	//	szXY.cy = m_pManager->getDefaultFontInfo()->tm.tmHeight + 8;
	//	if(pListInfo) szXY.cy += pListInfo->rcPadding.top + pListInfo->rcPadding.bottom;
	//}

	return szXY;
}
void folderElementUC::paint(HDC hdc, const RECT &rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
	//__super::paint(hdc, rcPaint);
	drawItem(hdc, m_rcItem);
}
void folderElementUC::drawItem(HDC hdc, const RECT& rcItem)
{
	assert(m_pOwner);
	if(!m_pOwner) return;
	BSListItemInfo *pListInfo = m_pOwner->getListInfo();
	assert(pListInfo);
	if(!pListInfo) return;

	IListCallbackUC *pCallback = m_pOwner->getItemCallback();
	IFolderCallbackUC *pFolderCallback = m_pOwner->getOwner();
	assert(pCallback);
	assert(pFolderCallback);
	if(!pCallback) return;
	if(!pFolderCallback || pFolderCallback->getNodeCount() == 0) return;
	//Gdiplus::Graphics grap(hdc);
	// ��������ÿ���ڵ�
	for (int iSubIdx = 0; iSubIdx < pListInfo->iColumns; ++iSubIdx)
	{
		RECT rcNode = {pListInfo->rcColumn[iSubIdx].left, m_rcItem.top, pListInfo->rcColumn[iSubIdx].right, m_rcItem.bottom};
		rcNode.left += pListInfo->rcTextPadding.left;
		rcNode.top += pListInfo->rcTextPadding.top;
		rcNode.right -= pListInfo->rcTextPadding.right;
		rcNode.bottom -= pListInfo->rcTextPadding.bottom;
		if (m_pOwner && m_pOwner->getFolderStyle() == BEFolderStyleFlagList)
		{	// ������б�ؼ�����Ҫ�ų��Ҳഹֱ��������ռ�õĿ��
			const scrollBarUC *pScroll = m_pOwner->getVerticalScrollBar();
			if (pScroll && pScroll->isVisible())
			{
				rcNode.right -= rectWidth(pScroll->getPosition());
			}
		}
		pCallback->drawItem(hdc, rcNode, m_iIndex, iSubIdx, this);
		//if (pCallback->getItemData(this, m_iIndex, iSubIdx))
		//{
		//	gtc::drawer::fillRectangle(grap, this->m_dwBackColor, rcNode);
		//	wchar_t pbuf[MAX_PATH] = {0};
		//	std::swprintf(pbuf, _countof(pbuf), L"row:%d col:%d", m_iIndex, iSubIdx);
		//	gtc::drawer::drawString(grap, pbuf, rcNode, m_pManager->getDefaultFontInfo()->hFont);
		//}
	}
}
int folderElementUC::findColumn(POINT pt, RECT *prcNodePos /*= NULL*/)
{
	assert(m_pOwner);
	if(!m_pOwner) return -1;
	BSListItemInfo *pListInfo = m_pOwner->getListInfo();
	assert(pListInfo);
	if(!pListInfo) return -1;

	for (int iSubIdx = 0; iSubIdx < pListInfo->iColumns; ++iSubIdx)
	{
		RECT rcNode = {pListInfo->rcColumn[iSubIdx].left, m_rcItem.top, pListInfo->rcColumn[iSubIdx].right, m_rcItem.bottom};
		rcNode.left += pListInfo->rcTextPadding.left;
		rcNode.top += pListInfo->rcTextPadding.top;
		rcNode.right -= pListInfo->rcTextPadding.right;
		rcNode.bottom -= pListInfo->rcTextPadding.bottom;
		if (PtInRect(&rcNode, pt))
		{
			if(prcNodePos) *prcNodePos = rcNode;
			return iSubIdx;
		}
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	folderViewUC��		2015-1-29
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
//BSFocusedNodeInfo	folderViewUC::mc_bsFocusedNode;	// �þ�̬�ģ��Ա�֤�������乲��
BSHotNodeInfo		folderViewUC::mc_bsHotNode;		// �þ�̬�ģ��Ա�֤�������乲��

folderViewUC::folderViewUC()
	: m_beFolderStyle(BEFolderStyleFlagIcon)
	, m_bSetterHorizontal(true), m_bSetterAnywhere(false)
	, m_pOwner(NULL)
	, m_iListItemHeight(24)
	, m_iCurNodeIndex(-1)
{
	memset(&m_szNode, 0, sizeof(m_szNode));
	memset(&m_szNodeSpace, 0, sizeof(m_szNodeSpace));
}
folderViewUC::~folderViewUC()
{

}

PCWSTR folderViewUC::getClass() const
{
	return L"folderViewUC";
}
LPVOID folderViewUC::getInterface(PCWSTR pstrName)
{
	if( _tcscmp(pstrName, L"FolderView") == 0 ) return static_cast<folderViewUC*>(this);
	if( _tcscmp(pstrName, L"IList") == 0 ) return static_cast<IListUC*>(this);
	if( _tcscmp(pstrName, L"IListOwner") == 0 ) return static_cast<IListOwnerUC*>(this);
	return __super::getInterface(pstrName);
}
UINT folderViewUC::getControlFlags() const
{
	return UCFLAG_TABSTOP | (isEnabled() ? UCFLAG_SETCURSOR : 0);
}

void folderViewUC::Event(BSEventUC& event)
{
	if( !isMouseEnabled() && event.Type > BEUCEventMouseBegin && event.Type < BEUCEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->Event(event);
		else __super::Event(event);
		return;
	}
	if (event.Type == BEUCEventDBLClick)
	{
		// ˫���հ׽ڵ��л�������ʾ
		BSNotifyUC notify;
		notify.dwTimestamp = GetTickCount();
		notify.wsType = L"showdesktop";
		notify.ptMouse = event.ptMouse;
		notify.pSender = this;
		if(m_pManager) m_pManager->sendNotify(notify);
	}
	__super::Event(event);
}
//SIZE folderViewUC::estimateSize(SIZE szAvailable)
//{
//	SIZE szXY = m_szxyFixed;
//	if (m_beFolderStyle == BEFolderStyleFlagIcon)
//	{	// ������ʾ1��2��
//	}
//	else if (m_beFolderStyle == BEFolderStyleFlagList)
//	{	// ������ʾ2�У��̶�Ϊ1�У�
//	}
//}

void folderViewUC::setPosition(RECT pos)
{
	//SEND_LOG(L"folderViewUC::setPosition begin<%s> ...", gtc::format::rect(m_rcItem, true).c_str());

	SIZE szOld = {rectWidth(m_rcItem), rectHeight(m_rcItem)};
	
	__super::setPosition(pos);
	if(!getHeader()) return;
	SIZE szNew = {rectWidth(m_rcItem), rectHeight(m_rcItem)};
	SIZE szNode = getNodeWorkSize();
	if ((szOld.cx != szNew.cx || szOld.cy != szNew.cy) && m_beFolderStyle == BEFolderStyleFlagIcon && szNode.cx > 0)
	{
		int cols = szNew.cx/szNode.cx;
		int rows = szNew.cy/szNode.cy;
		if(cols != getColumns() || rows != getRows())
		{
			refreshLayout();
		}
	}
	m_pOwner->positionChanged();
	//SEND_LOG(L"folderViewUC::setPosition end <%s> ...", gtc::format::rect(m_rcItem, true).c_str());
}
BEFolderStyleFlag folderViewUC::getFolderStyle() const
{
	return m_beFolderStyle;
}
void folderViewUC::setFolderStyle(BEFolderStyleFlag style)
{
	if(m_beFolderStyle == style) return;
	m_beFolderStyle = style;
	refreshLayout(true);
}

SIZE folderViewUC::getNodeWorkSize() const
{
	SIZE szSpace = getNodeSpaceSize();
	return szSpace + m_szNode;
}
SIZE folderViewUC::getNodeSize() const
{
	return m_szNode;
}
void folderViewUC::setNodeSize(SIZE szNode)
{
	if(m_szNode == szNode) return;
	m_szNode = szNode;
	if(m_beFolderStyle == BEFolderStyleFlagIcon)
		refreshLayout(true);
	needUpdate();
}
SIZE folderViewUC::getNodeSpaceSize() const
{
	if(m_beFolderStyle == BEFolderStyleFlagList)
	{
		SIZE sz = {0,0};
		return sz;
	}
	return m_szNodeSpace;
}	
void folderViewUC::setNodeSpaceSize(SIZE szNode)
{
	if(m_szNodeSpace == szNode) return;
	m_szNodeSpace = szNode;
	if(m_beFolderStyle == BEFolderStyleFlagIcon)
		refreshLayout(true);
}
void folderViewUC::setOwner(controlUC *pOwner)
{
	m_pOwner = NULL;
	if(pOwner)
		m_pOwner = static_cast<IFolderCallbackUC *>(pOwner->getInterface(L"IFolderCallback"));

}
IFolderCallbackUC* folderViewUC::getOwner() const
{
	return m_pOwner;
}
void folderViewUC::setListItemHeight(int iHeight)
{
	if(m_iListItemHeight == iHeight) return;
	m_iListItemHeight = iHeight;
}
int folderViewUC::getListItemHeight() const
{
	return m_iListItemHeight;
}

//BSFocusedNodeInfo& folderViewUC::getFocusedNode()
//{
//	return mc_bsFocusedNode;
//}

BSHotNodeInfo& folderViewUC::getHotNode()
{
	return mc_bsHotNode;
}

bool folderViewUC::isSetterHorizontal() const
{
	return m_bSetterHorizontal;
}
void folderViewUC::setSetterHorizontal(bool bHorizontal)
{
	if(m_bSetterHorizontal == bHorizontal) return;
	m_bSetterHorizontal = bHorizontal;
	refreshLayout();
}

bool folderViewUC::isSetterAnywhere() const
{
	return m_bSetterAnywhere;
}
void folderViewUC::setSetterAnywhere(bool bAnywhere)
{
	if(m_bSetterAnywhere == bAnywhere) return;
	m_bSetterAnywhere = bAnywhere;
	refreshLayout();
}
int folderViewUC::getCurrentNodeIndex() const
{
	return m_iCurNodeIndex;
}
std::wstring folderViewUC::toString() const
{
	std::wostringstream out;
	const listHeaderUC *pHeader = getHeader();
	out << L"pos=<" << gtc::format::rect(m_rcItem, true).c_str() << L">\n";
	out << (!pHeader ? L"�ޱ���" : L"���⣺");
	if(pHeader)
	{
		out << L"�ɼ�=" << pHeader->isVisible() << L" ������" << pHeader->getCount();
		for (int col = 0; col < pHeader->getCount(); ++col)
		{
			out << L"\n\tcol=" << col << L" pos=<" << gtc::format::rect(pHeader->getItemAt(col)->getPosition(), true).c_str() << L">";
		}
	}
	if (m_pList)
	{
		out << L"\n\t������" << m_pList->getCount();
		for (int row = 0; row < m_pList->getCount(); ++row)
		{
			out << L"\n\trow=" << row << L" pos=<" << gtc::format::rect(m_pList->getItemAt(row)->getPosition(), true).c_str() << L">";
		}
		out << L"\n\t��������" << this->getVerticalScrollBar()->getScrollRange();
	}
	return out.str();
}
void folderViewUC::init(BEFolderStyleFlag styleFlag, SIZE szNode, SIZE szNodeSpace)
{
	m_beFolderStyle = styleFlag;
	m_szNode = szNode;
	m_szNodeSpace = szNodeSpace;
	refreshLayout(true);
}
bool folderViewUC::modifyHeaderCount(int targetCols, int fixedWidth /*= 0*/, bool bforceUpdateWidth /*= false*/)
{
	listHeaderUC *pHeader = getHeader();
	assert(pHeader);
	if(!pHeader) return false;
	listHeaderItemUC *pHeaderItem = NULL;
	wchar_t pbuf[MAX_PATH] = {0};
	std::swprintf(pbuf, _countof(pbuf), L"%d", fixedWidth);
	bool bModify = getColumns() != targetCols;
	if (bModify)
	{
		// �Ƴ��������
		while (getColumns() > targetCols)
		{
			remove(pHeader->getItemAt(pHeader->getCount()-1));
		}
		// ���䲻�����
		while (getColumns() < targetCols)
		{
			pHeaderItem = new wuc::listHeaderItemUC();
			/*if(!bforceUpdateWidth) */pHeaderItem->setAttribute(L"width", pbuf);
			add(pHeaderItem);
		}
		if(m_pHeader) m_pHeader->needUpdate();
		needUpdate();
	}
	if (bforceUpdateWidth)
	{
		for (int idx = 0; idx < targetCols; ++idx)
		{
			controlUC *pControl = pHeader->getItemAt(idx);
			if (pControl)
			{
				pControl->setAttribute(L"width", pbuf);
			}
		}
	}
	return bModify;
}

bool folderViewUC::modifyElementCount(int targetRows, int fixedHeight /*= 0*/, bool bforceUpdateHeight /*= false*/)
{
	folderElementUC *pItem = nullptr;
	wchar_t pbuf[MAX_PATH] = {0};
	std::swprintf(pbuf, _countof(pbuf), L"%d", fixedHeight);
	bool bModify = getCount() != targetRows;
	if (bModify)
	{
		// �Ƴ������Ԫ����
		while(getCount() > targetRows)
			removeAt(getCount()-1);
		// ����ȱ�ٵ���
		while(getCount() < targetRows)
		{
			pItem = new folderElementUC();
			add(pItem);
			/*if(!bforceUpdateHeight) */pItem->setAttribute(L"height", pbuf);
			//pItem->setBkColor(getCount()%2 ? 0x009966 : 0x33FF99);
			//pItem->needParentUpdate();
			//pItem->invalidate();
		}
		m_pList->needUpdate();
		needUpdate();
	}

	if (bforceUpdateHeight)
	{
		for (int idx = 0; idx < getCount(); ++idx)
		{
			controlUC *pControl = getItemAt(idx);
			if (pControl)
			{
				pControl->setAttribute(L"height", pbuf);
			}
		}
	}
	return bModify;
}
void folderViewUC::refreshLayout(bool bforceUpdate /*= false*/)
{
	//return;
	if(!m_pOwner) {
		//SEND_LOG(L"ERROR refreshLayout OWNER NULL tag:%u", getTag());
		return;
	}
	if(isEmpty(m_rcItem)) {
		//SEND_LOG(L"ERROR refreshLayout λ�ÿ� NULL tag:%u", getTag());
		return;
	}
	if(m_szNode.cx <= 0 || m_szNode.cy <= 0) {
		//SEND_LOG(L"ERROR refreshLayout �ڵ�� NULL tag:%u", getTag());
		return;
	}
	//if (getTag() == 999)
	//{
	//	SEND_LOG(L"ERROR refreshLayout ���� cols:%d rows:%d rect:<%s>", getCols(), getRows(), gtc::format::rect(m_rcItem, true).c_str());
	//	setAttribute(L"pos", gtc::format::rect(m_rcItem).c_str());
	//}
	//getHeader()->setVisible(false);
	listHeaderUC *pHeader = getHeader();
	assert(pHeader);
	listHeaderItemUC *pHeaderItem = NULL;
	wchar_t pbuf[MAX_PATH] = {0};
	const std::size_t nodeCount = m_pOwner->getNodeCount();
	const SIZE szNode = getNodeWorkSize();	// ����Ԥ�����Ľڵ��С

	if (m_beFolderStyle == BEFolderStyleFlagList)
	{	
		if(nodeCount <= 0)
		{
			// �Ƴ������Ԫ����
			if(getCount() > 0) removeAll();
			return;
		}
		// ֻ����һ���б��⣬��Ԫ���и߹̶�
		modifyHeaderCount(1);
		pHeaderItem = static_cast<listHeaderItemUC *>(pHeader->getItemAt(0));
		if (pHeaderItem)
		{	// ��֤��ʽ���л�ʱ��list��ʽ����ȷ��ʾ
			pHeaderItem->setAttribute(L"pos", L"0,0,0,0");
			pHeaderItem->setAttribute(L"width", L"0");
		}

		// ������Ԫ��
		modifyElementCount(nodeCount, m_iListItemHeight, true);
	}
	else
	{
		// �ڵ��Ƿ����������� m_bSetterAnywhere
		// ��ģʽ���к��ź����� m_bSetterHorizontal
		// ���ţ������ҡ����ϵ��� m_bSetterHorizontal=true
		//	a���б��������������ȼ��㣬���п�=szNode.cx
		//	b����Ԫ�����������ܽڵ������������м��㣬�Ҹ߶�=szNode.cy
		// ���ţ����ϵ��¡������� m_bSetterHorizontal=false
		//	a��������Թ̶���ͨ�������߶Ⱥͽڵ�߶Ƚ��м���õ�
		//	b�����������ܽڵ������������м���õ�
		//assert(m_szNode.cx > 0 && m_szNode.cy > 0);
		
		if (m_bSetterAnywhere)
		{	
			// ����ģʽ�»���ݽڵ���б��СԤ�ȷ������нڵ�λ��
			// ���ڵ����������б����Ľڵ�λ����ʱ���Բ���ʾ
			// ���š����Ŷ��б�Ԫ�ط���û��Ӱ��
			// �����ʹ�õ�����������
			RECT rtWork = m_rcItem;// {0};
			////SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rtWork,0);
			SIZE szWork = {rectWidth(rtWork), rectHeight(rtWork)};
			////SIZE szNode = getNodeWorkSize();
			int rows = szWork.cy/szNode.cy;
			int cols = szWork.cx/szNode.cx;
			if(rows <= 0) rows = 1;
			if(cols <= 0) cols = 1;
			//// ����ڵ���϶
			//m_szNodeSpace.cx = (szWork.cx - m_szNode.cx * cols)/cols;
			//m_szNodeSpace.cy = (szWork.cy - m_szNode.cy * rows)/rows;
			//szNode = getNodeWorkSize();
			// ������
			modifyHeaderCount(cols, szNode.cx, true);
			// ������
			modifyElementCount(rows, szNode.cy, true);
		}
		else
		{
			if(nodeCount <= 0)
			{
				if(getCount() > 0) removeAll();
				return;
			}
			if (m_bSetterHorizontal)
			{	// ����
				// Ӧ���е���
				int cols = rectWidth(m_rcItem)/szNode.cx;		
				if(cols <= 0) cols = 1;
				modifyHeaderCount(cols, szNode.cx, true);
				// ����Ӧ�е�����
				int rows = nodeCount/cols;
				if(nodeCount%cols) ++rows;
				modifyElementCount(rows, szNode.cy, bforceUpdate);
			}
			else
			{	// ����
				// ����̶�������
				int rows = rectHeight(m_rcItem)/szNode.cy;
				if(rows <= 0) rows = 1;
				modifyElementCount(rows, szNode.cy, bforceUpdate);
				// ����Ӧ�е�����
				int cols = nodeCount/rows;
				if(nodeCount%rows) ++cols;
				modifyHeaderCount(cols, szNode.cx, bforceUpdate);
			}
		}
		
	}
//#if defined(_DEBUG)
//	SEND_LOG(L"NOTE folderViewUC::refreshLayout rows:%d cols:%d listPos:<%s> folderPos:<%s>", getRows(), getCols(), gtc::format::rect(m_pList->getPosition(), true).c_str(), gtc::format::rect(m_rcItem, true).c_str());
//	for (int idx = 0; idx < getCount(); ++idx)
//	{
//		controlUC *pControl = getItemAt(idx);
//		if (pControl)
//		{
//			SEND_LOG(L"NOTE folderViewUC::refreshLayout idx:%d pos:<%s> fixedsize:<%d,%d>", idx, gtc::format::rect(pControl->getPosition(), true).c_str(), pControl->getFixedWidth(), pControl->getFixedHeight());
//		}
//	}
//#endif
}

void folderViewUC::invalidateEx(int subItemIndex)
{
	RECT rcSubItem = {0};
	if (getSubItemPosition(subItemIndex, rcSubItem))
	{
		m_pManager->invalidate(rcSubItem);
	}

	//int iItem = -1;
	//if(m_bsInfo.iColumns > 0) iItem = nodeIndex/m_bsInfo.iColumns;
	////listHeaderUC *pHeader = getHeader();
	////if(!pHeader) return;
	//if (!bOnlyNode || m_beFolderStyle == BEFolderStyleFlagList)
	//{
	//	controlUC *pControl = getItemAt(iItem);
	//	if(pControl) pControl->invalidate();
	//	//invalidate();
	//}
	//else
	//{
	//	RECT rc = {0};
	//	if (m_pManager && getSubItemPosition(nodeIndex, rc))
	//	{
	//		m_pManager->invalidate(rc);
	//	}
	//}
}

bool folderViewUC::getSubItemPosition(int subItemIndex, RECT &rcPos)
{
	memset(&rcPos, 0, sizeof(rcPos));
	int iItem, iSubItem;
	if (getItemPositionIndex(subItemIndex, iItem, iSubItem))
	{
		rcPos = __super::getSubItemPosition(iItem, iSubItem);
		return true;
	}
	return false;
	//if (m_bSetterHorizontal)
	//{
	//	//if(nodeIndex < 0 || m_bsInfo.iColumns <= 0 || nodeIndex >= getCount()*m_bsInfo.iColumns) return false;
	//	//controlUC *pControl = getItemAt(nodeIndex/m_bsInfo.iColumns);
	//	//if (pControl)
	//	//{
	//	//	int col = nodeIndex%m_bsInfo.iColumns;
	//	//	rcPos = pControl->getPosition();
	//	//	rcPos.left = m_bsInfo.rcColumn[col].left;
	//	//	rcPos.right = m_bsInfo.rcColumn[col].right;
	//	//	return true;
	//	//}
	//}
	//else
	//{	// ����
	//	//const int rows = getCount();
	//	//if(nodeIndex < 0 || m_bsInfo.iColumns <= 0 || rows <= 0 || nodeIndex >= rows*m_bsInfo.iColumns) return false;
	//	//controlUC *pControl = getItemAt(nodeIndex%rows);
	//	//if (pControl)
	//	//{
	//	//	int col = nodeIndex/rows;
	//	//	rcPos = pControl->getPosition();
	//	//	rcPos.left = m_bsInfo.rcColumn[col].left;
	//	//	rcPos.right = m_bsInfo.rcColumn[col].right;
	//	//	return true;
	//	//}

	//}
	return false;
}
bool folderViewUC::getItemPositionIndex(int subItemIndex, int &refiItem, int &refiSubItem)
{
	refiItem = refiSubItem = -1;
	//assert(subItemIndex >= 0);
	if(subItemIndex < 0) return false;
	const int rows = getRows();
	const int cols = getCols();
	if(subItemIndex < 0 || subItemIndex >= cols*rows ) return false;
	if (m_bSetterHorizontal)
	{
		assert(cols > 0);
		refiItem = subItemIndex/cols;
		refiSubItem = subItemIndex%cols;
	}
	else
	{	// ����
		assert(rows > 0);
		refiSubItem = subItemIndex/rows;
		refiItem = subItemIndex%rows;
	}
	return true;
}
void folderViewUC::hoverNode(int iItem, int iSubItem)
{
	assert(m_pOwner);
	if(!m_pOwner) return;
	const int nodeIndex = m_pOwner->getNodeIndex(iItem, iSubItem);// iItem * m_pFolderView->getColumns() + iSubItem;
	const int nodeCount = m_pOwner->getNodeCount();
	if(nodeIndex < 0 || nodeIndex >= nodeCount || iSubItem < 0)
	{	// ��Ч�ڵ㣬ȡ��ԭ����hot
		if(mc_bsHotNode.iNodeIndex >= 0 && mc_bsHotNode.iNodeIndex < nodeCount)
		{
			m_pOwner->setNodeState(mc_bsHotNode.iNodeIndex, 0, UCSTATE_HOT);
			//_pdialogMainPtr->setNodeState(m_vecNodes[mc_bsHotNode.iNodeIndex], 0, UCSTATE_HOT);
		}
		mc_bsHotNode.iNodeIndex = -1;
		mc_bsHotNode.pfolderView = nullptr;
		//mc_bsHotNode.wsKeyName.clear();
		return ;
	}
	if (mc_bsHotNode.iNodeIndex != nodeIndex)
	{
		if(mc_bsHotNode.iNodeIndex >= 0 && mc_bsHotNode.iNodeIndex < nodeCount)
		{
			m_pOwner->setNodeState(mc_bsHotNode.iNodeIndex, 0, UCSTATE_HOT);
			//_pdialogMainPtr->setNodeState(m_vecNodes[mc_bsHotNode.iNodeIndex], 0, UCSTATE_HOT);
		}
		mc_bsHotNode.iNodeIndex = nodeIndex;
		mc_bsHotNode.pfolderView = this;
		m_pOwner->setNodeState(mc_bsHotNode.iNodeIndex, UCSTATE_HOT, 0);
		//mc_bsHotNode.wsKeyName = m_vecNodes[nodeIndex];
		//_pdialogMainPtr->setNodeState(mc_bsHotNode.wsKeyName, UCSTATE_HOT, 0);
	}
}

//void folderViewUC::selectNode(int iItem, int iSubItem, UINT keyState)
//{
//	assert(m_pOwner);
//	if(!m_pOwner) return ;
//	m_iCurNodeIndex = m_pOwner->getNodeIndex(iItem, iSubItem);// iItem * m_pFolderView->getColumns() + iSubItem;
//	const int nodeCount = m_pOwner->getNodeCount();
//	if(m_iCurNodeIndex < 0 || m_iCurNodeIndex >= nodeCount)
//	{
//		//setNodeFocus(-1, 0);		// ����ڵ�Ҳȡ��
//		m_pManager->sendNotify(this, L"selectNone");
//		return ;
//	}
//	int iFocusedNode = setNodeFocus(m_iCurNodeIndex);
//
//	//mc_bsFocusedNode.bDoneSelect = true;
//	//if (mc_bsFocusedNode.pfolderView == this)
//	//{
//	//	iFocusedNode = mc_bsFocusedNode.iNodeIndex;
//	//}
//	//else
//	//{
//	//	if (mc_bsFocusedNode.pfolderView)
//	//	{
//	//		// �ͷ�ԭ���Ľ���ڵ�
//	//		m_pOwner->setNodeState(mc_bsFocusedNode.wsKeyName, 0, ((keyState & (MK_CONTROL|MK_SHIFT)) ? UCSTATE_FOCUSED : (UCSTATE_FOCUSED|UCSTATE_SELECTED)));
//	//		//_pdialogMainPtr->setNodeState(mc_bsFocusedNode.wsKeyName, 0, ((keyState & (MK_CONTROL|MK_SHIFT)) ? UCSTATE_FOCUSED : (UCSTATE_FOCUSED|UCSTATE_SELECTED)));
//
//	//		mc_bsFocusedNode.pfolderView = NULL;
//	//		mc_bsFocusedNode.iNodeIndex = -1;
//	//		mc_bsFocusedNode.wsKeyName.clear();
//	//	}
//	//}
//	if (keyState & MK_SHIFT)
//	{
//		int iFirst = min(m_iCurNodeIndex, max(iFocusedNode, 0));
//		int iLast = max(m_iCurNodeIndex, max(iFocusedNode, 0));
//		// �Ƴ�����[iFirst,iLast]��Χ�ڵ�״̬���
//		for (int idx = 0; idx < nodeCount; ++idx)
//		{
//			if (idx < iFirst || idx > iLast)
//			{
//				m_pOwner->setNodeState(idx, 0, UCSTATE_SELECTED);
//				//_pdialogMainPtr->setNodeState(m_vecNodes[idx], 0, UCSTATE_SELECTED);
//			}
//		}
//		// �����µ�
//		for (int idx = iFirst; idx <= iLast; ++idx)
//		{
//			m_pOwner->setNodeState(idx, UCSTATE_SELECTED, 0);
//			//_pdialogMainPtr->setNodeState(m_vecNodes[idx], UCSTATE_SELECTED, 0);
//		}
//	}
//	else if (keyState & MK_CONTROL)
//	{   
//		m_pOwner->reverseNodeState(m_iCurNodeIndex, UCSTATE_SELECTED);
//		//_pdialogMainPtr->reverseNodeState(m_vecNodes[nodeIndex], UCSTATE_SELECTED);
//	}
//	else
//	{
//		//// ȡ��ԭ��ѡ����
//		//for (int idx = 0; idx < nodeCount; ++idx)
//		//{
//		//	if (idx != m_iCurNodeIndex)
//		//	{
//		//		m_pOwner->setNodeState(idx, 0, UCSTATE_SELECTED);
//		//	}
//		//}
//		//// ������ǰѡ����
//		//m_pOwner->reverseNodeState(m_iCurNodeIndex, UCSTATE_SELECTED);
//		if(keyState & MK_LBUTTON)
//			m_pOwner->selectNodeOnly(m_iCurNodeIndex);
//	}
//	//if(iFocusedNode != nodeIndex)
//	//{
//	//	if(iFocusedNode >= 0 && iFocusedNode < nodeCount)
//	//	{
//	//		m_pOwner->setNodeState(iFocusedNode, 0, UCSTATE_FOCUSED);
//	//		//_pdialogMainPtr->setNodeState(m_vecNodes[iFocusedNode], 0, UCSTATE_FOCUSED);
//	//	}
//	//	mc_bsFocusedNode.iNodeIndex = nodeIndex;
//	//	mc_bsFocusedNode.pfolderView = this;
//	//	mc_bsFocusedNode.wsKeyName = m_pOwner->getNodeKeyName(nodeIndex);
//	//	m_pOwner->setNodeState(nodeIndex, UCSTATE_FOCUSED, 0);
//	//	//_pdialogMainPtr->setNodeState(mc_bsFocusedNode.wsKeyName, UCSTATE_FOCUSED, 0);
//	//}
//}
int folderViewUC::selectNodeOnly(int iItem, int iSubItem)
{
	m_iCurNodeIndex = m_pOwner->getNodeIndex(iItem, iSubItem);
	if(m_pOwner->selectNodeOnly(m_iCurNodeIndex)) return m_iCurNodeIndex;
	m_iCurNodeIndex = -1;
	return m_iCurNodeIndex;
}

DWORD folderViewUC::getNodeState(int iItem, int iSubItem)
{
	return m_pOwner->getNodeState(m_pOwner->getNodeIndex(iItem, iSubItem));
}
int folderViewUC::setNodeFocus(int nodeIndex)
{
	std::wstring wskey = m_pOwner->getNodeKeyName(nodeIndex);
	if(wskey.empty()) return -1;
	//if (wskey.empty())
	//{
	//	mc_bsFocusedNode.releaseState();
	//	return -1;
	//}
	//if(wskey != mc_bsFocusedNode.wsKeyName) mc_bsFocusedNode.releaseState();
	//mc_bsFocusedNode.pfolderView = this;
	//mc_bsFocusedNode.wsKeyName = wskey;
	m_pOwner->setNodeState(wskey, UCSTATE_FOCUSED, 0);
	return nodeIndex;
}

int folderViewUC::selectRangeNode(int iItem, int iSubItem)
{
	int nodeIndex = -1;
	if (getOwner()->getFocusFolderView() != this || m_iCurNodeIndex < 0)
	//if (mc_bsFocusedNode.pfolderView != this || m_iCurNodeIndex < 0)
	{
		m_iCurNodeIndex = selectNodeOnly(iItem, iSubItem);
		setNodeFocus(m_iCurNodeIndex);
		nodeIndex = m_iCurNodeIndex;
	}
	else
	{
		nodeIndex = m_pOwner->getNodeIndex(iItem, iSubItem);
		if (nodeIndex < 0)
		{
			nodeIndex = 0;
		}
		assert(m_iCurNodeIndex >= 0 && m_iCurNodeIndex < (int)m_pOwner->getNodeCount());
		assert(nodeIndex >= 0 && nodeIndex < (int)m_pOwner->getNodeCount());
		int minIndex = min(nodeIndex, m_iCurNodeIndex);
		int maxIndex = max(nodeIndex, m_iCurNodeIndex);
		for (int idx = 0; idx < (int)m_pOwner->getNodeCount() ; ++idx)
		{
			if(idx >= minIndex && idx <= maxIndex) m_pOwner->setNodeState(idx, UCSTATE_SELECTED, 0);
			else m_pOwner->setNodeState(idx, 0, UCSTATE_SELECTED);
		}
		setNodeFocus(nodeIndex);
	}
	return nodeIndex;
}
int folderViewUC::getCols() const
{
	listHeaderUC *pHeader = getHeader();
	assert(pHeader);
	if(!pHeader) return 0;
	return pHeader->getCount();
}
int folderViewUC::getRows() const
{
	return getCount();
}

listElementUC* folderViewUC::findElement(POINT pt) const
{
	listElementUC *pelement = nullptr;
	for (int idx = 0; idx < m_pList->getCount(); ++idx)
	{
		pelement = static_cast<listElementUC *>(m_pList->getItemAt(idx));
		if (pelement && pelement->ptIn(pt))
		{
			return pelement;
		}
	}
	return nullptr;
}


}	// end wuc