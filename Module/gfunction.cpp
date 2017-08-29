#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fstream>
//#include <unistd.h>
//#include <net/if.h>     // struct ifreq
//#include <sys/ioctl.h>  // ioctl
//#include <netinet/in.h> // struct sockaddr_in

#include <windows.h>	// MultiByteToWideChar
#include "gfunction.h"

//----------------------------------------------------------------------------
// 描述: 判断一个字符串是不是一个整数#include<ctype.h>
//----------------------------------------------------------------------------
bool IsInt(const char *pStr)
{
    if (!pStr || !pStr[0]) return false;

    if (!isdigit(pStr[0]) && (pStr[0] != '-' || strlen(pStr) == 1)) return false;

    for (int i = 1; pStr[i]; i++)
        if (!isdigit(pStr[i])) return false;

    return true;
}

//----------------------------------------------------------------------------
// 描述: 字符串转换成整型(检测pSrc是否为NULL，是则返回nDefault)#include <stdlib.h>
//----------------------------------------------------------------------------
int StrToInt(const char *pStr, int nDefault)
{
    if (IsInt(pStr))
    {
        return atoi((char*)pStr);
    }
    else
        return nDefault;
}
int LStrToInt(const char *pStr, int nDefault)
{
	if (IsInt(pStr))
	{
		return atoi((char*)pStr);
	}
	else
		return nDefault;
}

//----------------------------------------------------------------------------
// 描述: 格式化字符串 (供FormatString函数调用)
//----------------------------------------------------------------------------
void FormatStringV(string& strResult, const char *pFormatString, va_list argList)
{
    //int nSize = 1024;
    int nSize = 2048;
    char *pBuffer = (char *)malloc(nSize);

    while (pBuffer)
    {
        int nChars;
		nChars = vsnprintf_s(pBuffer, nSize, _TRUNCATE, pFormatString, argList);
        if (nChars > -1 && nChars < nSize)
            break;
        if (nChars > -1)
            nSize = nChars + 1;
        else
            nSize *= 2;
        pBuffer = (char *)realloc(pBuffer, nSize);
    }

    if (pBuffer)
    {
        strResult = pBuffer;
        free(pBuffer);
    }
    else
        strResult = "";
}

void FormatStringWV(wstring& strResult, const wchar_t *pFormatString, va_list argList)
{
	int nSize = 2048;
	wchar_t *pBuffer = (wchar_t *)malloc(nSize * sizeof(wchar_t));
	int nChars(0);
	while (pBuffer)
	{
		nChars = _vsnwprintf_s(pBuffer, nSize, _TRUNCATE, pFormatString, argList);
		if (nChars > -1 && nChars < nSize)
		{
			break;
		}
		if (nChars > -1)
		{
			nSize = nChars + 1;
		}
		else
		{
			nSize *= 2;
		}
		pBuffer = (wchar_t *)realloc(pBuffer, nSize * sizeof(wchar_t));
	}

	if (pBuffer)
	{
		pBuffer[nChars] = 0;
		strResult = wstring(pBuffer, nChars);
		free(pBuffer);
	}
	else
		strResult = L"";
}

//----------------------------------------------------------------------------
// 描述: 格式化字符串
// 参数:
//   strResult      - 存放结果串
//   pFormatString  - 格式化字符串
//   ...            - 格式化参数
// 示例: 
//   FormatString(strResult, "ABC%sDEF%d", strTemp, nTemp);
//----------------------------------------------------------------------------
void FormatString(string& strResult, const char *pFormatString, ...)
{
    va_list argList;
    va_start(argList, pFormatString);
    FormatStringV(strResult, pFormatString, argList);
    va_end(argList);
}

void FormatStringW(wstring& strResult, const wchar_t *pFormatString, ...)
{
	va_list argList;
	va_start(argList, pFormatString);
	FormatStringWV(strResult, pFormatString, argList);
	va_end(argList);
}

//----------------------------------------------------------------------------
// 描述: 整型转换成字符串
//----------------------------------------------------------------------------
string IntToStr(int nValue)
{
    char sTemp[64];
    sprintf_s(sTemp, 64, "%d", nValue);
    return sTemp;
}

