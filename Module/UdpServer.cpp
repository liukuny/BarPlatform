#include "UdpServer.h"

#include "ConfigFile.h"
#include "log.h"
#include "packetbuf.h"
#include "BarInfo.h"
#include "JsonCode.h"
#include "CenterInterface.h"
//#include "resource.h"

template <> CUdpServer *CSingle<CUdpServer>::m_pInstance = 0;

void testPacket()
{
	CNetAddr addr;
	addr.SetAddr("192.168.102.177", 5050);
	string strInfo = "adsfasdfasdfasdf";
	int nLen = strInfo.size() + sizeof(CPacketHeader);
	char *p = new char[nLen];
	memset(p, 0, nLen);
	memcpy(p + sizeof(CPacketHeader), strInfo.c_str(), strInfo.size());
	((CPacketHeader *)p)->InitHeader(100, 1, nLen);
	CUdpServer::GetInstance()->SendPacket(addr, p, nLen);
}

CUdpServer::CUdpServer()
{
	//InitializeCriticalSection(&m_lkSendPacket);
	//m_nPort = CConfigFile::GetInstance()->GetLocalUDPListenPort();
	//int nRet = CCommInterface::StartUdpServer(m_nPort, ProcUdpConnect, this);
	//CLogSocket::WriteLog("CUdpServer::SendPacket bgn action: 4-%d", nRet);
	//if (nRet <= 0)
	//{
	//	// 
	//}
}

CUdpServer::~CUdpServer()
{
	Stop(); 
	//CCommInterface::StopUdpServer();
	//DeleteCriticalSection(&m_lkSendPacket);
}

// 发送报文
int CUdpServer::SendPacket(const char *pRemoteIP, int nRemotePort, void *p, int nLen)
{
	int nRet = -1;
	//CLogSocket::WriteLog("CUdpServer::SendPacket bgn action: %d--%s-%d", *((word *)p), pRemoteIP, nRemotePort);
	//nRet = CCommInterface::SendUdpDataByServer((char *)pRemoteIP, nRemotePort, p, nLen);
	return nRet;
}

int CUdpServer::LoginServer(const CNetAddr& stToAddr, void *p, int nLen)
{
	int nRet = -1;
	//CLogSocket::WriteLog("CUdpServer::LoginServer bgn action: %d--%s", *((word *)p), stToAddr.GetIpPortString());
	//nRet = CCommInterface::SendUdpDataByServer((char *)stToAddr.GetIpString().c_str(), stToAddr.GetPort(), p, nLen);
	return nRet;
}

void CUdpServer::ProcUdpConnect(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort)
{
	CLogSocket::WriteLog("CUdpServer::ProcUdpConnect recv packet bgn: %d, from : %s-%d", *((word *)pRecvBuf), pRemoteIp, nRemotePort);
	//CUdpServer::GetInstance()->RecvPacket(pCallbackParam, pRecvBuf, nBufLen, pRemoteIp, nRemotePort);
	//CLogSocket::WriteLog("CUdpServer::ProcUdpConnect recv packet end");
}
void CUdpServer::RecvPacket(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort)
{
	//// 过滤
	//CNetAddr addr(pRemoteIp, nRemotePort);
	//CPacketHeader *p = (CPacketHeader *)pRecvBuf;
	////if (p->nActionCode == 8)
	////{
	////	// 测试 
	////	return;
	////}
	//if (p->CheckHeaderIsValid())
	//{
	//	if (p->nActionCode == SC_LOGINSERVER)
	//	{
	//		ProcReSCLoginServer(addr, p);
	//	}
	//	if (p->nActionCode == CC_GAMEINFOEX || p->nActionCode == CCN_LOGINSERVER || p->nActionCode == CN_LOGINSERVER || p->nActionCode == CN_LOGINSERVER || p->nActionCode == NC_LOGINSERVER)
	//	{
	//		DefaultProcPacket(addr, pRecvBuf, nBufLen);
	//	}
	//	CSafeLock<CUdpServer> lk(this); // 可能产生互锁
	//	if (m_pHeader)
	//	{
	//		if (!m_pHeader->RecvPacket(addr, pRecvBuf, nBufLen))
	//		{
	//			DefaultProcPacket(addr, pRecvBuf, nBufLen);
	//		}
	//	}
	//}
}

