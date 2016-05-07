#ifndef __CURLWRAP_H__
#define __CURLWRAP_H__

/*****************************************************************************************
 *	��Ŀ	���libcurl����еķ�װ
 *
 *	2016-1-19		kp007		
 *	1��Ҫ����Ԥ����������BUILDING_LIBCURL�� !!!!!!!!!!!!!!!!!!!!!!!!
 *	2����Ҫ��������ws2_32.lib wldap32.lib libeay32.lib ssleay32.lib
 *	
 *	
 *	
 *****************************************************************************************/
#pragma once
#include <string>
#include "AC/baseException.h"
#include "AC/basedef.h"
#include "AC/stringUtil.h"
#include "curl/curl.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wldap32.lib")
//#pragma comment(lib,"D:\\NTools\\openssl\\libeay32.lib")
//#pragma comment(lib,"D:\\NTools\\openssl\\ssleay32.lib")
#if defined(_DEBUG)
//#	pragma comment(lib,"D:\\NTools\\curl\\libcurl_ssl_d.lib")
#	pragma comment(lib,"D:\\NTools\\curl\\libcurl_d.lib")

#else
//#	pragma comment(lib,"D:\\NTools\\curl\\libcurl_ssl.lib")
#	pragma comment(lib,"D:\\NTools\\curl\\libcurl.lib")
#endif
// ��Ҫ�ڡ���Ŀ����\C/C++\Ԥ��������BUILDING_LIBCURL
//#ifndef BUILDING_LIBCURL
//#define BUILDING_LIBCURL
//#endif
namespace gtc {

#define  CURL_ERRUSER_NORMAL		CURL_LAST+1000	// �Զ��������
static std::wstring _errorInfoCurl(int errCode);

#define CURL_EXCEPT(code) GTC_EXCEPT_EX(gtc::baseException::BSExceptionCodeCurlAssertionFailed, code, _errorInfoCurl(code))

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-9-14 Ҫpost�����ݸ�ʽ
enum BEPostDataFlag {
	BEPostDataFlagNormal		= 0x0001,		// �����ʽ��CURLFORM_PTRNAME, xxx, CURLFORM_PTRCONTENTS, yyy
	BEPostDataFlagData			= 0x0101,		// ���������ݣ�CURLFORM_PTRNAME����CURLFORM_BUFFER����CURLFORM_BUFFERPTR����CURLFORM_BUFFERLENGTH
	BEPostDataFlagFile			= 0x0201		// �ļ���CURLFORM_PTRNAME����CURLFORM_FILE
};

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-9-14 Ҫpost������
struct BSPostDataItem {
	BEPostDataFlag	dataFlag;		// ��ʽ
	std::string		strName;		// ����
	std::string		strData;		// ���ݣ���ͨ�ı� ���ļ�·�� �����������

	BSPostDataItem(const std::string &sname, const std::string &sdata, BEPostDataFlag flag = BEPostDataFlagNormal)
		: strName(sname), strData(sdata), dataFlag(flag)
	{}
	BSPostDataItem(const std::string &sname, const std::wstring &wsdata, BEPostDataFlag flag = BEPostDataFlagNormal)
		: strName(sname), strData(gtc::stringUtil::toChar(wsdata.c_str())), dataFlag(flag)
	{}
	BSPostDataItem(const std::string &sname, int idata, BEPostDataFlag flag = BEPostDataFlagNormal)
		: strName(sname), dataFlag(flag)
	{
		char pbuf[80] = {0};
		std::sprintf(pbuf, "%d", idata);
		strData = pbuf;
	}
	BSPostDataItem(const std::string &sname, const SBDataType &sdata)
		: strName(sname), dataFlag(BEPostDataFlagData)
	{
		strData.assign(sdata.cbegin(), sdata.cend());
	}
};
typedef std::vector<BSPostDataItem> PostDataArrayType;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	curl  ��		2015-7-7
//	����libcurl�ķ�װ��
//	1�������м�Ԥ�����BUILDING_LIBCURL
//	2�������м�Ԥ������ļ�ws2_32.lib��winmm.lib��wldap32.lib
////////////////////////////////////////////////////////////////////////////////////////////////////////
class curl
{
protected:
	class curl_ptr {
	public:
		curl_ptr() : _pcurl(NULL)
		{
			_pcurl = curl_easy_init();
			assert(_pcurl);
		}
		~curl_ptr()
		{
			if (_pcurl)
			{
				curl_easy_cleanup(_pcurl);
			}
		}
		operator CURL*() const 
		{
			return _pcurl;
		}
		operator bool() const 
		{
			return _pcurl != NULL;
		}
		CURL* get()
		{
			assert(_pcurl);
			return _pcurl;
		}
	private:
		curl_ptr(const curl_ptr &);
		curl_ptr& operator=(const curl_ptr &);
	private:
		CURL *_pcurl;
	};
public:
	curl();
	~curl();

public:
	// ��ʼ��libcurl����
	static void initEnvironment(long flags = CURL_GLOBAL_ALL);
	// ���libcurl����
	static void clearupEnvironment();

	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-7 ������ҳ
	static std::string downloadWeb(const std::string &webUrl, int timeout = 0);

	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-7-9 �����ļ�
	static SBDataType downloadFile(const std::string &fileUrl, int timeout = 0);

	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-9-7 post
	//static std::string post(const std::string &webUrl, const std::string &fields);
	static std::string post(const std::string &webUrl, const std::string &fields, int timeout = 0, PCSTR pCAinfo = NULL);
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-9-11 post ����multipart/formdata����	
	//////////////////////////////////////////////////////////////////////////////////////////
	static std::string post(const std::string &webUrl, const PostDataArrayType &postData, int timeout = 0, PCSTR pCAinfo = NULL);
	//	2015-9-8 get
	static std::string get(const std::string &webUrl);
protected:
	//static std::string mc_wrWebBuffer;		// downloadWeb��WRITEDATA��Ӧ����
	//static std::string mc_wrFileBuffer;		// downloadFile��WRITEDATA��Ӧ����
	//static std::string mc_rddataBuffer;		// READDATA��Ӧ����
};
}
#endif	// __CURLWRAP_H__