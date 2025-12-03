#ifndef ARTICLE_H
#define ARTICLE_H

typedef struct article* ArticlePtr;


typedef struct article {
    char name[64];          
    int quantity;           
    double price;           
    ArticlePtr next;        
} Article;


int InsertArticleSorted(ArticlePtr head, char* name, int quantity, double price);


int FreeAllArticles(ArticlePtr head);

#endif

