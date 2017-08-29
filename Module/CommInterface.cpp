#include <windows.h>

#include "CommInterface.h"
#include "ConfigFile.h"

HMODULE CCommInterface::m_hThreedll = 0;
HMODULE CCommInterface::m_hdll = 0;
// 启动UDP服务
PStartUdpServer CCommInterface::m_pStartUdpServer = 0;
// 停止UDP服务
PStopUdpServer CCommInterface::m_pStopUdpServer = 0;
// 发送UDP数据
PSendUdpData CCommInterface::m_pSendUdpData = 0;
// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
PSendUdpDataByServer CCommInterface::m_pSendUdpDataByServer = 0;

// 启动TCP服务
PStartTcpServer CCommInterface::m_pStartTcpServer = 0;
// 停止TCP服务
PStopTcpServer CCommInterface::m_pStopTcpServer = 0;
// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
PSendTcpDataByServerConnection CCommInterface::m_pSendTcpDataByServerConnection = 0;
// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
PRecvTcpDataByServerConnection CCommInterface::m_pRecvTcpDataByServerConnection = 0;
// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
PSendTcpDataByClient CCommInterface::m_pSendTcpDataByClient = 0;
// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
PSendTcpDataByClientConnection CCommInterface::m_pSendTcpDataByClientConnection = 0;
// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
PRecvTcpDataByClientConnection CCommInterface::m_pRecvTcpDataByClientConnection = 0;

// 上传Http/Https数据
PPostHttpData CCommInterface::m_pPostHttpData = 0;
// 请求Http协议数据
PGetHttpData CCommInterface::m_pGetHttpData = 0;
// 下载Http或Https的文件数据
PDownloadDataFromServer CCommInterface::m_pDownloadDataFromServer = 0;

// WebSock客户端
PCreateWebSockClient CCommInterface::m_pCreateWebSockClient = 0;
// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
PSendDataByWebSockClient CCommInterface::m_pSendDataByWebSockClient = 0;
// 关闭WebSock
PCloseWebSockClient CCommInterface::m_pCloseWebSockClient = 0;

// 将缓存解压到文件
PUnzipFile CCommInterface::m_pUnzipFile = 0;
// 将文件解压到文件夹
PUnzipFileToPath CCommInterface::m_pUnzipFileToPath = 0;

CCommInterface::CCommInterface()
{
}


CCommInterface::~CCommInterface()
{
}

// 加载第三方接口模块
int CCommInterface::LoadThreePlug()
{
	int nErr = 0;
	if (m_hThreedll)
	{
		return -1;
	}
	std::string strDll = CConfigFile::GetInstance()->GetMinProgramFilePath();// "\\ThreePlug.dll";
	strDll += "\\ThreePlug.dll";
	//strDll = CConfigFile::GetInstance()->CreateFullPathName(strDll);
	m_hThreedll = LoadLibraryA(strDll.c_str());
	if (!m_hThreedll)
	{
		nErr = ::GetLastError();
	}
	return nErr;
}


