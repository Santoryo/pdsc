#include <stdio.h>
#include <stdlib.h>

// Leo Ryba 252575
// Binary search function to find element in a sorted array.
// Returns a pointer to the found element or NULL if not found.
void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    size_t low = 0;
    size_t high = num - 1;

    while (low <= high)
    {
        size_t mid = (low + high) / 2;
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

int main()
{
    // Char array
    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    char key = 'd';
    char *found = (char *)bsearch(&key, arr, sizeof(arr)/sizeof(arr[0]), sizeof(char), compareChars);

    if (found)
    {
        printf("Element found: %c, Index: %ld\n", *found, found - arr);
    }
    else
    {
        printf("Element not found\n");
    }

    // Int array
    int arr2[] = {1, 2, 3, 4, 5};
    int key2 = 2;
    int *found2 = (int *)bsearch(&key2, arr2, sizeof(arr2)/sizeof(arr2[0]), sizeof(int), compareInts);

    if (found2)
    {
        printf("Element found: %d, Index: %ld\n", *found2, found2 - arr2);
    }
    else
    {
        printf("Element not found\n");
    }

    // Float array
    float arr3[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    float key3 = 4.4;
    float *found3 = (float *)bsearch(&key3, arr3, sizeof(arr3)/sizeof(arr3[0]), sizeof(float), compareFloats);

    if (found3)
    {
        printf("Element found: %.1f, Index: %ld\n", *found3, found3 - arr3);
    }
    else
    {
        printf("Element not found\n");
    }

    // Array of structs
    Person arr4[] = {{1, "Kacper"}, {2, "Jakub"}, {3, "Marek"}, {4, "Agata"}, {5, "Ewelina"}, {6, "Kamil"}};
    Person key4 = {6, ""}; // Search by id
    Person *found4 = (Person *)bsearch(&key4, arr4, sizeof(arr4)/sizeof(arr4[0]), sizeof(Person), comparePeople);

    if (found4)
    {
        printf("Element found: %d %s, Index: %ld\n", found4->id, found4->name, found4 - arr4);
    }
    else
    {
        printf("Element not found\n");
    }

    return 0;
}
