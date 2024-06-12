//Xochitl Triana & Mason Marrero
//12/5/21
//Input: a string for the name, the cards the user wants to hold, the bet
//Output: A text file with the users information
//		  the users cards
//		  if the user won
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//Struct of the card
typedef struct card_struct {
	int face;
	char suit;
	struct card_struct* next;
}card;

//Info of the player to be printed out later in text file
typedef struct player_struct {
	char name[20];
	int RFWins;
	int SFWins;
	int FoaKWins;
	int FHWins;
	int ToaKWins;
	int TPWins;
	int StrtWins;
	int FlushWins;
	int loses;

}player;

//Function to swap two cards
void swap(card* x, card* y) {
	int face = x->face;
	char suit = x->suit;
	x->face = y->face;
	x->suit = y->suit;
	y->face = face;
	y->suit = suit;
}

//Function to use bubble sort to sort the hand
void bubbleSort(card* deck) {
	int check = 1;
	int i;
	card* p;
	card* copy = NULL;
	
	while(check){
		check = 0;
		p = deck;
		while (p->next != copy) {
			if (p->face > p->next->face) {
				swap(p, p->next);
				check = 1;
			}
			p = p->next;
		}
		copy = p;
	}
}

//Function to check if a hand is straight or not. Will Return 1 if the deck is a straight, 0 if not
//Will work by going through the hand after it has been sorted and seeing if the previous card's face in the list is one less
//retunr 1 if its a straight, return 2 if its an Ace high straight
int isStraight(card* h) {
	int count = 1;
	int prev = h->face;
	//Check if there is the possibility of a straight with Ace high by checking if the first two cards are an A and 10
	if ((prev == 1) && (h->next->face == 10)) {
		h = h->next;
		prev = h->face;
		h = h->next;
		count++;
		while (h != NULL) {
			if (h->face == (prev + 1))
				count++;
			prev = h->face;
			h = h->next;
			if (count == 5)
				return 2; //There is a Ace high straight 
		}
	}
	else {
		while (h != NULL) {
			if (h->face == (prev + 1))
				count++;
			prev = h->face;
			h = h->next;
		}
		if (count == 5)
			return 1;//There is a straight
	}
	return 0;
}
//This function will check to see if there are any pairs in the hand. It does this by running a loop looking for a sequence of cards being 
//greater than 1 then there was a pair of some sort. If the sequence ended then it would look at if there was a second sequence. Depending on how long these
//sequences were, we could tell if the card was a Four of a kind, Three of a kind, Full house, or two pair
int checkPair(card* h) {
	int streak = 0;
	int streak2 = 0;
	int prev = h->face;
	//while (h != NULL) {
		//Will check for a streak
		while (h != NULL) {
			if (h->face == prev) {
				streak++;
				h = h->next;
			}
			else if(streak > 1){
				break;
			}
			else {
				streak = 1;
				prev = h->face;
				h = h->next;
			}
		}
		//Loop for the second streak if the linked list hasnt reached the end
		while (h != NULL) {
			if (h->face == prev) {
				streak2++;
				h = h->next;
			}
			else if (streak2 > 1) {
				break;
			}
			else {
				streak2 = 1;
				prev = h->face;
				h = h->next;
			}
			
		}
	if (streak == 4 || streak2 == 4) {
		return 1; //Four of a kind
	}
	else if ((streak == 2 && streak2 == 3) || (streak == 3 && streak2 == 2)) {
		return 2; // Full House
	}
	else if (streak == 3 || streak2 == 3) {
		return 3;//Three of a kind
	}
	else if (streak == 2 && streak2 == 2) {
		return 4;//two pair
	}
	return 0;
}

