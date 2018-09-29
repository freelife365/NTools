#ifndef __BASEEXCEPTION_H__
#define __BASEEXCEPTION_H__

#include <exception>
#include <string>
namespace gtc {

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	baseException��	2015-1-8
//	�ҵ��쳣����
//	�ο���OGRE
////////////////////////////////////////////////////////////////////////////////////////////////////////
class baseException : public std::exception {
public:
	// �쳣�룬�����Զ�����쳣����Ӧһ��ֵ
	enum BSExceptionCode {
		BSExceptionCodeDefault					= 0,
		BSExceptionCodeCannotWriteToFile		= 1,		// д�ļ������豸��ʧ��
		BSExceptionCodeInvalidState				,			// ��������Ч״̬��������Ҫ�õ��Ķ���δ׼���ã�
		BSExceptionCodeInvalidParams			,			// ��Ч����
		BSExceptionCodeRenderingAPIError		,			// ��֧�ֵ��õ�API���� 
		BSExceptionCodeDuplicateItem			,			// �����Ѿ�����
		BSExceptionCodeItemNotFound				,			// ���󲻴���
		BSExceptionCodeFileNotFound				,			// �ļ�������
		BSExceptionCodeInternalError			,			// �ڲ����������豸�ڲ�����
		BSExceptionCodeParseFailed				,			// �����ݽ�����ת��ʧ��

		// ��Դ��
		BSExceptionCodeDBAssertionFailed		= 1000,		// ���ݿ����
		BSExceptionCodeCurlAssertionFailed		,			// ��libcurl�����
		BSExceptionCodeJSONAssertionFailed		,			// ��json�����
		BSExceptionCodeXUnzipFailed				,			// ��xunzip�����
		BSExceptionCodeXZipFailed				,			// ��xzip�����

		BSExceptionCodeNotImplemented						// δʵ�ֵĹ���
	};

protected:
	baseException(int number = 0);
public:
	baseException(int number, const std::wstring &desc, const wchar_t *pFunction);
	baseException(int number, const std::wstring &desc, const wchar_t *pType, const wchar_t *pFunction, const wchar_t *pFile, int line);
	baseException(const baseException &exc);
	baseException& operator=(const baseException &exc);
	~baseException() throw();


public:
	virtual const std::wstring& description() const;
	// Override std::exception::what
	const char* what() const throw();
public:
	virtual int getNumber() const throw() {					return m_iNumber;						}
	virtual const std::wstring& getType() const {			return m_wsType;						}
	virtual const std::wstring& getDescription() const {	return m_wsDescription;					}
	virtual int getLine() const {							return m_iLine;							}
	virtual const std::wstring& getFunction() const {		return m_wsFunction;					}
	virtual const std::wstring& getFile() const {			return m_wsFile;						}
protected:
	int				m_iNumber;					// ������
	std::wstring	m_wsType;					// ����
	std::wstring	m_wsDescription;			// ����
	mutable std::wstring m_wsFullDescription;	// 
	
