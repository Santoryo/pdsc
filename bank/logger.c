#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "logger.h"


void cleanTerminal() {
    system("clear");
}

void error(char *message) {
    printf(RED "[ERROR]: %s\n" RESET, message);
}

void warning(char *message) {
    printf(YELLOW "[WARNING]: %s\n" RESET, message);
}

void info(char *message) {
    printf(GREEN "[INFO]: %s\n" RESET, message);
}

void help() {
    printf(BOLDCYAN "=====================================================\n");
    printf(BOLDCYAN "Bank Management System\n" RESET);
    printf(BOLDCYAN "Available commands:\n" RESET CYAN);
    printf("- create - create a new account\n");
    printf("- list - display all accounts (Alias: ls)\n");
    printf("- find <column> <value> - find an account by ID (Alias: filter, search)\n");
    printf("- payoff <id>  - pays off the loan from the account using account balance\n");
    printf("- takeloan <id> <amount> - take a loan from the bank\n");
    printf("- deposit <id> <amount> - deposit money to the account\n");
    printf("- withdraw <id> <amount> - withdraw money from the account\n");
    printf("- transfer <to> <from> <ammount> - transfer money between accounts\n");
    printf("- help - display this help message (Alias: h)\n");
    printf("- clear - clear the terminal screen (Alias: cls)\n");
    printf("- quit - exit the program (Alias: q, exit)\n");
    printf(BOLDCYAN "=====================================================\n" RESET);
}