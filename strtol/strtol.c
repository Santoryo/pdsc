#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

// Leo Ryba 252575

const char* skipWhiteSpaces(const char *currentPtr);
bool checkSign(const char **currentPtr);
int determineBase(const char **currentPtr, int base);
int convertCharToNumber(char c, int base);
bool checkOverflow(long finalValue, int current_number, int base, bool isPositive);
void handleOverflow(const char **currentPtr, char **endPtr, bool isPositive);

long strtol(const char *nPtr, char **endPtr, int base)
{
    const char *currentPtr = nPtr;
    int current_number;
    long finalValue = 0;
    
    bool isPositive = true;
    bool isEmptyString = true;
    bool isOverflow = false;

    // Check if nPtr is NULL
    if (nPtr == NULL)
    {
        errno = EINVAL;
        return 0;
    }

    if (*currentPtr == '\0')
    {
        if (endPtr)
            *endPtr = (char *)currentPtr;
        return 0;
    }

    // Check if base is valid
    if (base != 0 && (base < 2 || base > 36))
    {
        errno = EINVAL;
        return 0;
    }

    currentPtr = skipWhiteSpaces(currentPtr);

    isPositive = checkSign(&currentPtr);

    base = determineBase(&currentPtr, base);

    // Convert the string to a number
    while (*currentPtr != '\0')
    {
        current_number = convertCharToNumber(*currentPtr, base);
        if (current_number == -1)
            break;

        isOverflow = checkOverflow(finalValue, current_number, base, isPositive);

        // Check for overflow
        if (isOverflow)
        {
            handleOverflow(&currentPtr, endPtr, isPositive);
            return isPositive ? LONG_MAX : LONG_MIN;
        }

        if (isPositive)
        {
            finalValue = finalValue * base + current_number;
        }
        else
        {
            finalValue = finalValue * base - current_number;
        }
        currentPtr++;
        isEmptyString = false;
    }

    // Check if the string is empty
    if (isEmptyString)
    {
        if (base == 8 || base == 16)
        {
            if (base == 16)
                currentPtr--;
            if (endPtr)
            {
                *endPtr = (char *)currentPtr;
            }
        }
        return 0;
    }

	// Set endPtr to the end of the string if it is not NULL
    if (endPtr)
    {
        *endPtr = (char *)currentPtr;
    }

    return finalValue;
}

// Skip white spaces
const char* skipWhiteSpaces(const char *currentPtr)
{
    while (isspace(*currentPtr))
    {
        currentPtr++;
    }
    return currentPtr;
}

// Check if the number is positive or negative
bool checkSign(const char **currentPtr)
{
    bool isPositive = true;
    switch (**currentPtr)
    {
        case '-':
            isPositive = false;
            (*currentPtr)++;
            break;
        case '+':
            (*currentPtr)++;
            break;
    }
    return isPositive;
}

// Determine base
int determineBase(const char **currentPtr, int base)
{
    if (base == 16 && **currentPtr == '0' && (*(*currentPtr + 1) == 'X' || *(*currentPtr + 1) == 'x'))
    {
        *currentPtr += 2;
    }
    else if (base == 8 && **currentPtr == '0')
    {
        (*currentPtr)++;
    }
    else if (base == 0)
    {
        if (**currentPtr == '0')
        {
            (*currentPtr)++;
            base = (**currentPtr == 'X' || **currentPtr == 'x') ? 16 : 8;
            if (base == 16) (*currentPtr)++;
        }
        else
        {
            base = 10;
        }
    }
    return base;
}

// Convert character to number
int convertCharToNumber(char c, int base)
{
    if (isdigit(c) && (c - '0') < base)
    {
        return c - '0';
    }
    else if (isalpha(c) && (toupper(c) - 'A' + 10) < base)
    {
        return toupper(c) - 'A' + 10;
    }
    else
    {
        return -1; // Invalid character for the given base
    }
}

// Check for overflow
bool checkOverflow(long finalValue, int current_number, int base, bool isPositive)
{
    if (isPositive)
    {
        return finalValue > (LONG_MAX - current_number) / base;
    }
    else
    {
        return finalValue < (LONG_MIN + current_number) / base;
    }
}

// Handle overflow
void handleOverflow(const char **currentPtr, char **endPtr, bool isPositive)
{
    while (**currentPtr != '\0')
    {
        (*currentPtr)++;
        if (!isdigit(**currentPtr) && !isalpha(**currentPtr))
        {
            break;
        }
    }

    if (endPtr)
    {
        *endPtr = (char *)*currentPtr;
    }
    errno = ERANGE;
}