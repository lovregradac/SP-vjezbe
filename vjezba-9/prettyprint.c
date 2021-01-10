#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>¸
#define max(a,b) (((a) > (b)) ? (a) : (b))

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
Position FindParent(Position Tree, int value);
Position FindMinElement(Position StartingElement);

int DeleteChildFromParent(Position Tree, Position Child);
int InsertRecursively(Position CurrentElement, Position TargetElement);
int DeleteElement(Position Tree, Position TargetElement);
int PreOrder(Position Tree);

void ClearTree(Position Tree);

// Ostale funkcije.

void PrintMenu();

// Eksperimentalne prettyprint funkcije.

int CalculateLevels(Position Tree);
int XYToIndex(int X, int Y, int Width);

void Populate(Position Node, int Level, int *Matrix, int First, int LevelCount, int X, int Y, int Width);
void PrettyPrint(Position Tree);

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

            // Ako je stablo prazno, umetni novi element na mjestu prvog (Tree) i preskoči
            // ostatak petlje.
            if (NULL == Tree) {
                Tree = NewElement;
                printf("\t Element inserted!\n");
                continue;
            }

            // Inače, rekurzivno umetni element.
            if (InsertRecursively(Tree, NewElement) == 0) {
                printf("\t Element inserted!\n");
            }

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

            // Unos.
            printf("Unesi element: ");
            scanf("%d", &ElementValue);

            // Pronalaženje i brisanje.
            Position ElementToDelete = FindElement(Tree, ElementValue);
            if (DeleteElement(Tree, ElementToDelete) == 0) {
                printf("\t Element deleted!\n");
            }

            break;
         case 'x':
            PrettyPrint(Tree);
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

int DeleteElement(Position Tree, Position TargetElement) {
    // Funkcija za brisanje elementa.

    // Imamo 3 moguća slučaja:
    //      1. Element nema djece.
    //      2. Element ima 2 dijeteta.
    //      3. Element ima 1 dijete.

    // 1. Ako element nema djece, postavimo pokazivač njegovog roditelja na NULL i oslobodimo
    //    alociranu memoriju.
    if (TargetElement->left == NULL && TargetElement->right == NULL) {
        if (Tree == TargetElement) {
             // Slučaj ako želimo izbrisati korijen (ne tražimo roditelja, već samo brišemo element).
             free(TargetElement);
             return 0;
        } 
        else
            // Slučaj ako želimo izbrisati element koji nije korijen.
            DeleteChildFromParent(Tree, TargetElement);
            return 0;
    }
    // 2. Ako element ima 2 dijeteta, zamijenimo ga sa najmanjim elementom u njegovom desnom
    //    podstablu te izbrišemo taj element.
    else if (TargetElement->left != NULL && TargetElement->right != NULL) {
        // Najmanji element u desnom podstablu elementa kojeg brišemo.
        Position MinElement = FindMinElement(TargetElement->right);

        // Ne smijemo odmah dodijeliti vrijednost minimalnog elementa iz desnog podstabla onom
        // elementu kojeg brišemo, jer ćemo tada imati 2 elementa s istom vrijednosti, pa ga
        // spremamo u temp varijablu.
        int TempValue = MinElement->value;

        // Izbriši minimalni element iz desnog podstabla.
        DeleteElement(Tree, MinElement);
        // Dodijeli njegovu vrijednost elementu kojeg brišemo.
        TargetElement->value = TempValue;
        return 0;
    }
    // 3. Ako element ima 1 dijete, taj element zamijenimo s njegovim dijetetom.
    else {
        Position Child = NULL;

        // Znamo da element kojeg brišemo ima 1 dijete. Gledamo s koje strane se nalazi te spremamo
        // njegovu adresu u Child.
        if (TargetElement->left != NULL)
            Child = TargetElement->left;
        else if (TargetElement->right != NULL)
            Child = TargetElement->right;

        // Ako je element korijen stabla, ne trebamo tražiti roditelja, samo ga zamijenimo sa
        // dijetetom.
        if (Tree == TargetElement) {
            Tree->value = Child->value;
            Tree->left = Child->left;
            Tree->right = Child->right;
            free(Child);
            return 0;
        }
        // Ako element nije korijen stabla, tražimo njegovog roditelja, uspostavljamo vezu s
        // dijetetom pa brišemo element.
        else {
            // Pronalazimo roditelja od elementa kojeg želimo izbrisati.
            Position Parent = FindParent(Tree, TargetElement->value);

            // Provjeravamo s koje strane roditelja se nalazi element kojeg brišemo, te umjesto
            // njega insertiramo njegovo dijete (Child).
            if (Parent->left == TargetElement)
                Parent->left = Child;
            else if (Parent->right == TargetElement)
                Parent->right = Child;
            
            // Brišemo element.
            free(TargetElement);
            return 0;
        }
    }

    return 1;
}

