#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _set;
typedef struct _set* Position;
typedef struct _set {
	int member;
	Position next;
} Set;

void setIntersection(Position setA, Position setB, Position setC);
void setUnion(Position setA, Position setB, Position setC);
void readFromFile(char * fileName, Position set);
void insertMember(Position set, Position member);
void insertMemberEnd(Position set, Position member);
void printSet(Position set);
void freeMembers(Position set);

Position initMember(int value);
Position findPreviousMember(Position set, Position member);


int main() {
	Set A;
	Set B;
	Set C;
	Set D;

	char * fileNameA = "set1.txt";
	char * fileNameB = "set2.txt";

	A.next = NULL;
	B.next = NULL;
	C.next = NULL; 
	D.next = NULL;

	readFromFile(fileNameA, &A);
	readFromFile(fileNameB, &B);

	printSet(&A);
	printf("\n");
	printSet(&B);

	setUnion(&A, &B, &C);
	setIntersection(&A, &B, &D);

	printf("\n\nUnion: ");
	printSet(&C);
	printf("\nIntersection: ");
	printSet(&D);

	freeMembers(&A);
	freeMembers(&B);
	freeMembers(&C);
	freeMembers(&D);

	getchar();
	

	return 0;
}

void insertMemberEnd(Position set, Position member) {
    Position p = NULL;

    p = set;
    
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = member;
    member->next = NULL;
}
Position findPreviousMember(Position set, Position member) {
    Position p = NULL;
    p = set;

    while (p->next != NULL) {
        if (p->next == member) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

void freeMembers(Position set) {
	Position p = NULL;
	Position q = NULL;
	
	p = set->next;

	while (p != NULL) {
		q = p;
		p = p->next;
		free(q);
	}
}

void setUnion(Position setA, Position setB, Position setC) {
	Position p = NULL;
	Position q = NULL;
	Position newMember = NULL;

	int Counter = 0;

	p = setA->next;
	q = setB->next;

	while (p != NULL) {
		newMember = initMember(p->member);
		insertMember(setC, newMember);
		p = p->next;
	}

	while (q != NULL) {
		newMember = initMember(q->member);
		insertMember(setC, newMember);
		q = q->next;
	}

}

void setIntersection(Position setA, Position setB, Position setC) {
	Position p = NULL;
	Position q = NULL;
	Position newMember = NULL;

	p = setA->next;
	q = setB->next;

	while (p != NULL) {
		while (q != NULL) {
			if (q->member == p->member) {
				newMember = initMember(q->member);
				insertMember(setC, newMember);
				break;
			}
			q = q->next;
		}

		q = setB->next;
		p = p->next;
	}
}

void readFromFile(char * fileName, Position set) {
	int value = 0;

	Position newMember = NULL;
	FILE * fileStream = NULL;

	fileStream = fopen(fileName, "r");

	if (NULL == fileStream) {
		printf("\t fileStream is null @ readFromFile!\n");
		return;
	}

	while (fscanf(fileStream, "%d ", &value) != EOF) {
		newMember = initMember(value);
		insertMember(set, newMember);
	}

	fclose(fileStream);
}

void printSet(Position set) {
	Position p = NULL;
	
	p = set->next;

	while (p != NULL) {
		printf("%d ", p->member);
		p = p->next;
	}
}

void insertMember(Position set, Position element) {
	Position p = NULL;
	Position q = NULL;
	p = set->next;

    while (p != NULL) {
        if (p->member == element->member) {
            return;
        } else if (p->member > element->member) {
            q = findPreviousMember(set, p);
            q->next = element;
            element->next = p;
            return;
        }
        p = p->next;
    }

    insertMemberEnd(set, element);
}

Position initMember(int value) {
	Position p = NULL;

	p = (Position)malloc(sizeof(Set));

	if (NULL == p) {
		printf("\t malloc failed @ initMember!\n");
		return NULL;
	}

	p->member = value;
	p->next = NULL;

	return p;
}