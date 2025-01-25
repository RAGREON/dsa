// merge sort implementation in c

#include <stdio.h>

void merge(int* array, int left, int mid, int right) {
    int l_size = mid - left + 1;
    int r_size = right - mid;

    int l_array[l_size], r_array[r_size];
    
    for (int i = 0; i < l_size; i++)
        l_array[i] = array[left + i];

    for (int i = 0; i < r_size; i++)
        r_array[i] = array[mid + 1 + i];

    int l_p = 0, r_p = 0, a_p = left;

    while (l_p < l_size && r_p < r_size) {
        if (l_array[l_p] < r_array[r_p]) {
            array[a_p] = l_array[l_p++];
        } else {
            array[a_p] = r_array[r_p++];
        }

        a_p++;
    }

    while (l_p < l_size) {
        array[a_p++] = l_array[l_p++];
    } 

    while (r_p < r_size) {
        array[a_p++] = r_array[r_p++];
    }
}

void merge_sort(int* array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(array, left, mid);
        merge_sort(array, mid + 1, right);

        merge(array, left, mid, right);
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

    merge_sort(array, 0, size - 1);

    print_array(array, size);
    
    return 0;
}