Position FindMinElement(Position StartingElement) {
    // Funkcija za pronalaženje najmanjeg elementa u podstablu.
    // Najmanji element je najljeviji u podstablu, pa idemo lijevo sve dok ne naiđemo na NULL.

    if (StartingElement->left == NULL)
        return StartingElement;

    FindMinElement(StartingElement->left);    
}

int DeleteChildFromParent(Position Tree, Position Child) {
    // Funkcija za brisanje dijeteta tj. veze između roditelja i dijeteta.

    // Prvo pronalazimo roditelja čije dijete ime specificiranu vrijednost (value).
    Position Parent = FindParent(Tree, Child->value);

    // Provjeravamo s koje strane roditelja se nalazi dijete, pa ga onda brišemo.
    if (Parent->left == Child) {
        Child = Parent->left;
        Parent->left = NULL;
        free(Child);
        return 0; 
    }
    else if (Parent->right == Child) {
        Child = Parent->right;
        Parent->right = NULL;
        free(Child);
        return 0;
    }

    return 1;
}

Position FindParent(Position Tree, int value) {
    // Rekurzivna unkcija za pronalaženje roditelja dijeteta s specificiranom vrijednosti (value).
    // Tree je element na kojem se nalazimo u trenutnoj razini rekurzije.

    // Ako Tree ima lijevo dijete, provjeravamo je li njegova vrijednost jednaka traženoj
    // vrijednosti. Ako je, Tree je roditelj pa izlazimo iz rekurzije.
    if (Tree->left != NULL)
        if (Tree->left->value == value)
            return Tree;
    
    // Isto za desnu tranu.
    if (Tree->right != NULL)
        if (Tree->right->value == value)
            return Tree;

    // Ukoliko trenutni Tree nije roditelj kojeg tražimo, funkcija se širi na lijevu ili desnu
    // stranu, ovisno o vrijednosti.
    if (Tree != NULL) {
        if (Tree->value > value)
            FindParent(Tree->left, value);
        else if (Tree->value < value)
            FindParent(Tree->right, value);
    }
    
}

