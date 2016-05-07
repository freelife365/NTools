#pragma once
#include "db/dbtype.h"
// �б�ؼ��еĶ�����
#define DEF_ITEM_TEXT_ALL				L"ȫ��"
#define DEF_ITEM_TEXT_NONE				L"��"
#define DEF_ITEM_TEXT_NULL				L"Ϊ��"
// ����\�޸�\ɾ���еĳ�������
#define DEF_COMMAND_TEXT_SAVE						L"����"
#define DEF_COMMAND_TEXT_ADD						L"����"
#define DEF_COMMAND_TEXT_MODIFY						L"�޸�"
#define DEF_COMMAND_TEXT_DELETE						L"ɾ��"
// ���ؼ�IDת���ɶ���
#define DEF_TO_COMMBOX(ctrlID)						(CComboBox *)GetDlgItem(ctrlID)
// ���ť
#define DEF_COMMAND_INIT(add,modify,del)			GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(FALSE);\
													GetDlgItem(del)->EnableWindow(FALSE);
#define DEF_COMMAND_ADD_READY(add,modify,del)		GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_SAVE);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(FALSE);\
													GetDlgItem(del)->EnableWindow(FALSE);
#define DEF_COMMAND_ADD_AFTER(add,modify,del)		GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(FALSE);\
													GetDlgItem(del)->EnableWindow(FALSE);
#define DEF_COMMAND_MODIFY_READY(add,modify,del)	GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_SAVE);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(TRUE);\
													GetDlgItem(del)->EnableWindow(TRUE);
#define DEF_COMMAND_MODIFY_AFTER(add,modify,del)	GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(TRUE);\
													GetDlgItem(del)->EnableWindow(TRUE);
#define DEF_COMMAND_DELETE_AFTER(add,modify,del)	GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(FALSE);\
													GetDlgItem(del)->EnableWindow(FALSE);
#define DEF_COMMAND_SELECT(add,modify,del)			GetDlgItem(add)->SetWindowText(DEF_COMMAND_TEXT_ADD);\
													GetDlgItem(modify)->SetWindowText(DEF_COMMAND_TEXT_MODIFY);\
													GetDlgItem(del)->SetWindowText(DEF_COMMAND_TEXT_DELETE);\
													GetDlgItem(add)->EnableWindow(TRUE);\
													GetDlgItem(modify)->EnableWindow(TRUE);\
													GetDlgItem(del)->EnableWindow(TRUE);
// Ribbon ��ť
#define DEF_RIBBON_INIT(add,modify,del)				((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(FALSE);\
													((CMFCRibbonButton*)(del))->SetData(FALSE);
#define DEF_RIBBON_ADD_READY(add,modify,del)		((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_SAVE);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(FALSE);\
													((CMFCRibbonButton*)(del))->SetData(FALSE);
#define DEF_RIBBON_ADD_AFTER(add,modify,del)		((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(FALSE);\
													((CMFCRibbonButton*)(del))->SetData(FALSE);
#define DEF_RIBBON_MODIFY_READY(add,modify,del)		((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_SAVE);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(TRUE);\
													((CMFCRibbonButton*)(del))->SetData(TRUE);
#define DEF_RIBBON_MODIFY_AFTER(add,modify,del)		((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(TRUE);\
													((CMFCRibbonButton*)(del))->SetData(TRUE);
#define DEF_RIBBON_DELETE_AFTER(add,modify,del)		((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(FALSE);\
													((CMFCRibbonButton*)(del))->SetData(FALSE);
#define DEF_RIBBON_SELECT(add,modify,del)			((CMFCRibbonButton*)(add))->SetText(DEF_COMMAND_TEXT_ADD);\
													((CMFCRibbonButton*)(modify))->SetText(DEF_COMMAND_TEXT_MODIFY);\
													((CMFCRibbonButton*)(del))->SetText(DEF_COMMAND_TEXT_DELETE);\
													((CMFCRibbonButton*)(add))->SetData(TRUE);\
													((CMFCRibbonButton*)(modify))->SetData(TRUE);\
													((CMFCRibbonButton*)(del))->SetData(TRUE);