// 扫描处理 
bool CUdpServer::Proc(bool &bContinue)
{
	Sleep(100);
	string strType = CConfigFile::GetInstance()->GetGameMenuType();
	if (strType.empty())
	{
		string strUrl = CBarInfo::GetInstance()->GetTalkUrl();
		CCenterInterface::UploadInfo(strUrl, "3");
		// 无法取到游戏菜单类型
		CLogError::WriteLog("无法取到游戏菜单类型");
		bContinue = false;
		return true;
	}

	// 取升级信息
	static bool bDownloadUpdate = false;
	if (!bDownloadUpdate)
	{
		string strUpdate = CCenterInterface::GetUpdate();
		bDownloadUpdate = true;
		CLogClient::WriteLog("升级标志：%d, 升级返回信息： %s", bDownloadUpdate, strUpdate.c_str());
		if (!strUpdate.empty())
		{
			UpLevelDataItem Item;
			CJsonDecode::DeCodeSelfUpdateData(strUpdate, Item);
			if (!Item.url.empty())
			{
				CLogClient::WriteLog("有新版本平台。");
				Item.pathname = CConfigFile::GetInstance()->GetPlatformUpdateFilePath();
				Item.pathname += "\\";
				Item.pathname += Item.url.substr(Item.url.rfind('/') + 1);
				bool b = CCenterInterface::DownloadFile(Item.url, Item.pathname, Item.md5);
				if (b)
				{
					if (CCommInterface::UnzipFileToPath(Item.pathname.c_str(), CConfigFile::GetInstance()->GetPlatformUpdateFilePath().c_str()))
					{
						// 释放升级助手并运行
						FreeAndRunUpdate();
						// 退出程序
						exit(1);
					}
				}
			}
		}
	}

	static int64 nTime = 0;
	if (::GetTickCount64() - nTime > 6 * 1000)
	{
		//// 获取游戏库
		//CBarInfo::GetInstance()->GetGameLib();
		// 获取配置
		CBarInfo::GetInstance()->GetConfig();

		// 扫描其他游戏菜单上传
		CBarInfo::GetInstance()->ScanGameMenuAndUpload();

		nTime = ::GetTickCount64();
	}

	return true;
}

// 注册应用服务
void CUdpServer::RegeditServer(CNetServer *pServer)
{
	//CSafeLock<CUdpServer> lk(this);
	//if (!m_pHeader)
	//{
	//	m_pHeader = pServer;
	//}
	//else
	//{
	//	m_pHeader->AppendServer(pServer);
	//}
}

void CUdpServer::UnRegeditServer(CNetServer *pServer)
{
	//CSafeLock<CUdpServer> lk(this);
	//if (m_pHeader)
	//{
	//	if (m_pHeader == pServer)
	//	{
	//		m_pHeader = pServer->GetNext();
	//	}else
	//	{
	//		m_pHeader->RemoveServer(pServer);
	//	}
	//}
}

