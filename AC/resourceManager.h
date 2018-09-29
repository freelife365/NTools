#pragma once
#include "sysupport.h"
#include "image.h"
namespace gtc{


class resourceManager
{
public:
	// �˵�����ť������Ĵ�����
	typedef INT  (CALLBACK *COMMANDPROC)(UINT cmdID, void *pData);
public:
	resourceManager();
	~resourceManager();

	void free();
	// ͼƬ png
	gtc::image* addImage(const std::wstring &imgKey, UINT id, LPCTSTR pType = RT_RCDATA);
	gtc::image* addImage(const std::wstring &imgKey, PCWSTR pFile);
	gtc::image* findImage(const std::wstring &imgKey) const;

	// �˵�
	bool addMenu(const std::wstring &menuKey, UINT id);
	HMENU findMenu(const std::wstring &menuKey) const;
	// ����
	int addFont(PCWSTR pFontName, int nSize, bool bBold = false, bool bUnderline = false, bool bItalic = false);
	int addFont(const BSFontInfo &fontInfo);
	int addFont(LOGFONTW *plf);
	HFONT getFont(int fontIndex = -1);
	const BSFontInfoUC* findFontInfo(int fontIndex) const;
	HFONT setDefaultFont(PCWSTR pFontName, int nSize, bool bBold = false, bool bUnderline = false, bool bItalic = false);
	HFONT setDefaultFont(const BSFontInfo &fontInfo);
	HFONT setDefaultFont(LOGFONTW *plf);
	// �ؼ�Ĭ�������б�
	bool addAttributeList(const std::wstring &wsControlKey, const std::wstring &wsAttrList);
	PCWSTR findAttributeList(const std::wstring &wsControlKey);
	// ע���������
	bool registerCommandProc(COMMANDPROC procCommand);
	INT execCommand(UINT cmdID, void *pData);
public:
	static bool initEnvironment(HMODULE hModule);
	static void releaseEnvironment();
public:
	HMODULE getModule() const {				return mc_hModule;							}
	void setModuble(HMODULE hModule) {		mc_hModule = hModule;						}
	//ULONG_PTR getGDIToken() const {			return m_gdiToken;							}
	//void setGDIToken(ULONG_PTR token) {		m_gdiToken = token;							}
protected:
	static ULONG_PTR	mc_gdiToken;			// GDI+
	static Gdiplus::GdiplusStartupInput mc_gdiInput;
	static HMODULE		mc_hModule;			// ��Դ��������
protected:
	std::map<std::wstring, gtc::image*>	m_mapImageRes;		// ͼƬ����key=�û�����ı��
	std::map<std::wstring, HMENU>		m_mapMenuRes;		// �˵���Դ�����Ƕ����˵���
	std::vector<BSFontInfoUC *>			m_vecFontRes;		// ������Դ
	BSFontInfoUC						m_bsDefaultFont;	// Ĭ������
	StringMapType						m_mapAttributeList;	// ϵͳĬ�����Լ��ϣ�key=�û�����ı�� value=�����б�

	COMMANDPROC		m_procCommand;	// �������
};

}