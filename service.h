#include "model.h"

#ifndef SERVICE_H
#define SERVICE_H

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
int addCardInfo(Card card);//添加卡函数
Card* queryCardInfo(const char* pName);
void releaseList();//释放
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);//充值
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);//退费
int doAnnul(Card* pCard,float* fBalance);//注销
#endif // !SERVICE_H
#pragma once
