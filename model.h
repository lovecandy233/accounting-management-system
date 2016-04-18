#ifndef MODEL_H
#define MODEL_H

#include <time.h>

//���忨��Ϣget
typedef struct Card
{
	char aName[18];//����
	char aPwd[8];//����
	int nStatus;//��״̬��0-δʹ�ã�1-����ʹ�ã�2-��ע����3-ʧЧ��
	time_t tStart;//����ʱ��
	time_t tEnd;//���Ľ�ֹʱ��
	float fTotalUse;//�ۼƽ��
	time_t tLast;//���ʹ��ʱ��
	int nUseCount;//ʹ�ô���
	float fBalance;//���
	int nDel;//ɾ����־ 0-δɾ����1-ɾ��
}Card;

//�˻���Ϣ��������get
typedef struct CardNode
{
	Card data;//������������û��˻�
	struct  CardNode* next;//ָ����һ������ָ��
}CardNode,*IpCardNode;//�û��˻���Ϣ��������

//�Ʒ���Ϣ�ṹ��get
typedef struct Billing
{
	char aCardName[18];//����
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	float fAmount;//���ѽ��
	int nStatus;//��״̬(0-δ���㣻1-�ѽ���)
	int nDel;//ɾ����־ 0-δɾ����1-ɾ��
}Billing;

//�Ʒ���Ϣ�ڵ�
typedef struct BillingNode
{
	Billing data;
	struct BillingNode *next;
}BillingNode,*IpBillingNode;

//�ϻ���Ϣ�ṹ��
typedef struct LogonInfo
{
	char aCardName[18];//�ϻ�����
	time_t tLogon;//�ϻ�ʱ��
	float fBalance;//�ϻ�ʱ�Ŀ����
}LogonInfo;

//�»��ṹ�嶨��
typedef struct SettleInfo
{
	char aCardName[18];
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	float fAmount;//���ѽ��
	float fBalance;//���
}SettleInfo;

//��ֵ�˷���Ϣ�ṹ��
typedef struct Money
{
	char aCardName[18];//����
	time_t tTime;//����ʱ��
	int nStatus;//�������0-��ֵ��1-�˷ѣ�
	float fMoney;//��ֵ�˷����
	int nDel;//ɾ����־ 0-δɾ����1-ɾ��
}Money;

//��ֵ�˷���Ϣ�ṹ��
typedef struct MoneyInfo
{
	char aCardName[18];
	float fMoney;
	float fBalance;//���
}MoneyInfo;

//����Ա��Ϣ
typedef struct Administrator
{
	char name[18];//�û���
	char pwd[8];//����
	int privilge;//Ȩ�޵ȼ�
	int del;//ɾ����־ 0-δɾ����1-ɾ��
}Administrator;

#endif // !MODEL_H

