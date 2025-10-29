#include <stdio.h>   //biblioteka za ulazno-izlazne funkcije
#include <stdlib.h>  //biblioteka za rad s memorijom i konverzijama
#include <string.h>  //biblioteka za rad s nizovima znakova (stringovima)
#include <ctype.h>   //biblioteka za funkciju toupper() - pretvaranje u veliko slovo

//definicija strukture osoba
typedef struct _person {
    char firstName[50];  //ime osobe
    char lastName[50];   //prezime osobe
    int birthYear;       //godina rodenja osobe
    struct _person* next; //pokazivac na sljedecu osobu u listi
} Person;

//--------------------------- DEKLARACIJE FUNKCIJA -----------------------------

int createPerson(Person** newPerson, char* firstName, char* lastName, int birthYear); //funkcija za stvaranje nove osobe
int addToBeginning(Person* head, char* firstName, char* lastName, int birthYear);     //dodavanje osobe na pocetak liste
int printList(Person* head);                                                          //ispis liste
int addToEnd(Person* head, char* firstName, char* lastName, int birthYear);           //dodavanje osobe na kraj liste
Person* findByLastName(Person* head, char* lastName);                                 //pronalazenje osobe po prezimenu
int deleteByLastName(Person* head, char* lastName);                                   //brisanje osobe po prezimenu
int addAfter(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear); //dodavanje iza odredene osobe
int addBefore(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear); //dodavanje ispred odredene osobe
int sortListByLastName(Person* head);                                                 //sortiranje liste po prezimenima
int writeListToFile(Person* head, char* fileName);                                    //spremanje liste u datoteku
int readListFromFile(Person* head, char* fileName);                                   //ucitavanje liste iz datoteke

//---------------------------- FUNKCIJE ----------------------------------------

//funkcija za stvaranje nove osobe (alokacija memorije i postavljanje podataka)
int createPerson(Person** newPerson, char* firstName, char* lastName, int birthYear) {
    *newPerson = (Person*)malloc(sizeof(Person)); //dinamicka alokacija memorije za novu osobu
    if (*newPerson == NULL) {                    //provjera uspjesnosti alokacije
        printf("Neuspjesna alokacija memorije!\n");
        return -1;                               //ako nije uspjesno vraca -1
    }

    strcpy((*newPerson)->firstName, firstName);  //kopira ime u strukturu
    strcpy((*newPerson)->lastName, lastName);    //kopira prezime u strukturu
    (*newPerson)->birthYear = birthYear;         //postavlja godinu rodenja
    (*newPerson)->next = NULL;                   //inicijalizira pokazivac na NULL
    return 0;                                    //funkcija zavrsava uspjesno
}

//dodavanje osobe na pocetak liste
int addToBeginning(Person* head, char* firstName, char* lastName, int birthYear) {
    Person* newPerson = NULL;                                      //deklarira novi pokazivac
    if (createPerson(&newPerson, firstName, lastName, birthYear))  //poziva funkciju za stvaranje osobe
        return -1;                                                 //ako ne uspije, vraca -1

    newPerson->next = head->next;  //pokazuje na trenutnog prvog clana liste
    head->next = newPerson;        //glava sada pokazuje na novu osobu
    return 0;                      //uspjesno dodano
}

//ispis liste svih osoba
int printList(Person* head) {
    Person* current = head->next;             //postavlja pokazivac na prvi element
    if (current == NULL) {                    //ako je lista prazna
        printf("Greska, lista je prazna!\n");
        return -1;                            //vraca gresku
    }
    while (current != NULL) {                 //petlja prolazi kroz cijelu listu
        printf("Ime: %s, Prezime: %s, Godina rodenja: %d\n",
               current->firstName, current->lastName, current->birthYear); //ispis podataka osobe
        current = current->next;              //pomak na sljedecu osobu
    }
    return 0;                                 //uspjesan ispis
}

//dodavanje osobe na kraj liste
int addToEnd(Person* head, char* firstName, char* lastName, int birthYear) {
    Person* newPerson = NULL;                                      //deklaracija nove osobe
    if (createPerson(&newPerson, firstName, lastName, birthYear))  //stvaranje osobe
        return -1;                                                 //ako nije uspjesno, izlazi

    Person* current = head;                    //postavlja trenutnog na glavu
    while (current->next != NULL)              //prolazi do zadnjeg elementa
        current = current->next;
    current->next = newPerson;                 //dodaje novu osobu na kraj
    return 0;                                  //uspjesno
}

