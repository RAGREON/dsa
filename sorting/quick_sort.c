// quick sort implementation in c

#include <stdio.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* array, int left, int right) {
    int pivot = left + (right - left) / 2;
    int pivot_element = array[pivot];

    swap(&array[right], &array[pivot]);

    int j = left - 1;
    for (int i = left; i < right; i++) {
        if (array[i] < pivot_element) {
            swap(&array[++j], &array[i]); 
        }
    }

    swap(&array[++j], &array[right]);
    return j;
}

void quick_sort(int* array, int left, int right) {
    if (left < right) {
        int pivot = partition(array, left, right);

        quick_sort(array, left, pivot);
        quick_sort(array, pivot + 1, right);
    }
}

void print_array(int* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");
}

int main(int argc, char** argv) {
    int array[] = { 1, 7, 9, 2, 3, 1, 0, 99, 12, 121, -6 };
    int size = sizeof(array) / sizeof(int);

    quick_sort(array, 0, size - 1);

    print_array(array, size);
    
    return 0;
}