Position FindElement(Position Tree, int value) {
    // Funkcija za rekurzivnu pretragu binarnog stabla.

    // Vrati trenutni element u rekurziji ako je njegova vrijednost jednaka traženoj.
    if (Tree->value == value) {
        return Tree;
    }
    // Nastavi rekurziju lijevo ako je vrijednost manja od vrijednosti trenutnog elementa.
    else if (Tree->value > value) {
        FindElement(Tree->left, value);
    }
    // Nastavi rekurziju desno ako je vrijednost manja od vrijednosti trenutnog elementa.
    else {
        FindElement(Tree->right, value);
    }
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

int InsertRecursively(Position CurrentElement, Position TargetElement) {
    // Rekurzivna funkcija za umetanje elementa u stablo.

    // Umetanje u desno podstablo ako je novi element veći od čvora.
    if (TargetElement->value > CurrentElement->value) {
        // Ako je desni element NULL, umetni tu.
        if (CurrentElement->right == NULL) {
            CurrentElement->right = TargetElement;
            return 0;
        }
        // Inače nastavi tražiti mjesto.
        InsertRecursively(CurrentElement->right, TargetElement);
    }

    // Umetanje u lijevo podstablo ako je novi element manji od čvora.
    if (TargetElement->value < CurrentElement->value) {
        // Ako je lijevi element NULL, umetni tu.
        if (CurrentElement->left == NULL) {
            CurrentElement->left = TargetElement;
            return 0;
        }
        // Inače nastavi tražiti mjesto.
        InsertRecursively(CurrentElement->left, TargetElement);
    }

    // Ako naiđemo na element s istom vrijednosti...
    if (TargetElement->value == CurrentElement->value) {
        printf("\t Element already exists!\n");
        return 1;
    }

    return 0;
}

void PrintMenu() {
    printf("1) Insert element\n");
    printf("2) Print tree\n");
    printf("3) Find element\n");
    printf("4) Delete element\n");
    printf("x) Prettyprint\n");
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

int XYToIndex(int X, int Y, int Width) {
    // Funkcija za pretvaranje koordinata u indeks. Ovako 1D nizom možemo upravljati kao sa 2D
    // matricom.
    return Y + Width * X;
}

void Populate(Position Node, int Level, int *Matrix, int First, int LevelCount, int X, int Y, int Width){
    // Rekurzivna unkcija za popunjavanje matrice iz stabla.

    // Posebni slučaj za korijen stabla.
    if (Level == 1) {
        Matrix[XYToIndex(0, First, Width)] = Node->value;
        Populate(Node, Level+1, Matrix, First, LevelCount, 0, First, Width);
    }
    // Svi ostali elementi.
    else {
        // Prostorni pomak: varijabla za pozicioniranje elemenata u matrici tako da se ne sijeku.
        // Ovisi o ukupnom broju razina i o razini u kojoj se rekurzija trenutno nalazi.
        int K = ceil(3 * pow(2,LevelCount - Level - 1)); 

        // Postavljanje vrijednosti lijevog i desnog dijeteta.
        if (Node->left) 
            Matrix[XYToIndex(X+K, Y-K, Width)] = Node->left->value;
        if (Node->right) 
            Matrix[XYToIndex(X+K, Y+K, Width)] = Node->right->value;

        // Širenje rekurzije na lijevo i desno dijete. Razinu povećavamo za 1 i šaljemo nove
        // koordinate.
        if (Level != LevelCount + 1) {
            if (Node->left) 
                Populate(Node->left, Level+1, Matrix, First, LevelCount, X+K, Y-K, Width);
            if (Node->right) 
                Populate(Node->right, Level+1, Matrix, First, LevelCount, X+K, Y+K, Width);
        }
        
        return;
    }
}

int CalculateLevels(Position Tree) {
    // Funkcija za računanje razina stabla.

    if (Tree == NULL)
        return 0;
    else
        return max(CalculateLevels(Tree->left), CalculateLevels(Tree->right)) + 1;
}

void PrettyPrint(Position Tree) {
    // Funkcija za 2D printanje stabla.

    // Priprema: 
    int LevelCount = CalculateLevels(Tree); // Broj razina u 2D stablu.
    int MatrixWidth = 3 * pow(2, LevelCount-1) - 1; // Širina zadnje razine u matrici.
    int MatrixHeight = (MatrixWidth + 1)/2; // Visina matrice.
    int First = (MatrixWidth - 1)/2; // Y koordinata korijena.
    int *Matrix = malloc(sizeof(int) * MatrixWidth * MatrixHeight); // Matrica za realizaciju 2D prikaza.

    // Popuni matricu s nulama...
    int i, y;
    for (i = 0; i < MatrixHeight * MatrixWidth; i++) {
        Matrix[i] = 0;
    }

    // Popuni matricu s vrijednostima stabla...
    Populate(Tree, 1, Matrix, First, LevelCount, 0, 0, MatrixWidth);

    // Ispiši matricu.
    for (i = 0; i < MatrixHeight; i++) {
        for (y = 0; y < MatrixWidth; y++) {
            int Key = Matrix[XYToIndex(i, y, MatrixWidth)];

            if (Key != 0) printf("%d", Key);
            else printf(" ");
        }
        printf("\n");
    }
    
}
