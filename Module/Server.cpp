/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：imserver.cpp
* 内容摘要：主服务类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2012.6.10
* 修改记录：
* 1.
* 修改日期：
* 版 本 号：
* 修 改 人：
* 修改内容：
******************************************************************************/
// Server.cpp : 定义应用程序的入口点。
//

//#include "stdafx.h"
#include "Server.h"

#include "server.h"
#include "configfile.h"
#include "configfileconst.h"
#include "udpserver.h"
//#include "ChannelServer.h"
#include "JsonCode.h"
#include "CenterInterface.h"
#include "BarInfo.h"
//#include "SendRecvServer.h"
//#include "SendApplicationServer.h"

//1.pre_server		： 分流服务器，			UDP内网监听端口-6000; UDP外网监听端口-7070
#define PRESERVER_LINSTEN_UDPINNERPORT		6000
#define PRESERVER_LINSTEN_UDPOUTERPORT		7070
//2.state_server	： 状态服务器，			UDP内网监听端口-6001;
#define STATESERVER_LINSTEN_UDPINNERPORT	6001
//3.group_server	： 群在线服务器，		UDP内网监听端口-6002;
#define GROUPSERVER_LINSTEN_UDPINNERPORT	6002
//4.operator_server	： 业务服务器，			UDP内网监听端口-6003; UDP外网监听端口-7071 TCP外网监听端口7071
#define OPERATORSERVER_LINSTEN_UDPINNERPORT	6003
#define OPERATORSERVER_LINSTEN_UDPOUTERPORT	7071
#define OPERATORSERVER_LINSTEN_TCPOUTERPORT	7071
//5.file_server		： 文件服务器，			UDP内网监听端口-6004; TCP外网监听端口-7072
#define FILESERVER_LINSTEN_UDPINNERPORT		6004
#define FILESERVER_LINSTEN_TCPOUTERPORT		7072


#include <iostream>
using namespace std;

//// 键 - IP地址， 值 - 
//map<string, ContactPosItem *> m_mapItem;
//ContactPosItem m_cpItem;
//
void WINAPI WebSockClientCallbackFun(const void *pCallbackParam, const string &strRecv)
{
	strRecv;
	//char *szBuf = "test";
	//int nRet = CCommInterface::SendDataByWebSockClient(pWebSocketClient, szBuf, sizeof(szBuf));
	//char szRet[100];
	//int nFlag(0);
	//nRet = CCommInterface::RecvDataFromWebSockClient(pWebSocketClient, szRet, sizeof(szRet), nFlag);
}

void WINAPI WebSockClientCallbackFun2(const void *pCallbackParam, const void *pWebSocketClient)
{
	//char *szBuf = "test";
	//int nRet = CCommInterface::SendDataByWebSockClient(pWebSocketClient, szBuf, sizeof(szBuf));
//	char szRet[100];
	//int nFlag(0);
	//nRet = CCommInterface::RecvDataFromWebSockClient(pWebSocketClient, szRet, sizeof(szRet), nFlag);
}

