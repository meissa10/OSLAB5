#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;


void merge(int a, int b, int c){
  int x, y, z;
  int n1 = b - a + 1;
  int n2 = c - b;

  int L[n1], R[n2];
  for (x = 0; x < n1; x++)
  L[x] = array[a + x];
  for (y = 0; y < n2; y++)
  R[y] = array[b + 1+ y];
  x = 0; 
  y = 0; 
  z = a; 

  while (x < n1 && y < n2){
    if (L[x] <= R[y])
  {
    array[z] = L[x];
    x++;
  }
  else{
    array[z] = R[y];
    y++;
  }
    z++;
  }
  
  
  while (x < n1)
  {
    array[z] = L[x];
    x++;
    z++;
  }

  while (y < n2){
    array[z] = R[y];
    y++;
    z++;
  }

}

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes sei = *((StartEndIndexes*)args);
    int error1, error2, half;
    if (sei.start < sei.end)
    {
      half = (sei.start + sei.end) / 2;
      StartEndIndexes sei1, sei2;
      sei1.start = sei.start;
      sei1.end = half;
      sei2.start = half + 1;
      sei2.end = sei.end;
      pthread_t left_tid, right_tid;
      error1 = pthread_create(&left_tid, NULL, mergeSort, &sei1);
      error2 = pthread_create(&right_tid, NULL, mergeSort, &sei2);

    if (error1 != 0){
      printf("Cannot sort left tree");
    }



    if (error2 != 0){
      printf("Cannot sort right tree.\n");
    }



    error1 = pthread_join(left_tid, NULL);

    error2 = pthread_join(right_tid, NULL);

    if (error1 != 0){
      printf("Cannot sort left tree.\n");
    }



    if (error2 != 0){
      printf("Cannot sort right tree.\n");
    }




    merge(sei.start, half, sei.end);

    }

    return NULL;


}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    int err;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a 2 threads for merge sort.
    pthread_t tid;
    err = pthread_create(&(tid), NULL, &mergeSort, &sei);
    if (err != 0){
      printf("Error creating thread.\n"); 
    }
    
    
    // 4. Wait for mergesort to finish.
    err = pthread_join(tid, NULL);
    
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}
