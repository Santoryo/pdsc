#include "account.h"
#include "account.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "filehandler.h"
#include "constants.h"
#include "logger.h"
#include "utils.h"

void displayBorder()
{
    printf(BOLDCYAN "+-----+-----------------+----------------------+---------------------------+-------------+-----------------+--------------+----------------+\n" RESET);
}

// Get the last ID from the database
int getLastId() {
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int lastId = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        char* id = strtok(line, ";");
        lastId = atoi(id);
    }

    fclose(file);
    if (line)
        free(line);

    return lastId;
}

void createAccount(Account account) {
    FILE *file = fopen(FILENAME, "a");

    int id = 0;

    if(checkIfFileExists(FILENAME)) {
        id = getLastId();
    }

    account.id = id + 1;


    fprintf(file, "%d;%s;%s;%s;%s;%d;%d;%d\n", 
    account.id, account.firstName, account.lastName, account.address, 
    account.pesel, account.currentBalance, account.loanBalance, account.loanInterest);

    info("Account created successfully\n");

    fclose(file);
}

void displayAccount(Account account) {

    printf(CYAN "| %-3d | %-15s | %-20s | %-25s | %-11s | %-15.2f | %-12.2f | %-12.2f %-1s |\n" RESET, 
    account.id, account.firstName, account.lastName, account.address, 
    account.pesel, (float)account.currentBalance / 100.0f, (float)account.loanBalance / 100.0f, (float)account.loanInterest / 100.0f, "%");
    
}

void displayAccountWrapper(Account account) {
    displayAccount(account);
}

void displayHeaader() {
    displayBorder();
    printf(BOLDCYAN "| %-3s | %-15s | %-20s | %-25s | %-11s | %-15s | %-12s | %-14s |\n" RESET, 
    "ID", "First Name", "Last Name", "Address", "PESEL", "Current Balance", "Loan Balance", "Loan Interest");
    displayBorder();
}

void getAllAccounts()
{
    displayHeaader();

    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        error("File does not exist");
        return;
    }

    while(1) {
        Account account;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        read = getline(&line, &len, file);
        if(read == -1) {
            break;
        }

        char* id = strtok(line, ";");
        char* firstName = strtok(NULL, ";");
        char* lastName = strtok(NULL, ";");
        char* address = strtok(NULL, ";");
        char* pesel = strtok(NULL, ";");
        char* currentBalance = strtok(NULL, ";");
        char* loanBalance = strtok(NULL, ";");
        char* loanInterest = strtok(NULL, ";");

        account.id = atoi(id);
        account.firstName = strcpy(malloc(strlen(firstName) + 1), firstName);
        account.lastName = strcpy(malloc(strlen(lastName) + 1), lastName);
        account.address = strcpy(malloc(strlen(address) + 1), address);
        account.pesel = strcpy(malloc(strlen(pesel) + 1), pesel);
        account.currentBalance = atoi(currentBalance);
        account.loanBalance = atoi(loanBalance);
        account.loanInterest = atoi(loanInterest);

        displayAccount(account);

        free(account.firstName);
        free(account.lastName);
        free(account.address);
        free(account.pesel);
    }

    displayBorder();
}

void makeDeposit(int id, int amount)
{
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        error("File does not exist");
        return;
    }

    FILE *tempFile = fopen("temp.csv", "w");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    float newBalance = 0;

    bool found = false;

    while ((read = getline(&line, &len, file)) != -1) {
        char* idStr = strtok(line, ";");
        char* firstName = strtok(NULL, ";");
        char* lastName = strtok(NULL, ";");
        char* address = strtok(NULL, ";");
        char* pesel = strtok(NULL, ";");
        char* currentBalanceStr = strtok(NULL, ";");
        char* loanBalanceStr = strtok(NULL, ";");
        char* loanInterestStr = strtok(NULL, ";");

        int currentBalance = atoi(currentBalanceStr);
        int loanBalance = atoi(loanBalanceStr);
        int loanInterest = atoi(loanInterestStr);

        if(atoi(idStr) == id)
        {
            found = true;
            currentBalance += amount;
            newBalance = (float)currentBalance / 100.0f;
        }

        fprintf(tempFile, "%s;%s;%s;%s;%s;%d;%d;%d\n", 
        idStr, firstName, lastName, address, pesel, currentBalance, loanBalance, loanInterest);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if(found)
    {
        char message[200];
        sprintf(message,"The transaction of %.2f has been succesfully completed. Account ID #%i has now %.2f balance", (float)amount / 100.0f, id, (float)newBalance);
        info(message);
    }
    else
    {
        error("Account not found");
    }

    if (line)
        free(line);
}


