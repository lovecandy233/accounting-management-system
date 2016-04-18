#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>

/*
函数名：timeToString
功能：将time_t类型转换为字符串类型，字符串格式为“年-月-日 时：分”
参数：time_t t：需要转换的时间，char * pBuf：转换之后的字符串
返回值：void
*/
void timeToString(time_t t,char* pBuf) 
{
	struct tm *timeInfo;
	timeInfo = localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M",timeInfo);
}

/*
函数名：stringToTime
功能：将格式为“年-月-日 时：分”的字符串类型转换为time_t类型
参数：char * pTime：格式为“年-月-日 时：分”的字符串
返回值：time_t，转换后的时间类型
*/
time_t stringToTime(char* pTime)
{
	//定义时间变量
	struct tm tm1;
	time_t time1;

	//将字符串格式的时间保存到tm1变量中
	sscanf(pTime, "%d-%d-%d %d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min);
	
	//修改tm1中时间参数的值
	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	tm1.tm_sec = 0;
	tm1.tm_isdst = -1;

	//将struct tm类型变量的值转化为time_t类型变量的值
	time1 = mktime(&tm1);

	return time1;
}

//计算卡的截止时间
time_t endTime(time_t tStart,time_t tEnd)
{
	struct tm* startTime;//开卡时间
	struct tm* endTime;//截止时间
	startTime = localtime(&tStart);
	endTime = localtime(&tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	return tEnd = mktime(endTime);
}