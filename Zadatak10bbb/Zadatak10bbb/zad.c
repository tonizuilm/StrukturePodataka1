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
#define MAX_LEN 50
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _town;
typedef struct _town* TownPosition;
typedef struct _town {
	char name[MAX_LEN];
	int population;
	TownPosition next;
} Town;

struct _country;
typedef struct _country* CountryPosition;
typedef struct _country {
	char name[MAX_LEN];
	TownPosition TownHead;
	CountryPosition left;
	CountryPosition right;
} Country;

CountryPosition CreateNewCountry(char name[]);
TownPosition CreateNewTown(char name[], int population);

//otvara i cita datoteku drzave.txt te stvar astablo istih
CountryPosition ReadCountyFileAndCreateCountryTree(CountryPosition CountryRoot);
//otvara i cita datoteku sa gradovima te stavara listu istih
TownPosition ReadTownFileAndCreateTownList(TownPosition FirstTown, char TownFileNAme[]);
// ubaciva drzabe u stablo sortirano po imenu te ona otvara datoteku sa gradovima te ih sortira u listu
CountryPosition InsertCountrySorted(CountryPosition CountryRoot, char CountryName[], char TownFileName[]);
//ubacuje gradove sortirano u listu
TownPosition InsertTownSorted(TownPosition FirstTown, char TownName[], int Population);
//ispisuje drzave u inorder poretku i ispisuje listu gradova
int PrintCountryInOrederAndTown(CountryPosition CountryRoot);
//ispisuje listu gradova
int PrintTown(TownPosition FirstTown);

//trazi odredenu drzavu te ispisuje njene gradove koji imaju broj stanovnika veci od zadanog
int FindCountryAndPrintTowns(CountryPosition CountryRoot);

int main() {

	CountryPosition CountryRoot = NULL;


	CountryRoot = ReadCountyFileAndCreateCountryTree(CountryRoot);

	PrintCountryInOrederAndTown(CountryRoot);

	FindCountryAndPrintTowns(CountryRoot);

	return EXIT_SUCCESS;
}

CountryPosition CreateNewCountry(char name[]) {
	CountryPosition NewCountry = NULL;

	NewCountry = (CountryPosition)malloc(sizeof(Country));

	if (!NewCountry) {
		printf("Error fail to alllocate memory for new country!\n");
		return NULL;
	}

	NewCountry->left = NULL;
	NewCountry->right = NULL;
	strcpy(NewCountry->name, name);
	NewCountry->TownHead = NULL;

	return NewCountry;
}
TownPosition CreateNewTown(char name[], int population) {
	TownPosition NewTown = NULL;

	NewTown = (TownPosition)malloc(sizeof(Town));

	if (!NewTown) {
		printf("Error fail to allocate memory for new town!\n");
		return NULL;
	}

	NewTown->next = NULL;
	strcpy(NewTown->name, name);
	NewTown->population = population;
	NewTown->next = NULL;

	return NewTown;
}

//otvara i cita datoteku drzave.txt te stvar astablo istih
CountryPosition ReadCountyFileAndCreateCountryTree(CountryPosition CountryRoot) {
	char CountryName[MAX_LEN] = "0";
	char TownFileName[MAX_LEN] = "0";

	FILE* filepointer = NULL;

	filepointer = fopen("drzave.txt", "r");

	if (!filepointer) {
		printf("Error fail to open file drzave.txt!\n");
		return NULL;
	}

	while (fscanf(filepointer, " %s %s", CountryName, TownFileName) == 2) {
		CountryRoot = InsertCountrySorted(CountryRoot, CountryName, TownFileName);
	}

	return CountryRoot;
}

//otvara i cita datoteku sa gradovima te stavara listu istih
TownPosition ReadTownFileAndCreateTownList(TownPosition FirstTown, char TownFileName[]) {
	TownPosition NewTown = NULL;
	char TownName[MAX_LEN] = "0";
	int Population = 0;

	FILE* filepointer = NULL;

	filepointer = fopen(TownFileName, "r");

	if (!filepointer) {
		printf("Error fail to open file %s!\n", TownFileName);
		return NULL;
	}

	if (fscanf(filepointer, " %s %d", TownName, &Population) == 2) {
		FirstTown = CreateNewTown(TownName, Population);
	}

	while (fscanf(filepointer, " %s %d", TownName, &Population) == 2) {
		FirstTown = InsertTownSorted(FirstTown, TownName, Population);
	}

	return FirstTown;
}

