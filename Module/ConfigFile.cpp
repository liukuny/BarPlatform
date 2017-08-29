#include "ConfigFile.h"
#include "gfunction.h"

//#include "diskseri.h"
//#include <atlutil.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32")

// 服务器端配置文件名
#define CONFIG_FILENAME					"config.ini"
// 本地配置
#define CONFIG_LOCALAPP					"local"
// 获取同吧聊应用名称
#define CONFIG_LOCAL_IMAPPCLIENTNAME	"imapp"
// 获取比赛应用名称
#define CONFIG_LOCAL_MATCHAPPCLIENTNAME "matchapp"
// 获取控制台应用名称
#define CONFIG_LOCAL_CONTROLAPPCLIENTNAME "controlapp"
// 取出控制端版本
#define CONFIG_LOCAL_CONTROLVER			"1.0.0.1"
// 取出客户端版本
#define CONFIG_LOCAL_CLIENTVER			"1.0.0.1"
// tcp服务监听端口
#define CONFIG_LOCAL_TCPLISTENKEY		"tcpport"
// udp服务监听端口
#define CONFIG_LOCAL_UDPLISTENKEY		"udpport"
// 取出中心服务IP
#define CONFIG_LOCAL_UDPIP              "serverip"
// 向客户端发送访问地址
#define CONFIG_LOCAL_ClIENTIP     "serverClientIp"
// 取出中心服务http端口
#define CONFIG_LOCAL_HTTPPORT           "serverport"
// 图片文件路径
#define CONFIG_LOCAL_IMAGEPATH		    "imagepath"
// 本地UDP监听端口
#define CONFIG_LOCAL_LOCALUDPLISTENPORT "udplistenport"
// 机器编号
#define CONFIG_LOCAL_MACHINENAME		"machinename"
// 语音播报URL
#define CONFIG_LOCAL_YYBBURL            "yybburl"
// 网吧鱼服务IP地址
#define CONFIG_LOCAL_YSERVER            "yserver"

// BarId
#define CONFIG_LOCAL_NETBARID			"barid"
// 渠道Id
#define CONFIG_LOCAL_AGENCYID			"agencyid"
// 游戏菜单程序
#define CONFIG_LOCAL_GAMEMENU			"gamemenu"
// 游戏资源路径
#define CONFIG_LOCAL_GAMERESPATH		"respath"
// 游戏库名
#define CONFIG_LOCAL_GAMERLIB			"libname"
// 游戏类型名
#define CONFIG_LOCAL_GAMERTYPE			"typename"
// 游戏升级库名
#define CONFIG_LOCAL_GAMERSORT			"sortname"
// 小程序路径名
#define CONFIG_LOCAL_MINPROGRAM			"minprogram"
// 获取策略路径名
#define CONFIG_LOCAL_POLICYPATH			"policy"
// 平台升级包路径名
#define CONFIG_LOCAL_PLATFORMUPDATE		"platformupdate"
// 桌面快捷方式名称
#define CONFIG_LOCAL_LINKFILENAME		"linkfilename"

// 是否生成桌面图标
#define CONFIG_LOCAL_CREATEDESKICON		"createdeskicon"
// 是否调用菜单
#define CONFIG_LOCAL_CALLMENU			"callmenu"
// 是否调用P2P
#define CONFIG_LOCAL_CALLP2P			"callp2p"
// 是否下载图标库
#define CONFIG_LOCAL_DLICONLIB			"downloadiconlib"
// 开启配置下载扫描周期（单位分钟，0 - 不扫描）
#define CONFIG_LOCAL_DLCONFIGINTERVAL	"downloadconfiginterval"
// 游戏菜单类型
#define CONFIG_LOCAL_MENUTYPE			"gamemenutype"

// 服务端版本
const char *g_constServerVer = "1.0.0.0";

template <> CConfigFile *CSingle<CConfigFile>::m_pInstance = 0;

CConfigFile::CConfigFile()
{
	LoadConfig();
}


CConfigFile::~CConfigFile()
{
}