//Function to check if the function is a Flush. Will return 1 if the deck is a flush, 0 if not
//Will work by having a counter. Another variable char suit will be set to the suit of the first card in the list. 
//every card that matches this suit will add one to counter. If counter == 5 then its a flush
//return 1 if it is a flush
int isFlush(card* h) {
	int count = 0;
	char check = h->suit;
	while (h != NULL) {
		if (check == h->suit)
			count++;
		h = h->next;
	}
	if (count == 5)
		return 1;
	return 0;
}

//This is a checker that will be called, and it will check what the players hand is. It will check this in part with the isStraight(), and isFlush() functions
//it will first check if the player has a royal flush, if so it will return 1,both isflush and isstraight are true and it has an A and a K
//Straight Flush: 2, both isflush and isstraight are true
//Four of a kind: 3
//Full House: 4, possible can be checked with a counter and loops to see if the cards face match 
//Flush: 5, isFlush true
//Straight: 6, isStraight true
//Three of a kind: 7
//Two Pair: 8
//Nothing: 0
int checker(card* h) {
	int strt = isStraight(h); 
	int flush = isFlush(h);
	int pair = checkPair(h);
	//printf("%d %d %d", strt, flush, pair);
	if (strt == 2 && flush == 1)
		return 1; //Royal Flush
	else if (strt == 1 && flush == 1)
		return 2; //Straight Flush
	else if (pair == 1)
		return 3; // Four of a kind
	else if (pair == 2)
		return 4; //Full house
	else if (flush == 1)
		return 5;//flush
	else if (strt == 1 || strt == 2)
		return 6; // straight
	else if (pair == 3)
		return 7;//three of a kind
	else if (pair == 4)
		return 8; //two pair
	return 0;
}

//Will print the ascii art for the linked list passed to it.
void printAscii(card* p) {
	//3: heart
	//4: diamond
	//5: club
	//6: spade
	card* copy = p;
	for (int i = 0; i < 5; i++) {
		printf("----------\t");
	}
	printf("\n");
	for (int i = 0; i < 5; i++) {
		if(p->face == 11)
			printf("| %cJ     |\t", p->suit);
		else if (p->face == 12)
			printf("| %cQ     |\t", p->suit);
		else if (p->face == 13)
			printf("| %cK     |\t", p->suit);
		else if (p->face == 1)
			printf("| %cA     |\t", p->suit);
		else if (p->face == 10)
			printf("| %c%d    |\t", p->suit, p->face);
		else
			printf("| %c%d     |\t",p->suit, p->face);
		p = p->next;
	}
	p = copy;
	printf("\n");
	for (int i = 0; i < 5; i++) {
		printf("|        |\t");
	}
	printf("\n");
	for (int i = 0; i < 5; i++) {
		if (p->face == 11)
			printf("|     %cJ |\t", p->suit);
		else if (p->face == 12)
			printf("|     %cQ |\t", p->suit);
		else if (p->face == 13)
			printf("|     %cK |\t", p->suit);
		else if (p->face == 1)
			printf("|     %cA |\t", p->suit);
		else if (p->face == 10)
			printf("|    %c%d |\t", p->suit, p->face);
		else
			printf("|     %c%d |\t", p->suit, p->face);
		p = p->next;
	}
	printf("\n");
	for (int i = 0; i < 5; i++) {
		printf("----------\t");
	}
	printf("\n");
}

//Function to create the deck
card* createDeck() {
	card* headp = NULL, * temp, * tail = NULL;
	int count = 0;
	int faceC = 1;
	char suitC = 3;
	temp = (card*)malloc(sizeof(card));
	while (count < 52) {
		if (faceC < 14) {
			temp->face = faceC;
			temp->suit = suitC;
			faceC++;
			count++;
		}
		if (faceC == 14) {
			faceC = 1;
			suitC++;
		}
		if (headp == NULL) {
			headp = temp;
		}
		else {
			tail->next = temp;
		}
		tail = temp;
		tail->next = NULL;
		temp = (card*)malloc(sizeof(card));
	}
	free(temp);
	return headp;
}

