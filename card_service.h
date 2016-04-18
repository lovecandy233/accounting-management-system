#include "model.h"

#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
//int addCard(Card card);//声明添加卡函数,将结构体指针的数据写入文件
Card* queryCards(const char* pName, int* pIndex);//声明查询卡函数,查询用户账户
Card* queryCard(const char* pName);
void releaseCardList();
int getCard();
int initCardList();
Card* checkCard(const char* pName, const char* pPwd, int *pIndex);
#endif // !CARD_SERVICE_H
