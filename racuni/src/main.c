#include "../include/menu.h"
#include "../include/files.h"
#include "../include/receipt.h"

int main() {
    ReceiptPtr head = CreateReceiptHead();
    LoadAllReceipts(head, "data/receipts.txt");
    Menu(head);
    DestroyAllReceipts(head);
    return 0;
}