Account filterAccounts(char* column, char* value, bool isReturnNeeded)
{
    FILE *file = fopen(FILENAME, "r");
    Account accountToReturn = {0, "", "", "", "", 0, 0, 0};

    if(file == NULL) {
        error("File does not exist");
        return accountToReturn;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if(!isReturnNeeded) displayHeaader();


    while ((read = getline(&line, &len, file)) != -1) {
        Account account = {0, "", "", "", "", 0, 0, 0};
        char* id = strtok(line, ";");
        char* firstName = strtok(NULL, ";");
        char* lastName = strtok(NULL, ";");
        char* address = strtok(NULL, ";");
        char* pesel = strtok(NULL, ";");
        char* currentBalance = strtok(NULL, ";");
        char* loanBalance = strtok(NULL, ";");
        char* loanInterest = strtok(NULL, ";");

        account.id = atoi(id);
        account.firstName = strcpy(malloc(strlen(firstName) + 1), firstName);
        account.lastName = strcpy(malloc(strlen(lastName) + 1), lastName);
        account.address = strcpy(malloc(strlen(address) + 1), address);
        account.pesel = strcpy(malloc(strlen(pesel) + 1), pesel);
        account.currentBalance = atoi(currentBalance);
        account.loanBalance = atoi(loanBalance);
        account.loanInterest = atoi(loanInterest);

        if(strcasecmp(column, "id") == 0 && account.id == atoi(value))
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "name") == 0 && strcasecmp(account.firstName, value) == 0)
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "surname") == 0 && strcasecmp(account.lastName, value) == 0)
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "address") == 0 && strcasecmp(account.address, value) == 0)
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "pesel") == 0 && strcasecmp(account.pesel, value) == 0)
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "currentBalance") == 0 && account.currentBalance == atoi(value))
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "loanBalance") == 0 && account.loanBalance == atoi(value))
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
        else if(strcasecmp(column, "loanInterest") == 0 && account.loanInterest == atoi(value))
        {
            if(!isReturnNeeded) displayAccount(account);
            accountToReturn = account;
        }
    }

    if(!isReturnNeeded) displayBorder();

    fclose(file);
    if (line)
        free(line);


    if(!isReturnNeeded && accountToReturn.id != 0)
    {
        free(accountToReturn.firstName);
        free(accountToReturn.lastName);
        free(accountToReturn.address);
        free(accountToReturn.pesel);
    }

    if(accountToReturn.id == 0)
    {
        error("Account not found");
    }

    return accountToReturn;

}


void payLoan(int id, int amount) {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.csv", "w");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    bool found = false;

    while ((read = getline(&line, &len, file)) != -1) {
        char *idStr = strtok(line, ";");
        char *firstName = strtok(NULL, ";");
        char *lastName = strtok(NULL, ";");
        char *address = strtok(NULL, ";");
        char *pesel = strtok(NULL, ";");
        int currentBalance = atoi(strtok(NULL, ";"));
        int loanBalance = atoi(strtok(NULL, ";"));
        int loanInterest = atoi(strtok(NULL, ";"));

        if(atoi(idStr) == id) {
            found = true;
            float totalPayment = amount + (amount * ((float)loanInterest / 10000.0f));
            loanBalance = loanBalance - amount;
            currentBalance = (int)((float)currentBalance - totalPayment);

            if(currentBalance < 0) {
                error("Insufficient funds");
                fclose(file);
                fclose(tempFile);
                remove("temp.csv");
                return;
            }
        }

        fprintf(tempFile, "%s;%s;%s;%s;%s;%d;%d;%d\n", 
        idStr, firstName, lastName, address, pesel, currentBalance, loanBalance, loanInterest);
    }

    char message[100];
    sprintf(message, "Paid %.2f from Account #%d", (float)amount / 100, id);
    info(message);
    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if(!found) {
        error("Account not found");
    }
}

void transferMoney(char* senderId, char* receiptientId, char* amount)
{

    if (!validateInt(amount, "Amount has to be a positive number"))
        return;

    Account sender = filterAccounts("id", senderId, true);
    Account receiptient = filterAccounts("id", receiptientId, true);

    if (sender.id == 0 || receiptient.id == 0)
    {
        char message[100];
        sprintf(message, "Account with id %s does not exist", sender.id == 0 ? senderId : receiptientId);
        error(message);
        return;
    }

    if (sender.currentBalance < atoi(amount))
    {
        error("Insufficient funds");
        return;
    }

    makeDeposit(atoi(senderId), atoi(amount) * -1);
    makeDeposit(atoi(receiptientId), atoi(amount));

    char message[100];
    sprintf(message, "Transfered %.2f from Account #%s to Account #%s", (float)(atof(amount) / 100), senderId, receiptientId);
    info(message);

    free(sender.firstName);
    free(sender.lastName);
    free(sender.address);
    free(sender.pesel);

    free(receiptient.firstName);
    free(receiptient.lastName);
    free(receiptient.address);
    free(receiptient.pesel);
}

void takeLoan(int id, int amount)
{
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.csv", "w");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    bool found = false;

    while ((read = getline(&line, &len, file)) != -1) {
        char *idStr = strtok(line, ";");
        char *firstName = strtok(NULL, ";");
        char *lastName = strtok(NULL, ";");
        char *address = strtok(NULL, ";");
        char *pesel = strtok(NULL, ";");
        int currentBalance = atoi(strtok(NULL, ";"));
        int loanBalance = atoi(strtok(NULL, ";"));
        int loanInterest = atoi(strtok(NULL, ";"));

        if(atoi(idStr) == id) {
            found = true;
            loanBalance = loanBalance + amount;
            currentBalance = currentBalance + amount;
        }

        fprintf(tempFile, "%s;%s;%s;%s;%s;%d;%d;%d\n", 
        idStr, firstName, lastName, address, pesel, currentBalance, loanBalance, loanInterest);
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.csv", FILENAME);
    
    char message[100];
    if(found)
    {
        sprintf(message, "Took a loan of %.2f from Account #%d", (float)amount / 100, id);
        info(message);
    }
    else
    {
        info("Account not found");
    }
    
}