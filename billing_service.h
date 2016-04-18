#include "model.h"

#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H
int initBillingList();
void releaseBillingList();
Billing* queryBilling(const char* pName);
Billing* checkBilling(const char* pName, int* pIndex);
int getBilling();
#endif // !BILLING_SERVICE_H

