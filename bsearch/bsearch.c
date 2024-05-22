#include <stdio.h>

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
    return (*(const char *)a > *(const char *)b) - (*(const char *)a < *(const char *)b);
}

int main()
{
    // Char array
    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    char key = 'd';
    char *found = (char *)bsearch(&key, arr, sizeof(arr)/sizeof(arr[0]), sizeof(char), compareChars);

    if (found)
    {
        printf("Element found: %c\n", *found);
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
        printf("Element found: %d\n", *found2);
    }
    else
    {
        printf("Element not found\n");
    }

    return 0;
}