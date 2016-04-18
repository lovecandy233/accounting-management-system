#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "global.h"
#include "tool.h"

Card prassCard(char* pBuf);

#define CARDCHARNUM 256

/*
函数名：saveCard
功能：将添加的卡信息，保存到指定路径下的文本文件中
参数：pCard-需要保存到文件中的卡信息的结构体指针，pPath-保存卡信息的路径
返回值：TRUE-成功，FALSE-失败
*/
int saveCard(const Card* pCard, const char* pPath)
{

	FILE* fp = NULL;

	char startTime[TIMELENGTH] = { 0 };//开始时间
	char endTime[TIMELENGTH] = { 0 };//截止时间
	char lastTime[TIMELENGTH] = { 0 };//最后使用时间

									  //打开文件
	fp = fopen(pPath, "a");
	if ((fp == NULL))
	{
		fp = fopen(pPath, "w");
		if ((fp == NULL))
		{
			return FALSE;
		}
	}
	//将时间转换成字符串
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLast, lastTime);

	//将数据写进文件
	//卡号##密码##状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##当前余额##删除标识
	fprintf(fp, "%s##%s##%d##%s##%s##%0.1f##%s##%d##%0.1f##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//关闭文件
	fclose(fp);
	return TRUE;
}


/*
函数名:readCard
功能：用于将文件数据写入结构体
参数： Card* pCard，结构体指针 const char* pPath 文件路径
返回值：int；0-FALSE，失败；1-TRUE，成功
*/
int readCard(Card* pCard, const char* pPath)
{
	//从文件中读取卡信息
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };//卡信息字符串数组
	int nIndex = 0;//卡信息数量


	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}

	//从文件中逐行读取
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);//清空数组
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				//调用prassCard(),解析读取的卡信息
				pCard[nIndex] = prassCard(aBuf);
				nIndex++;
			}
		}
	}
	fclose(fp);
	return TRUE;
}

/*
函数名:prassCard
功能：解析函数，用于解析每个用户账户的结构体
参数：pBuf-保存卡信息的字符串
返回值：Card-字符串转换成的卡信息结构体
*/
Card prassCard(char* pBuf)
{
	Card card;//保存解析出的卡信息

	const char *delims = "##";//字符串中的分隔符
	char flag[10][20] = { 0 };//保存分割后的字符
	int index = 0;//提示flag数组下标
	char *buf = NULL;
	char *str = NULL;

	buf = pBuf;//第一次调用strtok时，buf为解析字符串
	str = strtok(buf, delims);
	while ((str != NULL))
	{
		strcpy(flag[index], str);
		buf = NULL;//后面调用strtok函数时，第一个参数为NULL
		index++;
		str = strtok(buf, delims);
	}
	//卡号##密码##状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##余额##删除标识
	//分割后的内容保存到结构体中
	strcpy(card.aName, flag[0]);//卡号
	strcpy(card.aPwd, flag[1]);//密码
	card.nStatus = atoi(flag[2]);//状态
	card.tStart = stringToTime(flag[3]);//卡开时间
	card.tEnd = stringToTime(flag[4]);//截止时间
	card.fTotalUse = (float)atof(flag[5]); //累计金额
	card.tLast = stringToTime(flag[6]);//最后使用时间
	card.nUseCount = atoi(flag[7]);//使用次数
	card.fBalance = (float)atof(flag[8]); //余额
	card.nDel = atoi(flag[9]);//删除标记

	return card;
}


/*
涵数名: getCardCount
功能：获取文件中用户账户的数量
参数：const char * pPath，存储用户账户文件路径
返回值：int，文件中的用户账户书目
*/
int getCardCount(const char* pPath)
{
	FILE* fp = NULL;
	int nIndex = 0;
	char aBuf[CARDCHARNUM] = { 0 };

	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);//清空数组
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				nIndex++;
			}
		}
	}
	fclose(fp);
	//获取文件中信息的数量
	return nIndex;
}


/*
函数名：updataCard()
参数：pCard为卡信息结构体指针;pPath为保存卡信息的地址;nIndex为上机卡在卡信息链表中的索引号
返回值：件中的上机卡信息更新成功返回TRUE，失败返回FALSE
*/
int updataCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;//文件指针
	char aBuf[CARDCHARNUM] = { 0 };

	char startTime[TIMELENGTH] = { 0 };//开卡时间
	char endTime[TIMELENGTH] = { 0 };//截止时间
	char lastTime[TIMELENGTH] = { 0 };//最后使用时间

	int nLine = 0;//文件行数
	long IPosition = 0;//文件位置标记

					   //将时间转换为字符串
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLast, lastTime);

	//打开文件，如果失败，返回FALSE
	fp = fopen(CARDPATH, "r+");
	if (fp == NULL)
	{
		return FALSE;
	}

	//遍历文件,找到该条记录，进行更新
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//获取文件标志位置
			IPosition = ftell(fp);
			nLine++;
		}
	}
	//移到文件标识位置
	fseek(fp, IPosition, 0);
	//将数据写进文件
	//卡号##密码##状态##开卡时间##截止时间##累计金额##最后使用时间##使用次数##当前余额##删除标识
	fprintf(fp, "%s##%s##%d##%s##%s##%0.1f##%s##%d##%0.1f##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//关闭文件
	fclose(fp);
	return TRUE;
}