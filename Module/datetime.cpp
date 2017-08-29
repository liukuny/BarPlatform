#include "datetime.h"
#include "gfunction.h"

//////////////////////////////////////////////////////////////////////////////
// class CDateTime

CDateTime::CDateTime()
{
    SetNow();
}

CDateTime::CDateTime(time_t tTime)
{
    SetValue(tTime);
}

CDateTime::CDateTime(const struct tm& stTime)
{
    m_stTime = stTime;
    m_tTime = mktime(&m_stTime);
}

CDateTime::CDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
    SetValue(nYear, nMonth, nDay, nHour, nMinute, nSecond);
}

void CDateTime::Modify(int nDeltaSecond)
{
    m_tTime += nDeltaSecond;
    localtime_s(&m_stTime, &m_tTime);
}

void CDateTime::SetValue(const time_t tTime)
{
    m_tTime = tTime;
	localtime_s(&m_stTime, &m_tTime);
}

void CDateTime::SetValue(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
    m_stTime.tm_year = nYear - 1900;
    m_stTime.tm_mon = nMonth - 1;
    m_stTime.tm_mday = nDay;
    m_stTime.tm_hour = nHour;
    m_stTime.tm_min = nMinute;
    m_stTime.tm_sec = nSecond;
    
    m_tTime = mktime(&m_stTime);
}

//----------------------------------------------------------------------------
// 描述: 将字符串转换成 CDateTime
// 注意: pDateTimeStr 的格式必须为 YYYY-MM-DD HH:MM:SS
//----------------------------------------------------------------------------
void CDateTime::SetValue(const char *pDateTimeStr)
{
    string strDateTime;
    int nYear, nMonth, nDay, nHour, nMinute, nSecond;

    strDateTime = pDateTimeStr;
    if (strDateTime.length() == 19)
    {
        nYear = StrToInt(strDateTime.substr(0, 4).c_str(), 0);
        nMonth = StrToInt(strDateTime.substr(5, 2).c_str(), 0);
        nDay = StrToInt(strDateTime.substr(8, 2).c_str(), 0);
        nHour = StrToInt(strDateTime.substr(11, 2).c_str(), 0);
        nMinute = StrToInt(strDateTime.substr(14, 2).c_str(), 0);
        nSecond = StrToInt(strDateTime.substr(17, 2).c_str(), 0);

        SetValue(nYear, nMonth, nDay, nHour, nMinute, nSecond);
    }
}

void CDateTime::SetNow()
{
    SetValue(time(NULL));
}

string CDateTime::ShortDateTime() const
{
    char sDateTime[15];
    sprintf_s(sDateTime, 15, "%04d%02d%02d%02d%02d%02d", Year(), Month(), Day(), 
        Hour(), Minute(), Second());
    return string(sDateTime);
}

string CDateTime::LongDateTime(char chSep1, char chSep2) const
{
    char sDateTime[20];
	sprintf_s(sDateTime, 20, "%04d%c%02d%c%02d %02d%c%02d%c%02d",
        Year(), chSep1, Month(), chSep1, Day(),
        Hour(), chSep2, Minute(), chSep2, Second());
    return string(sDateTime);
}
string CDateTime::LongDateTimeNoSep() const
{
	char sDateTime[20];
	sprintf_s(sDateTime, 20, "%04d%02d%02d%02d%02d%02d",
		Year(), Month(), Day(),	Hour(), Minute(), Second());
	return string(sDateTime);
}

string CDateTime::ShortDate() const
{
    char sDate[9];
    sprintf_s(sDate, 9, "%04d%02d%02d", Year(), Month(), Day());
    return string(sDate);
}

string CDateTime::LongDate(char chSep) const
{
    char sDate[11];
	sprintf_s(sDate, 11, "%04d%c%02d%c%02d", Year(), chSep, Month(), chSep, Day());
    return string(sDate);
}

string CDateTime::LongDateNoSep() const
{
	char sDate[11];
	sprintf_s(sDate, 11, "%04d%02d%02d", Year(), Month(), Day());
	return string(sDate);
}
// 是否是同一天
bool CDateTime::IsSameDate(time_t m_tTime)
{
	bool bRet(false);
	CDateTime dt(m_tTime);
	if (dt.Year() == Year() && dt.Month() == Month() && dt.Day() == Day())
	{
		bRet = true;
	}
	return bRet;
}

bool CDateTime::IsSameDate(time_t m_tTime, time_t m_tTime2)
{
	CDateTime dt(m_tTime);
	return dt.IsSameDate(m_tTime2);
}
