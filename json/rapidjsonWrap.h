#ifndef __RAPIDJSONWRAP_H__
#define __RAPIDJSONWRAP_H__
#include "AC/basedef.h"
#include "json/rapidjson/document.h"

namespace gtc {
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-29 ������
typedef enum {
	BEJsonCodeOk					= 0,	// �ɹ�
	BEJsonCodeInvalidArgument		,		// ��Ч����
	BEJsonCodeKeyNotFound			,		// ���󲻴���
	BEJsonCodeTypeError				,		// ���Ͳ�ƥ��
}BEJsonCode;

static std::wstring _errorInfoJson(BEJsonCode code);

#define JSON_EXCEPT2(code, pdesc) GTC_EXCEPT_EX(gtc::baseException::BSExceptionCodeJSONAssertionFailed, code, pdesc)
#define JSON_EXCEPT(code) GTC_EXCEPT_EX(gtc::baseException::BSExceptionCodeJSONAssertionFailed, code, _errorInfoJson(code))

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	rapidJsonWrap  ��		2015-10-29
//	���rapidjson�ķ�װ
////////////////////////////////////////////////////////////////////////////////////////////////////////
class rapidJsonWrap
{
private:
	rapidJsonWrap();
	rapidJsonWrap(const rapidJsonWrap &rjson);
	rapidJsonWrap& operator=(const rapidJsonWrap &rjson);
public:
	explicit rapidJsonWrap(const std::string &strjson);
	explicit rapidJsonWrap(rapidjson::Value *pVal);

	//bool emptyDoc() const {				return m_doc.Empty();								}
	/****************************************************************************************
		<< --- 	selectObject	2015-10-29 --- >>         
		˵������ָ��·��ѡ�������Ϊ��ǰ��������
		������
			strPath	= ����·������key�á�:���ָ�磺key1:key2:key3
		����ֵ�������Ƿ�ѡ��
	*****************************************************************************************/
	rapidjson::Value* selectObject(const std::string &strPath);
	rapidjson::Value* selectObject(const SStringArrayType &vecObjectPath);
	void selectEmpty();

	/****************************************************************************************
		<< --- 	nextItem	2015-10-29 --- >>         
		˵�����Ƶ�����������һ��ԭʼ
		������
		����ֵ���Ƿ����
	*****************************************************************************************/
	rapidjson::Value* nextItem();
	/****************************************************************************************
		<< --- 	asXXX	2015-10-29 --- >>         
		˵��������asXXX�������m_pVal���󣬵õ�������Ķ�Ӧֵ
		������
			strKey	= Ҫ����ֵ��key���ƣ���������Զ༶����Ŷ���磺key4:key5
		����ֵ��
	*****************************************************************************************/
	bool asBoolean(const std::string &strKey);
	int asInt(const std::string &strKey);
	unsigned int asUInt(const std::string &strKey);
	int64_t asInt64(const std::string &strKey);
	uint64_t asUInt64(const std::string &strKey);
	double asDouble(const std::string &strKey);
	std::wstring asString(const std::string &strKey, UINT codePage = CP_UTF8);
//protected:
	// �ӵ�ǰ����pCurVal�в���key��=strKey�Ķ���
	static rapidjson::Value* nextObject(rapidjson::Value *pCurVal, const std::string &strKey);

	rapidjson::Value* findValue(const std::string &strKey);
private:
	rapidjson::Document m_doc;
	rapidjson::Value	*m_pVal;

	rapidjson::Value	*m_pArrayVal;
	int					m_iValCurIndex;

};



} // gtc
#endif // __RAPIDJSONWRAP_H__