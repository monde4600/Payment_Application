#include <stdio.h>
#include "../stdTypes.h"
#include "Card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t Error_state = WRONG_NAME;
	uint8_t TEMP_CardName[25];
	printf("Enter the cardholder's name : ");
	do 
	{
		fgets(TEMP_CardName, 25, stdin);
	} while ('\n'==TEMP_CardName[0]);
	/*uint8_t* p;
	p = strchr(TEMP_CardName, '\n');
	if (p != NULL)
		*p = '\0';*/
	uint8_t CorrectCharactersCounter = 0;
	while (TEMP_CardName[CorrectCharactersCounter] && CorrectCharactersCounter<25)
	{
		if (TEMP_CardName[CorrectCharactersCounter] >= '0' &&
			TEMP_CardName[CorrectCharactersCounter] <= '9')
			return Error_state;


		CorrectCharactersCounter++;
	}
	if (CorrectCharactersCounter >= 20 && CorrectCharactersCounter <= 24)
	{
		do
		{
			cardData->cardHolderName[CorrectCharactersCounter] = TEMP_CardName[CorrectCharactersCounter];
		} while (CorrectCharactersCounter--);
		Error_state = OK;
	}
	return Error_state;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	EN_cardError_t Error_state = WRONG_EXP_DATE;
	uint8_t TEMP_CardDate[6];
	printf("Enter the  the card expiry date (MM/YY) EX: \"05/25\" ");
	do
	{
		fgets(TEMP_CardDate, 6, stdin);
	} while ('\n' == TEMP_CardDate[0]);
	if (((TEMP_CardDate[1] - '0') + 10 * (TEMP_CardDate[0] - '0')) <= 12 &&
		TEMP_CardDate[2] == '/' && !TEMP_CardDate[5])
	{
		uint8_t Itrator = 0;
		for (; Itrator < 6; Itrator++)
		{
			cardData->cardExpirationDate[Itrator] = TEMP_CardDate[Itrator];
		}
		Error_state = OK;
	}
	return Error_state;
}
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	EN_cardError_t Error_state = WRONG_PAN;
	uint8_t TEMP_CardPAN[20];
	printf("Enter the card PAN : ");
	do
	{
		fgets(TEMP_CardPAN, 20, stdin);
	}while ('\n' == TEMP_CardPAN[0]);
	uint8_t CorrectCharactersCounter = 0;
	while (TEMP_CardPAN[CorrectCharactersCounter] && CorrectCharactersCounter <20)
	{
		if (TEMP_CardPAN[CorrectCharactersCounter] < '0' || TEMP_CardPAN[CorrectCharactersCounter]>'9')
			return Error_state;
		CorrectCharactersCounter++;
	}
	if (CorrectCharactersCounter >= 16 && CorrectCharactersCounter <= 19)
	{
		do
		{
			cardData->primaryAccountNumber[CorrectCharactersCounter] = TEMP_CardPAN[CorrectCharactersCounter];
		} while (CorrectCharactersCounter--);
		Error_state= OK;
	}
	return Error_state;
}