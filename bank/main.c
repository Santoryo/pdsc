#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "account.h"
#include "filehandler.h"
#include "constants.h"
#include "logger.h"


bool validate(char *value, int len, char *message) {
    if(value == NULL || strlen(value) < len) {
        error(message);
        return false;
    }
    return true;
}

bool validatePESEL(char *value) {
    if(strlen(value) != 11) {
        error("PESEL has to be 11 characters long");
        return false;
    }
    return true;
}

bool validateInt(char *value, char *message) {
    if(value == NULL || atoi(value) <= 0) {
        error(message);
        return false;
    }
    return true;
}

bool askForConfirmation()
{
    char answer[5];
    printf(BOLDYELLOW "Are you sure you want to continue? [y/n]: " RESET);
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = 0;

    if(strcmp(answer, "y") == 0 || strcmp(answer, "yes") == 0) return true;
    return false;
    
}


int main(void)
{   
    while(1)
    {
        char command[1024];
        char *token;
        printf(BOLDMAGENTA "[ADMIN] $ " RESET);

        fgets(command, 1024, stdin);
        command[strcspn(command, "\n")] = 0;
        token = strtok(command, " ");

        if(strcmp("create", token) == 0 || strcmp("c", token) == 0)
        {
            Account account;
            char buffer[256];

            printf("Enter first name: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(!validate(buffer, 0, "First name can't be empty")) continue;
            account.firstName = strdup(buffer);

            printf("Enter last name: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(!validate(buffer, 0, "Last name can't be empty")) continue;
            account.lastName = strdup(buffer);

            printf("Enter address: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            account.address = strdup(buffer);
            if (!validate(account.address, 1, "Address can't be empty")) continue;

            printf("Enter PESEL: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(!validatePESEL(buffer)) continue;
            account.pesel = strdup(buffer);

            printf("Enter current balance (optional): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(buffer[0] == '\0') 
            {
                warning("Current balance not provided, setting to 0");
                account.currentBalance = 0;
            }
            else 
            {
                account.currentBalance = atoi(buffer);
            }

            printf("Enter loan balance (optional): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(buffer[0] == '\0') 
            {
                warning("Loan balance not provided, setting to 0");
                account.loanBalance = 0;
            }
            else 
            {
                account.loanBalance = atoi(buffer);
            }

            printf("Enter loan interest (optional): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character
            if(buffer[0] == '\0') 
            {
                warning("Loan interest not provided, setting to default value");
                account.loanInterest = 0;
            }
            else 
            {
                account.loanInterest = atoi(buffer);
            }

            createAccount(account);
        }
        else if(strcmp("list", token) == 0 || strcmp("ls", token) == 0)
        {
            getAllAccounts();
        }
        else if(strcmp("q", token) == 0 || strcmp("quit", token) == 0 || strcmp("exit", token) == 0)
        {
            info("Exiting program");
            exit(0);
            break;
        }
        else if(strcmp("filter", token) == 0 || strcmp("find", token) == 0 || strcmp("search", token) == 0)
        {
            char *column = strtok(NULL, " ");
            if(column == NULL) {
                error("Please provide a column to search for");
                printf(YELLOW "Usage: filter <column> <value>\n");
                printf(YELLOW "Available columns: id, name, surname, address, pesel, currentBalance, loanBalance, loanInterest\n" RESET);
                continue;
            }
            char *value = strtok(NULL, " ");
            if(value == NULL) {
                error("Please provide a value to search for or use 'list' to display all accounts");
                continue;
            }
            filterAccounts(column, value, false);
        }
        else if(strcmp("deposit", token) == 0)
        {
            char *id = strtok(NULL, " ");
            if(id == NULL) {
                error("Please provide an account ID to deposit to");
                continue;
            }

            char *amount = strtok(NULL, " ");
            if(amount == NULL) {
                error("Please provide an amount to deposit");
                continue;
            }

            if(!validateInt(amount, "Amount has to be a positive number")) continue;
            if(!askForConfirmation()) continue;

            makeDeposit(atoi(id), atoi(amount));

        }
        else if(strcmp("withdraw", token) == 0)
        {
            char *id = strtok(NULL, " ");
            if(id == NULL) {
                error("Please provide an account ID to withdraw from");
                continue;
            }

            char *amount = strtok(NULL, " ");
            if(amount == NULL) {
                error("Please provide an amount to withdraw");
                continue;
            }

            if(!validateInt(amount, "Amount has to be a positive number")) continue;
            if(!askForConfirmation()) continue;

            Account client = filterAccounts("id", id, true);

            if(client.currentBalance < atoi(amount))
            {
                error("Insufficient funds");
                continue;
            }

            makeDeposit(atoi(id), atoi(amount) * -1);
            free(client.firstName);
            free(client.lastName);
            free(client.address);
            free(client.pesel);
        }
        else if(strcmp("help", token) == 0 || strcmp("h", token) == 0)
        {   
            help();
        }
        else if(strcmp("clear", token) == 0 || strcmp("cls", token) == 0)
        {
            cleanTerminal();
        }
        else {
            error("Invalid command, type 'help' for available commands");
        }

    }

    return 0;
}