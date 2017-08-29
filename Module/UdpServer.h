#pragma once
#include "CommInterface.h"
#include "gclass.h"
#include "netbase.h"
#include "csprotocol.h"

class CUdpServer : public CSingleServer<CUdpServer>
{
public:
	// 扫描处理 
	bool Proc(bool &bContinue);
	// 注册应用服务
	void RegeditServer(CNetServer *pServer);
	void UnRegeditServer(CNetServer *pServer);
	// 发送报文
	int SendPacket(const char *pRemoteIP, int nRemotePort, void *p, int nLen);
	int SendPacket(const char *pRemoteIPAndPort, void *p, int nLen){ CNetAddr stToAddr(pRemoteIPAndPort); return SendPacket(stToAddr.GetIpString().c_str(), stToAddr.GetPort(), p, nLen); }
	int SendPacket(const CNetAddr& stToAddr, void *p, int nLen){ return SendPacket(stToAddr.GetIpString().c_str(), stToAddr.GetPort(), p, nLen); }
	int LoginServer(const CNetAddr& stToAddr, void *p, int nLen);
protected:
	CUdpServer();// {}
	~CUdpServer();// { Stop(); }
private:
	// 网络应用服务
	CNetServer *m_pHeader = 0;
	int m_nPort;
	static void __stdcall ProcUdpConnect(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort);
	void RecvPacket(void *pCallbackParam, char *pRecvBuf, int nBufLen, const char *pRemoteIp, int nRemotePort);
	//// 处理报文
	//bool ProcPacket(const CNetAddr &addrFrom, void *pPacket, int nPacketLen);
	// 处理登录服务应答
	void ProcReSCLoginServer(const CNetAddr &addrFrom, CPacketHeader *p);
private:
	// SRWLock的效率更高，但只有win7以上支持
	CRITICAL_SECTION m_lkSendPacket;
	void LockSend(){ EnterCriticalSection(&m_lkSendPacket); }
	void UnLockSend(){ LeaveCriticalSection(&m_lkSendPacket); }
	//// 处理没有应用处理的业务
	//void DefaultProcPacket(const CNetAddr &addFrom, void *pPacket, int nPacketLen);
	//// 处理用户游戏信息
	//void ProcDefaultCallGameInfoEx(const CNetAddr &addrFrom, COperatePacket *p);
	////处理吧台登录
	//void ProcControlLSByNet(const CNetAddr &addrFrom, COperatePacket *p);
	//// 处理客服端登录
	//void ProcLoginLSByNet(const CNetAddr &addrFrom, COperatePacket *p);
	//// 处理登录服务通知
	//void ProcLoginServerNotify(const CNetAddr &addrFrom, COperatePacket *p);
	//// 处理登录服务通知应答
	//void ProcReLoginServerNotify(const CNetAddr &addrFrom, COperatePacket *p);
	// 释放升级助手并运行
	bool FreeAndRunUpdate();

	friend class CSingle<CUdpServer>;

};

