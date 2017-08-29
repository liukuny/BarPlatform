#pragma once

#include "gclass.h"
#include "CommonStruct.h"
#include "netbase.h"
typedef struct tag_MEM_CONFIG_INFO_STRUCT
{	
	// clientId
	char clientId[20];
	// 本地Ip
	char localIp[16];
	// 渠道编号
	char agencyId[40];
	// 上传URL
	char szUploadUrl[512];
	// 菜单名称
	char szMenuName[40];
	//char szMoney[40];              
}CLIENT_CONFIG_INFO, *LP_CLIENT_CONFIG_INFO, *P_CLIENT_CONFIG_INFO;

class CBarInfo : public CSafeSingle<CBarInfo>
{
public:
	// 获取游戏库
	void GetGameLib();
	// 是否取得游戏库
	bool IsGetGameLib(){ CSafeLock<CBarInfo> lk(this); return m_bGetGameLib; }
	// 设置游戏库
	void SetGetGameLib(list<GameLibItem> &ltGame){ CSafeLock<CBarInfo> lk(this); m_ltGame = ltGame; }
	// 获取配置
	void GetConfig();
	// 扫描其他游戏菜单上传
	void ScanGameMenuAndUpload();
	// 是否取得配置
	bool IsGetConfig(){ CSafeLock<CBarInfo> lk(this); return m_bGetConfig; }
	// 设置配置
	ConfigDataItem GetLocalConfig()
	{
		CSafeLock<CBarInfo> lk(this);
		return m_cfg;
	}
	// 设置配置
	void SetConfig(const ConfigDataItem &cfg)
	{
		CSafeLock<CBarInfo> lk(this);
		m_cfg = cfg;
		m_bGetConfig = true;
	}
	string GetTalkUrl()
	{
		CSafeLock<CBarInfo> lk(this);
		return m_cfg.talking_url;
	}
private:
	// 云更新是否验证路径开关（true - 验证）
	bool m_bCloudSWitch = true;
	// 云更新虚拟盘符名-实际盘符名键值对
	map<string, string> m_mapDiskName;
	// 云更新游戏名-游戏本地ID键值对
	map<string, string> m_mapLocalId;
	// 云更新游戏名-路径名键值对
	map<string, string> m_mapNamePath;
	// 是否取得游戏库
	bool m_bGetGameLib = false;
	// 游戏类型
	list<GameTypeItem> m_ltGameType;
	// 游戏库
	list<GameLibItem> m_ltGame;
	// 配置数据
	ConfigDataItem m_cfg;
	// 配置文件内存句柄
	HANDLE m_hConfigMapping = 0;
	// 网吧配置
	// 是否取得配置
	bool m_bGetConfig = false;
	// 网吧配置项
	ConfigItem m_configItem;
	// 保存配置信息到内存
	void SaveInfoToMem(const ConfigDataItem &cfg);
	// 获取配置
	void GetConfigI();
	// 获取策略
	void GetConfigII();
	// 获取菜单
	void GetMenu();
	// 获取小程序
	void GetMinProgram();
	// 获取游戏库
	void GetGameLibI();
	// 扫描游戏更新环境，
	void ScanGameMenuEnv();
	// 保存将游戏库信息到文件
	void SaveGameLibInfoToFile();
	// 保存将游戏类型信息到文件
	void SaveGameTypeInfoToFile();
	// 过滤游戏库
	void FilterGameLib();
	// 获取游戏分类的序号
	string GetGameTypeLevel1Id(const string &strType); 
	// 是否存在指定的游戏序号
	bool IsExistGameId(int64 nGameId);
	// 从文件中解析出游戏库数据
	void GetGameLibFromDownloadFile(const string &strFileName, string &strRet);
	// 从云更新注册表中解析出游戏库数据
	void GetGameLibFromCloudUpdate();
	// 过滤云更新游戏库
	void FilterCloudUpdateGameLib();
	// 过滤顺网游戏库
	void FilterShunwangGameLib();
	// 取拼音
	string GetCharSpellCode(const string &strSource);
	string GetCharSpellCodeI(wchar_t wChar);
	// 加载第三方控件
	void LoadThreadPlug();
	// 获取进程名
	wstring GetProcessNameById(DWORD dwId);
private:
	CBarInfo();
	~CBarInfo();
	friend class CSingle<CBarInfo>;
};

