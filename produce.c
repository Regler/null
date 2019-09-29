#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#define BUFFER_SIZE 16
/*Linux是一种“多进程单线程”的操作系统。Linux本身 只有进程的概念，
* 而其所谓的“线程”本质上在内核里仍然是进程。
* 同一进程中的多个线程共享该进程的资源（如作为共享内   存的全局变量）。
* Linux中所谓的“线程”只是在被创建时clone了父进程的  资源，因此clone出来的进程 表现为“线程”。*/
int i=1;
struct prodcons
{
    pthread_mutex_t lock;
    pthread_cond_t  p;
    pthread_cond_t  c;

};
/*线程互斥 
* 该机制的作用是对某个需要互斥的部分，在进入时先得到   互斥体，如果没有得到互斥体，表明互斥部分被其它线程   拥有，此时欲获取互斥体的线程阻塞，直到拥有该互斥体   的线程完成互斥部分的操作为止
* 线程同步 
* 同步就是线程等待某个事件的发生。只有当等待的事件发   生线程才继续执行，否则线程挂起并放弃处理器。 */
typedef struct Buffer
{
    int b[BUFFER_SIZE];
    int readpos,writepos;

}Buffer;
Buffer buffer;
struct prodcons behavior;

  void init_buffer(Buffer *b)
  {
      b->readpos=0;
      b->writepos=0;

  }
  void init_behavior(struct prodcons *b)
  {
      pthread_mutex_init(&b->lock,NULL);
      //按缺省的属性初始化互斥体变量
      pthread_cond_init(&b->p,NULL);
      //按缺省的属性初始化条件变量
      pthread_cond_init(&b->c,NULL);

  }
  int put(int data)
  {
      int j;
      if(buffer.writepos>=BUFFER_SIZE)
      {   
          printf("Buffer is full!\n");
          printf("in buffer:");
          for(j=0;j<BUFFER_SIZE;j++)
          printf(" %d ",buffer.b[j]);
          printf("\n");   
          return 0;
      }
      buffer.b[buffer.writepos]=data;
      buffer.writepos++;
      return 1;

  }
  int get(  )
  {
      int data;
      int j;
      data=buffer.b[buffer.readpos];
      buffer.b[buffer.readpos]=0;
      buffer.readpos++;
      buffer.writepos--;  
      return data;

  }
  void *producer(void *data)
  {
      while(1)
      {
          pthread_mutex_lock(&behavior.lock);//给互斥体变量加锁 
          if(i%2!=0)
          {
              int n;
              n=rand()%100;
              if(put(n))
              {
                  printf("Producer put %d.\n",n);
                  i++;
                  pthread_cond_signal(&behavior.c);//释放信号

              }
              else
              pthread_exit(0);
              //线程本身调用：pthread_exit (void * retval) 退出。

          }
          else
          {
              pthread_cond_wait(&behavior.p,&behavior.lock);
              /*等待的事件p发生线程才继续执行，否则线程挂起并放  弃处理器。
              * 这个等待调用需要一个已经上锁的互斥体lock，这是为了 防止在真正进入等待状态之前别的线程有可能设置该条    件变量而产生竞争*/

          }       
          pthread_mutex_unlock(&behavior.lock);

      }
      return NULL;

  }

  void *consumer(void *data)
  {
      while(1)
      {
          pthread_mutex_lock(&behavior.lock);
          if(i%2==0)
          {
              int d,s;
              d=get();
              printf("Consumer get %d.\n",d);
              if((d%2==1))
              {
                  if(put(d))          
                  printf("Consumer put %d back.\n\n",d);
                  else
                  pthread_exit(0);

              }
              else
              {
                  while((d%2==0))
                  {
                      s=d/2;
                      d=s;

                  }   
                  if(put(d))
                  printf("Consumer put %d back.\n\n",d);
                  else
                  pthread_exit(0);

              }
              pthread_cond_signal(&behavior.p);
              i++;

          }
          else
          {
              pthread_cond_wait(&behavior.c,&behavior.lock);

          }
          pthread_mutex_unlock(&behavior.lock);

      }
      return NULL;

  }
  int main()
  {
      pthread_t th_a,th_b;
      void *retval;
      srand(time(0));
      init_buffer(&buffer);
      init_behavior(&behavior);
      pthread_create(&th_a,NULL,producer,0);
      pthread_create(&th_b,NULL,consumer,0);
      /*线程的创建
      * Pthread_create(pthread_t *thread, const pthread_attr_t  *attr, void * (start_routine)(void*), void *arg);第3个参数为新线程的入口函数，第4个参数为传递    给start_routine的参数。 */
      pthread_join(th_a,&retval);//一个线程等待另外一个线程结束
      return 0;

  }
