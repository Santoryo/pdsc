#include <stdio.h>

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *)) {
    size_t low = 0;
    size_t high = num - 1;

    while (low <= high) {
        size_t mid = (low + high) / 2;
        const void *mid_elem = (const char *)base + mid * size;
        int cmp_result = compar(key, mid_elem);

        if (cmp_result == 0) {
            return (void *)mid_elem;
        } else if (cmp_result < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return NULL;
}

int compare_ints(const void *a, const void *b) {
    int int_a = *(int *)a;
    int int_b = *(int *)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

int compare_chars(const void *a, const void *b) {
    char char_a = *(const char *)a;
    char char_b = *(const char *)b;
    return (char_a > char_b) - (char_a < char_b);
}

int main() {
    char arr[] = {'a', 'b', 'c', 'd', 'e'};
    char key = 'c';
    char *found = (char *)bsearch(&key, arr, sizeof(arr)/sizeof(arr[0]), sizeof(char), compare_chars);

    // Check if the element was found
    if (found) {
        printf("Element found: %d\n", *found);
    } else {
        printf("Element not found\n");
    }

    return 0;
}