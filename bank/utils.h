#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool validateInt(char *value, char *message);
bool validatePESEL(char *value);
bool validate(char *value, int len, char *message);

#endif // UTILS_H