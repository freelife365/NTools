#include "wallPaper.h"
#include <process.h>
#include <GdiPlus.h>
#include "sysupport.h"
#include "baseReg.h"
#include "AC/monitor.h"
#include "AC/file.h"
#include "AC/draw/bmp.h"
#include "AC/draw/drawer.h"
namespace acm {
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	wallPaper��	2015-1-21
//	ǽֽ��Ϣ�ռ����Լ����ǽֽ�ĸı�
////////////////////////////////////////////////////////////////////////////////////////////////////////
wallPaper::wallPaper()
	: m_beStyle(BEWallpaperStyleFill)
	, m_hReceiver(NULL), m_uiMsgFlag(0)
	, m_eventExit(NULL), m_hThread(NULL)
	, m_eventReg(NULL)
{
	// ǽֽ������ʱ�ļ�
	m_wsFullPath = gtc::app::getConfigPath();
	m_wsFullPath += L"\\wallpaper.tmp";
	//wchar_t pPath[MAX_PATH] = {0};
	//DWORD dwHigh = 0, dwLow = 0;
	//HGLOBAL hGlobal = NULL;
	//SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, pPath);
	//PathAppendW(PathAppendW, gtc::app::getConfigPath())
	//PathAppendW(pbuf, L"jzzm\\wall0001.tmp");

	refresh();
}

wallPaper::~wallPaper()
{
	stopMonitor();
}

void wallPaper::refresh(bool bcopyWallfile /*= true*/)
{
	//std::wstring wsWallPath;
	acm::regKey reg;

	m_wsSysWallFileFullPath.clear();
	if (!reg.open(HKEY_CURRENT_USER, L"Control Panel\\Desktop"))
	{
		WRITE_LOG(L"wallPaper::refresh ��ע���ʧ��<Control Panel\\Desktop>������");
	}
	else
	{
		WRITE_LOG(L"wallPaper::refresh readWallpaper ǽֽ 0");
		// ��ʽ
		DWORD dwStyle = 0;
		std::wstring wsStyle;
		if(reg.readString(L"WallpaperStyle", wsStyle))
			dwStyle = std::wcstoul(wsStyle.c_str(), NULL, 10);
		else
			reg.readUI4(L"WallpaperStyle", dwStyle);	// ����360��ֽ
		if(dwStyle == 10)
			m_beStyle = BEWallpaperStyleFill;
		else if(dwStyle == 6)
			m_beStyle = BEWallpaperStyleFit;
		else if(dwStyle == 2)
			m_beStyle = BEWallpaperStyleStretch;
		else if(dwStyle == 0)
		{
			if(reg.readString(L"TileWallpaper", wsStyle))
				dwStyle = std::wcstoul(wsStyle.c_str(), NULL, 10);
			else
				reg.readUI4(L"TileWallpaper", dwStyle);	// ����360��ֽ
			if(dwStyle == 1)
				m_beStyle = BEWallpaperStyleTile;
			else if(dwStyle == 0)
				m_beStyle = BEWallpaperStyleCenter;
		}
		// ǽֽ0
		//if (wsWallPath.empty())
		//{
			reg.readString(L"Wallpaper", m_wsSysWallFileFullPath);

		//}

	}
	wchar_t pbuf[MAX_PATH] = {0};
	// ��ֽ1
	if (m_wsSysWallFileFullPath.empty())
	{
		SystemParametersInfo(SPI_GETDESKWALLPAPER, _countof(pbuf), pbuf, 0);
		if(::PathFileExistsW(pbuf)) m_wsSysWallFileFullPath = pbuf;
	}
	// ��ֽ2
	if (m_wsSysWallFileFullPath.empty())
	{
		DWORD dwLen = 0;
		DWORD dwType = REG_SZ;
		if(gtc::app::isWindows7Later())
			SHGetValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Desktop\\General", L"WallpaperSource", &dwType, pbuf, &dwLen );
		else
			SHGetValue(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"Wallpaper", &dwType, pbuf,&dwLen );
		pbuf[dwLen] = L'\0';
		if(::PathFileExistsW(pbuf)) m_wsSysWallFileFullPath = pbuf;
	}

	//SEND_LOG(L"wallPaper::refresh wall:<%s> temp:<%s>", m_wsSysWallFileFullPath.c_str(), m_wsFullPath.c_str());
	// ��ֽ���Ϊ
	if (bcopyWallfile)
	{
		DWORD dwHigh = 0, dwLow = 0;
		HGLOBAL hGlobal = NULL;
		//SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, pbuf);
		//PathAppendW(pbuf, L"jzzm\\wall0001.tmp");
		if (m_wsSysWallFileFullPath.empty())
		{
			// ϵͳ��ɫ0x00BGR����GDI+��ʾΪ0xFFRGB
			DWORD clrdesk = GetSysColor(COLOR_DESKTOP);
			clrdesk = (GetRValue(clrdesk) << 16)|GetGValue(clrdesk) << 8|GetBValue(clrdesk)|0xFF000000;
			gtc::bmpFile fileBmp;

			m_wsSysWallFileFullPath = gtc::app::getConfigPath();
			m_wsSysWallFileFullPath += L"\\wallpaper_tmp.bmp";
			if (!fileBmp.create(m_wsSysWallFileFullPath, 100, 100, clrdesk))
			{
				m_wsSysWallFileFullPath = gtc::app::getConfigPath();
				m_wsSysWallFileFullPath += L"\\wallpaper.bmp";
			}

		}
		HANDLE hFile = CreateFile(m_wsSysWallFileFullPath.c_str(),GENERIC_READ ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);
		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		{
			dwLow = GetFileSize(hFile, &dwHigh);
			hGlobal = ::GlobalAlloc(GMEM_FIXED, dwLow);
			void* pBuffer = ::GlobalLock(hGlobal);
			if (pBuffer)
			{
				ReadFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
				::GlobalUnlock(hGlobal);
			}
			CloseHandle(hFile);
			hFile = NULL;
		}
		else
		{
			SEND_LOG(L"WARNING wallPaper::refresh ��read���޷���ǽֽ�ļ��������̳߳��Զ�ȡ�ļ���");
			//::SendMessage(m_hReceiver, m_uiMsgFlag, 0, 0);	// �ٴ��ػ�
			gtc::thread::start(threadCopyWallfile, this, true);
		}
		if (hGlobal)
		{
			hFile = CreateFile(m_wsFullPath.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
			{
				void* pBuffer = ::GlobalLock(hGlobal);
				if (pBuffer)
				{
					WriteFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
					::GlobalUnlock(hGlobal);
				}
				CloseHandle(hFile);
				hFile = NULL;
				::SendMessage(m_hReceiver, m_uiMsgFlag, 0, true);	// �ٴ��ػ�
			}
			else
			{
				SEND_LOG(L"WARNING wallPaper::refresh ��write���޷���ǽֽ�ļ��������̳߳��Զ�ȡ�ļ���");
				//::SendMessage(m_hReceiver, m_uiMsgFlag, 0, 0);	// �ٴ��ػ�
				gtc::thread::start(threadCopyWallfile, this, true);

			}
			::GlobalFree(hGlobal);
			hGlobal = NULL;
		}
	}
	
	//m_wsFullPath = pbuf;
	//pImage = Gdiplus::Image::FromFile(pPath);
}
void wallPaper::draw(HDC hdc)
{
	//SEND_LOG(L"ERROR wallPaper::draw ׼������ǽֽ begin ...");
	assert(hdc);
	if(!hdc) return;
	std::wstring wsWall = m_wsFullPath;
	BEWallpaperStyle beStyle = m_beStyle;
	// Win8��ǽֽ��ѡ
	if (gtc::app::isWindows8Later())
	{
		std::wstring pathWall;
		wchar_t path[MAX_PATH] = {0};
		if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			pathWall = path;
			pathWall += L"\\Microsoft\\Windows\\Themes";
			std::wstring wsPath = pathWall;
			wsPath += L"\\CachedFiles\\*.jpg";
			WIN32_FIND_DATA wfd = {0};
			HANDLE hFind = ::FindFirstFile(wsPath.c_str(), &wfd);
			if (hFind != INVALID_HANDLE_VALUE)
			{
				wsWall = pathWall;
				wsWall += L"\\CachedFiles\\";
				wsWall += wfd.cFileName;
				//WRITE_LOG(L"wallPaper::draw A WIN8 name:<%s>", wsWall.c_str());
				FindClose(hFind);
			}
		}
	}
	// ��������
	SIZE szWall = {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
	RECT rcWall = {0,0,szWall.cx,szWall.cy};

	// ϵͳ��ɫ0x00BGR����GDI+��ʾΪ0xFFRGB
	DWORD clrdesk = GetSysColor(COLOR_DESKTOP);
	clrdesk = (GetRValue(clrdesk) << 16)|GetGValue(clrdesk) << 8|GetBValue(clrdesk)|0xFF000000;
	if (wsWall.empty() && gtc::app::isWindowsXP())
	{
		acm::regKey reg;
		if (reg.open(HKEY_CURRENT_USER, L"Control Panel\\Desktop"))
		{
			std::wstring wsIniPath = gtc::app::getConfigPath();
			//wchar_t path[MAX_PATH] = {0};
			//if(FAILED(SHGetFolderPathW(NULL, CSIDL_APPDATA, nullptr, 0, path)))
			//{
			//	WRITE_LOG(L"wallPaper::draw XP�¶�ȡ·��CSIDL_APPDATAʧ�ܣ�");
			//}
			gtc::bmpFile file;
			wsWall = wsIniPath;
			wsWall += L"\\wallpaper_tmp.bmp";
			if (!file.create(wsWall.c_str(), 100, 100, clrdesk))
			{
				WRITE_LOG(L"wallPaper::draw ������ֽͼƬwallpaper_tmp.bmpʧ�ܣ�");
				wsWall = wsIniPath;
				wsWall += L"\\wallpaper.bmp";
			}

			beStyle = BEWallpaperStyleStretch;
			reg.writeString(L"Wallpaper", wsWall);
			::SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE|SPIF_SENDWININICHANGE);

		}
	}
	// ���Ʊ�ֽ
	Gdiplus::Graphics grap(hdc);
	//// ����ɫ
	//gtc::drawer::fillRectangle(grap, clrdesk, rcWall);

	// ͼƬ
	Gdiplus::Image *pImage = NULL;
	//if(!pImage)
	//{
	//	/*DWORD dwHigh = 0, dwLow = 0;
	//	HGLOBAL hGlobal = NULL;
	//	wchar_t pPath[MAX_PATH] = {0};
	//	SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, pPath);
	//	PathAppendW(pPath, L"jzzm\\wall0001.tmp");

	//	HANDLE hFile = CreateFile(m_wsFullPath.c_str(),GENERIC_READ ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);
	//	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	//	{
	//		dwLow = GetFileSize(hFile, &dwHigh);
	//		hGlobal = ::GlobalAlloc(GMEM_FIXED, dwLow);
	//		void* pBuffer = ::GlobalLock(hGlobal);
	//		if (pBuffer)
	//		{
	//			ReadFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
	//			::GlobalUnlock(hGlobal);
	//		}
	//		CloseHandle(hFile);
	//		hFile = NULL;
	//	}
	//	else
	//	{
	//		SEND_LOG(L"WARNING �޷���ǽֽ�ļ�������ϵͳ���ڻ���ֽ������");
	//	}
	//	if (hGlobal)
	//	{
	//		hFile = CreateFile(pPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	//		{
	//			void* pBuffer = ::GlobalLock(hGlobal);
	//			if (pBuffer)
	//			{
	//				WriteFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
	//				::GlobalUnlock(hGlobal);
	//			}
	//			CloseHandle(hFile);
	//			hFile = NULL;
	//		}
	//		::GlobalFree(hGlobal);
	//		hGlobal = NULL;
	//	}
	//	pImage = Gdiplus::Image::FromFile(pPath);*/
	//	//pImage = Gdiplus::Image::FromFile(m_wsFullPath.c_str());
	//	//if (pImage)
	//	//{

	//	//	hr = StgCreateDocfile(
	//	//		pbuf, 
	//	//		STGM_READWRITE|STGM_CREATE|STGM_SHARE_EXCLUSIVE, 
	//	//		0, 
	//	//		&pIStorage);
	//	//	hr = pIStorage->CreateStream(
	//	//		L"StreamImage1",
	//	//		STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
	//	//		0,
	//	//		0,
	//	//		&pIStream1);
	//	//	CLSID jpgClsid;
	//	//	gtc::image::getImageCLSID(L"jpg", &jpgClsid);

	//	//	pImage->Save(pIStream1, &jpgClsid);
	//	//
	//	//	delete pImage;

	//	//	pImage = Gdiplus::Image::FromStream(pIStream1);
	//	//}
	//	//else
	//	//{
	//	//	SEND_LOG(L"ERROR -------------------xxxxxxxxxx Ϊ��1");
	//	//}
	//}

	
	pImage = Gdiplus::Image::FromFile(m_wsFullPath.c_str());
	if (pImage)
	{	
		const SIZE szImage = {pImage->GetWidth(), pImage->GetHeight()};
		RECT rcImage;
		// 
		acm::monitor moni;
		for (int idx = 0; idx < acm::monitor::monitorCount(); ++idx)
		{
			moni.setNumber(idx);
			//WRITE_LOG(L"wallPaper::draw ��Ļ�ֱ��ʷ����ı�:%s", moni.toString().c_str());

			estimateWallparperArea(beStyle, szImage, moni.getWorkAreaSize(), rcImage, rcWall);
			::OffsetRect(&rcWall, moni.getWorkArea().left, moni.getWorkArea().top);
			//estimateWallparperArea(beStyle, szImage, szWall, rcImage, rcWall);

			//WRITE_LOG(L"wallPaper::draw deskColor:%08X style:%d  imageSrc:<%s> screenDest:<%s>  wall:<%d,%d> image:<%d,%d>", 
				//clrdesk, beStyle, gtc::format::rect(rcImage, true).c_str(), 
				//gtc::format::rect(rcWall, true).c_str(), 
				//szWall.cx, szWall.cy, szImage.cx, szImage.cy);

			if(isEmpty(rcImage))
			{
				//WRITE_LOG(L"wallPaper::draw BBB_1");
				RECT rcSrc = {0, 0, pImage->GetWidth(), pImage->GetHeight()};
				gtc::drawer::drawImage(grap, pImage, rcWall, rcSrc);
			}
			else
			{
				if (beStyle == BEWallpaperStyleTile)
				{
					//WRITE_LOG(L"wallPaper::draw BBB_2");
					SIZE szImage = {rectWidth(rcImage), rectHeight(rcImage)};
					int cols = szWall.cx/szImage.cx;
					if(szWall.cx%szImage.cx) ++cols;
					int rows = szWall.cy/szImage.cy;
					if(szWall.cy%szImage.cy) ++rows;
					RECT rc, rc2;
					for (int row = 0; row < rows; ++row)
					{
						for (int col = 0; col < cols; ++col)
						{
							rc2 = rcImage;
							rc.left = szImage.cx * col;
							rc.right = rc.left + szImage.cx;
							rc.top = szImage.cy * row;
							rc.bottom = rc.top + szImage.cy;
							if(rc.right > szWall.cx)
							{
								rc.right = szWall.cx;
								rc2.right = szWall.cx-rc.left;
							}
							if(rc.bottom > szWall.cy)
							{
								rc.bottom = szWall.cy;
								rc2.bottom = szWall.cy-rc.top;
							}
							gtc::drawer::drawImage(grap, pImage, rc, rc2);
						}
					}
				}
				else
				{
					//WRITE_LOG(L"wallPaper::draw BBB_3");
					gtc::drawer::drawImage(grap, pImage, rcWall, rcImage);
				}

			}			
			//WRITE_LOG(L"wallPaper::draw CCC");
		}

		delete pImage;
		pImage = NULL;
	}
	else
	{
		SEND_LOG(L"ERROR -------------------xxxxxxxxxx Ϊ��2");
	}
	//if(pIStream1) pIStream1->Release();
	//if(pIStorage) pIStorage->Release();

}

void wallPaper::estimateWallparperArea(BEWallpaperStyle style, SIZE szImage, SIZE szScreen, RECT &rcImage, RECT &rcScreen)
{
	// ��ȡ��Ļ��С
	//SIZE szScreen = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	float slopeScreen = float(szScreen.cy)/szScreen.cx;
	float slopeImage = float(szImage.cy)/szImage.cx;
	rcImage.left = rcImage.top = 0;
	rcImage.right = szImage.cx;
	rcImage.bottom = szImage.cy;
	rcScreen.left = rcScreen.top = 0;
	rcScreen.right = szScreen.cx;
	rcScreen.bottom = szScreen.cy;
	switch(style)
	{
	case BEWallpaperStyleCenter:		//,	//����
	case BEWallpaperStyleFit:			//,	//��Ӧ
		{	
			if (slopeImage > slopeScreen)
			{	// ͼ��ߴ�խ��Ҫ��ȡ���¸�һ����
				rcImage.left = 0;
				rcImage.right = szImage.cx;
				rcImage.top = 0;
				rcImage.bottom = szImage.cy;
				if (szImage.cy > szScreen.cy)
				{
					rcImage.top = (szImage.cy-szScreen.cy)/2;
					rcImage.bottom = szImage.cy-rcImage.top;
					rcScreen.left = (szScreen.cx-szImage.cx)/2;// (float(szScreen.cx)-float(szScreen.cy)*szImage.cx/szImage.cy)/2;
					rcScreen.right = szScreen.cx-rcScreen.left;
				}
				else
				{
					rcScreen.left = (szScreen.cx-szImage.cx)/2;
					rcScreen.right = szScreen.cx-rcScreen.left;
					rcScreen.top = (szScreen.cy-szImage.cy)/2;
					rcScreen.bottom = szScreen.cy-rcScreen.top;
				}
			}
			else
			{	// ͼ��Ƚϳ���Ҫ��ȥ���Ҹ�һ����
				rcImage.left = 0;
				rcImage.right = szImage.cx;
				rcImage.top = 0;
				rcImage.bottom = szImage.cy;
				if (szImage.cx > szScreen.cx)
				{
					rcImage.left = (szImage.cx - szScreen.cx)/2;
					rcImage.right = szImage.cx - rcImage.left;
					rcScreen.top = (szScreen.cy-szImage.cy)/2;// (float(szScreen.cy)-float(szScreen.cx)*szImage.cy/szImage.cx)/2;
					rcScreen.bottom = szScreen.cy-rcScreen.top;
				}
				else
				{
					rcScreen.left = (szScreen.cx-szImage.cx)/2;
					rcScreen.right = szScreen.cx-rcScreen.left;
					rcScreen.top = (szScreen.cy-szImage.cy)/2;
					rcScreen.bottom = szScreen.cy-rcScreen.top;

				}
			}
		}
		break;
		//{
		//	if (slopeImage > slopeScreen)
		//	{
		//		if (szImage.cx > szScreen.cx)
		//		{	// ����ͼ���м�һ����
		//			rcImage.left = (szImage.cx-szScreen.cx)/2;
		//			rcImage.right = szImage.cx-rcImage.left;
		//			rcImage.top = (szImage.cy-szScreen.cy)/2;
		//			rcImage.bottom = szImage.cy-rcImage.top;
		//		}
		//		else
		//		{
		//			if (szImage.cy > szScreen.cy)
		//			{
		//				rcImage.top = (szImage.cy-szScreen.cy)/2;
		//				rcImage.bottom = szImage.cy-rcImage.top;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		if (szImage.cy > szScreen.cy)
		//		{
		//			rcImage.left = (szImage.cx-szScreen.cx)/2;
		//			rcImage.right = szImage.cx-rcImage.left;
		//			rcImage.top = (szImage.cy-szScreen.cy)/2;
		//			rcImage.bottom = szImage.cy-rcImage.top;
		//		}
		//		else
		//		{
		//			if (szImage.cx > szScreen.cx)
		//			{
		//				rcImage.left = (szImage.cx-szScreen.cx)/2;
		//				rcImage.right = szImage.cx-rcImage.left;
		//			}
		//		}
		//	}
		//}
		//break;
	case BEWallpaperStyleTile:			//,	//ƽ��
		{	
			if (slopeImage > slopeScreen)
			{
				if (szImage.cx > szScreen.cx)
				{	// ����ͼ������һ����
					rcImage.right = szScreen.cx;
					rcImage.bottom = szScreen.cy;
				}
				else
				{
					if (szImage.cy > szScreen.cy)
					{
						rcImage.bottom = szScreen.cy;
					}
				}
			}
			else
			{
				if (szImage.cy > szScreen.cy)
				{
					rcImage.right = szScreen.cx;
					rcImage.bottom = szScreen.cy;
				}
				else
				{
					if (szImage.cx > szScreen.cx)
					{
						rcImage.right = szScreen.cx;
					}
				}
			}
		}
		break;
	case BEWallpaperStyleStretch:		//,	//����
		{
			memset(&rcImage, 0, sizeof(rcImage));
		}
		break;

	case BEWallpaperStyleFill:			//���
		if (slopeImage > slopeScreen)
		{	// ͼ��ߴ�խ��Ҫ��ȡ���¸�һ����
			rcImage.left = 0;
			rcImage.right = szImage.cx;
			rcImage.top = (LONG)(float(szImage.cy) - float(szImage.cx)*szScreen.cy/szScreen.cx)/2;
			rcImage.bottom = szImage.cy - rcImage.top;
		}
		else
		{	// ͼ��Ƚϳ���Ҫ��ȥ���Ҹ�һ����
			rcImage.top = 0;
			rcImage.bottom = szImage.cy;
			rcImage.left = (LONG)(float(szImage.cx) - float(szScreen.cx)*szImage.cy/szScreen.cy)/2;
			rcImage.right = szImage.cx - rcImage.left;
		}
		break;
	}
}

void wallPaper::init()
{
	wchar_t path[MAX_PATH] = {0};
	if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		m_wsSysWallPath = path;
		if (gtc::app::isWindowsXP())
		{
			m_wsSysWallPath += L"\\Microsoft";//\\Wallpaper1.bmp
		}
		else
		{
			m_wsSysWallPath += L"\\Microsoft\\Windows\\Themes";//\\TranscodedWallpaper.jpg
		}

	}
}

