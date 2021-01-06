#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIR_LENGTH (256)

typedef enum _insertResult {
	SUCCESS,
	DUPLICATE_DIRECTORY,
	ALLOCATION_FAILED
} InsertResult;

struct _dir;
struct _stack;
typedef struct _dir *PositionDir;
typedef struct _stack *PositionStack;

typedef struct _dir {
	char name[MAX_DIR_LENGTH];
	PositionDir sibling;
	PositionDir child;
} Dir;

typedef struct _stack{
	PositionDir dir;
	PositionStack next;
} Stack;

PositionDir InsertRecursively(PositionDir current, PositionDir element);
InsertResult md(PositionDir current, char *name);
int PushDirectory(PositionStack head, PositionDir dir);
PositionDir PopDirectory(PositionStack head);
int PrintDirectories(PositionDir current);

PositionDir CreateDirectory(char *name);
void DeleteDirectory(PositionDir current);

PositionDir FindDirectory(PositionDir current, char *name);

void PrintCurrentDir(PositionStack head);

int main() {
	PositionDir Root = NULL;
	PositionDir Current = NULL;
    
    char *Command;
    char *Argument;
    char Line[MAX_DIR_LENGTH+6];
	Stack Stog;

    Stog.next = NULL;
	Root = CreateDirectory("C:");

	if (NULL == Root) return -1;

    Current = Root;
    PushDirectory(&Stog, Current);

    while(1) {
        PrintCurrentDir(&Stog);
        fgets(Line, MAX_DIR_LENGTH + 6, stdin);
        Command = strtok(Line, " ");
        Argument = strtok(NULL, " ");
        Command = strtok(Command, "\n");
        Argument = strtok(Argument, "\n");

        if (strcmp(Command, "md") == 0) {
            if (Argument == NULL) {
                printf("\t Argument is NULL!\n");
                continue;
            }

            md(Current, Argument);
        } else if (strcmp(Command, "dir") == 0) {
            PrintDirectories(Current);
        } else if (strcmp(Command, "cd") == 0) {
            if (Argument == NULL) {
                printf("\t Argument is NULL!\n");
                continue;
            }

            PositionDir RequestedDir = FindDirectory(Current, Argument);

            if (RequestedDir == NULL) {
                printf("\t Directory doesn't exist!\n");
                continue;
            }
            
            PushDirectory(&Stog, RequestedDir);
            Current = RequestedDir;
        } else if (strcmp(Command, "cd..") == 0) {
            if ((Stog.next)->dir == Root)
                continue;
            PopDirectory(&Stog);
            Current = (Stog.next)->dir;
        } else if (strcmp(Command, "exit") == 0) {
            break;
        }

    }

	while (Stog.next != NULL)
		PopDirectory(&Stog);

	DeleteDirectory(Root);
	
	return 0;
}

PositionStack FindPreviousDir(PositionStack head, PositionStack target) {
    if (head->next == NULL)
        return NULL;

    PositionStack Dir = head;

    while (Dir->next != NULL) {
        if (Dir->next == target)
            return Dir;
        Dir = Dir->next;
    }

    return NULL;
}

void PrintCurrentDir(PositionStack head) {
    PositionStack Element = NULL;

    if (head->next == NULL)
        return;

    Element = head;

    while (Element->next != NULL) {
        Element = Element->next;
    }

    while(Element != head) {
        printf("%s\\", Element->dir->name);
         Element = FindPreviousDir(head, Element);
    }

    printf(">");
    
}

int PrintDirectories(PositionDir current) {
    PositionDir Dir = NULL;

    if (current->child == NULL)
        return -1;

    Dir = current->child;

    while (Dir != NULL) {
        printf("\t %s\n", Dir->name);
        Dir = Dir->sibling;
    }

    return 0;
     
}
PositionDir InsertRecursively(PositionDir current, PositionDir element) {
	if (current == NULL) return element;

	if (strcmp(element->name, current->name) < 0) {
		element->sibling = current;
		return element;
	}

	current->sibling = InsertRecursively(current->sibling, element);
	return current;
}

InsertResult md(PositionDir current, char *name) {
	PositionDir Element = NULL;
	PositionDir Child = NULL;

	if (NULL != FindDirectory(current, name)) 
		return DUPLICATE_DIRECTORY;

	Element = CreateDirectory(name);

	if (NULL == Element)
		return ALLOCATION_FAILED;

	current->child = InsertRecursively(current->child, Element);

	return EXIT_SUCCESS;
}

PositionDir FindDirectory(PositionDir current, char *name) {
	PositionDir Child = current->child;

	while (Child != NULL && strcmp(Child->name, name) != 0) {
		Child = Child->sibling;
	}

	return Child;
}

PositionDir PopDirectory(PositionStack head) {
	PositionStack p = head->next;
	PositionDir Result = NULL;

	if (NULL == p)
		return NULL;

	head->next = p->next;
	Result = p->dir;
	free(p);

	return Result;
}

int PushDirectory(PositionStack head, PositionDir dir) {
	PositionStack Element = NULL;

	Element = (PositionStack)malloc(sizeof(Stack));

	if (NULL == Element) {
		printf("\t malloc is null @ PushDirectory!");
		return -1;
	}

	Element->dir = dir;
	Element->next = head->next;
	head->next = Element;

	return 0;

}

void DeleteDirectory(PositionDir current) {
	if (NULL == current) return;

	DeleteDirectory(current->sibling);
	DeleteDirectory(current->child);
	free(current);
}

PositionDir CreateDirectory(char *name) {
	PositionDir Directory = NULL;
	Directory = (PositionDir)malloc(sizeof(Dir));

	if (NULL == Directory) {
		printf("\t malloc is null @ CreateDirectory!");
		return NULL;
	}

	strcpy(Directory->name, name);
	Directory->sibling = NULL;
	Directory->child = NULL;

	return Directory;
}