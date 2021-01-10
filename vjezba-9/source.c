#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>¸

struct _element;
typedef struct _element * Position;
typedef struct _element {
    int value;

    Position left;
    Position right;
}
Element;

// Funkcije za stablo.

Position InitElement(int value);
Position FindElement(Position Tree, int value);
Position FindParent(Position Tree, Position Child);
Position FindMinElement(Position StartingElement);

int DeleteChildFromParent(Position Tree, Position Child);
Position InsertRecursively(Position CurrentElement, Position TargetElement);
Position DeleteElement(Position Tree, Position TargetElement);
int PreOrder(Position Tree);

void ClearTree(Position Tree);

// Ostale funkcije.

void PrintMenu();

int main() {

    Position Tree = NULL; // Prvi element u stablu.
    char c; // Za spremanje odabira.
    int ElementValue; // Za unos/pronalaženje elementa u stablu (ovisno o vrijednosti).

    PrintMenu();

    do {
        c = getchar();

        switch (c) {
        case '1':
            // Unos novog elementa u stablo.

            printf("Enter element value: ");
            scanf("%d", & ElementValue);

            // Stvaranje novog elementa.
            Position NewElement = InitElement(ElementValue);

            // Ako memorija nije alocirana, preskoči ostatak petlje.
            if (NewElement == NULL)
                continue;

            // Ubacivanje elementa.
            Tree = InsertRecursively(Tree, NewElement);

            break;
        case '2':
            // Ispisivanje stabla.
            PreOrder(Tree);

            printf("\n");

            break;       
        case '3':
            // Pronalaženje elementa.

            // Unos vrijednosti.
            printf("Enter value: ");
            scanf("%d", & ElementValue);

            if (NULL == Tree)
                // Nema elemenata za tražiti...
                continue;

            Position TargetElement = FindElement(Tree, ElementValue);

            // Provjeri je li vrijednost pronađenog elementa jednaka traženoj vrijednosti. Ako
            // nije, takav element nije pronađen (funkcija je vratila default vrijednost).
            if (TargetElement->value != ElementValue) {
                printf("Element not found!");
                continue;
            }

            // Ako je element pronađen, ispiši njegovu adresu i djecu.
            printf("\t Element address: %p\n", TargetElement);

            if (TargetElement->left != NULL)
                printf("\t Left child: %d", TargetElement->left->value);
            if (TargetElement->right != NULL)
                printf("\t Right child: %d\n", TargetElement->right->value);

            break;

        case '4':
            // Brisanje elementa.

            if (NULL == Tree)
                // Nema elemenata za brisati...
                continue;

            // Unos.
            printf("Unesi element: ");
            scanf("%d", &ElementValue);

            // Pronalaženje i brisanje.
            Position ElementToDelete = FindElement(Tree, ElementValue);
            Tree = DeleteElement(Tree, ElementToDelete);

            break;
        }
    
    } while (c != '5');

    ClearTree(Tree);

    return 0;
}

void ClearTree(Position Tree){
    // Rekurzivna funkcija za dealociranje memorije elemenata stabla. Funkcija se prvo rekurzivno
    // širi na sve elemente, pa ih tek onda briše.

    if (NULL == Tree)
        return;

    ClearTree(Tree->left);
    ClearTree(Tree->right);

    // Ispisivanje za debug: 
    printf("\t Clearing %d...\n", Tree->value);

    free(Tree);
}

