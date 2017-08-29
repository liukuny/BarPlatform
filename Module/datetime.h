#ifndef _LKDATETIME_H_
#define _LKDATETIME_H_

#include <string>
#include <time.h>

using namespace std; 

//////////////////////////////////////////////////////////////////////////////
// class CDateTime - 日期时间类

class CDateTime
{
protected:
    time_t m_tTime;
    struct tm m_stTime;
    
public:
    CDateTime();
    CDateTime(time_t tTime);
    CDateTime(const struct tm& stTime);
    CDateTime(int nYear, int nMonth, int nDay, int nHour = 0, int nMinute = 0, int nSecond = 0);

    time_t GetValue() const { return m_tTime; }   
    const struct tm& GetStruct() const { return m_stTime; }
    void Modify(int nDeltaSecond);
    void SetValue(const time_t tTime);
    void SetValue(int nYear, int nMonth, int nDay, int nHour = 0, int nMinute = 0, int nSecond = 0);
    void SetValue(const char *pDateTimeStr);
    void SetNow();

    int Year() const { return m_stTime.tm_year + 1900; }
    int Month() const { return m_stTime.tm_mon + 1; }
    int Day() const { return m_stTime.tm_mday; }
    int Hour() const { return m_stTime.tm_hour; }
    int Minute() const { return m_stTime.tm_min; }
    int Second() const { return m_stTime.tm_sec; }
    int DayOfWeek() const { return m_stTime.tm_wday; }
    int DayOfYear() const { return m_stTime.tm_yday; }
	// 是否是同一天
	bool IsSameDate(time_t m_tTime);
	static bool IsSameDate(time_t m_tTime, time_t m_tTime2);

    string ShortDateTime() const;
	string LongDateTime(char chSep1 = '-', char chSep2 = ':') const;
	string LongDateTimeNoSep() const;
	string ShortDate() const;
    string LongDate(char chSep = '-') const;
	string LongDateNoSep() const;
};


#endif

