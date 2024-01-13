/*
4. Napisati program za zbrajanje i mnozenje polinoma. Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _element;
typedef struct _element* Position;
typedef struct _element {
	int Koeficient;
	int Exponent;
	Position next;
} Element;

Position CreateNewElement(int Koeficient, int Exponent);
int ReadPoliFromFile(Position head1, Position head2);
int PrintPoli(Position head);
int InsertSorted(Position head, int Koeficient, int Exponent);
int Sum(Position sum, Position head1, Position head2);
int Multiply(Position multi, Position head1, Position head2);



int main() {

	Element head1 = { .Exponent = 0, .Koeficient = 0, .next = NULL };
	Element head2 = { .Exponent = 0, .Koeficient = 0, .next = NULL };
	Element sum = { .Exponent = 0, .Koeficient = 0, .next = NULL };
	Element mult = { .Exponent = 0, .Koeficient = 0, .next = NULL };

	ReadPoliFromFile(&head1, &head2);

	PrintPoli(&head1);
	PrintPoli(&head2);
	printf("\n");

	

	Sum(&sum, &head1, &head2);
	PrintPoli(&sum);

	Multiply(&mult, &head1, &head2);
	PrintPoli(&mult);

	return EXIT_SUCCESS;
}

Position CreateNewElement(int Koeficient, int Exponent) {
	Position NewElement = NULL;

	NewElement = (Position)malloc(sizeof(Element));

	if (!NewElement) {
		printf("Error in create new element!\n");
		return EXIT_FAILURE;
	}

	NewElement->Exponent = Exponent;
	NewElement->Koeficient = Koeficient;
	NewElement->next = NULL;

	return NewElement;
}

//read from file njegov naccin
/*
int  ReadFromFile(Position head1, Position head2) {
	int coefficient = 0, exponent = 0;
	char buffer[MAX_LINE] = { 0 };
	char* currentbuffer = buffer;
	int numBytes = 0;
	int status = 0;


	Position newelement = NULL;

	FILE* filepointer = NULL;
	filepointer = fopen("polynoms.txt", "r");

	if (!filepointer) {
		printf("Error!");
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LINE, filepointer);

	while (sscanf(currentbuffer, " %dx^%d %n", &coefficient, &exponent, &numBytes) && strlen(currentbuffer) > 0) {

		newelement = NewElement(coefficient, exponent);
		if (!newelement) {
			return EXIT_FAILURE;
		}

		newelement->next = head2->next;
		head2->next = newelement;
		currentbuffer +=numBytes;
	}

	fclose(filepointer);
	return EXIT_SUCCESS;
}
*/

int ReadPoliFromFile(Position head1, Position head2) {
	int Exponent = 0, Koeficient = 0;
	char temp = '0';
	Position NewElement = NULL;

	FILE* filepointer = NULL;

	filepointer = fopen("polinomi.txt", "r");

	if (!filepointer) {
		printf("Error in open file!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, "%dx^%d", &Koeficient, &Exponent) == 2) {
		NewElement = CreateNewElement(Koeficient, Exponent);
		NewElement->next = head1->next;
		head1->next = NewElement;
		fscanf(filepointer, "%c", &temp);
		if (temp =='\n' || temp == 'EOF') {
			break;
		}
	}

	while (fscanf(filepointer, "%dx^%d", &Koeficient, &Exponent) == 2) {
		NewElement = CreateNewElement(Koeficient, Exponent);
		NewElement->next = head2->next;
		head2->next = NewElement;
		fscanf(filepointer, "%c", &temp);
		if (temp == '\n' || temp == 'EOF') {
			break;
		}
	}

	return EXIT_SUCCESS;
	
}
int PrintPoli(Position head) {
	Position Current = head->next;
	
	if (!head->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current){
		printf("%dx^%d ", Current->Koeficient, Current->Exponent);
		Current = Current->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}
int InsertSorted(Position head, int Koeficient, int Exponent) {
	Position Prev = head;
	Position Del = NULL;
	Position NewElement = NULL;
	if (head->next == NULL) {
		NewElement = CreateNewElement(Koeficient, Exponent);
		head->next = NewElement;
	}
	else {
		while (Prev->next != NULL && Exponent > Prev->next->Exponent) {
			Prev = Prev->next;
		}
		if (Prev->next != NULL && Exponent == Prev->next->Exponent) {
			Prev->next->Koeficient = Prev->next->Koeficient + Koeficient;
			if (Prev->next->Koeficient == 0) {
				Del = Prev->next;
				Prev->next = Prev->next->next;
				free(Del);
			}
		}
		else {
			NewElement = CreateNewElement(Koeficient, Exponent);
			NewElement->next = Prev->next;
			Prev->next = NewElement;
		}
	}

	return 0;
}

int Sum(Position Sum, Position head1, Position head2) {
	Position Current = head1->next;

	if (!head1->next || !head2->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current) {
		InsertSorted(Sum, Current->Koeficient, Current->Exponent);
		Current = Current->next;
	}

	Current = head2->next;

	while (Current) {
		InsertSorted(Sum, Current->Koeficient, Current->Exponent);
		Current = Current->next;
	}


	return EXIT_SUCCESS;
}
int Multiply(Position Multi, Position head1, Position head2) {
	Position Current1 = head1->next, Current2 = head2->next;

	if (!head1->next || !head2->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current1) {
		while (Current2!=NULL) {
			InsertSorted(Multi, Current1->Koeficient*Current2->Koeficient, Current1->Exponent+Current2->Exponent);
			Current2 = Current2->next;
		}
		Current1 = Current1->next;
		Current2 = head2->next;
	}

	return EXIT_SUCCESS;
}