//----------------------------------------------------------------------------
// 描述: 字符串替换
// 参数:
//   pStr   - 源字符串
//   nSize  - 源字符串缓冲区大小
//   pFrom  - 要被替换的子字符串
//   pTo    - 要替换成的字符串
// 返回:
//   >=0    - 子字符串在源串中的位置(0-based)
//   -1     - 子字符串没找到
//----------------------------------------------------------------------------
int StringReplace(char *pStr, int nSize, char *pFrom, char *pTo)
{
    char *p;
    int nFromLen, nToLen, nMaxLen;

    p = strstr(pStr, pFrom);
    if (!p) return -1;

    nFromLen = strlen(pFrom);
    nToLen = strlen(pTo);
    nMaxLen = ((nFromLen > nToLen)? nFromLen : nToLen);

    memmove(p + nToLen, p + nFromLen, nSize - (p - pStr) - nMaxLen);
    memmove(p, pTo, nToLen);
    pStr[nSize - 1] = 0;

    return p - pStr;
}

//----------------------------------------------------------------------------
// 描述: 分割字符串
// 参数:
//   pStr       - string which will be splited
//   pSplitter  - pSplitter string
//   pStrList   - store split results
//   nListSize  - specify the nSize of pStrList
//   nCount     - split result string nCount
// 返回:
//   = (*nCount)
//----------------------------------------------------------------------------
int StringSplit(const char *pStr, const char *pSplitter, char *pStrList[], 
    int nListSize, int *nCount)
{
    const char *p;
    int nSpLen;

    if (!pStr || strlen(pStr) == 0 || strlen(pSplitter) == 0)
    {
        *nCount = 0;
        return *nCount;
    }

    nSpLen = strlen(pSplitter);
    *nCount = 1;

    while (*nCount - 1 < nListSize)
    {
        pStrList[*nCount - 1] = (char*)pStr;
        p = strstr(pStr, pSplitter);
        if (p)
        {
            pStr = p + nSpLen;
            (*nCount)++;
        }
        else 
        {
            break;
        }
    }

    if (*nCount > nListSize) *nCount = nListSize;
    return *nCount;
}

//----------------------------------------------------------------------------
// 描述: split string pTo integer numbers
// 参数:
//   pStr       - string that will be splited
//   pSplitter  - pSplitter string
//   pIntList   - store split results
//   nListSize  - specify the nSize of pIntList
//   nCount     - split result string nCount
// 返回:
//   = (*nCount)
// 备注: 
//   max nSize of integer list is 128.
//----------------------------------------------------------------------------
int StringSplitToInt(const char *pStr, const char *pSplitter, int pIntList[], 
    int nListSize, int *nCount)
{
    char *pStrList[128];
    int i;
    
    StringSplit(pStr, pSplitter, pStrList, 128, nCount);
    
    for (i = 0; i < *nCount && i < nListSize; i++)
        pIntList[i] = atoi(pStrList[i]);

    if (*nCount > nListSize) *nCount = nListSize;
    return *nCount;
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
bool DirectoryExistsW(const wstring& strDir)
{
	string s = WideCharToMultiCharW(strDir);
	return DirectoryExists(s);
}

bool DirectoryOrFileExists(const string& strDirOrFile)
{
	bool bResult = false;
	struct _stat st;
	memset(&st, 0, sizeof(st));

	if (_stat(strDirOrFile.c_str(), &st) == 0)
		bResult = ((st.st_mode & S_IFDIR) == S_IFDIR || (st.st_mode & S_IFREG) == S_IFREG);

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
// 描述: 从文件名中取出文件路径
// 参数:
//   strFileName - 包含路径的文件名
// 返回:
//   文件的路径
// 示例:
//   ExtractFilePath("/home/user1/data/test.c");
//   将返回: "/home/user1/data/"
//----------------------------------------------------------------------------
string ExtractFilePath(const string& strFileName)
{
    int nLen, nPos;
    string strResult;

    nLen = strFileName.length();
    nPos = -1;
    for (int i = nLen - 1; i >= 0; i--)
    {
        if (strFileName[i] == '/')
        {
            nPos = i;
            break;
        }
    }

    if (nPos != -1)
        strResult = strFileName.substr(0, nPos + 1);
    else
        strResult = "";

    return strResult;
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
		bool b = ForceDirectories(strSub);
		if (b)
		{
			b = CreateDir(strDir);
		}
		return b;
	}

	return true;
}


// 描述: 从路径中获取文件名
// 参数:
//   strFileName - 包含路径的文件名
// 返回:
//   文件的路径
// 示例:
//   ExtractFilePath("/home/user1/data/test.c");
//   将返回: "test.c"
string GetFileNameByPath(string strDir)
{
	int nLen, nPos;
	string strResult;

	nLen = strDir.length();
	nPos = -1;
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (strDir[i] == '/' || strDir[i] == '\\')
		{
			nPos = i;
			break;
		}
	}

	if (nPos != -1)
		strResult = strDir.substr(nPos + 1);
	else
		strResult = "";

	return strResult;
}

