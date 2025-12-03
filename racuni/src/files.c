#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "receipt.h"
#include "article.h"


int LoadReceiptsFromFile(const char* fileList, ReceiptPtr head)
{
    FILE* fp = fopen(fileList, "r");
    if (!fp)
    {
        printf("Greska: ne mogu otvoriti datoteku %s\n", fileList);
        return -1;
    }

    char filename[128];

    while (fgets(filename, sizeof(filename), fp))
    {
        filename[strcspn(filename, "\n")] = '\0';  

        ReceiptPtr r = LoadSingleReceipt(filename);
        if (!r)
        {
            printf("Greska pri ucitavanju racuna %s\n", filename);
            continue;
        }

        InsertReceiptSorted(head, r);
    }

    fclose(fp);
    return 0;
}


ReceiptPtr LoadSingleReceipt(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Greska: ne mogu otvoriti racun %s\n", filename);
        return NULL;
    }

    char date[32];
    if (!fgets(date, sizeof(date), fp))
    {
        fclose(fp);
        return NULL;
    }

    date[strcspn(date, "\n")] = '\0';  

    ReceiptPtr r = CreateReceipt(date);
    if (!r)
    {
        fclose(fp);
        return NULL;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        char itemName[64];
        int quantity = 0;
        double price = 0.0;

        if (ParseItemLine(line, itemName, &quantity, &price) == 0)
        {
            AddItemToReceipt(r, itemName, quantity, price);
        }
    }

    fclose(fp);
    return r;
}


int ParseItemLine(const char* line, char* name, int* quantity, double* price)
{
    char tempLine[256];
    strcpy(tempLine, line);

    char* token = strtok(tempLine, ",");
    if (!token)
        return -1;

    strcpy(name, token);

    token = strtok(NULL, ",");
    if (!token)
        return -1;
    *quantity = atoi(token);

    token = strtok(NULL, ",");
    if (!token)
        return -1;
    *price = atof(token);

    return 0;
}
