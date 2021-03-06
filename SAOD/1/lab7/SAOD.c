#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MIN 0
#define MAX 100000
#define INT_SIZE 4

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max) {
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

void printArray(uint32_t arr[], int sizeArr) {
    for (int i = 0; i < sizeArr; i++) {
	printf(" %d", arr[i]);
    }
    printf("\n\n");
}

void fillArray(uint32_t arr[], int sizeArr) {
    for (int i = 0; i < sizeArr; i++) {
	arr[i] = getrand(MIN, MAX);
    }
}

void bubbleSort(uint32_t arr[], int sizeArr) {
    int swapped = 1;
    while (swapped == 1) {
	swapped = 0;
	int temp;
	for (int i = 0; i < sizeArr - 1; ++i) {
	    if (arr[i] > arr[i + 1]) {
		temp = arr[i];
		arr[i] = arr[i + 1];
		arr[i + 1] = temp;
		swapped = 1;
	    }
	}
    }
}

void countSort(uint32_t arr[], int sizeArr) {
    int max = 0;
    for (int i = 0; i < sizeArr; i++) {
	if (max < arr[i]) {
	    max = arr[i];
	}
    }
    max++;
    int count[max];

    for (int i = 0; i < max; i++) {
	count[i] = 0;
    }

    for (int i = 0; i < sizeArr; i++) {
	count[arr[i]]++;
    }

    int temp = 0;
    int currentSorteddIndex = 0;

    for (int size = 0; size < max; size++) {
	temp = count[size];
	for (int k = 0; k < temp; k++) {
	    arr[currentSorteddIndex] = size;
	    currentSorteddIndex++;
	}
    }
}

void swap(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(uint32_t arr[], int n, int i) {
    int l = 2 * i;
    int r = 2 * i + 1;
    int max = i;

    if (l < n && arr[l] > arr[max]) {
	max = l;
    } else {
	max = i;
    }

    if (r < n && arr[r] > arr[max]) {
	max = r;
    }

    if (max != i) {
	swap(&arr[i], &arr[max]);
	heapify(arr, n, max);
    }
}

void heapSort(uint32_t arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
	heapify(arr, n, i);
  }

    for (int i = n - 1; i >= 0; i--) {
	swap(&arr[0], &arr[i]);
	heapify(arr, i, 0);
    }
}

int main(void) {

    for (uint32_t size = 50000; size <= 1000001; size += 50000) {
	uint32_t arrCount[size];
	fillArray(arrCount, size);

	uint32_t arrBubble[size];
	memcpy(arrBubble, arrCount, size * INT_SIZE);

        uint32_t arrHeap[size];
	memcpy(arrHeap, arrCount, size * INT_SIZE);

	double dTimeCount = wtime();
	countSort(arrCount, size);
	dTimeCount = wtime() - dTimeCount;

	double dTimeBubble = wtime();
	\\bubbleSort(arrBubble, size);
	dTimeBubble = wtime() - dTimeBubble;

        double dTimeHeap = wtime();
	heapSort(arrHeap, size);
	dTimeHeap = wtime() - dTimeHeap;

    printf("Size: %d\n",size);
	printf("Time CountSort:  %f\n", dTimeCount);
	printf("Time BubbleSort: %f\n", dTimeBubble);
    printf("Time HeapSort:   %f\n", dTimeHeap);
    printf("\n");
    }

    return 0;
}