#ifndef __FILE_H__
#define __FILE_H__
#pragma once
#include "sysupport.h"

namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	file��	2014-04-25
//	�����ļ�������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class file
{
public:
	// ��Ŀ�귢������ʱ���Ҳ�����ǰ���FOF_RENAMEONCOLLISIONʱ�������ص������Գ����¾��ļ���
	typedef bool (CALLBACK *ENUMFILEMAPPROC)(LPSHNAMEMAPPING, LPVOID);
private:
	struct HANDLETOMAPPINGS 
	{
		UINT              uNumberOfMappings;  // Number of mappings in the array.
		LPSHNAMEMAPPING   lpSHNameMapping;    // Pointer to the array of mappings.
	};
	// �ļ�����ʱĿ�����������Ҳ�����ǰ���FOF_RENAMEONCOLLISIONʱ���ñ�����
	static int WINAPI _EnumFileMapping(HANDLE hNameMappings, ENUMFILEMAPPROC lpfnFileMapProc, LPVOID pData);
public:
	/*****************************************************************************************
		<< --- static app::message		2014-04-25 --- >>
		˵�������ѡ���ļ�
		������
		vpinfo		= Ҫ��ʾ����Ϣ
		vptitle		= ��Ϣ��ı���
		vtype		= ��Ϣ�������ť����
		viswriter	= �Ƿ���Ϣд����־�ļ�
		����ֵ��ѡ�е����ť���
	*****************************************************************************************/
	static std::wstring getAppPath(void)
	{
		wchar_t str[MAX_PATH];
		std::wstring path;
#if defined(_UNICODE)
		path = _wpgmptr;
#else
		path = convert::toWChar(_pgmptr);
#endif
		uint32_t len = 0;
		// �õ�ϵͳ����
	#if defined(_WIN32)
		len = ::GetModuleFileNameW(nullptr, str, MAX_PATH);
		str[len] = '\0';
	#endif
		path = str;
		path = path.substr(0, path.rfind('\\'));//.Left(path.ReverseFind('\\'));
		return path;
	}
	/*****************************************************************************************
		<< --- static file::removeLog		2014-04-30 --- >>
		˵����ɾ����ǰ����־�ļ�
		������
		pfullfile	= �ļ�ȫ·��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool remove(const wchar_t *pfullfile)
	{
		return std::remove(convert::toChar(pfullfile).c_str()) == 0;
	}
	/*****************************************************************************************
		<< --- static file::removeLog		2014-04-30 --- >>
		˵����ɾ����ǰ����־�ļ�
		������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	bool removeLog(const wchar_t *pnameflag = L"app")
	{
		std::wstring path(getAppPath());
		char stimer[40] = {0};
		char logname[40] = {0};

		std::time_t tm = {0};
		std::time(&tm);
		std::tm *ptminfo = std::localtime(&tm);
		assert(ptminfo);
		std::strftime(stimer, sizeof(stimer), "%Y-%m-%d %H:%M:%S", ptminfo);
		std::strftime(logname, sizeof(logname), "%Y%m%d.ino", ptminfo);
		path += L"\\";
		path += _NDEF_DIR_GTDATA;
		path += L"\\";
		path += _NDEF_DIR_GTLOG;
		path += L"\\";
		path += pnameflag;
		path += convert::toWChar(logname);

		return remove(path.c_str());
	}
	/*****************************************************************************************
		<< --- static file::size		2014-04-25 --- >>
		˵������ȡ�ļ��ĳߴ磨��λ���ֽڣ�
		������
		pfullfile	= �����ļ�ȫ·��
		����ֵ��
			>= 0���ļ����ȣ���λ���ֽڣ�
			< 0	��ʧ��
	*****************************************************************************************/
	static int64_t size(const std::wstring &fileName)
	{
		return size(fileName.c_str());
	}
	static int64_t size(const wchar_t *pfullfile)
	{
		int64_t len = -1;
		std::ifstream file(pfullfile, std::ifstream::in | std::ifstream::_Nocreate);
		if(file)
		{
			file.seekg(0, file.end);
			len = file.tellg();
			file.close();
		}
		return len;
	}

	/*****************************************************************************************
		<< --- static file::read		2014-04-25 --- >>
		˵�������ļ������Զ�������ʽ���浽�ֽ�����verdata��
		������
		pfullfile	= �����ļ�ȫ·��
		vecdata		= ��������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool read(const wchar_t *pfullfile, SBDataType &vecdata)
	{
		int64_t len = size(pfullfile);
		std::ifstream file(pfullfile, std::ifstream::in | std::ifstream::binary);
		if(file)
		{
			if(len > 0)
			{
				vecdata.resize((uint32_t)len);
				file.read((char*)vecdata.data(), len);
			}
			file.close();
			return true;
		}
		return false;

	}
	static bool read(const wchar_t *pfullfile, std::string &strData)
	{
		if(!::PathFileExists(pfullfile)) return false;
		int64_t len = size(pfullfile);
		std::ifstream file(pfullfile);
		if (file)
		{
			if(len > 0)
			{
				strData.resize((uint32_t)len);
				file.read((char*)strData.data(), len);
			}

		}
		return len > 0;
	}
	/*****************************************************************************************
		<< --- static file::write		2014-04-25 --- >>
		˵�����������Զ�������ʽд���ļ�
		������
		pfullfile	= �����ļ�ȫ·��
		vecdata		= ����������
		isAppend	= true:������׷�ӵ��ļ�β����false:����ԭ�ļ�������ļ������ھʹ������ļ�
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool write(const wchar_t *pfullfile, const char *pData, std::size_t dataLen, bool isAppend = false)
	{
		assert(pfullfile);
		assert(pData);
		std::ofstream::openmode mode = isAppend ?std::ofstream::app :std::ofstream::trunc;
		std::ofstream file(pfullfile, mode | std::ifstream::binary | std::ifstream::in);
		if(file)
		{
			file.write(pData, dataLen);
			file.close();
			return true;
		}
		return false;

	}
	static bool write(const wchar_t *pfullfile, const std::vector<uint8_t> &vecdata, bool isAppend = false)
	{	
		return write(pfullfile, (const char *)vecdata.data(), vecdata.size(), isAppend);
		//std::ofstream::openmode mode = isAppend ?std::ofstream::app :std::ofstream::trunc;
		//std::ofstream file(pfullfile, mode | std::ifstream::binary | std::ifstream::in);
		//if(file)
		//{
		//	file.write((const char *)vecdata.data(), vecdata.size());
		//	file.close();
		//	return true;
		//}
		//return false;
	}
	/*****************************************************************************************
		<< --- static file::writeLine		2014-04-25 --- >>
		˵�������ļ�������һ������
		������
		pfullfile	= �����ļ�ȫ·��
		pdata		= ��д����ַ���
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool writeLine(const wchar_t *pfullfile, const wchar_t *pdata)
	{
		std::wofstream file(pfullfile, std::wofstream::app | std::wofstream::out);
		if(file)
		{
			// ֧���������
			file.imbue(std::locale("", std::locale::all ^ std::locale::numeric));
			file << pdata << std::endl;
			file.close();
			return true;
		}
		return false;
	}
	/*****************************************************************************************
		<< --- static file::existsDirectory		2014-04-25 --- >>
		˵��������Ŀ¼���ļ��Ƿ����
		������
		vpdirectory	= �������Ŀ¼���ļ�
		����ֵ��
			true	��Ŀ¼����
			false	��������
	*****************************************************************************************/
	static bool exists(const wchar_t *vpdirOrFile)
	{
		assert(vpdirOrFile);
		return ::PathFileExists(vpdirOrFile) == TRUE;
	//	bool bl = false;
	//#if defined(_WIN32)
	//	HANDLE hfind = NULL;
	//	WIN32_FIND_DATAW data = {0};
	//	try
	//	{
	//		hfind = ::FindFirstFileW(vpdirectory, &data);
	//		if(hfind == INVALID_HANDLE_VALUE || hfind == (HANDLE)(LONG)INVALID_FILE_SIZE)
	//			bl = false;
	//		else
	//			bl = true;
	//	}
	//	catch(...)
	//	{
	//		bl =false;
	//	}
	//	if(hfind != NULL)
	//		FindClose(hfind);
	//#endif
	//	return bl;
	}
	/****************************************************************************************
		<< --- fileAttributeInfo		2014-11-10 --- >>         
		˵�����õ��ļ����ļ���������Ϣ
		������
			pFullPath		= �ļ������ļ��У�ȫ·��
			fileAttribute	= �������ļ�������Ϣ
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool fileAttributeInfo(PCWSTR pFullPath, BSFileAttribute &fileAttribute);
	/*****************************************************************************************
		<< --- static file::browseFile		2014-04-25 --- >>
		˵�������ѡ���ļ�
		������
		vpfilter	= ������ļ�������
		pname		= ��Ϊ��ʱ������ѡ���ļ����ļ�����������չ����
		pext		= ��Ϊ��ʱ������ѡ���ļ�����չ��
		����ֵ��ȫ·���ļ���
	*****************************************************************************************/
	static std::wstring browseFile(const wchar_t *vpfilter, std::wstring *pname = nullptr, std::wstring *pext = nullptr)
	{
		std::wstring file;
#if defined(_AFXDLL)
#if defined(_UNICODE)
		CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,vpfilter);
		if(dlg.DoModal() == IDOK)
		{
			if(pext)
				*pext = dlg.GetFileExt().GetBuffer();
			if(pname)
			{
				file = dlg.GetFileName().GetBuffer();
				*pname = file.substr(0, file.find('.'));
			}
			file = dlg.GetPathName().GetBuffer();
		}
#else
		CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, convert::toChar(vpfilter).c_str());
		if(dlg.DoModal() == IDOK)
		{
			if(pext)
				*pext = convert::toWChar(dlg.GetFileExt().GetBuffer());
			if(pname)
			{
				file = convert::toWChar(dlg.GetFileName().GetBuffer());
				*pname = file.substr(0, file.find('.'));
			}
			file = convert::toWChar(dlg.GetPathName().GetBuffer());
		}
