#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "model.h"
#include "card_service.h"
#include "service.h"
#include "tool.h"
#include "global.h"

int getSize(const char* pInfo);

/*
��������outputMenu
���ܣ����ϵͳ�˵�
������void
����ֵ��void
*/ 
void outputMenu()
{
	printf("\n");
	printf("-------�˵�---------\n");
	printf("1.��ӿ�\n");
	printf("2.��ѯ��\n");
	printf("3.�ϻ�\n");
	printf("4.�»�\n");
	printf("5.��ֵ\n");
	printf("6.�˷�\n");
	printf("7.��ѯͳ��\n");
	printf("8.ע����\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵�����(0~8): ");
}
/*
��������add
1.��ӿ�
����card_service.c�е�addCard() 
*/
void add()
{
	Card card;
	char aName[32] = {'\0'};
	char aPwd[20] = { '\0' };
	int nIndex = 0;

	printf("---------��ӿ�---------\n");

	//��ʾ����������Ŀ���Ϣ
	printf("�����뿨��<����Ϊ1~18>:"); 
	scanf("%s",aName);
	
	//�жϿ����Լ����볤���Ƿ�Ϸ�
	if (getSize(aName) > 18)
	{
		printf("���ų����涨�ĳ���\n");
		return;
	}

	//������Ŀ��ű��浽�ṹ����
	strcpy(card.aName, aName);
	//��ʾ��������������
	printf("����������<����Ϊ1~8>:");
	scanf("%s", aPwd);

	if (getSize(aPwd) > 8) {
		printf("���볬���涨�ĳ���\n");
		return;
	}

	//����������뱣�浽�ṹ����
	strcpy(card.aPwd, aPwd);

	printf("�����뿪�����<RMB>:"); 
	scanf("%f",&card.fBalance);
	card.fTotalUse = card.fBalance;//�ۼƽ��
	card.nDel = 0;//ɾ����־
	card.nUseCount = 0;//ʹ�ô���
	card.nStatus = 0;//��״̬
	card.tStart = card.tEnd = card.tLast = time(NULL);
	card.tEnd = endTime(card.tStart, card.tEnd);

	//����Ѵ�����ͬ���û��� �����
	//printf("\n�û����ѱ�ע�ᣡ��ӿ�ʧ�ܣ�\n");

	if (addCardInfo(card))
	{
	//�������Ϣ
	printf("\n------��ӵĿ���Ϣ����-----\n");
	printf("����\t����\t״̬\t�������\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
	}
}
/*
��������getSize
���ܣ�����ָ��pinfo��ָ���ַ������ַ�����
������pInfo��Ҫ���ַ������ַ���
����ֵ��int�ַ������ַ��ĸ���
*/
int getSize(const char* pInfo)
{
	int nSize = 0;
	while (*(pInfo+nSize) != '\0')
	{
		nSize++;
	}
	return nSize;
} 


/*
2.��ѯ��
��ʾ����������Ŀ���
����card_service.c�е�queryCard()
��ʾ��� 
*/
void query()
{
	char aName[18] = { 0 };//��������Ŀ���
	char aLastTime[TIMELENGTH] = { 0 };
	Card* pCard = NULL;//�����ѯ���Ŀ���Ϣ
	//int nIndex = 0;
	int i = 0;

	printf("----------��ѯ��---------\n");

	//��ʾ����������Ŀ���Ϣ
	printf("�������ѯ�Ŀ���<����Ϊ1~18>:");
	scanf("%s", aName);
	//pCard = queryCards(aName,&nIndex);
	pCard = queryCardInfo(aName);

	if (pCard == NULL)
	{
		printf("û�иÿ�����Ϣ\n");
	}
	else
	{
		printf("\n����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
		//for (i = 0; i < nIndex; i++)
		//{	//��ʾ
			timeToString(pCard[i].tLast, aLastTime);
			printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t\t%s\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
		//}	
		free(pCard);
		pCard = NULL;
	}
} 
 
void exitApp() 
{
	releaseList();
}

/*
3.�ϻ�
���ܣ���ʾ�������û������ϻ��Ŀ��ź����룬����ѯ�����ϻ�����Ϣ�����ʾ
*/
void logon()
{
	char aName[18] = { 0 };//�ϻ�����
	char aPwd[8] = { 0 };//�ϻ�������
	char lastTime[TIMELENGTH] = { 0 };//���ʹ��ʱ��
	int nResult = 0;//�����ϻ����
	LogonInfo* pInfo = NULL;

	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

	//��ʾ�û����뿨�ź�����

	printf("----------�ϻ���---------\n");
	printf("�������ϻ��Ŀ���<����Ϊ1~18>:");
	scanf("%s", aName);
	printf("�������ϻ�������<����Ϊ1~8>:");
	scanf("%s", aPwd);

	//��ʼ�ϻ�����ȡ�ϻ����,��ʾ��ͬ��Ϣ
	nResult = doLogon(aName, aPwd, pInfo);//��ʼ�ϻ�����ȡ�ϻ����
	switch (nResult)
	{
	case 0: {
		printf("\n�ϻ�ʧ�ܣ�\n");
		break;
	}
	case 1: {
		//��ʱ��ת�����ַ���
		timeToString(pInfo->tLogon, lastTime);
		printf("---------�ϻ���Ϣ����---------\n");
		printf("����\t���\t�ϻ�ʱ��\n");
		printf("%s\t%0.1f\t%s\n", pInfo->aCardName, pInfo->fBalance, lastTime);
		free(pInfo);//�ͷ��ϻ���Ϣ
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n�ÿ�����ʹ�û�����ע��/��ʧЧ��\n");
		break;
	}
	case 3: {
		printf("\n�����㣡\n");
		break;
	default:
		break;
	}
	}	
}

/*
4.�»�
*/
void settle()
{
	char aName[18] = { 0 };//�»�����
	char aPwd[8] = { 0 };//�»�������
	char startTime[TIMELENGTH] = { 0 };//�ϻ�ʱ��
	char endTime[TIMELENGTH] = { 0 };//�»�ʱ��
	int nResult = -1;//�»����
	SettleInfo* pInfo = NULL;

	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//��ʾ�û����뿨�ź�����

	printf("----------�»���---------\n");
	printf("�������»��Ŀ���<����Ϊ1~18>:");
	scanf("%s", aName);
	printf("�������»�������<����Ϊ1~8>:");
	scanf("%s", aPwd);

	nResult = doSettle(aName, aPwd, pInfo);//��ʼ�»�����ȡ�»����
	switch (nResult)
	{
	case 0: {
		printf("\n�»�ʧ�ܣ�\n");
		break;
	}
	case 1: {
		//��ʱ��ת�����ַ���
		timeToString(pInfo->tStart, startTime);
		timeToString(pInfo->tEnd, endTime);
		printf("---------�»���Ϣ����---------\n");
		printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, startTime, endTime);
		free(pInfo);//�ͷ��»���Ϣ
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n�ÿ�δ�ϻ�/��ע��/��ʧЧ��\n");
		break;
	}
	case 3: {
		printf("\n�����㣡�»�ʧ�ܣ�\n");
		break;
	}
	default:
		break;
	}
}

/*
5.��ֵ
���ܣ���ʾ�������û������ϻ��Ŀ��ź����뼰����ʾ��ֵ�����Ϣ
*/
void addMoney()
{
	char aName[18] = { 0 };//�ϻ�����
	char aPwd[8] = { 0 };//�ϻ�������
	float aMoney = 0;
	int nResult = 0;//�����ֵ���
	MoneyInfo* pInfo = NULL;
	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	//��ʾ�û����뿨�ź�����
	printf("----------��ֵ---------\n");
	printf("�������ֵ����<����Ϊ1~18>:");
	scanf("%s", aName);
	printf("�������ֵ����<����Ϊ1~8>:");
	scanf("%s", aPwd);
	printf("�������ֵ���<RMB>:");
	scanf("%f", &aMoney);

	pInfo->fMoney = aMoney;
	strcpy(pInfo->aCardName, aName);
	//��ʼ��ֵ����ȡ���,��ʾ��ͬ��Ϣ
	nResult = doAddMoney(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0: {
		printf("\n��ֵʧ�ܣ�\n");
		break;
	}
	case 1: {
		printf("---------��ֵ��Ϣ����---------\n");
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%0.1f\t\t%0.1f\n",pInfo->aCardName,pInfo->fMoney,pInfo->fBalance);
		free(pInfo);//�ͷ��»���Ϣ
		pInfo = NULL;
		break;
	}
	default:
		break;
	}
}

/*
���ܣ��˷�
*/
void refundMoney()
{
	char aName[18] = { 0 };//�ϻ�����
	char aPwd[8] = { 0 };//�ϻ�������
	float aMoney = 0;
	int nResult = 0;//�����ֵ���
	MoneyInfo* pInfo = NULL;
	pInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));

	//��ʾ�û����뿨�ź�����
	printf("----------��ֵ---------\n");
	printf("�������˷ѿ���<����Ϊ1~18>:");
	scanf("%s", aName);
	printf("�������˷�����<����Ϊ1~8>:");
	scanf("%s", aPwd);

	strcpy(pInfo->aCardName, aName);
	//��ʼ��ֵ����ȡ���,��ʾ��ͬ��Ϣ
	nResult = doRefundMoney(aName, aPwd, pInfo);
	switch (nResult)
	{
	case 0: {
		printf("\n�˷�ʧ�ܣ�\n");
		break;
	}
	case 1: {
		printf("---------�˷���Ϣ����---------\n");
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%0.1f\t%0.1f\n", pInfo->aCardName, pInfo->fMoney, pInfo->fBalance);
		free(pInfo);//�ͷ��»���Ϣ
		pInfo = NULL;
		break;
	}
	case 2: {
		printf("\n�ÿ�����ʹ�û���ʧЧ��\n");
		break;
	}
	case 3: {
		printf("\n�����㣡\n");
		break;
	}
	default:
		break;
	}
}

/*
8.ע��
*/
void annul()
{
	char aName[18] = { 0 };//ע������
	char aPwd[8] = { 0 };//ע��������
	int nResult = 0;//����ע�����
	float fBalance = 0;//�����˿���
	Card aCard;

	//��ʾ�û����뿨�ź�����
	printf("----------ע��---------\n");
	printf("������ע������<����Ϊ1~18>:");
	scanf("%s", aName);
	printf("������ע������<����Ϊ1~8>:");
	scanf("%s", aPwd);

	strcpy(aCard.aName, aName);
	strcpy(aCard.aPwd, aPwd);
	//��ȡע�����,��ʾ��ͬ��Ϣ
	nResult = doAnnul(&aCard,&fBalance);
	switch (nResult)
	{
	case 0: {
		printf("\nע����ʧ�ܣ�\n");
		break;
	}
	case 1: {
		printf("---------ע����Ϣ����---------\n");
		printf("����\t�˿���\n");
		printf("%s\t%0.1f\n",aCard.aName,fBalance);
		break;
	}
	case 2: {
		printf("\n�ÿ�����ʹ�û�/��ע��/��ʧЧ��\n");
		break;
	}
	default:
		break;
	}
}