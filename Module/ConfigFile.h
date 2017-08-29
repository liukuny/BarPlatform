/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：configfile.h
* 内容摘要：配置模块
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

class CConfigFile :
	public CSafeSingle<CConfigFile>
{
	//public:
public:
	// 转换为绝对路径 
	static void SwitchAbsPath(string &strPath);
public:
	// 获取是否生成桌面图标
	bool GetIsCreateDeskIcon() { return m_bCreateDeskIcon; }
	// 是否调用菜单
	bool GetIsCallMenu() { return m_bCallMenu; }
	// 是否调用P2P
	bool GetIsCallP2P() { return m_bCallP2P; }
	// 是否下载图标库
	bool GetIsDLIconLib() { return m_bDownloadIconlib; }
	// 开启配置下载扫描周期（单位分钟，0 - 不扫描）
	int GetDLConfigInterval() { return m_nDownloadConfigInterval; }

	// 获取渠道Id
	string GetAgencyId() { return m_strAgencyId; }
	// 设置游戏类型
	void SetGameMenuType(const string &strGameMenuType) { m_strGameMenuType = strGameMenuType; }
	// 是否方格子
	bool IsFGZ(){ return m_strGameMenuType.compare("500") == 0; }
	// 是否云更新
	bool IsClundUpdate(){ return m_strGameMenuType.compare("400") == 0; }
	// 是否顺网
	bool IsShunwang(){ return m_strGameMenuType.compare("100") == 0; }
	// 获取游戏类型
	string GetGameMenuType() { return m_strGameMenuType; }
	// 获取BarId
	string GetBarId() { return m_strBarId; }
	// 获取MAC
	string GetMac() { return m_strMac; }
	// 获取本地IP
	string GetLocalIp() { return m_strIp; }
	// 获取游戏菜单程序全名
	string GetGameMenuProgram() { return m_strGameMenu; }
	// 获取游戏资源路径
	string GetGameResPath();
	// 获取游戏库名
	string GetGameLibFileName() { return m_strGameLibFileName; }
	// 获取游戏类型名
	string GetGameTypeFileName() { return m_strGameTypeFileName; }
	// 获取游戏升级库名
	string GetGameSortFileName() { return m_strSortFileName; }
	// 获取小程序路径名
	string GetMinProgramFilePath();
	// 获取策略路径名
	string GetPolicyFilePath();
	// 获取平台升级包路径名
	string GetPlatformUpdateFilePath();
	// 获取桌面快捷方式名称
	string GetLinkFileName() { return m_strLinkFileName; }

	// 中心服务UDP监听端口
	int GetServerUdpPort() { return atoi(m_strServerUdpPort.c_str()); }
	// 中心服务监听ip
	string GetServerIp() { return m_strServerIp; }
	// 中心服务HTTP监听端口
	string GetServerHttpPort() { return m_strServerHttpPort; }
	// 中心服务HTTP监听IP端口
	string GetServerHttpIpPort() {

		string s = m_strServerIp;
		if (!m_strServerHttpPort.empty())
		{
			s += ":";
			s += m_strServerHttpPort;
		}
		return	s;
	}
	// 中心服务HTTP监听IP端口
	string GetClientIp() {
		return	m_strClientIp;
	}

	//// 网吧服务监听端口
	//int GetServerTcpPort(){ return atoi(m_strServerTcpPort.c_str()); }
	//string GetServerTcpPortStr(){ return m_strServerTcpPort; }
	//// 注册接口URL
	//string GetRegeditAddr(){ return m_strRegeditAddr; }
	// 服务端版本
	string GetServerVer() { return m_strServerVer; }
	void SetServerVer(string &strServerVer) { m_strServerVer = strServerVer; }
	// 客户端版本
	string GetClientVer() { return m_strClientVer; }
	// 菜单版本
	string GetMenuVer() { return m_strMenuVer; }

	// 获取图片文件路径
	string GetImageFilePath() { return m_strImagePath; }
	// 加载升级配置文件
	void LoadUpdateConfig(const string &strUpdateFileName);

	// 获取本地UDP监听端口
	word GetLocalUDPListenPort() { return m_nLocalUdpPort; }
	// 获取机器名
	string GetLocalMachineName() { return m_strMachineName; }

	// 生成全路径名
	std::string CreateFullPathName(std::string strFileName);
private:
	void	LoadConfig();

	// 取MAC,Ip
	void GetSysMacAndIp();
	// 取硬盘编号
	void GetSysDiskNum();
private:
	// 是否调用菜单
	bool m_bCallMenu = true;
	// 是否调用P2P
	bool m_bCallP2P = true;
	// 是否下载图标库
	bool m_bDownloadIconlib = true;
	// 开启配置下载扫描周期（单位分钟，0 - 不扫描）
	int m_nDownloadConfigInterval = 0;


	// 是否生成桌面图标
	bool m_bCreateDeskIcon = true;
	// 设置游戏更新类型(已知本地库类型：100-顺网；200-信佑；300-易游；400-云更新)
	string m_strGameMenuType; 
	// 中心服务IP
	string m_strServerIp;
	// 中心服务UDP监听端口
	string m_strServerUdpPort;
	// 中心服务TCP监听端口
	string m_strServerTcpPort;
	// 中心服务HTTP监听端口
	string m_strServerHttpPort;
	// 渠道Id
	string  m_strAgencyId; 
	// 游戏菜单程序全名
	string  m_strGameMenu;
	// 游戏资源路径
	string  m_strGameMenuResPath;
	// 游戏库名
	string  m_strGameLibFileName;
	// 获取游戏类型名
	string  m_strGameTypeFileName; 
	// 游戏升级库名
	string  m_strSortFileName;
	// 获取小程序路径名
	string m_strMinProgramFilePath;
	// 策略路径名
	string m_strPolicyFilePath;
	// 平台升级包路径名
	string m_strPlatformUpdateFilePath;
	// 桌面快捷方式名称
	string m_strLinkFileName;

private:
	// BarId
	string	m_strBarId;
	// MAC地址
	string	m_strMac;
	// Ip地址
	string m_strIp;
	// 硬盘编号
	string	m_strDiskNum;
	// 服务端版本
	string  m_strServerVer;
	// 客户端版本
	string  m_strClientVer;
	// 菜单版本
	string m_strMenuVer; 
	// 图片文件路径
	string  m_strImagePath;
	// 本地UDP监听端口
	word m_nLocalUdpPort;
	// 机器名
	string m_strMachineName;
	// 获取客户端需要的地址
	string  m_strClientIp;
private:
	CConfigFile();
	~CConfigFile();

	friend class CSingle<CConfigFile>;
};

