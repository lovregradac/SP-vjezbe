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
Position FindParent(Position Tree, Position Child);
Position FindMinElement(Position StartingElement);

int DeleteChildFromParent(Position Tree, Position Child);
Position InsertRecursively(Position CurrentElement, Position TargetElement);
Position DeleteElement(Position Tree, Position TargetElement);
int PreOrder(Position Tree);

void ClearTree(Position Tree);

// Ostale funkcije.

void PrintMenu();

// Eksperimentalne prettyprint funkcije.

int CalculateLevels(Position Tree);
int XYToIndex(int X, int Y, int Width);

void Populate(Position Node, int Level, int *Matrix, int LevelCount, int X, int Y, int Width);
void PrettyPrint(Position Tree);
void AddLines(int *Matrix, int Width, int Height);

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

            PrettyPrint(Tree);

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

            PrettyPrint(Tree);

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
    printf("x) Prettyprint\n");
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

int XYToIndex(int X, int Y, int Width) {
    // Funkcija za pretvaranje koordinata u indeks. Ovako 1D nizom možemo upravljati kao sa 2D
    // matricom.
    return Y + Width * X;
}

void Populate(Position Node, int Level, int *Matrix, int LevelCount, int X, int Y, int Width){
    // Rekurzivna unkcija za popunjavanje matrice iz stabla.

    // Posebni slučaj za korijen stabla.
    if (Level == 1) {
        int FirstY = (Width - 1)/2;
        Matrix[XYToIndex(0, FirstY, Width)] = Node->value;
        Populate(Node, Level+1, Matrix, LevelCount, 0, FirstY, Width);
    }
    // Svi ostali elementi.
    else {
        // Prostorni pomak K: varijabla za pozicioniranje elemenata u matrici tako da se ne sijeku.
        // Ovisi o ukupnom broju razina i o razini u kojoj se rekurzija trenutno nalazi. Određuje
        // koordinate lijevog i desnog dijeteta trenutnog elementa.

        /*          (x, y)              L = n (level)
                    /    \              .
           (x+K, y-K)    (x+K, y+K)     L = n+1, K=f(n+1)=3 * 2^(L_max - L - 1)
                .             .         .
                .             .         .
                .             .         L = L_max
        */

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
                Populate(Node->left, Level+1, Matrix, LevelCount, X+K, Y-K, Width);
            if (Node->right) 
                Populate(Node->right, Level+1, Matrix, LevelCount, X+K, Y+K, Width);
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

void AddLines(int *Matrix, int Width, int Height) {
    // Funkcija za dodavanje linija u 2D matricu stabla.

    int i, y;
    for (i = 1; i < Height; i++) {
        for (y = 1; y < Width-1; y++) {
            int A = Matrix[XYToIndex(i-1, y-1, Width)]; // Gornje lijevo polje u matrici.
            int B = Matrix[XYToIndex(i-1, y+1, Width)]; // Gornje desno polje u matrici.
            int C = Matrix[XYToIndex(i, y, Width)]; // Trenutno polje.

            // 0 je rezervirana za prazna polja, -1 za \ i -2 za /. Ako trenutno polje nije ništa od
            // toga, preskačemo.
            if (C != 0 && C != -1 && C != -2) 
                continue;

            // Provjeravamo uvjete za nastavak grananja. Ako su zadovoljeni, trenutno polje
            // označavamo kao liniju (-1 ili -2, ovisno o poziciji u odnosu na element ili prethodnu
            // liniju).
            if (A != 0 && A != -2)
                // Ako gornje lijevo polje nije prazno niti je /, trenutno polje postaje \.
                Matrix[XYToIndex(i, y, Width)] = -1;
            if (B != 0 && B != -1)
                // Ako gornje desno polje nije prazno niti je \, trenutno polje postaje /.
                Matrix[XYToIndex(i, y, Width)] = -2; 
        }
    }

    // Očisti višak linija na zadnjoj razini...
    for (i = 0; i < Width; i++) {
        int A = Matrix[XYToIndex(Height-1, i, Width)];

        if (A == -1 || A == -2)
            Matrix[XYToIndex(Height-1, i, Width)] = 0;
    }
}

void PrettyPrint(Position Tree) {
    // Funkcija za 2D printanje stabla.

    // todo: Padding kod velikih brojeva.

    if (NULL == Tree)
        // Šta ću ispisat kralju???
        return;
    
    // Priprema: 
    int LevelCount = CalculateLevels(Tree); // Broj razina u 2D stablu.
    int MatrixWidth = 3 * pow(2, LevelCount-1) - 1; // Širina zadnje razine u matrici.
    int MatrixHeight = (MatrixWidth + 1)/2; // Visina matrice.
    int *Matrix = malloc(sizeof(int) * MatrixWidth * MatrixHeight); // Matrica za realizaciju 2D prikaza.

    // Popuni matricu s nulama...
    int i, y;
    for (i = 0; i < MatrixHeight * MatrixWidth; i++)
        Matrix[i] = 0;

    // Popuni matricu s vrijednostima stabla...
    Populate(Tree, 1, Matrix, LevelCount, 0, 0, MatrixWidth);

    // Dodaj linije...
    AddLines(Matrix, MatrixWidth, MatrixHeight);

    // Ispiši matricu...
    for (i = 0; i < MatrixHeight; i++) {
        for (y = 0; y < MatrixWidth; y++) {
            int Key = Matrix[XYToIndex(i, y, MatrixWidth)]; // Trenutno mjesto u matrici.

            if (Key == -1) printf("\\"); // -1 je linija \.
            else if (Key == -2) printf("/"); // -2 je linija /.
            else if (Key != 0) printf("%d", Key); // Ostalo osim 0 je element.
            else printf(" "); // 0 je prazno mjesto.
        }
        printf("\n");
    }
    
    free(Matrix);
}
