#include <stdio.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/analytics.h"
#include "../include/receipt.h"

int Menu(ReceiptPtr head) {
    int choice = -1;
    char item[64], d1[11], d2[11];

    while (choice != 0) {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Potrosnja artikla u razdoblju\n");
        printf("2 - Najskuplji artikl\n");
        printf("3 - Najjeftiniji artikl\n");
        printf("4 - Najvise kupljeni artikl\n");
        printf("5 - Usporedba dva racuna\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int qty = 0;
            double total = 0.0;

            printf("Naziv artikla: ");
            scanf(" %63s", item);
            printf("Pocetni datum: ");
            scanf(" %10s", d1);
            printf("Zavrsni datum: ");
            scanf(" %10s", d2);

            total = TotalSpentOnItemInRange(head, item, d1, d2, &qty);

            printf("Potroseno: %.2f, Kolicina: %d\n", total, qty);
        }

        else if (choice == 2) {
            char name[64];
            if (MostExpensiveItem(head, name, 64) == 0)
                printf("Najskuplji artikl: %s\n", name);
            else
                printf("Nema artikala.\n");
        }

        else if (choice == 3) {
            char name[64];
            if (CheapestItem(head, name, 64) == 0)
                printf("Najjeftiniji artikl: %s\n", name);
            else
                printf("Nema artikala.\n");
        }

        else if (choice == 4) {
            char name[64];
            if (MostPurchasedItem(head, name, 64) == 0)
                printf("Najvise kupljeni artikl: %s\n", name);
            else
                printf("Nema artikala.\n");
        }

        else if (choice == 5) {
            double s1 = 0, s2 = 0;

            printf("Prvi datum: ");
            scanf(" %10s", d1);
            printf("Drugi datum: ");
            scanf(" %10s", d2);

            ReceiptPtr r1 = FindReceiptByDate(head, d1);
            ReceiptPtr r2 = FindReceiptByDate(head, d2);

            if (!r1 || !r2)
                printf("Racuni nisu pronadeni.\n");
            else {
                CompareTwoReceipts(r1, r2, &s1, &s2);
                printf("Vrijednost prvog: %.2f, drugog: %.2f\n", s1, s2);
            }
        }

        else if (choice == 0)
            printf("Izlaz.\n");

        else
            printf("Krivi unos.\n");
    }

    return 0;
}

