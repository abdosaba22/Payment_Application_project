#include "app.h"

void appStart(void) {
	ST_cardData_t* cardData = (ST_cardData_t*)malloc(sizeof(ST_cardData_t));
	ST_terminalData_t* termData = (ST_terminalData_t*)malloc(sizeof(ST_terminalData_t));
	ST_transaction_t* trans = (ST_transaction_t*)malloc(sizeof(ST_transaction_t));
	// into Card
	while (1) {
		if (getCardHolderName(cardData) == CARD_OK) break;
		else printf("Wrong name\n");
	}
	while (1) {
		if (getCardExpiryDate(cardData) == CARD_OK) break;
		else printf("WRONG EXP DATE\n");
	}
	while (1) {
		if (getCardPAN(cardData) == CARD_OK) break;
		else printf("WRONG PAN\n");
	}
	// into terminal
	while (1) {
		if (getTransactionDate(termData) == TERMINAL_OK) break;
		else printf("Wrong Date\n");
	}
	if (isCardExpired(cardData, termData) == EXPIRED_CARD) { printf("Declined Card Expired\n"); return; } // end
	while (1) {
		if (getTransactionAmount(termData) == TERMINAL_OK) break;
		else printf("INVALID_AMOUNT\n");
	}
	setMaxAmount(termData, MaxAmount); //5000.0 MAX
	if (isBelowMaxAmount(termData) == EXCEED_MAX_AMOUNT) { printf("EXCEED_MAX_AMOUNT\n"); return;} // end
	if (isValidCardPAN(cardData) == INVALID_CARD) { printf("INVALID_CARD PAN\n"); return; } //optional 
	// save card & terminal into struct transaction 
	strcpy_s(trans->cardHolderData.cardHolderName,25, cardData->cardHolderName); 
	strcpy_s(trans->cardHolderData.cardExpirationDate,6, cardData->cardExpirationDate);
	strcpy_s(trans->cardHolderData.primaryAccountNumber,20, cardData->primaryAccountNumber);
	strcpy_s(trans->terminalData.transactionDate,11,termData->transactionDate);
	trans->terminalData.maxTransAmount = termData->maxTransAmount;
	trans->terminalData.transAmount = termData->transAmount;
	
	// into Server	
	saveTransaction(trans);
}