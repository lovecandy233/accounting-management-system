#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "model.h"
#include "billing_file.h"
#include "billing_service.h"
#include "tool.h"

#include <stdlib.h>
#include <string.h>

IpBillingNode billingList = NULL;//�Ʒ���Ϣ����,ȫ�ֱ���
IpBillingNode bNode = NULL;//ָ������ͷ���

/*
���ܣ���ʼ���Ʒ���Ϣ����
������void
����ֵ��int�ͣ�TRUE��ʾ�ɹ�
*/
int initBillingList()
{
	IpBillingNode head = NULL;
	head = (IpBillingNode)malloc(sizeof(BillingNode));//Ϊ����ͷ�ڵ�����ڴ�
	if (head != NULL)
	{
		head->next = NULL;
		billingList = head;
		bNode = billingList;
		return TRUE;
	}
	return FALSE;
}

/*
}*/
void releaseBillingList()
{
	IpBillingNode cur;
	while (billingList != NULL)
	{
		cur = billingList->next;
		free(billingList);
		billingList = NULL;
		billingList = cur;
	}
}

Billing* queryBilling(const char* pName)
{
	//����ָ��P��ָ��cardList�ĵ�һ���ڵ�
	IpBillingNode cur = NULL;
	Billing* pBilling= NULL;
	int nIndex = 0;
	if (FALSE == getBilling())
	{
		return FALSE;
	}
	//���ȷ���һ��Card��С���ڴ�ռ�
	pBilling = (Billing*)malloc(sizeof(Billing));//�����ѯ���ķ��������Ŀ���Ϣ
										//��������
	if (billingList != NULL)
	{
		cur = billingList;
		//�����������Ϊ�ձ�ʾ����β��
		while (cur != NULL)
		{
			//�жϵ�ǰ�ڵ��еĿ����Ƿ������Ŀ�����ͬ
			if (strstr(cur->data.aCardName, pName) != NULL)
			{
				//����У��򱣴�ڵ��е�����
				pBilling[nIndex] = cur->data;
				nIndex++;
				//����Ϊָ������ڴ�
				pBilling = (Billing*)realloc(pBilling, (nIndex + 1)*sizeof(Billing));
			}
			//�Ƶ�������һ���
			cur = cur->next;
		}
	}
	return pBilling;
}


/*
��ѯ�Ʒ���Ϣ
*/
Billing* checkBilling(const char* pName,int* pIndex)
{
	Billing* pBilling = NULL;
	IpBillingNode node = NULL;
	int nIndex = 0;

	if (FALSE == getBilling())
	{
		return FALSE;
	} 
	//���ȷ���һ��Billing��С���ڴ�ռ�
	pBilling = (Billing*)malloc(sizeof(Billing));//�����ѯ���ķ��������ļƷ���Ϣ

	if (billingList != NULL)
	{
		//��ͷ�ڵ�ָ�����һ���ڵ㿪ʼ����
		node = billingList;
		//�����������Ϊ�ձ�ʾ����β��
		while (node != NULL)
		{
			//�ж��ڸñ������Ľڵ����Ϣ�У������Ƿ����pName�ַ���
			if (strcmp(node->data.aCardName, pName)==0 )
			{
				//����У��򱣴�ڵ��е�����
				pBilling = &node->data;
				(*pIndex) = nIndex;
				//����Ϊָ������ڴ�
			}
			//�Ƶ�������һ���
			node = node->next;
			nIndex++;
		}
	}
	return pBilling;
}

/*
���ܣ���ȡ�Ʒ���Ϣ
����ֵ��TRUE ��ȡ�Ʒ���Ϣ�ɹ���FALSE ��ȡ�Ʒ���Ϣʧ��
*/
int getBilling()
{
	int nCount = 0; //�Ʒ���Ϣ��
	int i = 0;
	Billing* pBilling = NULL;

	IpBillingNode node = NULL;
	IpBillingNode cur = NULL;

	//����������Ѵ��ڵ����ݣ�����ʼ��
	if (billingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();
	//��ȡ�Ʒ���Ϣ����
	nCount = getBillingCount(BILLINGPATH);

	//��̬�����ڴ汣��Ʒ���Ϣ
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling == NULL)
	{
		return FALSE;
	}
	//���ļ��л�ȡ�Ʒ���Ϣ
	if (FALSE == readBilling(pBilling, BILLINGPATH))
	{
		free(pBilling);
		pBilling = NULL;
		return FALSE;
	}
	
	for (i = 0, node = billingList; i < nCount; i++)
	{
		if (i == 0)
		{
			node->data = pBilling[i];
			node->next = NULL;
		}
		else
		{
			cur = (IpBillingNode)malloc(sizeof(BillingNode));
			//��������ڴ�ʧ�ܣ��򷵻�
			if (cur == NULL)
			{
				free(pBilling);
				return FALSE;
			}
			//��ʼ���µĿռ䣬ȫ����ֵΪ��
			memset(cur, 0, sizeof(BillingNode));
			cur->data = pBilling[i];
			cur->next = NULL;
			//���ڵ���ӵ������β
			node->next = cur;
			node = cur;
		}
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}