bool wallPaper::checkChanged()
{
	static std::wstring c_oldFullPath;
	static std::time_t	c_oldLastWrite;
	static int64_t		c_oldFileSize;
	// �ȶ�ȡע���ֵ
	refresh(false);
	// ͼƬ�Ƿ�ı�
	if (c_oldFullPath != m_wsSysWallFileFullPath)
	{
		c_oldFullPath = m_wsSysWallFileFullPath;
		WRITE_LOG(L"wallPaper::checkChanged ��ֽ�ļ������ı� name:<%s>", m_wsSysWallFileFullPath.c_str());
		return true;
	}
	// �ļ����Ա仯
	BSFileAttribute fattr = {0};
	if(gtc::file::fileAttributeInfo(m_wsSysWallFileFullPath.c_str(), fattr))
	{
		if (fattr.dtLastWrite != c_oldLastWrite || fattr.llSize != c_oldFileSize)
		{
			WRITE_LOG(L"wallPaper::checkChanged ��ֽ�ļ����Է����ı� size:<%ul,%ul> date:<%lld,%lld>", c_oldFileSize, fattr.llSize, c_oldLastWrite, fattr.dtLastWrite);
			c_oldLastWrite = fattr.dtLastWrite;
			c_oldFileSize = fattr.llSize;

			//if(ptm = std::localtime(&c_dtWallpaperWrite))
			//	SEND_LOG(L"ERROR dtLastWrite 3: %04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year+1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			return true;
		}
	}


	return false;
}
unsigned int WINAPI wallPaper::threadMonitor(void *pparam)
{
	wallPaper *pWall = static_cast<wallPaper *>(pparam);
	HANDLE hWallFile = FindFirstChangeNotification(pWall->m_wsSysWallPath.c_str(), FALSE, FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE);
	SEND_LOG(L"wallPaper::threadMonitor ��ֽ����߳� ����");
	assert(hWallFile && hWallFile != INVALID_HANDLE_VALUE);
	//if(hWallFile == NULL || hWallFile == INVALID_HANDLE_VALUE) 
	HANDLE hHandles[] = {pWall->m_eventExit, hWallFile, pWall->m_eventReg};
	DWORD wret = 0;
	while(true)
	{
		wret = WaitForMultipleObjects(3, hHandles, FALSE, INFINITE);
		//SEND_LOG(L"NOTE wallPaper::threadMonitor ��ֽ����߳� --�������ı� ret:%u", wret);
		if (wret == WAIT_OBJECT_0)
		{	// �˳����
			break;
		}
		else if (wret == WAIT_OBJECT_0 + 1)
		{
			// ��ֽ�ļ����������Ƿ����ı�
			//SEND_LOG(L"NOTE wallPaper::threadMonitor �����Ŀ¼�����ı� dir:<%s>", pWall->m_wsSysWallPath.c_str());
			if (pWall->checkChanged())
			{
				// ֪ͨ��ش���
				SEND_LOG(L"NOTE wallPaper::threadMonitor ����ء�Ŀ¼�����ݷ����ı�-->�ı�ǽֽ");
				pWall->refresh();
				//::SendMessage(pWall->m_hReceiver, pWall->m_uiMsgFlag, 0, 0);
			}
			// �������
			if (!FindNextChangeNotification(hWallFile))
			{
				SEND_LOG(L"ERROR wallPaper::threadMonitor ����Ŀ¼���ʧ�ܣ�����");
				break;
			}
		}
		else if (wret == WAIT_OBJECT_0 + 2)
		{	// ע���ֵ�����ı�
			SEND_LOG(L"NOTE wallPaper::threadMonitor �����ǽֽ��ע��������ı�-->�ı�ǽֽ");
			pWall->refresh();
			//::SendMessage(pWall->m_hReceiver, pWall->m_uiMsgFlag, 0, 0);
			pWall->m_regWall.notifyChangeKeyValue(false, REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET, pWall->m_eventReg);
		}
		else
		{
			/* WAIT_TIMEOUT WAIT_FAILED */
			break;
		}

	}
	// 
	if (hWallFile)
	{
		FindCloseChangeNotification(hWallFile);
	}

	SEND_LOG(L"wallPaper::threadMonitor ��ֽ����߳� �ر�");
	return 0;
}