//pronalaženje osobe po prezimenu
Person* findByLastName(Person* head, char* lastName) {
    Person* current = head->next;              //pocetak liste
    while (current != NULL) {                  //prolazi kroz sve osobe
        if (strcmp(current->lastName, lastName) == 0) //ako se prezime podudara
            return current;                    //vraca pokazivac na osobu
        current = current->next;               //pomak na sljedecu
    }
    return NULL;                               //ako nije pronadena
}

//brisanje osobe iz liste prema prezimenu
int deleteByLastName(Person* head, char* lastName) {
    Person* current = head;                    //pokazivac na glavu
    while (current->next != NULL) {            //prolazi kroz listu
        if (strcmp(current->next->lastName, lastName) == 0) { //ako je pronadena osoba
            Person* temp = current->next;      //privremeni pokazivac na osobu koju brisemo
            current->next = temp->next;        //prespaja pokazivace
            free(temp);                        //oslobadja memoriju
            printf("Osoba s prezimenom %s je obrisana.\n", lastName);
            return 0;                          //uspjesno brisanje
        }
        current = current->next;               //pomak dalje
    }
    printf("Osoba s prezimenom %s nije pronadena!\n", lastName);
    return -1;                                 //ako nije pronadena
}

//dodavanje osobe iza odredene osobe prema prezimenu
int addAfter(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear) {
    Person* target = findByLastName(head, targetLastName); //trazi osobu po prezimenu
    if (target == NULL) {                                  //ako nije pronadena
        printf("Osoba s prezimenom %s nije pronadena!\n", targetLastName);
        return -1;                                         //vraca gresku
    }

    Person* newPerson = NULL;                              //deklaracija nove osobe
    if (createPerson(&newPerson, firstName, lastName, birthYear)) //stvaranje osobe
        return -1;

    newPerson->next = target->next;                        //nova osoba pokazuje na sljedecu
    target->next = newPerson;                              //target pokazuje na novu osobu
    printf("Nova osoba je dodana iza osobe s prezimenom %s.\n", targetLastName);
    return 0;                                              //uspjesno
}

//dodavanje osobe ispred odredene osobe prema prezimenu
int addBefore(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear) {
    Person* current = head;                                //pokazivac na glavu
    while (current->next != NULL && strcmp(current->next->lastName, targetLastName) != 0)
        current = current->next;                           //prolazi dok ne nade target

    if (current->next == NULL) {                           //ako nije pronadeno
        printf("Osoba s prezimenom %s nije pronadena!\n", targetLastName);
        return -1;
    }

    Person* newPerson = NULL;                              //nova osoba
    if (createPerson(&newPerson, firstName, lastName, birthYear))
        return -1;

    newPerson->next = current->next;                       //nova osoba pokazuje na pronadenu
    current->next = newPerson;                             //prethodna pokazuje na novu
    printf("Nova osoba je dodana ispred osobe s prezimenom %s.\n", targetLastName);
    return 0;
}

//sortiranje liste po prezimenima (bubble sort)
int sortListByLastName(Person* head) {
    if (head->next == NULL || head->next->next == NULL)    //ako lista ima manje od 2 elementa
        return 0;                                          //nema sto sortirati

    int swapped;                                           //zastavica za pracenje zamjena
    do {
        swapped = 0;                                       //postavlja da nije bilo zamjena
        Person* prev = head;                               //pokazivac na prethodni element
        Person* current = head->next;                      //pokazivac na trenutni element
        while (current->next != NULL) {                    //dok postoji sljedeci element
            if (strcmp(current->lastName, current->next->lastName) > 0) { //usporedjuje prezimena
                Person* tmp = current->next;               //spremi sljedeceg
                current->next = tmp->next;                 //prespajanje pokazivaca
                tmp->next = current;
                prev->next = tmp;
                swapped = 1;                               //bila je zamjena
            }
            prev = prev->next;                             //pomak naprijed
            current = prev->next;                          //pomak naprijed
        }
    } while (swapped);                                     //ponavlja dok ima zamjena

    printf("Lista je sortirana po prezimenima!\n");
    return 0;                                              //uspjesno
}

//spremanje liste u datoteku
int writeListToFile(Person* head, char* fileName) {
    FILE* fp = fopen(fileName, "w");                       //otvaranje datoteke za pisanje
    if (!fp) {                                             //ako nije uspjesno
        printf("Neuspjelo otvaranje datoteke!\n");
        return -1;
    }

    Person* current = head->next;                          //prva osoba u listi
    while (current != NULL) {                              //dok ima osoba
        fprintf(fp, "%s %s %d\n", current->firstName, current->lastName, current->birthYear); //upis u datoteku
        current = current->next;                           //pomak dalje
    }
    fclose(fp);                                            //zatvaranje datoteke
    printf("Lista je uspjesno upisana u datoteku!\n");
    return 0;                                              //uspjesno
}

