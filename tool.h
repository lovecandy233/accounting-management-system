#include <time.h>
#ifndef TOOL_H
#define TOOL_H
void timeToString(time_t t, char* pBuf);//ʱ������ת�ַ�������
time_t endTime(time_t tStart, time_t tEnd);
time_t stringToTime(char* pTime);//�ַ�������תʱ������
#endif // !TOOL_H

