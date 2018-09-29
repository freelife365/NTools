#pragma once
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <atltypes.h>

// �ؼ����봰��߽��Ĭ�Ͼ���
#define CONTROL_INDIALOG_SPACE	15

namespace acm {
// �ں�box������area�ڲ���ζ���
enum BEAlignBoxInArea{
		BEAlignBoxInAreaLeftTop			= 0x0011,		// ͼƬ���������
		BEAlignBoxInAreaLeftCenter		= 0x0012,		// ͼƬ�м������
		BEAlignBoxInAreaLeftBottom		= 0x0014,		// ͼƬ�ײ������
		BEAlignBoxInAreaCenterTop		= 0x0021,		// ͼƬ��������
		BEAlignBoxInAreaCenterCenter	= 0x0022,		// ͼƬ�м����
		BEAlignBoxInAreaCenterBottom	= 0x0024,		// ͼƬ�ײ�����
		BEAlignBoxInAreaRightTop		= 0x0041,		// ͼƬ�����Ҷ���
		BEAlignBoxInAreaRightCenter		= 0x0042,		// ͼƬ�м��Ҷ���
		BEAlignBoxInAreaRightBottom		= 0x0044,		// ͼƬ�ײ��Ҷ���
		BEAlignBoxInAreaReally			= 0x1000,		// VSP3.1����ͼƬ�ߴ粻�������ʾ		BP_Auto			= 0x1000,		// ������
		BEAlignBoxInAreaExtend			= 0x2000,		// VSP3.1ͼƬ����ȫ����ʾ
		BEAlignBoxInAreaFull			= 0xFFFF		// ƽ��
};
/*****************************************************************************************
	CMLayout
	˵�������ִ����ؼ�
	���ڣ�2013-11-19
*****************************************************************************************/
class CMLayout
{
private:

	CMLayout(void) {	}

