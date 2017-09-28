#include "BarInfo.h"
#include "CommInterface.h"
#include "gfunction.h"
#include "MD5.h"
#include "CenterInterface.h"
#include "JsonCode.h"
#include "ConfigFile.h"
#include <TlHelp32.h>

#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

template <> CBarInfo *CSingle<CBarInfo>::m_pInstance = 0;
int g_nGameTypeId = 1;

wstring GetDesktopPath()
{
	wstring strRet;
	wchar_t szBuf[255] = {0};
	LPITEMIDLIST  ppidl = NULL;
	if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
	{
		BOOL flag = SHGetPathFromIDList(ppidl, szBuf);
		CoTaskMemFree(ppidl);
	}
	strRet = szBuf;
	return strRet;
}

bool CreateLinkFile(LPCTSTR szStartAppPath, LPCTSTR szAddCmdLine, LPCTSTR szDestLnkPath, LPCTSTR szIconPath)
{
	bool bRet = false;
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink *pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(szStartAppPath);
			wstring strTmp = szStartAppPath;
			int nStart = strTmp.find_last_of(_T("/\\"));
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			pShellLink->SetArguments(szAddCmdLine);
			if (szIconPath)
			{
				pShellLink->SetIconLocation(szIconPath, 0);
			}
			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save(szDestLnkPath, FALSE);
				if (SUCCEEDED(hr))
				{
					bRet = true;
				}
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return bRet;
}

BOOL TaskbarPin(LPTSTR lpPath, LPTSTR lpFileName, BOOL bIsPin = TRUE)
{
	CoInitialize(nullptr);
	BOOL bRet = FALSE;
	HMENU hmenu = NULL;
	LPSHELLFOLDER pdf = NULL;
	LPSHELLFOLDER psf = NULL;
	LPITEMIDLIST pidl = NULL;
	LPITEMIDLIST pitm = NULL;
	LPCONTEXTMENU pcm = NULL;

	if (SUCCEEDED(SHGetDesktopFolder(&pdf))
		&& SUCCEEDED(pdf->ParseDisplayName(NULL, NULL, lpPath, NULL, &pidl, NULL))
		&& SUCCEEDED(pdf->BindToObject(pidl, NULL, IID_IShellFolder, (void **)&psf))
		&& SUCCEEDED(psf->ParseDisplayName(NULL, NULL, lpFileName, NULL, &pitm, NULL))
		&& SUCCEEDED(psf->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST *)&pitm, IID_IContextMenu, NULL, (void **)&pcm))
		&& (hmenu = CreatePopupMenu()) != NULL
		&& SUCCEEDED(pcm->QueryContextMenu(hmenu, 0, 1, INT_MAX, CMF_NORMAL)))
	{
		CMINVOKECOMMANDINFO ici = { sizeof(CMINVOKECOMMANDINFO), 0 };
		ici.hwnd = NULL;
		ici.lpVerb = bIsPin ? "taskbarpin" : "taskbarunpin";
		pcm->InvokeCommand(&ici);
		bRet = TRUE;
	}

	if (hmenu)
		DestroyMenu(hmenu);
	if (pcm)
		pcm->Release();
	if (pitm)
		CoTaskMemFree(pitm);
	if (psf)
		psf->Release();
	if (pidl)
		CoTaskMemFree(pidl);
	if (pdf)
		pdf->Release();

	CoUninitialize();

	return bRet;
}

CBarInfo::CBarInfo()
{
#ifdef _DEBUG
	//DirectoryOrFileExists("D:\\ÎÒµÄÓÎÏ·");
	//GetGameLibFromCloudUpdate();

	//ScanGameMenuAndUpload();
	//TaskbarPin(_T("D:\\personalproject\\ÓÎÏ·²Ëµ¥\\code\\BarPlatform\\Debug\\GameMenu"), _T("ÓÎÏ·µ¼º½²Ëµ¥.lnk"));

	////string s = "linuxÊÇÒ»¸ö³öÉ«µÄ²Ù×÷ÏµÍ³";
	//wstring s = L"Ù»Å®ÓÄ»ê";
	////string sR = GetCharSpellCode(s);
	////wstring strDest = GetDesktopPath();
	////strDest += L"\\ÓÎÏ·²Ëµ¥.lnk";
	////CreateLinkFile(L"D:\\personalproject\\ÓÎÏ·²Ëµ¥\\code\\BarPlatform\\Debug\\GameMenu\\myoubox.exe", L"", strDest.c_str(), 0);
	//LPITEMIDLIST lpItemIdList;
	//SHGetSpecialFolderLocation(0, CSIDL_APPDATA, &lpItemIdList); 
	//char szBuf[MAX_PATH] = {0};
	//wchar_t wszBuf[MAX_PATH] = {0};
	//SHGetPathFromIDList(lpItemIdList, wszBuf);
	//if (DirectoryExistsW(wszBuf))
	//{
	//	s = wszBuf;
	//	s += L"\\Microsoft\\Internet Explorer\\Quick Launch\\ÓÎÏ·²Ëµ¥.lnk";
	//	CreateLinkFile(L"D:\\personalproject\\ÓÎÏ·²Ëµ¥\\code\\BarPlatform\\Debug\\GameMenu\\myoubox.exe", L"", s.c_str(), 0);
	//}


#endif
	string strType = CConfigFile::GetInstance()->GetGameMenuType();
	if (strType.empty())
	{
		// É¨ÃèÓÎÏ·¸üĞÂ»·¾³£¬
		ScanGameMenuEnv();
	}
	else
	{
		CLogClient::WriteLog("ÒÑÅäÖÃÓÎÏ·¿â£º %s", strType.c_str());
	}
}

CBarInfo::~CBarInfo()
{
	if (m_hConfigMapping)
	{
		CloseHandle(m_hConfigMapping);
	}
}

// »ñÈ¡ÓÎÏ·¿â
void CBarInfo::GetGameLib()
{
	if (!CBarInfo::GetInstance()->IsGetGameLib())
	{
		CLogClient::WriteLog("»ñÈ¡ÓÎÏ·¿â begin");
		// »ñÈ¡ÓÎÏ·¿â
		CBarInfo::GetInstance()->GetGameLibI();
		CLogClient::WriteLog("»ñÈ¡ÓÎÏ·¿â end");
	}
}

// »ñÈ¡ÅäÖÃ
void CBarInfo::GetConfig()
{
	static int64 nTime = 0;
	// {"data":[{"date":"2016-12-30","games":[{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":10000,"game_id":11001,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":998,"game_id":11002,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":995,"game_id":11003,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":30000,"game_id":11004,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":999,"game_id":11005,"launcher":""},{"game_name":"QQ»ªÏÄ","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":50000,"game_id":100644,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"},{"game_name":"QQÎ÷ÓÎ","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":20000,"game_id":100679,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"}]}],"ret_msg":"success","ret_code":0,"config":{"resources":[{"size":20984703,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/game_icons.zip","md5":"2cf0c87d74f333de1e6935c0ca82d207"}],"talking_url":"http://106.14.45.195:80/","update_packs":[{"ver":"20161101000000","size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/update1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}],"programs":[{"size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/smallprogram1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}]}}
	if (!CBarInfo::GetInstance()->IsGetConfig())
	{
		CLogClient::WriteLog("»ñÈ¡ÅäÖÃ begin");
		// »ñÈ¡²Ëµ¥
		CBarInfo::GetInstance()->GetMenu();
		// »ñÈ¡ÅäÖÃ
		CBarInfo::GetInstance()->GetConfigI();
		// »ñÈ¡²ßÂÔ
		CBarInfo::GetInstance()->GetConfigII();
		// »ñÈ¡Ğ¡³ÌĞò
		CBarInfo::GetInstance()->GetMinProgram();
		// ¼ÓÔØµÚÈı·½¿Ø¼ş
		LoadThreadPlug();
		nTime = ::GetTickCount64();
		CLogClient::WriteLog("»ñÈ¡ÅäÖÃ end");
	}
	else
	{
		// ÒÑ¾­»ñÈ¡ÅäÖÃ
		int nInterval = CConfigFile::GetInstance()->GetDLConfigInterval();
		if (nInterval > 0 && nTime)
		{
			if (::GetTickCount64() - nTime > nInterval * 60 * 1000)
			{
				CLogClient::WriteLog("ÖØĞÂ»ñÈ¡ÅäÖÃ begin :%d", nInterval);
				//// »ñÈ¡²Ëµ¥
				//CBarInfo::GetInstance()->GetMenu();
				// »ñÈ¡ÅäÖÃ
				CBarInfo::GetInstance()->GetConfigI();
				//// »ñÈ¡Ğ¡³ÌĞò
				//CBarInfo::GetInstance()->GetMinProgram();
				//// ¼ÓÔØµÚÈı·½¿Ø¼ş
				//LoadThreadPlug();
				nTime = ::GetTickCount64();
				CLogClient::WriteLog("ÖØĞÂ»ñÈ¡ÅäÖÃ end");
			}
		}
	}
}

// É¨ÃèÆäËûÓÎÏ·²Ëµ¥ÉÏ´«
void CBarInfo::ScanGameMenuAndUpload()
{
	if (CBarInfo::GetInstance()->IsGetConfig())
	{
		static HWND hOld = 0;
		HWND h = ::GetForegroundWindow();
		if (hOld == h)
		{
			return;
		}
		hOld = h;
		string strType;
		DWORD dwId = 0;
		GetWindowThreadProcessId(h, &dwId);
		if (dwId)
		{
			wstring strName = GetProcessNameById(dwId);
			transform(strName.begin(), strName.end(), strName.begin(), tolower);
			if (strName.compare(L"barclientview.exe") == 0)
			{
				// Ë³Íø
				strType = "10";
			}
			else if (strName.compare(L"knbmenu.exe") == 0)
			{
				// ÔÆ¸üĞÂ
				strType = "11";
			}
			else if (strName.compare(L"recreation.exe") == 0)
			{
				// Ò×ÓÎ
				strType = "12";
			}
			else if (strName.compare(L"hintclient.exe") == 0)
			{
				// ĞÅÓÓ
				strType = "13";
			}
			//else if (strName.compare(L"barclientview.exe") == 0)
			//{
			//	// ·½¸ñ×Ó
			//	strType = "14";
			//}
			//else if (strName.compare(L"barclientview.exe") == 0)
			//{
			//	// ÂÌ»¯´óÊ¦
			//	strType = "15";
			//}
		}

		if (!strType.empty())
		{
			// ÉÏ´«Æô¶¯Êı¾İ £¨10 - Ë³Íø£» 11 - ÔÆ¸üĞÂ£» 12 - Ò×ÓÎ£» 13 - ĞÅÓÓ£»14 - ·½¸ñ×Ó£»15 - ÂÌ»¯´óÊ¦£©
			string strUrl = GetTalkUrl();
			CCenterInterface::UploadInfo(strUrl, strType);
		}
	}
}

