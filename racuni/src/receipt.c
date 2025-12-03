#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receipt.h"
#include "article.h"


ReceiptPtr CreateReceipt(const char* date)
{
    ReceiptPtr r = (ReceiptPtr)malloc(sizeof(Receipt));
    if (!r)
        return NULL;

    strcpy(r->date, date);
    r->items = NULL;
    r->next = NULL;

    return r;
}


int CompareDates(const char* d1, const char* d2)
{
    return strcmp(d1, d2);
}


int InsertReceiptSorted(ReceiptPtr head, ReceiptPtr newReceipt)
{
    if (!head || !newReceipt)
        return -1;

    ReceiptPtr p = head;

    
    while (p->next != NULL && CompareDates(p->next->date, newReceipt->date) < 0)
    {
        p = p->next;
    }

    newReceipt->next = p->next;
    p->next = newReceipt;

    return 0;
}


int AddItemToReceipt(ReceiptPtr receipt, const char* name, int quantity, double price)
{
    if (!receipt)
        return -1;

    ArticlePtr newItem = CreateArticle(name, quantity, price);
    if (!newItem)
        return -1;

    
    ArticlePtr p = receipt->items;
    ArticlePtr prev = NULL;

    while (p != NULL && strcmp(p->name, name) < 0)
    {
        prev = p;
        p = p->next;
    }

    if (prev == NULL)
    {
        newItem->next = receipt->items;
        receipt->items = newItem;
    }
    else
    {
        newItem->next = prev->next;
        prev->next = newItem;
    }

    return 0;
}
