#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (50)
#define TABLE_SIZE (11)

struct _student;
typedef struct _student * Position;
typedef struct _student {
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    int broj;

    Position next;
} Student;

Position CreateStudent(char * ime, char * prezime, int broj);

int GetKey(char * prezime);
int GetIDFromName(char * ime, char * prezime, Position * table);

void ReadStudentsFromFile(char * filename, Position * table);
void PrintHashTable(Position * table);
void InitHashTable(Position * table);
void ClearHashTable(Position * table);
void ClearKey(Position head);
void InsertStudent(Position head, Position student);

int main() {
    Position hashtable[TABLE_SIZE] = { NULL };
    char * filename = "studenti.txt";

    InitHashTable(hashtable); // popuni sa headovima
    ReadStudentsFromFile(filename, hashtable); // ucitaj iz datoteke

    PrintHashTable(hashtable);

    // pretrazivanje maticnog broja po imenu i prezimenu (bez unosa)...
    char * ime = "Dolores";
    char * prezime = "Rada";
    printf("\n[i] Maticni broj od %s %s je %d.\n ", ime, prezime, GetIDFromName(ime, prezime, hashtable));

    ClearHashTable(hashtable);

    getchar();

    return 0;
}

int GetIDFromName(char * ime, char * prezime, Position * table) {
    int key = GetKey(prezime);

    Position p = table[key]->next;

    while (p->next != NULL) {
        if ((strcmp(ime, p->ime) == 0) && (strcmp(prezime, p->prezime) == 0))
            return p->broj;
        p = p->next;
    }

    return -1;
}

void ClearKey(Position head) {
    if (NULL == head) return;

    ClearKey(head->next);
    free(head);
}

void ClearHashTable(Position * table) {
    Position p = NULL;
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
        ClearKey(table[i]);
}

void InitHashTable(Position * table) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        Position head = NULL;

        head = malloc(sizeof(Student));
        head->next = NULL;

        table[i] = head;
    }
}

void InsertStudent(Position head, Position student) {
    Position p = head;
    int cmp = 0;

    while (p->next != NULL) {
        cmp = strcmp(student->prezime, p->next->prezime);

        if (cmp > 0) p = p->next;    
        else if (cmp == 0) {
            if (strcmp(student->ime, p->next->ime) < 0) break;
            else p = p->next;
        } else break;
    }
  
    student->next = p->next;
    p->next = student;
}

void PrintHashTable(Position * table) {
    int i;
    Position p = NULL;

    for (i = 0; i < TABLE_SIZE; i++) {
        p = (table[i])->next;

        while (p != NULL) {
            printf("%d \t %s %s (%d)\n", i, p->prezime, p->ime, p->broj);
            p = p->next;
        }
    }
}

void ReadStudentsFromFile(char * filename, Position * table) {
    FILE * fp;
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    int broj;

    fp = fopen(filename, "r");

    if (!fp) {
        printf("\t fopen failed @ ReadStudentsFromFile\n");
        return;
    }

    while (fscanf(fp, "%s %s %d", ime, prezime, &broj) != EOF) {
        Position newstudent = CreateStudent(ime, prezime, broj);

        if (!newstudent) return;

        int key = GetKey(prezime);
        InsertStudent(table[key], newstudent);
     }
}

int GetKey(char * prezime) {
    int key = 0;
    int i;

    int max = strlen(prezime);
    if (max > 5) max = 5;

    for (i = 0; i < max; i++)
        key += prezime[i];

    return key % TABLE_SIZE;
}

Position CreateStudent(char * ime, char * prezime, int broj) {
    Position p = NULL;

    p = malloc(sizeof(Student));

    if (!p) {
        printf("\t malloc fail @ CreateStudent\n");
        return NULL;
    }

    strcpy(p->ime, ime);
    strcpy(p->prezime, prezime);
    p->broj = broj;
    p->next = NULL;

    return p;
}