void CBarInfo::GetConfigI()
{
	string s = CCenterInterface::GetConfig();
	ConfigDataItem cfg;
	CJsonDecode::DeCodeConfigData(s, cfg); 
	// ÏÂÔØ×ÊÔ´
	for (list<ResourceDataItem>::iterator it = cfg.ltRD.begin(); it != cfg.ltRD.end(); it++)
	{
		if (it->url.find("http") == 0)
		{
			if (it->type == 1 && CConfigFile::GetInstance()->GetIsDLIconLib())
			{
				// ÏÂÔØ×ÊÔ´(Í¼±ê)
				string strUrl = it->url;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					string strDestPath = strFileName;
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
					{
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							it->iconpathname = strDestPath;
						}
					}
				}
			}
			else if (it->type == StrToInt(CConfigFile::GetInstance()->GetGameMenuType().c_str()))
			{
				// ÏÂÔØ×ÊÔ´(ÓÎÏ·¿â)
				string strUrl = it->url;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					string strDestPath = strFileName;
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
					{
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							strFileName.replace(strFileName.size() - 4, 4, ".json");
							string sTemp;
							GetGameLibFromDownloadFile(strFileName, sTemp);
							CJsonDecode::DeCodeGameData(sTemp, m_ltGame);
							// ¹ıÂËÓÎÏ·¿â
#ifndef _DEBUG
							FilterGameLib();
#endif
							// ±£´æ½«ÓÎÏ·¿âĞÅÏ¢µ½ÎÄ¼ş
							SaveGameLibInfoToFile();
							// ±£´æ½«ÓÎÏ·ÀàĞÍĞÅÏ¢µ½ÎÄ¼ş
							SaveGameTypeInfoToFile();

							it->iconpathname = strDestPath;
						}
					}
				}
			}
		}
	}
	CLogClient::WriteLog("×ÊÔ´ÏÂÔØÍê³É¡£");
	// ÏÂÔØÅÅĞò¿â
	for (list<SortDataItem>::iterator it = cfg.ltSD.begin(); it != cfg.ltSD.end(); it++)
	{
		for (list<SortGameItem>::iterator its = it->ltGame.begin(); its != it->ltGame.end(); its++)
		{
			string sId = IntToStr(its->game_id);
			its->iconpathname = CConfigFile::GetInstance()->GetGameResPath() + "\\";
			its->poppathname = its->iconpathname;
			its->iconpathname += sId;
			if (its->icon.find("http") == 0)
			{
				// ÏÂÔØÍ¼±ê
				its->iconpathname += its->icon.substr(its->icon.size() - 4);
				if (!CCenterInterface::DownloadFile(its->icon, its->iconpathname, its->iconmd5))
				{
					CLogError::WriteLog("ÏÂÔØÍ¼±êÊ§°Ü£º%d, Url£º%s", ::GetLastError(), its->icon.c_str());
				}
			}
			else
			{
				string sT = its->iconpathname + ".png";
				if (!FileExists(sT))
				{
					sT = its->iconpathname + ".ico";
				}
				its->iconpathname = sT;
			}
			if (its->popurl.find("http") == 0)
			{
				// ÏÂÔØ¸¡²ã¶¯Ì¬Í¼Æ¬
				its->poppathname += its->popurl.substr(its->popurl.rfind('/') + 1);
				if (!CCenterInterface::DownloadFile(its->popurl, its->poppathname, its->popmd5))
				{
					CLogError::WriteLog("ÏÂÔØ¸¡²ã¶¯Ì¬Í¼Æ¬Ê§°Ü£º%d, Url£º%s", ::GetLastError(), its->popurl.c_str());
				}
			}
			else
			{
				its->poppathname = "";
			}
			if (its->launcher.find("http") == 0)
			{
				// ÏÂÔØµÇÂ½³ÌĞò
				string strUrl = its->launcher;
				string strName;
				int nPos = strUrl.rfind('/');
				if (nPos != string::npos)
				{
					strName = strUrl.substr(nPos + 1);
				}
				if (!strName.empty())
				{
					string strFileName = CConfigFile::GetInstance()->GetGameResPath();
					strFileName += "\\";
					strFileName += strName;
					if (CCenterInterface::DownloadFile(strUrl, strFileName, its->launchermd5))
					{
						//CCommInterface::UnzipFileToPath("D:\\personalproject\\ÓÎÏ·²Ëµ¥\\code\\BarPlatform\\Debug\\GameMenu\\Game\\10002_HJS.zip", "D:\\personalproject\\ÓÎÏ·²Ëµ¥\\code\\BarPlatform\\Debug\\GameMenu\\Game\\10002_HJS");
						string strDestPath = strFileName.substr(0, strFileName.size() - 4);
						if (!DirectoryExists(strDestPath))
						{
							if (!CreateDir(strDestPath))
							{
								CLogError::WriteLog("´´½¨Ä¿Â¼Ê§°Ü£º%d, Ä¿Â¼£º%s", ::GetLastError(), strDestPath.c_str());
							}
						}
						if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
						{
							nPos = strDestPath.rfind('_');
							if (nPos != string::npos)
							{
								its->launchpathname = strDestPath;
								its->launchpathname += "\\";
								its->launchpathname += strDestPath.substr(nPos + 1);
								its->launchpathname += ".exe";
							}
							else
							{
								strDestPath += "\\launcher.exe";
								its->launchpathname = strDestPath;
							}
						}
					}
				}
			}
		}
	}
	CLogClient::WriteLog("ÅÅĞò¿âÏÂÔØÍê³É¡£");
	//// ÏÂÔØÉı¼¶°ü
	//for (list<UpLevelDataItem>::iterator it = cfg.ltUD.begin(); it != cfg.ltUD.end(); it++)
	//{
	//	string strOldVer = CConfigFile::GetInstance()->GetMenuVer();
	//	if (strOldVer.compare(it->ver) == 0)
	//	{
	//		break;
	//	}
	//	if (it->url.find("http") == 0)
	//	{
	//		// ÏÂÔØÉı¼¶°ü
	//		string strUrl = it->url;
	//		string strName;
	//		int nPos = strUrl.rfind('/');
	//		if (nPos != string::npos)
	//		{
	//			strName = strUrl.substr(nPos + 1);
	//		}
	//		if (!strName.empty())
	//		{
	//			string strFileName = CConfigFile::GetInstance()->GetGameMenuProgram();
	//			strFileName = strFileName.substr(0, strFileName.rfind('\\'));
	//			string strDestPath = strFileName;
	//			strFileName += "\\";
	//			strFileName += strName;
	//			if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
	//			{
	//				if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
	//				{
	//					it->pathname = strDestPath;
	//				}
	//			}
	//		}
	//	}
	//}
	//CLogClient::WriteLog("Éı¼¶°üÏÂÔØÍê³É¡£");
	//// ÏÂÔØĞ¡³ÌĞò
	//for (list<ProgramDataItem>::iterator it = cfg.ltPD.begin(); it != cfg.ltPD.end(); it++)
	//{
	//	if (it->url.find("http") == 0)
	//	{
	//		// ÏÂÔØĞ¡³ÌĞò
	//		string strUrl = it->url;
	//		string strName;
	//		int nPos = strUrl.rfind('/');
	//		if (nPos != string::npos)
	//		{
	//			strName = strUrl.substr(nPos + 1);
	//		}
	//		if (!strName.empty())
	//		{
	//			string strFileName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//			string strDestPath = strFileName;
	//			strFileName += "\\";
	//			strFileName += strName;
	//			CLogClient::WriteLog("Ğ¡³ÌĞòÏÂÔØÈ«Â·¾¶Ãû£º%s", strFileName.c_str());
	//			if (CCenterInterface::DownloadFile(strUrl, strFileName, it->md5))
	//			{
	//				if (CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
	//				{
	//					it->pathname = strDestPath;
	//				}
	//			}
	//			//strFileName += "\\ThreePlug.exe";
	//			//if (FileExists(strFileName.c_str()))
	//			//{
	//			//	HINSTANCE h = ShellExecuteA(NULL, "open", strFileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//			//	CLogClient::WriteLog("ÔËĞĞĞ¡³ÌĞò½á¹û£º%s,%d--%d", strFileName.c_str(), h, ::GetLastError());
	//			//}

	//		}
	//	}
	//}
	//CLogClient::WriteLog("Ğ¡³ÌĞòÏÂÔØÍê³É¡£");
	SaveInfoToMem(cfg);
	SetConfig(cfg);
	//string strPathName = CConfigFile::GetInstance()->GetGameMenuProgram();
	//CLogClient::WriteLog("ÔËĞĞ²Ëµ¥Ä¿Â¼£º%s", strPathName.c_str());
	//if (!strPathName.empty())
	//{
	//	if (FileExists(strPathName.c_str()))
	//	{
	//		HINSTANCE h = ShellExecuteA(NULL, "open", strPathName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//		CLogClient::WriteLog("ÔËĞĞ²Ëµ¥½á¹û£º%d--%d", h, ::GetLastError());
	//		wstring strDest = GetDesktopPath();
	//		wstring strDest2 = strDest;
	//		wstring strShutKey;
	//		strDest += L"\\";
	//		if (cfg.game_menu.empty())
	//		{
	//			//strDest += L"ÓÎÏ·²Ëµ¥";
	//			strShutKey = MultCharToWideCharA(CConfigFile::GetInstance()->GetLinkFileName());
	//			strDest += strShutKey;
	//		}
	//		else
	//		{
	//			strShutKey = MultCharToWideCharA(cfg.game_menu);
	//			strDest += strShutKey;
	//		}
	//		strShutKey += L".lnk";
	//		strDest += L".lnk";
	//		if (CreateLinkFile(MultCharToWideCharA(strPathName).c_str(), L"", strDest.c_str(), 0))
	//		{
	//			TaskbarPin((LPTSTR)strDest2.c_str(), (LPTSTR)strShutKey.c_str());
	//		}
	//	}
	//}

	//// ÔËĞĞĞ¡³ÌĞò
	//string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//strMinProgramName += "\\ThreePlug.exe";
	//CLogClient::WriteLog("Ğ¡³ÌĞòÈ«Ãû£º%s", strMinProgramName.c_str());
	//if (FileExists(strMinProgramName.c_str()))
	//{
	//	HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//	CLogClient::WriteLog("ÔËĞĞĞ¡³ÌĞò½á¹û£º%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	//}

}

