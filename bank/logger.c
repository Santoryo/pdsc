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
    printf("1. create - create a new account\n");
    printf("2. list - display all accounts (Alias: ls)\n");
    printf("3. find - find an account by ID (Alias: filter, search)\n");
    printf("4. update - update an account by ID\n");
    printf("5. delete - delete an account by ID (Alias: remove)\n");
    printf("6. help - display this help message (Alias: h)\n");
    printf("7. clear - clear the terminal screen (Alias: cls)\n");
    printf("10. quit - exit the program (Alias: q, exit)\n");
    printf(BOLDCYAN "=====================================================\n" RESET);
}