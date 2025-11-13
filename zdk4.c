#include <stdio.h>   //biblioteka za ulazno-izlazne funkcije
#include <stdlib.h>  //biblioteka za rad s memorijom
#include <string.h>  //biblioteka za rad sa stringovima

//definicija strukture za clan polinoma
typedef struct _term {
    int coef;             //koeficijent
    int exp;              //eksponent
    struct _term* next;   //pokazivac na sljedeci clan
} Term;

//---------------------- deklaracije funkcija ----------------------

int insertSorted(Term* head, int coef, int exp);         //dodaje clan sortiran po eksponentu
int readPolynomialFromFile(FILE* fp, Term* head);        //ucitava jedan polinom iz datoteke
int printPolynomial(Term* head);                         //ispisuje polinom
int addPolynomials(Term* p1, Term* p2, Term* result);    //zbrajanje polinoma
int multiplyPolynomials(Term* p1, Term* p2, Term* result);//mnozenje polinoma
int freePolynomial(Term* head);                          //oslobada memoriju

//-------------------------- MAIN FUNKCIJA --------------------------

int main() {
    Term poly1 = { .next = NULL };   //prvi polinom
    Term poly2 = { .next = NULL };   //drugi polinom
    Term sum = { .next = NULL };     //rezultat zbrajanja
    Term product = { .next = NULL }; //rezultat mnozenja

    FILE* fp = fopen("polinomi.txt", "r"); //otvaranje datoteke
    if (!fp) {
        printf("Ne mogu otvoriti datoteku!\n");
        return -1;
    }

    readPolynomialFromFile(fp, &poly1); //ucitavanje prvog polinoma
    readPolynomialFromFile(fp, &poly2); //ucitavanje drugog polinoma
    fclose(fp);                         //zatvaranje datoteke

    printf("Prvi polinom: ");
    printPolynomial(&poly1);            //ispis prvog

    printf("Drugi polinom: ");
    printPolynomial(&poly2);            //ispis drugog

    addPolynomials(&poly1, &poly2, &sum);       //zbrajanje
    multiplyPolynomials(&poly1, &poly2, &product); //mnozenje

    printf("\nZbroj polinoma: ");
    printPolynomial(&sum);                      //ispis zbroja

    printf("Umnozak polinoma: ");
    printPolynomial(&product);                  //ispis umnoska

    //oslobadanje memorije
    freePolynomial(&poly1);
    freePolynomial(&poly2);
    freePolynomial(&sum);
    freePolynomial(&product);

    return 0; //kraj programa
}

//---------------------- implementacija funkcija ------------------

//dodaje clan sortiran po eksponentu (ako vec postoji isti exp, zbraja koef.)
int insertSorted(Term* head, int coef, int exp) {
    Term* newTerm = NULL;                           //pokazivac za novi clan
    Term* current = head;                           //trenutni pokazivac

    while (current->next != NULL && current->next->exp > exp) //trazi poziciju po eksponentu (od veceg prema manjem)
        current = current->next;

    if (current->next != NULL && current->next->exp == exp) { //ako vec postoji isti eksponent
        current->next->coef += coef;                         //zbroji koeficijente
        if (current->next->coef == 0) {                      //ako postane nula, obrisi clan
            Term* temp = current->next;
            current->next = temp->next;
            free(temp);
        }
        return 0;
    }

    newTerm = (Term*)malloc(sizeof(Term));                   //alokacija memorije
    if (!newTerm) {
        printf("Neuspjesna alokacija memorije!\n");
        return -1;
    }

    newTerm->coef = coef;                                    //postavlja koeficijent
    newTerm->exp = exp;                                      //postavlja eksponent
    newTerm->next = current->next;                           //spaja novi clan
    current->next = newTerm;                                 //povezuje ga s listom

    return 0;                                                //uspjesno
}

//ucitavanje jednog polinoma iz datoteke
int readPolynomialFromFile(FILE* fp, Term* head) {
    int coef, exp;                           //privremene varijable
    char line[1024];                         //buffer za red
    if (!fgets(line, 1024, fp))              //ako nema vise redaka
        return -1;

    char* ptr = line;                        //pokazivac za citanje
    while (sscanf(ptr, "%d %d", &coef, &exp) == 2) { //citamo parove (koef, exp)
        insertSorted(head, coef, exp);       //ubacujemo sortiran clan
        while (*ptr && *ptr != ' ') ptr++;   //preskoci do razmaka
        while (*ptr == ' ') ptr++;           //preskoci razmake
        while (*ptr && *ptr != ' ') ptr++;   //preskoci exponent
        while (*ptr == ' ') ptr++;           //preskoci razmake
    }

    return 0; //uspjesno
}

//ispis polinoma
int printPolynomial(Term* head) {
    Term* current = head->next;               //pocetak liste
    if (!current) {                           //ako je prazan
        printf("0\n");
        return 0;
    }

    while (current != NULL) {                 //prolazi kroz listu
        if (current->coef > 0 && current != head->next)
            printf(" + ");                    //ispis plusa ako nije prvi clan
        if (current->coef < 0)
            printf(" - ");                    //ispis minusa
        printf("%d", abs(current->coef));     //ispis koeficijenta
        if (current->exp != 0)
            printf("x^%d", current->exp);     //ispis eksponenta ako nije 0
        current = current->next;              //sljedeci clan
    }
    printf("\n");
    return 0;
}

//zbrajanje polinoma
int addPolynomials(Term* p1, Term* p2, Term* result) {
    Term* t1 = p1->next;                       //pokazivac na prvi polinom
    Term* t2 = p2->next;                       //pokazivac na drugi

    while (t1 != NULL) {                       //prolazi kroz prvi
        insertSorted(result, t1->coef, t1->exp);
        t1 = t1->next;
    }
    while (t2 != NULL) {                       //prolazi kroz drugi
        insertSorted(result, t2->coef, t2->exp);
        t2 = t2->next;
    }
    return 0;                                  //uspjesno
}

//mnozenje polinoma
int multiplyPolynomials(Term* p1, Term* p2, Term* result) {
    Term* t1 = p1->next;                       //prvi polinom
    Term* t2 = NULL;                           //drugi polinom

    while (t1 != NULL) {                       //prolazi kroz prvi
        t2 = p2->next;                         //vrati se na pocetak drugog
        while (t2 != NULL) {                   //prolazi kroz drugi
            int newCoef = t1->coef * t2->coef; //mnozi koeficijente
            int newExp = t1->exp + t2->exp;    //zbraja eksponente
            insertSorted(result, newCoef, newExp); //dodaje u rezultat
            t2 = t2->next;
        }
        t1 = t1->next;
    }
    return 0;                                  //uspjesno
}

//oslobadanje memorije
int freePolynomial(Term* head) {
    Term* temp = NULL;
    while (head->next != NULL) {               //dok ima clanova
        temp = head->next;                     //pokazivac na prvi clan
        head->next = temp->next;               //preskoci ga
        free(temp);                            //oslobodi memoriju
    }
    return 0;
}
