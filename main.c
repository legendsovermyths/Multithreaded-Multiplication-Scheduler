#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdio.h>
#include<pthread.h>
#include <stdint.h>

FILE *fptr;
FILE *fptr2;
int sz=3;
int mat1[1000*1000];
int mat2[1000*1000];
int mat3[1000*1000];
void *runner(void* arg)
{
    int j=atoi(arg);
     for(int i=0;i<9;i++){
        fscanf(fptr,"%d",&mat1[j*3+i]);
        // printf("%d ",mat1[j*3+i]);
     }
}
void *runner2(void* arg)
{
    int j=atoi(arg);
     for(int i=0;i<9;i++){
        fscanf(fptr2,"%d",&mat2[j*3+i]);
        // printf("%d ",mat2[j*3+i]);
     }
}
void *multiply_threading(void *args) {
  int thread_number = (uintptr_t)args;
  const int n_elements = (sz*sz);
  const int n_operations = n_elements / 4;
  const int rest_operations = n_elements % 4;

  int start_op, end_op;

  if (thread_number == 0) {
    // First thread does more job
    start_op = n_operations * thread_number;
    end_op = (n_operations * (thread_number + 1)) + rest_operations;
  }
  else {
    start_op = n_operations * thread_number + rest_operations;
    end_op = (n_operations * (thread_number + 1)) + rest_operations;
  }

  for (int op = start_op; op < end_op; ++op) {
    const int row = op % sz;
    const int col = op / sz;
    int r = 0;
    for (int i = 0; i < sz; ++i) {
      const float e1 = mat1[row*sz+i];
      const float e2 = mat2[i*sz+col];
      r += e1 * e2;
    }

    mat3[row*sz+col] = r;
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
        pthread_create(&threads[i],NULL,runner,(void*)&i);
        pthread_join(threads[i],NULL);
    }
    for(int i=0;i<1;i++){
        pthread_create(&threads[i],NULL,runner2,(void*)&i);
        pthread_join(threads[i],NULL);
    }
   
    
}
void P2(){
    pthread_t threads[4];
    int threads_count=4;
  for (int i = 0; i < 4; ++i) {
    pthread_create(&threads[i],NULL,multiply_threading,(void*)(uintptr_t)i);
  }
  for (int i = 0; i < 4; ++i) {
    pthread_join(threads[i],NULL);
  }
  for(int i=0;i<sz;i++)
  {
    for(int j=0;j<sz;j++){
        printf("%d ",mat3[i*sz+j]);
    }
    printf("\n");
  }
}
int main(){
    P1();
    P2();
}