// »ñÈ¡²ßÂÔ
void CBarInfo::GetConfigII()
{
	string strConfig = CCenterInterface::GetConfigII();
	CLogClient::WriteLog("»ñÈ¡²ßÂÔ·µ»ØĞÅÏ¢£º %s", strConfig.c_str());
	if (!strConfig.empty())
	{
		ConfigIIItem Item;
		string strFileName;
		string strParam;
		CJsonDecode::DeCodeConfigIIData(strConfig, Item);
		for (auto it : Item.lt)
		{
			strFileName = CConfigFile::GetInstance()->GetPolicyFilePath();
			strFileName += "\\";
			strFileName += it.url.substr(it.url.rfind('/') + 1);
			string strDestPath = strFileName.substr(0, strFileName.rfind('.'));
			if (!ForceDirectories(strDestPath))
			{
				CLogError::WriteLog("´´½¨²ßÂÔÎÄ¼ş¼ĞÊ§°Ü£º %s", strDestPath.c_str());
				continue;
			}
			if (!CCenterInterface::DownloadFile(it.url, strFileName, it.md5))
			{
				CLogError::WriteLog("ÏÂÔØ²ßÂÔÎÄ¼şÊ§°Ü£º %s - %s", it.url.c_str(), it.md5.c_str());
				continue;
			}

			if (!CCommInterface::UnzipFileToPath(strFileName.c_str(), strDestPath.c_str()))
			{
				CLogError::WriteLog("½âÑ¹²ßÂÔÎÄ¼şÊ§°Ü£º %s - %s", strFileName.c_str(), strDestPath.c_str());
				continue;
			}
			//CFileFind find;
			//CString strZipTarget = szTempPath;
			//BOOL bWorking = find.FindFile(strZipTarget + "\\*.*");
			//strDestPath += "\\*.Exe";
			WIN32_FIND_DATAA FindData;
			HANDLE hError = FindFirstFileA((strDestPath + "\\*.exe").c_str(), &FindData);
			if (hError != INVALID_HANDLE_VALUE)
			{
				// µ÷ÓÃ
				string strNumIcon = it.param.substr(it.param.rfind('=') + 1);
				//FormatString(strParam, "{\"iconNum\":%s}", strNumIcon.c_str());
				FormatString(strParam, "{\"iconNum\":%s, \"barID\":\"%s\"}", strNumIcon.c_str(), CConfigFile::GetInstance()->GetBarId().c_str());
				strParam = Base64::base64_encode(strParam);
				strFileName = strDestPath + "\\" + FindData.cFileName;
				CLogClient::WriteLog("ÔËĞĞ²ßÂÔÎÄ¼şÈ«Ãû£º%s - %s", strFileName.c_str(), strParam.c_str());
				if (FileExists(strFileName.c_str()))
				{
					HINSTANCE h = ShellExecuteA(NULL, "open", strFileName.c_str(), strParam.c_str(), NULL, SW_SHOWNORMAL);
					string strUrl = GetTalkUrl();
					string strAction;
					FormatString(strAction, "%d", atoi(strNumIcon.c_str()) + 200);
					CCenterInterface::UploadInfo(strUrl, strAction);
					CLogClient::WriteLog("ÔËĞĞ²ßÂÔÎÄ¼ş½á¹û£º%s,%d--%d", strFileName.c_str(), h, ::GetLastError());
				}
			}
		}
	}
}


// É±Ö¸¶¨½ø³Ì
bool KillProcessByProcessName(const wstring& csAppName, bool& bIsProcessExist)
{
	//
	bool bResult = true;
	bIsProcessExist = false;

	//
	PROCESSENTRY32 pe;
	memset(&pe, 0, sizeof(PROCESSENTRY32));

	//
	try
	{
		HANDLE hShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hShot)
		{
			DWORD dwErrorCode = ::GetLastError();
			//CLogError::WriteLog("É±½ø³Ì  Ê§°Ü£¬»ñÈ¡ÏµÍ³½ø³Ì¿ìÕÕÊ§°Ü(%d)", dwErrorCode);
			return false;
		}

		pe.dwSize = sizeof(PROCESSENTRY32);	//	Ò»¶¨ÒªÏÈÎªdwSize¸³Öµ
		if (::Process32First(hShot, &pe))
		{
			do
			{
				wstring csAppNameTmp = pe.szExeFile;
				wstring csAppNameP = csAppName;
				transform(csAppNameTmp.begin(), csAppNameTmp.end(), csAppNameTmp.begin(), ::tolower);
				transform(csAppNameP.begin(), csAppNameP.end(), csAppNameP.begin(), ::tolower);

				int nAt = csAppNameP.rfind(L'.');
				csAppNameP = csAppNameP.substr(0, nAt);
				int nNameLen = csAppNameP.size();
				//
				if (csAppNameTmp.substr(0, nNameLen).compare(csAppNameP) == 0)
				{
					//
					bIsProcessExist = TRUE;
					//
					HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
					if (!hProcess)
					{
						DWORD dwErrorCode = ::GetLastError();
						//CLogError::WriteLog("É±½ø³Ì  Ê§°Ü£¬´ò¿ªÖ¸¶¨½ø³ÌÊ§°Ü(%d)", dwErrorCode);
					}
					else
					{
						bResult = ::TerminateProcess(hProcess, 0);
						//CLogError::WriteLog("É±½ø³Ì  TerminateProcess end: %d", bResult);
						if (!bResult)
						{
							DWORD dwErrorCode = ::GetLastError();
						}
						::CloseHandle(hProcess);
					}
					break;
				}
			} while (::Process32Next(hShot, &pe));
		}
		else
		{
			DWORD dwErrorCode = ::GetLastError();
			//CLogError::WriteLog("É±½ø³Ì  Ê§°Ü£¬»ñÈ¡¿ìÕÕÖĞÊ×½ø³ÌÊ§°Ü(%d)", dwErrorCode);
		}
		//
		::CloseHandle(hShot);
	}
	catch (...)
	{
	}

	//
	return bResult;
}

// »ñÈ¡²Ëµ¥
void CBarInfo::GetMenu()
{
	string strUpdate = CCenterInterface::GetMenuUpdate();
	CLogClient::WriteLog("»ñÈ¡²Ëµ¥·µ»ØĞÅÏ¢£º %s", strUpdate.c_str());
	if (!strUpdate.empty())
	{
		UpLevelDataItem Item;
		CJsonDecode::DeCodeSelfUpdateData(strUpdate, Item);
		if (!Item.url.empty())
		{
			CLogClient::WriteLog("ÓĞĞÂ°æ±¾²Ëµ¥¡£");
			Item.pathname = CConfigFile::GetInstance()->GetGameMenuProgram();
			string strMenuName = Item.pathname.substr(Item.pathname.rfind('\\') + 1);
			Item.pathname = Item.pathname.substr(0, Item.pathname.rfind('\\'));
			string strPath = Item.pathname;
			Item.pathname += "\\";
			Item.pathname += Item.url.substr(Item.url.rfind('/') + 1);
			bool bIsProcessExist = false;
			if (KillProcessByProcessName(MultCharToWideCharA(strMenuName), bIsProcessExist))
			{
				bool b = CCenterInterface::DownloadFile(Item.url, Item.pathname, Item.md5);
				if (b)
				{
					if (CCommInterface::UnzipFileToPath(Item.pathname.c_str(), strPath.c_str()))
					{
						CLogClient::WriteLog("ĞÂ°æ±¾²Ëµ¥¸üĞÂ³É¹¦¡£");
					}
				}
			}
		}
	}
}
// »ñÈ¡Ğ¡³ÌĞò
void CBarInfo::GetMinProgram()
{
	string strUpdate = CCenterInterface::GetMinProgram();
	CLogClient::WriteLog("»ñÈ¡Ğ¡³ÌĞò·µ»ØĞÅÏ¢£º %s", strUpdate.c_str());
	if (!strUpdate.empty())
	{
		UpLevelDataItem Item;
		CJsonDecode::DeCodeSelfUpdateData(strUpdate, Item);
		if (!Item.url.empty())
		{
			CLogClient::WriteLog("ÓĞĞ¡³ÌĞò¡£");
			Item.pathname = CConfigFile::GetInstance()->GetMinProgramFilePath();
			Item.pathname += "\\";
			Item.pathname += Item.url.substr(Item.url.rfind('/') + 1);
			bool b = CCenterInterface::DownloadFile(Item.url, Item.pathname, Item.md5);
			if (b)
			{
				if (CCommInterface::UnzipFileToPath(Item.pathname.c_str(), CConfigFile::GetInstance()->GetMinProgramFilePath().c_str()))
				{
					CLogClient::WriteLog("»ñÈ¡Ğ¡³ÌĞòÏÂÔØ³É¹¦¡£");
				}
			}
		}
	}

	//// ÔËĞĞĞ¡³ÌĞò
	//string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	//strMinProgramName += "\\ThreePlug.exe";
	//CLogClient::WriteLog("Ğ¡³ÌĞòÈ«Ãû£º%s", strMinProgramName.c_str());
	//if (FileExists(strMinProgramName.c_str()))
	//{
	//	HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//	CLogClient::WriteLog("ÔËĞĞĞ¡³ÌĞò½á¹û£º%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	//}


}

// ±£´æÅäÖÃĞÅÏ¢µ½ÄÚ´æ
void CBarInfo::SaveInfoToMem(const ConfigDataItem &cfg)
{
	//	½«ÅäÖÃĞÅÏ¢·ÅÈëÄÚ´æÎÄ¼ş
	if (!m_hConfigMapping)
	{
		m_hConfigMapping = (HANDLE)::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, sizeof(CLIENT_CONFIG_INFO), L"BarPlatform Config Struct Map");
	}
	if (m_hConfigMapping)
	{
		//string strMoney = IntToStr(CUserOnlineInfo::GetInstance()->GetUserMoney());
		CLIENT_CONFIG_INFO *lpClientConfigInfo = (CLIENT_CONFIG_INFO*)::MapViewOfFile(m_hConfigMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		memset(lpClientConfigInfo, 0, sizeof(CLIENT_CONFIG_INFO));
		memcpy(lpClientConfigInfo->clientId, CConfigFile::GetInstance()->GetMac().c_str(), CConfigFile::GetInstance()->GetMac().size());
		memcpy(lpClientConfigInfo->localIp, CConfigFile::GetInstance()->GetLocalIp().c_str(), CConfigFile::GetInstance()->GetLocalIp().size());
		memcpy(lpClientConfigInfo->agencyId, CConfigFile::GetInstance()->GetAgencyId().c_str(), CConfigFile::GetInstance()->GetAgencyId().size());
		memcpy(lpClientConfigInfo->szUploadUrl, cfg.talking_url.c_str(), cfg.talking_url.size());
		memcpy(lpClientConfigInfo->szMenuName, cfg.game_menu.c_str(), cfg.game_menu.size());
		//string strUserId = CUserOnlineInfo::GetInstance()->GetUserId().c_str();
		//CMD5 md5;
		//strUserId = md5.MD5(strUserId.c_str());
		//strUserId += "m";
		//memcpy(lpClientConfigInfo->szUserId, CUserOnlineInfo::GetInstance()->GetUserId().c_str(), CUserOnlineInfo::GetInstance()->GetUserId().size());
		//memcpy(lpClientConfigInfo->szMoney, strMoney.c_str(), strMoney.size());
		::UnmapViewOfFile(lpClientConfigInfo);
	}
	// ±£´æ²ÎÊı
	string strPFileName = CConfigFile::GetInstance()->GetGameMenuProgram();
	strPFileName = strPFileName.substr(0, strPFileName.rfind('\\') + 1);
	strPFileName += "param.ini";
	CLogClient::WriteLog("±¾µØ²ÎÊıÎÄ¼ş£º%s", strPFileName.c_str());
	WritePrivateProfileStringA("platform", "clientid", CConfigFile::GetInstance()->GetMac().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "localip", CConfigFile::GetInstance()->GetLocalIp().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "agencyid", CConfigFile::GetInstance()->GetAgencyId().c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "uploadurl", cfg.talking_url.c_str(), strPFileName.c_str());
	WritePrivateProfileStringA("platform", "menuname", cfg.game_menu.c_str(), strPFileName.c_str());
	//ofstream ofP(strPFileName.c_str(), ios_base::out | ios_base::binary);
	//string strPInfo;
	//FormatString(strPInfo, "%lld|%s|%s|%s|%d|%d", its.game_id, its.game_name.c_str(), its.launchpathname.c_str(), its.iconpathname.c_str(), its.rank, its.flag);
	//ofP << strPInfo << "\r\n";
	//ofP.close();


	// ±£´æÅÅÎ»¿â
	for (auto it : cfg.ltSD)
	{
		if (it.ltGame.size() > 0)
		{
			string strFileName = CConfigFile::GetInstance()->GetGameSortFileName();
			ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
			for (auto its : it.ltGame)
			{
				if (!IsExistGameId(its.game_id))
				{
					CLogClient::WriteLog("±¾µØ²»´æÔÚ´ËÓÎÏ·£º%s", its.game_name.c_str());
				}
				string strInfo;
				FormatString(strInfo, "%lld|%s|%s|%s|%d|%d|%s", its.game_id, its.game_name.c_str(), 
					its.launchpathname.c_str(), its.iconpathname.c_str(), its.rank, its.flag, its.poppathname.c_str());
				of << strInfo << "\r\n";
			}
			of.close();
		}
	}
}