	virtual ~CMLayout(void)	{	}
public:
	/*****************************************************************************************
		<< --- CMLayout::reviseArea		2013-11-19 --- >>
		˵������ָ������varea�����������vaspectratio��������
		������
		varea			= ��ת��������
		vaspectratio	= �������ֵ
		����ֵ�������������
	*****************************************************************************************/
	CRect reviseArea(const CRect& varea, float vaspectratio)
	{
		CRect rect = varea;
		long boxH, boxW, w, h;
		boxH = rect.Height();
		boxW = rect.Width();

		if((float(boxW)/boxH) > vaspectratio)
		{ 
			h = boxH;
			w = long(vaspectratio * h);
			rect.DeflateRect((boxW - w)/2, 0);
		}
		else
		{
			w = boxW;
			h = long(vaspectratio * w);
			rect.DeflateRect(0, (boxH - h)/2);
		}

		return rect;
	}
	/*****************************************************************************************
		<< --- CMLayout::computerImageArea		2013-11-19 --- >>
		˵������ָ������varea�����������vaspectratio��������
		������
		varea			= ��ת��������
		vaspectratio	= �������ֵ
		����ֵ�������������
	*****************************************************************************************/
	/* ����ͼƬ�ߴ��Լ�ͼƬ�ڹ�������ڷŵ�λ�ã�����һ��ͼƬ��Ч���򣬲�������һ��Ч���� */
	static CRect computerImageArea(const CRect& varea, const CSize& vbox, BEAlignBoxInArea valign)
	{
		int signx = 1, signy = 1;
		if(varea.Width() < 0)
			signx = -1;
		if(varea.Height() < 0)
			signy = -1;
		int imgw = vbox.cx, imgh = vbox.cy;
		CRect area(varea.left, varea.top, varea.left + imgw*signx, varea.top + imgh*signy);
		float fbox, farea; // ��߱�
		fbox = float(varea.Width()*signx)/varea.Height()*signy;
		farea = float(imgw)/imgh;
		if(fbox > farea && imgh > abs(varea.Height()))
		{	// ͼƬ�Ȼ�������խ���ұȻ��������
			area.SetRect(area.left, area.top, area.left + int(abs(varea.Height())*farea)*signx, area.top + varea.Height());
		}
		else if(fbox < farea && imgw > abs(varea.Width()))
		{	// ͼƬ�Ȼ���������ұȻ��������
			area.SetRect(area.left, area.top, area.left + varea.Width(), area.top + int(abs(varea.Width())/farea)*signy);
		}
		long spw = (abs(varea.Width())-abs(area.Width()))/2;
		long sph = (abs(varea.Height())-abs(area.Height()))/2;
		switch(valign)
		{
		case BEAlignBoxInAreaLeftTop:		//= 0,		// ͼƬ���������
			break;
		case BEAlignBoxInAreaLeftCenter:		//= 1,		// ͼƬ�м������
			area.OffsetRect(0, sph*signy);
			break;
		case BEAlignBoxInAreaLeftBottom:		//= 2,		// ͼƬ�ײ������
			area.OffsetRect(0, sph*2*signy);
			break;
		case BEAlignBoxInAreaCenterTop:		//= 3,		// ͼƬ��������
			area.OffsetRect(spw*signx, 0);
			break;
		case BEAlignBoxInAreaCenterCenter:	//= 4,		// ͼƬ�м����
			area.OffsetRect(spw*signx, sph*signy);
			break;
		case BEAlignBoxInAreaCenterBottom:	//= 5,		// ͼƬ�ײ�����
			area.OffsetRect(spw*signx, sph*2*signy);
			break;
		case BEAlignBoxInAreaRightTop:		//= 6,		// ͼƬ�����Ҷ���
			area.OffsetRect(spw*2*signx, 0);
			break;
		case BEAlignBoxInAreaRightCenter:	//= 7,		// ͼƬ�м��Ҷ���
			area.OffsetRect(spw*2*signx, sph*signy);
			break;
		case BEAlignBoxInAreaRightBottom:	//= 8,		// ͼƬ�ײ��Ҷ���
			area.OffsetRect(spw*2*signx, sph*2*signy);
			break;
		case BEAlignBoxInAreaReally:			//			// 
			{
				area = varea;
				long boxH, boxW, w, h;
				boxH = abs(varea.Height());
				boxW = abs(varea.Width());

				if((float(boxW)/boxH) > (float(imgw)/imgh))
				{ 
					h = boxH;
					w = long(float(imgw)/imgh * h);
					area.DeflateRect((boxW - w)/2, 0);
				}
				else
				{
					w = boxW;
					h = long(float(imgh)/imgw * w);
					area.DeflateRect(0, (boxH - h)/2);
				}
			}
			break;
		case BEAlignBoxInAreaExtend:
			{
				if(fbox > farea)
				{	// ͼƬ�Ȼ�������խ���ұȻ��������
					area.SetRect(area.left, area.top, area.left + int(abs(varea.Height())*farea)*signx, area.top + varea.Height());
					area.OffsetRect((abs(varea.Width())-abs(area.Width()))/2*signx, 0);
				}
				else if(fbox < farea)
				{	// ͼƬ�Ȼ���������ұȻ��������
					area.SetRect(area.left, area.top, area.left + varea.Width(), area.top + int(abs(varea.Width())/farea)*signy);
					area.OffsetRect(0, (abs(varea.Height())-abs(area.Height()))/2*signy);
				}
				else
					area = varea;
				//long boxH, boxW, imgH, imgW, w, h;
				//boxH = abs(varea.Height());
				//boxW = abs(varea.Width());

				//imgH = imgh;
				//imgW = vimgW;
				//if((float(boxW)/boxH) > (float(imgW)/imgH))
				//{ 
				//	h = boxH;
				//	w = long(float(imgW)/imgH * h);
				//	area.DeflateRect((boxW - w)/2, 0);
				//}
				//else
				//{
				//	w = boxW;
				//	h = long(float(imgH)/imgW * w);
				//	area.DeflateRect(0, (boxH - h)/2);
				//}
			}
			break;
		case BEAlignBoxInAreaFull:			//= 10		// ƽ��
			area = varea;
			break;
		default:
			{
				long val = valign;
				long offx = 0, offy = 0;
				if(fbox > farea)
				{	// ͼƬ�Ȼ�������խ���ұȻ��������
					area.SetRect(area.left, area.top, area.left + int(abs(varea.Height())*farea)*signx, area.top + varea.Height());
				}
				else if(fbox < farea)
				{	// ͼƬ�Ȼ���������ұȻ��������
					area.SetRect(area.left, area.top, area.left + varea.Width(), area.top + int(abs(varea.Width())/farea)*signy);
				}
				spw = (abs(varea.Width())-abs(area.Width()))/2;
				sph = (abs(varea.Height())-abs(area.Height()))/2;
				if(val & 0x1000)
				{	// �� BP_Auto
					if(val & 0x0020)
						offx = spw;
					else if(val & 0x0040)
						offx = spw * 2;
					if(val & 0x0002)
						offy = sph;
					else if(val & 0x0004)
						offy = sph * 2;

					area.OffsetRect(offx*signx, offy*signy);
				}
			}
			break;
		}
		return area;
	}
	/* �ؼ����ֺ��� */