//#include "MatchAppClient.h"
void test()
{
	//CMatrix::GetInstance()->Init();
	//string strMachine = "001"; //"W6SYL9UKUJMBG6D";
	//CNetAddr addrFrom("192.166.0.244", 7000);
	//// 插入邻居
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// 插入邻居
	//strMachine = "002";// "WYOFFICE_";
	//addrFrom.SetIp("92.168.5.52");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// 插入邻居
	//strMachine = "034";
	//addrFrom.SetIp("192.166.0.34");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	//// 插入邻居
	//strMachine = "041";
	//addrFrom.SetIp("192.166.0.41");
	//CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);
	////// 插入邻居
	////strMachine = "W6SYL9UKUJMBG6D";
	////addrFrom.SetIp("192.166.0.244");
	////CMatrix::GetInstance()->InsertNeighbourTest(strMachine, addrFrom, 0);



	string strAction;
	string strMatchInfo;
	string strUserId;
	string strArea;
	string strGameName;
	string strTier;
	string strWinner;
	string strRecv = "{\"ActionName\":\"GetLolMatchInfo\",\"Values\":{\"killlist\":[{\"name\":\"要打打我别打队友\",\"kill\":\"2\"}], \"win\":\"0\"}}";
	//if (CJsonDecode::DecodeLolInfo(strAction, strRecv, strUserId, strArea, strGameName, strTier, strMatchInfo, strWinner))
	//{
	//	if (!strMatchInfo.empty())
	//	{
	//		// 向中心发送赛后数据
	//		CCenterInterface::FinishMatch(strMatchInfo, strWinner);
	//	}
	//}
	//	
		
	string strRet;
	//strRet = CCenterInterface::GetScreenShow();
	//strRet = CCenterInterface::CreateScreenShow("asdfsdfaasdf", "8:00", "asdf", "3|5|7|8|", "0");
	//strRet = CCenterInterface::ModifyScreenShow("11bce23cfce14a6fbdfc366f2a763f53", "aaaaaaaaaa", "9:00", "asdf", "3|5|7|8|9", "1");
	//strRet = CCenterInterface::SetScreenShowSwitch("11bce23cfce14a6fbdfc366f2a763f53", "2");
	//strRet = CCenterInterface::DelScreenShow("11bce23cfce14a6fbdfc366f2a763f53");
	//// {"createtime":"2016-06-21 15:57:09","daqu":"","dashangj":0,"endtime":"","id":"02823c2d5dda4b61bca15ee3e00f9fe2","leaderid":"testuserinfo","matchpwd":"111","modifytime":"2016-06-21 15:57:09","netbarid":"testbarid","netbarname":"testbarName","objname":"111","objstatus":"0","opt":"create","pipei":"6","starttime":"","tiaozhanj":1000,"users":[{"id":"testuserinfo","netbarid":"testbarid","netbarname":"testbarName","objname":"testbarName-004"}],"youxi":""}
	//string s = "{\"createtime\":\"2016-06-21 15:57:09\",\"daqu\":\"\",\"dashangj\":0,\"endtime\":\"\",\"id\":\"02823c2d5dda4b61bca15ee3e00f9fe2\",\"leaderid\":\"testuserinfo\",\"matchpwd\":\"111\",\"modifytime\":\"2016-06-21 15:57:09\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"111\",\"objstatus\":\"0\",\"opt\":\"create\",\"pipei\":\"6\",\"starttime\":\"\",\"tiaozhanj\":1000,\"users\":[{\"id\":\"testuserinfo\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"testbarName-004\"}],\"youxi\":\"\"}";
	////wstring s = L"{ \"ActionName\":\"CreateRoom\", \"Values\" : {\"Area\":\"\", \"Game\" : \"\", \"IsLocalBar\" : \"0\", \"MatchMoney\" : \"1000\", \"PersonNum\" : \"8\", \"RoomName\" : \"111\", \"RoomPass\" : \"111\"} }";
	//       s = "{\"createtime\":\"2016 - 06 - 21 16:32 : 04\",\"daqu\":\"\",\"dashangj\":0,\"endtime\":\"\",\"id\":\"608e0a5920e04e3a8e9a7da7b0973a6a\",\"leaderid\":\"UserTest\",\"matchpwd\":\"111\",\"modifytime\":\"2016 - 06 - 21 16 : 32 : 04\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"111\",\"objstatus\":\"0\",\"opt\":\"create\",\"pipei\":\"6\",\"starttime\":\"\",\"tiaozhanj\":1000,\"users\":[{\"id\":\"UserTest\",\"netbarid\":\"testbarid\",\"netbarname\":\"testbarName\",\"objname\":\"testbarName - 004\"}],\"youxi\":\"\"}";
	//CMatchCommonInfo item;
	//CJsonDecode::DeCodeMatchCommonData(s, item);
	//CMatchAppClient ct(22, L"xxxxx");
	//ct.ProcData(s);
	// 获取比赛列表
	//string strInfo = CCenterInterface::GetMatchList("", "", "", "");
	//// 创建
	//// 创建比赛接口
	//CCenterInterface::CreateRoom("1", "3109", "liukun的房间", "111111", "10", "100", 0, &WebSockClientCallbackFun);
	//Sleep(6000);
	//// 进入房间接口
	//CCenterInterface::EnterRoom("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// 退出房间接口
	//CCenterInterface::QuitRoom("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// 打赏记录接口
	//CCenterInterface::SetReward("100", "4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// 开始比赛接口
	//CCenterInterface::StartMatch("4b7e2ca0d6b446f995ae4ffa19175dac", 0, &WebSockClientCallbackFun);
	//// 修改比赛接口
	//CMatchCommonInfo item;
	//item.id = "c85ba5e912f8445a88b3ed59625238b4";
	//item.youxi = "1";
	//item.daqu = "3901";
	//item.objname = "liukun的房间";
	//item.matchpwd = "222222";
	//item.dashangj = 100;
	//item.tiaozhanj = 100;
	//item.pipei = "8";
	//CCenterInterface::ModiMatch(item, 0, &WebSockClientCallbackFun);
	//Sleep(1000000);
	//CCenterInterface::GetBroadcastList();
	// 编辑定时语音播报
	//string strRet;// = CCenterInterface::ModifyBroadcastRecord("cb5e90555bc548d9861a163ad8eafde2", "15:00", "0", "1");
	//strRet = CCenterInterface::SetBroadcastSwitch("cb5e90555bc548d9861a163ad8eafde2", "0");
	//// 获取
	//CCenterInterface::GetMatchList("", "", "", "");
	// 
	//const char *sUrl = "ws://192.168.102.186:8080/ybing/websocket";
	//CCommInterface::CreateWebSockClient(sUrl, 0, WebSockClientCallbackFun2);
	//string strRet = CCenterInterface::CreateMatch("LOL001", "3601", "liukun", "111111", 10, "100");
	//int nSex = GetSexById("360103197811194418");
	// [{"id":"111", "jsonrpc":"2.0","method":"play_mp3","params":["女神驾到.mp3"]},{"jsonrpc":"2.0","method":"play_mp3","params":["1.mp3"],"id":"12255"},{"id":"111","jsonrpc":"2.0","method":"play_mp3","params":["号机.mp3"]},{"id":"111", "jsonrpc":"2.0","method":"play_mp3","params":["女神闪亮登场.mp3"]}]
	//strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"提示9.mp3\"],\"id\":\"12255\"},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"提示3.mp3\"],\"id\":\"12256\"}]"; //
	//string strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"欢迎光临网鱼网咖\"],\"id\":\"12255\"}]"; //
	strRet = "[{\"jsonrpc\":\"2.0\",\"method\":\"play_tts\",\"params\":[\"1号机需要网管帮助\"],\"id\":\"12255\"}]"; //
	//strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB(CConfigFile::GetInstance()->GetLocalMachineName(), 0);
	//strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB("223", 0);
	//strRet = "亲爱的鱼粉，本网咖通宵时间为晚上24点至次日早上8点，请您检查卡内余额";
	//strRet = CJsonEncode::EnCodeYYBBTTSMessage(strRet);

	const int nRetLen = 10 * 1024;
	char *szRet = new char[nRetLen];
	int nState(0); 
	string s("亲爱的鱼粉，本网咖通宵时间为晚上24点至次日早上8点，请您检查卡内余额。为了防止重复扣费，请不要随意更换机器。如有疑问，请联系吧台服务员。");

	////s = "亲爱的鱼粉，本网咖正在进行英雄联盟比赛，有丰厚奖品哦，赶快约上队友来吧台报名吧。";
	//strRet = CJsonEncode::EnCodeYYBBMp3Message(s, "");
	////strRet = "[{\"id\":\"\", \"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"50.本网咖通宵.mp3\"]},{\"id\":\"\", \"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"51.晚上.mp3\"]},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"20.mp3\"],\"id\":\"\"},{\"jsonrpc\":\"2.0\",\"method\":\"play_mp3\",\"params\":[\"4.mp3\"],\"id\":\"\"}]";
	CCommInterface::PostHttpData("http://192.168.102.82:8080/voice_api", strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//CCommInterface::PostHttpData("192.168.101.157", strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//CCommInterface::PostHttpData(CConfigFile::GetInstance()->getYYBBServerUrl().c_str(), strRet.c_str(), strRet.size(), szRet, nRetLen, nState, "application/json");
	//string strRet = CJsonEncode::GetInstance()->EnCodeUserLoginYYBB("2", 0);
	//m_cpItem.strMachineName = "003";
	//ContactPosItem *pItem = new ContactPosItem;
	//pItem->strMachineName = "001";
	//ContactPosItem *pItem2 = new ContactPosItem;
	//pItem2->strMachineName = "002";
	//ContactPosItem *pItem4 = new ContactPosItem;
	//pItem4->strMachineName = "004";
	//ContactPosItem *pItem5 = new ContactPosItem;
	//pItem5->strMachineName = "005";
	//m_mapItem["192.168.102.168:7000"] = pItem;
	//m_mapItem["192.168.101.243:7000"] = pItem2;
	//m_mapItem["192.168.102.169:7000"] = pItem4;
	//m_mapItem["192.168.101.244:7000"] = pItem5;
	//CalcNeighbourRef();
	exit(0);
}

CServer::CServer()
//:m_serTcp(OPERATORSERVER_LINSTEN_UDPOUTERPORT)
{
//#ifdef _DEBUG
//	test();
//#endif
	m_bStop = false;
	init();
}

CServer::~CServer()
{
	uninit();
}

// 服务运行 
void CServer::Run()
{
	// 防止重复运行程序
	SECURITY_DESCRIPTOR sd;
	if (0 == InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) || 0 == SetSecurityDescriptorDacl(&sd, TRUE, (PACL)0, FALSE))
	{
		return;
	}
	else
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;
		HANDLE hSem = CreateSemaphore(&sa, 1, 1, _T("BarPlatform_Semaphore"));
		//HANDLE hSem = CreateSemaphore(NULL, 1, 1, _T("DW_Bar_Client"));
		DWORD dwCheckThread = GetLastError();
		if (dwCheckThread == ERROR_ALREADY_EXISTS)
		{
			return;
		}
	}
	if (!LoadConfig())
	{
		return;
	}
	////CYYBBClient::GetInstance()->Run();
	//CUserOnlineInfo::GetInstance();
	// 启动UDP监听服务 
	CUdpServer::GetInstance()->Run();
	//// 启动TCP监听服务 
	//m_serTcp.Run();

	//CMatrix::GetInstance()->Init();

	//CChannelServer::GetInstance()->StartServer();
	//CSendRecvServer::GetInstance()->Run();
	//CSendAppServer::GetInstance()->Run();
	//CCenterInterface::UploadInfo();
}

