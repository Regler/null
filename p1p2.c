/*************************************************************************
	> File Name: p1p2.c
	> Author:lluo 
	> Mail:1263826590@qq.com 
	> Created Time: 2019年09月25日 星期三 16时20分12秒
 ************************************************************************/

#include<stdio.h>
//file name: pthreadtext.c

#include <stdio.h>
#include <pthread.h> //线程头文件  
//pthread不是linux下的默认的库，也就是在链接的时候，无法找到phread库中哥函数的入口地址，于是链接会失败 
//在gcc编译的时候，附加要加 -lpthread参数即可解决。gcc -o run pthreadtext.c  -lpthread

void *myThread1(void) //线程函数
{
        int i;
        for(i=0;i<5; i++)
    {
                printf("This is the 1st pthread \n");
                sleep(1);
            
    }    

}

void *myThread2(void)
{
        int i;
        for(i=0;i<5;i++)
    {
                printf("this is the 2st \n");
                sleep(1);
            
    }

}

int main()
{
        int i=0,ret=0;
        pthread_t id1,id2;
        ret= pthread_create(&id1,NULL,(void*)myThread1,NULL ); //创建线程
        if(ret)
    {
                printf("create error\n");
                return 1;
            
    }
        ret = pthread_create(&id2,NULL,(void*)myThread2,NULL); //创建线程
        if(ret)
    {
                printf("create error\n");
                return 1;
            
    }
        
        pthread_join(id1,NULL); //当前线程会处于阻塞状态，直到被调用的线程结束后，当前线程才会重新开始执行
        pthread_join(id2,NULL);
        return 0;

}
