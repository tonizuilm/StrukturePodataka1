/*
Napisati program koji prvo procita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamicki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i ucitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrzi ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE (50)
#define MAX_LINE (1024)
#define FILE_ERROR_OPEN (-1)

typedef struct _student {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;
} Student;

int NumOfStud();
int ReadFromFile(Student* Studenti, int NumOfSt);
int Print(Student* Studenti, int NumOfSt);

int main() {
	int NumOfSt = NumOfStud();

	Student* Studenti = (Student*) malloc(NumOfSt * sizeof(Student));

	ReadFromFile(Studenti, NumOfSt);

	Print(Studenti, NumOfSt);

	return 0;
}

int NumOfStud() {
	int Counter= 0;
	Student temp = { .name = {0}, .surname = {0}, .points = 0 };
	FILE* filepointer = NULL;
	

	filepointer = fopen("Studenti.txt", "r");

	if (!filepointer) {
		printf("Error in open file!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, "%s %s %lf", temp.name, temp.surname, &temp.points) == 3)
		Counter++;

	fclose(filepointer);
	return Counter;
}

int ReadFromFile(Student* Studenti, int NumOfSt) {
	int i = 0;

	FILE* filepointer = NULL;

	filepointer = fopen("Studenti.txt", "r");

	if (!filepointer) {
		printf("Error in open file!\n");
		return EXIT_FAILURE;
	}

	for (i = 0; i < NumOfSt; i++) {
		fscanf(filepointer, "%s %s %lf", Studenti[i].name, Studenti[i].surname, &Studenti[i].points);
	}

	return EXIT_SUCCESS;
}

int Print(Student* Studenti, int NumOfSt) {
	int i = 0;
	int MaxPoints = 0;
	//Nalazi max br bodova
	for (i = 0; i < NumOfSt; i++) {
		if (Studenti[i].points > MaxPoints) {
			MaxPoints = Studenti[i].points;
		}
	}

	for (i = 0; i < NumOfSt; i++) {
		printf("%s %s %lf %lf\n", Studenti[i].name, Studenti[i].surname, Studenti[i].points, Studenti[i].points/MaxPoints*100);
	}

	return EXIT_SUCCESS;

}