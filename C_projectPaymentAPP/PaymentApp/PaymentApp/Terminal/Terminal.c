
#include <stdio.h>
#include <time.h>
#include "../stdTypes.h"
#include "../Terminal/Terminal.h"
#include "../Card/Card.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = WRONG_DATE;
	
	time_t currentTime = time(NULL);
	struct tm tm = *localtime(&currentTime);
	uint8_t i = 0;
	termData->transactionDate[i++] = (tm.tm_mday / 10) % 10 + '0';  //i=0
	termData->transactionDate[i++] = tm.tm_mday  % 10 + '0';		//i=1
	termData->transactionDate[i++] = '/';							//i=2
	termData->transactionDate[i++] = ((++tm.tm_mon) / 10) % 10 + '0';//i=3
	termData->transactionDate[i++] = tm.tm_mon % 10 + '0';			//i=4
	termData->transactionDate[i++] = '/';							//i=5
	tm.tm_year += 1900;
	for(i=10; i > 5; i--)
	{
		if (i != 10)
		{
			termData->transactionDate[i] = tm.tm_year%10 +'0';
			tm.tm_year /=10;
		}
		else
		{
			termData->transactionDate[i] = '\0';
			ErrorState = oK;
		}
	}
	return ErrorState;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = EXPIRED_CARD;
	
	uint8_t CardYY = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');
	uint8_t TermYY = (termData->transactionDate[8] - '0') * 10 + (termData->transactionDate[9] - '0');
	if (CardYY == TermYY)
	{
		uint8_t CardMM = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
		uint8_t TermMM = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');
		if(CardMM>= TermMM)		ErrorState = oK;
	}
	else if (CardYY > TermYY)	ErrorState = oK;
	return ErrorState;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = INVALID_AMOUNT;
	printf("enter the transaction amount : ");
	float LocalAmount;
	scanf("%f", &LocalAmount);
	if (LocalAmount > 0)
	{
		termData->transAmount = LocalAmount;
		ErrorState = oK;
	}

	return ErrorState;
}

/*EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	EN_terminalError_t ErrorState = WRONG_DATE;
	
	return ErrorState;
}*/

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = EXCEED_MAX_AMOUNT;
	if (termData->transAmount <= termData->maxTransAmount)	ErrorState = oK;
	return ErrorState;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t ErrorState = INVALID_MAX_AMOUNT;
	printf("enter the Maximum transaction amount : ");
	float LocalAmount;
	scanf("%f", &LocalAmount);
	if (LocalAmount > 0)
	{
		termData->maxTransAmount = LocalAmount;
		ErrorState = oK;
	}

	return ErrorState;
}
