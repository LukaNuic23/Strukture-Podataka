#ifndef ARTICLE_H
#define ARTICLE_H

typedef struct article* ArticlePtr;

// structure representing a single item on a receipt
typedef struct article {
    char name[64];          // item name
    int quantity;           // how many purchased
    double price;           // price per unit
    ArticlePtr next;        // pointer to the next item
} Article;

// inserts an item into a sorted list (sorted by item name)
int InsertArticleSorted(ArticlePtr head, char* name, int quantity, double price);

// frees all items from a list
int FreeAllArticles(ArticlePtr head);

#endif

