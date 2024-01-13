/*
6. Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
racun izdat u formatu YYYY-MM-DD. Svaki sljedeci red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena. Potrebno je formirati vezanu listu racuna sortiranu po datumu. Svaki cvor
vezane liste sadrzava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
artikl u odredenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>4
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024



struct _article;
typedef struct _article* ArticlePosition;
typedef struct _article {
	char ArticleName[50];
	int Amount;
	int Price;
	ArticlePosition next;
} Article;

struct _receipt;
typedef struct _receipt* ReceiptPosition;
typedef struct _receipt {
	char ReceiptDate[11];
	Article ArticleHead;
	ReceiptPosition next;
} Receipt;

ReceiptPosition CreateNewReceipt(char ReceiptDate[]);
ArticlePosition CreateNewArticle(char ArticleName[], int Amount, int Price);
//cita imena racuna i stvara listu racuna te ih popunjava
int ReadFile_CreteReceiptList(ReceiptPosition ReceiptHead);
//prima ime racuna "racun1.txt" otvara tu datoteku, stvara novi racun te vraca taj racun
ReceiptPosition CreateNewReceipt_FillReceipt(char NameOfReceiptFile[]);
//ubacuje sortirano
int InsertReceiptSorted(ReceiptPosition ReceiptHead, ReceiptPosition NewReceipt);
int InsertArticleSorted(ArticlePosition ArticleHead, ArticlePosition NewArticle);
//ispisuje listu racunna i artikle svakog racuna
int PrintReceipt(ReceiptPosition ReceiptHead);
//ispisuje artikle racuna
int PrintReceiptArticle(ArticlePosition Articlehead);
//koliko je odredenih artikala kupljeno u odredenom vremenskom razdoblju i koliko su placeni
int ArticlesBought(ReceiptPosition ReceiptHead);

int main() {
	Receipt ReceiptHead = { .ArticleHead = {.Amount = 0, .ArticleName = {0}, .next = NULL, .Price = 0},.next = NULL,.ReceiptDate = {0} };

	ReadFile_CreteReceiptList(&ReceiptHead);

	PrintReceipt(&ReceiptHead);

	ArticlesBought(&ReceiptHead);

	return EXIT_SUCCESS;
}

ReceiptPosition CreateNewReceipt(char ReceiptDate[]) {
	ReceiptPosition NewReceipt = NULL;

	NewReceipt = (ReceiptPosition)malloc(sizeof(Receipt));

	if (!NewReceipt) {
		printf("Error in allocating memory for receipt!\n");
		return NULL;
	}

	strcpy(NewReceipt->ReceiptDate, ReceiptDate);

	NewReceipt->ArticleHead.next = NULL;
	NewReceipt->ArticleHead.Amount = 0;
	NewReceipt->ArticleHead.Price = 0;

	NewReceipt->next = NULL;

	return NewReceipt;
}


ArticlePosition CreateNewArticle(char ArticleName[], int Amount, int Price) {
	ArticlePosition NewArticle = NULL;

	NewArticle = (ArticlePosition)malloc(sizeof(Article));

	if (!NewArticle) {
		printf("Error in allocating memory for article!\n");
		return NULL;
	}

	strcpy(NewArticle->ArticleName, ArticleName);

	NewArticle->next = NULL;
	NewArticle->Amount = Amount;
	NewArticle->Price = Price;

	return NewArticle;
}

//cita imena racuna i stvara listu racuna te ih popunjava
int ReadFile_CreteReceiptList(ReceiptPosition ReceiptHead) {
	char NameOfReceiptFile[50] = { 0 };
	ReceiptPosition NewReceipt = NULL;

	FILE* filepointer = NULL;

	filepointer = fopen("racuni.txt", "r");

	if (!filepointer) {
		printf("Error in open file in function ReadFile_CreteReceiptList\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filepointer, " %s", NameOfReceiptFile) == 1) {
		NewReceipt= CreateNewReceipt_FillReceipt(NameOfReceiptFile);
		InsertReceiptSorted(ReceiptHead, NewReceipt);
		
	}

	fclose(filepointer);
	return EXIT_SUCCESS;
}

//prima ime racuna "racun1.txt" otvara tu datoteku, stvara novi racun te vraca taj racun
ReceiptPosition CreateNewReceipt_FillReceipt(char NameOfReceiptFile[]) {
	ReceiptPosition NewReceipt = NULL;
	ArticlePosition NewArticle = NULL;
	char ReceiptDate[11] = { 0 };
	char ArticleName[50] = { 0 };
	int Amount = 0, Price = 0;

	FILE* filepointer = NULL;

	filepointer = fopen(NameOfReceiptFile, "r");

	if (!filepointer) {
		printf("Error in open file in function CreateNewReceipt_FillReceipt\n");
		return EXIT_FAILURE;
	}

	fscanf(filepointer, " %s", ReceiptDate);

	NewReceipt = CreateNewReceipt(ReceiptDate);

	while (fscanf(filepointer, " %s %d %d", ArticleName, &Amount, &Price) == 3) {
		NewArticle = CreateNewArticle(ArticleName, Amount, Price);

		InsertArticleSorted(&NewReceipt->ArticleHead, NewArticle);
	}
	return NewReceipt;
}

int InsertReceiptSorted(ReceiptPosition ReceiptHead, ReceiptPosition NewReceipt) {
	ReceiptPosition PrevReceipt = ReceiptHead;

	if (!ReceiptHead->next) {
		ReceiptHead->next = NewReceipt;
		return EXIT_SUCCESS;
	}

	while (PrevReceipt->next !=NULL && strcmp(NewReceipt->ReceiptDate, PrevReceipt->next->ReceiptDate) > 0) {
		PrevReceipt = PrevReceipt->next;
	}

	if (PrevReceipt->next == NULL) {
		PrevReceipt->next = NewReceipt;
		return EXIT_SUCCESS;
	}

	NewReceipt->next = PrevReceipt->next;
	PrevReceipt->next = NewReceipt;

	return EXIT_SUCCESS;
}
int InsertArticleSorted(ArticlePosition ArticleHead, ArticlePosition NewArticle) {
	ArticlePosition PrevArticle = ArticleHead;

	if (!PrevArticle->next) {
		PrevArticle->next = NewArticle;
		return EXIT_SUCCESS;
	}

	while (PrevArticle->next != NULL && strcmp(NewArticle->ArticleName, PrevArticle->next->ArticleName) > 0) {
		PrevArticle = PrevArticle->next;
	}

	if (PrevArticle->next == NULL) {
		PrevArticle->next = NewArticle;
		return EXIT_SUCCESS;
	}

	NewArticle->next = PrevArticle->next;
	PrevArticle->next = NewArticle;

	return EXIT_SUCCESS;
}

//ispisuje listu racunna
int PrintReceipt(ReceiptPosition ReceiptHead) {
	ReceiptPosition CurrentReceipt = ReceiptHead->next;

	if (!CurrentReceipt) {
		printf("Error, empty receipt list");
		return EXIT_FAILURE;
	}

	while (CurrentReceipt) {
		printf("%s\n", CurrentReceipt->ReceiptDate);
		PrintReceiptArticle(&CurrentReceipt->ArticleHead);
		CurrentReceipt = CurrentReceipt->next;
	}

	return EXIT_SUCCESS;

}

int PrintReceiptArticle(ArticlePosition Articlehead) {
	ArticlePosition CurrentArticle = Articlehead->next;

	if (!CurrentArticle) {
		printf("Error, empty article list");
		return EXIT_FAILURE;
	}

	while (CurrentArticle) {
		printf("	%s %d %d\n", CurrentArticle->ArticleName, CurrentArticle->Amount, CurrentArticle->Price);
		CurrentArticle = CurrentArticle->next;
	}

	return EXIT_SUCCESS;
}

int ArticlesBought(ReceiptPosition ReceiptHead) {
	char ReceiptDate1[11] = { 0 }, ReceiptDate2[11] = { 0 };
	char ArticleName[50] = { 0 };
	int NumberOfArticles = 0, MoneySpent = 0;

	ReceiptPosition CurrentReceipt = ReceiptHead->next;
	ArticlePosition CurrentArticle = NULL;

	if (!ReceiptHead) {
		printf("Empty Receipt list!\n");
		return EXIT_FAILURE;
	}

	printf("\nEnter two dates(YYYY-MM-DD):\n ");
	scanf(" %s", ReceiptDate1);
	scanf(" %s", ReceiptDate2);

	printf("What article are you searching for?\n");
	scanf(" %s", ArticleName);

	//Ovde se prolazi kroz svaki racun
	while (CurrentReceipt) {
		//ovde se provjerava jel odredeni racun unutar određenog intervala
		if (strcmp(CurrentReceipt->ReceiptDate, ReceiptDate1) >= 0 && strcmp(CurrentReceipt->ReceiptDate, ReceiptDate2) <= 0) {
			CurrentArticle = &CurrentReceipt->ArticleHead;

			//ovde se prolazi kroz svaki artikl
			while (CurrentArticle) {
				//provjera jel to taj artikl
				if (strcmp(CurrentArticle->ArticleName, ArticleName) == 0) {
					NumberOfArticles = NumberOfArticles + CurrentArticle->Amount;
					MoneySpent = MoneySpent + CurrentArticle->Price;
				}
				CurrentArticle = CurrentArticle->next;
			}
		}
		CurrentReceipt = CurrentReceipt->next;
	}

	printf("in the period from %s to %s, %d %s worth %d were sold", ReceiptDate1, ReceiptDate2, NumberOfArticles, ArticleName, MoneySpent);
	return EXIT_SUCCESS;
}