//ucitavanje liste iz datoteke
int readListFromFile(Person* head, char* fileName) {
    FILE* fp = fopen(fileName, "r");                       //otvaranje datoteke za citanje
    if (!fp) {                                             //ako nije uspjesno
        printf("Neuspjelo otvaranje datoteke!\n");
        return -1;
    }

    char firstName[50];                                    //privremene varijable za citanje
    char lastName[50];
    int birthYear;

    while (fscanf(fp, "%s %s %d", firstName, lastName, &birthYear) == 3) //cit
        addToEnd(head, firstName, lastName, birthYear);   //dodavanje na kraj

    fclose(fp);                                            //zatvaranje datoteke
    printf("Lista je uspjesno procitana iz datoteke!\n");
    return 0;                                              //uspjesno
}



int main() {
    Person head = { .next = NULL };                        //glava liste bez elemenata

    int choice;                                            //varijabla za izbor korisnika
    char firstName[50], lastName[50], targetLastName[50];  //privremene varijable
    int birthYear;                                         //varijabla za godinu rodenja

    do {                                                   //glavna petlja programa
        //izbornik za korisnika
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Dodaj osobu na kraj\n");
        printf("3. Dodaj osobu iza prezimena\n");
        printf("4. Dodaj osobu ispred prezimena\n");
        printf("5. Ispisi listu\n");
        printf("6. Sortiraj po prezimenima\n");
        printf("7. Obrisi osobu po prezimenu\n");
        printf("8. Spremi listu u datoteku\n");
        printf("9. Ucitaj listu iz datoteke\n");
        printf("0. Izlaz\n");
        printf("Odaberi opciju: ");
        scanf("%d", &choice);                              //unos opcije

        switch (choice) {                                  //provjera izbora korisnika
            case 1:
                printf("Unesite ime: ");
                scanf("%s", firstName);
                printf("Unesite prezime: ");
                scanf("%s", lastName);
                printf("Unesite godinu rodenja: ");
                scanf("%d", &birthYear);
                firstName[0] = toupper(firstName[0]);      //prvo slovo veliko
                lastName[0] = toupper(lastName[0]);
                addToBeginning(&head, firstName, lastName, birthYear);
                break;

            case 2:
                printf("Unesite ime: ");
                scanf("%s", firstName);
                printf("Unesite prezime: ");
                scanf("%s", lastName);
                printf("Unesite godinu rodenja: ");
                scanf("%d", &birthYear);
                firstName[0] = toupper(firstName[0]);
                lastName[0] = toupper(lastName[0]);
                addToEnd(&head, firstName, lastName, birthYear);
                break;

            case 3:
                printf("Unesite prezime osobe iza koje zelite dodati: ");
                scanf("%s", targetLastName);
                printf("Unesite ime nove osobe: ");
                scanf("%s", firstName);
                printf("Unesite prezime nove osobe: ");
                scanf("%s", lastName);
                printf("Unesite godinu rodenja: ");
                scanf("%d", &birthYear);
                firstName[0] = toupper(firstName[0]);
                lastName[0] = toupper(lastName[0]);
                addAfter(&head, targetLastName, firstName, lastName, birthYear);
                break;

            case 4:
                printf("Unesite prezime osobe ispred koje zelite dodati: ");
                scanf("%s", targetLastName);
                printf("Unesite ime nove osobe: ");
                scanf("%s", firstName);
                printf("Unesite prezime nove osobe: ");
                scanf("%s", lastName);
                printf("Unesite godinu rodenja: ");
                scanf("%d", &birthYear);
                firstName[0] = toupper(firstName[0]);
                lastName[0] = toupper(lastName[0]);
                addBefore(&head, targetLastName, firstName, lastName, birthYear);
                break;

            case 5:
                printList(&head);
                break;

            case 6:
                sortListByLastName(&head);
                break;

            case 7:
                printf("Unesite prezime osobe koju zelite obrisati: ");
                scanf("%s", targetLastName);
                deleteByLastName(&head, targetLastName);
                break;

            case 8:
                writeListToFile(&head, "osobe.txt");
                break;

            case 9:
                readListFromFile(&head, "osobe.txt");
                break;

            case 0:
                printf("Izlaz iz programa...\n");
                break;

            default:
                printf("Nepoznata opcija!\n");
        }
    } while (choice != 0);                                 //petlja se ponavlja dok korisnik ne unese 0

    return 0;                                              //kraj programa
}