// ubaciva drzave u stablo sortirano po imenu
CountryPosition InsertCountrySorted(CountryPosition CountryRoot, char CountryName[], char TownFileName[]) {
	CountryPosition CurrentCountry = CountryRoot;
	CountryPosition NewCountry = NULL;

	if (!CountryRoot) {
		NewCountry = CreateNewCountry(CountryName);
		NewCountry->TownHead = ReadTownFileAndCreateTownList(NewCountry->TownHead, TownFileName);
		return NewCountry;
	}

	if (strcmp(CurrentCountry->name, CountryName) > 0) {
		CurrentCountry->left = InsertCountrySorted(CurrentCountry->left, CountryName, TownFileName);
	}
	else {
		CurrentCountry->right = InsertCountrySorted(CurrentCountry->right, CountryName, TownFileName);
	}

	return CountryRoot;
}

//ubacuje gradove sortirano u listu
TownPosition InsertTownSorted(TownPosition FirstTown, char TownName[], int Population){
	TownPosition PrevTown = NULL;
	TownPosition NewTown = CreateNewTown(TownName, Population);;

	//gleda jel lista prazana i ako je ubaciva novi grad na prvo mjesto
	if (!FirstTown) {
		return NewTown;
	}

	//gleda jel populacija prvog grada veca od populacije novog grada
	if (FirstTown->population > Population) {
		NewTown->next = FirstTown;
		return NewTown;
	}

	PrevTown = FirstTown;

	while (PrevTown->next !=NULL && PrevTown->next->population<Population){
		PrevTown = PrevTown->next;
	}

	NewTown->next = PrevTown->next;
	PrevTown->next = NewTown;

	return FirstTown;
}

//ispisuje drzave u inorder poretku i ispisuje listu gradova
int PrintCountryInOrederAndTown(CountryPosition CountryRoot) {
	CountryPosition CurrentCountry = CountryRoot;

	if (CurrentCountry->left != NULL) {
		PrintCountryInOrederAndTown(CurrentCountry->left);
	}

	printf("%s\n", CurrentCountry->name);
	PrintTown(CurrentCountry->TownHead);

	if (CurrentCountry->right != NULL) {
		PrintCountryInOrederAndTown(CurrentCountry->right);
	}

	return EXIT_SUCCESS;
}

//ispisuje listu gradova
int PrintTown(TownPosition FirstTown) {
	TownPosition CurrentTown = FirstTown;

	if (!CurrentTown) {
		printf("Error empty town list!\n");
		return EXIT_FAILURE;
	}

	while (CurrentTown) {
		printf("	%s %d\n", CurrentTown->name, CurrentTown->population);
		CurrentTown = CurrentTown->next;
	}

	return EXIT_SUCCESS;
}

//trazi odredenu drzavu te ispisuje njene gradove koji imaju broj stanovnika veci od zadanog
int FindCountryAndPrintTowns(CountryPosition CountryRoot) {
	CountryPosition CurrentCountry = CountryRoot;
	TownPosition CurrentTown = NULL;
	char CountryName[MAX_LEN] = "0";
	int Population = 0;

	printf("Enter Country Name:\n");
	scanf(" %s", CountryName);

	while (CurrentCountry && strcmp(CurrentCountry->name, CountryName)!=0) {
		if (strcmp(CurrentCountry->name, CountryName) > 0)
			CurrentCountry = CurrentCountry->left;
		else if (strcmp(CurrentCountry->name, CountryName) < 0)
			CurrentCountry = CurrentCountry->right;
	}

	if (CurrentCountry == NULL) {
		printf("%s does not exist!\n", CountryName);
		return EXIT_FAILURE;
	}

	CurrentTown = CurrentCountry->TownHead;

	printf("Enter Population:\n");
	scanf(" %d", &Population);

	while (CurrentTown && CurrentTown->population < Population) {
		CurrentTown = CurrentTown->next;
	}

	PrintTown(CurrentTown);

	return EXIT_SUCCESS;
}