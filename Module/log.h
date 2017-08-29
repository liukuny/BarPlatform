//******************************************************************************
// 日志文件按日存储 
//******************************************************************************
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <fstream>
#include <fcntl.h>
#include <sys/types.h>
#include <windows.h>

#include "datetime.h"

//#include <sys/file.h>
//#include <pthread.h>
//
//#include <sys/syscall.h>	// syscall(__NR_gettid)
//
//#define gettid() syscall(__NR_gettid)
//


//#include "exception.h"
//
#include <iostream>		// cout, cin
using namespace std; 

// 日志文件相对可执行文件的路径
#define IM_LOG_FILENAME_JSONCODE      "jsoncode.log"
#define IM_LOG_FILENAME_IMAPPCLIENT   "imapp.log"
#define IM_LOG_FILENAME_MATCHAPPCLIENT "matchapp.log"
#define IM_LOG_FILENAME_CONTROLAPPCLIENT "controlapp.log"
#define IM_LOG_FILENAME_CHANNEL       "channel.log"
#define IM_LOG_FILENAME_APPFACTORY    "appfactory.log"
#define IM_LOG_FILENAME_WEBSOCK       "websock.log"
#define IM_LOG_FILENAME_SOCKET        "socket.log"
#define IM_LOG_FILENAME_INTERFACE     "interface.log"
#define IM_LOG_FILENAME_WORK          "work.log"
#define IM_LOG_FILENAME_CONTROL       "control.log"
#define IM_LOG_FILENAME_CLIENT        "client.log"
#define IM_LOG_FILENAME_ONLINELIST    "onlinelist.log"
#define IM_LOG_FILENAME_TCPSERVER	  "tcpserver.log"
#define IM_LOG_FILENAME_PINGSERVER	  "ping.log"
#define IM_LOG_FILENAME_SCANSERVER	  "scan.log"
#define IM_LOG_FILENAME_DOWNLOADRES	  "download.log"
#define IM_LOG_FILENAME_DB			  "db.log"
#define IM_LOG_FILENAME_MSG			  "msg.log"
#define IM_LOG_FILENAME_MATRIX			"Matrix.log"
#define IM_LOG_FILENAME_SENDRECVSERVER	"sendrecvserver.log"
#define IM_LOG_FILENAME_SENDAPPSERVER	"sendappserver.log"
#define IM_LOG_ERROR				  "error.log"

////////////////////////////////////////////////////////////////////////////////
// class CLogStream - 日志文件流类

template<class T>
class CLogStream : public ofstream
{
public:
	static T *GetInstance()
	{
		if (!T::m_pInstance)
		{
			T::m_pInstance = new T;
		}
		return T::m_pInstance;
	}
	static void FreeInstance()
	{
		if (T::m_pInstance)
		{
			delete T::m_pInstance;
			T::m_pInstance = 0;
		}
	}
	static void WriteLog(const char *pFormatString, ...)
	{
		T *p = GetInstance();
		p->Lock();
		try
		{
			p->ConnectFile();
			string strText;

			va_list argList;
			va_start(argList, pFormatString);
			FormatStringV(strText, pFormatString, argList);
			va_end(argList);

			p->WriteTimeStamp();
			p->WriteTid();
			*p << "<" << strText.c_str() << ">\r\n" << endl;
			p->close();
			p->m_bOpen = false;

		}
		catch (...){}
		p->UnLock();
	}

	static void WriteLog(const string &strLog)
	{
		T *p = GetInstance();
		string s = strLog;
		s += "%s";
		p->WriteLog(s.c_str(), "");
	}

