#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define FILENAME "accounts.txt"


// Structure for an account, with the following fields:
// - id: unique identifier for the account
// - firstName: first name of the account holder
// - lastName: last name of the account holder
// - address: address of the account holder
// - pesel: PESEL number of the account holder
// - currentBalance: current balance of the account in cents
// - loanBalance: balance of the loan in cents
// - loanInterest: interest rate of the loan
typedef struct {
  int id;
  char *firstName;
  char *lastName;
  char *address;
  char *pesel;
  int currentBalance; // in cents
  int loanBalance;
  int loanInterest;
} Account;

// Check if a file exists with the given filename
bool checkIfFileExists(char *filename) {
    FILE *file = fopen(filename, "r");

    if(file == NULL) {
        return false;
    }

    fclose(file);
    return true;
}

// Create a file with the given filename
void createFile(char *filename) {
    FILE *file = fopen(filename, "w");
    fclose(file);
}

void clearTerminal() {
    system("clear");
}

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


    fprintf(file, "%d;%s;%s;%s;%s;%d;%d;%d\n", account.id, account.firstName, account.lastName, account.address, account.pesel, account.currentBalance, account.loanBalance, account.loanInterest);

    fclose(file);
}

void displayAccount(Account account) {

    printf("%.3d %.20s %.20s %.20s %.11s %.9d %.9d %.9d\n", account.id, account.firstName, account.lastName, account.address, account.pesel, account.currentBalance, account.loanBalance, account.loanInterest);
    
    }

void raiseError(char *message) {
    printf("Error: %s\n", message);
}

void getAllAccounts()
{
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        raiseError("File does not exist");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char* id = strtok(line, ";");
        char* firstName = strtok(NULL, ";");
        char* lastName = strtok(NULL, ";");
        char* address = strtok(NULL, ";");
        char* pesel = strtok(NULL, ";");
        char* currentBalance = strtok(NULL, ";");
        char* loanBalance = strtok(NULL, ";");
        char* loanInterest = strtok(NULL, ";");

        Account account;
        account.id = atoi(id);
        account.firstName = firstName;
        account.lastName = lastName;
        account.address = address;
        account.pesel = pesel;
        account.currentBalance = atoi(currentBalance);
        account.loanBalance = atoi(loanBalance);
        account.loanInterest = atoi(loanInterest);

        displayAccount(account);
    }

    fclose(file);
    if (line)
        free(line);
}

void filterAccounts(char* column, char* value)
{
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        raiseError("File does not exist");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char* id = strtok(line, ";");
        char* firstName = strtok(NULL, ";");
        char* lastName = strtok(NULL, ";");
        char* address = strtok(NULL, ";");
        char* pesel = strtok(NULL, ";");
        char* currentBalance = strtok(NULL, ";");
        char* loanBalance = strtok(NULL, ";");
        char* loanInterest = strtok(NULL, ";");

        Account account;
        account.id = atoi(id);
        account.firstName = firstName;
        account.lastName = lastName;
        account.address = address;
        account.pesel = pesel;
        account.currentBalance = atoi(currentBalance);
        account.loanBalance = atoi(loanBalance);
        account.loanInterest = atoi(loanInterest);

        if(strcmp(column, "id") == 0 && account.id == atoi(value))
        {
            displayAccount(account);
        }
        else if(strcmp(column, "firstName") == 0 && strcmp(account.firstName, value) == 0)
        {
            displayAccount(account);
        }
        else if(strcmp(column, "lastName") == 0 && strcmp(account.lastName, value) == 0)
        {
            displayAccount(account);
        }
        else if(strcmp(column, "address") == 0 && strcmp(account.address, value) == 0)
        {
            displayAccount(account);
        }
        else if(strcmp(column, "pesel") == 0 && strcmp(account.pesel, value) == 0)
        {
            displayAccount(account);
        }
        else if(strcmp(column, "currentBalance") == 0 && account.currentBalance == atoi(value))
        {
            displayAccount(account);
        }
        else if(strcmp(column, "loanBalance") == 0 && account.loanBalance == atoi(value))
        {
            displayAccount(account);
        }
        else if(strcmp(column, "loanInterest") == 0 && account.loanInterest == atoi(value))
        {
            displayAccount(account);
        }
    }

    fclose(file);
    if (line)
        free(line);


}

int main(void)
{
    clearTerminal();

    char command[1024];

    filterAccounts("firstName", "AA");

    printf("Bank Management System\n");
    
    while(1)
    {
      printf("Enter command (c - create account, d - display account, q - quit): ");
      fgets(command, sizeof(command), stdin);
      switch(command[0])
      {
        case 'c':
          printf("Creating account\n");
          Account account;
          account.firstName = "AA";
          account.lastName = "BB";
          account.address = "123";
          account.pesel = "123";
          account.currentBalance = 1000;
          account.loanBalance = 0;
          account.loanInterest = 0;
          createAccount(account);
          break;
        case 'd':
          printf("Displaying accounts\n");
          getAllAccounts();
          break;
        case 'q':
          printf("Quitting\n");
          return 0;
        case 'p':
          clearTerminal();
          break;
        default:
          printf("Invalid command\n");
          break;
      }
    }

    return 0;
}