	/*****************************************************************************************
		<< --- CMLayout::alignToLeft		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��ʹ����vtargetid�ؼ������
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect alignToLeft(CWnd* vpwnd, int vmoveid, int vtargetid)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);
		rect.OffsetRect(rt.left - rect.left, 0);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	
	/*****************************************************************************************
		<< --- CMLayout::alignToRight		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��ʹ����vtargetid�ؼ��Ҷ���
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect alignToRight(CWnd* vpwnd, int vmoveid, int vtargetid)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);
		rect.OffsetRect(rt.right - rect.right, 0);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	/*****************************************************************************************
		<< --- CMLayout::alignToTop		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��ʹ����vtargetid�ؼ��϶���
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect alignToTop(CWnd* vpwnd, int vmoveid, int vtargetid)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);
		rect.OffsetRect(0, rt.top - rect.top);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	/*****************************************************************************************
		<< --- CMLayout::alignToBottom		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��ʹ����vtargetid�ؼ��¶���
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect alignToBottom(CWnd* vpwnd, int vmoveid, int vtargetid)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);
		rect.OffsetRect(0, rt.bottom - rect.bottom);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	/*****************************************************************************************
		<< --- CMLayout::moveToLeft		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��vtargetid�ؼ�����ߣ����߼��vspace
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ���ļ��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToLeft(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.OffsetRect(rt.left - rect.left - rect.Width()-vspace, 0);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToRight		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��vtargetid�ؼ����ұߣ����߼��vspace
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ���ļ��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToRight(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.OffsetRect(rt.right - rect.right + rect.Width()+vspace, 0);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToTop		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��vtargetid�ؼ����ϱߣ����߼��vspace
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ���ļ��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToTop(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.OffsetRect(0, rt.top - rect.top - rect.Height()-vspace);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToBottom		2013-11-19 --- >>
		˵�����ƶ��ؼ�vmoveid��vtargetid�ؼ����±ߣ����߼��vspace
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ���ļ��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToBottom(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.OffsetRect(0, rt.bottom - rect.bottom + rect.Height() + vspace);

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	// 
	/*****************************************************************************************
		<< --- CMLayout::expandLeft		2013-11-19 --- >>
		˵������չ�ؼ�vmoveid����ߵ�����vtargetid�ؼ�vspace��
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ��ı߽��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandLeft(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.left = rt.right + vspace;

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandRight		2013-11-19 --- >>
		˵������չ�ؼ�vmoveid���ұߵ�����vtargetid�ؼ�vspace��
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ��ı߽��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandRight(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.right = rt.left - vspace;

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandTop		2013-11-19 --- >>
		˵������չ�ؼ�vmoveid���ϱߵ�����vtargetid�ؼ�vspace��
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ��ı߽��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandTop(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.top = rt.bottom + vspace;

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandBottom		2013-11-19 --- >>
		˵������չ�ؼ�vmoveid�ĵױߵ�����vtargetid�ؼ�vspace��
		������
		vpwnd		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vtargetid	= Ŀ��ؼ�
		vspace		= ���ؼ��ı߽��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandBottom(CWnd* vpwnd, int vmoveid, int vtargetid, int vspace)
	{
		CRect rt, rect;
		if(vpwnd == NULL)
			return rect;
		vpwnd->GetDlgItem(vmoveid)->GetWindowRect(rect);
		vpwnd->GetDlgItem(vtargetid)->GetWindowRect(rt);

		rect.bottom = rt.top - vspace;

		vpwnd->ScreenToClient(rect);
		vpwnd->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToLeftborderInDialog		2013-11-19 --- >>
		˵�������ؼ�vmoveid�ƶ������봰��pdialog��߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ര��߽����
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToLeftborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		int offx = 0, offy = 0;
		pdialog->GetWindowRect(rect);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rt);

		offx = rect.left - rt.left + vspace;
		rt.OffsetRect(offx, offy);
	
		pdialog->ScreenToClient(rt);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rt);
		return rt;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToRightborderInDialog		2013-11-19 --- >>
		˵�������ؼ�vmoveid�ƶ������봰��pdialog�ұ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ര��߽����
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToRightborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		int offx = 0, offy = 0;
		pdialog->GetWindowRect(rect);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rt);
		offx = rect.right - rt.right - vspace;
		rt.OffsetRect(offx, offy);
	
		pdialog->ScreenToClient(rt);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rt);
		return rt;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToTopborderInDialog		2013-11-19 --- >>
		˵�������ؼ�vmoveid�ƶ������봰��pdialog�ϱ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ര��߽����
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToTopborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		int offx = 0, offy = 0;
		pdialog->GetWindowRect(rect);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rt);

		offy = rect.top - rt.top + vspace + 25;	// ���������ռ25
		rt.OffsetRect(offx, offy);
	
		pdialog->ScreenToClient(rt);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rt);
		return rt;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::moveToBottomborderInDialog		2013-11-19 --- >>
		˵�������ؼ�vmoveid�ƶ������봰��pdialog�ױ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ര��߽����
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect moveToBottomborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		int offx = 0, offy = 0;
		pdialog->GetWindowRect(rect);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rt);

		offy = rect.bottom - rt.bottom - vspace;
		rt.OffsetRect(offx, offy);
	
		pdialog->ScreenToClient(rt);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rt);
		return rt;
	}

	// 
	/*****************************************************************************************
		<< --- CMLayout::expandToLeftborderInDialog		2013-11-19 --- >>
		˵������չvmoveid����߽絽������߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ؼ��߽��봰��߽��ľ���
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandToLeftborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		pdialog->GetWindowRect(rt);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rect);

		rect.left = rt.left + vspace;
	
		pdialog->ScreenToClient(rect);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandToRightborderInDialog		2013-11-19 --- >>
		˵������չvmoveid����߽絽�����ұ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ؼ��߽��봰��߽��ľ���
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandToRightborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		pdialog->GetWindowRect(rt);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rect);

		rect.right = rt.right - vspace;
	
		pdialog->ScreenToClient(rect);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandToTopborderInDialog		2013-11-19 --- >>
		˵������չvmoveid����߽絽�����ϱ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ؼ��߽��봰��߽��ľ���
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandToTopborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		pdialog->GetWindowRect(rt);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rect);

		rect.top = rt.top + vspace + 25;	// 25�Ǵ���������ĸ߶�
	
		pdialog->ScreenToClient(rect);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}
	// 
	/*****************************************************************************************
		<< --- CMLayout::expandToBottomborderInDialog		2013-11-19 --- >>
		˵������չvmoveid����߽絽����ױ߽�vspace��
		������
		pdialog		= �ؼ���������
		vmoveid		= ���ƶ��Ŀؼ�
		vspace		= �ؼ��߽��봰��߽��ľ���
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect expandToBottomborderInDialog(CWnd* pdialog, int vmoveid, int vspace)
	{
		CRect rect, rt;
		if(pdialog == NULL)
			return rect;
		pdialog->GetWindowRect(rt);
		pdialog->GetDlgItem(vmoveid)->GetWindowRect(rect);

		rect.bottom = rt.bottom - vspace;
	
		pdialog->ScreenToClient(rect);
		pdialog->GetDlgItem(vmoveid)->MoveWindow(rect);
		return rect;
	}

	/*****************************************************************************************
		<< --- CMLayout::fullZoomDialog		2013-11-19 --- >>
		˵���������趨�Ի���Ĵ�С��vzoomx,vzoomy������������ŵİٷֱ�
		������
		pwnd		= ���ı䴰��
		vzoomx		= ˮƽ���ű���[0--100],0=����ԭ��100=������ͬ��
		vzoomy		= ��ֱ���ű���[0--100],0=����ԭ�ߣ�100=������ͬ��
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect fullZoomDialog(CWnd* pwnd, uint16_t vzoomx, uint16_t vzoomy)
	{
		CRect rect, rtdesk;
		rect.SetRectEmpty();
		if(pwnd == NULL)
			return rect;
		if(vzoomx > 100)
			vzoomx = 100;
		if(vzoomy > 100)
			vzoomy = 100;
		pwnd->GetWindowRect(rect);
		CWnd wnd;
		wnd.Attach(::GetDesktopWindow());
		wnd.GetWindowRect(rtdesk);
		wnd.Detach();
		long x = long(rtdesk.Width()*((100-vzoomx)/100.0f));
		long y = long(rtdesk.Height()*((100-vzoomy)/100.0f));
		if(vzoomx == 0)
			x = rtdesk.Width() - rect.Width();
		if(vzoomy == 0)
			y = rtdesk.Height() - rect.Height();
		rect = rtdesk;
		rect.right = rect.left + rect.right - x;
		rect.bottom = rect.top + rect.bottom - y;
		rect.OffsetRect(x/2, y/2);
		pwnd->MoveWindow(rect);
	
		pwnd->ScreenToClient(rect);
		return rect;
	}

	/*****************************************************************************************
		<< --- CMLayout::fullSizeDialog		2013-11-19 --- >>
		˵�����趨�Ի���Ĵ�С
		������
		pwnd		= ���ı䴰��
		vwidth		= �趨�Ŀ�ȣ�<=0ʱ����ԭ���
		vheight		= �趨�ĸ߶ȣ�<=0ʱ����ԭ�߶�
		����ֵ���ƶ����λ��
	*****************************************************************************************/
	static CRect fullSizeDialog(CWnd* pwnd, int vwidth, int vheight)
	{
		CRect rect;
		if(pwnd == NULL)
			return rect;
		pwnd->GetWindowRect(rect);
		long width = rect.Width();
		long height = rect.Height();
		if(vwidth > 0)
			width = vwidth;
		if(vheight > 0)
			height = vheight;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		pwnd->MoveWindow(rect);

		pwnd->ScreenToClient(rect);
		return rect;
	}
};

}