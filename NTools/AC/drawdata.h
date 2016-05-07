#ifndef __DRAWDATA_H__
#define __DRAWDATA_H__
#pragma once
#include "generalobj.h"
namespace gtc {


///////////////////////////////////////////////////////////////////////////////////////////////////
// ������λ
static entry* gc_tbUnitWeight[] = {
	new entry(BEUnitWeightMilligram,	L"΢��"),
	new entry(BEUnitWeightGram,			L"��"),
	new entry(BEUnitWeightLiang,		L"��"),
	new entry(BEUnitWeightJing,			L"��"),
	new entry(BEUnitWeightKilogram,		L"����"),
	new entry(BEUnitWeightTon,			L"��")
};
///////////////////////////////////////////////////////////////////////////////////////////////////
// ʱ�䵥λ
//typedef enum BEUnitTime {
//	BEUnitTimeMillisecond = 1,			// ����
//	BEUnitTimeSecond,					// ��
//	BEUnitTimeMinute					// ��
//}BEUnitTime;
static entry* gc_tbUnitTime[] = {
	new entry(BEUnitTimeMillisecond,	L"����"),
	new entry(BEUnitTimeSecond,			L"��"),
	new entry(BEUnitTimeMinute,			L"��")
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- BEAlignMode���͵��ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbAlignMode[] = {
	new entry(BEAlignModeAllLeft,		L"���о���"),
	new entry(BEAlignModeAllRight,		L"���о���"),
	new entry(BEAlignModeAllCenter,		L"���о���"),
	new entry(BEAlignModeLeftTop,		L"��࿿��"),
	new entry(BEAlignModeLeftCenter,	L"��࿿��"),
	new entry(BEAlignModeLeftBottom,	L"��࿿��"),
	new entry(BEAlignModeCenterTop,		L"�м俿��"),
	new entry(BEAlignModeCenterCenter,	L"�м俿��"),
	new entry(BEAlignModeCenterBottom,	L"�м俿��"),
	new entry(BEAlignModeRightTop,		L"�Ҳ࿿��"),
	new entry(BEAlignModeRightCenter,	L"�Ҳ࿿��"),
	new entry(BEAlignModeRightBottom,	L"�Ҳ࿿��")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ������ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbPenStyle[] = {
	new entry(BEPenStyleSolid,		L"Solid"),
	new entry(BEPenStyleDash,		L"Dash"),		/* -------  */
	new entry(BEPenStyleDot,		L"Dot"),		/* .......  */
	new entry(BEPenStyleDashDot,	L"DashDot"),	/* _._._._  */
	new entry(BEPenStyleDashDotDot,	L"DashDotDot"),	/* _.._.._  */
	new entry(BEPenStyleCustom,		L"Custom"),
	new entry(BEPenStyleNull,		L"Null")

};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ��ˢ��ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbBrushStyle[] = {
	new entry(BEBrushStyleSolid,	L"SOLID"),
	new entry(BEBrushStyleNull,		L"NULL"),
	new entry(BEBrushStyleHatch,	L"HATCHED")//,	
	//entry(3,	L"PATTERN"),
	//entry(5,	L"DIBPATTERN"),
	//entry(6,	L"DIBPATTERNPT"),
	//entry(7,	L"PATTERN8X8"),
	//entry(8,	L"DIBPATTERN8X8")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ��ˢ��ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbBrushHatch[] = {
	new entry(BEHatchStyleHorizontal,		L"Horizontal"),			/* ----- */
	new entry(BEHatchStyleVertical,			L"Vertical"),			/* ||||| */
	new entry(BEHatchStyleForwardDiagonal,	L"ForwardDiagonal"),	/* \\\\\ */
	new entry(BEHatchStyleBackwardDiagonal,	L"BackwardDiagonal"),	/* ///// */
	new entry(BEHatchStyleCross,			L"Cross"),				/* +++++ */
	new entry(BEHatchStyleDiagonalCross,	L"DiagonalCross")		/* xxxxx */
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ֽ�ŷ�����ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbPaperOrientation[] = {
	new entry(1,	L"����"), 
	new entry(0,	L"����")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ҳ����ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbPageNoStyle[] = {
	new entry(0,	L"�հ�"), 
	new entry(1,	L"�ڣ�ҳ ��xҳ"),
	new entry(2,	L"����")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- CBItemGrid�б������ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbGridColCaptionStyle[] = {
	new entry(BEGridColCaptionStyleNone,	L"�ޱ���"), 
	new entry(BEGridColCaptionStyleSingle,	L"���б���"),
	new entry(BEGridColCaptionStyleDouble,	L"˫�б���")
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ��Ԫ��߿���ʽ���ֵ�����entry[]	2013-12-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////
static entry* gc_tbBoardStyle[] = {
	new entry(BEBorderStyleNone,		L"�ޱ߿�"), 
	new entry(BEBorderStyleTop,			L"�����ϱ߿�"),
	new entry(BEBorderStyleBottom,		L"�����±߿�"),
	new entry(BEBorderStyleLeft,		L"������߿�"),
	new entry(BEBorderStyleRight,		L"�����ұ߿�"),
	new entry(BEBorderStyleTopBottom,	L"�����±߿�"),
	new entry(BEBorderStyleLeftRight,	L"�����ұ߿�"),
	new entry(BEBorderStyleRectangle,	L"���α߿�")
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- ��Ԫ��߿���ʽ���ֵ�����entry[]	2015-08-06 --- >>
////////////////////////////////////////////////////////////////////////////////////////////////////////

}
#endif