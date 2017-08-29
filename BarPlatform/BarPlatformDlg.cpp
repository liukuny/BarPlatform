
// BarPlatformDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BarPlatform.h"
#include "BarPlatformDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 设置打印参数
LONG SetPrintParam()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//MessageBox(L"SetPrintParam");
	LONG nRet = 1;
	CString strType;
	HKEY hKey = NULL;
	DWORD szType = REG_SZ;
	DWORD dwCount = 1024;
	CString strBuf;
	// HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\PageSetup
	//if (::RegOpenKey(HKEY_CURRENT_USER, L"\\Software\\Microsoft\\Internet Explorer\\PageSetup", &hKey) != ERROR_SUCCESS)
	LONG nState = ::RegOpenKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\PageSetup", &hKey);
	if (nState != ERROR_SUCCESS)
	{
		////打印背景  
		//HKEY_Key = "Print_Background";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "yes");
		////缩放
		//HKEY_Shrink = "Shrink_To_Fit";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Shrink, "no");
		////设置页眉（为空）
		//HKEY_Key = "header";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "");
		////设置页脚（为空） 
		//HKEY_Key = "footer";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "");
		////设置下页边距（0）
		//HKEY_Key = "margin_bottom";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0.25");
		////设置左页边距（0）
		//HKEY_Key = "margin_left";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0");
		////设置右页边距（0）
		//HKEY_Key = "margin_right";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0");
		////设置上页边距（0.25） 
		//HKEY_Key = "margin_top";
		//Wsh.RegWrite(HKEY_Root + HKEY_Path + HKEY_Key, "0.25");
		TCHAR szBuf[1024];
		memset(szBuf, 0, 1024);
		HKEY	hSubkey;
		DWORD	dwDisposition;
		if (RegCreateKeyEx(hKey, L"Print_Background", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubkey, &dwDisposition) != ERROR_SUCCESS)
		{
			return 2;
		}
		RegCloseKey(hSubkey);
	}
	else
	{
		strBuf = L"yes";
		if (RegSetValueEx(hKey, L"Print_Background", 0, szType, (unsigned char *)strBuf.GetBuffer(), (strBuf.GetLength() + 1) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			::RegCloseKey(hKey);
			return 3;
		}
	}
	return 0;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

#include "gfunction.h"
// CBarPlatformDlg 对话框
CBarPlatformDlg::CBarPlatformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarPlatformDlg::IDD, pParent)
{
	//SetPrintParam();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBarPlatformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBarPlatformDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CBarPlatformDlg 消息处理程序

BOOL CBarPlatformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	SetWindowText(_T(""));
#ifndef _DEBUG
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	CRect rt(-100, -100, -1, -1);
	MoveWindow(&rt);
#endif
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBarPlatformDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBarPlatformDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else 
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBarPlatformDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBarPlatformDlg::OnNcPaint()
{
#ifndef _DEBUG
	ShowWindow(SW_HIDE);
#else
	CDialogEx::OnNcPaint();
#endif
}