#endif
#endif
		return file;
	}

	/****************************************************************************************
		<< --- 	modifyFileNameBaseNumber	2015-2-26 --- >>         
		˵����������������ļ���
		������
			wsFullPath	= ��Ҫ�������ļ�ȫ·����������չ��
			pSuffix		= �������ĺ�׺��һ��Ϊ�ļ���չ�����磺.lnk
		����ֵ����������ļ���ȫ·��
	*****************************************************************************************/
	static std::wstring modifyFileNameBaseNumber(const std::wstring &wsFullPath, PCWSTR pSuffix = NULL);
	/****************************************************************************************
		<< --- 	updateFileTime	2015-2-27 --- >>         
		˵���������ļ�ʱ��
		������
			wsFullPath	= ���������ļ�
			timeFlag	= ��Ҫ�����Ĳ���ʱ��
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool updateFileTime(const std::wstring &wsFullPath, UINT timeFlag = FILE_ATTRIBUTE_ALL_TIME);
	/****************************************************************************************
		<< --- 		2015-3-17 --- >>         
		˵���������ļ�����ȡ�ļ�������չ������·��
		������
			wsFullPath			= �������ļ���Ŀ¼��ȫ·��
			wsName				= ����ȡ���ļ������ļ���
			poutExtName			= ��Ϊ��ʱ��������չ�����磺.txt
			poutParentFolder	= ��Ϊ��ʱ�������ļ����ļ��еĸ�Ŀ¼
		����ֵ��
	*****************************************************************************************/
	static bool analysisFilePath(std::wstring wsFullPath, std::wstring &wsName
		, std::wstring *poutExtName = NULL, std::wstring *poutParentFolder = NULL);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-18 ��ģ���ļ������ļ������ڣ�	1)%ALLUSERSPROFILE%\Templates 2)%USERPROFILE%\Templates 3)%SystemRoot%\shellnew
	static HANDLE openTemplateFile(PCWSTR pfileName);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-3-16 ��������ʼ��FileOperation*�ӿ�
	static bool createAndInitializeFileOperation(REFIID riid, void **ppv);
	/****************************************************************************************
		<< --- 	createFolder	2015-3-16 --- >>         
		˵������ָ���ļ����ڴ����µ��ļ��С�createFolderEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pRootName	= ��Ŀ¼�������ļ��еĸ�Ŀ¼
			pfolderName	= ���ļ��е�����
			nFlag		= �������
			poutNew		= ��Ϊ��ʱ�������½��ļ���IShellItem*
		����ֵ�������½��ļ�ȫ·��
	*****************************************************************************************/
	static std::wstring createFolder(PCWSTR pRootName, PCWSTR pfolderName, DWORD nFlag = FOF_RENAMEONCOLLISION);
	static bool createFolderEx(PCWSTR pRootName, PCWSTR pfolderName, DWORD nFlag = FOF_RENAMEONCOLLISION, IShellItem **poutNew = NULL);

	/****************************************************************************************
		<< --- 	createFile	2015-3-16 --- >>         
		˵������ָ���ļ����ڴ����µ��ļ���createFileEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pRootName		= ��Ŀ¼�������ļ��еĸ�Ŀ¼
			pfileName		= ���ļ������ƣ��磺newfile.txt
			pTemplateName	= �ο���ģ���ļ��������Դ�ģ���ļ������ݴ������ļ���
								���ļ������ڣ�	1)%ALLUSERSPROFILE%\Templates
											2)%USERPROFILE%\Templates
											3)%SystemRoot%\shellnew
			nFlag			= �������
			poutNew			= ��Ϊ��ʱ�������½��ļ���IShellItem*
		����ֵ���Ƿ񴴽��ɹ�
	*****************************************************************************************/
	static bool createFile(PCWSTR pRootName, PCWSTR pfileName, PCWSTR pTemplateName = NULL, DWORD nFlag = FOF_RENAMEONCOLLISION, IShellItem **poutNew = NULL);
	static bool createFileEx(PCWSTR pRootName, PCWSTR pfileName, PCWSTR pTemplateName = NULL, DWORD nFlag = FOF_RENAMEONCOLLISION, IShellItem **poutNew = NULL);

	/****************************************************************************************
		<< --- 	copyFile	2015-3-17 --- >>         
		˵���������ļ����ļ��С�copyFileEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pToFolder	= Ŀ���ļ������ļ���
			pName		= Ŀ���ļ����ļ��е�����
			pFrom		= Դ�ļ����ļ���
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool copyFile(PCWSTR pToFolder, PCWSTR pName, PCWSTR pFrom, DWORD nFlag = FOF_ALLOWUNDO);
	static bool copyFileEx(PCWSTR pToFolder, PCWSTR pName, PCWSTR pFrom, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	copyFiles	2015-3-17 --- >>         
		˵����һ�θ��ƶ���ļ���copyFilesEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pToFolder	= Ŀ���ļ���
			vecFiles	= ���������ļ��л��ļ�����
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool copyFiles(PCWSTR pToFolder, StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO, ENUMFILEMAPPROC plfnFileMapProc = NULL, LPVOID pData = NULL);
	static bool copyFilesEx(PCWSTR pToFolder, StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	moveFile	2015-3-17 --- >>         
		˵�����ƶ��ļ���moveFileEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pToFolder	= Ŀ���ļ���
			pFrom		= ���ƶ����ļ����ļ���
			pNewName	= ��Ϊ��ʱ��ָ��һ�����ļ���
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool moveFile(PCWSTR pToFolder, PCWSTR pFrom, PCWSTR pNewName = NULL, DWORD nFlag = FOF_ALLOWUNDO);
	static bool moveFileEx(PCWSTR pToFolder, PCWSTR pFrom, PCWSTR pNewName = NULL, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	moveFiles	2015-3-17 --- >>         
		˵����һ���ƶ�����ļ���moveFilesEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pToFolder	= Ŀ���ļ���
			vecFiles	= ���������ļ��л��ļ�����
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool moveFiles(PCWSTR pToFolder, StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO);
	static bool moveFilesEx(PCWSTR pToFolder, StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	deleteFile	2015-3-17 --- >>         
		˵����ɾ��ָ���ļ����ļ��С�deleteFileEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pFrom		= ��ɾ�����ļ����ļ���
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool deleteFile(PCWSTR pFrom, DWORD nFlag = FOF_ALLOWUNDO|FOF_NOCONFIRMATION);
	static bool deleteFileEx(PCWSTR pFrom, DWORD nFlag = FOF_ALLOWUNDO|FOF_NOCONFIRMATION);

	/****************************************************************************************
		<< --- 	deleteFiles	2015-3-17 --- >>         
		˵����һ��ɾ������ļ���deleteFilesEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			vecFiles	= ���������ļ��л��ļ�����
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool deleteFiles(StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO|FOF_NOCONFIRMATION);
	static bool deleteFilesEx(StringArrayType &vecFiles, DWORD nFlag = FOF_ALLOWUNDO|FOF_NOCONFIRMATION);

	/****************************************************************************************
		<< --- 	renameFile	2015-3-17 --- >>         
		˵�����������ļ����ļ��С�renameFileEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
		������
			pFrom		= �����������ļ����ļ���
			pNewName	= ��Ϊ��ʱ��ָ��һ�����ļ���
			nFlag		= ������ǣ�FOF_RENAMEONCOLLISION�����Ŀ���ļ����ڣ����Զ�����ļ������磺xxx (2).txt
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool renameFile(PCWSTR pFrom, PCWSTR pNewName, DWORD nFlag = FOF_ALLOWUNDO);
	static bool renameFileEx(PCWSTR pFrom, PCWSTR pNewName, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	renameFiles	2015-3-17 --- >>         
		˵����һ������������ļ���renameFilesEx����IFileOperationʵ�֣�xpϵͳ��֧�֡�
			ִ�к���ļ���Ϊnewfile(1).txt, newfile(2).txt, and newfile(3).txt
		������
			vecFiles	= ���������ļ��л��ļ�����
			pNewName	= �µ�����
			nFlag		= �������
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool renameFiles(StringArrayType &vecFiles, PCWSTR pNewName, DWORD nFlag = FOF_ALLOWUNDO);
	static bool renameFilesEx(StringArrayType &vecFiles, PCWSTR pNewName, DWORD nFlag = FOF_ALLOWUNDO);

	/****************************************************************************************
		<< --- 	createShortCut	2015-3-18 --- >>         
		˵���������ļ����ļ��еĿ�ݷ�ʽ
		������
			pToFolder		= ��ݼ��ļ��������ļ���
			pLnkName		= ��ݼ��ļ����������ͬ���ļ��������Զ������ļ���
			ptargetFilePath	= ��ݼ��ļ�����ʾ��Ŀ���ļ����ļ���
			pParam			= �����б����
			pIconFilePath	= ��ݼ�ͼ�������ļ�
			iconIndex		= ͼ���ļ��е�����
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool createShortCut(PCWSTR pToFolder, PCWSTR pLnkName, PCWSTR ptargetFilePath
		, PCWSTR pParam = NULL, PCWSTR pIconFilePath = NULL, int iconIndex = 0);
	static bool createShortCut(PCWSTR pToFolder, PCWSTR ptargetFilePath);
	static bool createShortCuts(PCWSTR pToFolder, StringArrayType &vecTargetFiles);
	/****************************************************************************************
		<< --- 	undoDelete	2015-5-12 --- >>         
		˵�����ָ���ɾ�����ļ�
		������
			vecFiles		= ��Ҫ����ɾ�����ļ�
			dtThisLaterUTC	= ��ʱ����Ժ�ɾ�����ļ��ſɱ��ָ�
		����ֵ���Ƿ�ִ�гɹ�
	*****************************************************************************************/
	static bool undoDelete(StringArrayType &vecFiles, DATE dtThisLaterUTC);
};

}
#endif	// end __FILE_H__