// »ñÈ¡ÓÎÏ·¿â
void CBarInfo::GetGameLibI()
{
	string s = CCenterInterface::GetGameLib();
	CJsonDecode::DeCodeGameData(s, m_ltGame);
	// ¹ıÂËÓÎÏ·¿â
	FilterGameLib();
	// ±£´æ½«ÓÎÏ·¿âĞÅÏ¢µ½ÎÄ¼ş
	SaveGameLibInfoToFile();
	// ±£´æ½«ÓÎÏ·ÀàĞÍĞÅÏ¢µ½ÎÄ¼ş
	SaveGameTypeInfoToFile();

}

// É¨ÃèÓÎÏ·¸üĞÂ»·¾³£¬
void CBarInfo::ScanGameMenuEnv()
{
	//CConfigFile::GetInstance()->SetGameMenuType("100");
	//return;
	CLogClient::WriteLog("É¨ÃèÓÎÏ·¸üĞÂ»·¾³");
	string strType;
	HKEY hKey = NULL;
	// ·½¸ñ×Ó
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\FgzPnp", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "500";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("·½¸ñ×Ó");
	}
	if (!strType.empty()){ return; }

	// Ë³ÍøÓÎÏ·´óÊ¦
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\iCafe8", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"BarId", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "100";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		CLogClient::WriteLog("Ë³ÍøÓÎÏ·´óÊ¦");
		::RegCloseKey(hKey);
	}
	if (!strType.empty()){ return; }

	// ÔÆ¸üĞÂ
	//if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\knbclient", KEY_READ | KEY_WOW64_64KEY, NULL, &hKey) == ERROR_SUCCESS)
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\knbclient", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "400";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		CLogClient::WriteLog("ÔÆ¸üĞÂ");
		::RegCloseKey(hKey);
	}
	//if (!strType.empty())
	//{ 
	//	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\GINFO", &hKey) == ERROR_SUCCESS)
	//	{
	//		DWORD szType = REG_SZ;
	//		for (int i = 0; 1; i++)
	//		{
	//			DWORD dwCount = 1024;
	//			DWORD dwKeyLen = 1024;
	//			char szKey[1024] = { 0 };
	//			char szBuf[1024] = { 0 };
	//			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
	//			{
	//				string s = szBuf;
	//				GameLibItem item;
	//				s = s.substr(0, s.find_first_of('|'));
	//				if (!s.empty())
	//				{
	//					m_mapNameIcon[s] = szKey;
	//				}
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		::RegCloseKey(hKey);
	//	}
	//	return;
	//}
	//// ÔÆ¸üĞÂ
	//if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}", &hKey) == ERROR_SUCCESS)
	//{
	//	DWORD szType = REG_SZ;
	//	DWORD dwCount = 1024;
	//	TCHAR szBuf[1024];
	//	memset(szBuf, 0, 1024);
	//	if (RegQueryValueEx(hKey, L"netbarname", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
	//	{
	//		strType = "400";
	//		CConfigFile::GetInstance()->SetGameMenuType(strType);
	//	}
	//	::RegCloseKey(hKey);
	//}
	if (!strType.empty()){ return; }
	// ĞÅÓÓ
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Hintsoft1\\XunShanPro", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"PowerSaving", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "200";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("ĞÅÓÓ");
	}
	if (!strType.empty()){ return; }

	// Ò×ÓÎ
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\YileyooClient", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"HadInstall", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "300";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("Ò×ÓÎ");
	}
	if (!strType.empty()){ return; }

	// ÂÌ»¯´óÊ¦
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\zhdisk", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_DWORD;
		DWORD dwCount = 1024;
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		if (RegQueryValueEx(hKey, L"Type", NULL, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
		{
			strType = "600";
			CConfigFile::GetInstance()->SetGameMenuType(strType);
		}
		::RegCloseKey(hKey);
		CLogClient::WriteLog("ÂÌ»¯´óÊ¦");
	}
	if (!strType.empty()){ return; }
}

// ±£´æ½«ÓÎÏ·¿âĞÅÏ¢µ½ÎÄ¼ş
void CBarInfo::SaveGameLibInfoToFile()
{
	if (m_ltGame.size() == 0){ return; }
	// ½«ÓÎÏ·¿âĞÅÏ¢·ÅÈëÎÄ¼ş
	string strFileName = CConfigFile::GetInstance()->GetGameLibFileName();
	ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
	for (auto it : m_ltGame)
	{
		string strInfo;
		if (it.game_icon.empty())
		{
			it.game_icon = CConfigFile::GetInstance()->GetGameResPath() + "\\";
			it.game_icon += IntToStr(it.center_game_id);
			it.game_icon += ".png";
		} 
		if (CConfigFile::GetInstance()->IsFGZ())
		{
			// ·½¸ñ×Ó
			if (!::FileExists(it.game_icon))
			{
				string sPath = it.launcher;
				sPath = sPath.substr(0, sPath.rfind('\\'));
				sPath += "\\game.png";
				it.game_icon = sPath;
			}
		}
		// ÓÎÏ·±àºÅ£¬ÖĞĞÄÓÎÏ·¿âµÄÓÎÏ·±àºÅ center_game_id | ÓÎÏ·Ãû³Æ£¬·½±ã²é¿´ | Æô¶¯³ÌĞò¾ø¶ÔÂ·¾¶ | ÓÎÏ·Í¼±ê¾ø¶ÔÂ·¾¶ | ÓÎÏ·µã»÷Á¿£¨ÅÅĞòÖµ£© | ÓÎÏ··ÖÀà | ÓÎÏ·Ãû³ÆÆ´Òô£¬·½±ã²éÕÒ | Ö÷³ÌÃû | ÓÎÏ·ÏÔÊ¾Ãû
		//FormatString(strInfo, "%lld|%s|%s|%s|%lld|%s|%s|%s", it.center_game_id, it.game_name.c_str(), it.launcher.c_str(), it.game_icon.c_str(), it.game_dp, it.category_1.c_str(), it.name_py.c_str(), it.process.c_str());
		FormatString(strInfo, "%lld|%s|%s|%s|%lld|%s|%s|%s|%s", it.center_game_id, it.game_name.c_str(), it.launcher.c_str(), it.game_icon.c_str(), it.game_dp, 
			it.category_1.c_str(), GetCharSpellCode(it.game_name).c_str(), it.process.c_str(), it.game_showname.c_str());
		of << strInfo << "\r\n";
	}
	of.close();
}

// ±£´æ½«ÓÎÏ·ÀàĞÍĞÅÏ¢µ½ÎÄ¼ş
void CBarInfo::SaveGameTypeInfoToFile()
{
	if (m_ltGame.size() == 0){ return; }
	// ½«ÓÎÏ·ÀàĞÍĞÅÏ¢·ÅÈëÎÄ¼ş
	string strFileName = CConfigFile::GetInstance()->GetGameTypeFileName();
	ofstream of(strFileName.c_str(), ios_base::out | ios_base::binary);
	string strAllType;
	string strInfo;
	for (auto it : m_ltGameType)
	{
		if (strAllType.empty())
		{
			strAllType = it.strId;
		}
		else
		{
			strAllType += ",";
			strAllType += it.strId;
		}
		// ÓÎÏ·ÀàĞÍ±àºÅ | ÓÎÏ·ÀàĞÍÃû³Æ | ÓÎÏ·¸¸ÀàĞÍ±àºÅ
		FormatString(strInfo, "%s|%s|%s", it.strId.c_str(), it.strName.c_str(), it.strParentId.c_str());
		of << strInfo << "\r\n";
	}
	// g_nGameTypeId
	FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "ËùÓĞÓÎÏ·", strAllType.c_str());
	of << strInfo << "\r\n";
	//FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "×îÈÈ", strAllType.c_str());
	//of << strInfo << "\r\n";
	//FormatString(strInfo, "%d|%s|%s", g_nGameTypeId++, "ºÃÆÀ", strAllType.c_str());
	//of << strInfo << "\r\n";
	of.close();
}

// ¹ıÂËÓÎÏ·¿â
void CBarInfo::FilterGameLib()
{
	CLogClient::WriteLog("¹ıÂËÓÎÏ·¿â");
	if (CConfigFile::GetInstance()->IsClundUpdate())
	{
		// ¹ıÂËÔÆ¸üĞÂÓÎÏ·¿â
		FilterCloudUpdateGameLib();
		return;
	}
	if (CConfigFile::GetInstance()->IsShunwang())
	{
		// ¹ıÂËË³ÍøÓÎÏ·¿â
		FilterShunwangGameLib();
		return;
	}
	char buf[100] = { 0 };
	list<string> ltHeader;
	DWORD len = GetLogicalDriveStringsA(100, buf);
	for (char *p = buf; *p; p += strnlen(p, 4) + 1)
	{
		string s = p;
		CLogClient::WriteLog("É¨Ãèµ½ÅÌ·û£º %s", s.c_str());
		ltHeader.push_back(s);
	}
	string strFileName;
	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		for (list<string>::iterator ith = ltHeader.begin(); ith != ltHeader.end(); ith++)
		{
			strFileName = *ith;
			strFileName += it->category_dir;
			strFileName += "\\";
			strFileName += it->game_name;
			//strFileName += "\\";
			string strLanuch = strFileName;
			strLanuch += "\\";
			if (!it->process.empty())
			{
				strFileName += "\\";
			}
			strFileName += it->process; 
			//if (FileExists(strFileName) || DirectoryExists(strFileName))
			if (DirectoryOrFileExists(strFileName))
			{
				it->launcher = strLanuch + it->launcher;
				it->process = strFileName;
				it->category_1 = GetGameTypeLevel1Id(it->category_dir);
				m_ltGame.push_back(*it);
				break;
			}
		}
	}
}

// ÊÇ·ñ´æÔÚÖ¸¶¨µÄÓÎÏ·ĞòºÅ
bool CBarInfo::IsExistGameId(int64 nGameId)
{
	for (auto it : m_ltGame)
	{
		if (it.center_game_id == nGameId)
		{
			return true;
		}
	}
	return false;
}

