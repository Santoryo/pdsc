#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Binary search function to find element in a sorted array.
// Returns a pointer to the found element or NULL if not found.
void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    ssize_t low = 0;
    ssize_t high = num - 1;

    while (low <= high)
    {
        ssize_t mid = (low + high) / 2;
        const void *middle = (const char *)base + mid * size;
        int outputResult = compar(key, middle);

        switch (outputResult)
        {
            case 0:
                return (void *)middle;
            case -1:
                // Target is smaller, so we search in the left half
                high = mid - 1;
                break;
            default:
                // Target is larger, so we search in the right half
                low = mid + 1;
                break;
        }
    }

    return NULL;
}

int compareInts(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}

int compareChars(const void *a, const void *b)
{
    return (*(char *)a > *(char *)b) - (*(char *)a < *(char *)b);
}

int compareFloats(const void *a, const void *b)
{
    float diff = *(float *)a - *(float *)b;
    return (diff > 0) - (diff < 0);
}

typedef struct
{
    int id;
    char name[20];
} Person;

int comparePeople(const void *a, const void *b)
{
    return ((Person *)a)->id - ((Person *)b)->id;
}

int compareStrings(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

int main()
{
    // Char array
    printf("Char tests:\n");

    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    char key = 'a';
    char *found = (char *)bsearch(&key, arr, sizeof(arr)/sizeof(arr[0]), sizeof(char), compareChars);

    printf("Searching for: %c\n", key);

    if (found)
    {
        printf("Element found: %c, Index: %ld\n", *found, found - arr);
    }
    else
    {
        printf("Element not found\n");
    }


    printf("\nInteger tests:\n");
    // Int array
    int arr2[] = {-3, -1, 1, 2, 3, 4, 5, 6};
    int key2 = -1;
    int *found2 = (int *)bsearch(&key2, arr2, sizeof(arr2)/sizeof(arr2[0]), sizeof(int), compareInts);

    printf("Searching for: %d\n", key2);

    if (found2)
    {
        printf("Element found: %d, Index: %ld\n", *found2, found2 - arr2);
    }
    else
    {
        printf("Element not found\n");
    }

    int key2a = 0;
    int *found2a = (int *)bsearch(&key2a, arr2, sizeof(arr2)/sizeof(arr2[0]), sizeof(int), compareInts);

    printf("Searching for: %d\n", key2a);

    if (found2a)
    {
        printf("Element found: %d, Index: %ld\n", *found2a, found2a - arr2);
    }
    else
    {
        printf("Element not found\n");
    }


    printf("\nFloat tests:\n");

    // Float array
    float arr3[] = {0.01, 0.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 33.33};
    float key3 = -4.4;
    float *found3 = (float *)bsearch(&key3, arr3, sizeof(arr3)/sizeof(arr3[0]), sizeof(float), compareFloats);

    printf("Searching for: %.1f\n", key3);

    if (found3)
    {
        printf("Element found: %.1f, Index: %ld\n", *found3, found3 - arr3);
    }
    else
    {
        printf("Element not found\n");
    }

    float key3a = 1.23;
    float *found3a = (float *)bsearch(&key3a, arr3, sizeof(arr3)/sizeof(arr3[0]), sizeof(float), compareFloats);

    printf("Searching for: %.1f\n", key3a);

    if (found3a)
    {
        printf("Element found: %.1f, Index: %ld\n", *found3a, found3a - arr3);
    }
    else
    {
        printf("Element not found\n");
    }

    printf("\nStruct tests:\n");

    // Array of structs
    Person arr4[] = {{1, "Kacper"}, {2, "Jakub"}, {3, "Marek"}, {4, "Agata"}, {5, "Ewelina"}, {6, "Kamil"}};
    Person key4 = {6, ""}; // Search by id
    Person *found4 = (Person *)bsearch(&key4, arr4, sizeof(arr4)/sizeof(arr4[0]), sizeof(Person), comparePeople);

    printf("Searching for: %d %s\n", key4.id, key4.name);

    if (found4)
    {
        printf("Element found: %d %s, Index: %ld\n", found4->id, found4->name, found4 - arr4);
    }
    else
    {
        printf("Element not found\n");
    }

    Person key4a = {7, ""}; // Search by id
    Person *found4a = (Person *)bsearch(&key4a, arr4, sizeof(arr4)/sizeof(arr4[0]), sizeof(Person), comparePeople);

    printf("Searching for: %d %s\n", key4a.id, key4a.name);

    if (found4a)
    {
        printf("Element found: %d %s, Index: %ld\n", found4a->id, found4a->name, found4a - arr4);
    }
    else
    {
        printf("Element not found\n");
    }

    printf("\nString tests:\n");

    // Strings comparision
    char *arr5[] = {"ab", "bc", "cd", "de", "efgh", "ijklm", "nopqrst", "uvwxyz"};
    char *key5 = "nopqrst";
    char **found5 = (char **)bsearch(&key5, arr5, sizeof(arr5)/sizeof(arr5[0]), sizeof(char *), compareStrings);

    printf("Searching for: %s\n", key5);

    if (found5)
    {
        printf("Element found: %s, Index: %ld\n", *found5, found5 - arr5);
    }
    else
    {
        printf("Element not found\n");
    }

    char *key5a = "zzzzz";
    char **found5a = (char **)bsearch(&key5a, arr5, sizeof(arr5)/sizeof(arr5[0]), sizeof(char *), compareStrings);
    
    printf("Searching for: %s\n", key5a);

    if (found5a)
    {
        printf("Element found: %s, Index: %ld\n", *found5a, found5a - arr5);
    }
    else
    {
        printf("Element not found\n");
    }



    return 0;
}
