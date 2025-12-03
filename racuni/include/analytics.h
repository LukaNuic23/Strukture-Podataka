#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "receipt.h"

// calculates total spent and quantity purchased for a specific item in a date range
double TotalSpentOnItemInRange(
    ReceiptPtr head,
    char* itemName,
    char* dateFrom,
    char* dateTo,
    int* totalQuantity
);

// finds the most expensive item (by price)
int MostExpensiveItem(ReceiptPtr head, char* resultName, int maxLen);

// finds the cheapest item (by price)
int CheapestItem(ReceiptPtr head, char* resultName, int maxLen);

// finds the most purchased item (by total quantity across all receipts)
int MostPurchasedItem(ReceiptPtr head, char* resultName, int maxLen);

// compares two receipts by total sum
int CompareTwoReceipts(ReceiptPtr r1, ReceiptPtr r2, double* sum1, double* sum2);

#endif
