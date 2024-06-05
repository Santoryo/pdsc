#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "account.h"
#include "filehandler.h"
#include "constants.h"
#include "logger.h"
#include "utils.h"

bool askForConfirmation()
{
    char answer[5];
    printf(BOLDYELLOW "Are you sure you want to continue? [y/n]: " RESET);
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = 0;

    if (strcmp(answer, "y") == 0 || strcmp(answer, "yes") == 0)
        return true;
    return false;
}

void createHandler()
{
    Account account;
    char buffer[256];

    printf("Enter first name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate(buffer, 0, "First name can't be empty"))
        return;
    account.firstName = strdup(buffer);

    printf("Enter last name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validate(buffer, 0, "Last name can't be empty"))
        return;
    account.lastName = strdup(buffer);

    printf("Enter address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    account.address = strdup(buffer);
    if (!validate(account.address, 1, "Address can't be empty"))
        return;

    printf("Enter PESEL: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (!validatePESEL(buffer))
        return;
    account.pesel = strdup(buffer);

    printf("Enter current balance (optional): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (buffer[0] == '\0')
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
    if (buffer[0] == '\0')
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
    if (buffer[0] == '\0')
    {
        warning("Loan interest not provided, setting to default value");
        account.loanInterest = DEFAULT_LOAN_INTEREST;
    }
    else
    {
        account.loanInterest = atoi(buffer);
    }

    createAccount(account);
}

void filterHandler()
{
    char *column = strtok(NULL, " ");
    if (column == NULL)
    {
        error("Please provide a column to search for");
        printf(YELLOW "Usage: filter <column> <value>\n");
        printf(YELLOW "Available columns: id, name, surname, address, pesel, currentBalance, loanBalance, loanInterest\n" RESET);
        return;
    }
    char *value = strtok(NULL, " ");
    if (value == NULL)
    {
        error("Please provide a value to search for or use 'list' to display all accounts");
        return;
    }
    filterAccounts(column, value, false);
}

void depositHandler()
{
    char *id = strtok(NULL, " ");
    if (id == NULL)
    {
        error("Please provide an account ID to deposit to");
        return;
    }

    char *amount = strtok(NULL, " ");
    if (amount == NULL)
    {
        error("Please provide an amount to deposit");
        return;
    }

    if (!validateInt(amount, "Amount has to be a positive number"))
        return;
    if (!askForConfirmation())
        return;

    makeDeposit(atoi(id), atof(amount) * 100.0f);
    
}

void loanPayoffHandler()
{
    char *id = strtok(NULL, " ");
    if (id == NULL)
    {
        error("Please provide an account ID to pay off the loan");
        return;
    }

    Account client = filterAccounts("id", id, true);

    if (client.loanBalance == 0)
    {
        error("No loan to pay off");
        return;
    }

    if (!askForConfirmation())
        return;

    payLoan(atoi(id), atof(client.loanBalance) * 100.0f);
    free(client.firstName);
    free(client.lastName);
    free(client.address);
    free(client.pesel);
}

void withdrawHandler()
{
    char *id = strtok(NULL, " ");
    if (id == NULL)
    {
        error("Please provide an account ID to withdraw from");
        return;
    }

    char *amount = strtok(NULL, " ");
    if (amount == NULL)
    {
        error("Please provide an amount to withdraw");
        return;
    }

    if (!validateInt(amount, "Amount has to be a positive number"))
        return;
    if (!askForConfirmation())
        return;

    Account client = filterAccounts("id", id, true);

    if (client.currentBalance < atoi(amount))
    {
        error("Insufficient funds");
        return;
    }

    makeDeposit(atoi(id), atoi(amount) * -1);
    free(client.firstName);
    free(client.lastName);
    free(client.address);
    free(client.pesel);
}

void transferHandler()
{
    char *senderId = strtok(NULL, " ");
    if (senderId == NULL)
    {
        error("Please provide an account ID to transfer from");
        return;
    }

    char *receiptientId = strtok(NULL, " ");
    if (receiptientId == NULL)
    {
        error("Please provide an account ID to transfer to");
        return;
    }

    char *amount = strtok(NULL, " ");
    if (amount == NULL)
    {
        error("Please provide an amount to transfer");
        return;
    }

    if (!validateInt(amount, "Amount has to be a positive number"))
        return;
    if (!askForConfirmation())
        return;

    transferMoney(senderId, receiptientId, amount);
}

void takeLoanHandler()
{
    char *id = strtok(NULL, " ");
    if (id == NULL)
    {
        error("Please provide an account ID to take a loan from");
        return;
    }

    char *amount = strtok(NULL, " ");
    if (amount == NULL)
    {
        error("Please provide an amount to take a loan");
        return;
    }

    if (!validateInt(amount, "Amount has to be a positive number"))
        return;
    if (!askForConfirmation())
        return;

    Account client = filterAccounts("id", id, true);

    if (client.loanBalance > 0)
    {
        error("Loan already taken");
        return;
    }

    takeLoan(atoi(id), atoi(amount));
    free(client.firstName);
    free(client.lastName);
    free(client.address);
    free(client.pesel);
}

int main(void)
{
    while (1)
    {
        char command[1024];
        char *token;
        printf(BOLDMAGENTA "[ADMIN] $ " RESET);

        fgets(command, 1024, stdin);
        command[strcspn(command, "\n")] = 0;

        if(command[0] == '\0')
            continue;

        token = strtok(command, " ");

        if (strcmp("create", token) == 0 || strcmp("c", token) == 0)
        {
            createHandler();
        }
        else if (strcmp("list", token) == 0 || strcmp("ls", token) == 0)
        {
            getAllAccounts();
        }
        else if (strcmp("q", token) == 0 || strcmp("quit", token) == 0 || strcmp("exit", token) == 0)
        {
            info("Exiting program");
            exit(0);
        }
        else if (strcmp("filter", token) == 0 || strcmp("find", token) == 0 || strcmp("search", token) == 0)
        {
            filterHandler();
        }
        else if (strcmp("deposit", token) == 0)
        {
            depositHandler();
        }
        else if (strcmp("withdraw", token) == 0)
        {
            withdrawHandler();
        }
        else if (strcmp("help", token) == 0 || strcmp("h", token) == 0)
        {
            help();
        }
        else if (strcmp("clear", token) == 0 || strcmp("cls", token) == 0)
        {
            cleanTerminal();
        }
        else if(strcmp("payoff", token) == 0)
        {
            loanPayoffHandler();
        }
        else if(strcmp("transfer", token) == 0)
        {
            transferHandler();
        }
        else if(strcmp("takeloan", token) == 0)
        {
            takeLoanHandler();
        }
        else
        {
            error("Invalid command, type 'help' for available commands");
        }
    }

    return 0;
}