namespace acm {

class CNControl
{
private:
	CNControl(void) {}
	virtual ~CNControl(void) {}
public:
	/*****************************************************************************************
		<< --- CNControl::setFocus		2013-11-19 --- >>
		˵����ʹָ���ؼ��õ����
		������
		vpwnd	= �ؼ���������
		vctrlid	= �õ����Ŀؼ�ID
		����ֵ��
	*****************************************************************************************/
	static void setFocus(CWnd* vpwnd, int vctrlid)
	{
		if(vpwnd != NULL && vctrlid != 0)
		{
			CWnd* pwnd = vpwnd->GetDlgItem(vctrlid);
			if(pwnd != NULL)
			{
				pwnd->PostMessage(EM_SETSEL, 0, -1);
				pwnd->SetFocus();
			}
		}
	}
	/*****************************************************************************************
		<< --- CNControl::setReadOnly		2013-11-19 --- >>
		˵�����趨�ؼ���ֻ������
		������
		vpwnd	= �ؼ���������
		veditid	= �õ����Ŀؼ�ID
		visonlyread	= �Ƿ�ֻ��
		����ֵ��
	*****************************************************************************************/
	static void setReadOnly(CWnd* vpwnd, UINT veditid, bool visonlyread)
	{
		if(vpwnd == NULL || veditid <= 0)
			return;
		vpwnd->GetDlgItem(veditid)->SendMessage(EM_SETREADONLY, visonlyread, 0);
	}
	/*****************************************************************************************
		<< --- CNControl::getIDFromBox		2013-11-19 --- >>
		˵��������������õ���Ŀ��ID����
		������
		pcbo	= ComboBox�ؼ�
		vindex	= ComboBox������
		����ֵ���ɹ�=ComboBox��������Ӧ��ItemDataֵ��ʧ��=0
	*****************************************************************************************/
	static long getIDFromBox(CComboBox *pcbo, long vindex, bool isExtSet = false)
	{
		assert(isExtSet == false);
		assert(pcbo);
		if (!isExtSet)
		{
			return pcbo->GetItemData(vindex);
		}
		gtc::entry *pitem = getDataFromBox(pcbo, vindex);
		if(pitem)
			return pitem->getID();
		return 0;
	}
	static gtc::entry* getDataFromBox(CComboBox *pcbo, long vindex)
	{
		if(pcbo == nullptr || vindex < 0 || vindex >= pcbo->GetCount())
			return nullptr;
		gtc::entry *pitem = nullptr;
		try
		{
			pitem = (gtc::entry*)pcbo->GetItemData(vindex);
		}
		catch(...)
		{
			pitem = nullptr;
			gtc::app::lastError();
		}
		return pitem;
	}

