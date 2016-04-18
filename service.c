#define _CRT_SECURE_NO_WARNINGS

//分别管理card_service和billing_service
//card_service处理与卡信息链表相关的业务逻辑
//billing_service处理与消费信息链表相关的业务逻辑
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "model.h"
#include "global.h"
#include "card_file.h"
#include "card_service.h"
#include "billing_service.h"
#include "billing_file.h"
#include "money_file.h"
#include "tool.h"

double getAmount(time_t tStart);

/*
函数名：int doLogon（）
参数：pName上机卡号;pPwd上机卡密码;pInfo上机信息
返回值：表示上机结果
*/
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	int nIndex = 0;
	Billing* pBilling = NULL;//计费信息
	Card* pCard = NULL;
	//调用card_service中的checkCard()实现从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd,&nIndex);
	if (pCard != NULL)
	{
		//只有未上机的卡才能进行上机
		if (pCard->nStatus != 0 )
		{
			return UNUSE;
		}
		//余额大于0，才能上机
		if (pCard->fBalance <= 0)
		{
			return ENOUGHMONEY;
		}
		//更新链表中的卡信息
		pCard->nStatus = 1;//状态为正在使用
		pCard->nUseCount++;//使用次数加
		pCard->tLast = time(NULL);//最后使用时间为当前时间

		//调用updataCard()更新文件中的卡信息
		updataCard(pCard, CARDPATH, nIndex);
		//调用billing_file.c中的saveBilling,保存计费信息到文件
		//添加消费记录
		pBilling = (Billing*)malloc(sizeof(Billing));
		strcpy(pBilling->aCardName, pName);
		pBilling->nDel = 0;
		pBilling->nStatus = 0;
		pBilling->fAmount = 0;
		pBilling->tStart =pBilling->tEnd = time(NULL);
		pBilling->tEnd = endTime(pBilling->tStart, pBilling->tEnd);

		if (saveBilling(pBilling, BILLINGPATH))
		{
			//成功上机，返回上机信息
			strcpy(pInfo->aCardName, pCard->aName);
			pInfo->fBalance = pCard->fBalance;
			pInfo->tLogon = pBilling->tStart;
			//上机成功
			return TRUE;
		}
	}
	return FALSE;
}

/*
参数：pName下机卡号;pPwd下机卡密码;pInfo下机信息
返回值：表示下机结果
*/
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
	int nIndex = 0;
	int nPosition = 0;//计费信息索引
	Card* pCard = NULL;
	Billing* pBilling = NULL;//计费信息
	float dbAmount = 0;
	float fBalance = 0;
	//调用card_service中的checkCard()实现从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//只有上机的卡才能进行下机
		if (pCard->nStatus != 1 )
		{
			return UNUSE;
		}
		//添加消费记录
		pBilling =checkBilling(pName, &nPosition);
		if (pBilling == NULL)
		{
			return FALSE;
		}
		//计算消费金额
		dbAmount = (float)getAmount(pBilling->tStart);
		
		//余额大于等于0，才能下机
		fBalance = pCard->fBalance - dbAmount;
		if (fBalance < 0)
		{
			return ENOUGHMONEY;
		}
		//更新链表中的卡信息
		pCard->nStatus = 0;//状态为未使用
		pCard->tLast = pBilling->tEnd = time(NULL);//下机时间为当前时间
		pCard->fBalance = fBalance;
		pBilling->fAmount = dbAmount;

		//调用updataCard()更新文件中的卡信息
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		//更新文件中的计费信息
		if (!updataBilling(pBilling, BILLINGPATH, nPosition))
		{
			return FALSE;
		}
		strcpy(pInfo->aCardName, pName);
		pInfo->fAmount = dbAmount;
		pInfo->fBalance = fBalance;
		pInfo->tEnd = pBilling->tEnd;//下机时间
		pInfo->tStart = pBilling->tStart;
		return TRUE;
	}
	return FALSE;
}
/*
函数名:int addCardInfo(Card card)
功能：将结构体指针的数据写入文件，用于调用saveCard()函数
参数：Card card，结构体指针
返回值：int；0-FALSE，失败；1-TRUE，成功
*/
int addCardInfo(Card card)
{
	return saveCard(&card, CARDPATH);
}

