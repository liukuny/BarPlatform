#pragma once

#include <tchar.h>
#include <map>

#include "..\..\..\Communication\Communication\Communication.h"

// 底层通讯接口模块
//#define COMMMODULE_INTERFACEDLL_NAME _T("D:\\personalproject\\bar\\source\\Communication\\x64\\Debug\\Communication.dll")
#define COMMMODULE_INTERFACEDLL_NAME _T("Communication.dll")
//#define COMMMODULE_INTERFACEDLL_NAME _T("D:\\personalproject\\bar\\source\\Communication\\Release\\Communication.dll")

// 启动UDP服务
typedef int(WINAPI *PStartUdpServer)(int nPort, PSTARTUDPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// 停止UDP服务
typedef void (WINAPI *PStopUdpServer)();
// 发送UDP数据
typedef int (WINAPI *PSendUdpData)(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);
// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PSendUdpDataByServer)(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);

// 启动TCP服务
typedef int (WINAPI *PStartTcpServer)(int nPort, PSTARTTCPSERVERCALLBACKFUN pFun, void *pCallbackParam);
// 停止TCP服务
typedef void (WINAPI *PStopTcpServer)();
// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PSendTcpDataByServerConnection)(void *pConnect, void *pBuf, int nBufLen);
// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PRecvTcpDataByServerConnection)(void *pConnect, void *pBuf, int nBufLen);
// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PSendTcpDataByClient)(void *pCallbackParam, char *pRemoteIp, int nRemotePort, PSTARTTCPCLIENTCALLBACKFUN pFun, void *pBuf, int nBufLen);
// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PSendTcpDataByClientConnection)(void *pSreamSocket, void *pBuf, int nBufLen);
// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PRecvTcpDataByClientConnection)(void *pSreamSocket, void *pBuf, int nBufLen);


// 上传Http/Https数据
typedef int (WINAPI *PPostHttpData)(const char *sUrl, const char *strSource, int nSourceLen, const char *strContentType, char *strRet, int nSize, int &nState);
// 请求Http数据
typedef int (WINAPI *PGetHttpData)(const char *sUrl, char *strRet, int nSize, int &nState);
// 下载Http或Https的文件数据
typedef int (WINAPI *PDownloadDataFromServer)(const char *sUrl, char *strFileName, int &nState);

// WebSock客户端
typedef int (WINAPI *PCreateWebSockClient)(const char *sUrl, const void *pCallbackParam, PWEBSOCKCLIENTCALLBACKFUN pFun, PWEBSOCKCLIENTONRECV pOnRecv, PWEBSOCKCLIENTCALLBACKFUN pOnClose, PWEBSOCKCLIENTCALLBACKFUN pOnFail);
// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
typedef int (WINAPI *PSendDataByWebSockClient)(const void *pWebSock, const void *pBuf, int nBufLen);
// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
typedef void (WINAPI *PCloseWebSockClient)();

// 将缓存解压到文件
typedef int (WINAPI *PUnzipFile)(const char *szBuf, int nBufLen, const char *szFileName, char *szDestFileName, int nDestFileLen);
// 将文件解压到文件夹
typedef int (WINAPI *PUnzipFileToPath)(const char *szZipName, const char *szPathName);

class CCommInterface
{
public:
	CCommInterface();
	~CCommInterface();
	// 加载第三方接口模块
	static int LoadThreePlug();
	// 安装底层通讯接口模块
	static void InstallCommModule();
	// 释放底层通讯接口模块
	static void FreeCommModule();

	// 启动UDP服务
	static int StartUdpServer(int nPort, PSTARTUDPSERVERCALLBACKFUN pFun, void *pCallbackParam);
	// 停止UDP服务
	static void StopUdpServer();
	// 发送UDP数据
	static int SendUdpData(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);
	// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
	static int SendUdpDataByServer(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen);

	// 启动TCP服务
	static int WINAPI StartTcpServer(int nPort, PSTARTTCPSERVERCALLBACKFUN pFun, void *pCallbackParam);
	// 停止TCP服务
	static void WINAPI StopTcpServer();
	// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static int WINAPI SendTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);
	// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	static int WINAPI RecvTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen);
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static int SendTcpDataByClient(void *pCallbackParam, char *pRemoteIp, int nRemotePort, PSTARTTCPCLIENTCALLBACKFUN pFun, void *pBuf, int nBufLen);
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static int SendTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);
	// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	static int RecvTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen);

	// 上传Http数据
	static int WINAPI PostHttpData(const char *sUrl, const char *strSource, int nSourceLen, char *strRet, int nSize, int &nState, const char *strContentType = 0);
	// 请求Http数据
	static int WINAPI GetHttpData(const char *sUrl, char *strRet, int nSize, int &nState);
	// 下载Http或Https的文件数据
	static int WINAPI DownloadDataFromServer(const char *sUrl, char *strFileName, int &nState);

	// WebSock客户端
	static int WINAPI CreateWebSockClient(const char *sUrl, const void *pCallbackParam, PWEBSOCKCLIENTCALLBACKFUN pFun, PWEBSOCKCLIENTONRECV pOnRecv, PWEBSOCKCLIENTCALLBACKFUN pOnClose, PWEBSOCKCLIENTCALLBACKFUN pOnFail);
	// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	static int WINAPI SendDataByWebSockClient(const void *pWebSock, const void *pBuf, int nBufLen);
	// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	static void WINAPI CloseWebSockClient();

	// 将缓存解压到文件
	static int WINAPI UnzipFile(const char *szBuf, int nBufLen, const char *szFileName, char *szDestFileName, int nDestFileLen);
	// 将文件解压到文件夹
	static int WINAPI UnzipFileToPath(const char *szZipName, const char *szPathName);

public:
	// 底层通讯接口模块
	static HMODULE m_hdll;
	static HMODULE m_hThreedll;
	// 启动UDP服务
	static PStartUdpServer m_pStartUdpServer;
	// 停止UDP服务
	static PStopUdpServer m_pStopUdpServer;
	// 发送UDP数据
	static PSendUdpData m_pSendUdpData;
	// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
	static PSendUdpDataByServer m_pSendUdpDataByServer;

	// 启动TCP服务
	static PStartTcpServer m_pStartTcpServer;
	// 停止TCP服务
	static PStopTcpServer m_pStopTcpServer;
	// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static PSendTcpDataByServerConnection m_pSendTcpDataByServerConnection;
	// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	static PRecvTcpDataByServerConnection m_pRecvTcpDataByServerConnection;
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static PSendTcpDataByClient m_pSendTcpDataByClient;
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	static PSendTcpDataByClientConnection m_pSendTcpDataByClientConnection;
	// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	static PRecvTcpDataByClientConnection m_pRecvTcpDataByClientConnection;

	// 上传Http/Https数据
	static PPostHttpData m_pPostHttpData;
	// 请求Http协议数据
	static PGetHttpData m_pGetHttpData;
	// 下载Http或Https的文件数据
	static PDownloadDataFromServer m_pDownloadDataFromServer;

	// WebSock客户端
	static PCreateWebSockClient m_pCreateWebSockClient;
	// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	static PSendDataByWebSockClient m_pSendDataByWebSockClient;
	// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	static PCloseWebSockClient m_pCloseWebSockClient;

	// 将缓存解压到文件
	static PUnzipFile m_pUnzipFile;
	// 将文件解压到文件夹
	static PUnzipFileToPath m_pUnzipFileToPath;

};

