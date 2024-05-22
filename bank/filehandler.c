#include "filehandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "constants.h"
#include "logger.h"

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