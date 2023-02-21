#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
int nthreads = 1;
struct thread_info
{
    int count;
    int min;
    int max;
    FILE *fp;
};

FILE *fptr;
FILE *fptr2;
FILE *fpt;
int threads_t = 16;
int mat1[1000 * 1000];
int mat2[1000 * 1000];
int mat3[1000 * 1000];
int dim1, dim2, dim3, dim4;

uint64_t tot = 0;
void *runner(void *arguments)
{
    struct thread_info *arg = (struct thread_info *)arguments;

    if (arg->count != 0)
    {
        char holder;
        int line = 0;
        while ((holder = fgetc(arg->fp)) != EOF)
        {
            if (holder == '\n')
                line++;
            if (line == arg->min)
                break; /* 1 because count start from 0,you know */
        }
    }
    struct timespec start_time, stop_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    for (int i = arg->min; i < arg->max; i++)
    {
        for (int j = 0; j < dim2; j++)
        {
            fscanf(arg->fp, "%d", &mat1[dim2 * i + j]);
            // printf("Thread %d \n", arg->count);
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop_time);
    uint64_t nsecs = (stop_time.tv_sec - start_time.tv_sec) * 1000000000 + (stop_time.tv_nsec - start_time.tv_nsec);
    tot += nsecs;
}
void *runner2(void *arguments)
{
    struct thread_info *arg = (struct thread_info *)arguments;
    if (arg->count != 0)
    {
        char holder;
        int line = 0;
        while ((holder = fgetc(arg->fp)) != EOF)
        {
            if (holder == '\n')
                line++;
            if (line == arg->min)
                break; /* 1 because count start from 0,you know */
        }
    }
    struct timespec start_time, stop_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    for (int i = arg->min; i < arg->max; i++)
    {
        for (int j = 0; j < dim4; j++)
        {
            fscanf(arg->fp, "%d", &mat2[dim4 * i + j]);
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop_time);
    uint64_t nsecs = (stop_time.tv_sec - start_time.tv_sec) * 1000000000 + (stop_time.tv_nsec - start_time.tv_nsec);
    tot += nsecs;
}

void P1()
{
    pthread_t threads[nthreads];
    pthread_t threads2[nthreads];
    struct thread_info s1[nthreads];
    struct thread_info s2[nthreads];

    for (int i = 0; i < nthreads; i++)
    {
        s1[i].count = i;
        s1[i].min = i * dim1 / nthreads;
        s1[i].max = (i + 1) * dim1 / nthreads;
        s1[i].fp = fopen("matrix1.txt", "r");

        s2[i].count = i;
        s2[i].min = i * dim3 / nthreads;
        s2[i].max = (i + 1) * dim3 / nthreads;
        s2[i].fp = fopen("matrix2.txt", "r");
    }
    for (int i = 0; i < nthreads; i++)
    {
        pthread_create(&threads[i], NULL, runner, &s1[i]);
    }
    for (int i = 0; i < nthreads; i++)
    {

        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < nthreads; i++)
    {
        pthread_create(&threads2[i], NULL, runner2, &s2[i]);
    }
    for (int i = 0; i < nthreads; i++)
    {

        pthread_join(threads2[i], NULL);
    }

    printf("time taken for reading matrices: %lu\n", tot);
}

int main(int argc, char *argv[])
{
    fpt = fopen("reading.csv", "a+");
    fptr = fopen("matrix1.txt", "r");
    fptr2 = fopen("matrix2.txt", "r");
    dim1 = atoi(argv[1]);
    dim2 = atoi(argv[1]);
    dim3 = atoi(argv[1]);
    dim4 = atoi(argv[1]);
    int n = atoi(argv[2]);
    for (size_t i = 1; i < n; i++)
    {
        nthreads = nthreads * 4;
    }
    P1();
    fprintf(fpt, "%d,%lu,%d\n", dim1, tot, nthreads);
}