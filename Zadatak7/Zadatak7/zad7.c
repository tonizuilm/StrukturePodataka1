/*
7. Napisati program koji pomocu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguciti unos novih direktorija i pod-direktorija, ispis sadrzaja direktorija i
povratak u prethodni direktorij. Tocnije program treba preko menija simulirati
koristenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/

#define _CRT_SECURE_NO_WARNINGS
#define MAX_NAME_LENGHT 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct _directory;
typedef struct _directory* Position;
typedef struct _directory {
	char name[MAX_NAME_LENGHT];
	Position subDirectories;
	Position next;
} Directory;

struct _stog;
typedef struct _stog* StogPosition;
typedef struct _stog {
	Position adress;
	StogPosition next;
} Stog;

Position CreateNewDirectory(char name[]);
int menu(Position RootDirectory, StogPosition StogHead);
int CreateSubDirectory(Position RootDirectory);
Position ChangeDirectory(Position RootDirectory);
int PrintSubDirectories(Position RootDirectory);

StogPosition CreateNewElement(Position adresa);
Position Pop(StogPosition StogHead);
int Push(StogPosition StogHead, Position adress);

int main() {
	Directory Head = { .name = {0}, .next = NULL, .subDirectories = NULL };
	Stog StogHead = { .next = NULL, .adress = NULL };

	Position RootDirectory = CreateNewDirectory("C:");
	Head.subDirectories = RootDirectory;

	StogPosition NewElement = CreateNewElement(RootDirectory);
	StogHead.next = NewElement;


	menu(RootDirectory, &StogHead);
	return EXIT_SUCCESS;
}

Position CreateNewDirectory(char name[]) {
	Position NewDirectory = NULL;

	NewDirectory = (Position)malloc(sizeof(Directory));

	if (!NewDirectory) {
		printf("Error in CreateNewDirectory!");
		return NULL;
	}

	strcpy(NewDirectory->name, name);
	NewDirectory->subDirectories = NULL;
	NewDirectory->next = NULL;
}

int menu(Position RootDirectory, StogPosition StogHead) {
	Position CurrentDirectory = RootDirectory;
	int selection = 0;

	while (1) {
		printf("MENU:\n");
		printf("1 - md (Create Directory)\n");
		printf("2 - cd dir (Change Directory)\n");
		printf("3 - cd.. (Go Up)\n");
		printf("4 - dir (List Contents)\n");
		printf("5 - exit\n");

		scanf(" %d", &selection);

		switch (selection) {
		case 1:
			CreateSubDirectory(CurrentDirectory);
			break;
		case 2:
			Push(StogHead, CurrentDirectory);
			CurrentDirectory = ChangeDirectory(CurrentDirectory);
			break;
		case 3:
			if (CurrentDirectory != RootDirectory)
				CurrentDirectory = Pop(StogHead);
			else
				printf("You are in RootDirectory!\n");
			break;
		case 4:
			PrintSubDirectories(CurrentDirectory);
			break;
		case 5:
			return EXIT_SUCCESS;
			break;
		default:
			printf("Wrong enter\n");
			break;
		}
	}
}

int CreateSubDirectory(Position RootDirectory) {
	char name[MAX_NAME_LENGHT] = { 0 };
	Position NewSubDirectory = NULL;

	printf("Enter new directory name:\n");
	scanf(" %s", name);

	NewSubDirectory = CreateNewDirectory(name);

	NewSubDirectory->next = RootDirectory->subDirectories;
	RootDirectory->subDirectories = NewSubDirectory;

	return EXIT_SUCCESS;
}

Position ChangeDirectory(Position RootDirectory) {
	Position CurrentSubDirectory = RootDirectory->subDirectories;
	char name[MAX_NAME_LENGHT] = { 0 };

	printf("Enter subdirectory name:\n");
	scanf(" %s", name);

	while (CurrentSubDirectory->next != NULL && strcmp(CurrentSubDirectory, name) != 0) {
		CurrentSubDirectory = CurrentSubDirectory->next;
	}

	if (strcmp(CurrentSubDirectory, name) == 0) {
		return CurrentSubDirectory;
	}

	printf("%s does not exist!\n", name);
	return RootDirectory;
}

int PrintSubDirectories(Position RootDirectory) {
	Position CurrentSubDirectory = RootDirectory->subDirectories;

	if (!CurrentSubDirectory) {
		printf("Empty directory!\n");
		return EXIT_SUCCESS;
	}

	while (CurrentSubDirectory) {
		printf("%s\n", CurrentSubDirectory->name);
		CurrentSubDirectory = CurrentSubDirectory->next;
	}

	return EXIT_SUCCESS;
}

StogPosition CreateNewElement(Position adresa) {
	StogPosition NewElement = NULL;

	NewElement = (StogPosition)malloc(sizeof(Stog));

	if (!NewElement) {
		printf("Error in allocate stog memory");
		return NULL;
	}

	NewElement->adress = adresa;
	NewElement->next = NULL;

	return NewElement;
}

Position Pop(StogPosition StogHead) {
	StogPosition ToDelete = StogHead->next;
	Position adress = ToDelete->adress;

	StogHead->next = StogHead->next->next;

	free(ToDelete);
	return adress;
}

int Push(StogPosition StogHead, Position adress) {
	StogPosition NewStog = CreateNewElement(adress);

	NewStog->next = StogHead->next;
	StogHead->next = NewStog;

	return EXIT_SUCCESS;
}
