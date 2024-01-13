/*
2.
Definirati strukturu osoba(ime, prezime, godina rodenja) i napisati program koji :
A.dinamicki dodaje novi element na pocetak liste,
B.ispisuje listu,
C.dinamicki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.brise određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

3. Prethodnom zadatku dodati funkcije:
A. dinamicki dodaje novi element iza određenog elementa,
B. dinamicki dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. cita listu iz datoteke.

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 54

struct _person;
typedef struct _person* Position;
typedef struct _person {
	char name[MAXLINE];
	char surname[MAXLINE];
	int year;
	Position next;
} Person;

Position CreateNewPerson();
Position FindPersonBefore(Position head);
int Menu(Position head);
int AddNewPersonToBeginning(Position head);
int AddNewPersonToEnd(Position head);
int Print(Position head);
int DeletePerson(Position head);
int AddPersonAfter(Position head);
int AddPersonBefore(Position head);
int PrintToFile(Position head);
int ReadFromFile(Position head);

int main() {
	Person head = { .name = {0}, .surname = {0}, .year = 0, .next = NULL };

	Menu(&head);

	return EXIT_SUCCESS;
}

Position CreateNewPerson() {
	Position NewPerson = NULL;

	NewPerson = (Position)malloc(sizeof(Person));

	if (!NewPerson) {
		printf("Error in create person!\n");
		return NULL;
	}

	printf("Unesi ime prezime i godinu rodenja osobe:\n");
	scanf("%s %s %d", NewPerson->name, NewPerson->surname, &NewPerson->year);

	NewPerson->next = NULL;

	return NewPerson;
}

Position FindPersonBefore(Position head) {
	Position Current = head;
	char TargetSurname [MAXLINE] = {0};

	if (!head->next) {
		printf("Empty list!n");
		return NULL;
	}

	printf("Enter Surname:\n");
	scanf("%s", TargetSurname);

	while (Current->next) {
		if (strcmp(Current->next->surname, TargetSurname) == 0) {
			return Current;
		}
		Current = Current->next;
	}
	printf("Person does not exist\n");
	return NULL;
}

int Menu(Position head) {

	int flag = 1;
	char selected = '0';
	Position TargetPerson = NULL;

	while (flag) {
		printf("\nMENU\nenter what you want to do\n\na - Add new person to the begining\ne - Add new person to the end of the list\np - Print list\nf - Find person\nd - Delete person\nb - Add person beafore\nn - Add person after\nt - Print to file\nr - Read from file\nx - Exit \n");
		scanf(" %c", &selected);

		switch (selected)
		{
		case 'a':
			AddNewPersonToBeginning(head);
			break;
		case 'e':
			AddNewPersonToEnd(head);
			break;
		case 'p':
			Print(head);
			break;
		case 'f':
			TargetPerson = FindPersonBefore(head)->next;
			if(TargetPerson)
				printf("%s %s %d", TargetPerson->name, TargetPerson->surname, TargetPerson->year);
			break;
		case 'd':
			DeletePerson(head);
			break;
		case 'b':
			AddPersonBefore(head);
			break;
		case 'n':
			AddPersonAfter(head);
			break;
		case 't':
			PrintToFile(head);
			break;
		case 'r':
			ReadFromFile(head);
			break;
		case 'x':
			flag = 0;
			break;
		default:
			printf("Wrong enter!\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}

int AddNewPersonToBeginning(Position head) {

	Position NewPerson = CreateNewPerson();

	NewPerson->next = head->next;
	head->next = NewPerson;

	return EXIT_SUCCESS;
}

int AddNewPersonToEnd(Position head) {
	Position Current = head;

	while (Current->next != NULL) {
		Current = Current->next;
	}

	Current->next = CreateNewPerson();

	return EXIT_SUCCESS;
}

int Print(Position head) {
	Position Current = head->next;

	if (!head->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current) {
		printf("%s %s %d\n", Current->name, Current->surname, Current->year);
		Current = Current->next;
	}

	return EXIT_SUCCESS;
}

int DeletePerson(Position head) {

	Position DeletePersonPrev = FindPersonBefore(head);
	Position DeletePerson = DeletePersonPrev->next;
	DeletePersonPrev->next = DeletePersonPrev->next->next;

	free(DeletePerson);

	return EXIT_SUCCESS;
}

int AddPersonAfter(Position head) {

	Position TargetPerson = FindPersonBefore(head)->next;

	AddNewPersonToBeginning(TargetPerson);

	return EXIT_SUCCESS;
}

int AddPersonBefore(Position head) {

	Position TargetPersonPrev = FindPersonBefore(head);

	AddNewPersonToBeginning(TargetPersonPrev);

	return EXIT_SUCCESS;
}

int PrintToFile(Position head) {
	Position Current = head->next;

	if (!head->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	FILE *filepointer = NULL;

	filepointer = fopen("Osobe.txt", "w");

	if (!filepointer) {
		printf("Error in open file!\n");
		return EXIT_FAILURE;
	}

	while (Current) {
		fprintf(filepointer, "%s %s %d\n", Current->name, Current->surname, Current->year);
		Current = Current->next;
	}

	fclose(filepointer);
	return EXIT_SUCCESS;
}
int ReadFromFile(Position head) {
	Person Temp = { .name = {0}, .surname = {0}, .next = NULL };

	FILE* filepointer = NULL;

	filepointer = fopen("Osobe.txt", "r");

	if (!filepointer) {
		printf("Error in open file!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, "%s %s %d", Temp.name, Temp.surname, &Temp.year)==3) {
		Position NewPerson = NULL;

		NewPerson = (Position)malloc(sizeof(Person));

		if (!NewPerson) {
			printf("Error in create person!\n");
			return NULL;
		}
		
		strcpy(NewPerson->name, Temp.name);
		strcpy(NewPerson->surname, Temp.name);
		NewPerson->year = Temp.year;

		NewPerson->next = head->next;
		head->next = NewPerson;
	}

	fclose(filepointer);
	return EXIT_SUCCESS;
}

