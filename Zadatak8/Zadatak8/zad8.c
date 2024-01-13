/*
8. Napisati program koji omogucava rad s binarnim stablom pretrazivanja.Treba
omoguciti unosenje novog elementa u stablo, ispis elemenata(inorder, preorder, postorder i
level order), brisanje i pronalazenje nekog elementa.
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

struct _red;
typedef struct _red* RedPosition;
typedef struct _red {
	Position node;
	RedPosition next;
} Red;

Position CreateNewNode(int value);
RedPosition CreateNewElement(Position node);
Position Insert(Position Root, int value);
int InOrder(Position Root);
int PostOrder(Position Root);
int PreOrder(Position Root);
int LevelOrder(Position Root, RedPosition Head);
int Delete(Position Root);
int Find(Position Root);

int PrintRed(RedPosition Head);
int PushRed(RedPosition Head, Position node);

int main() {
	Red head = { .next = NULL, .node = NULL };
	Position Root = NULL;

	Root = Insert(Root, 10);

	Insert(Root, 5);
	Insert(Root, 3);
	Insert(Root, 4);
	Insert(Root, 7);
	Insert(Root, 3);
	Insert(Root, 8);
	Insert(Root, 3);
	Insert(Root, 15);
	Insert(Root, 12);
	Insert(Root, 14);
	Insert(Root, 4);

	InOrder(Root);
	printf("\n");
	PostOrder(Root);
	printf("\n");
	PreOrder(Root);
	printf("\n");
	LevelOrder(Root, &head);
	printf("\n");

	Delete(Root);
	PreOrder(Root);

	Find(Root);



	return EXIT_SUCCESS;
}

Position CreateNewNode(int value) {
	Position NewNode = NULL;

	NewNode = (Position)malloc(sizeof(Node));

	if (!NewNode) {
		printf("Error in allocating memory\n");
		return NULL;
	}

	NewNode->left = NULL;
	NewNode->right = NULL;
	NewNode->value = value;

	return NewNode;
}

RedPosition CreateNewElement(Position node) {
	RedPosition NewElement = NULL;

	NewElement = (RedPosition)malloc(sizeof(Red));

	if (!NewElement) {
		printf("Error failed to allocate memory!\n");
		return NULL;
	}

	NewElement->next = NULL;
	NewElement->node = node;

	return NewElement;
}

Position Insert(Position Root, int value) {

	if (Root == NULL)
		return CreateNewNode(value);

	else if (value > Root->value)
		Root->right = Insert(Root->right, value);

	else
		Root->left = Insert(Root->left, value);

	return Root;
}

//livi trenutni desni
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

//livi desni trenutni
int PostOrder(Position Root) {

	if (Root->left != NULL) {
		PostOrder(Root->left);
	}

	if (Root->right != NULL) {
		PostOrder(Root->right);
	}

	printf("%d, ", Root->value);

	return EXIT_SUCCESS;
}

//trenutni ljevi desni
int PreOrder(Position Root) {

	printf("%d, ", Root->value);

	if (Root->left != NULL) {
		PreOrder(Root->left);
	}

	if (Root->right != NULL) {
		PreOrder(Root->right);
	}
	return Root;
}

//po rezini najviši ide prvi pa onda ostali
int LevelOrder(Position root, RedPosition Head) {
	RedPosition CurrentRed = Head;
	PushRed(Head, root);

	while (CurrentRed->next != NULL) {
		CurrentRed = CurrentRed->next;

		if (CurrentRed->node->left != NULL) {
			PushRed(Head, CurrentRed->node->left);
		}

		if (CurrentRed->node->right != NULL) {
			PushRed(Head, CurrentRed->node->right);
		}
	}

	PrintRed(Head);

	return EXIT_SUCCESS;


}

int Delete(Position Root) {
	Position CurrentNode = Root;
	Position Prev = NULL;
	int TargetNode = 0;

	printf("Whaat nmber you want to delete?\n");
	scanf("%d", &TargetNode);

	while (CurrentNode !=NULL && TargetNode != CurrentNode->value){
		if (TargetNode > CurrentNode->value) {
			Prev = CurrentNode;
			CurrentNode = CurrentNode->right;
		}
		else if (TargetNode < CurrentNode->value) {
			Prev = CurrentNode;
			CurrentNode = CurrentNode->left;
		}
	}
	//ako ne postoji
	if (CurrentNode == NULL) {
		printf("Target number does not exist!\n");
		return EXIT_FAILURE;
	}
	//ako nema djece
	else if (CurrentNode->left==NULL && CurrentNode->right==NULL) {
		if (Prev->left == CurrentNode) {
			Prev->left = NULL;
		}
		else if (Prev->right == CurrentNode) {
			Prev->right = NULL;
		}
		free(CurrentNode);
	}
	//ako ima jedno dijete
	else if (CurrentNode->left == NULL || CurrentNode->right == NULL) {
		if (CurrentNode->left != NULL) {
			if (Prev->left == CurrentNode) {
				Prev->left = CurrentNode->left;
			}
			else if (Prev->right == CurrentNode) {
				Prev->right = CurrentNode->left;
			}
			free(CurrentNode);
		}
		else if (CurrentNode->right != NULL) {
			if (Prev->left == CurrentNode) {
				Prev->left = CurrentNode->right;
			}
			else if (Prev->right == CurrentNode) {
				Prev->right = CurrentNode->right;
			}
			free(CurrentNode);
		}
	}
	//ako ima dvoje djece
	else if (CurrentNode->left != NULL && CurrentNode->right != NULL) {
		Position ToMovePrev = CurrentNode;
		Position ToMove = CurrentNode->right;
		while (ToMove->left != NULL) {
			ToMovePrev = ToMove;
			ToMove = ToMove->left;
		}

		CurrentNode->value = ToMove->value;

		ToMovePrev->left = NULL;
		free(ToMove);

	}

	return EXIT_SUCCESS;

}
int Find(Position Root) {
	Position CurrentNode = Root;
	int TargetNumber = 0;
	Position TargetPosition = NULL;

	printf("Enter number you are looking for: \n");
	scanf("%d", &TargetNumber);


	while (CurrentNode != NULL && TargetNumber != CurrentNode->value) {
		if (TargetNumber > CurrentNode->value) {
			
			CurrentNode = CurrentNode->right;
		}
		else if (TargetNumber < CurrentNode->value) {

			CurrentNode = CurrentNode->left;
		}
	}

	if (CurrentNode == NULL) {
		printf("Target number does not exist!\n");
		return EXIT_FAILURE;
	}
	
	printf("Target Number is at: %d", CurrentNode);

	return EXIT_SUCCESS;
}


int PrintRed(RedPosition Head) {
	RedPosition Current = Head->next;

	if (Head->next == NULL) {
		printf("Empty list!\n");
		return EXIT_FAILURE;
	}

	while (Current) {
		printf("%d, ", Current->node->value);
		Current = Current->next;
	}
	return EXIT_SUCCESS;
}
int PushRed(RedPosition Head, Position node) {
	RedPosition Current = Head->next;

	if (Head->next == NULL) {
		Head->next = CreateNewElement(node);
		return EXIT_SUCCESS;
	}

	while (Current->next != NULL) {
		Current = Current->next;
	}
	Current->next = CreateNewElement(node);
	return EXIT_SUCCESS;
}