// binary search solution in c
// [problem: 704]

#include <stdio.h>

int search (int* nums, int numsSize, int target) {
    if (numsSize < 1)
        return -1;

    int low = 0, high = numsSize - 1, mid;

    while (low <= high) {
        mid = low + (high - low) / 2;

        if (nums[mid] == target) {
            return mid;
        } 
        else if (nums[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return -1;
}

int main(int argc, char** argv) {
    int array[] = { -1,0,3,5,9,12 };
    int size = sizeof(array) / sizeof(int);
    int target = 9;

    int index = search(array, size, target);

    printf("index: %d\n", index);

    return 0;
}