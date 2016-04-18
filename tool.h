#include <time.h>
#ifndef TOOL_H
#define TOOL_H
void timeToString(time_t t, char* pBuf);//时间类型转字符串类型
time_t endTime(time_t tStart, time_t tEnd);
time_t stringToTime(char* pTime);//字符串类型转时间类型
#endif // !TOOL_H

