#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "model.h"
#include "tool.h"

#include <stdio.h>

int saveMoney(const Money* pMoney,const char* pPath)
{
	//将充值信息保存
	FILE* fp = NULL;
	char operationTime[TIMELENGTH] = { 0 };//操作时间
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
	timeToString(pMoney->tTime, operationTime);

	//将数据写进文件
	//卡号##操作时间##操作类别##充值退费金额##删除标识
	fprintf(fp, "%s##%s##%d##%0.1f##%d\n",pMoney->aCardName,operationTime,pMoney->nStatus,pMoney->fMoney,pMoney->nDel);
	//关闭文件
	fclose(fp);
	return TRUE;
}