	/*****************************************************************************************
		<< --- CNControl::getIndexFromBox		2013-11-19 --- >>
		˵����������Ŀ��ID���ݣ��õ��������
		������
		pcbo	= ComboBox�ؼ�
		vid		= ComboBox��ItemDataֵ
		����ֵ���ɹ�=ComboBox��������Ӧ��ItemDataֵ��ʧ��=-1
	*****************************************************************************************/
	static long getIndexFromBox(CComboBox *pcbo, long vid, bool isExtSet = false)
	{
		assert(isExtSet == false);
		assert(pcbo);
		if(pcbo == nullptr)
			return -1;
		long index = -1;
		gtc::entry *pitem = nullptr;
		try
		{
			for(int idx = 0; idx < pcbo->GetCount(); idx++)
			{
				if (isExtSet)
				{
					if((pitem = (gtc::entry*)pcbo->GetItemData(idx)) && pitem->getID() == vid)
					{
						index = idx;
						break;
					}
				}
				else
				{
					if (pcbo->GetItemData(idx) == vid)
					{
						index = idx;
						break;
					}
				}
			}
		}
		catch(...)
		{
			index = -1;
			gtc::app::lastError();
		}

		return index;
	}
	static long getIndexFromBox(HWND hWnd, int cboID, long vid, bool isExtSet = false)
	{
		assert(hWnd);
		long idx = -1;
		CComboBox cbo;
		cbo.Attach(GetDlgItem(hWnd, cboID));
		idx = getIndexFromBox(&cbo, vid, isExtSet);
		cbo.Detach();
		return idx;
	}
	/*****************************************************************************************
		<< --- CNControl::setDataComboBox		2013-11-19 --- >>
		˵�����趨ComboBox�ؼ�������
		������
		pcbo		= ComboBox�ؼ�
		����ֵ���ɹ�=ComboBox����Ŀ������ʧ��=-1
	*****************************************************************************************/
	static void freeDataFromBox(CComboBox *pcbo)
	{
		if(pcbo == nullptr || pcbo->GetCount() <= 0)
			return ;
		//gtc::entry *pitem = nullptr;
		try
		{
			for(int idx = 0; idx < pcbo->GetCount(); idx++)
			{
				delete (gtc::entry*)pcbo->GetItemData(idx);
			}
			pcbo->ResetContent();
		}
		catch(...)
		{
			gtc::app::lastError();
		}
	}
	static void freeDataFromBox(CMFCRibbonComboBox *pcbo)
	{
		if(pcbo == nullptr || pcbo->GetCount() <= 0)
			return ;
		//gtc::entry *pitem = nullptr;
		try
		{
			for(int idx = 0; idx < pcbo->GetCount(); idx++)
			{
				delete (gtc::entry*)pcbo->GetItemData(idx);
			}
			pcbo->RemoveAllItems();
		}
		catch(...)
		{
			gtc::app::lastError();
		}
	}

	/*****************************************************************************************
		<< --- CNControl::setDataBox		2013-11-19 --- >>
		˵�����趨ComboBox�ؼ�������
		������
		pcbo		= ComboBox�ؼ�
		vdata		= ComboBox������
		����ֵ���ɹ�=ComboBox����Ŀ������ʧ��=-1
	*****************************************************************************************/
	static void setDataBox(CComboBox *pcbo, const gtc::entry::PtrArrayType &vdata)
	{
		if(pcbo == nullptr)
			return ;

		try
		{
			// 
			freeDataFromBox(pcbo);

			// 
			gtc::entry::PtrArrayType::const_iterator iter, end = vdata.cend();
			for(iter = vdata.cbegin(); iter != end; ++iter)
			{
				pcbo->SetItemData(pcbo->AddString((*iter)->getTitle().c_str()), DWORD_PTR(*iter));
			}

		}
		catch(...)
		{
			gtc::app::lastError();
		}

	}
	static void setDataBox(CMFCRibbonComboBox *pcbo, const gtc::entry::PtrArrayType &vdata)
	{
		if(!pcbo)
			return ;

		try
		{
			freeDataFromBox(pcbo);

			// 
			gtc::entry::PtrArrayType::const_iterator iter, end = vdata.cend();
			for(iter = vdata.cbegin(); iter != end; ++iter)
			{
				pcbo->AddItem((*iter)->getTitle().c_str(), DWORD_PTR(*iter));
			}

		}
		catch(...)
		{
			gtc::app::lastError();
		}

	}