// »ñÈ¡ÓÎÏ··ÖÀàµÄĞòºÅ
string CBarInfo::GetGameTypeLevel1Id(const string &strType)
{
	string strRet;
	for (list<GameTypeItem>::iterator it = m_ltGameType.begin(); it != m_ltGameType.end(); it++)
	{
		if (it->strName.compare(strType) == 0)
		{
			strRet = it->strId;
			break;
		}
	}
	if (strRet.empty())
	{
		GameTypeItem item;
		item.strId = IntToStr(g_nGameTypeId++);
		item.strName = strType;
		item.strParentId = "0";
		m_ltGameType.push_back(item);
		strRet = item.strId;
	}
	return strRet;
}

// ´ÓÎÄ¼şÖĞ½âÎö³öÓÎÏ·¿âÊı¾İ
void CBarInfo::GetGameLibFromDownloadFile(const string &strFileName, string &strRet)
{
	//string s("Í¨ÓÃ¹¤¾ß");
	fstream f(strFileName.c_str(), ios::in);
	if (f.is_open())
	{
		const int nBufLen = 10 * 1024;
		f.seekg(0, std::ios::end);
		int nLen = f.tellg();
		f.seekg(0, std::ios::beg);
		char *pBuf = new char[nLen + 1];
		f.read(pBuf, nLen);
		pBuf[nLen] = 0;
		strRet = pBuf;
		strRet = Utf8ToLocal(strRet);
		delete[]pBuf;
		f.close();
	}
}

// ´ÓÔÆ¸üĞÂ×¢²á±íÖĞ½âÎö³öÓÎÏ·¿âÊı¾İ
void CBarInfo::GetGameLibFromCloudUpdate()
{
	CLogClient::WriteLog("´ÓÔÆ¸üĞÂ×¢²á±íÖĞ½âÎö³öÓÎÏ·¿âÊı¾İ");
	// ÔÆ¸üĞÂ
	m_mapDiskName.clear();
	string strName;
	HKEY hKey = NULL;
	// ¼ÓÔØÅÌ·û¶ÔÓ¦¹ØÏµ
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\VDINFO", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		for (int i = 0; 1; i++)
		{
			DWORD dwCount = 1024;
			DWORD dwKeyLen = 1024;
			char szKey[1024] = { 0 };
			char szBuf[1024] = { 0 };
			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
			{
				strName = szKey;
				string s = szBuf;
				if (!strName.empty() && !s.empty())
				{
					m_mapDiskName[strName] = s;
					CLogClient::WriteLog("É¨ÃèÔÆ¸üĞÂÅÌ·û£º %s--%s", strName.c_str(), m_mapDiskName[strName].c_str()); 
					s += ":";
					if (GetDriveTypeA(s.c_str()) == DRIVE_NO_ROOT_DIR)
					{
						CLogClient::WriteLog("É¨ÃèÔÆ¸üĞÂÅÌ·û£º %s²»´æÔÚ", s.c_str());
						m_bCloudSWitch = false;
					}
					else
					{
						CLogClient::WriteLog("É¨ÃèÔÆ¸üĞÂÅÌ·û£º %s´æÔÚ", s.c_str());
					}
				}
			}
			else
			{
				break;
			}
		}
		::RegCloseKey(hKey);
	}

	// ¼ÓÔØÓÎÏ·
	m_mapNamePath.clear();
	if (::RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\{3798BE84-4E13-4b81-B8CF-5063730FF905}\\GINFO", &hKey) == ERROR_SUCCESS)
	{
		DWORD szType = REG_SZ;
		for (int i = 0; 1; i++)
		{
			DWORD dwCount = 1024;
			DWORD dwKeyLen = 1024;
			char szKey[1024] = { 0 };
			char szBuf[1024] = { 0 };
			if (RegEnumValueA(hKey, i, szKey, &dwKeyLen, 0, &szType, (LPBYTE)szBuf, &dwCount) == ERROR_SUCCESS)
			{
				string s = szBuf;
				GameLibItem item;
				string sField = s.substr(0, s.find_first_of('|'));
				if (!sField.empty())
				{
					strName = sField;
				}
				s = s.substr(s.find_first_of('|') + 1);
				sField = s.substr(0, s.find_first_of('|') - 1);
				//sField += strName;
				if (!sField.empty())
				{
					string strDiskName = sField.substr(0, 1);
					if (m_mapDiskName.count(strDiskName) > 0)
					{
						sField[0] = m_mapDiskName[strDiskName][0];
						if (!DirectoryExists(sField))
						{
							for (auto it : m_mapDiskName)
							{
								if (DirectoryExists(sField))
								{
									break;
								}
								sField[0] = it.second[0];
							}
						}
					}
					else
					{
						//string sTempF = sField;
						for (auto it : m_mapDiskName)
						{
							if (!m_bCloudSWitch)
							{
								break;
							}
							if (DirectoryExists(sField))
							{
								break;
							}
							sField[0] = it.second[0];
						}
					}
					m_mapNamePath[strName] = sField;// +".exe";
					m_mapLocalId[strName] = string(szKey);
					//CLogClient::WriteLog("É¨ÃèÔÆ¸üĞÂÓÎÏ·£º %s--%s", strName.c_str(), m_mapNamePath[strName].c_str());
				}
			}
			else
			{
				break;
			}
		}
		::RegCloseKey(hKey);
	}
	CLogClient::WriteLog("´ÓÔÆ¸üĞÂ×¢²á±íÖĞ½âÎö³öÓÎÏ·¿âÊı¾İ: %d", m_mapNamePath.size());
}

// ¹ıÂËË³ÍøÓÎÏ·¿â
void CBarInfo::FilterShunwangGameLib()
{
	char buf[100] = { 0 };
	list<string> ltHeader;
	DWORD len = GetLogicalDriveStringsA(100, buf);
	for (char *p = buf; *p; p += strnlen(p, 4) + 1)
	{
		string s = p;
		string st = s;
		st += "ÎÒµÄÓÎÏ·";
		if (DirectoryOrFileExists(st))
		{
			st += "\\";
			CLogClient::WriteLog("É¨Ãèµ½ÎÄ¼ş¼Ğ£º %s", st.c_str());
			ltHeader.push_back(st);
		}
		CLogClient::WriteLog("É¨Ãèµ½ÅÌ·û£º %s", s.c_str());
		ltHeader.push_back(s);
	}
	string strFileName;
	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		for (list<string>::iterator ith = ltHeader.begin(); ith != ltHeader.end(); ith++)
		{
			strFileName = *ith;
			strFileName += it->category_dir;
			strFileName += "\\";
			strFileName += it->game_name;
			//strFileName += "\\";
			string strLanuch = strFileName;
			strLanuch += "\\";
			if (!it->process.empty())
			{
				strFileName += "\\";
			}
			strFileName += it->process;
			//if (FileExists(strFileName) || DirectoryExists(strFileName))
			if (DirectoryOrFileExists(strFileName))
			{
				it->launcher = strLanuch + it->launcher;
				it->process = strFileName;
				it->category_1 = GetGameTypeLevel1Id(it->category_dir);
				m_ltGame.push_back(*it);
				break;
			}
		}
	}
}

// ¹ıÂËÔÆ¸üĞÂÓÎÏ·¿â
void CBarInfo::FilterCloudUpdateGameLib()
{
	CLogClient::WriteLog("¹ıÂËÔÆ¸üĞÂÓÎÏ·¿â");
	// ´ÓÔÆ¸üĞÂ×¢²á±íÖĞ½âÎö³öÓÎÏ·¿âÊı¾İ
	GetGameLibFromCloudUpdate();
	string strFileName;
	string strIconName;
	string strType;
	string strLanuch;

	list<GameLibItem> ltTmp = m_ltGame;
	m_ltGame.clear();
	for (list<GameLibItem>::iterator it = ltTmp.begin(); it != ltTmp.end(); it++)
	{
		strFileName = it->game_name;
		//CLogClient::WriteLog("¼ì²âÔÆ¸üĞÂÓÎÏ·Ä¿Â¼£º %s, m_mapNamePath.count: %d", strFileName.c_str(), m_mapNamePath.size());
		if (m_mapNamePath.count(strFileName) > 0)
		{
			//CLogClient::WriteLog("¼ì²âÔÆ¸üĞÂÓÎÏ·Ä¿Â¼£º %s", m_mapNamePath[strFileName].c_str());
			if (DirectoryExists(m_mapNamePath[strFileName]) || !m_bCloudSWitch)
			{
				strLanuch = m_mapNamePath[strFileName] + "\\";
				it->launcher = strLanuch + it->launcher;
				strType = m_mapNamePath[strFileName].substr(3);
				strType = strType.substr(0, strType.find('\\'));
				it->process = strLanuch + it->process;
				it->category_1 = GetGameTypeLevel1Id(strType);

				it->game_icon = CConfigFile::GetInstance()->GetGameResPath() + "\\";
				it->game_icon += IntToStr(it->center_game_id);
				it->game_icon += ".png";
				if (!FileExists(it->game_icon))
				{
				    CLogClient::WriteLog("¼ì²âÔÆ¸üĞÂÓÎÏ·²»´æÔÚÍ¼±êÄ¿Â¼£º %s", it->game_icon.c_str());
					strIconName = strLanuch + m_mapLocalId[strFileName]+".ico";
					if (FileExists(strIconName))
					{
						it->game_icon = strIconName;
					}
				}
				m_ltGame.push_back(*it);
			}
			else
			{
				CLogClient::WriteLog("¼ì²âÔÆ¸üĞÂÓÎÏ·²»´æÔÚÓÎÏ·Ä¿Â¼£º %s", m_mapNamePath[strFileName].c_str());
			}
		}
		else
		{
			CLogClient::WriteLog("¼ì²âÔÆ¸üĞÂÓÎÏ·±¾µØ²»´æÔÚÓÎÏ·Ä¿Â¼£º %s", strFileName.c_str());
		}
	}
}

string CBarInfo::GetCharSpellCode(const string &strSource)
{
	string strRet;
	wstring sw = MultCharToWideCharA(strSource);
	for (int i = 0; i < sw.size(); i++)
	{
		strRet += GetCharSpellCodeI(sw[i]);
	}
	return strRet;
}

