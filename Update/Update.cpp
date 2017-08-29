// Update.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Update.h"


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <TlHelp32.h>
#include <algorithm>    // std::transform
//#include <Windows.h>    // ShellExecute
#include <Shellapi.h>  // ShellExecute
//#pragma comment(lib, "IPHLPAPI.lib")

using namespace std;

#define MAX_LOADSTRING 100


// 把char *转换成wstring
std::wstring MultCharToWideCharA(const char *pStr, int nLen)
{
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字符计算的
	int len = MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), NULL, 0);
	wchar_t *buffer = new wchar_t[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), buffer, len);
	buffer[len] = L'\0';//添加字符串结尾
	//删除缓冲区并返回值
	std::wstring return_value;
	return_value.append(buffer);
	delete[]buffer;
	return return_value;
}

//----------------------------------------------------------------------------
// 描  述: 获取可执行文件路径(带/)
// 参  数:
// 返回值: 
//   strName - 存放结果串
//----------------------------------------------------------------------------
string &GetNewPlatformPathName()
{
	static string strFilePathName;
	if (strFilePathName.empty())
	{
		string strName;
		const int BUFSIZE = 1024;
		char exec_name[BUFSIZE];
		::GetModuleFileNameA(0, exec_name, BUFSIZE - 1);
		strName = exec_name;
		strName = strName.substr(0, strName.rfind('\\'));
		strFilePathName = strName;
		strName += "config.ini";
		char szTemp[MAX_PATH] = { 0 };
		wchar_t szTempW[MAX_PATH] = { 0 };
		// 平台升级包路径名
		GetPrivateProfileStringA("local", "machinename", "\\PlatformUpdate", szTemp, MAX_PATH - 1, strName.c_str());
		strFilePathName += szTemp;
		strFilePathName += "\\BarPlatform.exe";
	}
	return strFilePathName;
}

