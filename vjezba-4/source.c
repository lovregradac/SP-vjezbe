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
void multiplyPolynomials(Pos headA, Pos headB, Pos product);
void sumPolynomials(Pos headA, Pos headB, Pos sum);
Pos constructElement(int Coefficient, int Exponent);
void printPolynomial(Pos head);
void printPolynomialFormatted(Pos head);

int main() {
            
    Element PolinomA; // input
    Element PolinomB; // input
    Element PolinomC; // sum
    Element PolinomD; // product

    char* fileNameA = "polinom_a.txt";
    char* fileNameB = "polinom_b.txt";

    PolinomA.next = NULL;
    PolinomB.next = NULL;
    PolinomC.next = NULL;
    PolinomD.next = NULL;

    readFromFile(fileNameA, &PolinomA);
    printf("\n");
    readFromFile(fileNameB, &PolinomB);
    
    printf("Input A:\t");
    printPolynomial(&PolinomA);
    printf("\n\t\t");
    printPolynomialFormatted(&PolinomA);
    printf("\n\nInput B:\t");
    printPolynomial(&PolinomB);
    printf("\n\t\t");
    printPolynomialFormatted(&PolinomB);

    sumPolynomials(&PolinomA, &PolinomB, &PolinomC);
    multiplyPolynomials(&PolinomA, &PolinomB, &PolinomD);

    printf("\n\n\nSum:\t\t");
    printPolynomial(&PolinomC);
    printf("\n\t\t");
    printPolynomialFormatted(&PolinomC);
    printf("\n\nProduct:\t");
    printPolynomial(&PolinomD);
    printf("\n\t\t");
    printPolynomialFormatted(&PolinomD);

    getchar();

    return 0;
}

void multiplyPolynomials(Pos headA, Pos headB, Pos product) {
    Pos p = NULL;
    Pos q = NULL;
    Pos newElement = NULL;
    Pos rememberQ = NULL;

    int counter = 0;

    p = headA->next;
    q = headB->next;
    rememberQ = q;

    if (NULL == p || NULL == q) {
        printf("\t Input is null @ multiplyPolynomials!\n");
        return;
    }

    while (p != NULL){
        while(q != NULL) {
            newElement = constructElement(p->Coefficient * q->Coefficient, p->Exponent + q->Exponent);
            insertElementSort(product, newElement);
            q = q->next;
        }
        p = p->next;
        q = rememberQ;
    }
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

            /*
                If exponents are equal, sum the coefficients and store them in a new
                element in 'sum'.
            */

            newElement = constructElement(p->Coefficient + q->Coefficient, p->Exponent);
            insertElementEnd(sum, newElement);
        } else if (p->Exponent > q->Exponent) {

            // If p > q, deep copy both (q first).

            deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
            deepCopyElementB = constructElement(q->Coefficient, q->Exponent);
            insertElementEnd(sum, deepCopyElementB);
            insertElementEnd(sum, deepCopyElementA);
        } else {

            // If p < q, deep copy borth (p first).

            deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
            deepCopyElementB = constructElement(q->Coefficient, q->Exponent);
            insertElementEnd(sum, deepCopyElementA);
            insertElementEnd(sum, deepCopyElementB);
        }
        p = p->next;
        q = q->next;
    }

    while (p != NULL) {

        /*
            Continue looping through p if it's longer than q and
            deep copy the remaining elements.
        */

        deepCopyElementA = constructElement(p->Coefficient, p->Exponent);
        insertElementEnd(sum, deepCopyElementA);
        p = p->next;
    }

    while (q != NULL) {

        /*
            Continue looping through q if it's longer than p and
            deep copy the remaining elements.
        */

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

void printPolynomial(Pos head) {
    Pos p = NULL;
    p = head->next;

    while(p != NULL) {
        printf("%d %d ", p->Coefficient, p->Exponent);
        p = p->next;
    }

    return;
}

void printPolynomialFormatted(Pos head) {
    // todo: don't print 1s

    Pos p = NULL;
    p = head->next;

    while(p != NULL) {
        
        if (p->Coefficient == 0) {
            p = p->next;
            continue;
        } else if (p->Coefficient == -1 && p->Exponent != 0) {
            printf("-");
        } else if (p->Coefficient == 1 && p->Exponent != 0) {
            if (p != head->next)
                printf("+");
        } else {
            if (p != head->next && p->Coefficient > 0)
                printf("+");
            printf("%d", p->Coefficient);
        }

        if (p->Exponent == 0) {

        } else if (p->Exponent == 1) {
            printf("x");
        } else if (p->Exponent < 0) {
            printf("x^(%d)", p->Exponent);
        } else {
            printf("x^%d", p->Exponent);
        }

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

    // Auto-sorts polynomial elements at input.

    Pos p = NULL;
    p = head;

    if (head->next == NULL) {
        insertElementAfter(head, element);
        return;
    }

    while (p->next != NULL) {
        if ((element->Exponent) < (p->next->Exponent)) {
            /* 
               If the exponent of the next element is larger than our input exponent,
               insert input element before the next element / after the current element.
            */
            insertElementAfter(p, element);
            return;
        } else if (element->Exponent == (p->next->Exponent)) {
            // If exponents are equal, don't insert, sum coefficients.
            p->next->Coefficient = p->next->Coefficient + element->Coefficient;
            return;
        }
        p = p->next;
    }

    insertElementEnd(head, element);
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

    while (fscanf(fileStream, "%d %d", &coefficient, &exponent) != EOF) { 
        newElement = constructElement(coefficient, exponent);
        insertElementSort(head, newElement);
    }

    fclose(fileStream);

    return 0;
}
