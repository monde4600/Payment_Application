#include <stdio.h>
#include <time.h>
#include "../stdTypes.h"
#include "../Card/Card.h"
#include "../Terminal/Terminal.h"
#include "../Server/Server.h"
#include "Application.h"


void appStart(void)
{
	
	printf("\n///////////////payment APP////////////////////////\n");
	ST_transaction APP_TransData;
	while (getCardHolderName(&APP_TransData.cardHolderData) == WRONG_NAME)
		printf("the card holder must be must be bettween 20 and 24 charachter and mustn't content any numbers\n");

	while (getCardExpiryDate(&APP_TransData.cardHolderData) == WRONG_DATE)
		printf("wrong date !!");

	while (getCardPAN(&APP_TransData.cardHolderData) == WRONG_PAN)
		printf("wrong PAN must be between 16 and 19 digits !!\n");

	/*printf("card name : %s\n", APP_TransData.cardHolderData.cardHolderName);
	printf("card PA numper : %s\n", APP_TransData.cardHolderData.primaryAccountNumber);
	printf("card EX date : %s\n",APP_TransData.cardHolderData.cardExpirationDate);*/

	getTransactionDate(&APP_TransData.terminalData);
	printf("date : %s\n", APP_TransData.terminalData.transactionDate);
	while (setMaxAmount(&APP_TransData.terminalData) == INVALID_MAX_AMOUNT)
		printf("max Amount must be more than zero\n");
	if (isCardExpired(&APP_TransData.cardHolderData, &APP_TransData.terminalData) == oK)
	{
		while (getTransactionAmount(&APP_TransData.terminalData) == INVALID_AMOUNT)
			printf("Transaction amount must be more than zero\n");
		if (isBelowMaxAmount(&APP_TransData.terminalData) == oK)
			switch (recieveTransactionData(&APP_TransData))
			{
			case APPROVED:
				printf("sucessful transaction .\ntransaction numper is: %d\n",
					APP_TransData.transactionSequenceNumber);
				break;
			case FRAUD_CARD:
				printf("\nfake card.\n");
				break;
			case DECLINED_INSUFFECIENT_FUND:
				printf("\namount not avelable\n");
				break;
			case DECLINED_STOLEN_CARD:
				printf("\nstolen card\n");
				break;
			case INTERNAL_SERVER_ERROR:
				printf("\nserver error\n");
				break;
			default:
				break;
			}
		else
			printf("\nDeclined amount exceeding limit .\n");
	}
	else
		printf("\nExpired Card.\n");
	
}