/*
参数：pName为需要查询的卡号
返回值：查询到的卡信息指针
*/
Card* queryCardInfo(const char* pName) 
{
	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;
}

int addBillingInfo(Billing billing)
{
	return saveBilling(&billing, BILLINGPATH);
}
Billing* queryBillingInfo(const char* pName)
{
	Billing* pBilling = NULL;
	pBilling = queryBilling(pName);
	return pBilling;
}


/*
释放卡信息链表内存
*/
void releaseList()
{
	//调用card_service 中的releaseCardList()
	releaseCardList();
	releaseBillingList();
}

/*
功能：计算消费金额
*/
double getAmount(time_t tStart)
{
	double dbAmount = 0;
	int nMinutes = 0;
	int nCount = 0;
	time_t tEnd = time(NULL);
	time_t nSec = 0;
	//计算消费时长
	nSec = tEnd - tStart;
	nMinutes = (int)nSec / 60;
	//计算消费的时间单元数
	if (nMinutes % UNIT == 0)
	{
		nCount = nMinutes / UNIT;
	}
	else
	{
		nCount = nMinutes / UNIT + 1;
	}
	//计算消费金额
	dbAmount = nCount*CHARGE;
	return dbAmount;
}

/*
功能：充值
*/
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	int nIndex = 0;
	Card* pCard = NULL;
	Money* pMoney = NULL;//充值退费信息
	float fBalance = 0;
	//调用card_service中的checkCard()实现从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//只有上机的卡才能进行充值
		if (pCard->nStatus == 1)
		{
			pMoney = (Money*)malloc(sizeof(Money));

			//能充值，则更新链表中的卡信息
			pCard->fBalance += pMoneyInfo->fMoney;
			pCard->fTotalUse += pMoneyInfo->fMoney;
			pCard->tLast = pMoney->tTime = time(NULL);//当前时间

			strcpy(pMoney->aCardName, pName);
			pMoney->fMoney = pMoneyInfo->fMoney;
			pMoney->nDel = 0;
			pMoney->nStatus = 0;//充值
			//调用updataCard()更新文件中的卡信息
			if (!updataCard(pCard, CARDPATH, nIndex)) {
				return FALSE;
			}
			//更新文件中的充值退费信息
			if (!saveMoney(pMoney, MONEYPATH))
			{
				return FALSE;
			}
			pMoneyInfo->fBalance = (pCard->fBalance);
			return TRUE;
		}
	}
	return FALSE;
}

int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	int nIndex = 0;
	Card* pCard = NULL;
	Money* pMoney = NULL;//充值退费信息
	float fBalance = 0;
	//调用card_service中的checkCard()实现从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//只有未使用的卡才能进行退费
		if (pCard->nStatus == 1 || pCard->nStatus == 3)
		{
			return UNUSE;
		}
		//余额不足
		if (pCard->fBalance <= 0)
		{
			return ENOUGHMONEY;
		}
		pMoney = (Money*)malloc(sizeof(Money));
		//能退费，则更新链表中的卡信息
		pMoneyInfo->fMoney = pCard->fBalance;
		pCard->fBalance = 0;

		pMoney->tTime = time(NULL);//操作时间
		strcpy(pMoney->aCardName, pName);
		pMoney->fMoney = 0;
		pMoney->nDel = 0;
		pMoney->nStatus = 1;//退费
							//调用updataCard()更新文件中的卡信息
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		//更新文件中的充值退费信息
		if (!saveMoney(pMoney, MONEYPATH))
		{
			return FALSE;
		}
		pMoneyInfo->fBalance = 0;
		return TRUE;
		}
	return FALSE;
}

int doAnnul(Card* pCard,float *fBalance)
{
	int nIndex = 0;
	//调用card_service中的checkCard()实现从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pCard->aName,pCard->aPwd, &nIndex);
	if (pCard != NULL)
	{
		//正在使用的卡不能注销
		if (pCard->nStatus == 1||pCard->nStatus==2)
		{
			return UNUSE;
		}
		//
		if (pCard->fBalance < 0)
		{
			return ENOUGHMONEY;
		}
		//更新链表中的卡信息
		pCard->nStatus = 2;//状态为已注销
		*fBalance = pCard->fBalance;
		pCard->fBalance = 0;//余额
		pCard->tLast = time(NULL);//最后使用为当前时间

		//调用updataCard()更新文件中的卡信息
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}