	static void WriteLogW(const wchar_t *pFormatString, ...)
	{
		T *p = GetInstance();
		p->Lock();
		try
		{
			p->ConnectFile();
			wstring strText;
			string strTextA;

			va_list argList;
			va_start(argList, pFormatString);
			FormatStringWV(strText, pFormatString, argList);
			va_end(argList);

			p->WriteTimeStamp();
			p->WriteTid();
			strTextA = WideCharToMultiCharW(strText.c_str(), strText.size());
			*p << "<" << strTextA.c_str() << ">\r\n" << endl;
			p->close();
			p->m_bOpen = false;

		}
		catch (...){}
		p->UnLock();
	}

protected:
	CLogStream(string &strLogName): m_strLogName(strLogName), m_bOpen(false)
	{
		InitializeCriticalSection(&m_lock);
		ConnectFile(); 
		Write("======================begin==========================");
		Write("编译时间：%s %s", __DATE__, __TIME__);
	}
	CLogStream(const char *pLogFile): m_strLogName(pLogFile), m_bOpen(false)
	{
		InitializeCriticalSection(&m_lock);
		ConnectFile(); 
		Write("======================begin==========================");
		Write("编译时间：%s %s", __DATE__, __TIME__);
	}
	~CLogStream(){ DeleteCriticalSection(&m_lock); }
    void Open(const char *pLogFile)
	{
		open(pLogFile, ios::out | ios::app);
		m_bOpen = true;
	}
	void Lock(){ EnterCriticalSection(&m_lock); }
	void UnLock(){ LeaveCriticalSection(&m_lock); }
    void WriteTimeStamp()
	{
		*this << "[" << CDateTime().LongDateTime() << "]";
	}
	void WriteTid()
	{
		*this << "(" << /*syscall(SYS_gettid)*/GetCurrentThreadId() << ")";
	}
    void Write(const string &strLog)
	{
		ConnectFile();

		WriteTimeStamp();
		WriteTid();
		*this << "<" << strLog.c_str() << ">\r\n" << endl;
	}
    void Write(const char *pFormatString, ...)
	{
		ConnectFile();
		string strText;

		va_list argList;
		va_start(argList, pFormatString);
		FormatStringV(strText, pFormatString, argList);
		va_end(argList);

		WriteTimeStamp();
		WriteTid();
		*this << "<" << strText.c_str() << ">\r\n" << endl;
	}
	// 打开文件(如果需要打开的当前日志文件存在则返回,否则创建并打开) 
    void ConnectFile(bool bForce = false)
	{
		string strCurDate = CDateTime().LongDate();
		string strName = GetModulePath();
		strName += "log";
		if (!DirectoryExists(strName))
		{
			if (!CreateDir(strName))
			{
				::MessageBox(0, L"创建日志文件目录失败！", L"错误", 0);
				exit(1);
			}
		}
		strName += "\\";
		strName += strCurDate;
		if (!DirectoryExists(strName))
		{
			if (!CreateDir(strName))
			{
				::MessageBox(0, L"创建日志文件失败！", L"错误", 0);
				exit(1);
			}
		}
		strName += "\\";
		strName += m_strLogName;
		if (bForce)
		{
			close();
			Open(strName.c_str());
			m_bOpen = true;
		}
		else
		{
			if (!FileExists(strName) || !m_bOpen)
			{
				Open(strName.c_str());
				m_bOpen = true;
			}
		}
	}
protected:
    string m_strLogName;

private:
	// SRWLock的效率更高，但只有win7以上支持
	CRITICAL_SECTION m_lock;

private:
	static T *m_pInstance;
    // 是否打开文件
    bool m_bOpen; 
    CLogStream(){}
};

