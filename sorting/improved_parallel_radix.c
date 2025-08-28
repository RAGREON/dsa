#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define MAX_THREADS 16

typedef struct {
  int   thread_id;
  int*  input;
  int*  output;
  int   n; 
  int   start;
  int   end;
  int   shift;
  int** counts;
  int** offsets; 
} ThreadData;

static inline int get_max(const int* array, int count) {
  int max = array[0];
  
  for (int i = 1; i < count; i++) {
    if (max < array[i])
      max = array[i];
  }

  return max;
}

static void* count_phase(void* arg) {
  ThreadData* data = (ThreadData*)arg;

  int* histogram = data->counts[data->thread_id];
  const int* input = data->input;
  int shift = data->shift;

  for (int i = data->start; i < data->end; i++) {
    unsigned digit = ((unsigned)input[i] >> shift) & 0xFFu;
    histogram[digit]++;
  }

  return NULL;
}

static void* scatter_phase(void* arg) {
  ThreadData* data = (ThreadData*)arg;

  int* output = data->output;
  const int* input = data->input;
  int shift = data->shift;

  int next[256];
  memcpy(next, data->offsets[data->thread_id], 256 * sizeof(int));

  for (int i = data->start; i < data->end; i++) {
    unsigned digit = ((unsigned)input[i] >> shift) & 0xFFu;
    int position = next[digit]++;
    output[position] = input[i];
  }

  return NULL;
}

static void radix_sort_parallel_base256(int* array, int size) {
  if (size <= 1) return;

  int* buffer = (int*)malloc(sizeof(int) * size);
  if (!buffer) {
    perror("malloc");
    exit(1);
  }

  int** counts = (int**)malloc(sizeof(int*) * MAX_THREADS);
  int** offsets = (int**)malloc(sizeof(int*) * MAX_THREADS);
  if (!counts || !offsets) {
    perror("malloc");
    exit(1);
  }
  for (int i = 0; i < MAX_THREADS; i++) {
    counts[i] = (int*)calloc(256, sizeof(int));
    offsets[i] = (int*)calloc(256, sizeof(int));
  }

  pthread_t threads[MAX_THREADS];
  ThreadData thread_data[MAX_THREADS];

  int chunk = (size + MAX_THREADS - 1) / MAX_THREADS;

  int* input = array;
  int* output = buffer;

  int max = get_max(array, size);
  int passes = 0;
  while (((unsigned)max >> (passes * 8)) && passes < 4) passes++;

  for (int i = 0; i < passes; i++) {
    int shift = i * 8;

    for (int t = 0; t < MAX_THREADS; t++) 
      memset(counts[t], 0, 256 * sizeof(int));

    for (int t = 0; t < MAX_THREADS; t++) {
      thread_data[t].thread_id = t;
      thread_data[t].input = input;
      thread_data[t].output = output;
      thread_data[t].n = size;
      thread_data[t].start = chunk * t;
      thread_data[t].end = thread_data[t].start + chunk;

      if (thread_data[t].start > size) thread_data[t].start = size;
      if (thread_data[t].end > size) thread_data[t].end = size;

      thread_data[t].shift = shift;
      thread_data[t].counts = counts;
      thread_data[t].offsets = offsets;

      pthread_create(&threads[t], NULL, count_phase, &thread_data[t]);
    }
    
    for (int i = 0; i < MAX_THREADS; i++) 
      pthread_join(threads[i], NULL);

    int global[256];
    for (int i = 0; i < 256; i++) {
      int sum = 0;
      for (int t = 0; t < MAX_THREADS; t++)
        sum += counts[t][i];
      global[i] = sum;
    }

    int prefix[256];
    int run = 0;
    for (int i = 0; i < 256; i++) {
      prefix[i] = run;
      run += global[i];
    }

    for (int i = 0; i < 256; i++) {
      int accum = prefix[i];
      for (int t = 0; t < MAX_THREADS; t++) {
        offsets[t][i] = accum;
        accum += counts[t][i];
      }
    }

    for (int i = 0; i < MAX_THREADS; i++) 
      pthread_create(&threads[i], NULL, scatter_phase, &thread_data[i]);

    for (int i = 0; i < MAX_THREADS; i++) 
      pthread_join(threads[i], NULL);

    int* temp = input;
    input = output;
    output = temp;
  }

  if (input != array) {
    memcpy(array, input, size * sizeof(int));
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    free(counts[i]);
    free(offsets[i]);
  }

  free(counts);
  free(offsets);
  free(buffer);
}

int* generate_array(int n) {
  int* array = (int*)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    int num = rand() % 100000 + 1;    // random 1-10
    array[i] = num;
  }
  return array;
}

void start_query(LARGE_INTEGER* freq, LARGE_INTEGER* start) {
  QueryPerformanceFrequency(freq);
  QueryPerformanceCounter(start);
}

double elapsed_time(LARGE_INTEGER* freq, LARGE_INTEGER* start, LARGE_INTEGER* end) {
  QueryPerformanceCounter(end);
  return (double)(end->QuadPart - start->QuadPart) / freq->QuadPart;
}


int main(int argc, char** argv) {
  srand(time(NULL));
  LARGE_INTEGER start, end, freq;

  start_query(&freq, &start);
  int n = atoi(argv[1]);
  int* arr = generate_array(n);
  printf("\n\ngenerator: %.10f\n", elapsed_time(&freq, &start, &end));

  // printf("Original array: ");
  // for(int i=0;i<n;i++) printf("%d ", arr[i]);
  // printf("\n");

  start_query(&freq, &start);
  radix_sort_parallel_base256(arr, n);
  double time_taken = elapsed_time(&freq, &start, &end);

  // printf("Sorted array:   ");
  // for(int i=0;i<n;i++) printf("%d ", arr[i]);
  // printf("\n");

  printf("\nsorting:\t%.10fs\n", time_taken);

  return 0;
}