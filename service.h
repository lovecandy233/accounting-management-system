#include "model.h"

#ifndef SERVICE_H
#define SERVICE_H

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
int addCardInfo(Card card);//��ӿ�����
Card* queryCardInfo(const char* pName);
void releaseList();//�ͷ�
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);//��ֵ
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);//�˷�
int doAnnul(Card* pCard,float* fBalance);//ע��
#endif // !SERVICE_H
#pragma once