//////////////////////////////////////////////////////////////////////////////
// class CLogSocket - Socket日志类
class CLogSocket : public CLogStream<CLogSocket>
{
private:
	CLogSocket() : CLogStream<CLogSocket>(IM_LOG_FILENAME_SOCKET){}
	friend class CLogStream<CLogSocket>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogInterface - CLogInterface日志类
class CLogInterface : public CLogStream<CLogInterface>
{
private:
	CLogInterface() : CLogStream<CLogInterface>(IM_LOG_FILENAME_INTERFACE){}
	friend class CLogStream<CLogInterface>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogWork - Work日志类
class CLogWork : public CLogStream<CLogWork>
{
private:
	CLogWork() : CLogStream(IM_LOG_FILENAME_WORK){ }
	~CLogWork(){}
	friend class CLogStream<CLogWork>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogControl - Control日志类
class CLogControl : public CLogStream<CLogControl>
{
private:
	CLogControl() : CLogStream(IM_LOG_FILENAME_CONTROL){ }
	friend class CLogStream<CLogControl>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogClient - Client日志类
class CLogClient : public CLogStream<CLogClient>
{
private:
	CLogClient() : CLogStream(IM_LOG_FILENAME_CLIENT){ }
	friend class CLogStream<CLogClient>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogOnlineList - COnlineList扫描日志类
class CLogOnlineList : public CLogStream<CLogOnlineList>
{
private:
	CLogOnlineList() : CLogStream(IM_LOG_FILENAME_ONLINELIST){}
	friend class CLogStream<CLogOnlineList>;
};

//////////////////////////////////////////////////////////////////////////////////
//// class CLogJsonDecode - CLogJsonDecode扫描日志类
//class CLogJsonDecode : public CLogStream<CLogJsonDecode>
//{
//private:
//	CLogJsonDecode() : CLogStream(IM_LOG_FILENAME_JSONDECODE){}
//	friend class CLogStream<CLogJsonDecode>;
//};
//
//////////////////////////////////////////////////////////////////////////////////
//// class CLogJsonEncode - CLogJsonEncode扫描日志类
//class CLogJsonEncode : public CLogStream<CLogJsonEncode>
//{
//private:
//	CLogJsonEncode() : CLogStream(IM_LOG_FILENAME_JSONENCODE){}
//	friend class CLogStream<CLogJsonEncode>;
//};

////////////////////////////////////////////////////////////////////////////////
// class CLogTcpServer - CLogTcpServer扫描日志类
class CLogTcpServer : public CLogStream<CLogTcpServer>
{
private:
	CLogTcpServer() : CLogStream(IM_LOG_FILENAME_TCPSERVER){}
	friend class CLogStream<CLogTcpServer>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogPingServer - CLogPingServer扫描日志类
class CLogPingServer : public CLogStream<CLogPingServer>
{
private:
	CLogPingServer() : CLogStream(IM_LOG_FILENAME_PINGSERVER){}
	friend class CLogStream<CLogPingServer>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogScanServer - CLogScanServer扫描日志类
class CLogScanServer : public CLogStream<CLogScanServer>
{
private:
	CLogScanServer() : CLogStream(IM_LOG_FILENAME_SCANSERVER){}
	friend class CLogStream<CLogScanServer>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogDownloadRes - CLogDownloadRes扫描日志类
class CLogDownloadRes : public CLogStream<CLogDownloadRes>
{
private:
	CLogDownloadRes() : CLogStream(IM_LOG_FILENAME_DOWNLOADRES){}
	friend class CLogStream<CLogDownloadRes>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogDb - CLogDb扫描日志类
class CLogDb : public CLogStream<CLogDb>
{
private:
	CLogDb() : CLogStream(IM_LOG_FILENAME_DB){}
	friend class CLogStream<CLogDb>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogMsg - CLogMsg日志类
class CLogMsg : public CLogStream<CLogMsg>
{
private:
	CLogMsg() : CLogStream(IM_LOG_FILENAME_MSG){}
	friend class CLogStream<CLogMsg>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogMatrix - CLogMatrix日志类
class CLogMatrix : public CLogStream<CLogMatrix>
{
private:
	CLogMatrix() : CLogStream(IM_LOG_FILENAME_MATRIX){}
	friend class CLogStream<CLogMatrix>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogSendRecv - CLogSendRecv日志类
class CLogSendRecv : public CLogStream<CLogSendRecv>
{
private:
	CLogSendRecv() : CLogStream(IM_LOG_FILENAME_SENDRECVSERVER){}
	friend class CLogStream<CLogSendRecv>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogSendApp - CLogSendApp日志类
class CLogSendApp : public CLogStream<CLogSendApp>
{
private:
	CLogSendApp() : CLogStream(IM_LOG_FILENAME_SENDAPPSERVER){}
	friend class CLogStream<CLogSendApp>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogJsonCode - CLogJsonCode日志类 
class CLogJsonCode : public CLogStream<CLogJsonCode>
{
private:
	CLogJsonCode() : CLogStream(IM_LOG_FILENAME_JSONCODE){}
	friend class CLogStream<CLogJsonCode>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogIMAppClient - CLogIMAppClient日志类
class CLogIMAppClient : public CLogStream<CLogIMAppClient>
{
private:
	CLogIMAppClient() : CLogStream(IM_LOG_FILENAME_IMAPPCLIENT){}
	friend class CLogStream<CLogIMAppClient>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogMatchAppClient - CLogIMAppClient日志类
class CLogMatchAppClient : public CLogStream<CLogMatchAppClient>
{
private:
	CLogMatchAppClient() : CLogStream(IM_LOG_FILENAME_MATCHAPPCLIENT){}
	friend class CLogStream<CLogMatchAppClient>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogControlAppClient - CLogControlAppClient日志类
class CLogControlAppClient : public CLogStream<CLogControlAppClient>
{
private:
	CLogControlAppClient() : CLogStream(IM_LOG_FILENAME_CONTROLAPPCLIENT){}
	friend class CLogStream<CLogControlAppClient>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogChannel - CLogChannel日志类
class CLogChannel : public CLogStream<CLogChannel>
{
private:
	CLogChannel() : CLogStream(IM_LOG_FILENAME_CHANNEL){}
	friend class CLogStream<CLogChannel>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogAppClientFactory - CLogAppClientFactory日志类
class CLogAppClientFactory : public CLogStream<CLogAppClientFactory>
{
private:
	CLogAppClientFactory() : CLogStream(IM_LOG_FILENAME_APPFACTORY){}
	friend class CLogStream<CLogAppClientFactory>;
};

////////////////////////////////////////////////////////////////////////////////
// class CLogWebSock - CLogMatrix日志类
class CLogWebSock : public CLogStream<CLogWebSock>
{
private:
	CLogWebSock() : CLogStream(IM_LOG_FILENAME_WEBSOCK){}
	friend class CLogStream<CLogWebSock>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogThread - 线程日志类
class CLogThread : public CLogStream<CLogThread>
{
public:
    static void WriteLog(const char *pFormatString, ...);
    static void WriteLog(const string &strLog);
private:
	string GetThreadId();
    CLogThread();
	friend class CLogStream<CLogThread>;
};

//////////////////////////////////////////////////////////////////////////////
// class CLogError - 异常日志类
class CLogError : public CLogStream<CLogError>
{
private:
	CLogError() : CLogStream<CLogError>(IM_LOG_ERROR){}
	friend class CLogStream<CLogError>;
};

extern void InitLogs();
extern void FreeLogs();
#endif