	/*****************************************************************************************
		<< --- CNControl::setDataBox		2013-11-19 --- >>
		˵�����趨ComboBox�ؼ�������
		������
		pcbo		= ComboBox�ؼ�ID
		vdata		= ComboBox������
		����ֵ���ɹ�=ComboBox����Ŀ������ʧ��=-1
	*****************************************************************************************/
	static void setDataBox(CComboBox *pcbo, const gtc::entry vdata[], const int32_t vdatacount)
	{
		if(!pcbo)
			return ;

		try
		{
			// 
			pcbo->ResetContent();
			// 
			for(int idx = 0; idx < vdatacount; ++idx)
			{
				pcbo->SetItemData(pcbo->AddString(vdata[idx].getTitle().c_str()), DWORD_PTR(&vdata[idx]));
			}
		}
		catch(...)
		{
			gtc::app::lastError();
		}
	}
	/*****************************************************************************************
		<< --- CNControl::isCheckRadioBtn		2013-11-19 --- >>
		˵�������Radio�ؼ��Ƿ�ѡ��
		������
		vpwnd	= �ؼ���������
		vchkid	= �ؼ�ID
		����ֵ��true=ѡ�У�false=δѡ��
	*****************************************************************************************/
	static bool isCheckRadioBtn(CWnd* vpwnd, UINT vchkid)
	{
		if(vpwnd == NULL || vchkid <= 0)
			return false;
		bool ischk = false;
		CButton btn;
		try
		{
			btn.Attach(vpwnd->GetDlgItem(vchkid)->m_hWnd);
			ischk = btn.GetCheck() != 0;
		}
		catch(...)
		{
			ischk = false;
			gtc::app::lastError();
		}
		if(btn.m_hWnd != NULL)
			btn.Detach();

		return ischk;
	}
	/*****************************************************************************************
		<< --- CNControl::setCheckRadioBtn		2013-11-19 --- >>
		˵�����趨Radio�ؼ�ѡ�б��
		������
		vpwnd	= �ؼ���������
		vchkid	= �ؼ�ID
		vischked= �Ƿ�ѡ��
		����ֵ��
	*****************************************************************************************/
	static void setCheckRadioBtn(CWnd* vpwnd, UINT vchkid, bool vischked)
	{
		if(vpwnd == NULL || vchkid <= 0)
			return ;
		CButton btn;
		try
		{
			btn.Attach(vpwnd->GetDlgItem(vchkid)->m_hWnd);
			btn.SetCheck(vischked?BST_CHECKED:BST_UNCHECKED);
			btn.Detach();
		}
		catch(...)
		{
			gtc::app::lastError();
		}
		if(btn.m_hWnd != NULL)
			btn.Detach();
	}
/*
public:
	// ��տؼ�����
	virtual void doClearInputBox(void);
	// ��֤���ڱ����������
	virtual bool doUpdateParams(db::parameter::ArrayType &vparams);
	// �ı�ؼ�״̬
	virtual void doChangeInputStatus(bool venable);
	// ��ʼ���ؼ�������
	virtual void doInitData(void);
	// ˢ����������
	virtual void doRefreshData(void);

	// ����ձ�����ؼ�����
	virtual void doClearMustReinputBox(void);
	// ���ÿؼ�����
	virtual void doSetBoxAndData(db::dataSet &rst);
	// ���ݿؼ����ݵõ�sql���Ĳ�ѯ����
	virtual CString doGetQueryWhere(void);
*/
/*
// ��տؼ�����
void CDlgHandkey::doClearInputBox(void)
{

	UpdateData(FALSE);
}
// ��֤���ڱ����������
bool CDlgHandkey::doUpdateParams(db::parameter::ArrayType &vparams)
{
	UpdateData(TRUE);

	return true;
}
// �ı�ؼ�״̬
void CDlgHandkey::doChangeInputStatus(bool venable)
{
}
// ��ʼ���ؼ�������
void CDlgHandkey::doInitData(void)
{
}
// ˢ����������
void CDlgHandkey::doRefreshData(void)
{
}


////////////////////////////////////
// ���ݿؼ����ݵõ�sql���Ĳ�ѯ����
CString CDlgHandkey::doGetQueryWhere(void)
{	
	return L"";	
}
// ����ձ�����ؼ�����
void CDlgHandkey::doClearMustReinputBox(void)
{

	UpdateData(FALSE);
}
// ���ÿؼ�����
void CDlgHandkey::doSetBoxAndData(db::dataSet &rst)
{
	
	UpdateData(FALSE);
}
*/
};












}