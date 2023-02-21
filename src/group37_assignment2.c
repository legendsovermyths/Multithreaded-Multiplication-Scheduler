#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

static int *flag1, *flag2;
int status;
struct thread_info
{
  int count;
  int min;
  int max;
  FILE *fp;
};

FILE *fptr;
FILE *fptr2;
FILE *fptr3;
FILE *fpt;
int nthreads = 1;
int threads_t = 64;
long long int *mat1;
long long int *mat2;
long long int mat3[1000 * 1000];
int dim1, dim2, dim3, dim4;
char *fname1, *fname2;
int *sr, *sc;
void *create_shared_memory(size_t size)
{
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}
void round_robin_scheduler(pid_t p1_pid, pid_t p2_pid)
{

  pid_t c1_pid, c2_pid;

  // (c1_pid = fork()) && (c2_pid = fork()); // Creates two children

  // if (c1_pid != 0 && c2_pid != 0)
  // {

  while (1)
  {

    // think of condintion to break the while loop
    if (*flag2 == 0)
    {
      // time stop
      // printf("%lu", waitingTime);
      kill(p2_pid, SIGCONT);
      // after 2ms pause child2
      usleep(1000);
      kill(p2_pid, SIGTSTP);
      // time start
    }
    if (*flag1 == 0)
    {
      kill(p1_pid, SIGCONT);
      // after 2ms pause child1 and resume child2
      usleep(1000); // sleep for 4 ms
      kill(p1_pid, SIGTSTP);
    }
    if (*flag1 == 1 && *flag2 == 1)
    {
      exit(0);
      break;
    }
  }
}
void *runner(void *arguments)
{
  // printf("runner called");
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

  for (int i = arg->min; i < arg->max; i++)
  {
    for (int j = 0; j < dim2; j++)
    {
      fscanf(arg->fp, "%lld", &mat1[dim2 * i + j]);
      // printf("Thread %d \n", arg->count);
    }
    sr[i] = 1;
  }
}
void *runner2(void *arguments)
{
  // printf("runner2 called");
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
  for (int i = arg->min; i < arg->max; i++)
  {
    for (int j = 0; j < dim4; j++)
    {
      fscanf(arg->fp, "%lld", &mat2[dim4 * i + j]);
      // printf("Thread %d \n", arg->count);
    }
    if (i == dim3 - 1)
      for (int k = 0; k < dim4; k++)
      {
        // printf("%d", k);
        sc[k] = 1;
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
    long long res = 0;
    while (sr[r] != 1 || sc[c] != 1)
      // printf("%d-%d", r, c);
      ;
    for (int i = 0; i < dim2; ++i)
    {
      long long int ele1 = mat1[r * dim2 + i], ele2 = mat2[i * dim4 + c];
      res += ele1 * ele2;
    }
    mat3[r * dim4 + c] = res;
  }
}

void P1()
{
  pthread_t threads[nthreads];
  pthread_t threads2[nthreads];
  for (int i = 0; i < dim1; i++)
    sr[i] = 0;
  for (int i = 0; i < dim4; i++)
    sc[i] = 0;
  struct thread_info s1[nthreads];
  struct thread_info s2[nthreads];

  for (int i = 0; i < nthreads; i++)
  {
    s1[i].count = i;
    s1[i].min = i * dim1 / nthreads;
    s1[i].max = (i + 1) * dim1 / nthreads;
    s1[i].fp = fopen(fname1, "r");

    s2[i].count = i;
    s2[i].min = i * dim3 / nthreads;
    s2[i].max = (i + 1) * dim3 / nthreads;
    s2[i].fp = fopen(fname2, "r");
  }

  struct timespec start, stop;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  for (int i = 0; i < nthreads; i++)
  {
    pthread_create(&threads2[i], NULL, runner2, &s2[i]);
  }
  for (int i = 0; i < nthreads; i++)
  {

    pthread_join(threads2[i], NULL);
  }
  for (int i = 0; i < nthreads; i++)
  {
    pthread_create(&threads[i], NULL, runner, &s1[i]);
  }
  for (int i = 0; i < nthreads; i++)
  {

    pthread_join(threads[i], NULL);
  }
  *flag1 = 1;
  shmdt(mat1);
  shmdt(mat2);
  shmdt(sc);
  shmdt(sr);
  exit(0);
}
void P2()
{
  pthread_t threads[threads_t];
  int threads_count = threads_t;

  for (int i = 0; i < threads_t; ++i)
  {

    pthread_create(&threads[i], NULL, multithreading, (void *)(uintptr_t)i);
  }

  for (int i = 0; i < threads_t; ++i)
  {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < dim1; i++)
  {
    for (int j = 0; j < dim4; j++)
    {
      fprintf(fptr3, "%lld ", mat3[i * dim4 + j]);
    }
    fprintf(fptr3, "\n");
  }
  *flag2 = 1;
  shmdt(mat1);
  shmdt(mat2);
  shmdt(sc);
  shmdt(sr);
  exit(0);
}
void EXEC()
{
  // printf("I am Called\n");
  pid_t c1_pid,
      c2_pid;
  (c1_pid = fork()) && (c2_pid = fork());
  if (c1_pid == 0)
  {
    P1();
  }
  else if (c2_pid == 0)
  {
    P2();
  }
  else
  {
    kill(c1_pid, SIGTSTP);
    kill(c2_pid, SIGTSTP);
    round_robin_scheduler(c1_pid, c2_pid);

    wait(&status);
    wait(&status);
    exit(0);
  }
}
int main(int argc, char *argv[])
{
  flag1 = mmap(NULL, sizeof *flag1, PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *flag1 = 0;
  flag2 = mmap(NULL, sizeof *flag2, PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *flag2 = 0;
  mat1 = create_shared_memory(8000000);
  mat2 = create_shared_memory(8000000);
  sr = create_shared_memory(4000);
  sc = create_shared_memory(4000);
  fname1 = argv[4];
  fname2 = argv[5];
  fptr = fopen(argv[4], "r");
  fptr2 = fopen(argv[5], "r");
  fptr3 = fopen(argv[6], "w");
  dim1 = atoi(argv[1]);
  dim2 = atoi(argv[2]);
  dim3 = atoi(argv[2]);
  dim4 = atoi(argv[3]);
  EXEC();
  shmdt(mat1);
  shmdt(mat2);
  shmdt(sc);
  shmdt(sr);
  // }
}