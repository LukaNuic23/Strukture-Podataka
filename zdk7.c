#include <stdio.h>      // standardni ulaz/izlaz
#include <stdlib.h>     // malloc, free, exit
#include <string.h>     // strcpy, strcmp, strncpy

/* definicija strukture direktorija */
typedef struct directory {
    char name[64];              // ime direktorija
    struct directory* firstChild; // pokazivac na prvi poddirektorij
    struct directory* nextSibling; // pokazivac na sljedeci direktorij na istoj razini
    struct directory* parent;     // pokazivac na roditelja (NULL za root)
} Directory;

/* deklaracije funkcija (prototipovi) */
/* svi prototipi vracaju int: 0 uspjeh, negativno = greska */

/* kreira novu strukturu direktorija i vraca pokazivac preko out parametra */
int CreateDir(Directory** outDir, const char* name);
/* dodaje child u parent (sortirano po nazivu - opcionalno) */
int AddChild(Directory* parent, Directory* child);
/* pronalazi child s danim imenom i vraca preko out parametra */
int FindChild(Directory* parent, const char* name, Directory** outChild);
/* naredba md: napravi novi direktorij u trenutnom direktoriju */
int MakeDir(Directory* current, const char* name);
/* naredba cd <dir>: promijeni trenutni direktorij na poddirektorij sa imenom */
int ChangeDir(Directory** current, const char* name);
/* naredba cd.. : vrati se na roditelja */
int ChangeToParent(Directory** current);
/* naredba dir: ispis sadrzaja trenutnog direktorija */
int ListDir(Directory* current);
/* oslobodi cijelo stablo rekurzivno */
int FreeTree(Directory* node);

/* implementacije funkcija */

/* alocira memoriju za novi direktorij i inicijalizira polja */
int CreateDir(Directory** outDir, const char* name) {
    if (outDir == NULL || name == NULL) return -1;            // provjera argumenata
    Directory* d = (Directory*)malloc(sizeof(Directory));     // alokacija memorije
    if (d == NULL) return -2;                                 // greska alokacije
    /* inicijalizacija polja */
    strncpy(d->name, name, sizeof(d->name) - 1);              // kopiraj ime sigurnosno
    d->name[sizeof(d->name) - 1] = '\0';                      // osiguraj null terminator
    d->firstChild = NULL;                                     // nema djece
    d->nextSibling = NULL;                                    // nema brata
    d->parent = NULL;                                         // roditelj se postavlja kasnije
    *outDir = d;                                              // vrati adresu novog cvora
    return 0;                                                 // uspjeh
}

/* doda child cvor u listu djece parent-a */
int AddChild(Directory* parent, Directory* child) {
    if (parent == NULL || child == NULL) return -1;           // provjera argumenata
    /* postavi parent pokazivac u child-u */
    child->parent = parent;                                   // postavi roditelja
    /* ako nema djece, postavi kao prvo dijete */
    if (parent->firstChild == NULL) {
        parent->firstChild = child;                           // prvi child
        child->nextSibling = NULL;                            // nema sljedeca
        return 0;                                             // uspjeh
    }
    /* inace idi do kraja liste siblinga i dodaj na kraj */
    Directory* p = parent->firstChild;                        // pocetak liste djece
    
    while (p->nextSibling != NULL) p = p->nextSibling;        // pronadi kraj
    p->nextSibling = child;                                   // dodaj kao sljedeci sibling
    child->nextSibling = NULL;                                // kraj liste
    return 0;                                                 // uspjeh
}

/* trazi dijete po imenu u parent-u */
int FindChild(Directory* parent, const char* name, Directory** outChild) {
    if (parent == NULL || name == NULL || outChild == NULL) return -1; // provjera argumenata
    Directory* p = parent->firstChild;                         // pocetak liste djece
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) {                      // ako se ime poklapa
            *outChild = p;                                     // vrati cvor
            return 0;                                          // uspjeh
        }
        p = p->nextSibling;                                    // idemo na sljedeceg brata
    }
    return -2;                                                 // nije pronadeno
}

/* stvori novi direktorij u trenutnom */
int MakeDir(Directory* current, const char* name) {
    if (current == NULL || name == NULL) return -1;           // provjera argumenata
    /* provjeri da li vec postoji poddirektorij s tim imenom */
    Directory* exist = NULL;
    if (FindChild(current, name, &exist) == 0) {
        printf("Direktorij '%s' vec postoji.\n", name);      // ispis na hrvatskom
        return -2;                                           // greska: vec postoji
    }
    Directory* newDir = NULL;                                 // pomocni pokazivac
    int rc = CreateDir(&newDir, name);                        // kreiraj novi cvor
    if (rc != 0) return rc;                                   // proslijedi gresku
    rc = AddChild(current, newDir);                           // dodaj u listu djece
    if (rc != 0) {                                            // ako dodavanje ne uspije
        free(newDir);                                         // oslobodi memoriju
        return -3;                                            // greska
    }
    printf("Kreiran direktorij: %s\n", name);                 // uspjesan ispis
    return 0;                                                 // uspjeh
}

