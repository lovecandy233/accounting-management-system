#ifndef BILLING_FILE_H
#define BILLING_FILE_H
int saveBilling(const Billing* pBilling, const char* pPath);//����Ʒ���Ϣ
int getBillingCount(const char* pPath);
Billing prassBilling(char* pBuf);
int updataBilling(const Billing* pBilling, const char* pPath, int nIndex);
int readBilling(Billing* pBilling, const char* pPath);

#endif // !BILLING_FILE_H




