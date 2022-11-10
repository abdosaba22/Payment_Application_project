#ifndef CARD_H_

#define CARD_H_

#define MAX_NAME_SIZE 24
#define MIN_NAME_SIZE 20
#define EXP_DATE_SIZE 5 //  MM/YY 5 char 
#define MAX_PAN_SIZE 19 //primary Account Number 
#define MIN_PAN_SIZE 16

#include "../header.h"


typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
    // 0        1           2               3
    CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

//prototypes funcs
EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

// helping func
uint8_t isnamealphabetic(uint8_t* str);
uint8_t isrightformat(uint8_t* str);
uint8_t isPANalphanumeric(uint8_t* str);


#endif