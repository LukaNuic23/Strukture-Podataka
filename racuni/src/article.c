#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/article.h"

// inserts article in sorted order by name
int InsertArticleSorted(ArticlePtr head, char* name, int quantity, double price) {
    ArticlePtr prev = head;
    ArticlePtr temp = NULL;

    while (prev->next != NULL && strcmp(prev->next->name, name) < 0)
        prev = prev->next;

    temp = malloc(sizeof(Article));
    if (!temp) {
        printf("Greska: neuspjela alokacija artikla!\n");
        return -1;
    }

    strcpy(temp->name, name);
    temp->quantity = quantity;
    temp->price = price;

    temp->next = prev->next;
    prev->next = temp;

    return 0;
}

// frees entire article list
int FreeAllArticles(ArticlePtr head) {
    ArticlePtr temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return 0;
}