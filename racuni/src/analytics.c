#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/analytics.h"
#include "../include/receipt.h"
#include "../include/article.h"


static int DateInRange(char* date, char* from, char* to) {
    if (strcmp(date, from) < 0)
        return 0;
    if (strcmp(date, to) > 0)
        return 0;
    return 1;
}


double TotalSpentOnItemInRange(
    ReceiptPtr head,
    char* itemName,
    char* dateFrom,
    char* dateTo,
    int* totalQuantity) {

    double total = 0.0;
    *totalQuantity = 0;

    ReceiptPtr r = head->next;

    while (r != NULL) {
        if (DateInRange(r->date, dateFrom, dateTo)) {

            ArticlePtr a = r->items->next;

            while (a != NULL) {
                if (strcmp(a->name, itemName) == 0) {

                    total += a->price * a->quantity;
                    *totalQuantity += a->quantity;
                }
                a = a->next;
            }
        }
        r = r->next;
    }

    return total;
}


int MostExpensiveItem(ReceiptPtr head, char* resultName, int maxLen) {
    double maxPrice = -1.0;
    char best[64] = "";

    ReceiptPtr r = head->next;

    while (r != NULL) {
        ArticlePtr a = r->items->next;

        while (a != NULL) {
            if (a->price > maxPrice) {
                maxPrice = a->price;
                strcpy(best, a->name);
            }
            a = a->next;
        }
        r = r->next;
    }

    if (maxPrice < 0.0)
        return -1;

    strncpy(resultName, best, maxLen - 1);
    resultName[maxLen - 1] = '\0';

    return 0;
}


int CheapestItem(ReceiptPtr head, char* resultName, int maxLen) {
    double minPrice = -1.0;
    char best[64] = "";

    ReceiptPtr r = head->next;

    while (r != NULL) {
        ArticlePtr a = r->items->next;

        while (a != NULL) {
            if (minPrice < 0.0 || a->price < minPrice) {
                minPrice = a->price;
                strcpy(best, a->name);
            }
            a = a->next;
        }
        r = r->next;
    }

    if (minPrice < 0.0)
        return -1;

    strncpy(resultName, best, maxLen - 1);
    resultName[maxLen - 1] = '\0';

    return 0;
}


static int TotalQuantityForItem(ReceiptPtr head, char* name) {
    int sum = 0;
    ReceiptPtr r = head->next;

    while (r != NULL) {
        ArticlePtr a = r->items->next;

        while (a != NULL) {
            if (strcmp(a->name, name) == 0)
                sum += a->quantity;

            a = a->next;
        }
        r = r->next;
    }

    return sum;
}


int MostPurchasedItem(ReceiptPtr head, char* resultName, int maxLen) {
    int maxQ = -1;
    char best[64] = "";

    ReceiptPtr r = head->next;

    while (r != NULL) {
        ArticlePtr a = r->items->next;

        while (a != NULL) {
            int q = TotalQuantityForItem(head, a->name);
            if (q > maxQ) {
                maxQ = q;
                strcpy(best, a->name);
            }
            a = a->next;
        }
        r = r->next;
    }

    if (maxQ < 0)
        return -1;

    strncpy(resultName, best, maxLen - 1);
    resultName[maxLen - 1] = '\0';

    return 0;
}


static double TotalValueOfReceipt(ReceiptPtr r) {
    double s = 0.0;
    ArticlePtr a = r->items->next;

    while (a != NULL) {
        s += a->price * a->quantity;
        a = a->next;
    }

    return s;
}


int CompareTwoReceipts(ReceiptPtr r1, ReceiptPtr r2, double* sum1, double* sum2) {
    if (!r1 || !r2)
        return -1;

    *sum1 = TotalValueOfReceipt(r1);
    *sum2 = TotalValueOfReceipt(r2);

    return 0;
}
