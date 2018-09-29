#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__
#include <string>

// ��־�����������Ϣ
#define DBGER_WND_MAIN_CLASS		L"debugerWndClass"
#define DBGER_WND_MAIN_CAPTION		L"debugerWndCaption"
namespace gtc {
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-9-22 ��־����	
enum BELogType {
	BELogTypeNormal,		// ���������Ϣ
	BELogTypeNote,			// ע��
	BELogTypeWarning,		// ����
	BELogTypeError			// ����
};
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-9-22 ͨ��WM_COPYDATA��Ϣ���ݵ����ݽṹ
#define DBGER_MAX_LOG_LEN	1024
#define DBGER_LOG_TYPE		2			// 
struct BSLogData {
	BELogType	beType;
	wchar_t		pLogDesc[DBGER_MAX_LOG_LEN];
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	debugger��	2015-1-8
//	���������Ϣ�����õ��Զϵ㣬���������־
////////////////////////////////////////////////////////////////////////////////////////////////////////
class debugger {
private:
	debugger(){}
public:
	/// Returns true if a debugger is available, false otherwise.
	/// On Windows, this function uses the IsDebuggerPresent()
	static bool isAvailable();

	/// Writes a message to the debugger log, if available, otherwise to
	/// standard error output.
	//static void message(const std::wstring &msg);
	//static void message(const std::string &msg);
	static void message(const wchar_t *pformat, ...);
	// д��־�ļ�
	static void write(const wchar_t *pformat, ...);
	// ����־���͵����Խ���
	static void send(const wchar_t *pformat, ...);
	//static void send_tick(const wchar_t *pformat, ...);
	static void sendA(const char *pformat, ...);
	//static void send(BELogType logType, const wchar_t *pformat, ...);
	//static void sendA(BELogType logType, const char *pformat, ...);
	/// Writes a message to the debugger log, if available, otherwise to
	/// standard error output.
	//static void message(const std::string &msg, const char *file, int line);

	/// Breaks into the debugger, if it is available.
	/// On Windows, this is done using the DebugBreak() function.
	static void enter();

	/// Writes a debug message to the debugger log and breaks into it.
	static void enter(const std::wstring &msg);

	/// Writes a debug message to the debugger log and breaks into it.
	static void enter(const std::wstring &msg, const char *file, int line);

	/// Writes a debug message to the debugger log and breaks into it.
	static void enter(const char *file, int line);


protected:
	// Ԥ������Ϣ��ʽ
	static BELogType preprocessFormat(const wchar_t *pformat, int &offset);
	static BELogType preprocessFormat(const char *pformat, int &offset);
};



} // end gtc



#endif