string GetFirstLetter(string strChar)
{

	string ls_second_eng = "CJWGNSPGCGNESYPBTYYZDXYKYGTDJNNJQMBSGZSCYJSYYQPGKBZGYCYWJKGKLJSWKPJQHYTWDDZLSGMRYPYWWCCKZNKYDGTTNGJEYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJZXDTTSQZYCDXXHGCKBPHFFSSWYBGMXLPBYLLLHLXSPZMYJHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZCDJZWQJBDZBXGZNZCPWHKXHQKMWFBPBYDTJZZKQHYLYGXFPTYJYYZPSZLFCHMQSHGMXXSXJJSDCSBBQBEFSJYHWWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPYCJNDQJWXQXFYYFJLEJBZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQTRBCJTHZTQFRXQ"
		"HXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSGLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWYWSBHTRXXGLHQHFNMNYKLDYXZPWLGGTMTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHZXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKOHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPCJOQLCDHJJYSPRCHNWJNLHLYYQYYWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMYDJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYDCFCXYHLXCHYZJQSFQAGMNYXPFRKSSB"
		"JLYXYSYGLNSCMHCWWMNZJJLXXHCHSYDSTTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLYSDCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQHZPQSQSCFYMMDMGBWHWLGSSLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZPMGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHBBGDMYCNQQWLQHJJZYWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSLXHTZKZJECXJCJNMFBYCSFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJXJYZGWKYP"
		"SGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJBPMLMSXLZJQQHZYJCZYDJWBMJKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHHJLJAXFGFJZSLCFDQZLCLGJDJCSNCLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTXYXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKYFCJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJS"
		"BBSGGHFJLWPMZJNLYYWDQSHZXTYYWHMCYHYWDBXBTLMSYYYFSXJCSDXXLHJHFSSXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMGYYDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGSQQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSPRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJJGYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTTSSXTBNPDKLEYCJNYCDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLGBDJLSLYGKDZPZXJYYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZB"
		"YTDKYXJYYCNRJMDJGKLCLJBCTBJDDBBLBLCZQRPXJCGLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRPHQQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYJDJJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZKHHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQJYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZM"
		"ALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNDSZFDEQFHBSAQTGLLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ";
	string ls_second_ch = "Ø¡Ø¢Ø£Ø¤Ø¥Ø¦Ø§Ø¨Ø©ØªØ«Ø¬Ø­Ø®Ø¯Ø°Ø±Ø²Ø³Ø´ØµØ¶Ø·Ø¸Ø¹ØºØ»Ø¼Ø½"
		"Ø¾Ø¿ØÀØÁØÂØÃØÄØÅØÆØÇØÈØÉØÊØËØÌØÍØÎØÏØĞØÑØÒØÓØÔØÕØÖØ×ØØØÙØÚØÛØÜØİØŞØßØàØáØâØãØäØåØæØçØèØéØêØëØìØíØîØïØğØñØòØóØôØõØöØ÷ØøØùØúØûØüØıØşÙ¡Ù¢Ù£Ù¤Ù¥Ù¦Ù§Ù¨Ù©ÙªÙ«Ù¬Ù­Ù®Ù¯Ù°Ù±Ù²Ù³Ù´ÙµÙ¶Ù·Ù¸Ù¹ÙºÙ»Ù¼Ù½Ù¾Ù¿ÙÀÙÁÙÂÙÃÙÄÙÅÙÆÙÇÙÈÙÉÙÊÙËÙÌÙÍÙÎÙÏÙĞÙÑÙÒÙÓÙÔÙÕÙÖÙ×ÙØÙÙÙÚÙÛÙÜÙİÙŞÙßÙàÙáÙâÙãÙäÙåÙæÙçÙèÙéÙêÙëÙìÙíÙîÙïÙğÙñÙòÙóÙôÙõÙöÙ÷ÙøÙùÙúÙûÙüÙıÙşÚ¡Ú¢Ú£Ú¤Ú¥Ú¦Ú§Ú¨Ú©ÚªÚ«Ú¬Ú­Ú®Ú¯Ú°Ú±Ú²Ú³Ú´ÚµÚ¶Ú·Ú¸Ú¹ÚºÚ»Ú¼Ú½Ú¾Ú¿ÚÀÚÁÚÂÚÃÚÄÚÅÚÆÚÇÚÈÚÉÚÊÚËÚÌÚÍÚÎÚÏÚĞÚÑÚÒÚÓÚÔÚÕÚÖÚ×ÚØÚÙÚÚÚÛÚÜÚİÚŞÚßÚàÚáÚâÚãÚäÚåÚæÚçÚè"
		"ÚéÚêÚëÚìÚíÚîÚïÚğÚñÚòÚóÚôÚõÚöÚ÷ÚøÚùÚúÚûÚüÚıÚşÛ¡Û¢Û£Û¤Û¥Û¦Û§Û¨Û©ÛªÛ«Û¬Û­Û®Û¯Û°Û±Û²Û³Û´ÛµÛ¶Û·Û¸Û¹ÛºÛ»Û¼Û½Û¾Û¿ÛÀÛÁÛÂÛÃÛÄÛÅÛÆÛÇÛÈÛÉÛÊÛËÛÌÛÍÛÎÛÏÛĞÛÑÛÒÛÓÛÔÛÕÛÖÛ×ÛØÛÙÛÚÛÛÛÜÛİÛŞÛßÛàÛáÛâÛãÛäÛåÛæÛçÛèÛéÛêÛëÛìÛíÛîÛïÛğÛñÛòÛóÛôÛõÛöÛ÷ÛøÛùÛúÛûÛüÛıÛşÜ¡Ü¢Ü£Ü¤Ü¥Ü¦Ü§Ü¨Ü©ÜªÜ«Ü¬Ü­Ü®Ü¯Ü°Ü±Ü²Ü³Ü´ÜµÜ¶Ü·Ü¸Ü¹ÜºÜ»Ü¼Ü½Ü¾Ü¿ÜÀÜÁÜÂÜÃÜÄÜÅÜÆÜÇÜÈÜÉÜÊÜËÜÌÜÍÜÎÜÏÜĞÜÑÜÒÜÓÜÔÜÕÜÖÜ×ÜØÜÙÜÚÜÛÜÜÜİÜŞÜßÜàÜáÜâÜãÜäÜåÜæÜçÜèÜéÜêÜëÜìÜíÜîÜïÜğÜñÜòÜóÜôÜõÜöÜ÷ÜøÜùÜúÜûÜüÜıÜşİ¡İ¢İ£İ¤İ¥İ¦İ§İ¨İ©İªİ«İ¬İ­İ®İ¯İ°İ±İ²İ³İ´İµİ¶"
		"İ·İ¸İ¹İºİ»İ¼İ½İ¾İ¿İÀİÁİÂİÃİÄİÅİÆİÇİÈİÉİÊİËİÌİÍİÎİÏİĞİÑİÒİÓİÔİÕİÖİ×İØİÙİÚİÛİÜİİİŞİßİàİáİâİãİäİåİæİçİèİéİêİëİìİíİîİïİğİñİòİóİôİõİöİ÷İøİùİúİûİüİıİşŞ¡Ş¢Ş£Ş¤Ş¥Ş¦Ş§Ş¨Ş©ŞªŞ«Ş¬Ş­Ş®Ş¯Ş°Ş±Ş²Ş³Ş´ŞµŞ¶Ş·Ş¸Ş¹ŞºŞ»Ş¼Ş½Ş¾Ş¿ŞÀŞÁŞÂŞÃŞÄŞÅŞÆŞÇŞÈŞÉŞÊŞËŞÌŞÍŞÎŞÏŞĞŞÑŞÒŞÓŞÔŞÕŞÖŞ×ŞØŞÙŞÚŞÛŞÜŞİŞŞŞßŞàŞáŞâŞãŞäŞåŞæŞçŞèŞéŞêŞëŞìŞíŞîŞïŞğŞñŞòŞóŞôŞõŞöŞ÷ŞøŞùŞúŞûŞüŞıŞşß¡ß¢ß£ß¤ß¥ß¦ß§ß¨ß©ßªß«ß¬ß­ß®ß¯ß°ß±ß²ß³ß´ßµß¶ß·ß¸ß¹ßºß»ß¼ß½ß¾ß¿ßÀßÁßÂßÃßÄßÅßÆßÇßÈßÉßÊßËßÌßÍßÎßÏßĞßÑßÒßÓßÔßÕßÖß×ßØßÙßÚßÛßÜßİßŞßßßàßáßâßã"
		"ßäßåßæßçßèßéßêßëßìßíßîßïßğßñßòßóßôßõßöß÷ßøßùßúßûßüßıßşà¡à¢à£à¤à¥à¦à§à¨à©àªà«à¬à­à®à¯à°à±à²à³à´àµà¶à·à¸à¹àºà»à¼à½à¾à¿àÀàÁàÂàÃàÄàÅàÆàÇàÈàÉàÊàËàÌàÍàÎàÏàĞàÑàÒàÓàÔàÕàÖà×àØàÙàÚàÛàÜàİàŞàßàààáàâàãàäàåàæàçàèàéàêàëàìàíàîàïàğàñàòàóàôàõàöà÷àøàùàúàûàüàıàşá¡á¢á£á¤á¥á¦á§á¨á©áªá«á¬á­á®á¯á°á±á²á³á´áµá¶á·á¸á¹áºá»á¼á½á¾á¿áÀáÁáÂáÃáÄáÅáÆáÇáÈáÉáÊáËáÌáÍáÎáÏáĞáÑáÒáÓáÔáÕáÖá×áØáÙáÚáÛáÜáİáŞáßáàáááâáãáäáåáæáçáèáéáêáëáìáíáîáïáğáñáòáóáôáõáöá÷áøáùáúáûáüáıáşâ¡â¢â£â¤â¥â¦â§â¨â©âªâ«â¬â­â®â¯â°â±â²â³â´âµ"
		"â¶â·â¸â¹âºâ»â¼â½â¾â¿âÀâÁâÂâÃâÄâÅâÆâÇâÈâÉâÊâËâÌâÍâÎâÏâĞâÑâÒâÓâÔâÕâÖâ×âØâÙâÚâÛâÜâİâŞâßâàâáâââãâäâåâæâçâèâéâêâëâìâíâîâïâğâñâòâóâôâõâöâ÷âøâùâúâûâüâıâşã¡ã¢ã£ã¤ã¥ã¦ã§ã¨ã©ãªã«ã¬ã­ã®ã¯ã°ã±ã²ã³ã´ãµã¶ã·ã¸ã¹ãºã»ã¼ã½ã¾ã¿ãÀãÁãÂãÃãÄãÅãÆãÇãÈãÉãÊãËãÌãÍãÎãÏãĞãÑãÒãÓãÔãÕãÖã×ãØãÙãÚãÛãÜãİãŞãßãàãáãâãããäãåãæãçãèãéãêãëãìãíãîãïãğãñãòãóãôãõãöã÷ãøãùãúãûãüãıãşä¡ä¢ä£ä¤ä¥ä¦ä§ä¨ä©äªä«ä¬ä­ä®ä¯ä°ä±ä²ä³ä´äµä¶ä·ä¸ä¹äºä»ä¼ä½ä¾ä¿äÀäÁäÂäÃäÄäÅäÆäÇäÈäÉäÊäËäÌäÍäÎäÏäĞäÑäÒäÓäÔäÕäÖä×äØäÙäÚäÛäÜäİäŞäßäàäáäâäãäääåäæäçäè"
		"äéäêäëäìäíäîäïäğäñäòäóäôäõäöä÷äøäùäúäûäüäıäşå¡å¢å£å¤å¥å¦å§å¨å©åªå«å¬å­å®å¯å°å±å²å³å´åµå¶å·å¸å¹åºå»å¼å½å¾å¿åÀåÁåÂåÃåÄåÅåÆåÇåÈåÉåÊåËåÌåÍåÎåÏåĞåÑåÒåÓåÔåÕåÖå×åØåÙåÚåÛåÜåİåŞåßåàåáåâåãåäåååæåçåèåéåêåëåìåíåîåïåğåñåòåóåôåõåöå÷åøåùåúåûåüåıåşæ¡æ¢æ£æ¤æ¥æ¦æ§æ¨æ©æªæ«æ¬æ­æ®æ¯æ°æ±æ²æ³æ´æµæ¶æ·æ¸æ¹æºæ»æ¼æ½æ¾æ¿æÀæÁæÂæÃæÄæÅæÆæÇæÈæÉæÊæËæÌæÍæÎæÏæĞæÑæÒæÓæÔæÕæÖæ×æØæÙæÚæÛæÜæİæŞæßæàæáæâæãæäæåæææçæèæéæêæëæìæíæîæïæğæñæòæóæôæõæöæ÷æøæùæúæûæüæıæşç¡ç¢ç£ç¤ç¥ç¦ç§ç¨ç©çªç«ç¬ç­ç®ç¯ç°ç±ç²ç³ç´çµç¶ç·ç¸ç¹çºç»ç¼ç½"
		"ç¾ç¿çÀçÁçÂçÃçÄçÅçÆçÇçÈçÉçÊçËçÌçÍçÎçÏçĞçÑçÒçÓçÔçÕçÖç×çØçÙçÚçÛçÜçİçŞçßçàçáçâçãçäçåçæçççèçéçêçëçìçíçîçïçğçñçòçóçôçõçöç÷çøçùçúçûçüçıçşè¡è¢è£è¤è¥è¦è§è¨è©èªè«è¬è­è®è¯è°è±è²è³è´èµè¶è·è¸è¹èºè»è¼è½è¾è¿èÀèÁèÂèÃèÄèÅèÆèÇèÈèÉèÊèËèÌèÍèÎèÏèĞèÑèÒèÓèÔèÕèÖè×èØèÙèÚèÛèÜèİèŞèßèàèáèâèãèäèåèæèçèèèéèêèëèìèíèîèïèğèñèòèóèôèõèöè÷èøèùèúèûèüèıèşé¡é¢é£é¤é¥é¦é§é¨é©éªé«é¬é­é®é¯é°é±é²é³é´éµé¶é·é¸é¹éºé»é¼é½é¾é¿éÀéÁéÂéÃéÄéÅéÆéÇéÈéÉéÊéËéÌéÍéÎéÏéĞéÑéÒéÓéÔéÕéÖé×éØéÙéÚéÛéÜéİéŞéßéàéáéâéãéäéåéæéçéèéééêéëéìéíéîéïéğéñéòéó"
		"éôéõéöé÷éøéùéúéûéüéıéşê¡ê¢ê£ê¤ê¥ê¦ê§ê¨ê©êªê«ê¬ê­ê®ê¯ê°ê±ê²ê³ê´êµê¶ê·ê¸ê¹êºê»ê¼ê½ê¾ê¿êÀêÁêÂêÃêÄêÅêÆêÇêÈêÉêÊêËêÌêÍêÎêÏêĞêÑêÒêÓêÔêÕêÖê×êØêÙêÚêÛêÜêİêŞêßêàêáêâêãêäêåêæêçêèêéêêêëêìêíêîêïêğêñêòêóêôêõêöê÷êøêùêúêûêüêıêşë¡ë¢ë£ë¤ë¥ë¦ë§ë¨ë©ëªë«ë¬ë­ë®ë¯ë°ë±ë²ë³ë´ëµë¶ë·ë¸ë¹ëºë»ë¼ë½ë¾ë¿ëÀëÁëÂëÃëÄëÅëÆëÇëÈëÉëÊëËëÌëÍëÎëÏëĞëÑëÒëÓëÔëÕëÖë×ëØëÙëÚëÛëÜëİëŞëßëàëáëâëãëäëåëæëçëèëéëêëëëìëíëîëïëğëñëòëóëôëõëöë÷ëøëùëúëûëüëıëşì¡ì¢ì£ì¤ì¥ì¦ì§ì¨ì©ìªì«ì¬ì­ì®ì¯ì°ì±ì²ì³ì´ìµì¶ì·ì¸ì¹ìºì»ì¼ì½ì¾ì¿ìÀìÁìÂìÃìÄìÅìÆìÇìÈìÉìÊìËìÌìÍ"
		"ìÎìÏìĞìÑìÒìÓìÔìÕìÖì×ìØìÙìÚìÛìÜìİìŞìßìàìáìâìãìäìåìæìçìèìéìêìëìììíìîìïìğìñìòìóìôìõìöì÷ìøìùìúìûìüìıìşí¡í¢í£í¤í¥í¦í§í¨í©íªí«í¬í­í®í¯í°í±í²í³í´íµí¶í·í¸í¹íºí»í¼í½í¾í¿íÀíÁíÂíÃíÄíÅíÆíÇíÈíÉíÊíËíÌíÍíÎíÏíĞíÑíÒíÓíÔíÕíÖí×íØíÙíÚíÛíÜíİíŞíßíàíáíâíãíäíåíæíçíèíéíêíëíìíííîíïíğíñíòíóíôíõíöí÷íøíùíúíûíüíıíşî¡î¢î£î¤î¥î¦î§î¨î©îªî«î¬î­î®î¯î°î±î²î³î´îµî¶î·î¸î¹îºî»î¼î½î¾î¿îÀîÁîÂîÃîÄîÅîÆîÇîÈîÉîÊîËîÌîÍîÎîÏîĞîÑîÒîÓîÔîÕîÖî×îØîÙîÚîÛîÜîİîŞîßîàîáîâîãîäîåîæîçîèîéîêîëîìîíîîîïîğîñîòîóîôîõîöî÷îøîùîúîûîüîıîşï¡ï¢ï£ï¤ï¥ï¦ï§ï¨ï©ïª"
		"ï«ï¬ï­ï®ï¯ï°ï±ï²ï³ï´ïµï¶ï·ï¸ï¹ïºï»ï¼ï½ï¾ï¿ïÀïÁïÂïÃïÄïÅïÆïÇïÈïÉïÊïËïÌïÍïÎïÏïĞïÑïÒïÓïÔïÕïÖï×ïØïÙïÚïÛïÜïİïŞïßïàïáïâïãïäïåïæïçïèïéïêïëïìïíïîïïïğïñïòïóïôïõïöï÷ïøïùïúïûïüïıïşğ¡ğ¢ğ£ğ¤ğ¥ğ¦ğ§ğ¨ğ©ğªğ«ğ¬ğ­ğ®ğ¯ğ°ğ±ğ²ğ³ğ´ğµğ¶ğ·ğ¸ğ¹ğºğ»ğ¼ğ½ğ¾ğ¿ğÀğÁğÂğÃğÄğÅğÆğÇğÈğÉğÊğËğÌğÍğÎğÏğĞğÑğÒğÓğÔğÕğÖğ×ğØğÙğÚğÛğÜğİğŞğßğàğáğâğãğäğåğæğçğèğéğêğëğìğíğîğïğğğñğòğóğôğõğöğ÷ğøğùğúğûğüğığşñ¡ñ¢ñ£ñ¤ñ¥ñ¦ñ§ñ¨ñ©ñªñ«ñ¬ñ­ñ®ñ¯ñ°ñ±ñ²ñ³ñ´ñµñ¶ñ·ñ¸ñ¹ñºñ»ñ¼ñ½ñ¾ñ¿ñÀñÁñÂñÃñÄñÅñÆñÇñÉñÊñËñÌñÍñÎñÏñĞñÑñÒñÓñÔñÕñÖñ×ñØñÙñÚñÛñÜñİñŞñßñàñâñãñäñåñæñç"
		"ñèñéñêñëñìñíñîñïñğñññòñóñôñõñöñ÷ñøñùñúñûñüñıñşò¡ò¢ò£ò¤ò¥ò¦ò§ò¨ò©òªò«ò¬ò­ò®ò¯ò°ò±ò²ò³ò´òµò¶ò·ò¸ò¹òºò»ò¼ò½ò¾ò¿òÀòÁòÂòÃòÄòÅòÆòÇòÈòÉòÊòËòÌòÍòÎòÏòĞòÑòÒòÓòÔòÕòÖò×òØòÙòÚòÛòÜòİòŞòßòàòáòâòãòäòåòæòçòèòéòêòëòìòíòîòïòğòñòòòóòôòõòöò÷òøòùòúòûòüòıòşó¡ó¢ó£ó¤ó¥ó¦ó§ó¨ó©óªó«ó¬ó­ó®ó¯ó°ó±ó²ó³ó´óµó¶ó·ó¸ó¹óºó»ó¼ó½ó¾ó¿óÀóÁóÂóÃóÄóÅóÆóÇóÈóÉóÊóËóÌóÍóÎóÏóĞóÑóÒóÓóÔóÕóÖó×óØóÙóÚóÛóÜóİóŞóßóàóáóâóãóäóåóæóçóèóéóêóëóìóíóîóïóğóñóòóóóôóõóöó÷óøóùóúóûóüóıóşô¡ô¢ô£ô¤ô¥ô¦ô§ô¨ô©ôªô«ô¬ô­ô®ô¯ô°ô±ô²ô³ô´ôµô¶ô·ô¸ô¹ôºô»ô¼ô½ô¾ô¿ôÀôÁôÂôÃôÄôÅôÆôÇ"
		"ôÈôÉôÊôËôÌôÍôÎôÏôĞôÑôÒôÓôÔôÕôÖô×ôØôÙôÚôÛôÜôİôŞôßôàôáôâôãôäôåôæôçôèôéôêôëôìôíôîôïôğôñôòôóôôôõôöô÷ôøôùôúôûôüôıôşõ¡õ¢õ£õ¤õ¥õ¦õ§õ¨õ©õªõ«õ¬õ­õ®õ¯õ°õ±õ²õ³õ´õµõ¶õ·õ¸õ¹õºõ»õ¼õ½õ¾õ¿õÀõÁõÂõÃõÄõÅõÆõÇõÈõÉõÊõËõÌõÍõÎõÏõĞõÑõÒõÓõÔõÕõÖõ×õØõÙõÚõÛõÜõİõŞõßõàõáõâõãõäõåõæõçõèõéõêõëõìõíõîõïõğõñõòõóõôõõõöõ÷õøõùõúõûõüõıõşö¡ö¢ö£ö¤ö¥ö¦ö§ö¨ö©öªö«ö¬ö­ö®ö¯ö°ö±ö²ö³ö´öµö¶ö·ö¸ö¹öºö»ö¼ö½ö¾ö¿öÀöÁöÂöÃöÄöÅöÆöÇöÈöÉöÊöËöÌöÍöÎöÏöĞöÑöÒöÓöÔöÕöÖö×öØöÙöÚöÛöÜöİöŞößöàöáöâöãöäöåöæöçöèöéöêöëöìöíöîöïöğöñöòöóöôöõööö÷öøöùöúöûöüöıöş÷¡÷¢÷£÷¤÷¥÷¦÷§"
		"÷¨÷©÷ª÷«÷¬÷­÷®÷¯÷°÷±÷²÷³÷´÷µ÷¶÷·÷¸÷¹÷º÷»÷¼÷½÷¾÷¿÷À÷Á÷Â÷Ã÷Ä÷Å÷Æ÷Ç÷È÷É÷Ê÷Ë÷Ì÷Í÷Î÷Ï÷Ğ÷Ñ÷Ò÷Ó÷Ô÷Õ÷Ö÷×÷Ø÷Ù÷Ú÷Û÷Ü÷İ÷Ş÷ß÷à÷á÷â÷ã÷ä÷å÷æ÷ç÷è÷é÷ê÷ë÷ì÷í÷î÷ï÷ğ÷ñ÷ò÷ó÷ô÷õ÷ö÷÷÷ø÷ù÷ú÷û÷ü÷ı÷ş";
	string return_py = "";
	if ((unsigned char)strChar[0] >= 215) //¶ş¼¶ºº×Ö
	{
		int nPos = ls_second_ch.find(strChar);
		if (string::npos != nPos)
		{
			return_py = ls_second_eng[nPos / 2];
		}
	}
	return return_py;
}