// 把文件读入内存
char *GetFileMemBuf(string strLocalPath, int &nLen)
{
	char *pRet(0);
	ifstream inf(strLocalPath.c_str(), ios_base::in | ios_base::binary);
	inf.seekg(0, inf.end);
	nLen = inf.tellg();
	if (nLen > 0)
	{
		pRet = new char[nLen];
		inf.seekg(0, inf.beg);
		inf.read(pRet, nLen);
	}
	return pRet;
}

//----------------------------------------------------------------------------
// 描述: 整形IP(主机字节顺序) -> 串型IPV4
//----------------------------------------------------------------------------
string IpToString(int nIp)
{
#pragma pack(1)
    union CIpUnion
    {
        int nValue;
        struct
        {
            unsigned char ch1;  //nValue的最低字节
            unsigned char ch2;
            unsigned char ch3;
            unsigned char ch4;
        } Bytes;
    } IpUnion;
#pragma pack()
    char strString[64];

    IpUnion.nValue = nIp;
    sprintf_s(strString, 64, "%u.%u.%u.%u", IpUnion.Bytes.ch4, IpUnion.Bytes.ch3, IpUnion.Bytes.ch2, IpUnion.Bytes.ch1);
    return strString;
}

//----------------------------------------------------------------------------
// 描述: 串型IP -> 整形IP(主机字节顺序)
//----------------------------------------------------------------------------
int StringToIp(const string& strString)
{
#pragma pack(1)
    union CIpUnion
    {
        int nValue;
        struct
        {
            unsigned char ch1;
            unsigned char ch2;
            unsigned char ch3;
            unsigned char ch4;
        } Bytes;
    } IpUnion;
#pragma pack()
    int nValues[4];
    int nCount;

    StringSplitToInt(strString.c_str(), ".", nValues, 4, &nCount);
    if (nCount == 4)
    {
        IpUnion.Bytes.ch1 = nValues[3];
        IpUnion.Bytes.ch2 = nValues[2];
        IpUnion.Bytes.ch3 = nValues[1];
        IpUnion.Bytes.ch4 = nValues[0];
        return IpUnion.nValue;
    }
    else
        return 0;
}

//----------------------------------------------------------------------------
// 描述: 获取本机IP
//----------------------------------------------------------------------------
string GetLocalIp()
{
	return "";
}


//----------------------------------------------------------------------------
// 描  述: 获取可执行文件路径(带/)
// 参  数:
// 返回值: 
//   strResult - 存放结果串
//----------------------------------------------------------------------------
string &GetModulePath()
{
    static string strName;
    if(strName.empty())
    {
        const int BUFSIZE = 1024;
        char exec_name[BUFSIZE];
		::GetModuleFileNameA(0, exec_name, BUFSIZE - 1);
        strName = exec_name;
        strName = strName.substr(0, strName.rfind('\\') + 1);
    }
    return strName;
} 

// 获取可执行文件名
wstring &GetAppPathName()
{
	static wstring strName;
	if (strName.empty())
	{
		const int BUFSIZE = 1024;
		wchar_t exec_name[BUFSIZE];
		::GetModuleFileName(0, exec_name, BUFSIZE - 1);
		strName = exec_name;
	}
	return strName;
}

wstring GetFileVersionEx(const wstring &strFileName)
{
	BYTE byteInfo[2048];
	int nSize = 2048;
	LPVOID lpData;
	UINT nDataSize;
	wstring	strFileVer;
	//wstring	strFileVerW;
	int nPos;

	if (GetFileVersionInfo(strFileName.c_str(), NULL, nSize, byteInfo) != 0)
	{
		VS_FIXEDFILEINFO *pInfo;
		if (VerQueryValue(byteInfo, L"\\", (void **)&pInfo, &nDataSize))
			//if (VerQueryValue(byteInfo, L"\\StringFileInfo\\080404b0\\FileVersion", &lpData, &nDataSize))
		{
			//pInfo->dwFileVersionMS = 1;
			FormatStringW(strFileVer, L"%d.%d.%d.%d", HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS), HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
			//strFileVer = (wchar_t *)lpData;
		}
	}
	return strFileVer;
}

