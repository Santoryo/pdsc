#include "filehandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "constants.h"
#include "logger.h"

void parseFile(void (*func)(Account)) {
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL) {
        error("File not found");
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

        func(account);
    }

    fclose(file);
    if (line)
        free(line);
}

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