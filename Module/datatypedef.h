#ifndef _DATATYPEDEF_H
#define _DATATYPEDEF_H

#include <map>
#include <time.h>
#include <string>
#include <list>
#include <queue>

#include "gconst.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
// 类型定义

typedef unsigned char       byte;
typedef unsigned short      word;
typedef unsigned int        dword;
typedef unsigned int        uint;
#ifdef _WINDOWS
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
typedef long long			int64;
typedef unsigned long long	uint64;
#endif

#pragma pack(1)     //1字节对齐
// IP - Port 结构
struct NetAddr
{
	char sIp[16];           // 服务器的IP地址 (网络字节顺序)
	word nPort;             // 服务器的UDP端口
	NetAddr() :nPort(0) { memset(sIp, 0, 16); }
	NetAddr(const char *ip, word wport) :nPort(wport) { memcpy(sIp, ip, 15); sIp[15] = 0; }
};

#pragma pack()     //1字节对齐

//// 比赛详情
//struct CMatchDetail
//{
//	// {"KDA":"4",
//	string strKDA;
//	// "aReward":"0",
//	string strReward;
//	// "area":"艾欧尼亚",
//	string strArea;
//	// "assists":"0",
//	string strAssists;
//	// "beginTime":"22",
//	string strBeginTime;
//	// "damTaken":"32121",
//	string strDamTaken;
//	// "damToChan":"1213",
//	string strDamToChan;
//	// "df":"闪现点燃",
//	string strDf;
//	// "die":"12",
//	string strDie;
//	// "game":"英雄联盟",
//	string strGame;
//	// "gameName":"妹子进",
//	string strGameName;
//	// "killed":"1",
//	string strKilled;
//	// "mvp":"0",
//	string strMvp;
//	// "playTime":"30",
//	string strPlayTime;
//	// "qita":"",
//	string strQita;
//	// "result":"1",
//	string strResult;
//	//// "shopName":"1",
//	//string strBarName;
//	// "shopNo":"1",
//	string strBarId;
//	// "userId":"2",
//	string strUserId;
//	// "userName":"das",
//	string strUserName;
//	// "zhuangBei":"卡上打卡上的空间啊是打开"}]
//	string strZB;
//	//
//	//
//};
//// 房间里的用户信息
//struct MatchUserInfo
//{
//	string id;		// 用户标识
//	string objname;//名称
//	string netbarid;//网吧ID
//	string netbarname;//网吧名称
//	string gamename;//游戏内名称
//};
//
//// 比赛详情
//struct CMatchCommonInfo
//{
//	// "id" : "", 
//	string id; //比赛ID
//	// "objname" : "liukun的房间", 
//	string objname;//比赛名称
//	// "netbarid" : "testbarid", 
//	string netbarid;//网吧ID
//	// "netbarname" : "testbarname", 
//	string netbarname;//网吧名
//	// "matchpwd" : "111111", 
//	string matchpwd;//加入密码
//	// "tiaozhanj" : 100,
//	int tiaozhanj = 0;//挑战金
//	// "dashangj" : 0, 
//	int dashangj = 0;//打赏金
//	// "pipei" : "10", 
//	string pipei;//匹配模式
//	string dashangw;//打赏物
//	// "youxi" : "1" }
//	string youxi;//游戏
//	// "daqu" : "3109", 
//	string daqu;//大区
//	string objstatus;//状态
//	// { "createtime":"2016-06-19 13:37:51", 
//	string createtime;//创建时间
//	// "opt" : "", 
//	string opt = "";//操作（创建，加入，退出）
//	// "starttime" : "", 
//	string starttime = "";//比赛开始时间
//	// "endtime" : "", 
//	string endtime = "";//比赛结束时间
//	// "leaderid" : "testuserinfo", 
//	string leaderid = "";//房主id
//	// objtype("0" - 本吧； ”1“ - 跨吧)
//	string objtype;
//	// 模式
//	string gamemode;
//	// 比赛绑定的活动id
//	string gameactiveid;
//	// 比赛绑定的活动是不是要跳转
//	string gameisjump;
//	// "users" : ["testuserinfo"], 
//	list<MatchUserInfo> ltUsers;//成员ID列表
//};
// 游戏类型项
struct GameTypeItem
{
	// 游戏类型ID(整型int)
	string strId;
	// 游戏类型名称
	string strName;
	// 游戏父类型ID
	string strParentId;
};

