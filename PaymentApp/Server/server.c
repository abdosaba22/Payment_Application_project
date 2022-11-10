#include "server.h"                             //   all valid Card PAN 
ST_accountsDB_t accountsDB[255] = { {2000.0,RUNNING,"5470768394003117"},{1000.0 ,RUNNING,"4749236977248283"},
	{2000.0 ,BLOCKED,"0209369223825389"},{500.0 , RUNNING,"0568029498492878"},
	{4000.0 ,BLOCKED,"8830524396392340"},{9000.0, RUNNING,"2841561999519606"},
	{10000.0,BLOCKED,"1555047311192924"},{2500.0, RUNNING,"693798783598042181"} };
ST_transaction_t transactionsDB[255] = {0};
uint8_t sequenceNumb=0;  
ST_accountsDB_t * accountReff;
EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	if (isValidAccount(&transData->cardHolderData, accountReff) == ACCOUNT_NOT_FOUND) return FRAUD_CARD;
	if (isBlockedAccount(accountReff) == BLOCKED_ACCOUNT) return DECLINED_STOLEN_CARD;
	if (isAmountAvailable(&transData->terminalData,accountReff) == LOW_BALANCE ) return DECLINED_INSUFFECIENT_FUND;
	//if (saveTransaction(transData) == SAVING_FAILED) return INTERNAL_SERVER_ERROR; // note or ask how to save the return of this func 
	if (sequenceNumb>255) return INTERNAL_SERVER_ERROR; // instead of using saveTransactionData cause in app i will call receiveT then call save
	accountReff->balance -= transData->terminalData.transAmount; // update new balance after transaction 
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	uint8_t i;
	uint8_t str[20];
	strcpy_s(str,17,cardData->primaryAccountNumber);
	for (i = 0; i < 8; i++) {
		if (strcmp(str,accountsDB[i].primaryAccountNumber) == 0) {
			accountReff = &accountsDB[i];
			return SERVER_OK;
		}
	}
	accountReff = NULL; // not found ref --> NULL 
	return ACCOUNT_NOT_FOUND;;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	
	if (accountReff->state == BLOCKED ) return BLOCKED_ACCOUNT;
	//printf("not blocked");
	return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	
	if (termData->transAmount > accountReff->balance) return LOW_BALANCE;
	return SERVER_OK;
}
//Assuming that the connection between the terminal and server is always connected, then it will return SERVER_OK.
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	// DB 255 from 0 to 254 
	if (sequenceNumb > 254) return SAVING_FAILED;
	//EN_transState_t storevalue= recieveTransactionData(transData) ;
	//strcpy(transactionsDB[sequenceNumb].transState, storevalue);
	transactionsDB[sequenceNumb].transState = recieveTransactionData(transData);
	
	strcpy_s(transactionsDB[sequenceNumb].cardHolderData.cardHolderName,25,transData->cardHolderData.cardHolderName);
	strcpy_s(transactionsDB[sequenceNumb].cardHolderData.cardExpirationDate,6,transData->cardHolderData.cardExpirationDate);
	strcpy_s(transactionsDB[sequenceNumb].cardHolderData.primaryAccountNumber,20,transData->cardHolderData.primaryAccountNumber);
	//transactionsDB[sequenceNumb].cardHolderData = transData->cardHolderData;
	strcpy_s(transactionsDB[sequenceNumb].terminalData.transactionDate,11, transData->terminalData.transactionDate);
	transactionsDB[sequenceNumb].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
	transactionsDB[sequenceNumb].terminalData.transAmount = transData->terminalData.transAmount;
	
	transactionsDB[sequenceNumb].transactionSequenceNumber = sequenceNumb;
	
	sequenceNumb++;
	listSavedTransactions();
	return SERVER_OK;


}
void listSavedTransactions(void) {
	uint8_t i = 0;
	uint8_t* trnstate[] = {"APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR" };
	while (i < sequenceNumb) {
		printf("############################\n");
		printf("Transaction Sequence Number: %d \n",i);
		printf("Transaction Date: ");
		puts(transactionsDB[i].terminalData.transactionDate);
		printf("Transaction Amount: %f \n", transactionsDB[i].terminalData.transAmount);
		printf("Transaction State: ");
		puts(trnstate[transactionsDB[i].transState]); // return number get its value from arra transtate
		printf("Terminal Max Amount: %f\n", transactionsDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name: ");
		puts(transactionsDB[i].cardHolderData.cardHolderName);
		printf("PAN: ");
		puts(transactionsDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: ");
		puts(transactionsDB[i].cardHolderData.cardExpirationDate);
		printf("############################\n");
		i++;
	}
}
