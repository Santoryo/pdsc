#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "account.h"
#include <stdbool.h>

void createFile(char *filename);
void parseFile(void (*func)(Account));
void raiseError(char *message);
bool checkIfFileExists(char *filename);

#endif // FILEHANDLER_H