// 游戏库数据项
struct GameLibItem
{
	//// 和请求参数一致，本地库类型
	//int type;
	// 游戏编号，中心游戏库的游戏编号 center_game_id
	int64  center_game_id;// game_id;
	// 游戏名称，方便查看
	string game_name;
	// 游戏显示名称
	string game_showname;
	// 游戏名称拼音，方便查找
	string name_py;
	// 游戏图标相对路径
	string game_icon;
	// 启动程序相对路径
	string launcher;
	// 主程相对路径
	string process;
	// 游戏点击量;
	int64  game_dp = 0;
	// 游戏分类，也是游戏分类目录
	string category_dir;
	// 游戏一级分类
	string category_1;
};

// 排位库游戏数据
struct SortGameItem
{
	// 游戏编号，中心游戏库的游戏编号
	int64 game_id;
	// 游戏名称，方便查看
	string game_name;
	// 启动程序URL，本地保存时请去掉md5字符串
	string launcher;
	// 启动程序md5字符串
	string launchermd5;
	// 可选的图标URL，可以是PNG/ICO/EXE/DLL，如果是EXE/DLL，则从资源部分获取
	string icon;
	// icon md5字符串
	string iconmd5;
	// 对应本地文件名
	string iconpathname;
	// 对应本地文件名
	string launchpathname;
	// 排位顺序号
	int rank;
	// 标志位 0 的时候是普通，1的时候是高亮显示
	int flag = 0;
	// 浮层URL
	string popurl;
	// 浮层 md5字符串
	string popmd5;
	// 浮层对应本地文件名
	string poppathname;
};

// 排位库数据
struct SortDataItem
{
	// 策略针对日期，yyyy-MM-dd格式的字符串
	string strDate;
	list<SortGameItem> ltGame;
};

// 升级包数据
struct UpLevelDataItem
{
	// 升级程序类型
	int type;
	// 版本带有到秒的日期，用来判断是否需要升级
	string ver;
	// 下载URL
	string url;
	// 单位字节（B），下载文件的大小
	int size;
	// 对应本地文件名
	string pathname;
	// md5
	string md5;
};

// 小程序数据
struct ProgramDataItem
{
	// 小程序类型，默认1
	int type;
	// 版本带有到秒的日期，用来判断是否需要升级
	string ver;
	// 下载URL
	string url;
	// 单位字节（B），下载文件的大小
	int size;
	// 对应本地文件名
	string pathname;
	// md5
	string md5;
};

// 资源数据
struct ResourceDataItem
{
	// "resources":[{"size":20984703,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/game_icons.zip","md5":"2cf0c87d74f333de1e6935c0ca82d207"}]
	// 单位字节（B），下载文件的大小
	int size;
	// 资源类型，默认1-游戏图标; 100-顺网游戏库；200-信佑游戏库；300-易游游戏库；400-云更新游戏库
	int type;
	// 下载URL
	string url;
	// md5
	string md5;
	// 对应本地文件名
	string iconpathname;
};

// 配置数据
struct ConfigDataItem
{
	// 资源列表
	list<ResourceDataItem> ltRD;
	// 排位库
	list<SortDataItem> ltSD;
	// 升级包列表
	list<UpLevelDataItem> ltUD;
	// 小程序列表
	list<ProgramDataItem> ltPD;
	// 上报的URL
	string talking_url;
	// 游戏快捷方式的名称
	string game_menu;

	// 获取对应的图标本地文件全名
	string GetIconPathName(const string &strGameId, const string &strIconMd5)
	{
		string strRet;
		for (auto it : ltRD)
		{
			if (strIconMd5.empty())
			{
				//if ()
			}
		}
		return strRet;
	}
};

// 游戏类型项
struct ConfigIIDataItem
{
	// 小程序类型，默认1
	int type;
	// 下载URL
	string url;
	// md5
	string md5;
	// 单位字节（B），下载文件的大小
	int size;
	// 对应本地文件名
	string param;
};

// 游戏类型项
struct ConfigIIItem
{
	// 返回结果（0 - 成功）
	int ret_code;
	string ret_msg;
	// 策略列表
	list<ConfigIIDataItem> lt;
};


#endif
