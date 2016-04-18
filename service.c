#define _CRT_SECURE_NO_WARNINGS

//�ֱ����card_service��billing_service
//card_service�����뿨��Ϣ������ص�ҵ���߼�
//billing_service������������Ϣ������ص�ҵ���߼�
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
��������int doLogon����
������pName�ϻ�����;pPwd�ϻ�������;pInfo�ϻ���Ϣ
����ֵ����ʾ�ϻ����
*/
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	int nIndex = 0;
	Billing* pBilling = NULL;//�Ʒ���Ϣ
	Card* pCard = NULL;
	//����card_service�е�checkCard()ʵ�ִ������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd,&nIndex);
	if (pCard != NULL)
	{
		//ֻ��δ�ϻ��Ŀ����ܽ����ϻ�
		if (pCard->nStatus != 0 )
		{
			return UNUSE;
		}
		//������0�������ϻ�
		if (pCard->fBalance <= 0)
		{
			return ENOUGHMONEY;
		}
		//���������еĿ���Ϣ
		pCard->nStatus = 1;//״̬Ϊ����ʹ��
		pCard->nUseCount++;//ʹ�ô�����
		pCard->tLast = time(NULL);//���ʹ��ʱ��Ϊ��ǰʱ��

		//����updataCard()�����ļ��еĿ���Ϣ
		updataCard(pCard, CARDPATH, nIndex);
		//����billing_file.c�е�saveBilling,����Ʒ���Ϣ���ļ�
		//������Ѽ�¼
		pBilling = (Billing*)malloc(sizeof(Billing));
		strcpy(pBilling->aCardName, pName);
		pBilling->nDel = 0;
		pBilling->nStatus = 0;
		pBilling->fAmount = 0;
		pBilling->tStart =pBilling->tEnd = time(NULL);
		pBilling->tEnd = endTime(pBilling->tStart, pBilling->tEnd);

		if (saveBilling(pBilling, BILLINGPATH))
		{
			//�ɹ��ϻ��������ϻ���Ϣ
			strcpy(pInfo->aCardName, pCard->aName);
			pInfo->fBalance = pCard->fBalance;
			pInfo->tLogon = pBilling->tStart;
			//�ϻ��ɹ�
			return TRUE;
		}
	}
	return FALSE;
}

/*
������pName�»�����;pPwd�»�������;pInfo�»���Ϣ
����ֵ����ʾ�»����
*/
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
	int nIndex = 0;
	int nPosition = 0;//�Ʒ���Ϣ����
	Card* pCard = NULL;
	Billing* pBilling = NULL;//�Ʒ���Ϣ
	float dbAmount = 0;
	float fBalance = 0;
	//����card_service�е�checkCard()ʵ�ִ������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//ֻ���ϻ��Ŀ����ܽ����»�
		if (pCard->nStatus != 1 )
		{
			return UNUSE;
		}
		//������Ѽ�¼
		pBilling =checkBilling(pName, &nPosition);
		if (pBilling == NULL)
		{
			return FALSE;
		}
		//�������ѽ��
		dbAmount = (float)getAmount(pBilling->tStart);
		
		//�����ڵ���0�������»�
		fBalance = pCard->fBalance - dbAmount;
		if (fBalance < 0)
		{
			return ENOUGHMONEY;
		}
		//���������еĿ���Ϣ
		pCard->nStatus = 0;//״̬Ϊδʹ��
		pCard->tLast = pBilling->tEnd = time(NULL);//�»�ʱ��Ϊ��ǰʱ��
		pCard->fBalance = fBalance;
		pBilling->fAmount = dbAmount;

		//����updataCard()�����ļ��еĿ���Ϣ
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		//�����ļ��еļƷ���Ϣ
		if (!updataBilling(pBilling, BILLINGPATH, nPosition))
		{
			return FALSE;
		}
		strcpy(pInfo->aCardName, pName);
		pInfo->fAmount = dbAmount;
		pInfo->fBalance = fBalance;
		pInfo->tEnd = pBilling->tEnd;//�»�ʱ��
		pInfo->tStart = pBilling->tStart;
		return TRUE;
	}
	return FALSE;
}
/*
������:int addCardInfo(Card card)
���ܣ����ṹ��ָ�������д���ļ������ڵ���saveCard()����
������Card card���ṹ��ָ��
����ֵ��int��0-FALSE��ʧ�ܣ�1-TRUE���ɹ�
*/
int addCardInfo(Card card)
{
	return saveCard(&card, CARDPATH);
}