	int				m_iLine;					// �쳣�������кţ���Ӧ __LINE__
	std::wstring	m_wsFile;					// �����ļ�����Ӧ __FILEW__
	std::wstring	m_wsFunction;					// ����Դ���纯��������Ӧ __FUNCTIONW__
	

};


//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-31 ���Ի��쳣������ʵ�ֺ�
#ifndef GTC_EXCEPT_IMPLEMENT
#define GTC_EXCEPT_IMPLEMENT(CLS, BASE, TYPE)	\
	class CLS : public BASE {	\
	public:						\
		CLS(int number, const std::wstring &description, const wchar_t *pFunction, const wchar_t *pfile, int line) \
			: BASE(number, description, TYPE, pFunction, pfile, line) \
		{} \
	};
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	  ��		2015-3-31
//	�쳣��ľ���ʵ��
////////////////////////////////////////////////////////////////////////////////////////////////////////
GTC_EXCEPT_IMPLEMENT(exceptionCannotWriteToFile,		baseException, L"exceptionCannotWriteToFile")
GTC_EXCEPT_IMPLEMENT(exceptionInvalidState,				baseException, L"exceptionInvalidState")
GTC_EXCEPT_IMPLEMENT(exceptionInvalidParams,			baseException, L"exceptionInvalidParams")
GTC_EXCEPT_IMPLEMENT(exceptionRenderingAPIError,		baseException, L"exceptionRenderingAPIError")
GTC_EXCEPT_IMPLEMENT(exceptionDuplicateItem,			baseException, L"exceptionDuplicateItem")
GTC_EXCEPT_IMPLEMENT(exceptionItemNotFound,				baseException, L"exceptionItemNotFound")
GTC_EXCEPT_IMPLEMENT(exceptionFileNotFound,				baseException, L"exceptionFileNotFound")
GTC_EXCEPT_IMPLEMENT(exceptionInternalError,			baseException, L"exceptionInternalError")
GTC_EXCEPT_IMPLEMENT(exceptionParseFailed,				baseException, L"exceptionParseFailed")

GTC_EXCEPT_IMPLEMENT(exceptionDBAssertionFailed,		baseException, L"exceptionDBAssertionFailed")
GTC_EXCEPT_IMPLEMENT(exceptionCurlAssertionFailed,		baseException, L"exceptionCurlAssertionFailed")
GTC_EXCEPT_IMPLEMENT(exceptionJSONAssertionFailed,		baseException, L"exceptionJSONAssertionFailed")
GTC_EXCEPT_IMPLEMENT(exceptionCodeXUnzipFailed,			baseException, L"exceptionCodeXUnzipFailed")
GTC_EXCEPT_IMPLEMENT(exceptionCodeXZipFailed,			baseException, L"exceptionCodeXZipFailed")

GTC_EXCEPT_IMPLEMENT(exceptionNotImplemented,			baseException, L"exceptionNotImplemented")


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	exceptionFactory  ��		2015-3-31
//	�쳣�����࣬�����׳��쳣
////////////////////////////////////////////////////////////////////////////////////////////////////////
class exceptionFactory {
private:
	exceptionFactory() {}
public:
	static void throwException(baseException::BSExceptionCode code, int iNumber, 
						const std::wstring &wsDesc, const wchar_t* function, const wchar_t* file, long line)
	{
		switch (code)
		{
		case baseException::BSExceptionCodeCannotWriteToFile:		// д�ļ������豸��ʧ��
			throw exceptionCannotWriteToFile(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeInvalidState:			// ��������Ч״̬��������Ҫ�õ��Ķ���δ׼���ã�
			throw exceptionInvalidState(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeInvalidParams:			// ��Ч����
			throw exceptionInvalidParams(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeRenderingAPIError:		// ��֧�ֵ��õ�API���� 
			throw exceptionRenderingAPIError(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeDuplicateItem:			// �����Ѿ�����
			throw exceptionDuplicateItem(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeItemNotFound:			// ���󲻴���
			throw exceptionItemNotFound(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeFileNotFound:			// �ļ�������
			throw exceptionFileNotFound(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeInternalError:			// �ڲ����������豸�ڲ�����
			throw exceptionInternalError(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeParseFailed:				// �����ݽ�����ת��ʧ��
			throw exceptionParseFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeDBAssertionFailed:		// ���ݿ����
			throw exceptionDBAssertionFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeCurlAssertionFailed:		// libcurl
			throw exceptionCurlAssertionFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeJSONAssertionFailed:		// json
			throw exceptionJSONAssertionFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeXUnzipFailed:			// ��xunzip�����
			throw exceptionCodeXUnzipFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeXZipFailed:				// ��xzip�����
			throw exceptionCodeXZipFailed(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeNotImplemented:			// δʵ�ֵĹ���
			throw exceptionNotImplemented(iNumber, wsDesc, function, file, line);
			break;
		case baseException::BSExceptionCodeDefault:
		default:
			throw baseException(iNumber, wsDesc, function, L"baseException", file, line);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-3-31 ������ʹ�õ��쳣��
#ifndef GTC_EXCEPT
#define GTC_EXCEPT_DEFAULT(desc, code)				gtc::exceptionFactory::throwException(gtc::baseException::BSExceptionCodeDefault, code, desc, __FUNCTIONW__, __FILEW__, __LINE__)
#define GTC_EXCEPT(typeCode, desc)					gtc::exceptionFactory::throwException(typeCode, typeCode, desc, __FUNCTIONW__, __FILEW__, __LINE__)
#define GTC_EXCEPT_EX(typeCode, errorNumber, desc)	gtc::exceptionFactory::throwException(typeCode, errorNumber, desc, __FUNCTIONW__, __FILEW__, __LINE__)
#else
#define GTC_EXCEPT_EX(typeCode, errorNumber, desc)	GTC_EXCEPT(typeCode, desc)
#endif
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////	idlNode��	2015-1-8
////	ͨ���쳣��꣬�ο�POCO 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define GTC_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
//	class CLS: public BASE														\
//	{																				\
//	public:																			\
//		CLS(int code = CODE);														\
//		CLS(const std::wstring& msg, int code = CODE);								\
//		CLS(const std::wstring& msg, const std::wstring& arg, int code = CODE);		\
//		CLS(const std::wstring& msg, const baseException& exc, int code = CODE);	\
//		CLS(const CLS& exc);														\
//		~CLS() throw();																\
//		CLS& operator = (const CLS& exc);											\
//		const char* name() const throw();											\
//		const char* className() const throw();										\
//		baseException* clone() const;												\
//		void rethrow() const;														\
//	};
//
//#define GTC_DECLARE_EXCEPTION(CLS, BASE) \
//	GTC_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)
//
//#define GTC_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
//	CLS::CLS(int code): BASE(code)																	\
//	{																								\
//	}																								\
//	CLS::CLS(const std::wstring& msg, int code): BASE(msg, code)										\
//	{																								\
//	}																								\
//	CLS::CLS(const std::wstring& msg, const std::wstring& arg, int code): BASE(msg, arg, code)		\
//	{																								\
//	}																								\
//	CLS::CLS(const std::wstring& msg, const baseException& exc, int code): BASE(msg, exc, code)	\
//	{																								\
//	}																								\
//	CLS::CLS(const CLS& exc): BASE(exc)																\
//	{																								\
//	}																								\
//	CLS::~CLS() throw()																				\
//	{																								\
//	}																								\
//	CLS& CLS::operator = (const CLS& exc)															\
//	{																								\
//		BASE::operator = (exc);																		\
//		return *this;																				\
//	}																								\
//	const char* CLS::name() const throw()															\
//	{																								\
//		return NAME;																				\
//	}																								\
//	const char* CLS::className() const throw()														\
//	{																								\
//		return typeid(*this).name();																\
//	}																								\
//	baseException* CLS::clone() const																\
//	{																								\
//		return new CLS(*this);																		\
//	}																								\
//	void CLS::rethrow() const																		\
//	{																								\
//		throw *this;																				\
//	}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
////	ͨ���쳣��	2015-1-8
//GTC_DECLARE_EXCEPTION(logicException, baseException)
//GTC_DECLARE_EXCEPTION(runtimeException, baseException)
//GTC_DECLARE_EXCEPTION(formatException, baseException)
//GTC_DECLARE_EXCEPTION(applicationException, baseException)



}
#endif