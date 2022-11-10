#include "card.h"

uint8_t isnamealphabetic(uint8_t* str) {
	
	for (uint8_t i = 0; str[i]; i++) {
		// 0x20 --> space 
		if (str[i] == 0x20) continue;
		//Not alphabetic return 0 
		else if (isalpha(str[i])==0) return 0; // stop loop 
	}
	return 1;
}

uint8_t isrightformat(uint8_t* str)
{
	// "num + num '/' num + num"
	if (!isalnum(str[0]) || !isalnum(str[1]) || !isalnum(str[3]) || !isalnum(str[4]) || !(str[2] == '/') )
		return 0;

	return 1;
}

uint8_t isPANalphanumeric(uint8_t* str) {

	for (uint8_t i = 0; str[i]; i++) {
		//Not alphanum return 0 
		if (isalnum(str[i]) == 0) return 0; // stop loop 
	}
	return 1;
}

/************************************************************************
* Function Name: getCardHolderName
* Purpose      : get card holder's name & check the entered wrong or good 
* Parameters   : ST_cardData_t* pointer :struct typeddef in card.h
* Return value : EN_cardError_t :enum typeddef in card.h
*************************************************************************/

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	uint8_t* str_name = (uint8_t*)malloc(30 * sizeof(uint8_t));
	printf("Enter Card Holder's Name: ");
	gets(str_name);
	if (str_name == NULL) {free(str_name); return WRONG_NAME; } // no name entered
	else if ( (strlen(str_name) > MAX_NAME_SIZE) || (strlen(str_name) < MIN_NAME_SIZE) ){free(str_name); return WRONG_NAME; } // error length
	else if (!isnamealphabetic(str_name)) {free(str_name); return WRONG_NAME;} // error not alphabetic entered
	// else good save name in our struct then free our dynamic array
	strcpy_s(cardData->cardHolderName,25, str_name);
	free(str_name);
	return CARD_OK;
}

/************************************************************************
* Function Name: getCardExpiryDate
* Purpose      : get card Expiry date & check the entered wrong or good
* Parameters   : ST_cardData_t* pointer:struct typeddef in card.h
* Return value : EN_cardError_t:enum typeddef in card.h
*************************************************************************/

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	uint8_t* str_Expiration = (uint8_t*)malloc(8 * sizeof(uint8_t));
	printf("Enter Card Expiry Date in format -->'MM / YY': ");
	gets(str_Expiration);
	if (str_Expiration == NULL){free(str_Expiration); return WRONG_EXP_DATE;} // no entered
	else if (strlen(str_Expiration) != EXP_DATE_SIZE){free(str_Expiration); return WRONG_EXP_DATE;}// error length
	else if (!isrightformat(str_Expiration)){free(str_Expiration); return WRONG_EXP_DATE;} // error not wright format entered
	// else good save expirydate in our struct then free our dynamic array
	strcpy_s(cardData->cardExpirationDate,6, str_Expiration);
	free(str_Expiration);
	return CARD_OK;
}

/************************************************************************
* Function Name: getCardPAN
* Purpose      : get card primary Account Number & check the entered wrong or good
* Parameters   : ST_cardData_t* pointer :struct typeddef in card.h
* Return value : EN_cardError_t:  enum typedef in card.h
*************************************************************************/
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	uint8_t* str_CardPAN = (uint8_t*)malloc(22 * sizeof(uint8_t));
	printf("Enter Card primary Account Number: ");
	gets(str_CardPAN);
	if (str_CardPAN == NULL) { free(str_CardPAN); return WRONG_PAN; } // no name entered
	else if ((strlen(str_CardPAN) > MAX_PAN_SIZE) || (strlen(str_CardPAN) < MIN_PAN_SIZE) ) { free(str_CardPAN); return WRONG_PAN; }// error length
	else if (!isPANalphanumeric(str_CardPAN)) { free(str_CardPAN); return WRONG_PAN; } // error not alphabetic entered
	// else good save CARD PAN in our struct then free our dynamic array
	strcpy_s(cardData->primaryAccountNumber,20, str_CardPAN);
	free(str_CardPAN);
	return CARD_OK;
}