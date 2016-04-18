#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "model.h"//包含数据定义头文件
#include "global.h"//包含全局定义头文件
#include "card_file.h"
#include "card_service.h"
#include "billing_service.h"

IpCardNode cardList = NULL;//卡信息链表,全局变量，用户账户的信息链表（第一次输入）
IpCardNode pNode = NULL;//指向链表头结点

/*
功能：初始化卡信息链表
参数：void
返回值：int型，TRUE表示成功
*/
int initCardList()
{
	IpCardNode head = NULL;
	head = (IpCardNode)malloc(sizeof(CardNode));//为链表头节点分配内存
	if (head != NULL)
	{
		head->next = NULL;
		cardList = head;
		pNode = cardList;
		return TRUE;
	}
	return FALSE;
}

void releaseCardList()
{
	IpCardNode cur;
	while (cardList != NULL)
	{
		cur = cardList->next;
		free(cardList);
		cardList = NULL;
		cardList = cur;
	}
}



/*
函数名:queryCards
功能：查询用户账户核心函数
参数：const char* pName, 要查询的用户账户名；int* pIndex，指向顺序数的指针，用于遍历链表及文件用
返回值：int；0-FALSE，失败；1-TRUE，成功
	*/
Card* queryCards(const char* pName, int* pIndex)
{
	IpCardNode node = NULL;
	Card* pCard = NULL;

	if (FALSE == getCard())
	{
		return FALSE;
	}

	//首先分配一个Card大小的内存空间
	pCard = (Card*)malloc(sizeof(Card));//保存查询到的符合条件的卡信息

	if (cardList != NULL)
	{
		//从头节点指向的下一个节点开始遍历
		node = cardList;
		//遍历链表，结点为空表示链表尾部
		while (node != NULL)
		{
			//判断在该遍历到的节点的信息中，查找是否包含pName字符串
			if (strstr(node->data.aName, pName) != NULL)
			{
				//如果有，则保存节点中的数据
				pCard[*pIndex] = node->data;
				(*pIndex)++;
				//重新为指针分配内存
				pCard = (Card*)realloc(pCard, ((*pIndex) + 1)*sizeof(Card));
			}
			//移到链表下一结点
			node = node->next;
		}
	}
	return pCard;
}

/*
查询卡信息
在结构体数组aCard中，查找卡号与pName相同的卡信息
结果通过返回值返回（NULL）
*/
Card* queryCard(const char* pName)
{
	//定义指针P，指向cardList的第一个节点
	IpCardNode cur = NULL;
	Card* pCard = NULL;
	int nIndex = 0;
	if (FALSE == getCard())
	{
		return FALSE;
	}
	//首先分配一个Card大小的内存空间
	pCard = (Card*)malloc(sizeof(Card));//保存查询到的符合条件的卡信息
	//遍历链表
	if (cardList != NULL)
	{
		cur = cardList;
		//遍历链表，结点为空表示链表尾部
		while (cur != NULL)
		{
			//判断当前节点中的卡号是否和输入的卡号相同
			if (strstr(cur->data.aName, pName) != NULL)
			{
				//如果有，则保存节点中的数据
				pCard[nIndex] = cur->data;
				nIndex++;
				//重新为指针分配内存
				pCard = (Card*)realloc(pCard, (nIndex + 1)*sizeof(Card));
			}
			//移到链表下一结点
			cur = cur->next;
		}
	}
	return pCard;
}

//清空链表中数据，已保存文件和链表中的数据同步
int getCard()
{
	int nCount = 0; //卡信息数
	int i = 0;
	Card* pCard = NULL;
	

	IpCardNode node = NULL;
	IpCardNode cur = NULL;

	//清除链表中已存在的数据，并初始化
	if (cardList != NULL)
	{
		releaseCardList();
	}
	initCardList();
	//获取卡信息个数
	nCount = getCardCount(CARDPATH);

	//动态分配内存保存卡信息
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		return FALSE;
	}

	//如果返回FALSE，表示读取卡信息失败
	if (FALSE == readCard(pCard, CARDPATH))
	{
		free(pCard);
		pCard = NULL;
		return FALSE;
    }

	//node->next = NULL;
	for (i = 0, node = cardList; i < nCount; i++)
	{
		if (i == 0)
		{
			node->data = pCard[i];
			node->next = NULL;
		}
		else
		{
			//为节点分配内存
			cur = (IpCardNode)malloc(sizeof(CardNode));
			//如果分配内存失败，则返回
			if (cur == NULL)
			{
				free(pCard);
				return FALSE;
			}
			//初始化新的空间，全部赋值为零
			memset(cur, 0, sizeof(CardNode));

			//将卡信息保存到节点中
			cur->data = pCard[i];
			cur->next = NULL;

			//将节点添加到链表结尾
			node->next = cur;
			node = cur;
		}
	}
	//free(cur);
	free(pCard);
	pCard = NULL;
	return TRUE;
}

/*
功能：根据卡号和密码，在链表中查询卡信息
参数：参数：pName上机卡号;pPwd上机卡密码;pIndex上机卡在链表中的索引的指针
返回值：符合条件的卡信息结构体指针
*/
Card* checkCard(const char* pName, const char* pPwd,int *pIndex)
{
	Card* pCard = NULL;
	IpCardNode node = NULL;
	int nIndex = 0;
	if (FALSE == getCard())
	{
		return FALSE;
	}

	//首先分配一个Card大小的内存空间
	pCard = (Card*)malloc(sizeof(Card));//保存查询到的符合条件的卡信息

	if (cardList != NULL)
	{
		//从头节点指向的下一个节点开始遍历
		node = cardList;
		//遍历链表，结点为空表示链表尾部
		while (node != NULL)
		{
			//判断在该遍历到的节点的信息中，查找是否包含pName字符串
			if (strcmp(node->data.aName, pName)==0  && strcmp(node->data.aPwd, pPwd)==0)
			{
				pCard = &node->data;
				(*pIndex) = nIndex;
			}
			//移到链表下一结点
			node = node->next;
			nIndex++;
		}
	}
	return pCard;
}