string CBarInfo::GetCharSpellCodeI(wchar_t wChar)
{
	string strRet;
	std::string sx = WideCharToMultiCharW(&wChar, 1);
	if (sx.size() == 1)
	{
		std::transform(sx.begin(), sx.end(), sx.begin(), toupper);
		return sx;
	}
	//sx = m2m_utf8(sx.c_str(), sx.size());
	unsigned char h = sx[0];
	unsigned char l = sx[1];
	unsigned short tmp = (sx[0] & 0xff) << 8;
	tmp |= (sx[1] & 0xff);
	//unsigned short tmp = htons(*((unsigned short *)sx.c_str()));
	if (tmp >= 45217 && tmp <= 45252) strRet = "A";
	else if (tmp >= 45253 && tmp <= 45760) strRet = "B";
	else if (tmp >= 45761 && tmp <= 46317) strRet = "C";
	else if (tmp >= 46318 && tmp <= 46825) strRet = "D";
	else if (tmp >= 46826 && tmp <= 47009) strRet = "E";
	else if (tmp >= 47010 && tmp <= 47296) strRet = "F";
	else if (tmp >= 47297 && tmp <= 47613) strRet = "G";
	else if (tmp >= 47614 && tmp <= 48118) strRet = "H";
	else if (tmp >= 48119 && tmp <= 49061) strRet = "J";
	else if (tmp >= 49062 && tmp <= 49323) strRet = "K";
	else if (tmp >= 49324 && tmp <= 49895) strRet = "L";
	else if (tmp >= 49896 && tmp <= 50370) strRet = "M";
	else if (tmp >= 50371 && tmp <= 50613) strRet = "N";
	else if (tmp >= 50614 && tmp <= 50621) strRet = "O";
	else if (tmp >= 50622 && tmp <= 50905) strRet = "P";
	else if (tmp >= 50906 && tmp <= 51386) strRet = "Q";
	else if (tmp >= 51387 && tmp <= 51445) strRet = "R";
	else if (tmp >= 51446 && tmp <= 52217) strRet = "S";
	else if (tmp >= 52218 && tmp <= 52697) strRet = "T";
	else if (tmp >= 52698 && tmp <= 52979) strRet = "W";
	//else if (tmp >= 52980 && tmp <= 53640) strRet = "X";
	else if (tmp >= 52980 && tmp <= 53688) strRet = "X";
	else if (tmp >= 53689 && tmp <= 54480) strRet = "Y";
	else if (tmp >= 54481 && tmp <= 55289) strRet = "Z";
	else strRet = GetFirstLetter(sx);
	return strRet;
}

