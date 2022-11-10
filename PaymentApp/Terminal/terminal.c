#include "terminal.h"

uint8_t isrightformatTD(uint8_t* str)
{
	// 0123456789
	// DD/MM/YYYY
	if (!isalnum(str[0]) || !isalnum(str[1]) || !isalnum(str[3]) || !isalnum(str[4]) || !isalnum(str[6]) || !isalnum(str[7]) || !isalnum(str[8]) || !isalnum(str[9]) || !(str[2] == '/') || !(str[5] == '/'))
		return 0;

	return 1;
}
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	uint8_t* str_transcdate = (uint8_t*)malloc(11,sizeof(uint8_t));//10
	printf("Enter Transaction Date in format --> 'DD/MM/YYYY': ");
	gets(str_transcdate);
	if (str_transcdate == NULL) { free(str_transcdate); return WRONG_DATE; } 
	else if(strlen(str_transcdate)<Trans_Date) { free(str_transcdate); return WRONG_DATE; } 
	else if (!isrightformatTD(str_transcdate)) { free(str_transcdate); return WRONG_DATE; } 
	// else good save TRANSAC DATE in our struct then free our dynamic array
	strcpy_s(termData->transactionDate,11, str_transcdate);
	free(str_transcdate);
	return TERMINAL_OK;
}
	
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	//                    01234
	//expirdate of format MM/YY
	//                    0123456789
	//Transdate of format DD/MM/YYYY
	//first check year YY in expirdate with last YY 89 in TransDate
	
	if (cardData->cardExpirationDate[3] < termData->transactionDate[8]) return EXPIRED_CARD;
	
	else if (cardData->cardExpirationDate[3] == termData->transactionDate[8]) {
		
		if (cardData->cardExpirationDate[4] < termData->transactionDate[9]) return EXPIRED_CARD;
		//equal in YY  then check MM 
		else if (cardData->cardExpirationDate[4] == termData->transactionDate[9]) {
			// M[0] in expirdate to M[3] in Transdate
			if (cardData->cardExpirationDate[0] < termData->transactionDate[3]) return EXPIRED_CARD;
			//M[0] in expirdate equal to M[3] in Transdate then check M[1] to M[4]
			else if (cardData->cardExpirationDate[0] == termData->transactionDate[3]) {

				if (cardData->cardExpirationDate[1] < termData->transactionDate[4]) return EXPIRED_CARD;
				else return TERMINAL_OK;
				
			}
			else return TERMINAL_OK;
		}
		else return TERMINAL_OK;
	}
	else return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	float TransacAmount;
	printf("Enter Transaction Amount:");
	scanf_s("%f", &TransacAmount);
	//If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT
	if(TransacAmount <= 0.0) return INVALID_AMOUNT;
	
	termData->transAmount = TransacAmount;
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) 
		return EXCEED_MAX_AMOUNT;
	else 
		return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	
	//If transaction max amount less than or equal to 0 will return the INVALID_MAX_AMOUNT
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;
 
	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	int i, num1, num2, sum_PAN=0;
	// for even dig
	for (i = 0; i < strlen(cardData->primaryAccountNumber); i += 2) {
		if ((cardData->primaryAccountNumber[i] - '0') * 2 >= 10) {
			num1 = ((cardData->primaryAccountNumber[i] - '0') * 2) / 10;
			num2 = ((cardData->primaryAccountNumber[i] - '0') * 2) % 10;
			sum_PAN += num1 + num2;
		}
		else {
			sum_PAN += (cardData->primaryAccountNumber[i] - '0') * 2;
		}
	}
	//for odd dig 
	for (i = 1; i < strlen(cardData->primaryAccountNumber); i += 2) {
		sum_PAN += cardData->primaryAccountNumber[i] - '0';
	}

	if (sum_PAN % 10 == 0)
		return TERMINAL_OK;
	else 
		return INVALID_CARD;
}