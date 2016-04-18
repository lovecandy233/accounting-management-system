#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "model.h"
#include "billing_file.h"
#include "billing_service.h"
#include "tool.h"

#include <stdlib.h>
#include <string.h>

IpBillingNode billingList = NULL;//计费信息链表,全局变量
IpBillingNode bNode = NULL;//指向链表头结点

/*
功能：初始化计费信息链表
参数：void
返回值：int型，TRUE表示成功
*/
int initBillingList()
{
	IpBillingNode head = NULL;
	head = (IpBillingNode)malloc(sizeof(BillingNode));//为链表头节点分配内存
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
	//定义指针P，指向cardList的第一个节点
	IpBillingNode cur = NULL;
	Billing* pBilling= NULL;
	int nIndex = 0;
	if (FALSE == getBilling())
	{
		return FALSE;
	}
	//首先分配一个Card大小的内存空间
	pBilling = (Billing*)malloc(sizeof(Billing));//保存查询到的符合条件的卡信息
										//遍历链表
	if (billingList != NULL)
	{
		cur = billingList;
		//遍历链表，结点为空表示链表尾部
		while (cur != NULL)
		{
			//判断当前节点中的卡号是否和输入的卡号相同
			if (strstr(cur->data.aCardName, pName) != NULL)
			{
				//如果有，则保存节点中的数据
				pBilling[nIndex] = cur->data;
				nIndex++;
				//重新为指针分配内存
				pBilling = (Billing*)realloc(pBilling, (nIndex + 1)*sizeof(Billing));
			}
			//移到链表下一结点
			cur = cur->next;
		}
	}
	return pBilling;
}


/*
查询计费信息
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
	//首先分配一个Billing大小的内存空间
	pBilling = (Billing*)malloc(sizeof(Billing));//保存查询到的符合条件的计费信息

	if (billingList != NULL)
	{
		//从头节点指向的下一个节点开始遍历
		node = billingList;
		//遍历链表，结点为空表示链表尾部
		while (node != NULL)
		{
			//判断在该遍历到的节点的信息中，查找是否包含pName字符串
			if (strcmp(node->data.aCardName, pName)==0 )
			{
				//如果有，则保存节点中的数据
				pBilling = &node->data;
				(*pIndex) = nIndex;
				//重新为指针分配内存
			}
			//移到链表下一结点
			node = node->next;
			nIndex++;
		}
	}
	return pBilling;
}

/*
功能：获取计费信息
返回值：TRUE 获取计费信息成功；FALSE 获取计费信息失败
*/
int getBilling()
{
	int nCount = 0; //计费信息数
	int i = 0;
	Billing* pBilling = NULL;

	IpBillingNode node = NULL;
	IpBillingNode cur = NULL;

	//清除链表中已存在的数据，并初始化
	if (billingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();
	//获取计费信息数量
	nCount = getBillingCount(BILLINGPATH);

	//动态分配内存保存计费信息
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling == NULL)
	{
		return FALSE;
	}
	//从文件中获取计费信息
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
			//如果分配内存失败，则返回
			if (cur == NULL)
			{
				free(pBilling);
				return FALSE;
			}
			//初始化新的空间，全部赋值为零
			memset(cur, 0, sizeof(BillingNode));
			cur->data = pBilling[i];
			cur->next = NULL;
			//将节点添加到链表结尾
			node->next = cur;
			node = cur;
		}
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}