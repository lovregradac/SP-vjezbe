#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (256)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
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


int main(void) {
    Person head;
    Position what = NULL;
    Position person = NULL;

    head.next = NULL;

    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    int birthYear = 0;
    int index, del;
    char c;

    printf("1) Insert person (start) & print\n");
    printf("2) Insert person (end) & print\n");
    printf("3) Find person & print\n");
    printf("4) Delete person & print\n");

    while (1)
    {
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
            default:
                return EXIT_SUCCESS;
        }

        getchar();  
        
    }
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
    }
    else if (NULL == pos->next->next){
        q = pos->next;
        pos->next = NULL;
        free(q);
    }
    else {
        q = pos->next;
        pos->next = pos->next->next;
        free(q);
    }

    return 0;
}
void insertAtEnd(Position where, Position what){

    while (NULL != where->next) 
        where = where->next;

    insertAfter(where, what);
}

Position createPerson(char* firstName, char* lastName, int birthYear)
{
    Position p = NULL;

    p = (Position)malloc(sizeof(Person));

    if (NULL == p)
    {
        printf("Memory allocation failed.\r\n");
        return NULL;
    }

    strcpy(p->firstName, firstName);

    strcpy(p->lastName, lastName);
    p->birthYear = birthYear;
    p->next = NULL;
}

void insertAfter(Position where, Position what)
{
    what->next = where->next;
    where->next = what;
}

void printList(Position head){
    Position p = NULL;

    p = head->next;

    printf("\r\n List of persons: \n");

    while (p != NULL)
    {
        printf("\t%s %s %d\r\n", p->firstName, p->lastName, p->birthYear);
        p = p->next;
    }
}


