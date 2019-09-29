#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
int sum;
sem_t sem1;
sem_t sem2;
void* fn_0(void* arg)
{
    int* sum = (int*)arg;
    for (int i = 0; i<10; i++)
    {
        *sum += 1;
        printf("write data:%d\n", *sum);
        sem_post(&sem2);
        sem_wait(&sem1);
    }
    return (void*)arg;
}
void* fn_1(void* arg)
{
    int* sum = (int*)arg;
    for (int i = 0; i<10; i++)
    {
        sem_wait(&sem2);
        printf("read data:%d\n", *sum);
        sem_post(&sem1);
    }
    return (void*)arg;
}
int main()
{
    sum=0;
    pthread_t thread0;
    pthread_t thread1;
    sem_init(&sem1,0,0);
    sem_init(&sem2,0,0);
    pthread_create(&thread0, NULL, fn_0, (void*)&sum);
    pthread_create(&thread1, NULL, fn_1, (void*)&sum);
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    return 0;
}
