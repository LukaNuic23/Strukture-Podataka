#include <stdio.h>   //biblioteka za ulazno-izlazne funkcije
#include <stdlib.h>  //biblioteka za rad s memorijom
#include <string.h>  //biblioteka za rad sa stringovima
#include <ctype.h>   //biblioteka za provjere znakova (isdigit itd.)

//definicija strukture za stog (stack)
typedef struct _stack {
    double value;          //vrijednost koju spremamo u stog (broj)
    struct _stack* next;   //pokazivac na sljedeci element u stogu
} Stack;



int push(Stack* head, double value);      //dodaje novi element na vrh stoga
int pop(Stack* head, double* result);     //skida element sa stoga i sprema u result
int readPostfixFromFile(char* fileName, char* buffer, int bufferSize); //ucitava postfix izraz iz datoteke
int calculatePostfix(Stack* head, char* expression, double* result);   //racuna rezultat postfix izraza
int performOperation(double a, double b, char operator, double* result); //obavlja racunsku operaciju



//funkcija za dodavanje vrijednosti na stog
int push(Stack* head, double value) {
    Stack* newElement = (Stack*)malloc(sizeof(Stack)); //alokacija memorije za novi element
    if (!newElement) {                                 //provjera je li uspjelo
        printf("Neuspjesna alokacija memorije!\n");
        return -1; //greska
    }

    newElement->value = value; //postavljanje vrijednosti u novi element
    newElement->next = head->next; //novi element pokazuje na ono na sto je head pokazivao
    head->next = newElement; //head sad pokazuje na novi element

    return 0; //sve ok
}

//funkcija za skidanje elementa sa stoga
int pop(Stack* head, double* result) {
    if (head->next == NULL) { //ako je stog prazan
        printf("Greska: stog je prazan!\n");
        return -1;
    }

    Stack* temp = head->next;   //privremeni pokazivac na prvi element
    *result = temp->value;      //sprema vrijednost iz stoga u result
    head->next = temp->next;    //head pokazuje na sljedeci element
    free(temp);                 //oslobadja memoriju

    return 0; //uspjesno skinuto
}

//funkcija za ucitavanje postfix izraza iz datoteke
int readPostfixFromFile(char* fileName, char* buffer, int bufferSize) {
    FILE* fp = fopen(fileName, "r"); //otvaranje datoteke za citanje
    if (!fp) {                       //ako nije uspjesno
        printf("Ne mogu otvoriti datoteku!\n");
        return -1;
    }

    fgets(buffer, bufferSize, fp);   //ucitava cijeli redak (postfix izraz)
    fclose(fp);                      //zatvara datoteku

    return 0; //uspjesno
}

//funkcija koja izvrsava racunske operacije (+, -, *, /)
int performOperation(double a, double b, char operator, double* result) {
    switch (operator) { //provjerava koji operator je u pitanju
        case '+':
            *result = a + b; //sabiranje
            break;
        case '-':
            *result = a - b; //oduzimanje
            break;
        case '*':
            *result = a * b; //mnozenje
            break;
        case '/':
            if (b == 0) { //provjera dijeljenja s nulom
                printf("Greska: dijeljenje s nulom!\n");
                return -1;
            }
            *result = a / b; //dijeljenje
            break;
        default:
            printf("Nepoznat operator: %c\n", operator);
            return -1;
    }
    return 0; //uspjesno
}

//funkcija koja racuna postfix izraz pomocu stoga
int calculatePostfix(Stack* head, char* expression, double* result) {
    char* token = strtok(expression, " "); //uzima prvi element (odvojen razmakom)
    double num1, num2, tempResult;         //pomocne varijable

    while (token != NULL) { //petlja dok ima tokena u izrazu
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            //ako je token broj (pozitivan ili negativan)
            push(head, atof(token)); //pretvara string u broj i stavlja ga na stog
        } 
        else {
            //ako nije broj, mora biti operator
            if (pop(head, &num2) != 0) return -1; //skida drugi broj sa stoga
            if (pop(head, &num1) != 0) return -1; //skida prvi broj sa stoga
            if (performOperation(num1, num2, token[0], &tempResult) != 0) return -1; //racuna
            push(head, tempResult); //rezultat gura natrag na stog
        }
        token = strtok(NULL, " "); //uzima sljedeci token
    }

    //nakon sto se sve obradi, u stogu mora ostat samo jedan rezultat
    if (pop(head, result) != 0) return -1; //uzima konacni rezultat

    if (head->next != NULL) { //ako jos nesto ima na stogu - greska
        printf("Greska: izraz nije ispravan!\n");
        return -1;
    }

    return 0; //uspjesno
}



int main() {
    Stack head = { .next = NULL }; //prazan stog
    char fileName[50] = "postfix.txt"; //ime datoteke iz koje se cita
    char buffer[1024]; //spremnik za izraz
    double result; //varijabla za rezultat

    if (readPostfixFromFile(fileName, buffer, sizeof(buffer)) != 0) { //ucitavanje izraza
        printf("Neuspjelo citanje datoteke!\n");
        return -1;
    }

    printf("Ucitani postfix izraz: %s\n", buffer); //ispis izraza koji se cita

    if (calculatePostfix(&head, buffer, &result) == 0) { //racunanje izraza
        printf("Rezultat izraza je: %.2lf\n", result); //ispis rezultata
    } else {
        printf("Dogodila se greska pri racunanju!\n"); //ako nesto nije ok
    }

    return 0; //kraj programa
}