string GetFileVersionEx(const string &strFileName)
{
	BYTE byteInfo[2048];
	int nSize = 2048;
	LPVOID lpData;
	UINT nDataSize;
	string	strFileVer;
	wstring	strFileVerW;
	int nPos;

	if (GetFileVersionInfoA(strFileName.c_str(), NULL, nSize, byteInfo) != 0)
	{
		VS_FIXEDFILEINFO *pInfo;
		if (VerQueryValue(byteInfo, L"\\", (void **)&pInfo, &nDataSize))
		{
			FormatString(strFileVer, "%d.%d.%d.%d", HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS), HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
		}
		//if (VerQueryValue(byteInfo, L"\\StringFileInfo\\080404b0\\FileVersion", &lpData, &nDataSize))
		//{
		//	strFileVerW = (wchar_t *)lpData;
		//	strFileVer = WideCharToMultiCharW(strFileVerW.c_str(), strFileVerW.size());
		//	//while ((nPos = strFileVer.find(',')) != string::npos)
		//	//{
		//	//	strFileVer.SetAt(nPos, '.');
		//	//	strFileVer.Delete(nPos + 1);
		//	//}
		//}
	}
	return strFileVer;
}

static int g_nCurPacketNumber = 0;
//----------------------------------------------------------------------------
// 描  述: 获取全局的报文序号
// 参  数:
// 返回值: 
//   nResult - 全局的报文序号
//----------------------------------------------------------------------------
int GetCurPacketNumber()
{
    return g_nCurPacketNumber;
}

//----------------------------------------------------------------------------
// 描  述: 生成全局的报文序号
// 参  数:
// 返回值: 
//   nResult - 全局的报文序号
//----------------------------------------------------------------------------
int AllocCurPacketNumber()
{
    return ++g_nCurPacketNumber;
}

//----------------------------------------------------------------------------
// 描  述: 删除指定字符
// 参  数:
//   s  - 待处理字符串
//   ch - 待删除字符
//----------------------------------------------------------------------------
void EraseChar(string& s, char ch)
{
	int index = s.find(ch);
	while (index != std::string::npos)
	{
		s.erase(index, 1);
		index = s.find(ch);
	}
}

// 
//----------------------------------------------------------------------------
// 描  述: 跟据指定字符分割字符串
// 参  数:
//   strSource  - 待处理字符串
//   strLeft	- 分隔符左边字符串(如果不存在分割符则等于strSource)
//   strRight	- 分隔符右边字符串(分割左边后剩下的)
//   ch			- 分隔符
//----------------------------------------------------------------------------
void ParseStringBySpecChar(const string &strSource, string &strLeft, string &strRight, char ch)
{
	int nIndex = strSource.find(ch);
	if (nIndex >= 0)
	{
		strLeft = strSource.substr(0, nIndex);
		strRight = strSource.substr(nIndex + 1, strSource.size() - nIndex - 1);
	}
	else
	{
		strLeft = strSource;
		strRight = "";
	}
}

// 
//----------------------------------------------------------------------------
// 描  述: 转换UTF8到本地编码
// 参  数:
//   strSource  - 待转换字符串
// 返回值：
//   string - 转换后字符串
//----------------------------------------------------------------------------
string Utf8ToLocal(const string &strSource)
{
	string strRet;
	int len = MultiByteToWideChar(CP_UTF8, 0, strSource.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	if (wstr)
	{
		memset(wstr, 0, sizeof(wchar_t)* (len + 1));
		MultiByteToWideChar(CP_UTF8, 0, strSource.c_str(), -1, wstr, len);

		int len2 = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char *szRet = new char[len2 + 1];
		if (szRet)
		{
			//memset(strRet, 0, len2);
			WideCharToMultiByte(CP_ACP, 0, wstr, len, szRet, len2, NULL, NULL);
			szRet[len2] = 0;
			strRet = szRet;
			delete szRet;
		}
		delete[] wstr;
	}
	return strRet;
}

// 把char *转换成wstring
std::wstring MultCharToWideCharA(const char *pStr, int nLen)
{
	if (nLen <= 0)
	{
		return L"";
	}
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
wstring MultCharToWideCharA(const string &strSource)
{ 
	return MultCharToWideCharA(strSource.c_str(), strSource.size()); 
}

// 把wchar_t *转换成string  
std::string WideCharToMultiCharW(const wchar_t *pStr, int nLen)
{
	if (nLen <= 0)
	{
		return "";
	}
	std::string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, pStr, static_cast<int>(nLen), NULL, 0, NULL, NULL);
	char *buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, pStr, static_cast<int>(nLen), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete[]buffer;
	return return_value;
}

// 把wchar_t *转换成string  
string WideCharToMultiCharW(const wstring &strSource)
{ 
	return WideCharToMultiCharW(strSource.c_str(), strSource.size());
}

// 宽字节编码转换成utf8多字节编码
std::string w2m_utf8(std::wstring &s)
{
	//WideCharToMultiByte(CP_ACP, NULL, s.c_str(), 1, pOut, sizeof(wchar_t), NULL, NULL);
	std::string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), NULL, 0, NULL, NULL);
	char *buffer = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete[]buffer;
	return return_value;
}