// 安装底层通讯接口模块
void CCommInterface::InstallCommModule()
{
	//FreeAdmodule();
	if (m_hdll)
	{
		return;
	}
	std::string strDll = "\\Communication.dll";
	strDll = CConfigFile::GetInstance()->CreateFullPathName(strDll);
	//m_hdll = LoadLibrary(COMMMODULE_INTERFACEDLL_NAME);
	m_hdll = LoadLibraryA(strDll.c_str());
	if (!m_hdll)
	{
		int nErr = ::GetLastError();
		return;
	}
	// 启动UDP服务
	m_pStartUdpServer = (PStartUdpServer)GetProcAddress(m_hdll, "StartUdpServer");
	// 停止UDP服务
	m_pStopUdpServer = (PStopUdpServer)GetProcAddress(m_hdll, "StopUdpServer");
	// 发送UDP数据
	m_pSendUdpData = (PSendUdpData)GetProcAddress(m_hdll, "SendUdpData");
	// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendUdpDataByServer = (PSendUdpDataByServer)GetProcAddress(m_hdll, "SendUdpDataByServer");

	// 启动TCP服务
	m_pStartTcpServer = (PStartTcpServer)GetProcAddress(m_hdll, "StartTcpServer");
	// 停止TCP服务
	m_pStopTcpServer = (PStopTcpServer)GetProcAddress(m_hdll, "StopTcpServer");
	// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendTcpDataByServerConnection = (PSendTcpDataByServerConnection)GetProcAddress(m_hdll, "SendTcpDataByServerConnection");
	// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pRecvTcpDataByServerConnection = (PRecvTcpDataByServerConnection)GetProcAddress(m_hdll, "RecvTcpDataByServerConnection");
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendTcpDataByClient = (PSendTcpDataByClient)GetProcAddress(m_hdll, "SendTcpDataByClient");
	// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendTcpDataByClientConnection = (PSendTcpDataByClientConnection)GetProcAddress(m_hdll, "SendTcpDataByClientConnection");
	// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pRecvTcpDataByClientConnection = (PRecvTcpDataByClientConnection)GetProcAddress(m_hdll, "RecvTcpDataByClientConnection");

	// 上传Http/Https数据 
	//m_pPostHttpData = (PPostHttpData)GetProcAddress(m_hdll, "PostHttpData");
	m_pPostHttpData = (PPostHttpData)GetProcAddress(m_hdll, "PostDataToServer");
	// 请求Http协议数据
	m_pGetHttpData = (PGetHttpData)GetProcAddress(m_hdll, "GetHttpDataFromUtf8");
	// 下载Http或Https的文件数据
	m_pDownloadDataFromServer = (PDownloadDataFromServer)GetProcAddress(m_hdll, "DownloadDataFromServer");

	// WebSock客户端
	m_pCreateWebSockClient = (PCreateWebSockClient)GetProcAddress(m_hdll, "CreateWebSockClient");
	// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendDataByWebSockClient = (PSendDataByWebSockClient)GetProcAddress(m_hdll, "SendDataByWebSockClient");
	// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	m_pCloseWebSockClient = (PCloseWebSockClient)GetProcAddress(m_hdll, "CloseWebSockClient");

	// 将缓存解压到文件
	m_pUnzipFile = (PUnzipFile)GetProcAddress(m_hdll, "UnzipFile");
	// 将文件解压到文件夹
	m_pUnzipFileToPath = (PUnzipFileToPath)GetProcAddress(m_hdll, "UnzipFileToPath");

}

// 释放底层通讯接口模块
void CCommInterface::FreeCommModule()
{
	if (m_hThreedll) FreeLibrary(m_hThreedll);
	m_hThreedll = 0;
	if (m_hdll) FreeLibrary(m_hdll);
	m_hdll = 0;
	// 启动UDP服务
	m_pStartUdpServer = 0;
	// 停止UDP服务
	m_pStopUdpServer = 0;
	// 发送UDP数据
	m_pSendUdpData = 0;
	// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendUdpDataByServer = 0;

	// 启动TCP服务
	m_pStartTcpServer = 0;
	// 停止TCP服务
	m_pStopTcpServer = 0;
	// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendTcpDataByServerConnection = 0;
	// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
	m_pRecvTcpDataByServerConnection = 0;

	// 上传Http/Https数据
	m_pPostHttpData = 0;
	// 请求Http协议数据
	m_pGetHttpData = 0;

	// WebSock客户端
	m_pCreateWebSockClient = 0;
	// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	m_pSendDataByWebSockClient = 0;
	// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
	m_pCloseWebSockClient = 0;
}


// 启动UDP服务
int CCommInterface::StartUdpServer(int nPort, PSTARTUDPSERVERCALLBACKFUN pFun, void *pCallbackParam)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pStartUdpServer)
	{
		return m_pStartUdpServer(nPort, pFun, pCallbackParam);
	}
	return -1;
}

// 停止UDP服务
void CCommInterface::StopUdpServer()
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pStopUdpServer)
	{
		m_pStopUdpServer();
	}
}

// 发送UDP数据
int CCommInterface::SendUdpData(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendUdpData)
	{
		return m_pSendUdpData(sRemoteIp, nRemotePort, pBuf, nBufLen);
	}
	return -1;
}

// 发送UDP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::SendUdpDataByServer(char *sRemoteIp, int nRemotePort, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendUdpDataByServer)
	{
		return m_pSendUdpDataByServer(sRemoteIp, nRemotePort, pBuf, nBufLen);
	}
	return -1;
}

// 启动TCP服务
int CCommInterface::StartTcpServer(int nPort, PSTARTTCPSERVERCALLBACKFUN pFun, void *pCallbackParam)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pStartTcpServer)
	{
		return m_pStartTcpServer(nPort, pFun, pCallbackParam);
	}
	return -1;
}