/*
������pNameΪ��Ҫ��ѯ�Ŀ���
����ֵ����ѯ���Ŀ���Ϣָ��
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
�ͷſ���Ϣ�����ڴ�
*/
void releaseList()
{
	//����card_service �е�releaseCardList()
	releaseCardList();
	releaseBillingList();
}

/*
���ܣ��������ѽ��
*/
double getAmount(time_t tStart)
{
	double dbAmount = 0;
	int nMinutes = 0;
	int nCount = 0;
	time_t tEnd = time(NULL);
	time_t nSec = 0;
	//��������ʱ��
	nSec = tEnd - tStart;
	nMinutes = (int)nSec / 60;
	//�������ѵ�ʱ�䵥Ԫ��
	if (nMinutes % UNIT == 0)
	{
		nCount = nMinutes / UNIT;
	}
	else
	{
		nCount = nMinutes / UNIT + 1;
	}
	//�������ѽ��
	dbAmount = nCount*CHARGE;
	return dbAmount;
}

/*
���ܣ���ֵ
*/
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo)
{
	int nIndex = 0;
	Card* pCard = NULL;
	Money* pMoney = NULL;//��ֵ�˷���Ϣ
	float fBalance = 0;
	//����card_service�е�checkCard()ʵ�ִ������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//ֻ���ϻ��Ŀ����ܽ��г�ֵ
		if (pCard->nStatus == 1)
		{
			pMoney = (Money*)malloc(sizeof(Money));

			//�ܳ�ֵ������������еĿ���Ϣ
			pCard->fBalance += pMoneyInfo->fMoney;
			pCard->fTotalUse += pMoneyInfo->fMoney;
			pCard->tLast = pMoney->tTime = time(NULL);//��ǰʱ��

			strcpy(pMoney->aCardName, pName);
			pMoney->fMoney = pMoneyInfo->fMoney;
			pMoney->nDel = 0;
			pMoney->nStatus = 0;//��ֵ
			//����updataCard()�����ļ��еĿ���Ϣ
			if (!updataCard(pCard, CARDPATH, nIndex)) {
				return FALSE;
			}
			//�����ļ��еĳ�ֵ�˷���Ϣ
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
	Money* pMoney = NULL;//��ֵ�˷���Ϣ
	float fBalance = 0;
	//����card_service�е�checkCard()ʵ�ִ������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard != NULL)
	{
		//ֻ��δʹ�õĿ����ܽ����˷�
		if (pCard->nStatus == 1 || pCard->nStatus == 3)
		{
			return UNUSE;
		}
		//����
		if (pCard->fBalance <= 0)
		{
			return ENOUGHMONEY;
		}
		pMoney = (Money*)malloc(sizeof(Money));
		//���˷ѣ�����������еĿ���Ϣ
		pMoneyInfo->fMoney = pCard->fBalance;
		pCard->fBalance = 0;

		pMoney->tTime = time(NULL);//����ʱ��
		strcpy(pMoney->aCardName, pName);
		pMoney->fMoney = 0;
		pMoney->nDel = 0;
		pMoney->nStatus = 1;//�˷�
							//����updataCard()�����ļ��еĿ���Ϣ
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		//�����ļ��еĳ�ֵ�˷���Ϣ
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
	//����card_service�е�checkCard()ʵ�ִ������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pCard->aName,pCard->aPwd, &nIndex);
	if (pCard != NULL)
	{
		//����ʹ�õĿ�����ע��
		if (pCard->nStatus == 1||pCard->nStatus==2)
		{
			return UNUSE;
		}
		//
		if (pCard->fBalance < 0)
		{
			return ENOUGHMONEY;
		}
		//���������еĿ���Ϣ
		pCard->nStatus = 2;//״̬Ϊ��ע��
		*fBalance = pCard->fBalance;
		pCard->fBalance = 0;//���
		pCard->tLast = time(NULL);//���ʹ��Ϊ��ǰʱ��

		//����updataCard()�����ļ��еĿ���Ϣ
		if (!updataCard(pCard, CARDPATH, nIndex)) {
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}