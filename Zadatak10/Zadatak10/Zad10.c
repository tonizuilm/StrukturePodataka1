/*
10. Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih drzava. Uz
ime drzave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
drzave. Svaka datoteka koja predstavlja drzavu sadrzi popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu drzava po nazivu. Svaki cvor vezane liste
sadrzi stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo drzava sortirano po nazivu. Svaki cvor stabla sadrzi vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati drzave i gradove te omoguciti korisniku putem
tastature pretragu gradova odredene drzave koji imaju broj stanovnika veci od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _town;
typedef struct _town* TownPosition;
typedef struct _town {
	char name[50];
	int population;
	TownPosition left;
	TownPosition right;
} Town;

struct _country;
typedef struct _country* CountryPosition;
typedef struct _country {
	char name[50];
	TownPosition TownRootTree;
	CountryPosition next;
} Country;

CountryPosition CreateNewCountry(char name[]);
TownPosition CreateNewTown(char name[], int population);
//cita datoteku drzave.txt te stvara listu od procitanih drzava, te poziva funkciju koja stvara stablo gradova
int ReadCountryFromFile(CountryPosition CountryHead);
CountryPosition InsertCountrySorted(CountryPosition CountryHead, char CountryName[]); //vraca adresu ubacene drzave
//ispisuje drzave i gradove inorder
int PrintCountryAndTown(CountryPosition CountryHead);
//prima ime datoteke sa gradovima otvara je i stvara stablo gradova
TownPosition ReadTownFromFile(TownPosition TownHead, char TownFileName[]);
// ubacuje gradove u stablo
TownPosition InsertTown(TownPosition TownHead, char TownName[], int TownPopulation);
//ispisuje gradove inorder
int InOrder(TownPosition TownHead);
//trazi oredenu drzavu te ispisuje sve gradove drzave koji imaju broj stanovnika veci od zadanog
int Find(CountryPosition CountryHead);
//trazi i ispisuje gradove koji imaju broj stanovnika veci od zadanog
int InOrderTownPopulation(TownPosition TownHead, int Population);


int main() {
	Country CountryHead = { .name = {0}, .next = NULL, .TownRootTree = NULL };

	ReadCountryFromFile(&CountryHead);

	PrintCountryAndTown(&CountryHead);

	Find(&CountryHead);

	return EXIT_SUCCESS;
}

CountryPosition CreateNewCountry(char name[]) {
	CountryPosition NewCountry = NULL;

	NewCountry = (CountryPosition)malloc(sizeof(Country));

	if (!NewCountry) {
		printf("Error in allocating memory");
		return NULL;
	}

	strcpy(NewCountry->name, name);
	NewCountry->TownRootTree = NULL;
	NewCountry->next = NULL;

	return NewCountry;
}

TownPosition CreateNewTown(char name[], int population) {
	TownPosition NewTown = NULL;

	NewTown = (TownPosition)malloc(sizeof(Town));

	if (!NewTown) {
		printf("Error in allocating memory");
		return NULL;
	}

	NewTown->left = NULL;
	NewTown->right = NULL;
	strcpy(NewTown->name, name);
	NewTown->population = population;

	return NewTown;
}

CountryPosition InsertCountrySorted(CountryPosition CountryHead, char CountryName[]) {
	CountryPosition NewCountry = NULL;
	CountryPosition PrevCountry = CountryHead;

	if (!CountryHead->next) {
		CountryHead->next = CreateNewCountry(CountryName);
		return CountryHead->next;
	}
	
	while (PrevCountry->next != NULL && strcmp(PrevCountry->next->name, CountryName) < 0) {
		PrevCountry = PrevCountry->next;
	}

	NewCountry = CreateNewCountry(CountryName);
	NewCountry->next = PrevCountry->next;
	PrevCountry->next = NewCountry;

	return NewCountry;
}

//cita datoteku drzave.txt te stvara listu od procitanih drzava, te poziva funkciju koja stvara stablo gradova
int ReadCountryFromFile(CountryPosition CountryHead) {
	CountryPosition NewCountry = NULL;
	char TownFileName[50] = { 0 };
	char CountryName[50] = { 0 };
	FILE* filepointer=NULL;

	filepointer = fopen("drzave.txt", "r");

	if (!filepointer) {
		printf("Error in open file drzave.txt");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, " %s %s", CountryName, TownFileName) == 2) {
		
		NewCountry = InsertCountrySorted(CountryHead, CountryName);
		NewCountry->TownRootTree = ReadTownFromFile(NewCountry->TownRootTree, TownFileName);

	}

	return EXIT_SUCCESS;
}

//ispisuje drzave
int PrintCountryAndTown(CountryPosition CountryHead) {
	CountryPosition CurrentCountry = CountryHead->next;

	if (!CurrentCountry) {
		printf("List is empty!\n");
		return EXIT_SUCCESS;
	}

	while (CurrentCountry){
		printf("%s\n", CurrentCountry->name);
		InOrder(CurrentCountry->TownRootTree);
		CurrentCountry = CurrentCountry->next;
	}
	return EXIT_SUCCESS;
}

//prima ime datoteke sa gradovima otvara je i stvara stablo gradova
TownPosition ReadTownFromFile(TownPosition TownHead, char TownFileName[]) {
	char TownName[50] = { 0 };
	int TownPopulation = 0;
	FILE* filepointer = NULL;
	int flag = 0;

	filepointer = fopen(TownFileName, "r");

	if (!filepointer) {
		printf("Error fail to open file in ReadTownFromFile\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, " %s %d", TownName, &TownPopulation) == 2) {
		if (flag == 0) {
			TownHead=InsertTown(TownHead, TownName, TownPopulation);
			flag = 1;
		}
		else if (flag == 1) {
			InsertTown(TownHead, TownName, TownPopulation);
		}
	}
	flag = 0;
	return TownHead;
}

// ubacuje gradove u stablo
TownPosition InsertTown(TownPosition TownHead, char TownName[], int TownPopulation) {
	TownPosition CurrentTown = TownHead;

	if (CurrentTown == NULL) {
		return CreateNewTown(TownName, TownPopulation);
	}

	if (CurrentTown->population > TownPopulation) {
		CurrentTown->left = InsertTown(CurrentTown->left, TownName, TownPopulation);
	}
	else if (CurrentTown->population < TownPopulation) {
		CurrentTown->right = InsertTown(CurrentTown->right, TownName, TownPopulation);
	}
	else if (CurrentTown->population == TownPopulation) {
		if (strcmp(CurrentTown->name, TownName) > 0) {
			CurrentTown->left = InsertTown(CurrentTown->left, TownName, TownPopulation);
		}
		else if (strcmp(CurrentTown->name, TownName) < 0) {
			CurrentTown->right = InsertTown(CurrentTown->right, TownName, TownPopulation);
		}
	}
	
	return CurrentTown;
}

int InOrder(TownPosition TownHead) {

	if (TownHead->left != NULL) {
		InOrder(TownHead->left);
	}
	printf("	%s %d\n", TownHead->name, TownHead->population);
	
	if (TownHead->right != NULL) {
		InOrder(TownHead->right);
	}
	return EXIT_SUCCESS;
}

//trazi oredenu drzavu te ispisuje sve gradove drzave koji imaju broj stanovnika veci od zadanog
int Find(CountryPosition CountryHead) {
	CountryPosition CurrentCountry = CountryHead->next;
	char CountryName[50] = { 0 };
	int Population = 0;


	printf("Enter Country Name:\n ");
	scanf(" %s", CountryName);

	while (CurrentCountry && strcmp(CurrentCountry->name, CountryName) != 0) {
		CurrentCountry = CurrentCountry->next;
	}

	if (CurrentCountry == NULL) {
		printf("%s does not exist!\n");
		return EXIT_FAILURE;
	}
	else if (strcmp(CurrentCountry->name, CountryName) == 0) {

		printf("Enter Town Population:\n ");
		scanf(" %d", &Population);

		InOrderTownPopulation(CurrentCountry->TownRootTree, Population);
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

//trazi i ispisuje gradove koji imaju broj stanovnika veci od zadanog
int InOrderTownPopulation(TownPosition TownHead, int Population) {

	if (TownHead->left != NULL) {
		InOrderTownPopulation(TownHead->left, Population);
	}
	if(TownHead->population>Population)
		printf("	%s %d\n", TownHead->name, TownHead->population);

	if (TownHead->right != NULL) {
		InOrderTownPopulation(TownHead->right, Population);
	}
	return EXIT_SUCCESS;
}

