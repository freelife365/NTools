#ifndef __BASE64_H__
#define __BASE64_H__
#pragma once
#include "basedef.h"


namespace gtc 
{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	base64	�� 2015-12-15	kp007
//	base64�����װ��
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class base64
{
private:
	base64();
	base64(const base64 &);
	base64& operator=(const base64 &);
public:
	/****************************************************************************************
		<< --- encode	2015-12-15	kp007 --- >>         
		˵����base64����
		������
			pinData	= �����������
			inLen	= pinData���������ݳ���
		����ֵ������������
	*****************************************************************************************/
	static std::string encode(const uint8_t *pinData, std::size_t inLen);
	static std::string encode(const std::string &inData);
	static std::wstring encodeW(const std::string &inData);
	static std::wstring encode(const std::wstring &inData);
	static std::string encode(const SBDataType &sbData);
	static std::wstring encodeW(const SBDataType &sbData);

	static std::string decode(const char *pinData, std::size_t inLen);
	static std::string decode(const std::string &inData);
	static std::wstring decode(const std::wstring &inData);
	static SBDataType decodeToData(const char *pinData, std::size_t inLen);
	static SBDataType decodeToData(const std::string &inData);
	static SBDataType decodeToData(const std::wstring &inData);

};

}	// gtc
#endif	// __BASE64_H__