////FUNCTION USED FOR TESTING FOR BUGS
//card* make() {
//	card* headp = NULL, * temp, * tail = NULL;
//	int count = 0;
//	int count2 = 1;
//	temp = (card*)malloc(sizeof(card));
//	while (count < 5) {
//		temp->face = count2;
//		temp->suit = 4;
//		count++;
//		count2++;
//
//		//count2++;
//		if (count == 1)
//			temp->suit = 3;
//		/*
//		else {
//			count2 = 5;
//			temp->suit = 3;
//		}*/
//		if (headp == NULL) {
//			headp = temp;
//		}
//		else {
//			tail->next = temp;
//		}
//		tail = temp;
//		tail->next = NULL;
//		temp = (card*)malloc(sizeof(card));
//	}
//	return headp;
//}

//Function to print a linked list of cards. used for testing purposes 
void print_card(card* p) {
	int count = 0;
	while (p != NULL) {
		if(p->face == 11)
			printf("%cJ ", p->suit);
		else if (p->face == 12)
			printf("%cQ ", p->suit);
		else if (p->face == 13)
			printf("%cK ", p->suit);
		else if (p->face == 1) {
			printf("%cA ", p->suit);
		}
		else
			printf("%c%d ", p->suit, p->face);
		count++;
		if (count == 13) {
			printf("\n");
			count = 0;
		}
		p = p->next;
	}
	if (count != 13)
		printf("\n");
}

//Function to shuffle cards
//p is the deck
void shuffle(card* p) {
	card* copy = p;
	card* copy2 = p;
	card* swap;
	char suit;
	int face;
	for (int i = 0; i < 51; i++) {
			copy = p;
			int num = rand() % (52-i);
			for (int i = 0; i < num; i++) {
				p = p->next;
				if (p == NULL)
					printf("Code Broke");
			}
			swap = p;
			face = p->face;
			suit = p->suit;
			p = copy;
			swap->face = p->face;
			swap->suit = p->suit;
			p->suit = suit;
			p->face = face;
			p = p->next;
	}
}

//Function to create the hand
//p is the deck passed to the function
card* createHand(card* p) {
	card* headp = NULL, * temp, * tail = NULL;
	int count = 0;
	temp = (card*)malloc(sizeof(card));
	while (count < 5) {
		temp->face = p->face;
		temp->suit = p->suit;
		count++;
		if (headp == NULL) {
			headp = temp;
		}
		else {
			tail->next = temp;
		}
		p = p->next;
		tail = temp;
		tail->next = NULL;
		temp = (card*)malloc(sizeof(card));
	}
	free(temp);
	return headp;
}

//Function that when called will ask the user what cards they want to hold, and will then replace them
//d is the deck, h is the hand
card* newCards(card** d, card* h) {
	card* temp, * tail = NULL;
	card* newD = NULL;
	card* copy = h;
	int count = 0;
	temp = (card*)malloc(sizeof(card));
	int count2 = 0;
	int pick;
	while (1) {
		if (count2 == 5) {
			break;
		}
		printf("Pick cards(between 1 - 5) to hold(-1 to stop): ");
		scanf("%d", &pick);
		if (pick == -1) {
			break;
		}
		else if (pick < 6 && pick >= 1) {
			count2++;
			for (int i = 0; i < pick -1; i++) {
				h = h->next;
			}
			temp->face = h->face;
			temp->suit = h->suit;
			count++;
			if (newD == NULL) {
				newD = temp;
			}
			else {
				tail->next = temp;
			}
			tail = temp;
			tail->next = NULL;
			h = copy;
			temp = (card*)malloc(sizeof(card));
		}
	}
	count = 5 - count; 
	//To replace the cards taken out of the hand with the ones in the deck
	for (int i = 0; i < count; i++) {
		temp->suit = (*d)->suit;
		temp->face = (*d)->face;
		if (newD == NULL) {
			newD = temp;
		}
		else {
			tail->next = temp;
		}
		tail = temp;
		tail->next = NULL;
		temp = (card*)malloc(sizeof(card));
		*d = (*d)->next;
	}
	free(temp);
	return newD;
}