std::string GetIpFromUrl(const std::string &strUrl)
{
	std::string strRet;

	/////* 网址: http://www.google.cn/ */
	//wstring strwUrl = MultCharToWideCharA(strUrl.c_str(), strUrl.size());
	//CUrl url;
	//url.CrackUrl(strwUrl.c_str());
	//wstring strwHost = url.GetHostName();
	//string strHost = WideCharToMultiCharW(strwHost.c_str(), strwHost.size());

	//unsigned long nIp = inet_addr(strHost.c_str());
	//if (nIp == INADDR_NONE)
	//{
	//	WSADATA WSAData;
	//	///*******************************************************************
	//	//使用Socket的程序在使用Socket之前必须调用WSAStartup函数。
	//	//该函数的第一个参数指明程序请求使用的Socket版本，
	//	//其中高位字节指明副版本、低位字节指明主版本；
	//	//操作系统利用第二个参数返回请求的Socket的版本信息。
	//	//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，
	//	//然后绑定找到的Socket库到该应用程序中。
	//	//以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。
	//	//该函数执行成功后返回0。
	//	//*****************************************************************/
	//	int WSA_return = WSAStartup(0x0101, &WSAData);
	//	if (WSA_return == 0)
	//	{
	//		/* 即要解析的域名或主机名 */
	//		HOSTENT *host_entry = gethostbyname(strHost.c_str());
	//		int nErr = GetLastError();
	//		if (host_entry != 0)
	//		{
	//			strRet = host_entry->h_name;
	//			FormatString(strRet, "%d.%d.%d.%d", host_entry->h_addr_list[0][0] & 0x00ff, host_entry->h_addr_list[0][1] & 0x00ff, host_entry->h_addr_list[0][2] & 0x00ff, host_entry->h_addr_list[0][3] & 0x00ff);
	//			//(host_entry->h_addr_list[0][0] & 0x00ff),
	//			//(host_entry->h_addr_list[0][1] & 0x00ff),
	//			//(host_entry->h_addr_list[0][2] & 0x00ff),
	//			//(host_entry->h_addr_list[0][3] & 0x00ff));
	//		}
	//	}
	//	WSACleanup();
	//}
	//else
	//{
	//	strRet = strHost;
	//}
	return strRet;
}