// gb2312编码转换成utf8多字节编码
std::string m2m_utf8(const char *pStr, int nLen)
{
	std::wstring ws = MultCharToWideCharA(pStr, nLen);
	return w2m_utf8(ws);
}

// 版本比较
// >0 strVer1 > strVer2
// =0 strVer1 > strVer2
// <0 strVer1 < strVer2
bool CompareVersion(const string &cstrVer1, const string &cstrVer2, int &nRet)
{
	string strVer1(cstrVer1);
	string strVer2(cstrVer2);
	int bRet = false;
	nRet = 0;
	char cSplit1 = '.';
	int nIndex1 = strVer1.find(cSplit1);
	if (nIndex1 < 0)
	{
		cSplit1 = ',';
		nIndex1 = strVer1.find(cSplit1);
	}

	char cSplit2 = '.';
	int nIndex2 = strVer1.find(cSplit2);
	if (nIndex2 < 0)
	{
		cSplit2 = ',';
		nIndex2 = strVer1.find(cSplit2);
	}

	int nVer1(0);
	int nVer2(0);
	string strTemp;
	for (int i = 0; i < 4; i++)
	{
		//if (nIndex1 < 0 || nIndex2 < 0)
		//{
		//	return false;
		//}
		strTemp = strVer1.substr(0, nIndex1);
		nVer1 = atoi(strTemp.c_str());
		strTemp = strVer2.substr(0, nIndex2);
		nVer2 = atoi(strTemp.c_str());
		if (nVer1 > nVer2)
		{
			nRet = 1;
			return true;
		}
		else if (nVer1 > nVer2)
		{
			nRet = -1;
			return true;
		}
		strVer1 = strVer1.substr(nIndex1 + 1, strVer1.size() - nIndex1 - 1);
		nIndex1 = strVer1.find(cSplit1);
		strVer2 = strVer2.substr(nIndex2 + 1, strVer2.size() - nIndex2 - 1);
		nIndex2 = strVer2.find(cSplit2);

	}
	return true;

}

//解析时分字符串
bool ParseHMString(const string &strHM, int &nHour, int &nMin)
{
	bool b(false);
	int nPos = strHM.find(":");
	if (nPos != string::npos)
	{
		nHour = StrToInt(strHM.substr(0, nPos).c_str());
		nMin = StrToInt(strHM.substr(nPos + 1).c_str());
		b = true;
	}
	return b;
}

// 解析身份证号码中的性别
// 返回值：（0 - 女； 1 - 男； 2 - 未知）
int GetSexById(const string &strIDNo)
{
	int nSex = 2;
	if (strIDNo.size() == 18)
	{
		string strSex = strIDNo.substr(16, 1);
		int n = StrToInt(strSex.c_str(), 10);
		if (n < 10)
		{
			if (n % 2 == 0)
			{
				// 女
				nSex = 0;
			}
			else
			{
				nSex = 1;
			}
		}

	}else if (strIDNo.size() == 15)
	{
		string strSex = strIDNo.substr(13, 1);
		int n = StrToInt(strSex.c_str(), 10);
		if (n < 10)
		{
			if (n % 2 == 0)
			{
				// 女
				nSex = 0;
			}
			else
			{
				nSex = 1;
			}
		}

	}
	return nSex;
}
