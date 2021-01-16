#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE (11)

struct _tree;
typedef struct _tree * PositionTree;
typedef struct _tree {
    PositionTree left;
    PositionTree right;

    char data[DATA_SIZE];
} Tree;

struct _stack;
typedef struct _stack * PositionStack;
typedef struct _stack {
    PositionStack next;
    PositionTree node;
} Stack;

PositionTree CreateNode(char *data);
int Push(PositionStack head, PositionTree node);
PositionTree Pop(PositionStack head);
PositionTree ReadFromFile(char *filename);
int IsNumber(char *string);
void PrintInOrder(PositionTree current);
int PrintInOrderToFile(PositionTree current, char *filename);
void RecursivePrint(FILE *fp, PositionTree current);
void ClearTree(PositionTree root);

int main() {
    PositionTree Root = NULL;
    char FileName[1024] = { 0 };
    
    printf("Enter file name: ");
    scanf("%s", FileName);

    Root = ReadFromFile(FileName);

    PrintInOrder(Root);
    printf("\n");
    PrintInOrderToFile(Root, "infiks.txt");

    ClearTree(Root);

    return 0;
}

void ClearTree(PositionTree root) {
    if (NULL == root)
        return;
    
    ClearTree(root->left);
    ClearTree(root->right);

    printf("\tClearing tree node %s...\n", root->data);

    free(root);
}

void RecursivePrint(FILE *fp, PositionTree current) {
    if (NULL == current)
        return;

    fprintf(fp, "(");
    RecursivePrint(fp, current->left);
    fprintf(fp, current->data);
    RecursivePrint(fp, current->right);
    fprintf(fp, ")");
}

int PrintInOrderToFile(PositionTree current, char *filename) {
    FILE *fp = NULL;

    fp = fopen(filename, "w");

    if (NULL == fp)
        return -1;

    RecursivePrint(fp, current);

    fclose(fp);
}

void PrintInOrder(PositionTree current) {
    if (NULL == current)
        return;

    printf("(");
    PrintInOrder(current->left);
    printf("%s", current->data);
    PrintInOrder(current->right);
    printf(")");
}

int IsNumber(char *string) {
    int Number = 0;

    if (sscanf(string, "%d", &Number) == 1) return 1;

    return 0;
}

PositionTree ReadFromFile(char *filename) {
    Stack head;
    FILE *fp = NULL;
    PositionTree Result = NULL;

    head.next = NULL;

    fp = fopen(filename, "r");

    if (NULL == fp)
        return NULL;

    while (!feof(fp)) {
        char Data[DATA_SIZE] = { 0 };
        PositionTree Node = NULL;

        fscanf(fp, "%s", Data);

        if (strlen(Data) == 0)
            break;

        Node = CreateNode(Data);

        if (!Node) {
            fclose(fp);
            return NULL;
        }

        if (IsNumber(Data))
            Push(&head, Node);
        else {
            Node->right = Pop(&head);

            if (!Node->right) {
                printf("\t Postfix fail!\n");
                fclose(fp);
                return NULL;
            }

            Node->left = Pop(&head);

            if (!Node->left) {
                printf("\t Postfix fail!\n");
                fclose(fp);
                return NULL;
            }

            Push(&head, Node);
        }

    }

    fclose(fp);
    Result = Pop(&head);

    if (!Result) {
        printf("\t File is empty!");
        return NULL;
    }

    if (Pop(&head) != NULL) {
        printf("\t Postfix fail!\n");
        return NULL;
    }

    return Result;

}

PositionTree Pop(PositionStack head) {
    PositionStack First = head->next;
    PositionTree Result = NULL;

    if (NULL == First)
        return NULL;

    head->next = First->next;
    Result = First->node;
    free(First);

    return Result;
}

int Push(PositionStack head, PositionTree node) {
    PositionStack NewNode = NULL;

    NewNode = malloc(sizeof(Stack));

    if (NULL == NewNode)
        return 1;

    NewNode->node = node;
    NewNode->next = head->next;
    head->next = NewNode;

    return 0;
}

PositionTree CreateNode(char *data) {
    PositionTree Node = NULL;

    Node = (PositionTree)malloc(sizeof(Tree));

    if (NULL == Node)
        return NULL;

    strcpy(Node->data, data);
    Node->left = NULL;
    Node->right = NULL;

    return Node;
}