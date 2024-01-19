/*
11. Prepraviti zadatak 10 na nacin da se formira hash tablica drzava. Tablica ima 11 mjesta, a
funkcija za preslikavanje kljuc racuna da se zbraja ASCII vrijednost prvih pet slova drzave zatim
racuna ostatak cjelobrojnog dijeljenja te vrijednosti s velicinom tablice. Drzave s istim kljucem se
pohranjuju u vezanu listu sortiranu po nazivu drzave. Svaki cvor vezane liste sadrzi stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#define _CRT_SECURE_NO_WARNINGS
#define MAX_LEN 50
#define HASH_TABLE_SIZE 11

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _town;
typedef struct _town* TownPosition;
typedef struct _town {
	char Name[MAX_LEN];
	int Population;
	TownPosition left;
	TownPosition right;
} Town;

struct _country;
typedef struct _country* CountryPosition;
typedef struct _country {
	char Name[MAX_LEN];
	TownPosition TownHead;
	CountryPosition next;
} Country;

typedef struct _hash_table {
	CountryPosition bucket[HASH_TABLE_SIZE];
} Hash_Table;


TownPosition CreateNewTown(char Name[], int Population);
CountryPosition CreateNewCountry(char Name[]);
Hash_Table* CreateHashTable();


//cita datoteku drzave.txt 
int ReadCountyFile(Hash_Table* HashTable);

TownPosition ReadTownFile(char TownFileName[MAX_LEN]);

TownPosition InsertTownInTree(TownPosition Root, char TownName[], int Population);

//ispisuje listu drzava
int PrintCountry(CountryPosition FirstCountry);
int PrintInOrder(TownPosition Root);

//find country and print town
int FindCountryAndPrintTown(Hash_Table* HashTable);

int PrintInOrderOverPopulation(TownPosition Root, int TargetPopulation);

int CalculateHash(char Name[]);

int main() {

	Hash_Table* HashTable = CreateHashTable();


	ReadCountyFile(HashTable);


	for(int i=0; i<11;i++)
		if(HashTable->bucket[i] != NULL)
			PrintCountry(HashTable->bucket[i]);

	FindCountryAndPrintTown(HashTable);


	return EXIT_SUCCESS;
}


TownPosition CreateNewTown(char Name[], int Population) {
	TownPosition NewTown = NULL;

	NewTown = (TownPosition)malloc(sizeof(Town));

	if (!NewTown) {
		printf("Error fail to allocate memory for new town!\n");
		return NULL;
	}

	strcpy(NewTown->Name, Name);
	NewTown->Population = Population;
	NewTown->left = NULL;
	NewTown->right = NULL;

	return NewTown;
}

CountryPosition CreateNewCountry(char Name[]) {
	CountryPosition NewCountry = NULL;

	NewCountry = (CountryPosition)malloc(sizeof(Country));

	if (!NewCountry) {
		printf("Error fail to allocate memory for new country!\n");
		return NULL;
	}
	
	strcpy(NewCountry->Name, Name);
	NewCountry->next = NULL;
	NewCountry->TownHead = NULL;

	return NewCountry;
}

Hash_Table* CreateHashTable() {
	Hash_Table* NewHashTable = NULL;

	NewHashTable = (Hash_Table*)malloc(sizeof(Hash_Table) * HASH_TABLE_SIZE);

	if (!NewHashTable) {
		printf("Error fail to allocate memory for hash table!\n");
		return NULL;
	}

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		NewHashTable->bucket[i] = NULL;

	return NewHashTable;
}

int ReadCountyFile(Hash_Table* HashTable) {
	char CountryName[MAX_LEN] = "0";
	char TownFileName[MAX_LEN] = "0";
	int HashNumber = 0;

	FILE* filepointer = NULL;

	filepointer = fopen("drzave.txt", "r");

	if (!filepointer) {
		printf("Errontt fail to open file drzave.txt!\n");
		return EXIT_FAILURE;
	}


	while (fscanf(filepointer, " %s %s", CountryName, TownFileName) == 2) {
		CountryPosition NewCountry = CreateNewCountry(CountryName);
		HashNumber = CalculateHash(CountryName);
		NewCountry->TownHead = ReadTownFile(TownFileName);

		if (HashTable->bucket[HashNumber] == NULL) {
			HashTable->bucket[HashNumber] = NewCountry;
		}
		else {
			NewCountry->next = HashTable->bucket[HashNumber]->next;
			HashTable->bucket[HashNumber]->next = NewCountry;
		}
	}
	
	fclose(filepointer);
	return EXIT_SUCCESS;
}

int CalculateHash(char Name[]) {
	int HashNumber = 0;

	for (int i = 0; i < 5; i++) {
		if(Name[i]!='\0')
			HashNumber += (int)Name[i];
	}

	return HashNumber % HASH_TABLE_SIZE;
}

TownPosition ReadTownFile(char TownFileName[MAX_LEN]) {
	TownPosition Root = NULL;
	char TownName[MAX_LEN] = "0";
	int Population = 0;

	FILE* filepointer = NULL;

	filepointer = fopen(TownFileName, "r");

	if (!filepointer) {
		printf("Error fail to open %s file!\n", TownFileName);
		return NULL;
	}

	while (fscanf(filepointer, " %s %d", TownName, &Population) == 2) {
		Root = InsertTownInTree(Root, TownName, Population);
	}

	return Root;
}

TownPosition InsertTownInTree(TownPosition Root, char TownName[], int Population) {

	if (Root == NULL) {
		return CreateNewTown(TownName, Population);
	}
	if (Root->Population > Population) {
		Root->left = InsertTownInTree(Root->left, TownName, Population);
	}
	else if (Root->Population < Population) {
		Root->right = InsertTownInTree(Root->right, TownName, Population);
	}
	else if (strcmp(Root->Name, TownName) > 0) {
		Root->left = InsertTownInTree(Root->left, TownName, Population);
	}
	else {
		Root->right = InsertTownInTree(Root->right, TownName, Population);
	}
	return Root;
}

int PrintCountry(CountryPosition FirstCountry) {
	CountryPosition CurrentCountry = FirstCountry;

	if (!FirstCountry) {
		printf("Empty list!\n\n");
		return EXIT_FAILURE;
	}

	while (CurrentCountry) {
		printf("%s\n", CurrentCountry->Name);

		if (CurrentCountry->TownHead != NULL)
			PrintInOrder(CurrentCountry->TownHead);
		else
			printf("Error %s does not have towns\n", CurrentCountry->Name);

		CurrentCountry = CurrentCountry->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int PrintInOrder(TownPosition Root) {

	if (Root->left != NULL)
		PrintInOrder(Root->left);

	printf("	%s %d\n", Root->Name, Root->Population);

	if (Root->right != NULL)
		PrintInOrder(Root->right);

	return EXIT_SUCCESS;
}

int FindCountryAndPrintTown(Hash_Table* HashTable) {
	char TargetCountryName[MAX_LEN] = "0";
	int TargetHashNumber = 0;
	int TargetPopulation = 0;
	CountryPosition CurrentCountry = NULL;

	printf("Enter target country name:\n");
	scanf(" %s", TargetCountryName);

	TargetHashNumber = CalculateHash(TargetCountryName);

	CurrentCountry = HashTable->bucket[TargetHashNumber];

	while(CurrentCountry && strcmp(CurrentCountry->Name,TargetCountryName)){
		CurrentCountry = CurrentCountry->next;
	}

	if (!CurrentCountry) {
		printf("%s does not exist!\n", TargetCountryName);
		return EXIT_FAILURE;
	}

	printf("Enter Population:\n");
	scanf(" %d", &TargetPopulation);

	PrintInOrderOverPopulation(CurrentCountry->TownHead, TargetPopulation);

	return EXIT_SUCCESS;
}

int PrintInOrderOverPopulation(TownPosition Root, int TargetPopulation) {

	if (Root->left != NULL)
		PrintInOrderOverPopulation(Root->left, TargetPopulation);

	if (Root->Population > TargetPopulation) 
		printf("	%s %d\n", Root->Name, Root->Population);

	if (Root->right != NULL)
		PrintInOrderOverPopulation(Root->right, TargetPopulation);

	return EXIT_SUCCESS;

}