//Function to count the cards in a linked list of cards
int countCards(card* c) {
	int count = 0;
	while (c != NULL) {
		c = c->next;
		count++;
	}
	return count;
}

int main(void) {
	player user;
	char name[20];
	int coins = 100;
	int bet;
	int wins = 0;
	srand((int)time(0));
	card* deck = NULL;
	card* hand = NULL;

	//Creation and shuffling of the deck
	deck = createDeck();
	shuffle(deck);
	for (int i = 0; i < 10000; i++) {
		shuffle(deck);
	}

	//Menu part
	printf("Enter your name: ");
	scanf(" %s", name);
	getchar();
	printf("$$$$$$$$$ $$$$$$$$$ $$$$$$$$$ $$$$$$$$$ $$$$$$$$$ \n");
	printf("\t  $$$$$$$$$ $$$$$$$$$ $$$$$$$$$  \n");
	printf("%s,\n\tLets Play Two Pairs or Better!\n", name);
	printf("\t  $$$$$$$$$ $$$$$$$$$ $$$$$$$$$  \n");
	printf("$$$$$$$$$ $$$$$$$$$ $$$$$$$$$ $$$$$$$$$ $$$$$$$$$ \n");
	printf("$$$$$$$$$         Rank of Winning       $$$$$$$$$ \n");
	printf("\t\tPay\n");
	printf("Roayal Flush\t%c10 %cJ %cQ %cK %cA\t250*bet\n", 4,4,4,4,4);
	printf("Straight Flush\t%c3 %c4 %c5 %c6 %c7\t100*bet\n", 4, 4,4,4,4);
	printf("Four of a Kind\t%c2 %c5 %c5 %c5 %c5\t50*bet\n",3, 3, 4, 5, 6);
	printf("Full House\t%c2 %c2 %c6 %c6 %c6\t25*bet\n", 4,5,5,3,4);
	printf("Flush\t\t%c_ %c_ %c_ %c_ %c_\t10*bet\n", 3,3,3,3,3);
	printf("Straight\t%c3 %c4 %c5 %c6 %c7\t5*bet\n",3,5,6,3,4);
	printf("Three of a Kind\t%c7 %c7 %c7 %cJ %cK\t4*bet\n",4,5,5,6,3);
	printf("Two Pair\t%c3 %c3 %c5 %c8 %c8\t2*bet\n",6,4,3,5,3);
	printf("\n");

	//Setting up the Struct for the player to be printed into the text file
	strcpy(user.name, name);
	user.FHWins = 0;
	user.loses = 0;
	user.RFWins = 0;
	user.SFWins = 0;
	user.FoaKWins = 0;
	user.ToaKWins = 0;
	user.TPWins = 0;
	user.FlushWins = 0;
	user.StrtWins = 0;

	//Loop to keep the game playing
	while (1) {
		printf("***********************************\n");
		printf("***%s, you have %d coins***\n", name, coins);
		printf("***********************************\n");
		while (1) {
			printf("Place your bet (1-%d) coins (-1 to quit playing): ", coins);
			scanf("%d", &bet);
			if (bet == -1 || (bet <= coins && bet >= 1))
				break;
		}
		if (bet == -1)
			break;


		hand = createHand(deck);
		for (int i = 0; i < 5; i++) {
			deck = deck->next;
		}

		bubbleSort(hand);
		printf("%s's hand: \n", name);
		printAscii(hand);
		hand = newCards(&deck, hand);
		bubbleSort(hand);

		//card* test = make();

		//check will be a number dependant on what the user holds, and from there we can deal with the bets
		int check = checker(hand);
		if (check == 1) {
			printf("%s's hand: (Royal Flush)\n", name);
			printAscii(hand);
			coins += bet * 250;
			printf("You WON %d coins and now have %d coins\n", bet * 250, coins);
			user.FHWins++;
			wins++;
		}
		else if (check == 2) {
			printf("%s's hand: (Straight Flush)\n", name);
			printAscii(hand);
			coins += bet * 100;
			printf("You WON %d coins and now have %d coins\n", bet * 100, coins);
			user.SFWins++;
			wins++;
		}
		else if (check == 3) {
			printf("%s's hand: (Four of a Kind)\n", name);
			printAscii(hand);
			coins += bet * 50;
			printf("You WON %d coins and now have %d coins\n", bet * 50, coins);
			user.FoaKWins++;
			wins++;
		}
		else if (check == 4){
			printf("%s's hand: (Full House)\n", name);
			printAscii(hand);
			coins += bet * 25;
			printf("You WON %d coins and now have %d coins\n", bet * 25, coins);
			user.FHWins++;
			wins++;
		}
		else if (check == 5){
			printf("%s's hand: (Flush)\n", name);
			printAscii(hand);
			coins += bet * 10;
			printf("You WON %d coins and now have %d coins\n", bet * 10, coins);
			user.FlushWins++;
			wins++;
		}
		else if (check == 6){
			printf("%s's hand: (Straight)\n", name);
			printAscii(hand);
			coins += bet * 5;
			printf("You WON %d coins and now have %d coins\n", bet * 5, coins);
			user.StrtWins++;
			wins++;
		}
		else if (check == 7){
			printf("%s's hand: (Three of a Kind)\n", name);
			printAscii(hand);
			coins += bet * 4;
			printf("You WON %d coins and now have %d coins\n", bet * 4, coins);
			user.ToaKWins++;
			wins++;
		}
		else if (check == 8){
			printf("%s's hand: (Two Pair)\n", name);
			printAscii(hand);
			coins += bet * 2;
			printf("You WON %d coins and now have %d coins\n", bet * 2, coins);
			user.TPWins++;
			wins++;
		}
		else {
			printf("%s's hand: \n", name);
			printAscii(hand);
			coins = coins - bet;
			printf("You LOST %d coins and now have %d coins\n", bet, coins);
			user.loses++;
		}
		if (coins == 0) {
			printf("\nGame over! You lost all your coins\n");
			break;
		}
	
		//If the cards left in the deck < 20, the a new deck is made and shuffled.
		if (countCards(deck) < 20) {
			deck = createDeck();
			shuffle(deck);
			for (int i = 0; i < 10000; i++) {
				shuffle(deck);
			}
		}
		char cont;
		printf("Would you like to continue? (Q or q to quit, any other key to continue): ");
		scanf(" %c", &cont);
		if (cont == 'Q' || cont == 'q')
			break;
		system("cls"); // Clear the screen 
	}
	//Will print out the information of the user after they quit or lost into a txt file.
	FILE* outp;
	outp = fopen("TwoPairData.txt", "a");
	fprintf(outp, "Player Name: %s\n", user.name);
	fprintf(outp, "Coins: %d\n", coins);
	fprintf(outp, "Royal Flush Wins: %d\n", user.RFWins);
	fprintf(outp, "Straight Flush Wins: %d\n", user.SFWins);
	fprintf(outp, "Four of a Kind Wins: %d\n", user.FoaKWins);
	fprintf(outp, "Full House Wins: %d\n", user.FHWins);
	fprintf(outp, "Flush Wins: %d\n", user.FlushWins);
	fprintf(outp, "Straight Wins: %d\n", user.StrtWins);
	fprintf(outp, "Three of a Kind Wins: %d\n", user.ToaKWins);
	fprintf(outp, "Two Pair Wins: %d\n", user.TPWins);
	fprintf(outp, "Total Wins: %d\n", wins);
	fprintf(outp, "Loses: %d\n\n", user.loses);
	fclose(outp);
	printf("Goodbye, Thank you for playing!");
	free(hand);
	free(deck);
	return 0;
}