//----------------------------------------------------------------------------
// 描  述: 获取当前平台文件全名
// 参  数:
// 返回值: 
//   strName - 存放结果串
//----------------------------------------------------------------------------
string &GetCurPlatformPathName()
{
	static string strName;
	if (strName.empty())
	{
		const int BUFSIZE = 1024;
		char exec_name[BUFSIZE];
		::GetModuleFileNameA(0, exec_name, BUFSIZE - 1);
		strName = exec_name;
		strName = strName.substr(0, strName.rfind('\\') + 1);
		strName += "BarPlatform.exe";
	}
	return strName;
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// 描述: 检查文件是否存在
//----------------------------------------------------------------------------
bool FileExists(const string& strDir)
{
	bool bResult;
	struct _stat st;
	memset(&st, 0, sizeof(st));

	if (_stat(strDir.c_str(), &st) == 0)
		bResult = ((st.st_mode & S_IFREG) == S_IFREG);
	else
		bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------
// 描述: 检查目录是否存在
//----------------------------------------------------------------------------
bool DirectoryExists(const string& strDir)
{
	bool bResult;
	struct _stat st;
	memset(&st, 0, sizeof(st));

	if (_stat(strDir.c_str(), &st) == 0)
		bResult = ((st.st_mode & S_IFDIR) == S_IFDIR);
	else
		bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------
// 描述: 创建目录
// 示例: CreateDir("/home/test");
//----------------------------------------------------------------------------
bool CreateDir(const string& strDir)
{
	//MakeSureDirectoryPathExists();
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	return !!CreateDirectoryA(strDir.c_str(), &attrib);
}

//----------------------------------------------------------------------------
// 描述: 强制创建目录
// 参数: 
//   strDir - 待创建的目录 (可以是多级目录)
// 返回:
//   true   - 成功
//   false  - 失败
// 示例:
//   ForceDirectories("/home/user1/data");
//----------------------------------------------------------------------------
bool ForceDirectories(string strDir)
{
	if (strDir.rfind('.') == strDir.size() - 4)
	{
		int n = strDir.rfind('/');
		if (n < 0) n = strDir.rfind('\\');
		strDir.resize(n);
	}
	if (DirectoryExists(strDir))
	{
		return true;
	}
	int nLen = strDir.rfind('/');
	if (nLen < 0) nLen = strDir.rfind('\\');

	if (strDir.empty() || nLen < 0)
	{
		return false;
	}
	string strSub(strDir);
	strSub.resize(nLen);

	if (DirectoryExists(strSub))
	{
		return CreateDir(strDir);
	}
	else
	{
		return ForceDirectories(strSub);
	}

	return true;
}

// 杀指定进程
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
			//CLogError::WriteLog("杀进程  失败，获取系统进程快照失败(%d)", dwErrorCode);
			return false;
		}

		pe.dwSize = sizeof(PROCESSENTRY32);	//	一定要先为dwSize赋值
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
						//CLogError::WriteLog("杀进程  失败，打开指定进程失败(%d)", dwErrorCode);
					}
					else
					{
						bResult = ::TerminateProcess(hProcess, 0);
						//CLogError::WriteLog("杀进程  TerminateProcess end: %d", bResult);
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
			//CLogError::WriteLog("杀进程  失败，获取快照中首进程失败(%d)", dwErrorCode);
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

//// 全局变量: 
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
//
//// 此代码模块中包含的函数的前向声明: 
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	string strNewPlatform = GetNewPlatformPathName();
	string strCurPlatform = GetCurPlatformPathName();
	if (::FileExists(strNewPlatform))
	{
		//CLogClient::WriteLog("更新包存在。");
		//CString strCurPlatformVer = CConfigFile::GetInstance()->GetCurPlatformVer().c_str();
		//CString strDownloadPlatformVer = GetFileVersionEx(CConfigFile::GetInstance()->GetDownloadPlatformPathName()).c_str();
		//if (strCurPlatformVer.CompareNoCase(strDownloadPlatformVer) != 0)
		//{
			// 杀死平台进程
			//CLogClient::WriteLog("本地不是最新版本的平台。");
			//CString strCurPlatformFileName = L"BarPlatForm.exe";
			bool bIsProcessExist = false;
			wstring strCurPlatformW = MultCharToWideCharA(strCurPlatform.c_str(), strCurPlatform.size());
			wstring strCurPlatformNameW = strCurPlatformW.substr(strCurPlatformW.rfind('\\') + 1);
			if (KillProcessByProcessName(strCurPlatformNameW, bIsProcessExist))
			{
				// 更新平台进程
cpfile:			if (::CopyFileA(strNewPlatform.c_str(), strCurPlatform.c_str(), FALSE))
				{
					//CLogClient::WriteLog("更新平台进程。");
					// 启动平台进程
					HINSTANCE h = ShellExecute(NULL, L"open", strCurPlatformW.c_str(), NULL, NULL, SW_HIDE);
					if ((int)h < 32)
					{
						//CLogError::WriteLogW(L"启动平台进程%s失败: %d", CConfigFile::GetInstance()->GetCurPlatformPathName().c_str(), (int)h);
					}
					else
					{
						//CLogClient::WriteLog("启动平台进程成功。");
						return TRUE;
					}
				}
				else
				{
					int nErr = ::GetLastError();
					//CLogError::WriteLogW(L"复制平台文件失败: %d, %s --> %s", nErr, CConfigFile::GetInstance()->GetDownloadPlatformPathName().c_str(), CConfigFile::GetInstance()->GetCurPlatformPathName().c_str());
					if (nErr == 32)
					{
						Sleep(100);
						goto cpfile;
					}
				}
			}
	}


	return TRUE;
	//MSG msg;
	//HACCEL hAccelTable;

	//// 初始化全局字符串
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_UPDATE, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	//// 执行应用程序初始化: 
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UPDATE));

	//// 主消息循环: 
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//return (int) msg.wParam;
}



////
////  函数:  MyRegisterClass()
////
////  目的:  注册窗口类。
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UPDATE));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UPDATE);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}
//
////
////   函数:  InitInstance(HINSTANCE, int)
////
////   目的:  保存实例句柄并创建主窗口
////
////   注释: 
////
////        在此函数中，我们在全局变量中保存实例句柄并
////        创建和显示主程序窗口。
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  目的:    处理主窗口的消息。
////
////  WM_COMMAND	- 处理应用程序菜单
////  WM_PAINT	- 绘制主窗口
////  WM_DESTROY	- 发送退出消息并返回
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// 分析菜单选择: 
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO:  在此添加任意绘图代码...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}