// 初始化
void CServer::init()
{
	// 提前初始化，防止多线程初次访问单件下出错(实例，多线程访问下queue.size() = -64 < 0)
	InitLogs();

	//CUserInfoList::GetInstance();
	CBarInfo::GetInstance();
	CConfigFile::GetInstance();
	CJsonEncode::GetInstance();
	CJsonDecode::GetInstance();
	//CCallServer::GetInstance();
	//CAppClientFactory::GetInstance()->Run();
	//CWebSocket::GetInstance();

}
void CServer::uninit()
{
	//CWebSocket::FreeInstance();
	//CSendAppServer::FreeInstance();
	//CSendRecvServer::FreeInstance();
	//CChannelServer::FreeInstance();
	//CMatrix::FreeInstance();
	//// 停止TCP监听
	//m_serTcp.Stop();
	// 停止UDP监听
	CUdpServer::FreeInstance();
	//CCallServer::FreeInstance();
	//CAppClientFactory::FreeInstance();
	CJsonDecode::FreeInstance();
	CJsonEncode::FreeInstance();
	CConfigFile::FreeInstance();
	CBarInfo::FreeInstance();
	//CUserInfoList::FreeInstance();
	//CUserOnlineInfo::FreeInstance();
	FreeLogs();
}

// 加载配置信息
bool CServer::LoadConfig()
{
	//CConfigFile *p = CConfigFile::GetInstance();
	//if(p->GetCount() != CONFIGFILE_PARAMCOUNT)
	//{
	//	return false;
	//}
	//// 内网监听IP
	//m_stInnerIp = p->GetStringValueA(CONFIGFILE_INDEX_INNERADDRIP);
	//if(m_stInnerIp.empty())
	//{
	//	return false;
	//}
	return true;
}

