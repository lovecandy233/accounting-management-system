#include "model.h"

#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
//int addCard(Card card);//������ӿ�����,���ṹ��ָ�������д���ļ�
Card* queryCards(const char* pName, int* pIndex);//������ѯ������,��ѯ�û��˻�
Card* queryCard(const char* pName);
void releaseCardList();
int getCard();
int initCardList();
Card* checkCard(const char* pName, const char* pPwd, int *pIndex);
#endif // !CARD_SERVICE_H
