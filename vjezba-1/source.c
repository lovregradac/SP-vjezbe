#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct _student;
typedef struct _student {
    char * ime[20];
    char * prezime[20];
    float bodovi;
} Student;

int CountRows(char* fileName);
int LoadStudents(char* fileName, Student* studenti);
int PrintStudents(Student * studenti, int rowCount, float maxPoints);
float FindMaxPoints(Student * studenti, int rowCount);

int main() {
    char * fileName = "studenti.txt";
    int rows = CountRows(fileName);
    Student * studenti = (Student * ) malloc(rows * sizeof(Student));

    LoadStudents(fileName, studenti);
    PrintStudents(studenti, rows, FindMaxPoints(studenti, rows));

    free(studenti);

    return 0;
}

int CountRows(char * filename) {
    FILE * f;
    char buffer[255];
    int rowsCounted = 0;

    f = fopen(filename, "r");
    
    if (NULL == f) {
        printf("Error!");
        return -1;
    }
    
    while (fgets(buffer, 255, f))
        rowsCounted++;
        
    fclose(f);

    return rowsCounted;
}

int LoadStudents(char * filename, Student* studenti) {
    FILE * f;
    char buffer[255];
    int i = 0;
    
    f = fopen(filename, "r");
    
    if (f == NULL) {
        printf("Error!");
        return -1;
    }
    
    while (fgets(buffer, 255, f)) {
        sscanf(buffer, "%20s %20s %f", studenti[i].ime, studenti[i].prezime, & (studenti)[i].bodovi);
        i++;
    }
    
    fclose(f);
    
    return 0;
}

int PrintStudents(Student * studenti, int rowCount, float maxPoints) {
    int i;
    
    for (i = 0; i < rowCount; i++)
        printf("%s\t%s\t%.2f\t%.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, (float)(studenti[i].bodovi * 100 / maxPoints));
        
    return 0;
}

float FindMaxPoints(Student * studenti, int rowCount) {
    int i;
    float maxPoints = 0;
	
    for (i = 0; i < rowCount; i++)
          if (studenti[i].bodovi > maxPoints)
               maxPoints = studenti[i].bodovi;
    
    return maxPoints;
}
