#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "utils.h"
#include "logger.h"

bool validateInt(char *value, char *message)
{
    if (value == NULL || atoi(value) <= 0)
    {
        error(message);
        return false;
    }
    return true;
}

bool validatePESEL(char *value)
{
    if (strlen(value) != 11)
    {
        error("PESEL has to be 11 characters long");
        return false;
    }
    return true;
}

bool validate(char *value, int len, char *message)
{
    if (value == NULL || strlen(value) < len)
    {
        error(message);
        return false;
    }
    return true;
}