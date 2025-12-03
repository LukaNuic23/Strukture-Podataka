#ifndef FILEIO_H
#define FILEIO_H

#include "receipt.h"


int LoadReceiptsFromFile(const char* fileList, ReceiptPtr head);


ReceiptPtr LoadSingleReceipt(const char* filename);


int ParseItemLine(const char* line, char* name, int* quantity, double* price);

#endif