Position DeleteElement(Position Tree, Position TargetElement) {
    // Rekurzivna unkcija za brisanje elementa.

    // Imamo 3 moguća slučaja:
    //      1. Element ima 2 dijeteta.
    //      2. Element ima 1 dijete.
    //      3. Element nema djece.

    if (NULL == Tree)
        return Tree;

    // Ako je vrijednost čvora veća od vrijednosti elementa kojeg brišemo, rekurzija ide lijevo.
    if (Tree->value > TargetElement->value)
        Tree->left = DeleteElement(Tree->left, TargetElement);
    // Ako je vrijednost čvora manja od vrijednosti elementa kojeg brišemo, rekurzija ide desno.
    else if (Tree->value < TargetElement->value)
        Tree->right = DeleteElement(Tree->right, TargetElement);
    // Inače, vrijednosti su jednake pa smo pronašli element kojeg brišemo.
    else {
        // 1. Slučaj kada element ima dvoje djece.
        if (Tree->left != NULL && Tree->right != NULL) {
            // Pronalazimo najmanji element u desnom podstablu elementa.
            Position MinElement = FindMinElement(Tree->right);

            // Mijenjamo vrijednost trenutnog elementa.
            Tree->value = MinElement->value;

            // Brišemo najmanji element iz desnog podstabla.
            Tree->right = DeleteElement(Tree->right, MinElement);
        }
        else {
            Position Temp = Tree;

            // Uvjet za upravljanje sa preostalim slučajevima.
            // 2. Element ima 1 dijete:
            //      Ako je lijevo dijete NULL, znači da desno nije, pa element kojeg brišemo poprima
            //      njegovu vrijednost. Element zatim brišemo.
            // 3. Element nema djece: 
            //      Brišemo element, uvjet nema utjecaja.
            if (Tree->left == NULL)
                Tree = Tree->right;
            else
                Tree = Tree->left;

            free(Temp);
        }
    }

    // Funkcija će uvijek vratiti vrijednost s kojom je prvotno pozvana kako bi korijen ostao
    // sačuvan.
    return Tree;
}

Position FindMinElement(Position StartingElement) {
    // Funkcija za pronalaženje najmanjeg elementa u podstablu.
    // Najmanji element je najljeviji u podstablu, pa idemo lijevo sve dok ne naiđemo na NULL.

    if (StartingElement->left == NULL)
        return StartingElement;

    FindMinElement(StartingElement->left);    
}

Position FindElement(Position Tree, int value) {
    // Funkcija za rekurzivnu pretragu binarnog stabla.

    // Vrati trenutni element u rekurziji ako je njegova vrijednost jednaka traženoj.
    if (Tree->value == value)
        return Tree;
    // Nastavi rekurziju lijevo ako je vrijednost manja od vrijednosti trenutnog elementa.
    else if (Tree->value > value)
        FindElement(Tree->left, value);
    // Nastavi rekurziju desno ako je vrijednost veća od vrijednosti trenutnog elementa.
    else
        FindElement(Tree->right, value);
}

int PreOrder(Position Tree) {
    // Nisam siguran je li ovo preorder, ali ispisiva stablo.

    if (Tree != NULL) {
        printf("%d ", Tree->value);
        PreOrder(Tree->left);
        PreOrder(Tree->right);
    } 
    else
        return 0;
}

Position InsertRecursively(Position CurrentElement, Position TargetElement) {
    // Rekurzivna funkcija za umetanje novog elementa u stablo.

    if (CurrentElement == NULL)
        // Ako smo naišli na NULL, vraćamo element kojeg insertiramo i time zaustavljamo rekurziju,
        // što znači da će određeni čvor poprimiti adresu TargetElement.
        return TargetElement;
    else if (CurrentElement->value < TargetElement->value)
        // Ako je vrijednost trenutnog čvora manja od vrijednosti elementa kojeg ubacivamo,
        // rekurzija ide desno.
        CurrentElement->right = InsertRecursively(CurrentElement->right, TargetElement);
    else if (CurrentElement->value > TargetElement->value)
        // Ako je vrijednost trenutnog čvora veća od vrijednosti elementa kojeg ubacivamo,
        // rekurzija ide lijevo.
        CurrentElement->left = InsertRecursively(CurrentElement->left, TargetElement);
    else 
        // Ako naiđemo na čvor s istom vrijednosti...
        printf("\t Element already exists!\n");

    return CurrentElement;
}

void PrintMenu() {
    printf("1) Insert element\n");
    printf("2) Print tree\n");
    printf("3) Find element\n");
    printf("4) Delete element\n");
    printf("5) Izlaz\n");
}

Position InitElement(int value) {
    // Standardna funkcija za alokaciju memorije novog elementa.

    Position NewElement = NULL;

    NewElement = (Position) malloc(sizeof(Element));

    if (NULL == NewElement) {
        printf("\t malloc failed @ InitElement!\n");
        return NULL;
    }

    NewElement->value = value;
    NewElement->left = NULL;
    NewElement->right = NULL;

    return NewElement;
}