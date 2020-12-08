#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 

struct _element;
typedef struct _element * Position;
typedef struct _element {
	int value;
	Position next;
}Element;

Position InitElement(int value);
void PrintElements(Position head);
int Random();
void PrintMenu();

// stog
void Push(Position stog, Position element);
int Pop(Position stog);

// red
Position Enqueue(Position red, Position element);
void Dequeue(Position red);


int main() {
	Element stog;
	Element red;

	Position temp = NULL;
	char c;
	
	stog.next = NULL;
	red.next = NULL;

	while (1) {
		PrintMenu();
		c = getchar();

		switch (c) {
		case '1':
			temp = InitElement(Random());
			Push(&stog, temp);
			PrintElements(&stog);
			printf("\n");
			break;
		case '2':
			printf("Pop: %d\n", Pop(&stog));
			PrintElements(&stog);
			printf("\n");
			break;
		case '3':
			temp = InitElement(Random());
			Push(&stog, temp);
			PrintElements(&stog);
			printf("\n");
			break;
		case '4':
			Dequeue(&stog);
			PrintElements(&stog);
			printf("\n");
			break;
		default:
			return 0;
		}

		getchar();
	}

	return 0;
}

void Dequeue(Position red) {
	Position p = NULL;
	Position temp = NULL;

	if (red->next == NULL) {
		printf("\t Queue is empty.");
		return;
	}

	p = red;

	while (p->next->next != NULL) {
		p = p->next;
	}

	temp = p->next;
	p->next = NULL;
	free(temp);
}

Position Enqueue(Position red, Position element) {
	Position temp = red->next;
	red->next = element;
	element->next = temp;
}

int Random() {
	return (rand() % 101) + 10;
}

void PrintMenu() {
	printf("1) push\n");
	printf("2) pop\n");
	printf("3) enqueue \n");
	printf("4) dequeue \n");
}


void Push(Position stog, Position element) {
	Position temp = stog->next;
	stog->next = element;
	element->next = temp;
}

int Pop(Position stog) {
	int value;

	if (stog->next == NULL) {
		printf("\t Nothing to pop!\n");
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

	printf("\t");
	while (p->next != NULL) {
		printf("%d ", p->next->value);
		p = p->next;
	}
}

Position InitElement(int value) {
	Position p = NULL;

	p = (Position)malloc(sizeof(Element));
	p->value = value;
	p->next = NULL;

	return p;
}