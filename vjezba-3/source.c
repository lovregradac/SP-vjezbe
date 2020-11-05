#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (256)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    int birthYear;
    Position next;
} Person;

Position createPerson(char* firstName, char* lastName, int birthYear);
Position findPreviousPerson(Position head, char* firstName, char* lastName);
int deletePerson(Position head, Position pos);
void insertAfter(Position where, Position what);
void insertAtEnd(Position where, Position what);
void printPerson(Position person);
void printList(Position head);
int sortList(Position head);
void switchPerson(Position first, Position second);
int writeToFile(Position head, char* fileName);
int readFromFile(Position head, char* fileName);

int main(void) {
    Person head;
    Position what = NULL;
    Position person = NULL;

    head.next = NULL;

    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    char fileName[MAX_NAME];
    int birthYear = 0;
    int index, del;
    char c;

    printf("1) Insert person @ start\n");
    printf("2) Insert person @ end & print\n");
    printf("3) Find person\n");
    printf("4) Delete person\n");
    printf("5) Insert after person\n");
    printf("6) Insert before person\n");
    printf("7) Sort by last name\n");
    printf("8) Write list to file\n");
    printf("9) Read list from file\n");

    while (1) {
        printf("Choice: ");
        c = getchar();

        switch (c){
            case '1':
                printf("Insert first name, last name & birth year: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);

                what = createPerson(firstName, lastName, birthYear);

                insertAfter(&head, what);
                printList(&head);
                break;
            case '2':
                printf("Insert first name, last name & birth year: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);

                what = createPerson(firstName, lastName, birthYear);

                insertAtEnd(&head, what);
                printList(&head);
                break;
            case '3':
                printf("Enter first & last name: ");
                scanf("%s %s", firstName, lastName);

                person = findPreviousPerson(&head, firstName, lastName);
                printPerson(person->next);
                break;
            case '4':
                printf("Enter first & last name: ");
                scanf("%s %s", firstName, lastName);

                person = findPreviousPerson(&head, firstName, lastName);
                deletePerson(&head, person);
                printList(&head);
                break;

            case '5':
                printf("Insert first name, last name & birth year (of new person): ");
                scanf("%s %s %d", firstName, lastName, &birthYear);

                what = createPerson(firstName, lastName, birthYear);

                printf("Insert first & last name of target person: ");
                scanf("%s %s", firstName, lastName);

                person = findPreviousPerson(&head, firstName, lastName);

                insertAfter(person->next, what);
                printList(&head);
                break;

            case '6':
                printf("Insert first name, last name & birth year (of new person): ");
                scanf("%s %s %d", firstName, lastName, &birthYear);

                what = createPerson(firstName, lastName, birthYear);

                printf("Insert first & last name of target person: ");
                scanf("%s %s", firstName, lastName);

                person = findPreviousPerson(&head, firstName, lastName);

                insertAfter(person, what);
                printList(&head);
                break;
            case '7':
                sortList(&head);
                printList(&head);
                break;
            case '8':
                printf("Enter file name: ");
                scanf("%s", fileName);

                writeToFile(&head, fileName);
                break;
            case '9':
                printf("Enter file name: ");
                scanf("%s", fileName);

                readFromFile(&head, fileName);
                printList(&head);
                break;
            default:
                return EXIT_SUCCESS;
        }
        getchar();        
    }

    return EXIT_SUCCESS;
}

int readFromFile(Position head, char* fileName) {
    Position p = NULL;
    FILE* fileStream;
    char buffer[MAX_NAME*2 + 6];
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    int birthYear;
    Position person;

    p = head->next;

    fileStream = fopen(fileName, "r");

    if (NULL == fileStream) {
        printf("\t fopen is null @ writeToFile!\n");
        return -1;
    }

    while(fgets(buffer, 256, fileStream)) {
        sscanf(buffer, "%s %s %d", firstName, lastName, &birthYear);
        person = createPerson(firstName, lastName, birthYear);
        insertAfter(head, person);
    }

    fclose(fileStream);
}

int writeToFile(Position head, char* fileName) {
    Position p = NULL;
    p = head->next;
    FILE* fileStream;

    if (NULL == p) {
        printf("\t list is null @ writeToFile!\n");
        return -1;
    }

    fileStream = fopen(fileName, "w");

    if (NULL == fileStream) {
        printf("\t fopen is null @ writeToFile!\n");
        return -1;
    }

    while (p != NULL) {
        fprintf(fileStream, "%s %s %d\n", p->firstName, p->lastName, p->birthYear);
        p = p->next;
    }

    fclose(fileStream);

}
void switchPerson(Position a, Position b){
    char a_firstName[MAX_NAME];
    char a_lastName[MAX_NAME];
    int a_birthYear = a->birthYear;

    strcpy(a_firstName, a->firstName);
    strcpy(a_lastName, a->lastName);

    strcpy(a->firstName, b->firstName);
    strcpy(a->lastName, b->lastName);
    a->birthYear = b->birthYear;

    strcpy(b->firstName, a_firstName);
    strcpy(b->lastName, a_lastName);
    b->birthYear = a_birthYear;
}

int sortList(Position head){
    Position p;
    Position temp = NULL;
    int i, check;

    if (NULL == head){
        printf("\t List is null @ sortList.");
        return EXIT_FAILURE;
    }

    do {
        check = 0;
        p = head->next;
         
        while (p->next != temp){
            //printf("\t%s %s\n", p->lastName, p->next->lastName);
            if (strcmp(p->lastName, p->next->lastName) > 0) {
                switchPerson(p, p->next);
                check = 1;
            }
            p = p->next;
        }
        temp = p;
    } while (check);

    return EXIT_SUCCESS;
}

void printPerson(Position person) {
    printf("\t%s %s %d\n", person->firstName, person->lastName, person->birthYear);
}

Position findPreviousPerson(Position head, char* firstName, char* lastName){
    Position p = head;

    if (NULL == head->next){
        printf("\thead = NULL @ findPreviousPerson\n");
    }

    while ((head->next != NULL) && ((strcmp(firstName, head->next->firstName) || strcmp(lastName, head->next->lastName)) != 0)){
            p = head->next;
            head = head->next;  
    }

    if ((strcmp(firstName, p->next->firstName) || strcmp(lastName, p->next->lastName)) != 0){
        p = NULL;
    }

    return p;
}

int deletePerson(Position head, Position pos){
    Position q = NULL;
    q = (Position)malloc(sizeof(Person));

    if (NULL == q) {
        printf("\tmalloc failed @ deletePerson\n");
        return -1;
    }

    if (NULL == pos->next){
        q = pos;
        head->next = NULL;
        free(q);
        return 0;
    }
    else if (NULL == pos->next->next){
        q = pos->next;
        pos->next = NULL;
        free(q);
        return 0;
    }
    else {
        q = pos->next;
        pos->next = pos->next->next;
        free(q);
        return 0;
    }

    return 1;
}

void insertAtEnd(Position where, Position what) {
    while (NULL != where->next) 
        where = where->next;

    insertAfter(where, what);
}

Position createPerson(char* firstName, char* lastName, int birthYear){
    Position p = NULL;

    p = (Position)malloc(sizeof(Person));

    if (NULL == p){
        printf("Memory allocation failed.\r\n");
        return NULL;
    }

    strcpy(p->firstName, firstName);

    strcpy(p->lastName, lastName);
    p->birthYear = birthYear;
    p->next = NULL;
}

void insertAfter(Position where, Position what){
    what->next = where->next;
    where->next = what;
}

void printList(Position head){
    Position p = NULL;

    p = head->next;

    printf("\n\tList of persons: \n");

    while (p != NULL) {
        printf("\t%s %s %d\r\n", p->firstName, p->lastName, p->birthYear);
        p = p->next;
    }
}


