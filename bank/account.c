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

    printf(CYAN "| %-3d | %-15s | %-20s | %-25s | %-11s | %-15.2f | %-12.2f | %-14.2f |\n" RESET, 
    account.id, account.firstName, account.lastName, account.address, 
    account.pesel, (float)account.currentBalance / 100, (float)account.loanBalance / 100, (float)account.loanInterest / 100);
    
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
    parseFile(displayAccountWrapper);
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
        info("Transaction made successfully");
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

void payLoan(int id, int amount)
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
            loanBalance -= amount * (1 + loanInterest / 100);
            currentBalance -= amount * (1 + loanInterest / 100);
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
        info("Loan paid successfully");
    }
    else
    {
        error("Account not found");
    }

    if (line)
        free(line);
}