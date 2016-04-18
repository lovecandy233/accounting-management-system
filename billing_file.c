#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "global.h"
#include "tool.h"

Billing prassBilling(char* pBuf);

#define BILLINGCHARNUM 256


/*
��������saveCard
���ܣ�����ӵĿ���Ϣ�����浽ָ��·���µ��ı��ļ���
������pCard-��Ҫ���浽�ļ��еĿ���Ϣ�Ľṹ��ָ�룬pPath-���濨��Ϣ��·��
����ֵ��TRUE-�ɹ���FALSE-ʧ��
*/
int saveBilling(const Billing* pBilling, const char* pPath)
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
	timeToString(pBilling->tStart, startTime);
	timeToString(pBilling->tEnd, endTime);

	//������д���ļ�
	//����##״̬##�ϻ�ʱ��##�»�ʱ��##���ѽ��##ɾ����ʶ
	fprintf(fp, "%s##%d##%s##%s##%0.1f##%d\n", pBilling->aCardName, pBilling->nStatus, startTime, endTime, pBilling->fAmount, pBilling->nDel);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}


/*
������:readBilling
���ܣ����ڽ��ļ�����д��ṹ��
������Billing* pBilling���ṹ��ָ�� const char* pPath �ļ�·��
����ֵ��int��0-FALSE��ʧ�ܣ�1-TRUE���ɹ�
*/
int readBilling(Billing* pBilling, const char* pPath)
{
	//���ļ��ж�ȡ����Ϣ
	FILE* fp = NULL;
	char aBuf[BILLINGCHARNUM] = { 0 };//����Ϣ�ַ�������
	int nIndex = 0;//����Ϣ����


	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}

	//���ļ������ж�ȡ
	while (!feof(fp))
	{
		memset(aBuf, 0, BILLINGCHARNUM);//�������
		if (fgets(aBuf, BILLINGCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				//����prassCard(),������ȡ�Ŀ���Ϣ
				pBilling[nIndex] = prassBilling(aBuf);
				nIndex++;
			}
		}
	}
	fclose(fp);
	return TRUE;
}

/*
������:prassBilling
���ܣ��������������ڽ���ÿ���û��˻��Ľṹ��
������pBuf-����������Ϣ���ַ���
����ֵ��Billing-�ַ���ת���ɵ�������Ϣ�ṹ��
*/
Billing prassBilling(char* pBuf)
{
	Billing billing;//����������Ŀ���Ϣ

	const char *delims = "##";//�ַ����еķָ���
	char flag[6][20] = { 0 };//����ָ����ַ�
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
	//����##״̬##�ϻ�ʱ��##�»�ʱ��##���ѽ��##ɾ����ʶ
	//�ָ������ݱ��浽�ṹ����
	strcpy(billing.aCardName, flag[0]);//����
	billing.nStatus = atoi(flag[1]);//״̬
	billing.tStart = stringToTime(flag[2]);//�ϻ�ʱ��
	billing.tEnd = stringToTime(flag[3]);//�»�ʱ��
	billing.fAmount = (float)atof(flag[4]); //���ѽ��
	billing.nDel = atoi(flag[5]);//ɾ�����
	return billing;
}


/*
������: getCardCount
���ܣ���ȡ�ļ����û��˻�������
������const char * pPath���洢�û��˻��ļ�·��
����ֵ��int���ļ��е��û��˻���Ŀ
*/
int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	int nIndex = 0;
	char aBuf[BILLINGCHARNUM] = { 0 };

	if ((fp = fopen(pPath, "rb")) == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		memset(aBuf, 0, BILLINGCHARNUM);//�������
		if (fgets(aBuf, BILLINGCHARNUM, fp) != NULL)
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
int updataBilling(const Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;//�ļ�ָ��
	char aBuf[BILLINGCHARNUM] = { 0 };

	char startTime[TIMELENGTH] = { 0 };//�ϻ�ʱ��
	char endTime[TIMELENGTH] = { 0 };//�»�ʱ��

	int nLine = 0;//�ļ�����
	long IPosition = 0;//�ļ�λ�ñ��

					   //��ʱ��ת�����ַ���
	timeToString(pBilling->tStart, startTime);
	timeToString(pBilling->tEnd, endTime);

	//���ļ������ʧ�ܣ�����FALSE
	fp = fopen(BILLINGPATH, "r+");
	if (fp == NULL)
	{
		return FALSE;
	}

	//�����ļ�,�ҵ�������¼�����и���
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (fgets(aBuf, BILLINGCHARNUM, fp) != NULL)
		{
			//��ȡ�ļ���־λ��
			IPosition = ftell(fp);
			nLine++;
		}
	}
	//�Ƶ��ļ���ʶλ��
	fseek(fp, IPosition, 0);
	//������д���ļ�
	//����##״̬##�ϻ�ʱ��##�»�ʱ��##���ѽ��##ɾ����ʶ
	fprintf(fp, "%s##%d##%s##%s##%0.1f##%d\n",pBilling->aCardName,pBilling->nStatus,startTime,endTime,pBilling->fAmount,pBilling->nDel);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}
