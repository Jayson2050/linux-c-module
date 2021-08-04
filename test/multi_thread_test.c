/* mutex.c */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

/* 全局变量 */
int gnum = 0;
/* 互斥量 */
pthread_mutex_t mutex;

/* 声明线程运行服务程序. */
static void pthread_func_1(void);
static void pthread_func_2(void);

/*线程1的服务程序*/
static void pthread_func_1(void)
{
	int i = 0;
	int ret =0;
	ret = pthread_mutex_lock(&mutex); /* 获取互斥锁 */
	if(ret == 0)
	{
		printf("pthread_1 get mutex_lock!\r\n");
	}
	
		/* 注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用. */
	sleep(1);
	
	for (i=0; i<3; i++)
	{
		/* 注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用. */
		sleep (1);
		/*临界资源*/
		gnum++;
		printf ("Thread_1 add one to num:%d\r\n", gnum);
	}
	ret = pthread_mutex_unlock(&mutex); /* 释放互斥锁,否则会造成死锁 */
	if(ret == 0)
	{
		printf("pthread_1 release mutex_lock!\r\n");
	}
	// pthread_exit(NULL);
}

/*线程2的服务程序*/
static void pthread_func_2(void)
{
	int i = 0;
	int ret = 0;
	
	ret = pthread_mutex_lock(&mutex); /* 获取互斥锁 */
	if(ret == 0)
	{
		printf("pthread_2 get mutex_lock!\r\n");
	}
	else
	{
		printf("pthread_2 can't get the mutex_lock!\r\n");
	}
	/* 注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用. */
	sleep(1);
	
	for (i=0; i<5; i++)  
	{
		/* 临界资源. */
		gnum++;
		printf ("Thread_2 add one to num:%d\r\n",gnum);
	}
	ret = pthread_mutex_unlock(&mutex); /* 释放互斥锁. */
	if(ret == 0)
	{
		printf("pthread_2 release mutex_lock!\r\n");
	}
	// pthread_exit (NULL);
}

int main (void)
{
 /*线程的标识符*/
  pthread_t pt_1 = 0;
  pthread_t pt_2 = 0;
  int ret = 0;

  /* 互斥锁初始化. */
  ret = pthread_mutex_init(&mutex, NULL);
  if(ret != 0)
  {
	  printf("mutex init error!,ret=%d\r\n", ret);
  }
  else
  {
	  printf("Mutex init successfully!\r\n");
  }
  /*分别创建线程1、2*/
  ret = pthread_create(&pt_1,  //线程标识符指针
                       NULL,  //默认属性
                       (void*)pthread_func_1, //运行函数
                       NULL); //无参数
  if (ret == 0)
  {
     printf("pthread_1 create successfully!\r\n");
  }

  ret = pthread_create(&pt_2, //线程标识符指针
                       NULL,  //默认属性
                       (void *)pthread_func_2, //运行函数
                       NULL); //无参数
  if (ret == 0)
  {
     printf ("pthread_2 create successfully!\r\n");
  }
  /*等待线程1、2的结束*/
   pthread_join(pt_1, NULL);
   pthread_join(pt_2, NULL);

  printf ("main programme exit!\r\n");
  return 0;
}