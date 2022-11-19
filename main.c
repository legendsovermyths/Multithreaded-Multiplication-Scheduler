#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdio.h>
#include<pthread.h>
#include <stdint.h>

FILE *fptr;
FILE *fptr2;
int threads_t=9;
int mat1[1000*1000];
int mat2[1000*1000];
int mat3[1000*1000];
int dim1=4,dim2=6;
int dim3=6,dim4=5;
void *runner(void* arg)
{
     for(int i=0;i<dim1;i++){
        for(int j=0;j<dim2;j++){
         fscanf(fptr,"%d",&mat1[dim2*i+j]);
      }
     }
}
void *runner2(void* arg)
{
     for(int i=0;i<dim3;i++){
      for(int j=0;j<dim4;j++){
         fscanf(fptr2,"%d",&mat2[dim4*i+j]);
      }
       
     }
}
void *multithreading(void *args) {
  int thread_number = (uintptr_t)args;
  int totalElements = (dim1*dim4);
  int threadOperation = totalElements / threads_t, restOperations = totalElements % threads_t;
  int st, ed;
  if (thread_number == 0) st = threadOperation * thread_number,ed = (threadOperation * (thread_number + 1)) + restOperations;
  else st = threadOperation * thread_number + restOperations,ed = (threadOperation * (thread_number + 1)) + restOperations;
  printf("--%d %d",st,ed);
  for (int k = st; k < ed; k++) {
    int r = k / dim4, c = k % dim4;
    int res = 0;
    for (int i = 0; i < dim2; ++i) {
      int ele1 = mat1[r*dim2+i],ele2 = mat2[i*dim4+c];
      res += ele1 * ele2;
    }
    mat3[r*dim4+c] = res;
  }
}

void P1(){
     pthread_t threads[1];
    key_t spkey=ftok("osproject",65);
    fptr = fopen("in1.txt","r");
    fptr2 = fopen("in2.txt","r");
    int shid=shmget(spkey,sizeof(mat1),0666|IPC_CREAT);
    *mat1=*(int*)shmat(shid,NULL,0);
    for(int i=0;i<1;i++){
        pthread_create(&threads[i],NULL,runner,NULL);
        pthread_join(threads[i],NULL);
    }
    for(int i=0;i<1;i++){
        pthread_create(&threads[i],NULL,runner2,NULL);
        pthread_join(threads[i],NULL);
    }
   
    
}
void P2(){
    pthread_t threads[threads_t];
    int threads_count=threads_t;
  for (int i = 0; i < threads_t; ++i) {
    
    pthread_create(&threads[i],NULL,multithreading,(void*)(uintptr_t)i);
     pthread_join(threads[i],NULL);
  }
  for (int i = 0; i < threads_t; ++i) {
   
  }
  for(int i=0;i<dim1;i++)
  {
    for(int j=0;j<dim2;j++){
        printf("%d ",mat1[i*dim2+j]);
    }
    printf("\n");
  }
  for(int i=0;i<dim3;i++)
  {
    for(int j=0;j<dim4;j++){
        printf("%d ",mat2[i*dim4+j]);
    }
    printf("\n");
  }
  for(int i=0;i<dim1;i++)
  {
    for(int j=0;j<dim4;j++){
        printf("%d ",mat3[i*dim4+j]);
    }
    printf("\n");
  }
}
int main(){
    P1();
    P2();
}