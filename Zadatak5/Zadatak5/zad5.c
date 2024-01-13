/*
 5. Napisati program koji iz datoteke cita postfiks izraz i zatim koristenjem stoga racuna
rezultat. Stog je potrebno realizirati preko vezane liste
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGHT 1024

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement {
	int number;
	Position next;
} StackElement;

Position CreateNewElement(int number);
int ReadFromFile(char* buffer);
int push(Position head, int number);
int pop(Position head);
int print(Position head);
int Pop_DoOperation_push(Position head, char operation);
int StringToStack(Position head, char* buffer);


int main() {
	char* buffer[MAX_LENGHT] = { 0 };
	StackElement head = { .next = NULL, .number = 0 };

	ReadFromFile(buffer);

	StringToStack(&head, buffer);

	print(&head);

	return 0;
}

Position CreateNewElement(int Number) {
	Position NewElement = NULL;

	NewElement = (Position)malloc(sizeof(StackElement));

	NewElement->number = Number;
	NewElement->next = NULL;

	return NewElement;
}

int ReadFromFile(char* buffer) {
	
	FILE* filepointer = NULL;

	filepointer = fopen("postfix.txt", "r");

	if (!filepointer) {
		printf("Error in file open!\n");
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LENGHT, filepointer);
	printf("%s\n", buffer);

	fclose(filepointer);
	return EXIT_SUCCESS;
}

int push(Position head, int number) {
	Position NewElement = NULL;
	NewElement=CreateNewElement(number);
	NewElement->next = head->next;
	head->next = NewElement;

	return EXIT_SUCCESS;
}

int pop(Position head) {
	Position ToDelete = NULL;
	int Number = 0;

	if (!head->next) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	ToDelete = head->next;
	Number = ToDelete->number;

	head->next = head->next->next;

	free(ToDelete);
	return Number;
}

int print(Position head) {
	Position Current = head->next;

	if (!Current) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current) {
		printf("%d\n", Current->number);
		Current = Current->next;
	}

	return EXIT_SUCCESS;
}

int Pop_DoOperation_push(Position head, char operation) {
	int Number2 = pop(head);
	int Number1 = pop(head);
	int result = 0;

	switch (operation) {
	case '+':
		result = Number1 + Number2;
		break;
	case '-':
		result = Number1 - Number2;
		break;
	case '*':
		result = Number1 * Number2;
		break;
	case '/':
		result = Number1 / Number2;
		break;
	default:
		return EXIT_FAILURE;
		break;
	}

	push(head, result);

	return EXIT_SUCCESS;
}

int StringToStack(Position head, char* buffer) {
	char* CurrentBuffer = buffer;
	Position NewElement = NULL;
	int NumberBytes = 0, status = 0, CurrentNumber = 0;
	char operation = 0;

	while (strlen(CurrentBuffer)>0) {
		NumberBytes = 0;
		status = sscanf(CurrentBuffer, " %d %n", &CurrentNumber, &NumberBytes);
		if (status == 1) {
			push(head, CurrentNumber);
		}
		else if (status != 1) {
			sscanf(CurrentBuffer, " %c %n", &operation, &NumberBytes);
			Pop_DoOperation_push(head, operation);
		}
		CurrentBuffer = CurrentBuffer + NumberBytes;

	}
	return EXIT_SUCCESS;
}