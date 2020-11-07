#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _element;
typedef struct _element* Pos;
typedef struct _element {
    int Coefficient;
    int Exponent;
    Pos next;
} Element;

int readFromFile(char* fileName, Pos head);
void insertElementBeginning(Pos head, Pos element);
void insertElementAfter(Pos afterThis, Pos element);
void insertElementEnd(Pos head, Pos element);
void insertElementSort(Pos head, Pos element);
void reducePolynomial(Pos head);
void sumPolynomials(Pos headA, Pos headB, Pos sum);
Pos constructElement(int Coefficient, int Exponent);
void printPolynomial(Pos head);

int main() {

    // input format: (+/-)Ax^(+/-)B... (sorted)
    //      A: coefficient
    //      B: exponent
    // example:
    //      -2x^-2+1x^+1+4x^+10
            
    Element PolinomA;
    Element PolinomB;
    Element PolinomC;

    char* fileNameA = "polinom_a.txt";
    char* fileNameB = "polinom_b.txt";

    PolinomA.next = NULL;
    PolinomB.next = NULL;
    PolinomC.next = NULL;

    readFromFile(fileNameA, &PolinomA);
    printf("\n");
    readFromFile(fileNameB, &PolinomB);

    reducePolynomial(&PolinomA);
    reducePolynomial(&PolinomB);
    
    printPolynomial(&PolinomA);
    printf("\n");
    printPolynomial(&PolinomB);
    printf("\n");

    sumPolynomials(&PolinomA, &PolinomB, &PolinomC);
    reducePolynomial(&PolinomC);
    printPolynomial(&PolinomC);

    getchar();

    return 0;
}

void insertElementEnd(Pos head, Pos element) {
    Pos p = NULL;
    p = head;

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = element;
    element->next = NULL;
}

void sumPolynomials(Pos headA, Pos headB, Pos sum) {
    Pos p = NULL;
    Pos q = NULL;
    Pos newElement = NULL;
    Pos deepCopyElementA = NULL;
    Pos deepCopyElementB = NULL;

    p = headA->next;
    q = headB->next;

    while (p != NULL & q != NULL) {
        if (p->Exponent == q->Exponent) {

            // if exponents are equal, sum the coefficients and store them in a new
            // element in 'sum'

            newElement = constructElement(p->Coefficient + q->Coefficient, p->Exponent);
            insertElementEnd(sum, newElement);
        } else if (p->Exponent > q->Exponent) {

            // if p > q, deep copy both (q first)

            deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
            deepCopyElementB = constructElement(q->Coefficient, q->Exponent);
            insertElementEnd(sum, deepCopyElementB);
            insertElementEnd(sum, deepCopyElementA);
        } else {

            // if p < q, deep copy borth (p first)

            deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
            deepCopyElementB = constructElement(q->Coefficient, q->Exponent);
            insertElementEnd(sum, deepCopyElementA);
            insertElementEnd(sum, deepCopyElementB);
        }
        p = p->next;
        q = q->next;
    }

    while (p != NULL) {

        // continue looping through p if it's longer than q and
        // deep copy the remaining elements

        deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
        insertElementEnd(sum, deepCopyElementA);
        p = p->next;
    }

    while (q != NULL) {

        // continue looping through q if it's longer than p and
        // deep copy the remaining elements

        deepCopyElementB = constructElement(q->Coefficient, q->Exponent);
        insertElementEnd(sum, deepCopyElementB);
        q = q->next;
    }
}

void deleteElement(Pos prevElement) {
    Pos targetElement = prevElement->next;
    prevElement->next = prevElement->next->next;
    free(targetElement);
}

void reducePolynomial(Pos head) {

    // if two consecutive elements have equal exponents:
    //      1. sum coefficients and store them in the first element
    //      2. delete the second element (with appropriate address management)

    Pos p = NULL;
    p = head->next;

    while (p->next != NULL) {
        if (p->Exponent == p->next->Exponent) {
            p->Coefficient = p->Coefficient + p->next->Coefficient;
            deleteElement(p);
            return;
        }
        p = p->next;
    }
}

void printPolynomial(Pos head) {
    Pos p = NULL;
    p = head->next;

    while(p != NULL) {
        printf("%dx^%d+", p->Coefficient, p->Exponent);
        p = p->next;
    }

    return;
}

Pos constructElement(int Coefficient, int Exponent) {
    Pos p = NULL;
    p = (Pos)malloc(sizeof(Element));

    p->Coefficient = Coefficient;
    p->Exponent = Exponent;
    p->next = NULL;

    return p;
}

void insertElementBeginning(Pos head, Pos element) {
    element->next = head->next;
    head->next = element;
}

void insertElementAfter(Pos afterThis, Pos element) {
    element->next = afterThis->next;
    afterThis->next = element;
}

void insertElementSort(Pos head, Pos element) {

    // auto-sort polynomial elements at input

    Pos p = NULL;
    p = head;

    if (head->next == NULL) {
        insertElementAfter(head, element);
        return;
    }

    while (p->next != NULL) {
        if ((element->Exponent) <= (p->next->Exponent)) {
            insertElementAfter(p, element);
            return;
        }
        p = p->next;
    }

    insertElementEnd(head, element);
}

int parsePolynome(char* polynome){
    return 0;
}

int readFromFile(char* fileName, Pos head) {
    Pos newElement = NULL;
    FILE* fileStream = fopen(fileName, "r");
    char coefficient_sign, exponent_sign;
    int coefficient = 0, exponent = 0;
    int counter = 0;
    
    if (NULL == fileStream) {
        printf("\t fopen is null @ readFromFile!\n");
        return -1;
    }

    while (fscanf(fileStream, "%c%dx^%c%d", &coefficient_sign, &coefficient, &exponent_sign, &exponent) != EOF) {
        if (coefficient_sign == '-')
            coefficient = coefficient * (-1);
        
        if (exponent_sign == '-')
            exponent = exponent * (-1);

        newElement = constructElement(coefficient, exponent);
        insertElementSort(head, newElement);
    }

    fclose(fileStream);

    return 0;
}
