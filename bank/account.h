#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>

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

int getLastId();
void createAccount(Account account);
void displayAccount(Account account);
void displayAccountWrapper(Account account);
void getAllAccounts();
Account filterAccounts(char* column, char* value, bool isReturnNeeded);
void makeDeposit(int id, int amount);
void payLoan(int id, int amount);
void transferMoney(char* senderId, char* receiptientId, char* amount);
void takeLoan(int id, int amount);

#endif // ACCOUNT_H