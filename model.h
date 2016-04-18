#ifndef MODEL_H
#define MODEL_H

#include <time.h>

//定义卡信息get
typedef struct Card
{
	char aName[18];//卡号
	char aPwd[8];//密码
	int nStatus;//卡状态（0-未使用；1-正在使用；2-已注销；3-失效）
	time_t tStart;//开卡时间
	time_t tEnd;//卡的截止时间
	float fTotalUse;//累计金额
	time_t tLast;//最后使用时间
	int nUseCount;//使用次数
	float fBalance;//余额
	int nDel;//删除标志 0-未删除，1-删除
}Card;

//账户信息的链表结点get
typedef struct CardNode
{
	Card data;//结点数据区—用户账户
	struct  CardNode* next;//指向下一个结点的指针
}CardNode,*IpCardNode;//用户账户信息的链表结点

//计费信息结构体get
typedef struct Billing
{
	char aCardName[18];//卡号
	time_t tStart;//上机时间
	time_t tEnd;//下机时间
	float fAmount;//消费金额
	int nStatus;//卡状态(0-未结算；1-已结算)
	int nDel;//删除标志 0-未删除，1-删除
}Billing;

//计费信息节点
typedef struct BillingNode
{
	Billing data;
	struct BillingNode *next;
}BillingNode,*IpBillingNode;

//上机信息结构体
typedef struct LogonInfo
{
	char aCardName[18];//上机卡号
	time_t tLogon;//上机时间
	float fBalance;//上机时的卡余额
}LogonInfo;

//下机结构体定义
typedef struct SettleInfo
{
	char aCardName[18];
	time_t tStart;//上机时间
	time_t tEnd;//下机时间
	float fAmount;//消费金额
	float fBalance;//余额
}SettleInfo;

//充值退费信息结构体
typedef struct Money
{
	char aCardName[18];//卡号
	time_t tTime;//操作时间
	int nStatus;//操作类别（0-充值；1-退费）
	float fMoney;//充值退费余额
	int nDel;//删除标志 0-未删除，1-删除
}Money;

//充值退费信息结构体
typedef struct MoneyInfo
{
	char aCardName[18];
	float fMoney;
	float fBalance;//余额
}MoneyInfo;

//管理员信息
typedef struct Administrator
{
	char name[18];//用户名
	char pwd[8];//密码
	int privilge;//权限等级
	int del;//删除标志 0-未删除，1-删除
}Administrator;

#endif // !MODEL_H