// ¼ÓÔØµÚÈı·½¿Ø¼ş
void CBarInfo::LoadThreadPlug()
{
	// ÔËĞĞ²Ëµ¥
	ConfigDataItem cfg = GetLocalConfig();
	string strPathName = CConfigFile::GetInstance()->GetGameMenuProgram();
	CLogClient::WriteLog("ÔËĞĞ²Ëµ¥Ä¿Â¼£º%s", strPathName.c_str());
	if (!strPathName.empty() && CConfigFile::GetInstance()->GetIsCallMenu())
	{
		if (FileExists(strPathName.c_str()))
		{
			HINSTANCE h = ShellExecuteA(NULL, "open", strPathName.c_str(), NULL, NULL, SW_SHOWNORMAL);
			CLogClient::WriteLog("ÔËĞĞ²Ëµ¥½á¹û£º%d--%d", h, ::GetLastError());
			wstring strDest = GetDesktopPath();
			wstring strDest2 = strDest;
			wstring strShutKey;
			strDest += L"\\";
			if (cfg.game_menu.empty())
			{
				//strDest += L"ÓÎÏ·²Ëµ¥";
				strShutKey = MultCharToWideCharA(CConfigFile::GetInstance()->GetLinkFileName());
				strDest += strShutKey;
			}
			else
			{
				strShutKey = MultCharToWideCharA(cfg.game_menu);
				strDest += strShutKey;
			}
			strShutKey += L".lnk";
			strDest += L".lnk";
			if (CConfigFile::GetInstance()->GetIsCreateDeskIcon())
			{
				if (CreateLinkFile(MultCharToWideCharA(strPathName).c_str(), L"", strDest.c_str(), 0))
				{
					TaskbarPin((LPTSTR)strDest2.c_str(), (LPTSTR)strShutKey.c_str());
				}
			}
		}
	}

	// ÔËĞĞĞ¡³ÌĞò
	string strMinProgramName = CConfigFile::GetInstance()->GetMinProgramFilePath();
	strMinProgramName += "\\ThreePlug.exe";
	CLogClient::WriteLog("Ğ¡³ÌĞòÈ«Ãû£º%s", strMinProgramName.c_str());
	if (FileExists(strMinProgramName.c_str()))
	{
		HINSTANCE h = ShellExecuteA(NULL, "open", strMinProgramName.c_str(), NULL, NULL, SW_SHOWNORMAL);
		CLogClient::WriteLog("ÔËĞĞĞ¡³ÌĞò½á¹û£º%s,%d--%d", strMinProgramName.c_str(), h, ::GetLastError());
	}

	// ¼ÓÔØµÚÈı·½½Ó¿ÚÄ£¿é
	int nRet = CCommInterface::LoadThreePlug();
	if (nRet == 0)
	{
		CLogClient::WriteLog("¼ÓÔØ½Ó¿Ú³É¹¦");
	}
	else
	{
		CLogError::WriteLog("¼ÓÔØ½Ó¿ÚÊ§°Ü£º %d", nRet);
	}
}

wstring CBarInfo::GetProcessNameById(DWORD dwId)
{
	HANDLE h;
	PROCESSENTRY32 pe;
	wstring strRet;
	// »ñÈ¡½ø³Ì¿ìÕÕ
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE != h)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(h, &pe))
		{
			do{
				if (dwId == pe.th32ProcessID)
				{
					strRet = pe.szExeFile;
					break;
				}
			} while (Process32Next(h, &pe));
		}
		CloseHandle(h);
	}
	return strRet;
}

/*

º¯Êı¹¦ÄÜ£º¶ÔÖ¸¶¨ÎÄ¼şÔÚÖ¸¶¨µÄÄ¿Â¼ÏÂ´´½¨Æä¿ì½İ·½Ê½

º¯Êı²ÎÊı£º

lpszFileName    Ö¸¶¨ÎÄ¼ş£¬ÎªNULL±íÊ¾µ±Ç°½ø³ÌµÄEXEÎÄ¼ş¡£

lpszLnkFileDir  Ö¸¶¨Ä¿Â¼£¬²»ÄÜÎªNULL¡£

lpszLnkFileName ¿ì½İ·½Ê½Ãû³Æ£¬ÎªNULL±íÊ¾EXEÎÄ¼şÃû¡£

wHotkey         Îª0±íÊ¾²»ÉèÖÃ¿ì½İ¼ü

pszDescription  ±¸×¢

iShowCmd        ÔËĞĞ·½Ê½£¬Ä¬ÈÏÎª³£¹æ´°¿Ú

*/
//
//BOOL CreateFileShortcut(LPCWSTR lpszFileName, LPCWSTR lpszLnkFileDir, LPCWSTR lpszLnkFileName, LPCWSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL)
//{
//	if (lpszLnkFileDir == NULL)
//		return FALSE;
//	HRESULT hr;
//	IShellLink     *pLink;  //IShellLink¶ÔÏóÖ¸Õë
//	IPersistFile   *ppf; //IPersisFil¶ÔÏóÖ¸Õë
//	//´´½¨IShellLink¶ÔÏó
//	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
//	if (FAILED(hr))
//		return FALSE;
//	//´ÓIShellLink¶ÔÏóÖĞ»ñÈ¡IPersistFile½Ó¿Ú
//	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
//	if (FAILED(hr))
//	{
//		pLink->Release();
//		return FALSE;
//	}
//	//Ä¿±ê
//	pLink->SetPath(lpszFileName);
//	//¹¤×÷Ä¿Â¼
//	if (lpszWorkDir != NULL)
//		pLink->SetPath(lpszWorkDir);
//	//¿ì½İ¼ü
//	if (wHotkey != 0)
//		pLink->SetHotkey(wHotkey);
//	//±¸×¢
//	if (lpszDescription != NULL)
//		pLink->SetDescription(lpszDescription);
//	//ÏÔÊ¾·½Ê½
//	pLink->SetShowCmd(iShowCmd);
//	//¿ì½İ·½Ê½µÄÂ·¾¶ + Ãû³Æ
//	char szBuffer[MAX_PATH];
//	if (lpszLnkFileName != NULL) //Ö¸¶¨ÁË¿ì½İ·½Ê½µÄÃû³Æ
//		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, lpszLnkFileName);
//	else
//	{
//		//Ã»ÓĞÖ¸¶¨Ãû³Æ£¬¾Í´ÓÈ¡Ö¸¶¨ÎÄ¼şµÄÎÄ¼şÃû×÷Îª¿ì½İ·½Ê½Ãû³Æ¡£
//		char *pstr;
//		if (lpszFileName != NULL)
//			pstr = strrchr(lpszFileName, '\\');
//		else
//			pstr = strrchr(_pgmptr, '\\');
//		if (pstr == NULL)
//		{
//			ppf->Release();
//			pLink->Release();
//			return FALSE;
//		}
//		//×¢Òâºó×ºÃûÒª´Ó.exe¸ÄÎª.lnk
//		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, pstr);
//		int nLen = strlen(szBuffer);
//		szBuffer[nLen - 3] = 'l';
//		szBuffer[nLen - 2] = 'n';
//		szBuffer[nLen - 1] = 'k';
//	}
//	//±£´æ¿ì½İ·½Ê½µ½Ö¸¶¨Ä¿Â¼ÏÂ
//	WCHAR  wsz[MAX_PATH];  //¶¨ÒåUnicode×Ö·û´®
//	MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, wsz, MAX_PATH);
//	hr = ppf->Save(wsz, TRUE);
//	ppf->Release();
//	pLink->Release();
//	return SUCCEEDED(hr);
//}
//
//void CreateLinkFile(const string &strStartAppPath, const string &strDestLnkPath)
//{
//	//CFileLink  link;
//	//TChar     szLink[260] = { 0 };
//	//TChar     szPath[260] = { _T("Òª´´½¨¿ì½İ·½Ê½µÄÎÄ¼şÂ·¾¶") };
//
//	SHGetSpecialFolderPath(NULL, szLink, CSIDL_DESKTOP, FALSE);
//	_tcscat(szLink, _T("ExePath"));//"¿ì½İ·½Ê½ÔÚ×ÀÃæÏÔÊ¾µÄÃû×Ö"
//
//	_tcscat(szLink, _T(".lnk"));
//	link.CreateLink(szLink);
//	link.SetLinkInfo(szPath);
//}
//
