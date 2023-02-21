#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>

FILE *fptr;
FILE *fptr2;
FILE *fpt;
int threads_t = 1;
int mat1[1000 * 1000];
int mat2[1000 * 1000];
int mat3[1000 * 1000];
int dim1, dim2, dim3, dim4;
void *runner(void *arg)
{
    for (int i = 0; i < dim1; i++)
    {
        for (int j = 0; j < dim2; j++)
        {
            fscanf(fptr, "%d", &mat1[dim2 * i + j]);
        }
    }
}
void *runner2(void *arg)
{
    for (int i = 0; i < dim3; i++)
    {
        for (int j = 0; j < dim4; j++)
        {
            fscanf(fptr2, "%d", &mat2[dim4 * i + j]);
        }
    }
}
void *multithreading(void *args)
{
    int thread_number = (uintptr_t)args;
    int totalElements = (dim1 * dim4);
    int threadOperation = totalElements / threads_t, restOperations = totalElements % threads_t;
    int st, ed;
    if (thread_number == 0)
        st = threadOperation * thread_number, ed = (threadOperation * (thread_number + 1)) + restOperations;
    else
        st = threadOperation * thread_number + restOperations, ed = (threadOperation * (thread_number + 1)) + restOperations;
    for (int k = st; k < ed; k++)
    {
        int r = k / dim4, c = k % dim4;
        int res = 0;
        for (int i = 0; i < dim2; ++i)
        {
            int ele1 = mat1[r * dim2 + i], ele2 = mat2[i * dim4 + c];
            res += ele1 * ele2;
        }
        mat3[r * dim4 + c] = res;
    }
}

void P1()
{
    pthread_t threads[1];
    for (int i = 0; i < 1; i++)
    {
        pthread_create(&threads[i], NULL, runner, NULL);
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < 1; i++)
    {
        pthread_create(&threads[i], NULL, runner2, NULL);
        pthread_join(threads[i], NULL);
    }
}
void P2()
{
    pthread_t threads[threads_t];
    int threads_count = threads_t;

    struct timespec start_time, stop_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    for (int i = 0; i < threads_t; ++i)
    {

        pthread_create(&threads[i], NULL, multithreading, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < threads_t; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop_time);
    uint64_t nsecs = (stop_time.tv_sec - start_time.tv_sec) * 1000000000 + (stop_time.tv_nsec - start_time.tv_nsec);
    printf("time taken: %lu\n", nsecs);
    fprintf(fpt, "%d, %lu, %d\n", dim1, nsecs, threads_t);
    // for (int i = 0; i < dim1; i++)
    // {
    //     for (int j = 0; j < dim4; j++)
    //     {
    //         printf("%d ", mat3[i * dim4 + j]);
    //     }
    //     printf("\n");
    // }
}
int main(int argc, char *argv[])
{
    // fprintf(fpt, "Size, Time\n");
    dim1 = atoi(argv[1]);
    dim2 = atoi(argv[1]);
    dim3 = atoi(argv[1]);
    dim4 = atoi(argv[1]);
    P1();

    P2();
}