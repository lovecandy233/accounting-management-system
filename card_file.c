#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "global.h"
#include "tool.h"

Card prassCard(char* pBuf);

#define CARDCHARNUM 256

/*
��������saveCard
���ܣ�����ӵĿ���Ϣ�����浽ָ��·���µ��ı��ļ���
������pCard-��Ҫ���浽�ļ��еĿ���Ϣ�Ľṹ��ָ�룬pPath-���濨��Ϣ��·��
����ֵ��TRUE-�ɹ���FALSE-ʧ��
*/
int saveCard(const Card* pCard, const char* pPath)
{

	FILE* fp = NULL;

	char startTime[TIMELENGTH] = { 0 };//��ʼʱ��
	char endTime[TIMELENGTH] = { 0 };//��ֹʱ��
	char lastTime[TIMELENGTH] = { 0 };//���ʹ��ʱ��

									  //���ļ�
	fp = fopen(pPath, "a");
	if ((fp == NULL))
	{
		fp = fopen(pPath, "w");
		if ((fp == NULL))
		{
			return FALSE;
		}
	}
	//��ʱ��ת�����ַ���
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLast, lastTime);

	//������д���ļ�
	//����##����##״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##��ǰ���##ɾ����ʶ
	fprintf(fp, "%s##%s##%d##%s##%s##%0.1f##%s##%d##%0.1f##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}


/*
������:readCard
���ܣ����ڽ��ļ�����д��ṹ��
������ Card* pCard���ṹ��ָ�� const char* pPath �ļ�·��
����ֵ��int��0-FALSE��ʧ�ܣ�1-TRUE���ɹ�
*/
int readCard(Card* pCard, const char* pPath)
{
	//���ļ��ж�ȡ����Ϣ
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };//����Ϣ�ַ�������
	int nIndex = 0;//����Ϣ����


	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}

	//���ļ������ж�ȡ
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);//�������
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				//����prassCard(),������ȡ�Ŀ���Ϣ
				pCard[nIndex] = prassCard(aBuf);
				nIndex++;
			}
		}
	}
	fclose(fp);
	return TRUE;
}

/*
������:prassCard
���ܣ��������������ڽ���ÿ���û��˻��Ľṹ��
������pBuf-���濨��Ϣ���ַ���
����ֵ��Card-�ַ���ת���ɵĿ���Ϣ�ṹ��
*/
Card prassCard(char* pBuf)
{
	Card card;//����������Ŀ���Ϣ

	const char *delims = "##";//�ַ����еķָ���
	char flag[10][20] = { 0 };//����ָ����ַ�
	int index = 0;//��ʾflag�����±�
	char *buf = NULL;
	char *str = NULL;

	buf = pBuf;//��һ�ε���strtokʱ��bufΪ�����ַ���
	str = strtok(buf, delims);
	while ((str != NULL))
	{
		strcpy(flag[index], str);
		buf = NULL;//�������strtok����ʱ����һ������ΪNULL
		index++;
		str = strtok(buf, delims);
	}
	//����##����##״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##���##ɾ����ʶ
	//�ָ������ݱ��浽�ṹ����
	strcpy(card.aName, flag[0]);//����
	strcpy(card.aPwd, flag[1]);//����
	card.nStatus = atoi(flag[2]);//״̬
	card.tStart = stringToTime(flag[3]);//����ʱ��
	card.tEnd = stringToTime(flag[4]);//��ֹʱ��
	card.fTotalUse = (float)atof(flag[5]); //�ۼƽ��
	card.tLast = stringToTime(flag[6]);//���ʹ��ʱ��
	card.nUseCount = atoi(flag[7]);//ʹ�ô���
	card.fBalance = (float)atof(flag[8]); //���
	card.nDel = atoi(flag[9]);//ɾ�����

	return card;
}


/*
������: getCardCount
���ܣ���ȡ�ļ����û��˻�������
������const char * pPath���洢�û��˻��ļ�·��
����ֵ��int���ļ��е��û��˻���Ŀ
*/
int getCardCount(const char* pPath)
{
	FILE* fp = NULL;
	int nIndex = 0;
	char aBuf[CARDCHARNUM] = { 0 };

	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);//�������
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				nIndex++;
			}
		}
	}
	fclose(fp);
	//��ȡ�ļ�����Ϣ������
	return nIndex;
}


/*
��������updataCard()
������pCardΪ����Ϣ�ṹ��ָ��;pPathΪ���濨��Ϣ�ĵ�ַ;nIndexΪ�ϻ����ڿ���Ϣ�����е�������
����ֵ�����е��ϻ�����Ϣ���³ɹ�����TRUE��ʧ�ܷ���FALSE
*/
int updataCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;//�ļ�ָ��
	char aBuf[CARDCHARNUM] = { 0 };

	char startTime[TIMELENGTH] = { 0 };//����ʱ��
	char endTime[TIMELENGTH] = { 0 };//��ֹʱ��
	char lastTime[TIMELENGTH] = { 0 };//���ʹ��ʱ��

	int nLine = 0;//�ļ�����
	long IPosition = 0;//�ļ�λ�ñ��

					   //��ʱ��ת��Ϊ�ַ���
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLast, lastTime);

	//���ļ������ʧ�ܣ�����FALSE
	fp = fopen(CARDPATH, "r+");
	if (fp == NULL)
	{
		return FALSE;
	}

	//�����ļ�,�ҵ�������¼�����и���
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			//��ȡ�ļ���־λ��
			IPosition = ftell(fp);
			nLine++;
		}
	}
	//�Ƶ��ļ���ʶλ��
	fseek(fp, IPosition, 0);
	//������д���ļ�
	//����##����##״̬##����ʱ��##��ֹʱ��##�ۼƽ��##���ʹ��ʱ��##ʹ�ô���##��ǰ���##ɾ����ʶ
	fprintf(fp, "%s##%s##%d##%s##%s##%0.1f##%s##%d##%0.1f##%d\n", pCard->aName, pCard->aPwd, pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}