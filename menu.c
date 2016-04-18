#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "model.h"
#include "card_service.h"
#include "service.h"
#include "tool.h"
#include "global.h"

int getSize(const char* pInfo);

/*
函数名：outputMenu
功能：输出系统菜单
参数：void
返回值：void
*/ 
void outputMenu()
{
	printf("\n");
	printf("-------菜单---------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("0.退出\n");
	printf("请选择菜单项编号(0~8): ");
}
/*
函数名：add
1.添加卡
调用card_service.c中的addCard() 
*/
void add()
{
	Card card;
	char aName[32] = {'\0'};
	char aPwd[20] = { '\0' };
	int nIndex = 0;

	printf("---------添加卡---------\n");

	//提示并接受输入的卡信息
	printf("请输入卡号<长度为1~18>:"); 
	scanf("%s",aName);
	
	//判断卡号以及密码长度是否合法
	if (getSize(aName) > 18)
	{
		printf("卡号超过规定的长度\n");
		return;
	}

	//将输入的卡号保存到结构体中
	strcpy(card.aName, aName);
	//提示并接受输入密码
	printf("请输入密码<长度为1~8>:");
	scanf("%s", aPwd);

	if (getSize(aPwd) > 8) {
		printf("密码超过规定的长度\n");
		return;
	}

	//将输入的密码保存到结构体中
	strcpy(card.aPwd, aPwd);

	printf("请输入开卡金额<RMB>:"); 
	scanf("%f",&card.fBalance);
	card.fTotalUse = card.fBalance;//累计金额
	card.nDel = 0;//删除标志
	card.nUseCount = 0;//使用次数
	card.nStatus = 0;//卡状态
	card.tStart = card.tEnd = card.tLast = time(NULL);
	card.tEnd = endTime(card.tStart, card.tEnd);

	//如果已存在相同的用户名 则不添加
	//printf("\n用户名已被注册！添加卡失败！\n");

	if (addCardInfo(card))
	{
	//输出卡信息
	printf("\n------添加的卡信息如下-----\n");
	printf("卡号\t密码\t状态\t开卡金额\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	}
}
/*
函数名：getSize
功能：计算指针pinfo所指向字符串的字符数量
参数：pInfo需要求字符数的字符串
返回值：int字符串中字符的个数
*/
int getSize(const char* pInfo)
{
	int nSize = 0;
	while (*(pInfo+nSize) != '\0')
	{
		nSize++;
	}
	return nSize;
} 


/*
2.查询卡
提示并接受输入的卡号
调用card_service.c中的queryCard()
显示结果 
*/
void query()
{
	char aName[18] = { 0 };//保存输入的卡号
	char aLastTime[TIMELENGTH] = { 0 };
	Card* pCard = NULL;//保存查询到的卡信息
	//int nIndex = 0;
	int i = 0;

	printf("----------查询卡---------\n");

	//提示并接收输入的卡信息
	printf("请输入查询的卡号<长度为1~18>:");
	scanf("%s", aName);
	//pCard = queryCards(aName,&nIndex);
	pCard = queryCardInfo(aName);

	if (pCard == NULL)
	{
		printf("没有该卡的信息\n");
	}
	else
	{
		printf("\n卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
		//for (i = 0; i < nIndex; i++)
		//{	//显示
			timeToString(pCard[i].tLast, aLastTime);
			printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t\t%s\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
		//}	
		free(pCard);
		pCard = NULL;
	}
} 
 
void exitApp() 
{
	releaseList();
}

/*
3.上机
功能：提示并接受用户输入上机的卡号和密码，将查询到的上机卡信息输出显示
*/
void logon()
{
	char aName[18] = { 0 };//上机卡号
	char aPwd[8] = { 0 };//上机卡密码
	char lastTime[TIMELENGTH] = { 0 };//最后使用时间
	int nResult = 0;//保存上机结果
	LogonInfo* pInfo = NULL;

	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

	//提示用户输入卡号和密码

	printf("----------上机卡---------\n");
	printf("请输入上机的卡号<长度为1~18>:");
	scanf("%s", aName);
	printf("请输入上机的密码<长度为1~8>:");
	scanf("%s", aPwd);

	//开始上机，获取上机结果,提示不同信息
	nResult = doLogon(aName, aPwd, pInfo);//开始上机，获取上机结果
	switch (nResult)
	{
	case 0: {
		printf("\n上机失败！\n");
		break;
	}
	case 1: {
		//将时间转换成字符串
		timeToString(pInfo->tLogon, lastTime);
		printf("---------上机信息如下---------\n");
		printf("卡号\t余额\t上机时间\n");
		printf("%s\t%0.1f\t%s\n", pInfo->aCardName, pInfo->fBalance, lastTime);
		free(pInfo);//释放上机信息
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n该卡正在使用或者已注销/已失效！\n");
		break;
	}
	case 3: {
		printf("\n卡余额不足！\n");
		break;
	default:
		break;
	}
	}	
}

/*
4.下机
*/
void settle()
{
	char aName[18] = { 0 };//下机卡号
	char aPwd[8] = { 0 };//下机卡密码
	char startTime[TIMELENGTH] = { 0 };//上机时间
	char endTime[TIMELENGTH] = { 0 };//下机时间
	int nResult = -1;//下机结果
	SettleInfo* pInfo = NULL;

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//提示用户输入卡号和密码

	printf("----------下机卡---------\n");
	printf("请输入下机的卡号<长度为1~18>:");
	scanf("%s", aName);
	printf("请输入下机的密码<长度为1~8>:");
	scanf("%s", aPwd);

	nResult = doSettle(aName, aPwd, pInfo);//开始下机，获取下机结果
	switch (nResult)
	{
	case 0: {
		printf("\n下机失败！\n");
		break;
	}
	case 1: {
		//将时间转换成字符串
		timeToString(pInfo->tStart, startTime);
		timeToString(pInfo->tEnd, endTime);
		printf("---------下机信息如下---------\n");
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, startTime, endTime);
		free(pInfo);//释放下机信息
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n该卡未上机/已注销/已失效！\n");
		break;
	}
	case 3: {
		printf("\n卡余额不足！下机失败！\n");
		break;
	}
	default:
		break;
	}
}

/*
5.充值
功能：提示并接受用户输入上机的卡号和密码及金额，显示充值后的信息
*/
void addMoney()
{
	char aName[18] = { 0 };//上机卡号
	char aPwd[8] = { 0 };//上机卡密码
	float aMoney = 0;
	int nResult = 0;//保存充值结果
	MoneyInfo* pInfo = NULL;
	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	//提示用户输入卡号和密码
	printf("----------充值---------\n");
	printf("请输入充值卡号<长度为1~18>:");
	scanf("%s", aName);
	printf("请输入充值密码<长度为1~8>:");
	scanf("%s", aPwd);
	printf("请输入充值金额<RMB>:");
	scanf("%f", &aMoney);

	pInfo->fMoney = aMoney;
	strcpy(pInfo->aCardName, aName);
	//开始充值，获取结果,提示不同信息
	nResult = doAddMoney(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0: {
		printf("\n充值失败！\n");
		break;
	}
	case 1: {
		printf("---------充值信息如下---------\n");
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%0.1f\t\t%0.1f\n",pInfo->aCardName,pInfo->fMoney,pInfo->fBalance);
		free(pInfo);//释放下机信息
		pInfo = NULL;
		break;
	}
	default:
		break;
	}
}

/*
功能：退费
*/
void refundMoney()
{
	char aName[18] = { 0 };//上机卡号
	char aPwd[8] = { 0 };//上机卡密码
	float aMoney = 0;
	int nResult = 0;//保存充值结果
	MoneyInfo* pInfo = NULL;
	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	//提示用户输入卡号和密码
	printf("----------充值---------\n");
	printf("请输入退费卡号<长度为1~18>:");
	scanf("%s", aName);
	printf("请输入退费密码<长度为1~8>:");
	scanf("%s", aPwd);

	strcpy(pInfo->aCardName, aName);
	//开始充值，获取结果,提示不同信息
	nResult = doRefundMoney(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0: {
		printf("\n退费失败！\n");
		break;
	}
	case 1: {
		printf("---------退费信息如下---------\n");
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%0.1f\t%0.1f\n", pInfo->aCardName, pInfo->fMoney, pInfo->fBalance);
		free(pInfo);//释放下机信息
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n该卡正在使用或已失效！\n");
		break;
	}
	case 3: {
		printf("\n卡余额不足！\n");
		break;
	}
	default:
		break;
	}
}

/*
8.注销
*/
void annul()
{
	char aName[18] = { 0 };//注销卡号
	char aPwd[8] = { 0 };//注销卡密码
	int nResult = 0;//保存注销结果
	float fBalance = 0;//保存退款金额
	Card aCard;

	//提示用户输入卡号和密码
	printf("----------注销---------\n");
	printf("请输入注销卡号<长度为1~18>:");
	scanf("%s", aName);
	printf("请输入注销密码<长度为1~8>:");
	scanf("%s", aPwd);

	strcpy(aCard.aName, aName);
	strcpy(aCard.aPwd, aPwd);
	//获取注销结果,提示不同信息
	nResult = doAnnul(&aCard,&fBalance);
	switch (nResult)
	{
	case 0: {
		printf("\n注销卡失败！\n");
		break;
	}
	case 1: {
		printf("---------注销信息如下---------\n");
		printf("卡号\t退款金额\n");
		printf("%s\t%0.1f\n",aCard.aName,fBalance);
		break;
	}
	case 2: {
		printf("\n该卡正在使用或/已注销/已失效！\n");
		break;
	}
	default:
		break;
	}
}