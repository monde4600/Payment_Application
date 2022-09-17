#include <stdio.h>
#include "../stdTypes.h"
#include "../Terminal/Terminal.h"
#include "../Card/Card.h"
#include "../Server/Server.h"

 static ST_accountsDB_t ST_AccountsDB[255] = {
{ 100000,RUNNING,"1234567890123456789"},
{ 250000.15,RUNNING,"678132873214669726"},
{ 100001568.5,RUNNING,"9873213456789931352" },
{ 2000,BLOCKED,"9873134566546666"},
{ 50000,RUNNING,"878973243546265799"},
{ 20,RUNNING,"0548479434679879998"},
{ 0.25,RUNNING,"9834135459514756657"},
{ 2454.5,BLOCKED,"8798781354333464446"},
{ 10,BLOCKED,"9879434357812367543"},
{ 20.5,RUNNING,"97877822546684846"}
};


static ST_transaction TransDataDB[255];
static strCompare CompareStr(char* str1, char* str2)
{
	strCompare state = notmatched;
	uint8_t Itrator = 0;
	do 
	{
		if(str1[Itrator] != str2[Itrator])	break;
		if (!str1[Itrator]) state = matched;
	}while(str2[Itrator++]);
		return state; 
}

EN_transStat_t recieveTransactionData(ST_transaction* transData)
{
	EN_transStat_t ErrorState = INTERNAL_SERVER_ERROR;
	transData->transState = INTERNAL_SERVER_ERROR;
	ST_accountsDB_t* RefranceACC;
	if (isValidAccount(&transData->cardHolderData, &RefranceACC) == ok)
		if (isBlockedAccount(RefranceACC) == ok)
		{
			if (isAmountAvailable(&transData->terminalData, RefranceACC) == ok)
			{
				RefranceACC->balance -= transData->terminalData.transAmount;
				transData->transState = APPROVED;
				if (saveTransaction(transData) == ok)
					if (getTransaction(transData->transactionSequenceNumber, transData) == oK)
					{
						ErrorState = APPROVED;
						printf("balance=%f,,PA=%s", RefranceACC->balance, RefranceACC->primaryAccountNumber);
						printf("\n%s\n", transData->cardHolderData.cardHolderName);
					}
					else	ErrorState = INTERNAL_SERVER_ERROR;
				else ErrorState = INTERNAL_SERVER_ERROR;
			}
			else	ErrorState = DECLINED_INSUFFECIENT_FUND;
		}
		else	ErrorState = DECLINED_STOLEN_CARD;
	else	ErrorState= FRAUD_CARD;
	transData->transState = ErrorState;
	return ErrorState;
}


EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence)
{
	EN_serverError_t ErrorState = ACCOUNT_NOT_FOUND;
	uint8_t i = 0;
	while(i < 255 && ST_AccountsDB[i].primaryAccountNumber[0]
		&& ErrorState != APPROVED)
	{
		if (CompareStr(cardData->primaryAccountNumber, ST_AccountsDB[i].primaryAccountNumber))
		{
			ErrorState = APPROVED;
			*accountRefrence = &ST_AccountsDB[i];
		}
		i++;
	}
	return ErrorState;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t ErrorState = BLOCKED_ACCOUNT;
	if (RUNNING == accountRefrence->state)	ErrorState = ok;
	return ErrorState;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData,ST_accountsDB_t * accountRefrence)
{
	EN_serverError_t ErrorState = LOW_BALANCE;
	if (termData->transAmount <= accountRefrence->balance)	ErrorState = ok;
	return ErrorState;
}

EN_serverError_t saveTransaction(ST_transaction* transData)
{
	EN_serverError_t ErrorState = SAVING_FAILED;
	transData->transactionSequenceNumber=0;
	//the best case best case scenario that the server data is not full to the max (index 254)
	if (!TransDataDB[254].transactionSequenceNumber)
	{
		while (TransDataDB[transData->transactionSequenceNumber].transactionSequenceNumber
			&& transData->transactionSequenceNumber++ < 255);
		if (transData->transactionSequenceNumber < 255)
		{
			transData->transactionSequenceNumber++;
			TransDataDB[transData->transactionSequenceNumber - 1] = *transData;
			ErrorState = oK;
		}
	}
	//else then we should find the oldest saved transaction and stored this rans data in it
	else
	{
		uint8_t LowestIND = 0;
		for (uint8_t i = 1; i < 255; i++)
		{
			if (TransDataDB[LowestIND].transactionSequenceNumber >
				TransDataDB[i].transactionSequenceNumber)
				LowestIND = i;
		}
		// now we should know the biggest trans sequace and add +1
		uint8_t BiggestIND = 0;
		for (uint8_t i = 1; i < 255; i++)
		{
			if (TransDataDB[BiggestIND].transactionSequenceNumber <
				TransDataDB[i].transactionSequenceNumber)
				BiggestIND = i;
		}
		transData->transactionSequenceNumber = TransDataDB[BiggestIND].transactionSequenceNumber + 1;
		TransDataDB[LowestIND] = *transData;
		ErrorState = oK;

	}
	//printf("%s\n", TransDataDB[0].cardHolderData.primaryAccountNumber);
	return ErrorState;
}

void TestCaseForFullTransDB(void)
{
	for (uint8_t i = 0; i < 200; i++)
		TransDataDB[i].transactionSequenceNumber = i + 1;
}
void PrintAllTransDB(void)
{
	for (uint8_t i = 0; i < 255; i++)
		printf("indx=%d,,seq=%d\n", i, TransDataDB[i].transactionSequenceNumber);
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData)
{
	EN_serverError_t ErrorState = TRANSACTION_NOT_FOUND;
	uint8_t i = 0;
	while (i < 255 && ErrorState)
	{
		if (transactionSequenceNumber == TransDataDB[i].transactionSequenceNumber)
		{	
			*transData = TransDataDB[i];
			ErrorState = oK;
		}
		i++;
	}
	
	return ErrorState;
}