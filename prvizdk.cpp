#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024   // maksimalna duljina redka
#define MAX_NAME 50     // maksimalna duljina imena i prezimena

// definicija strukture Student
typedef struct {
    char ime[MAX_NAME];     // ime 
    char prezime[MAX_NAME]; // prezime 
    int bodovi;             // broj bodova
} Student;

// deklaracije funkcija
int prebrojiRedkeUDatoteci(char* imeDatoteke);
int ucitajStudenteIzDatoteke(char* imeDatoteke, Student* studenti, int brojStudenata, int* maxBodovi);
int ispisiStudente(Student* studenti, int brojStudenata, int maxBodovi);

int main() {
    char* imeDatoteke = "studenti.txt"; // ime datoteke koja se cita

    // prebroji koliko redaka tj studenata ima u datoteci
    int brojStudenata = prebrojiRedkeUDatoteci(imeDatoteke);
    if (brojStudenata < 1) { // provjera datoteke
        printf("Greška: datoteka nije otvorena ili je prazna.\n");
        return 1; // izlaz s kodom greske
    }

    // alociranje memorije za niz studenata
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (!studenti) { // provjera za malloc
        printf("Greška: memorija nije alocirana.\n");
        return 2;
    }

    int maxBodovi = 0; // varijabla za maksimalni broj bodova

    // ucitavanje podataka studenata
    if (ucitajStudenteIzDatoteke(imeDatoteke, studenti, brojStudenata, &maxBodovi) != 0) {
        printf("Greška pri učitavanju studenata.\n");
        free(studenti); // oslobadanje memorije
        return 3;
    }

    // ispisivanje svih studenata s realnim i apsolutnim bodovima
    ispisiStudente(studenti, brojStudenata, maxBodovi);

    free(studenti); // oslobadanje memorije
    return 0; // program uspjesno zavrsen
}

// funkcija za brojanje redaka
int prebrojiRedkeUDatoteci(char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r"); // otvaranje datoteke za citanje
    if (!fp) return -1; // greska za slucaj da datoteka ne postoji

    int brojac = 0; // brojac redaka
    char buffer[MAX_LINE]; // buffer za svaki redak

    while (fgets(buffer, MAX_LINE, fp)) { // citanje redak po redak
        if (strlen(buffer) > 1) // ignoriranje prazne linije
            brojac++; // povecavanje brojaca
    }

    fclose(fp); // zatvaranje datoteke
    return brojac; // vracanje broja redaka
}

// funkcija za ucitavanje studenata i pronalazak max bodova
int ucitajStudenteIzDatoteke(char* imeDatoteke, Student* studenti, int brojStudenata, int* maxBodovi) {
    FILE* fp = fopen(imeDatoteke, "r"); // otvaranje datoteke
    if (!fp) return -1; // greska ako se ne otvara

    char buffer[MAX_LINE]; // buffer za citanje linije
    *maxBodovi = 0; // inicijaliziranje maxBodovi
    int i = 0; // indeks za niz studenata

    while (fgets(buffer, MAX_LINE, fp) && i < brojStudenata) { // citanje dok ima redaka i mjesta u nizu
        if (strlen(buffer) <= 1) continue; // preskakanje prazne linije

        char ime[MAX_NAME], prezime[MAX_NAME];
        int bodovi;

        // parsiraj redak: ime, prezime, bodovi
        if (sscanf(buffer, " %49s %49s %d", ime, prezime, &bodovi) == 3) {
            strcpy(studenti[i].ime, ime);       // kopiranje imena u strukturu
            strcpy(studenti[i].prezime, prezime); // kopiranje prezimena
            studenti[i].bodovi = bodovi;        // spremanje bodova

            if (bodovi > *maxBodovi) // azuriranje max bodova
                *maxBodovi = bodovi;

            i++; // prelazak na iduceg studenta
        }
        // inace preskoci liniju koja nije pravilno formatirana
    }

    fclose(fp); // zatvaranje datoteke

    if (i != brojStudenata) { // upozorenje ako procitan broj nije ocekivan
        printf("Upozorenje: pročitan broj studenata (%d) ne odgovara očekivanom (%d)\n", i, brojStudenata);
    }

    return 0; // uspjesno odradeno
}

// funkcija za ispis svih studenata i njihovih relativnih bodova
int ispisiStudente(Student* studenti, int brojStudenata, int maxBodovi) {
    printf("Ime\tPrezime\tBodovi\tRelativni bodovi (%%)\n"); // zaglavlje

    for (int i = 0; i < brojStudenata; i++) { // prolazak kroz sve studente
        double relativniBodovi = (double)studenti[i].bodovi / maxBodovi * 100.0; // izracun relativnog %
        printf("%s\t%s\t%d\t%.2f%%\n",
               studenti[i].ime,      // ime
               studenti[i].prezime,  // prezime
               studenti[i].bodovi,   // apsolutni bodovi
               relativniBodovi);     // relativni bodovi
    }

    return 0; // uspješno ispisano
}