// 转换为绝对路径
void CConfigFile::SwitchAbsPath(string &strPath)
{
	string s(strPath);
	strPath = CConfigFile::GetInstance()->CreateFullPathName(s);
}
// 获取游戏资源路径
string CConfigFile::GetGameResPath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strGameMenuResPath))
		{
			if (!ForceDirectories(m_strGameMenuResPath))
			{
				::MessageBox(0, L"获取游戏资源路径失败！", L"错误", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strGameMenuResPath;
}

// 获取小程序路径名
string CConfigFile::GetMinProgramFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strMinProgramFilePath))
		{
			if (!ForceDirectories(m_strMinProgramFilePath))
			{
				::MessageBox(0, L"获取小程序路径失败！", L"错误", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strMinProgramFilePath;
}

// 获取策略路径名
string CConfigFile::GetPolicyFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strPolicyFilePath))
		{
			if (!ForceDirectories(m_strPolicyFilePath))
			{
				::MessageBox(0, L"获取策略路径失败！", L"错误", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strPolicyFilePath;
}

// 获取平台升级包路径名
string CConfigFile::GetPlatformUpdateFilePath()
{
	static bool b = false;
	if (!b)
	{
		if (!DirectoryExists(m_strPlatformUpdateFilePath))
		{
			if (!ForceDirectories(m_strPlatformUpdateFilePath))
			{
				::MessageBox(0, L"获取平台升级包路径失败！", L"错误", 0);
				exit(1);
			}
			b = true;
		}
	}
	return m_strPlatformUpdateFilePath;
}

void CConfigFile::LoadConfig()
{
	char szTemp[MAX_PATH] = { 0 };
	wchar_t szTempW[MAX_PATH] = { 0 };
	string strFileName = CreateFullPathName(CONFIG_FILENAME);
	// 获取机器名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MACHINENAME, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strMachineName = szTemp;
#ifdef _DEBUG
	m_strMachineName = "035";
#endif
	if (m_strMachineName.empty())
	{
		// 机器名
		std::string strName;
		char szComputerName[51] = { 0 };
		DWORD dwSize = 50;
		::GetComputerNameA(szComputerName, &dwSize);
		m_strMachineName = szComputerName;
		size_t nPos = m_strMachineName.find("PC");
		if (string::npos != nPos)
		{
			m_strMachineName.erase(nPos, 2);
		}
		//m_strMachineName.erase('C');
	}

	// 获取本地UDP监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_LOCALUDPLISTENPORT, "7001", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_nLocalUdpPort = StrToInt(szTemp, 7001);
	// 取出客户端版本
	wstring strTemp = GetAppPathName();
	m_strClientVer = GetFileVersionEx(WideCharToMultiCharW(strTemp));
	//GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CLIENTVER, "1.0.0.1", szTemp, MAX_PATH - 1, strFileName.c_str());
	//m_strClientVer = szTemp;
	// 取出tcp服务监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_TCPLISTENKEY, "7070", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerTcpPort = szTemp;
	// 取出udp服务监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_UDPLISTENKEY, "5050", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerUdpPort = szTemp;
	// 取出中心服务IP
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_UDPIP, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerIp = szTemp;
	// 取出HTTP服务监听端口
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_HTTPPORT, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strServerHttpPort = szTemp;
	// 获取客户端需要的地址
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_ClIENTIP, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strClientIp = szTemp;
	// 图片文件路径
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_IMAGEPATH, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strImagePath = szTemp;
	if (m_strImagePath.empty())
	{
		m_strImagePath = "\\image";
		m_strImagePath = CreateFullPathName(m_strImagePath);
	}
	// 取MAC,Ip
	GetSysMacAndIp();
	// 取硬盘编号
	GetSysDiskNum();

	// BarId
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_NETBARID, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strBarId = szTemp;

	// 渠道Id
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_AGENCYID, "agencyid", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strAgencyId = szTemp;

	// 游戏菜单程序全名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMEMENU, "\\GameMenu\\myoubox.exe", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenu = szTemp;
	m_strGameMenu = CreateFullPathName(m_strGameMenu);
	// 菜单版本
	m_strMenuVer = GetFileVersionEx(m_strGameMenu);

	// 游戏资源路径
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERESPATH, "\\GameMenu\\Game", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenuResPath = szTemp;
	m_strGameMenuResPath = CreateFullPathName(m_strGameMenuResPath);

	// 游戏库名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERLIB, "\\GameMenu\\Game\\game.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameLibFileName = szTemp;
	m_strGameLibFileName = CreateFullPathName(m_strGameLibFileName);

	// 游戏类型名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERTYPE, "\\GameMenu\\Game\\type.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameTypeFileName = szTemp;
	m_strGameTypeFileName = CreateFullPathName(m_strGameTypeFileName);

	// 游戏升级库名	
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_GAMERSORT, "\\GameMenu\\Game\\Index.txt", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strSortFileName = szTemp;
	m_strSortFileName = CreateFullPathName(m_strSortFileName);

	// 获取小程序路径名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MINPROGRAM, "\\MinProgram", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strMinProgramFilePath = szTemp;
	m_strMinProgramFilePath = CreateFullPathName(m_strMinProgramFilePath);

	// 获取策略路径名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_POLICYPATH, "\\Policy", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strPolicyFilePath = szTemp;
	m_strPolicyFilePath = CreateFullPathName(m_strPolicyFilePath);

	// 平台升级包路径名
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_PLATFORMUPDATE, "\\PlatformUpdate", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strPlatformUpdateFilePath = szTemp;
	m_strPlatformUpdateFilePath = CreateFullPathName(m_strPlatformUpdateFilePath);

	// 桌面快捷方式名称
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_LINKFILENAME, "myoubox", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strLinkFileName = szTemp;

	// 是否生成桌面图标
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CREATEDESKICON, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCreateDeskIcon = StrToInt(szTemp, 1) == 0 ? false : true;
	
	// 是否调用菜单
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CALLMENU, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCallMenu = StrToInt(szTemp, 1) == 0 ? false : true;

	// 是否调用P2P
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_CALLP2P, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bCallP2P = StrToInt(szTemp, 1) == 0 ? false : true;

	// 是否下载图标库
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_DLICONLIB, "1", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_bDownloadIconlib = StrToInt(szTemp, 1) == 0 ? false : true;

	// 开启配置下载扫描周期（单位分钟，0 - 不扫描）
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_DLCONFIGINTERVAL, "0", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_nDownloadConfigInterval = StrToInt(szTemp, 0);

	// 游戏菜单类型
	GetPrivateProfileStringA(CONFIG_LOCALAPP, CONFIG_LOCAL_MENUTYPE, "", szTemp, MAX_PATH - 1, strFileName.c_str());
	m_strGameMenuType = szTemp;
}

// 生成全路径名
std::string CConfigFile::CreateFullPathName(std::string strFileName)
{
	std::string strPath = strFileName;
	if (strPath.find(':') != 0)
	{
		// 相对路径
		int nOff(0);
		if (strFileName[0] != '\\' && strFileName[0] != '/')
		{
			nOff = 1;
		}
		char szName[MAX_PATH] = { 0 };
		::GetModuleFileNameA(0, szName, MAX_PATH - 1);
		strPath = szName;
		int nIndex = strPath.rfind('\\');
		if (nIndex < 0) nIndex = strPath.rfind('/');
		strPath = strPath.substr(0, nIndex + nOff);
		strPath += strFileName;
	}
	if (strPath.size() > 0)
	{
		if (strPath[strPath.size() - 1] == '\\' || strPath[strPath.size() - 1] == '/')
		{
			strPath = strPath.substr(0, strPath.size() - 1);
		}
	}
	return strPath;
}

// 取MAC
void CConfigFile::GetSysMacAndIp()
{
	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	DWORD AdapterInfoSize;
	int nResult;
	AdapterInfoSize = 0;
	if ((nResult = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
	{
		if (nResult != ERROR_BUFFER_OVERFLOW)
		{
			return;
		}
	}
	if ((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
	{
		return;
	}
	if ((nResult = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	{
		GlobalFree(pAdapterInfo);
		return;
	}
	pAdapt = pAdapterInfo;
	while (pAdapt)
	{
		std::string strIpTemp = pAdapt->IpAddressList.IpAddress.String;
		if (strIpTemp.compare("0.0.0.0") == 0)
		{
			pAdapt = pAdapt->Next;
			continue;
		}
		if (pAdapt->Type == MIB_IF_TYPE_ETHERNET)
		{
			//处理虚拟网卡
			std::string strDescription;
			strDescription = pAdapt->Description;
			std::transform(strDescription.begin(), strDescription.end(), strDescription.begin(), tolower);
			if ((strDescription.find("virtual") != std::string::npos) && MIB_IF_TYPE_ETHERNET != pAdapt->Type)
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			int nFindIndex = strDescription.find("vmware");
			if (nFindIndex >= 0)
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			std::string strLocalIP = pAdapt->IpAddressList.IpAddress.String;
			if (strLocalIP == "0.0.0.0")
			{
				pAdapt = pAdapt->Next;
				continue;
			}
			//摄像头虚拟网卡MAC为0
			BYTE btBuffer[6] = { 0 };
			if (memcmp(pAdapt->Address, btBuffer, 6) == 0)
			{
				pAdapt = pAdapt->Next;
				continue;
			}

			int iAddressLength = min(pAdapt->AddressLength, 6);
			for (int j = 0; j < iAddressLength; j++)
			{
				btBuffer[j] = pAdapt->Address[j];
			}
			char szMac[13] = { 0 };
			sprintf_s(szMac, 13, "%02x%02x%02x%02x%02x%02x", pAdapt->Address[0], pAdapt->Address[1], pAdapt->Address[2], pAdapt->Address[3], pAdapt->Address[4], pAdapt->Address[5]);
			m_strMac = szMac;
			m_strIp = pAdapt->IpAddressList.IpAddress.String;
			break;
		}
		else if (71 == pAdapt->Type)
		{
			char szMac[13] = { 0 };
			sprintf_s(szMac, 13, "%02x%02x%02x%02x%02x%02x", pAdapt->Address[0], pAdapt->Address[1], pAdapt->Address[2], pAdapt->Address[3], pAdapt->Address[4], pAdapt->Address[5]);
			m_strMac = szMac;
			m_strIp = pAdapt->IpAddressList.IpAddress.String;
			break;
		}
		pAdapt = pAdapt->Next;
	}
	GlobalFree(pAdapterInfo);
}

// 取硬盘编号
void CConfigFile::GetSysDiskNum()
{
	//m_strDiskNum = diskseri::getHardDriveComputerID();
	//EraseChar(m_strDiskNum, ' ');
}