// 处理登录服务应答
void CUdpServer::ProcReSCLoginServer(const CNetAddr &addrFrom, CPacketHeader *p)
{
	//CLogSocket::WriteLog("CUdpServer::ProcReSCLoginServer bgn");
	//if (!CUserOnlineInfo::GetInstance()->IsLoginServer() && !CUserOnlineInfo::GetInstance()->GetIsControl())
	//{
	//	COperatePacket *p1 = COperatePacket::GetPacketFromBuf(p, p->nPacketLen);
	//	CUserOnlineInfo::GetInstance()->ProcReSCLoginServer(addrFrom, p1, true);
	//	delete p1;
	//	CUserOnlineInfo::GetInstance()->GetBarConfig();
	//}

}
//
//// 处理没有应用处理的业务
//void CUdpServer::DefaultProcPacket(const CNetAddr &addFrom, void *pPacket, int nPacketLen)
//{
//	//COperatePacket *p = COperatePacket::GetPacketFromBuf(pPacket, nPacketLen);
//	//if (CConfigFile::GetInstance()->GetLocalMachineName().compare(p->GetMachineName()) != 0)
//	//{
//	//	if (p->GetHeader()->nActionCode == CC_GAMEINFOEX)
//	//	{
//	//		ProcDefaultCallGameInfoEx(addFrom, p);
//	//	}
//	//	else if (p->GetHeader()->nActionCode == CCN_LOGINSERVER)
//	//	{
//	//		ProcControlLSByNet(addFrom, p);
//	//	}
//	//	else if (p->GetHeader()->nActionCode == CN_LOGINSERVER)
//	//	{
//	//		ProcLoginLSByNet(addFrom, p);
//	//	}
//	//	else if (p->GetHeader()->nActionCode == CN_LOGINSERVER)
//	//	{
//	//		ProcLoginServerNotify(addFrom, p);
//	//	}
//	//	else if (p->GetHeader()->nActionCode == NC_LOGINSERVER)
//	//	{
//	//		ProcReLoginServerNotify(addFrom, p);
//	//	}
//	//}
//	//delete p;
//}
//
//// 处理用户游戏信息
//void CUdpServer::ProcDefaultCallGameInfoEx(const CNetAddr &addrFrom, COperatePacket *p)
//{
//}
//
//// 处理吧台登录
//void CUdpServer::ProcControlLSByNet(const CNetAddr &addrFrom, COperatePacket *p)
//{
//	CLogSocket::WriteLog("CUdpServer::ProcControlLSByNet bgn");
//	//CBarInfo::GetInstance()->SetAddrControl(addrFrom);
//	//string strInfo = CGameInfo::GetInstance()->GetGameUserInfoA();
//	//if (!strInfo.empty())
//	//{
//	//	CLogSocket::WriteLog("获取游戏信息发送到吧台，目标地址： %s", addrFrom.GetIpPortString().c_str());
//	//	string strGameType = "0";
//	//	string strGameState = CGameInfo::GetInstance()->GetUserGameState();
//	//	CCGameInfoExPacket packet;
//	//	CPacketHeader *pHeader = packet.CreateSendBuf(CConfigFile::GetInstance()->GetLocalMachineName(), CUserOnlineInfo::GetInstance()->GetUserId(), strGameType, strGameState, strInfo);
//	//	CUdpServer::GetInstance()->SendPacket(addrFrom, pHeader, pHeader->nPacketLen);
//	//}
//	CLogSocket::WriteLog("CUdpServer::ProcControlLSByNet end");
//}
//
//// 处理登录服务应答
//void CUdpServer::ProcLoginLSByNet(const CNetAddr &addrFrom, COperatePacket *p)
//{
//	CLogSocket::WriteLog("CUdpServer::ProcLoginLSByNet bgn");
//	//CBarInfo::GetInstance()->SetAddrControl(addrFrom);
//	//string strInfo = CGameInfo::GetInstance()->GetGameUserInfoA();
//	//if (!strInfo.empty())
//	//{
//	//	CLogSocket::WriteLog("获取游戏信息发送到对方，目标地址： %s", addrFrom.GetIpPortString().c_str());
//	//	string strGameType = "0";
//	//	string strGameState = CGameInfo::GetInstance()->GetUserGameState();
//	//	CCGameInfoExPacket packet;
//	//	CPacketHeader *pHeader = packet.CreateSendBuf(CConfigFile::GetInstance()->GetLocalMachineName(), CUserOnlineInfo::GetInstance()->GetUserId(), strGameType, strGameState, strInfo);
//	//	CUdpServer::GetInstance()->SendPacket(addrFrom, pHeader, pHeader->nPacketLen);
//	//}
//	CLogSocket::WriteLog("CUdpServer::ProcLoginLSByNet end");
//}
//
//// 处理登录服务通知
//void CUdpServer::ProcLoginServerNotify(const CNetAddr &addrFrom, COperatePacket *p)
//{
//	CLogSocket::WriteLog("处理登录服务通知 beg");
//	//CLoginServerNotifyPacket *pPacket = (CLoginServerNotifyPacket *)p;
//	//CUserInfo info;
//	//info.strMachineName = p->GetMachineName();// CMatrix::GetInstance()->GetMachineName(addrFrom);
//	//info.strUserId = pPacket->GetUserId();
//	//info.nMoney1 = pPacket->GetMoney1();
//	//info.nMoney2 = pPacket->GetMoney2();
//	//info.nVirtualItem1 = pPacket->GetVirtualItem1();
//	//info.nVirtualItem2 = pPacket->GetVirtualItem2();
//	//info.strUserSex = pPacket->GetUserSex();
//	//CUserInfoList::GetInstance()->InsertUserInfo(addrFrom, info);
//	//if (CUserOnlineInfo::GetInstance()->IsLoginServer())
//	//{
//	//	CReLoginServerPacket RePacket;
//	//	CUserOnlineInfo::GetInstance()->GetUserInfo(info);
//	//	CPacketHeader *pHeader = RePacket.CreateSendBuf(info.strMachineName, info);
//	//	SendPacket(addrFrom, pHeader, pHeader->nPacketLen);
//	//}
//	CLogSocket::WriteLog("处理登录服务通知 end");
//}
//
//// 处理登录服务通知应答
//void CUdpServer::ProcReLoginServerNotify(const CNetAddr &addrFrom, COperatePacket *p)
//{
//	CLogSocket::WriteLog("处理登录服务通知应答 bgn");
//	//CReLoginServerPacket *pPacket = (CReLoginServerPacket *)p;
//	//CUserInfo info;
//	//info.strMachineName = p->GetMachineName();// CMatrix::GetInstance()->GetMachineName(addrFrom);
//	//info.strUserId = pPacket->GetUserId();
//	//info.nMoney1 = pPacket->GetMoney1();
//	//info.nMoney2 = pPacket->GetMoney2();
//	//info.nVirtualItem1 = pPacket->GetVirtualItem1();
//	//info.nVirtualItem2 = pPacket->GetVirtualItem2();
//	//info.strUserSex = pPacket->GetUserSex();
//	//CUserInfoList::GetInstance()->InsertUserInfo(addrFrom, info);
//	CLogSocket::WriteLog("处理登录服务通知应答 end");
//}