unsigned int WINAPI wallPaper::threadCopyWallfile(gtc::thread::_BSThreadInfo *pthr)
{
	wallPaper *pWall = static_cast<wallPaper *>(pthr->pParameter);
	assert(pWall);
	HANDLE hFile = NULL;
	DWORD dwHigh = 0, dwLow = 0;
	const DWORD tryCount = 5;
	DWORD idx = 0;
	HGLOBAL hGlobal = NULL;
	SEND_LOG(L"NOTE wallPaper::threadCopyWallfile begin ...");
	__try
	{
		OleInitialize(NULL);
		if(!PathFileExistsW(pWall->m_wsSysWallFileFullPath.c_str())) __leave;

		hFile = CreateFile(pWall->m_wsSysWallFileFullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		while((hFile == NULL || hFile == INVALID_HANDLE_VALUE) && idx < tryCount*2)
		{
			++idx;
			SEND_LOG(L"NOTE wallPaper::threadCopyWallfile ׼���ٴΡ�read����%u����ǽֽ�ļ����Ա����ƣ�����err:%u wallfile:<%s>", idx, GetLastError(), pWall->m_wsSysWallFileFullPath.c_str());
			Sleep(100);
			hFile = CreateFile(pWall->m_wsSysWallFileFullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		{
			dwLow = GetFileSize(hFile, &dwHigh);
			hGlobal = ::GlobalAlloc(GMEM_FIXED, dwLow);
			void* pBuffer = ::GlobalLock(hGlobal);
			if (pBuffer)
			{
				ReadFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
				::GlobalUnlock(hGlobal);
			}
			CloseHandle(hFile);
			hFile = NULL;
		}
		else
		{
			SEND_LOG(L"ERROR wallPaper::threadCopyWallfile �޷���ǽֽ�ļ���read��������ϵͳ���ڻ���ֽ������");
			//::SendMessage(m_hReceiver, m_uiMsgFlag, 0, 0);	// �ٴ��ػ�
		}
		if (hGlobal)
		{
			idx = 0;
			hFile = CreateFile(pWall->m_wsFullPath.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			while((hFile == NULL || hFile == INVALID_HANDLE_VALUE) && idx < tryCount)
			{
				++idx;
				SEND_LOG(L"NOTE wallPaper::threadCopyWallfile ׼���ٴΡ�write����%u����ǽֽ�ļ����Ա����ƣ�����err:%u targetfile:<%s>", idx, GetLastError(), pWall->m_wsFullPath.c_str());
				Sleep(100);
				hFile = CreateFile(pWall->m_wsFullPath.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			}
			if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
			{
				void* pBuffer = ::GlobalLock(hGlobal);
				if (pBuffer)
				{
					WriteFile(hFile, pBuffer, dwLow, &dwHigh, NULL);
					::GlobalUnlock(hGlobal);
				}
				CloseHandle(hFile);
				hFile = NULL;
				::SendMessage(pWall->m_hReceiver, pWall->m_uiMsgFlag, 0, true);	// �ٴ��ػ�
			}
			else
			{
				SEND_LOG(L"ERROR wallPaper::threadCopyWallfile �޷���ǽֽ�ļ���write��������ϵͳ���ڻ��Ʊ�ֽ������");
				::SendMessage(pWall->m_hReceiver, pWall->m_uiMsgFlag, 0, false);	// �ٴ��ػ�

			}
			::GlobalFree(hGlobal);
			hGlobal = NULL;

		}
	}
	__except(gtc::app::sehFilterDefault(GetExceptionInformation(), L"wallPaper::threadCopyWallfile"))
	{
		WRITE_LOG(L"wallPaper::threadCopyWallfile SEH�쳣 ");
	}

	// 
	if(hGlobal) GlobalFree(hGlobal);
	CoUninitialize();
	SEND_LOG(L"NOTE wallPaper::threadCopyWallfile end ...");

	if(pthr->bAutofree) gtc::thread::free(pthr);
	return 1;
}
bool wallPaper::startMonitor(HWND hReceiver, UINT uMsg)
{
	assert(!m_hThread);
	if(m_hThread) return true;
	assert(hReceiver && uMsg > 0);
	if(!hReceiver || uMsg == 0) return false;
	init();
	m_hReceiver = hReceiver;
	m_uiMsgFlag = uMsg;
	// ע�����
	m_eventReg = CreateEvent(NULL, FALSE, TRUE, NULL);
	assert(m_eventReg);
	if(!m_eventReg) return false;
	if (gtc::app::isWindows7Later()/*gtc::app::isWindows8Later()*/)
	{
		if (m_regWall.open(HKEY_CURRENT_USER, L"Control Panel\\Desktop", KEY_NOTIFY|KEY_READ))
		{
			// ����Wallpaper��TranscodedImageCacheֵ�ı仯
			m_regWall.notifyChangeKeyValue(false, REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET, m_eventReg);
		}
	}
	//else if (gtc::app::isWindows7Later())
	//{
	//	if (m_regWall.open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Desktop\\General", KEY_NOTIFY|KEY_READ))
	//	{
	//		// ����WallpaperSourceֵ�ı仯
	//		m_regWall.notifyChangeKeyValue(false, REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET, m_eventReg);
	//	}
	//}
	else
	{
		if (m_regWall.open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Desktop\\General", KEY_NOTIFY|KEY_READ))
		{
			// ����WallpaperSource��WallpaperFileTime��WallpaperLocalFileTimeֵ�ı仯
			m_regWall.notifyChangeKeyValue(false, REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET, m_eventReg);
		}

	}
	// ���˳����ʱ��������Ϣ
	m_eventExit = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, threadMonitor, this, 0, NULL);
	//if (m_hThread)
	//{
	//	InterlockedCompareExchange(&m_lThreadSwitch, 1, 0);
	//	ResumeThread(m_hThread);
	//}

	return m_hThread != NULL;
}

void wallPaper::stopMonitor()
{
	//if(m_eventReg) SetEvent(m_eventReg);
	if(m_eventExit) SetEvent(m_eventExit);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	CloseHandle(m_eventExit);
	CloseHandle(m_eventReg);
	m_eventExit = NULL;
	m_eventReg = NULL;
	m_hThread = NULL;
	m_regWall.close();
}



}	// end acm