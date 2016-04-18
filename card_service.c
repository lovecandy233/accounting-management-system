#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "model.h"//�������ݶ���ͷ�ļ�
#include "global.h"//����ȫ�ֶ���ͷ�ļ�
#include "card_file.h"
#include "card_service.h"
#include "billing_service.h"

IpCardNode cardList = NULL;//����Ϣ����,ȫ�ֱ������û��˻�����Ϣ������һ�����룩
IpCardNode pNode = NULL;//ָ������ͷ���

/*
���ܣ���ʼ������Ϣ����
������void
����ֵ��int�ͣ�TRUE��ʾ�ɹ�
*/
int initCardList()
{
	IpCardNode head = NULL;
	head = (IpCardNode)malloc(sizeof(CardNode));//Ϊ����ͷ�ڵ�����ڴ�
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
������:queryCards
���ܣ���ѯ�û��˻����ĺ���
������const char* pName, Ҫ��ѯ���û��˻�����int* pIndex��ָ��˳������ָ�룬���ڱ��������ļ���
����ֵ��int��0-FALSE��ʧ�ܣ�1-TRUE���ɹ�
	*/
Card* queryCards(const char* pName, int* pIndex)
{
	IpCardNode node = NULL;
	Card* pCard = NULL;

	if (FALSE == getCard())
	{
		return FALSE;
	}

	//���ȷ���һ��Card��С���ڴ�ռ�
	pCard = (Card*)malloc(sizeof(Card));//�����ѯ���ķ��������Ŀ���Ϣ

	if (cardList != NULL)
	{
		//��ͷ�ڵ�ָ�����һ���ڵ㿪ʼ����
		node = cardList;
		//�����������Ϊ�ձ�ʾ����β��
		while (node != NULL)
		{
			//�ж��ڸñ������Ľڵ����Ϣ�У������Ƿ����pName�ַ���
			if (strstr(node->data.aName, pName) != NULL)
			{
				//����У��򱣴�ڵ��е�����
				pCard[*pIndex] = node->data;
				(*pIndex)++;
				//����Ϊָ������ڴ�
				pCard = (Card*)realloc(pCard, ((*pIndex) + 1)*sizeof(Card));
			}
			//�Ƶ�������һ���
			node = node->next;
		}
	}
	return pCard;
}

/*
��ѯ����Ϣ
�ڽṹ������aCard�У����ҿ�����pName��ͬ�Ŀ���Ϣ
���ͨ������ֵ���أ�NULL��
*/
Card* queryCard(const char* pName)
{
	//����ָ��P��ָ��cardList�ĵ�һ���ڵ�
	IpCardNode cur = NULL;
	Card* pCard = NULL;
	int nIndex = 0;
	if (FALSE == getCard())
	{
		return FALSE;
	}
	//���ȷ���һ��Card��С���ڴ�ռ�
	pCard = (Card*)malloc(sizeof(Card));//�����ѯ���ķ��������Ŀ���Ϣ
	//��������
	if (cardList != NULL)
	{
		cur = cardList;
		//�����������Ϊ�ձ�ʾ����β��
		while (cur != NULL)
		{
			//�жϵ�ǰ�ڵ��еĿ����Ƿ������Ŀ�����ͬ
			if (strstr(cur->data.aName, pName) != NULL)
			{
				//����У��򱣴�ڵ��е�����
				pCard[nIndex] = cur->data;
				nIndex++;
				//����Ϊָ������ڴ�
				pCard = (Card*)realloc(pCard, (nIndex + 1)*sizeof(Card));
			}
			//�Ƶ�������һ���
			cur = cur->next;
		}
	}
	return pCard;
}

//������������ݣ��ѱ����ļ��������е�����ͬ��
int getCard()
{
	int nCount = 0; //����Ϣ��
	int i = 0;
	Card* pCard = NULL;
	

	IpCardNode node = NULL;
	IpCardNode cur = NULL;

	//����������Ѵ��ڵ����ݣ�����ʼ��
	if (cardList != NULL)
	{
		releaseCardList();
	}
	initCardList();
	//��ȡ����Ϣ����
	nCount = getCardCount(CARDPATH);

	//��̬�����ڴ汣�濨��Ϣ
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		return FALSE;
	}

	//�������FALSE����ʾ��ȡ����Ϣʧ��
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
			//Ϊ�ڵ�����ڴ�
			cur = (IpCardNode)malloc(sizeof(CardNode));
			//��������ڴ�ʧ�ܣ��򷵻�
			if (cur == NULL)
			{
				free(pCard);
				return FALSE;
			}
			//��ʼ���µĿռ䣬ȫ����ֵΪ��
			memset(cur, 0, sizeof(CardNode));

			//������Ϣ���浽�ڵ���
			cur->data = pCard[i];
			cur->next = NULL;

			//���ڵ���ӵ������β
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
���ܣ����ݿ��ź����룬�������в�ѯ����Ϣ
������������pName�ϻ�����;pPwd�ϻ�������;pIndex�ϻ����������е�������ָ��
����ֵ�����������Ŀ���Ϣ�ṹ��ָ��
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

	//���ȷ���һ��Card��С���ڴ�ռ�
	pCard = (Card*)malloc(sizeof(Card));//�����ѯ���ķ��������Ŀ���Ϣ

	if (cardList != NULL)
	{
		//��ͷ�ڵ�ָ�����һ���ڵ㿪ʼ����
		node = cardList;
		//�����������Ϊ�ձ�ʾ����β��
		while (node != NULL)
		{
			//�ж��ڸñ������Ľڵ����Ϣ�У������Ƿ����pName�ַ���
			if (strcmp(node->data.aName, pName)==0  && strcmp(node->data.aPwd, pPwd)==0)
			{
				pCard = &node->data;
				(*pIndex) = nIndex;
			}
			//�Ƶ�������һ���
			node = node->next;
			nIndex++;
		}
	}
	return pCard;
}