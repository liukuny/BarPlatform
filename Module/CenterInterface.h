/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：centerinterface.h
* 内容摘要：中心接口
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2014.7.28
* 修改记录：
* 1.
* 修改日期：
* 版 本 号：
* 修 改 人：
* 修改内容：
******************************************************************************/
#pragma once
#include "gclass.h"
#include "WebSocket.h"
#include "CommonStruct.h"

#pragma pack(1)
typedef struct tag_WY_COMMU_HEAD
{
	UINT32	_head_flag;			//包头
	INT32	_seq_no;			//请求方生成，响应方原值返回  正包号是请求的，负包号是服务器直接推送
	UINT32	_command_id;		// 指令ID
	UINT32	_data_len;			//不含包头长度，无消息体时，长度为0
	UINT32	_checksum;			//预留checksum，以后校验
}CCommonTCPPacket;

#pragma pack()	//	end #pragma pack(1)



class CCenterInterface
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 获取资源接口
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 获取更新包配置
	static string GetUpdate();
	// 获取配置
	static string GetConfig();
	// 获取策略
	static string GetConfigII();
	// 获取菜单更新包
	static string GetMenuUpdate();
	// 获取小程序
	static string GetMinProgram();
	// 获取游戏库
	static string GetGameLib();
	// 上传启动数据 （10 - 顺网； 11 - 云更新； 12 - 易游； 13 - 信佑；14 - 方格子；15 - 绿化大师）
	static string UploadInfo(const string &strUrl, const string strAction = "1");
	// 从本地下载资源文件到本地
	static bool DownloadFromP2P(const string &strUrl, const string &strFileName, const string &strMd5);
	// 下载资源文件到本地
	static bool DownloadFile(const string &strUrl, const string &strFileName, const string &strMd5);

public:
	CCenterInterface();
	~CCenterInterface();
private:
	// 获取Http数据
	static string GetHttpData(const string &strUrl, map<string, string>mapParam, bool bBigRet = false, bool bIsPost = false);
	// 获取Http数据请求链接
	static string GetHttpDataUrl(const string &strPath);
	// 回调函数指针类型
	// 参数：
	//   pCallbackParam - 传给回调函数的上层参数；
	//   pSreamSocket	- 此连接SreamSocket对象指针；
	//   pRemoteIp		- 此连接中的远程Ip；
	//   nRemotePort	- 此连接中的远程端口；
	static void WINAPI ClientProcTcpConnect(void *pCallbackParam, void *pSreamSocket, char *pSendBuf, int nBufLen);
};

