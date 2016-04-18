#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "model.h"
#include "tool.h"

#include <stdio.h>

int saveMoney(const Money* pMoney,const char* pPath)
{
	//����ֵ��Ϣ����
	FILE* fp = NULL;
	char operationTime[TIMELENGTH] = { 0 };//����ʱ��
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
	timeToString(pMoney->tTime, operationTime);

	//������д���ļ�
	//����##����ʱ��##�������##��ֵ�˷ѽ��##ɾ����ʶ
	fprintf(fp, "%s##%s##%d##%0.1f##%d\n",pMoney->aCardName,operationTime,pMoney->nStatus,pMoney->fMoney,pMoney->nDel);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}