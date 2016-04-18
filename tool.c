#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>

/*
��������timeToString
���ܣ���time_t����ת��Ϊ�ַ������ͣ��ַ�����ʽΪ����-��-�� ʱ���֡�
������time_t t����Ҫת����ʱ�䣬char * pBuf��ת��֮����ַ���
����ֵ��void
*/
void timeToString(time_t t,char* pBuf) 
{
	struct tm *timeInfo;
	timeInfo = localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M",timeInfo);
}

/*
��������stringToTime
���ܣ�����ʽΪ����-��-�� ʱ���֡����ַ�������ת��Ϊtime_t����
������char * pTime����ʽΪ����-��-�� ʱ���֡����ַ���
����ֵ��time_t��ת�����ʱ������
*/
time_t stringToTime(char* pTime)
{
	//����ʱ�����
	struct tm tm1;
	time_t time1;

	//���ַ�����ʽ��ʱ�䱣�浽tm1������
	sscanf(pTime, "%d-%d-%d %d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min);
	
	//�޸�tm1��ʱ�������ֵ
	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	tm1.tm_sec = 0;
	tm1.tm_isdst = -1;

	//��struct tm���ͱ�����ֵת��Ϊtime_t���ͱ�����ֵ
	time1 = mktime(&tm1);

	return time1;
}

//���㿨�Ľ�ֹʱ��
time_t endTime(time_t tStart,time_t tEnd)
{
	struct tm* startTime;//����ʱ��
	struct tm* endTime;//��ֹʱ��
	startTime = localtime(&tStart);
	endTime = localtime(&tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	return tEnd = mktime(endTime);
}