/* promijeni trenutni direktorij na poddirektorij */
int ChangeDir(Directory** current, const char* name) {
    if (current == NULL || *current == NULL || name == NULL) return -1; // provjera argumenata
    Directory* target = NULL;                                  // spremnik za pronadeno dijete
    int rc = FindChild(*current, name, &target);               // trazi child
    if (rc != 0) {
        printf("Direktorij '%s' nije pronaden.\n", name);     // ispisi gresku ako nema
        return -2;                                            // nije pronadeno
    }
    *current = target;                                         // postavi trenutni na target
    printf("Trenutni direktorij: %s\n", (*current)->name);     // ispis nove lokacije
    return 0;                                                  // uspjeh
}

/* vrati se na roditelja */ 
int ChangeToParent(Directory** current) {

    if (curret == NULL || *current == NULL) return -1;       // provjera argumenata
    if ((*current)->parent == NULL) {                         // ako nema roditelja (root)
        printf("Vec ste u korijenskom direktoriju.\n");      // ispisi poruku
        return -2;                                            // nema roditelja
    }
    *current = (*current)->parent;                            // postavi na roditelja
    printf("Trenutni direktorij: %s\n", (*current)->name);    // ispis trenutne lokacije
    return 0;                                                 // uspjeh
}

/* ispis svih poddorektorija trenutnog direktorija */
int ListDir(Directory* current) {
    if (current == NULL) return -1;                           // provjera argumenata
    printf("Sadrzaj direktorija '%s':\n", current->name);      // zaglavlje ispis
    Directory* p = current->firstChild;                       // pocetak liste djece
    if (p == NULL) {                                          // ako nema djece
        printf("  (prazan)\n");                               // ispisi prazno
        return 0;                                             // uspjeh
    }
    while (p != NULL) {                                       // prolaz kroz sve childove
        printf("  %s\n", p->name);                            // ispis imena child-a
        p = p->nextSibling;                                   // idemo na sljedeci sibling
    }
    return 0;                                                 // uspjeh
}

/* rekurzivno oslobodi cvorove */
int FreeTree(Directory* node) {
    if (node == NULL) return 0;                               // nista za osloboditi
    Directory* child = node->firstChild;                      // prvo dijete
    while (child != NULL) {                                   // prolaz kroz djecu
        Directory* next = child->nextSibling;                 // sacuvaj sljedeci sibling
        FreeTree(child);                                      // rekurzivno oslobodi subtree
        child = next;                                         // nastavi na sljedeci sibling
    }
    free(node);                                               // oslobodi trenutni cvor
    return 0;                                                 // uspjeh
}


int main(void) {
    /* inicijalizacija root direktorija */
    Directory* root = NULL;                                   // pokazivac na korijen
    if (CreateDir(&root, "/") != 0) {                         // kreiraj korijen
        printf("Greska prilikom stvaranja root direktorija.\n"); // ispis greske
        return -1;                                            // prekid programa
    }
    Directory* current = root;                                // trenutni direktorij = root

    /* ispis osnovnih uputa */
    printf("Jednostavan simulator direktorija (md, cd <dir>, cd.., dir, exit)\n"); // upute
    printf("Upisi naredbu i pritisni Enter.\n");              // upute korisniku

    char command[128];                                        // buffer za cijelu naredbu
    while (1) {                                               // glavna petlja izbornika
        printf("> ");                                        // prompt
        if (fgets(command, sizeof(command), stdin) == NULL) { // procitaj liniju
            printf("\n");                                    // ako EOF
            break;                                           // izadji
        }
        /* ukloni newline na kraju */
        command[strcspn(command, "\n")] = '\0';              // makni newline

        
        if (strncmp(command, "md ", 3) == 0) {                // ako pocinje s md
            char name[64];                                   // buffer za ime
            /* izvuci ime nakon 'md ' */
            if (sscanf(command + 3, "%63s", name) == 1) {    // procitaj ime
                MakeDir(current, name);                      // kreiraj direktorij
            } else {
                printf("Neispravan sintaks za md. Koristi: md ImeDirektorija\n"); // greska
            }
        } else if (strncmp(command, "cd..", 4) == 0 || strcmp(command, "cd ..") == 0) {
            /* podrzavamo 'cd..' i 'cd ..' varijante */
            ChangeToParent(&current);                        // idi na roditelja
        } else if (strncmp(command, "cd ", 3) == 0) {        // cd sa imenom
            char name[64];                                   // buffer za ime
            if (sscanf(command + 3, "%63s", name) == 1) {    // procitaj ime
                ChangeDir(&current, name);                   // promijeni direktorij
            } else {
                printf("Neispravan sintaks za cd. Koristi: cd ImeDirektorija\n"); // greska
            }
        } else if (strcmp(command, "dir") == 0) {            // lista trenutnog direktorija
            ListDir(current);                                // ispis sadrzaja
        } else if (strcmp(command, "exit") == 0) {           // izlaz iz programa
            break;                                           // prekini petlju
        } else if (strcmp(command, "") == 0) {               // prazna linija
            continue;                                        // nastavi bez poruke
        } else {
            printf("Nepoznata naredba: '%s'\n", command);    // nepoznata naredba
            printf("Podrzane: md, cd <dir>, cd.., dir, exit\n"); // pomoc
        }
    }

    /* prije izlaza oslobodi cijelo stablo */
    FreeTree(root);                                           // oslobodi memoriju
    printf("Kraj programa.\n");                               // ispis pri kraju
    return 0;                                                 // sve ok
}
