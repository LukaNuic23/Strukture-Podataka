#ifndef RECEIPT_H
#define RECEIPT_H

#include "article.h"


typedef struct receipt* ReceiptPtr;


typedef struct receipt {
    char date[11];          
    ArticlePtr items;       
    ReceiptPtr next;        
} Receipt;


ReceiptPtr CreateReceipt(const char* date);


int InsertReceiptSorted(ReceiptPtr head, ReceiptPtr newReceipt);


int CompareDates(const char* d1, const char* d2);


int AddItemToReceipt(ReceiptPtr receipt, const char* name, int quantity, double price);

#endif
