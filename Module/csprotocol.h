// csprotocol.h
#ifndef _CSPROTOCOL_H
#define _CSPROTOCOL_H

#include "gconst.h"
#include "datatypedef.h"
//////////////////////////////////////////////////////////////////////////////

#define RET_SUCCESS         0
#define RET_FAILD           -1

// 已加入网络
#define RET_ALREAD_ADDNET   100
// 等待加入网络
#define RET_WAITTING_ADDNET   10

//----------------------------------------------------------------------------
//-- 动作代码:

//客户端 -> 客户端: (占用代码段: 1 .. 100)
const int CN_DEBUG = 1;						// 调试							
const int CN_LOGIN = 2;						// 加入网络							
const int NC_LOGIN = 3;						// 回应加入网络	（普通节点回应登录）						
const int CC_NOTIFYINSERT = 6;				// 通知周围机器有新节点加入网络						
const int CC_KEEPALIVE = 8;					// 						
const int CC_REKEEPALIVE = 9;				// 						
const int CN_NOTIFYCHANGEMACHINE = 10;      // 通知所有机器网络节点数量改变						
const int NC_NOTIFYCHANGEMACHINE = 11;      // 通知所有机器网络节点数量改变回复						
const int CN_LOGINSERVER = 12;				// 通知登录中心服务器							
const int NC_LOGINSERVER = 13;				// 通知登录中心服务器							
const int CS_LOGINSERVER = 14;				// 通知登录是否成功							
const int SC_LOGINSERVER = 14;				// 通知登录是否成功							
const int CN_CHAT = 15;						// 公聊聊天信息							
const int CC_CHAT = 16;						// 私聊聊天信息		
const int CC_DCHAT = 17;					// 吧台指定弹幕
const int CC_GAMEINFO = 18;                 //吧台采集游戏信息
const int SC_GAMEINFO = 19;       
const int SC_JIONGAME = 20;                    //邀请别人进入房间
const int CC_GAMEINFOEX = 21;                    // 游戏信息(客户端打开、退出游戏相关模块)

const int CN_COMMON_UPDATEDATA = 30;			// 有数据更新， 如：比赛房间信息更新

const int CCN_LOGINSERVER = 50;				// 控制台通知登录中心服务器							
const int CCN_CALLSERVER = 51;				// 控制台通知登录中心服务器							

// 通用应答
const int CC_ACK = 1000;      // 通用应答						

#pragma pack(1)     //1字节对齐
//----------------------------------------------------------------------------
//-- 数据包首部:

struct CPacketHeader
{
    word nActionCode;       // 动作代码
    byte nProtoVer;         // 协议版本
	word nSeri;				// 报文序号
    word nPacketLen;        // 报文全长
    uint nCheckSum;          // 校验和

private:
    int GetCheckSum()
	{
		int nChek = 0;// nProtoVer ^ nActionCode ^ nSeri ^ nPacketLen;
		for (int i = 0; i < sizeof(CPacketHeader)-4; i++)
		{
			unsigned char *p = (unsigned char *)this;
			nChek ^= *(p + i);
		}
		for (int i = sizeof(CPacketHeader); i < nPacketLen; i++)
		{
			unsigned char *p = (unsigned char *)this;
			nChek ^= *(p + i);
		}
		return nChek;
	}
public:
	void InitHeader(word nActionCodeA, word nSeriA, word nPacketLenA, int nProtoVerA = 0)
	{
		nProtoVer = nProtoVerA;
		nActionCode = nActionCodeA;
		nSeri = nSeriA;
		nPacketLen = nPacketLenA;
		nCheckSum = GetCheckSum();
	}
	bool CheckHeaderIsValid(){ return (GetCheckSum() == nCheckSum) && (nPacketLen >= sizeof(CPacketHeader)); }
};

#pragma pack()



#endif
