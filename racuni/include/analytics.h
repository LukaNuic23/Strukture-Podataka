#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "receipt.h"


double TotalSpentOnItemInRange(
    ReceiptPtr head,
    char* itemName,
    char* dateFrom,
    char* dateTo,
    int* totalQuantity
);


int MostExpensiveItem(ReceiptPtr head, char* resultName, int maxLen);


int CheapestItem(ReceiptPtr head, char* resultName, int maxLen);


int MostPurchasedItem(ReceiptPtr head, char* resultName, int maxLen);


int CompareTwoReceipts(ReceiptPtr r1, ReceiptPtr r2, double* sum1, double* sum2);

#endif
