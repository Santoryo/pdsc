#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_SIZE 20

void raiseError() 
{
    printf("Error in memory allocation\n");
}

char* getLine() 
{
    int character;
    int allocatedSize = ALLOC_SIZE;
    int size = 0;
    char* line = calloc(allocatedSize, sizeof(char));
    
    if (!line) return NULL;
    while (1) 
    {
        character = getchar();
        if (character == EOF || character == '\n') break;

        line[size++] = character;
        if (size == allocatedSize) 
        {
            allocatedSize += ALLOC_SIZE;
            char* tempLine = realloc(line, allocatedSize * sizeof(char));
            if (!tempLine) 
            {
                raiseError();
                free(line);
                return NULL;
            }
            line = tempLine;
        }
    }
    line[size] = '\0';

    if (size == 0 && character == EOF) 
    {
        free(line);
        return NULL;
    }

    return line;
}

void printBackwards(char* str) 
{
    int length = strlen(str);
    char* reversed = malloc((length + 1) * sizeof(char));
    
    if (!reversed) 
    {
        raiseError();
        return;
    }
    strcpy(reversed, str);

    char** words = calloc(length, sizeof(char*));
    if (!words) 
    {
        raiseError();
        free(reversed);
        return;
    }

    int wordIdx = 0;
    char* word = strtok(reversed, " ");
    while (word) 
    {
        words[wordIdx++] = word;
        word = strtok(NULL, " ");
    }

    for (int i = wordIdx - 1; i >= 0; i--) 
    {
        printf("%s", words[i]);
        if (i > 0) printf(" ");
    }
    printf("\n");

    free(words);
    free(reversed);
}

void cleanup(char*** lines, int count) 
{
    for (int i = 0; i < count; i++) 
    {
        free((*lines)[i]);
    }
    free(*lines);
    *lines = NULL;
}

int main()
{
    int count = 0;
    int capacity = ALLOC_SIZE;
    char* line;
    char** lines = calloc(ALLOC_SIZE, sizeof(char*));

    if (lines == NULL) 
    {
        raiseError();
        return 1;
    }

    while ((line = getLine()) != NULL) 
    {
        lines[count] = line;
        count++;
        if (count == capacity) 
        {
            capacity += ALLOC_SIZE;
            char** tempLines = realloc(lines, capacity * sizeof(char*));
            if (!tempLines) 
            {
                raiseError();
                cleanup(&lines, count);
            }
            lines = tempLines;
        }
    }

    for (int i = count - 1; i >= 0; i--) 
    {
        printBackwards(lines[i]);
    }

    cleanup(&lines, count);
    return 0;
}