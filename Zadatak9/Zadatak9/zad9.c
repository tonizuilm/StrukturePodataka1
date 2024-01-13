/*
9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraca
pokazivac na korijen stabla.
b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat ce se u stablo na
slici Slika 2.
c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
funkcije rand() generira slucajne brojeve u rasponu <10, 90>. Takoder, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
dijela zadatka.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _node;
typedef struct _node* Position;
typedef struct _node {
	int value;
	Position left;
	Position right;
} Node;

Position CreteNewNode(int value);
Position Insert(Position Root, int value);
int Replace(Position Root);
int SumTree(Position Root);
int InOrder(Position Root);
int Random();

int main() {

	Position Root = NULL;

	Root = Insert(Root, 2);
	Insert(Root, 5);
	Insert(Root, 7);
	Insert(Root, 8);
	Insert(Root, 11);
	Insert(Root, 1);
	Insert(Root, 4);
	Insert(Root, 2);
	Insert(Root, 3);
	Insert(Root, 7);

	InOrder(Root);
	printf("\n");

	Replace(Root);
	InOrder(Root);

	printf("\n");
	printf("\n");

	Root = NULL;

	Root = Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());
	Insert(Root, Random());


	InOrder(Root);
	printf("\n");

	Replace(Root);
	InOrder(Root);

	printf("\n");
	printf("\n");


	return EXIT_SUCCESS;
}

Position CreteNewNode(int value) {
	Position NewNode = NULL;

	NewNode = (Position)malloc(sizeof(Node));

	if (!NewNode) {
		printf("Error in allocating memory!\n");
		return NULL;
	}

	NewNode->left = NULL;
	NewNode->right = NULL;
	NewNode->value = value;

	return NewNode;
}

Position Insert(Position Root, int value) {
	Position NewNode = CreteNewNode(value);

	if (Root == NULL) {
		return NewNode;
	}

	if (Root->value > value) {
		Root->left = Insert(Root->left, value);
	}
	else {
		Root->right = Insert(Root->right, value);
	}

	return Root;
}

int Replace(Position Root) {

	Root->value = SumTree(Root);

	if (Root->left != NULL)
		Replace(Root->left);

	if (Root->right != NULL)
		Replace(Root->right);

	return EXIT_SUCCESS;
}

int SumTree(Position Root) {
	int Sum = 0;

	if (Root->left != NULL) {
		Sum = Root->left->value + SumTree(Root->left);
	}

	if (Root->right != NULL) {
		Sum = Sum + Root->right->value + SumTree(Root->right);
	}

	return Sum;
}

int InOrder(Position Root) {

	if (Root->left != NULL) {
		InOrder(Root->left);
	}

	printf("%d, ", Root->value);

	if (Root->right != NULL) {
		InOrder(Root->right);
	}

	return EXIT_SUCCESS;
}

int Random() {
	//srand(time(NULL));

	return rand() % 80 + 10;
}