#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

// Leo Ryba 252575

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

	// Skip white spaces

	while (isspace(*currentPtr))
	{
		currentPtr++;
	}

	// Check if the number is positive or negative

	switch (*currentPtr)
	{
		case '-':
			isPositive = false;
			currentPtr++;
			break;
		case '+':
			currentPtr++;
			break;
	}

	// Base

	if (base == 16 && *currentPtr == '0' && (*(currentPtr + 1) == 'X' || *(currentPtr + 1) == 'x'))
	{
		currentPtr += 2;
	}

	if (base == 8 && *currentPtr == '0')
	{
		currentPtr++;
	}

	if (base == 0 && *currentPtr == '0')
	{
		currentPtr++;
		base = (*currentPtr == 'X' || *currentPtr == 'x') ? 16 : 8;
		if (base == 16) currentPtr++;
	}

	else if(base == 0)
	{
		base = 10;
	}

	// Convert the string to a number

	while (*currentPtr != '\0')
	{
		if (isdigit(*currentPtr) && (*currentPtr - '0') < base)
		{
			current_number = *currentPtr - '0';
		}
		else if (isalpha(*currentPtr) && (toupper(*currentPtr) - 'A' + 10) < base)
		{
			current_number = toupper(*currentPtr) - 'A' + 10;
		}
		else
		{
			break;
		}

		if (finalValue > (LONG_MAX - current_number) / base || finalValue < (LONG_MIN + current_number) / base)
		{
			isOverflow = true;
		}

		// Check for overflow

		if (isOverflow)
		{
			while (*currentPtr != '\0')
			{
				currentPtr++;
				if (!isdigit(*currentPtr) && !isalpha(*currentPtr))
				{
					break;
				}
			}

			if (endPtr)
			{
				*endPtr = (char *)currentPtr;
			}
			errno = ERANGE;

			switch (isPositive)
			{
				case true:
					return LONG_MAX;
				case false:
					return LONG_MIN;
			}
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

	if (isEmptyString && base != 8 && base != 16)
	{
		return 0;
	}

	if (isEmptyString && base == 16)
	{
		currentPtr--;
		if (endPtr)
		{
			*endPtr = (char *)currentPtr;
		}
		return 0;
	}

	if (endPtr)
	{
		*endPtr = (char *)currentPtr;
	}

	return finalValue;
}