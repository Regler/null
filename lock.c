#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Data
{
    int sum;
    pthread_mutex_t mlock;

}data;

void* fn_0(void* arg)
{
    data* da1 = (data*)arg;
    for (int i = 0; i<10; i++)
    {
        pthread_mutex_lock(&da1->mlock);
        da1->sum += 1;
        printf("\nwrite data:%d", da1->sum);
        pthread_mutex_unlock(&da1->mlock);
    }
    return (void*)da1;

}
void* fn_1(void* arg)
{
    data* da2 = (data*)arg;
    for (int i = 0; i<10; i++)
    {
        pthread_mutex_lock(&da2->mlock);
        printf("\nread data:%d", da2->sum);
        pthread_mutex_unlock(&da2->mlock);
    }
    return (void*)da2;

}

int main()
{
    pthread_t thread0;
    pthread_t thread1;
    data da;
    da.sum = 0;
    pthread_mutex_init(&da.mlock, NULL);
    pthread_create(&thread0, NULL, fn_0, (void*)&da);
    pthread_create(&thread1, NULL, fn_1, (void*)&da);
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    pthread_mutex_destroy(&da.mlock);
    return 0;
}
