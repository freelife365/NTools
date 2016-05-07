#ifndef __WUCINC_H__
#define __WUCINC_H__
#pragma once

// ����ҪŶ��ָ����common�ؼ��İ汾������tooltip�޷���ȷʵ��
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "wucdef.h"				// baseobj.h  resourceManager.h  image.h  delegateBase.h

#include "windowWnd.h"			// 
#include "drawManager.h"			// 
#include "wucControl.h"
#include "wucContainer.h"
#include "wucBase.h"
#include "wucMenu.h"
#include "wucCombo.h"
#include "wucAnimate.h"
#include "wucWkeWebkit.h"
#include "wucWnd.h"				// windowWnd.h
#include "wucBubbleWnd.h"
#include "dialogWnd.h"



namespace wuc {
//////////////////////////////////////////////////////////////////////////////////////////
// 2015-12-11		kp007	�ؼ�����

//////////////////////////////////////////////////////////////////////////////////////////
// 2015-12-25		kp007	ĳ�������������λ��
//	1	2	3
//	4	5	6
//	7	8	9
enum BEItemPosition
{
	BEItemPositionTopLeft			= 1,
	BEItemPositionTopCenter			,
	BEItemPositionTopRight			,
	BEItemPositionCenterLeft		,
	BEItemPositionCenterCenter		,
	BEItemPositionCenterRight		,
	BEItemPositionBottomLeft		,
	BEItemPositionBottomCenter		,
	BEItemPositionBottomRight	
};

/****************************************************************************************
	<< --- 	2015-12-25	kp007 --- >>         
	˵������ָ������ָ��λ�ò���һ���ؼ���������
	������
		pParent		= ������
		szControl	= Ҫ���ɿؼ���С
		oriPos		= ���ɿؼ���Ը�������λ��
	����ֵ���������ɵĿؼ�
*****************************************************************************************/
extern horizontalLayoutUC* layoutControl(verticalLayoutUC *pParent, SIZE szControl, BEItemPosition oriPos);
extern horizontalLayoutUC* layoutControl(horizontalLayoutUC *pParent, SIZE szControl, BEItemPosition oriPos);


/****************************************************************************************
	<< --- layoutCenterIcon	2015-12-11	kp007 --- >>         
	˵������ָ������������ʾͼƬ
	������
		pParent		= ͼ����ʾ������
		pIconKey	= ͼ���ڼ����е�key��
	����ֵ������ͼƬ��������ͼƬ�����ڷ���null
*****************************************************************************************/
extern horizontalLayoutUC* layoutCenterIcon(verticalLayoutUC *pParent, gtc::image *pImage);
extern horizontalLayoutUC* layoutCenterIcon(verticalLayoutUC *pParent, const wchar_t *pIconKey);
extern horizontalLayoutUC* layoutCenterIcon(horizontalLayoutUC *pParent, const wchar_t *pIconKey);

/****************************************************************************************
	<< --- layoutGrid	2015-12-25	kp007 --- >>         
	˵��������һ������������������ÿ����Ԫ��
	������
		pParent		= ������
		vecRow		= ������и�
		vecCol		= ������п�
	����ֵ������Ԫ�񼯺ϣ�����horizontalLayoutUC*����
*****************************************************************************************/
extern HContainerArrayType layoutGrid(verticalLayoutUC *pParent, IntArrayType &vecRow, IntArrayType &vecCol);
extern HContainerArrayType layoutGrid(horizontalLayoutUC *pParent, IntArrayType &vecRow, IntArrayType &vecCol);
extern HContainerArrayType layoutGrid(verticalLayoutUC *pParent, int rows, int cols, int rowCY, int colCX);
extern HContainerArrayType layoutGrid(horizontalLayoutUC *pParent, int rows, int cols, int rowCY, int colCX);
// ��ż��ͬ
extern HContainerArrayType layoutGrid(verticalLayoutUC *pParent, int rows, int cols, int rowOddCY, int rowEvenCY, int colOddCX, int colEvenCX);
extern HContainerArrayType layoutGrid(horizontalLayoutUC *pParent, int rows, int cols, int rowOddCY, int rowEvenCY, int colOddCX, int colEvenCX);
// ��ͬ���в�ͬ
extern HContainerArrayType layoutGrid(verticalLayoutUC *pParent, int rows, int rowCY, IntArrayType &vecCol);
extern HContainerArrayType layoutGrid(horizontalLayoutUC *pParent, int rows, int rowCY, IntArrayType &vecCol);


}	// wuc

#endif	// __WUCINC_H__
