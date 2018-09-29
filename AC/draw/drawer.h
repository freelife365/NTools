#pragma once
#include "AC/basedef.h"
#include "AC/drawdef.h"
#include "AC/baseobj.h"
#include "AC/image.h"
#if defined(_WIN32)
// GDI+
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#endif
namespace gtc{
/////////////////////////////////////////////////////////////////////////////////////////////////////
////	2015-1-22	
//static Gdiplus::Rect toGdiplus(const RECT &rt)
//{
//	Gdiplus::Rect rc(rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top);
//	return rc;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	drawer��		2014-06-21
//	��ͼ��ķ�װ
////////////////////////////////////////////////////////////////////////////////////////////////////////

class drawer
{
public:
	//drawer(HDC hdc);
	//~drawer();
	/*****************************************************************************************
		<< --- drawer::drawLine	2014-08-04 --- >>
		˵����������
		������
		grap	= ��ͼ����
		pen		= ����
		x1,y1	= �������
		x2,y2	= �յ�����
		����ֵ��
	*****************************************************************************************/
	static void drawLine(Gdiplus::Graphics &grap, const BSPen &pen, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
	/*****************************************************************************************
		<< --- drawer::fillRectangle	2014-06-28 --- >>
		˵�������ƾ��εı߿�
		������
		grap	= ��ͼ����
		brush	= �����ʽ
		rt		= �߿�����
		����ֵ��
	*****************************************************************************************/
	static void fillRectangle(Gdiplus::Graphics &grap, const BSBrush &brush, const rect &rt);
	static void fillRectangle(Gdiplus::Graphics &grap, DWORD clrbk, const rect &rt);

	/*****************************************************************************************
		<< --- drawer::fillRoundRectangle	2014-08-04 --- >>
		˵�������Բ�Ǿ�������
		������
		grap		= ��ͼ����
		brush		= �����ʽ
		rcArea		= �߿�����
		roundWidth	= Բ������Ŀ��
		roundHeight	= Բ������ĸ߶�
		����ֵ��
	*****************************************************************************************/
	static void fillRoundRectangle(Gdiplus::Graphics &grap, const BSBrush &brush, const rect &rcArea, int roundWidth, int roundHeight);

	/*****************************************************************************************
		<< --- drawer::rectBorder	2014-06-21 --- >>
		˵�������ƾ��εı߿�
		������
		grap	= ��ͼ����
		pen		= ����
		rt		= �߿�����
		style	= �߿���ʽ
		����ֵ��
	*****************************************************************************************/
	static void drawBorder(Gdiplus::Graphics &grap, const BSPen &pen, const rect &rt, int style = BEBorderStyleRectangle);
	/*****************************************************************************************
		<< --- drawer::drawRoundBorder	2014-06-21 --- >>
		˵�������ƾ��ε�Բ�Ǳ߿�
		������
		grap		= ��ͼ����
		pen			= ����
		rcArea		= �߿�����
		roundWidth	= Բ������Ŀ��
		roundHeight	= Բ������ĸ߶�
		����ֵ��
	*****************************************************************************************/
	static void drawRoundBorder(Gdiplus::Graphics &grap, const BSPen &pen, const rect &rcArea, int roundWidth, int roundHeight);

	/****************************************************************************************
		<< ---  measureString		2015-7-31 --- >>         
		˵���������ַ�������ʾ����
		������
			grap		= GDI+��������
			ptext		= ���������ı�
			rtArea		= width=0ʱ�����ı���ʾ��Ҫ�ĸ߶ȣ�height=0ʱ�����ı���ʾ��Ҫ�ĸ߶�
			hFont		= ����
			uiFormat	= �ı���ʾ��ʽ
		����ֵ��
	*****************************************************************************************/
	static rect measureString(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, HFONT hFont, UINT uiFormat, INT *pcodepointsFitted = NULL, INT *plinesFilled = NULL);
	static rect measureString(PCWSTR ptext, const rect &rtArea, HFONT hFont, UINT uiFormat, INT *pcodepointsFitted = NULL, INT *plinesFilled = NULL);
	
	/****************************************************************************************
		<< --- 	stringMeasure	2015-10-10 --- >>         
		˵��������ָ������������ʾ��Щ�ַ���������
		������
			grap		= GDI+��������
			ptext		= ���������ı�
			rtArea		= �ַ���ʾ����
			hFont		= ����
			uiFormat	= �ı���ʾ��ʽ
		����ֵ������ʾ���ַ�
	*****************************************************************************************/
	static std::wstring stringMeasure(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, HFONT hFont, UINT uiFormat);
	static std::wstring stringMeasure(PCWSTR ptext, const rect &rtArea, HFONT hFont, UINT uiFormat);
	/*****************************************************************************************
		<< --- drawer::drawString	2014-01-13 --- >>
		˵������ָ����������ı�
		������
		grap		= GDI+��������
		ptext		= ��������ı�
		font		= ��������
		algin		= �ı�����Ķ��뷽ʽ
		rtArea		= �ı��������
		clrText		= �ı���ɫ
		isVertical	= �ı������ʽ��true=��ֱ��false=ˮƽ
		bTrimming	= ������ʾ�����������"..."����
		����ֵ��
	*****************************************************************************************/ 
	static void drawString(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, const BSFont &font, BEAlignMode algin = BEAlignModeCenterCenter, uint32_t clrText = 0xFE000000, bool isVertical = false, bool bTrimming = false);
	static void drawString(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, HFONT hFont, BEAlignMode algin = BEAlignModeCenterCenter, uint32_t clrText = 0xFE000000, bool isVertical = false, bool bTrimming = false);
	//static void drawString(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, const Gdiplus::Font &font, BEAlignMode algin, uint32_t clrText, bool isVertical, bool bTrimming);
	static void drawString(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, HFONT hFont, UINT uiFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER, uint32_t clrText = 0xFE000000, bool isVertical = false);
	
	/****************************************************************************************
		<< --- drawStringEffectBk		2014-11-6 --- >>         
		˵������������win7����ڵ�������ʽ����Ч
		������
			grap		= GDI+��������
			ptext		= ��������ı�
			rtArea		= �ı��������
			font		= ��������
			clrText		= �ı���ɫ
			clrBk		= ���ֱ���ɫ
		����ֵ��
	*****************************************************************************************/
	static void drawStringEffectBk(Gdiplus::Graphics &grap, PCWSTR ptext, const rect &rtArea, HFONT hFont, BEAlignMode algin, uint32_t clrText = 0xFEFFFFFF, uint32_t clrBk = 0xFE212121);
	/*****************************************************************************************
		<< --- drawer::drawImage	2014-07-16 --- >>
		˵������ͼƬĳ���������ָ��λ��
		������
		grap		= GDI+��������
		pImage		= �������ͼ��
		rcDest		= ͼ�������Ŀ������
		rcScr		= ͼ���е�ĳһ����
		unit		= ��λ
		����ֵ��ִ���Ƿ�ɹ�
	*****************************************************************************************/ 
	static bool drawImage(Gdiplus::Graphics &grap, Gdiplus::Image *pImage, const rect &rcDest, const rect &rcScr, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	static bool drawImage(Gdiplus::Graphics &grap, gtc::image *pImage, const rect &rcDest, const rect &rcScr, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	/*****************************************************************************************
		<< ---  controlUC::drawImage		2014-07-31 --- >>
		˵������ͼƬĳһ������Ƶ�Ŀ��λ��
		������
		grap	= ��ͼ����
		pImage	= ͼ��
		rcDest	= Ŀ��λ��
		����ֵ��ִ���Ƿ�ɹ�
	*****************************************************************************************/
	static bool drawImage(Gdiplus::Graphics &grap, gtc::image *pImage, const rect &rcDest);
	static bool drawImage(Gdiplus::Graphics &grap, gtc::image *pImage, const rect &rcDest, uint32_t clrMask);
	/****************************************************************************************
		<< --- drawImage9Part		2015-1-30 --- >>         
		˵�������þŹ���ʽ����ͼƬ���磺���ƴ���Բ�ǵı���ͼ
				�ĸ���Դ��Ŀ��ߴ�һ�£����±ߵĸ߶���Ŀ��һ�£��������죻���ұߵĿ����Ŀ��һ�£��߶����죻�м�ƽ��
		������
		grap		= GDI+��������
		pImage		= �������ͼ��
		rcDest		= ͼ�������Ŀ������
		rcScr		= ͼ���е�ĳһ����
		szPatch		= 4���ǵĳߴ磬���±ߵĸ�=cy�����ұߵĿ�=cx
		unit		= ��λ
		����ֵ��
	*****************************************************************************************/
	//static bool drawImage9Patch(Gdiplus::Graphics &grap, Gdiplus::Image *pImage, const rect &rcDest, const rect &rcScr, size szPatch, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	static bool drawImage9Patch(Gdiplus::Graphics &grap, Gdiplus::Image *pImage, const rect &rcDest, const rect &rcScr, RECT rcCorner, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	static bool drawImage9Patch(Gdiplus::Graphics &grap, gtc::image *pImage, const rect &rcDest, const rect &rcScr, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	/****************************************************************************************
		<< --- drawImage9Part		2015-1-30 --- >>         
		˵��������ȡ���˺��м�ķ�ʽ����ͼƬ��һ���������ͼƬ�߶�һ�£��磺����խ��ˮƽ������
				������Ŀ��ߴ�һ�£��м�߶���Ŀ��һ�£���������
		������
		grap		= GDI+��������
		pImage		= �������ͼ��
		rcDest		= ͼ�������Ŀ������
		rcScr		= ͼ���е�ĳһ����
		iPatch		= �������˵Ŀ�ȣ�=0ʱ��ʾ��ֵΪͼƬ�ĸ߶�
		unit		= ��λ
		����ֵ��
	*****************************************************************************************/
	//static bool drawImage3PatchH(Gdiplus::Graphics &grap, Gdiplus::Image *pImage, const rect &rcDest, const rect &rcScr, int iPatch = 0, Gdiplus::Unit unit = Gdiplus::UnitPixel);
	/****************************************************************************************
		<< --- drawImage9Part		2015-1-30 --- >>         
		˵��������ȡ���˺��м�ķ�ʽ����ͼƬ��һ���������ͼƬ�߶�һ�£��磺����խ��ˮƽ������
				������Ŀ��ߴ�һ�£��м�����Ŀ��һ�£��߶�����
		������
		grap		= GDI+��������
		pImage		= �������ͼ��
		rcDest		= ͼ�������Ŀ������
		rcScr		= ͼ���е�ĳһ����
		iPatch		= �������˵ĸ߶ȣ�=0ʱ��ʾ��ֵΪͼƬ�Ŀ��
		unit		= ��λ
		����ֵ��
	*****************************************************************************************/
	//static bool drawImage3PatchV(Gdiplus::Graphics &grap, Gdiplus::Image *pImage, const rect &rcDest, const rect &rcScr, int iPatch = 0, Gdiplus::Unit unit = Gdiplus::UnitPixel);

	/****************************************************************************************
		<< --- drawImage 	2016-3-4	kp007 --- >>         
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	static void drawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
		const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255,
		bool hole = false, bool xtiled = false, bool ytiled = false);
public:
	/****************************************************************************************
		<< --- 		2015-2-6 --- >>         
		˵�������»���λͼ����λͼ�����ȣ��ҵ���ԭͼ��ߣ�����ԭͼ��ͼ��������ͼ����
		������
			phBitmap	= �����ε�λͼ�������ó���λͼ
			destCXCY	= λͼ��С�ߴ磬С�ڴ˴�С��ͼ�������ʾ
		����ֵ��
	*****************************************************************************************/
	static void repositionBitmap(HBITMAP *phBitmap, int minCXCY);
protected:
	/****************************************************************************************
		<< --- makeRoundRect		2014-11-6 --- >>         
		˵����
		������
		����ֵ��
	*****************************************************************************************/
	static Gdiplus::GraphicsPath* makeRoundRect(const rect &rcArea, int roundWidth, int roundHeight);
	/****************************************************************************************
		<< --- 	buildStringFormat	2014-11-6 --- >>         
		˵�����������ʵĸ�ʽ���ַ���
		������
			strformat	= �������ַ�����ʽ
			algin		= ���뷽ʽ
		����ֵ��
	*****************************************************************************************/
	static void buildStringFormat(Gdiplus::StringFormat &strformat, BEAlignMode algin);
	static void convertToStringFormat(Gdiplus::StringFormat &strformat, UINT uiFormat, bool isVertical);
	//protected:
//	HDC m_hDC;
//	Gdiplus::Graphics m_grap;
};

}