// 停止TCP服务
void CCommInterface::StopTcpServer()
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pStopTcpServer)
	{
		return m_pStopTcpServer();
	}
}

// 通过服务端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::SendTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendTcpDataByServerConnection)
	{
		return m_pSendTcpDataByServerConnection(pConnect, pBuf, nBufLen);
	}
	return -1;
}

// 通过服务端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::RecvTcpDataByServerConnection(void *pConnect, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pRecvTcpDataByServerConnection)
	{
		return m_pRecvTcpDataByServerConnection(pConnect, pBuf, nBufLen);
	}
	return -1;
}

// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::SendTcpDataByClient(void *pCallbackParam, char *pRemoteIp, int nRemotePort, PSTARTTCPCLIENTCALLBACKFUN pFun, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendTcpDataByClient)
	{
		return m_pSendTcpDataByClient(pCallbackParam, pRemoteIp, nRemotePort, pFun, pBuf, nBufLen);
	}
	return -1;
}

// 通过客户端发送TCP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::SendTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendTcpDataByClientConnection)
	{
		return m_pSendTcpDataByClientConnection(pSreamSocket, pBuf, nBufLen);
	}
	return -1;
}

// 通过客户端接收TCP数据(非线程安全，注意不要再不同的线程里调用)
int CCommInterface::RecvTcpDataByClientConnection(void *pSreamSocket, void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pRecvTcpDataByClientConnection)
	{
		return m_pRecvTcpDataByClientConnection(pSreamSocket, pBuf, nBufLen);
	}
	return -1;
}

// 上传Http数据
int CCommInterface::PostHttpData(const char *sUrl, const char *strSource, int nSourceLen, char *strRet, int nSize, int &nState, const char *strContentType)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pPostHttpData)
	{
		std::string strC;
		if (strContentType)
		{
			strC = strContentType;
		}
		if (strC.empty())
		{
			strC = "text/html,application/xhtml+xml,*/*";
		}
		return m_pPostHttpData(sUrl, strSource, nSourceLen, strC.c_str(), strRet, nSize, nState);
	}
	return -1;
}

// 请求Http数据
int WINAPI CCommInterface::GetHttpData(const char *sUrl, char *strRet, int nSize, int &nState)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pGetHttpData)
	{
		return m_pGetHttpData(sUrl, strRet, nSize, nState);
	}
	return -1;
}

// 下载Http或Https的文件数据
int WINAPI CCommInterface::DownloadDataFromServer(const char *sUrl, char *strFileName, int &nState)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pDownloadDataFromServer)
	{
		return m_pDownloadDataFromServer(sUrl, strFileName, nState);
	}
	return -1;
}

// WebSock客户端
// WebSock客户端
int WINAPI CCommInterface::CreateWebSockClient(const char *sUrl, const void *pCallbackParam, PWEBSOCKCLIENTCALLBACKFUN pFun, PWEBSOCKCLIENTONRECV pOnRecv, PWEBSOCKCLIENTCALLBACKFUN pOnClose, PWEBSOCKCLIENTCALLBACKFUN pOnFail)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pCreateWebSockClient)
	{
		return m_pCreateWebSockClient(sUrl, pCallbackParam, pFun, pOnRecv, pOnClose, pOnFail);
	}
	return -1;
}
// 发送WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
int WINAPI CCommInterface::SendDataByWebSockClient(const void *pWebSock, const void *pBuf, int nBufLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pSendDataByWebSockClient)
	{
		return m_pSendDataByWebSockClient(pWebSock, pBuf, nBufLen);
	}
	return -1;
}

// 接收WebSock客户端数据(非线程安全，注意不要再不同的线程里调用)
void WINAPI CCommInterface::CloseWebSockClient()
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pCloseWebSockClient)
	{
		m_pCloseWebSockClient();
	}
}

// 将缓存解压到文件
int WINAPI CCommInterface::UnzipFile(const char *szBuf, int nBufLen, const char *szFileName, char *szDestFileName, int nDestFileLen)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pUnzipFile)
	{
		return m_pUnzipFile(szBuf, nBufLen, szFileName, szDestFileName, nDestFileLen);
	}
	return -1;
}
// 将文件解压到文件夹
int WINAPI CCommInterface::UnzipFileToPath(const char *szZipName, const char *szPathName)
{
	if (!m_hdll)
	{
		InstallCommModule();
	}
	if (m_pUnzipFileToPath)
	{
		return m_pUnzipFileToPath(szZipName, szPathName);
	}
	return -1;
}
