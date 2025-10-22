#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person {
    char firstName[50]; //ime osobe 
    char lastName[50]; //prezime osobe
    int birthYear; //godina rodenja
    struct _person* next; //pokazivac na sljedecu osobu na listi
} Person;

//deklaracije funkcija koje ce se koristiti u programu
Person* createPerson(char* firstName, char* lastName, int birthYear); //funkcija za dinamicko stvaranje osobe
int addToBeginning(Person* head, char* firstName, char* lastName, int birthYear); //dodaje osobu na pocetak
int printList(Person* head); //ispisuje listu
int addToEnd(Person* head, char* firstName, char* lastName, int birthYear); // dodaje osobu na kraj liste
Person* findByLastName(Person* head, char* lastName); //pronalazi osobu po prezimenu
int deleteByLastName(Person* head, char* lastName); //brise osobu tako da je nade po prezimenu

int createPerson(Person** newPerson, char* firstName, char* lastName, int birthYear) {
    *newPerson=(Person*)malloc(sizeof(Person)); //alociranje memorije za osobu
    if(newPerson* == NULL){ 
        printf("Neuspjesna alokacija memorije!\n");
        return -1; //provjera je li memorija alocirana
    }
    //kopiranje podataka u strukturu
    strcpy((*newPerson)->firstName, firstName);
    strcpy((*newPerson)->lastName, lastName);
    (*newPerson)->birthYear = birthYear;
    //postavaljanje pokazivaca na iducu osobu na nulu
    (*newPerson)->next = NULL;

    return 0; //sve proslo u redu
}

int addToBeginning(Person* head, char*firstName, char* lastName, int birthYear) {
    Person* newPerson == NULL; //postavljanje pokazivaca na novu osobu
    if(createPerson(&newPerson, firstName, lastName, birthYear) != 0){
        return -1; //provjera za stvaranje nove osobe
    }

    newPerson->next = head->next; //postavljanje pokazivaca od novostvorene osobe na osobu na koju je head prije pokaziva
    head->next = newPerson; //postavljanje head pokazivaca na tu novu osobu

    return 0; //sve proslo u redu
}

int printList(Person* head) {
    Person* current = head->next; //postavlja se pokazivac "trenutno" na prvu osobu tj na onu na koju head pokazivac pokazuje
    if(current == NULL){
        printf("Greska, lista je prazna\n"); //provjera je li lista koju smo uzeli za trenutnu prazna
        return -1;
    }
    while(current != NULL) { //petlja dok god trenutna lista ne bude dosla do kraja lista tj da joj pokazivac pokazuje na nista tj nulu
        printf("Ime: %s, Prezime: %s, Godina rodenja: %d\n", current->firstName, current->lastName, current->birthYear); //printanje liste
        current = current->next; //prelazak na iducu listu tj osobu
    }
    return 0; //sve proslo u redu
}

int addToEnd(Person* head, char* firstName, char* lastName, int birthYear) {
    Person* newPerson = NULL; //postavljanje pokazivaca na nulu

    if(createPerson(&newPerson, firstName, lastName, birthYear) != 0){
        return -1; //greska pri stvaranju osobe
    }

    Person* current = head; //stavljanje trenutnog na pocetak liste
    while(current->next != NULL) { //dok pokazivac od trenutnog ne dode do kraja liste tj pokazuje na nul adresu
        current = current->next; //krece kroz vezane liste
    }

    current->next = newPerson; //kada se petlja izvrti do kraja tj kad pronade zadnji element pokazivac istog postavlja na adresu nove osobe koja je stvara 

    return 0; //sve proslo u redu
}

Person* findByLastName(Person* head, char* lastName) {
    Person* current = head->next; //postavljanje trenutnog na pocetak liste
    while(current != NULL) { //dok
        if(strcmp(current->lastName, lastName) == 0){ //usporedba dva elementa
            return current; //kada pronade vraca sta je pronasa
        }
        current=current->next; //trenutni postaje iduci element
    }

    return NULL; //vraca nulu
}

int deleteByLastName(Person* head, char* lastName) {
    Person* current = head; //postavljanje trenutnog na pocetak

    while(current->next != NULL) { //dok ne dode do zadnjeg elementa
        current = current->next0; //vrti se trenutni do kraja liste(prolazi kroz listu)
    }
    if(current->next != NULL && strcmp(current->next->lastName, lastName) != 0) { //provjera postoji li osoba sa zadanim prezimenom u listi
        printf("Osoba s prezimenom %s nije pronadena\n", lastName);
        return -1; //vraca gresku
    }
    else {
        Person* temp = current->next; //postavljanje privremenog pokazivaca na osobu koja se brise
        current->next = temp->next; //prespajanje pokazivaca
        free(temp); //oslobadanje memorije/brisanje elementa
        
        printf("Osoba s prezimenom %s je obrisana\n", lastName); //ispisuje koga je obrisa
        return 0; //sve proslo u redu
    }
    }

    int main() {
        Person head = { .next = NULL }; //prazna pocetna lista, postavljanje head nexta na null adresu
        
        //dodavanje osoba na pocetak liste
        addToBeginning(&head, "Mijo", "Milas", 2001);
        addToBeginning(&head, "Ivan", "Ivic", 1999);
        addToBeginning(&head, "Ana", "Maric", 1997);
        
        //ispis liste nakon dodavanja na pocetak
        printf("\n--- Lista nakon dodavanja na pocetak ---\n");
        printList(&head);

        //dodavanje osoba na kraj liste
        addToEnd(&head, "Pero", "Peric", 1978);
        addToEnd(&head, "Sara", "Misic", 2003);

        //ispis liste jos nakon dodavanja na kraj
        printf("\n--- Lista nakon dodavanja na kraj ---\n");
        printList(&head);
        
        //inicijalizira se koje cemo prezime traziti da izbrisemo
        char prezime[50] = "Maric";
        Person* found = findByLastName(&head, prezime); //novi pokazivac kojim trazimo osobu i u ciji cemo pokazivac spremit ako oaobu nademo
        if(found != NULL){ //ako pokazivac nije osta prazan tj nasa je osobu
            printf("\nPronadena osoba: %s %s %d", found->firstName, found->lastName, found->birthYear); //ispisivanje osobe kad je pronadena
        }
        else{
            printf("\nOsoba s prezimenom %s nije pronadena", prezime); //nije pronaden nitko s tim prezimenom
        }

        deleteByLastName(&head, "Peric"); //brisanje osobe iz liste

        printf("\n--- Lista nakon brisanja ---\n"); //ispis liste nakon brisanja
        printList(&head);

        return 0; //sve proslo u redu
    }