// 释放升级助手并运行
bool CUdpServer::FreeAndRunUpdate()
{
	string csExtraPath = GetModulePath();

	//HRSRC hRsrc = FindResourceA(GetModuleHandle(NULL), MAKEINTRESOURCEA(IDR_BIN_UPDATE), "bin");
	HRSRC hRsrc = FindResourceA(GetModuleHandle(NULL), MAKEINTRESOURCEA(129), "bin");
	if (hRsrc == NULL)
	{
		CLogError::WriteLog("释放升级助手,查找资源失败：%d", ::GetLastError());
		return false;
	}

	DWORD dwSize = SizeofResource(GetModuleHandle(NULL), hRsrc);
	if (dwSize == 0)
	{
		CLogError::WriteLog("释放升级助手,查找资源长度失败：%d", ::GetLastError());
		return false;
	}

	HGLOBAL hGlobal = LoadResource(GetModuleHandle(NULL), hRsrc);
	if (hGlobal == NULL)
	{
		CLogError::WriteLog("释放升级助手,加载资源失败：%d", ::GetLastError());
		return false;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (pBuffer == NULL)
	{
		FreeResource(hGlobal);
		CLogError::WriteLog("释放升级助手,锁定资源失败：%d", ::GetLastError());
		return false;
	}


	string sFilePath = csExtraPath;
	sFilePath += "update.exe";
	ofstream of;
	of.open(sFilePath.c_str(), ios::out | ios::ate | ios::binary);
	if (of.is_open())
	{
		of.write((const char *)pBuffer, dwSize);
		of.close();
	}
	//CFile file;
	//if (file.Open((LPCSTR)sFilePath, CFile::modeCreate | CFile::modeWrite))
	//{
	//	file.Write(pBuffer, dwSize);
	//	file.Close();
	//}
	else
	{
		FreeResource(hGlobal);
		CLogError::WriteLog("释放升级助手,写入文件失败：%d", ::GetLastError());
		return false;
	}
	FreeResource(hGlobal);
	HINSTANCE h = ShellExecuteA(NULL, "open", sFilePath.c_str(), NULL, NULL, SW_HIDE);
	if ((int)h < 32)
	{
		CLogError::WriteLog("释放升级助手,运行文件失败：%d", ::GetLastError());
		return false;
	}
	return true;
}

