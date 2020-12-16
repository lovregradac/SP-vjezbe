#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

struct _element;
typedef struct _element * Position;
typedef struct _element {
	float value;
	Position next;
} Element;

Position InitElement(float value);
void PrintElements(Position head);
void FreeElements(Position head);

void Push(Position stog, Position element);
int Pop(Position stog);
int Sum(Position stog);
int Multiply(Position stog);
int Divide(Position stog);


int main() {
	char* fileName = "postfiks.txt";
	char* buffer = NULL;
	FILE* fileStream = NULL;
	Element stog;
	Position element = NULL;

	int value = 0;

	fileStream = fopen(fileName, "r");
	buffer = (char *)malloc(50 * sizeof(char));

	stog.next = NULL;
	
	while (fscanf(fileStream, "%s", buffer) != EOF) {
		value = sscanf(buffer, "%d", &value);

		if (value == 1) {
			element = InitElement((float)atoi(buffer));
			Push(&stog, element);
		}
		else {
			if (strcmp(buffer, "+") == 0)
				Sum(&stog);
			else if (strcmp(buffer, "*") == 0)
				Multiply(&stog);
			else if (strcmp(buffer, "/") == 0)
				Divide(&stog);
		}
	}

	fclose(fileStream);
	PrintElements(&stog);
    FreeElements(&stog);
	getchar();

	return 0;
}

int Sum(Position stog) {

	Position p = NULL;

	if (stog->next == NULL) {
		printf("\tStack is empty!\n");
		return 1;
	}

	if (stog->next->next == NULL) {
		return 0;
	}

	p = stog->next;
	p->next->value = p->value + p->next->value;
	Pop(stog);
	Sum(stog);
}

int Divide(Position stog) {

	Position p = NULL;

	if (stog->next == NULL) {
		printf("\tStack is empty!\n");
		return 1;
	}

	if (stog->next->next == NULL) {
		return 0;
	}

	p = stog->next;
	p->next->value = p->value / p->next->value;
	Pop(stog);
	Sum(stog);
}


int Multiply(Position stog) {

	Position p = NULL;

	if (stog->next == NULL) {
		printf("\tStack is empty!\n");
		return 1;
	}

	if (stog->next->next == NULL) {
		return 0;
	}

	p = stog->next;
	p->next->value = p->value * p->next->value;
	Pop(stog);
	Multiply(stog);
}

void Push(Position stog, Position element) {
	Position temp = stog->next;
	stog->next = element;
	element->next = temp;
}

int Pop(Position stog) {
	int value = 0;

	if (stog->next == NULL) {
		printf("\t Nothing to pop!");
		return 0;
	}

	Position temp = stog->next;
	stog->next = temp->next;

	value = temp->value;
	free(temp);

	return value;
}

void PrintElements(Position head) {
	Position p = NULL;

	if (head->next == NULL) {
		return;
	}

	p = head;

	while (p->next != NULL) {
		printf("%f ", p->next->value);
		p = p->next;
	}
}

Position InitElement(float value) {
	Position p = NULL;

	p = (Position)malloc(sizeof(Element));
	p->value = value;
	p->next = NULL;

	return p;
}

void FreeElements(Position head) {
    Position p = NULL;
    Position q = NULL;

    p = head->next;

    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }
}