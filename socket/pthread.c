#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#include <pthread.h>
#include <unistd.h>

typedef unsigned char      uint8;   /* 8-bit quantity  */
typedef unsigned short     uint16;  /* 16-bit quantity */
typedef unsigned int       uint32;  /* 32-bit quantity */

typedef signed char        int8;    /* 8-bit quantity  */
typedef signed short       int16;   /* 16-bit quantity */
typedef signed int         int32;   /* 32-bit quantity */

#define THREAD_SLEEP 1
#define THREAD_WAIT  5
pthread_mutex_t mutex;

void cleanup_release_mutex(void *arg)
{
    /* 解锁 */ 
    pthread_mutex_unlock(&mutex);
    printf("线程 %s cleanup清理函数执行成功！\r\n", (char *)arg); 
}

/* 
pthreads标准指定了几个取消点，其中包括：
    (1)通过pthread_testcancel调用以编程方式建立线程取消点。 
    (2)线程等待pthread_cond_wait或pthread_cond_timewait()中的特定条件。 
    (3)被sigwait(2)阻塞的函数 
    (4)一些标准的库调用。通常，这些调用包括线程可基于阻塞的函数，如printf函数。
 */

/* 该线程中有死循环，线程可以取消，线程取消类型为延迟取消，且线程有取消点 */
void* thread0_with_cancel_point(void* arg)
{
    /* 本线程取消状态使能，如果不使能无法被取消 */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    /* 当该线程收到pthread_cancel()函数提出的取消请求时，该线程不会立刻取消，而是会在下一个取消点进行退出 */
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); 

    printf("本线程0暂时无法被取消，直到该线程0状态被设置回默认可取消状态\r\n");
    while(1)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        sleep(THREAD_SLEEP);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        
        /* 设置取消点，当收到pthread_cancel()请求后会在此处退出线程 */
        pthread_testcancel();
    }

}

/* 该线程中有死循环，线程可以取消，线程取消类型为延迟取消，且线程没有取消点 */
void* thread1_without_cannel_point(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    printf("本线程1无取消点，pthread_cancel()对我无效! \r\n");
    while(1)
    {
        // printf("本线程1无取消点，pthread_cancel()对我无效! \r\n");
        /* printf函数为系统调用函数，会引起阻塞，也是一个取消点 */
        sleep(THREAD_SLEEP);
    }
}

/* 该线程中有死循环，线程可以取消，且线程取消类型为异步取消(立刻取消) */
void* thread2_without_cannel_point(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    printf("本线程2可以异步取消，不需要线程取消点，pthread_cancel()对我有效! \r\n");

    while(1)
    {
        sleep(THREAD_SLEEP);
    }
}

/* 该线程中有死循环，线程不可以取消，且线程取消类型为异步取消(立刻取消) */
void* thread3_without_cannel_point(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    printf("本线程3可以异步取消，不需要线程取消点，pthread_cancel()对我有效! \r\n");

    while(1)
    {
        sleep(THREAD_SLEEP);
    }
}

/* 该线程中有死循环，线程可以取消，且线程取消类型为延时取消，数据处理带互斥量，无死锁问题*/
void* thread4_without_dead_mutex_lock(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); 

    /* 设置线程退出时执行相关资源回收 */
    pthread_cleanup_push(cleanup_release_mutex,"4");

    printf("线程4已获得互斥锁，此处运行的程序无法被取消，直到运行结束并释放互斥锁 \r\n");
    while(1)
    {
        /* 加锁 */
        pthread_mutex_lock(&mutex);  
        
        sleep(THREAD_SLEEP);

        /* 解锁 */
        pthread_mutex_unlock(&mutex); 

        /* 设置取消点，位于解锁之后，当收到pthread_cancel()请求后会在此处退出线程，不会产生死锁问题 */
        pthread_testcancel();
    }
    /* 与pthread_cleanup_push()成对出现 */
    pthread_cleanup_pop(0);
}

/* 该线程中有死循环，线程可以取消，且线程取消类型为延时取消，数据处理带互斥量，有死锁问题*/
void* thread5_with_dead_mutex_lock(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); 
    printf("线程5已获得互斥锁，此处运行的程序无法被取消，线程取消后互斥锁无法被释放 \r\n");
    while(1)
    {
        /* 加锁 */
        pthread_mutex_lock(&mutex);  
        sleep(THREAD_SLEEP);

        /* 设置取消点，位于解锁之前，当收到pthread_cancel()请求后会在此处退出线程，会产生死锁问题 */
        pthread_testcancel();

        pthread_mutex_unlock(&mutex); /* 解锁 */
        // printf("线程5已释放互斥锁!\r\n");
    }
}


/* 该线程中有死循环，线程可以取消，且线程取消类型为异步取消(立刻取消)，有线程清理函数，不会产生死锁问题 */
void* thread6_without_dead_mutex_lock(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    /* 设置线程退出时执行相关资源回收 */
    pthread_cleanup_push(cleanup_release_mutex, "6");

    printf("线程6已获得互斥锁，本线程可以异步取消，不需要线程取消点，pthread_cancel()对我有效! \r\n");
    while(1)
    {
        /* 加锁 */
        pthread_mutex_lock(&mutex); 
        sleep(THREAD_SLEEP);

        pthread_mutex_unlock(&mutex); /* 解锁 */
        // printf("线程6已释放互斥锁!\r\n");
    }
    pthread_cleanup_pop(0);
}


/* 该线程中有死循环，线程可以取消，且线程取消类型为异步取消(立刻取消)，没有线程清理函数，会产生死锁问题 */
void* thread7_with_dead_mutex_lock(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    printf("本线程7可以异步取消，不需要线程取消点，pthread_cancel()对我有效! \r\n");
    while(1)
    {
        /* 加锁 */
        pthread_mutex_lock(&mutex);
        sleep(THREAD_SLEEP);

        pthread_mutex_unlock(&mutex); /* 解锁 */
        // printf("线程7已释放互斥锁!\r\n");
    }
}

#define THREAD_NUM 8
pthread_t tid[THREAD_NUM];
/*=================================各种应用场景测试============================*/


void test0(void)
{
    /* 创建线程，后创建的线程会先被系统调度 */
    pthread_create(&tid[0], NULL, thread0_with_cancel_point, NULL);
    pthread_create(&tid[1], NULL, thread1_without_cannel_point, NULL);
    pthread_create(&tid[2], NULL, thread2_without_cannel_point, NULL);
    pthread_create(&tid[3], NULL, thread3_without_cannel_point, NULL);
    pthread_create(&tid[4], NULL, thread4_without_dead_mutex_lock, NULL);
    pthread_create(&tid[5], NULL, thread5_with_dead_mutex_lock, NULL);
    pthread_create(&tid[6], NULL, thread6_without_dead_mutex_lock, NULL);
    pthread_create(&tid[7], NULL, thread7_with_dead_mutex_lock, NULL);

    /* 主线程延时时间，目的是让子线程都有机会被执行 */
    sleep(20);

    /* 取消某个线程 */
    printf("即将发起线程取消请求！\r\n");
    pthread_cancel(tid[0]);
    pthread_cancel(tid[1]);
    pthread_cancel(tid[2]);
    pthread_cancel(tid[3]);
    pthread_cancel(tid[4]);
    pthread_cancel(tid[5]);
    pthread_cancel(tid[6]);
    pthread_cancel(tid[7]);
    
    /* 等待指定线程结束 */
    printf("等待线程结束！\r\n");
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    pthread_join(tid[2], NULL); 
    pthread_join(tid[3], NULL); 
    pthread_join(tid[4], NULL); 
    pthread_join(tid[5], NULL); 
    pthread_join(tid[6], NULL); 
    pthread_join(tid[7], NULL);
}

/* 异步线程取消产生死锁 */
void test1_has_dead_mutex_lock(void)
{
    /* 创建线程 */
    pthread_create(&tid[6], NULL, thread6_without_dead_mutex_lock, NULL);
    pthread_create(&tid[7], NULL, thread7_with_dead_mutex_lock, NULL);

    /* 等待线程之间竞争 */
    sleep(THREAD_WAIT);

    /* 仅取消线程7，该线程取消时可能来不及释放锁 */
    printf("即将发起线程7取消请求！\r\n");
    pthread_cancel(tid[7]);

    /* 等待这两个线程结束 */
    printf("等待线程7结束！\r\n");
    pthread_join(tid[7], NULL);
    printf("线程7运行结束\r\n");
    printf("等待线程6结束！\r\n");
    pthread_join(tid[6], NULL);
    printf("线程6运行结束\r\n");
}

/* 异步线程取消不产生死锁 */
void test2_no_dead_mutex_lock(void)
{
    /* 创建线程 */
    pthread_create(&tid[6], NULL, thread6_without_dead_mutex_lock, NULL);
    pthread_create(&tid[7], NULL, thread7_with_dead_mutex_lock, NULL);

    /* 等待线程之间竞争 */
    sleep(THREAD_WAIT);

    /* 仅取消线程7，该线程取消时可能来不及释放锁 */
    printf("即将发起线程6取消请求！\r\n");
    pthread_cancel(tid[6]);

    /* 等待这两个线程结束 */
    printf("等待线程6结束！\r\n");
    pthread_join(tid[6], NULL);
    printf("线程6运行结束\r\n");
    printf("等待线程7结束！\r\n");
    pthread_join(tid[7], NULL);
    printf("线程7运行结束\r\n");
}

/* 延迟线程取消不产生死锁 */
void test3_no_dead_mutex_lock(void)
{
    /* 创建线程 */
    pthread_create(&tid[4], NULL, thread4_without_dead_mutex_lock, NULL);
    pthread_create(&tid[5], NULL, thread5_with_dead_mutex_lock, NULL);

    /* 等待线程之间竞争 */
    sleep(THREAD_WAIT);

    /* 仅取消线程7，该线程取消时可能来不及释放锁 */
    printf("即将发起线程4取消请求！\r\n");
    pthread_cancel(tid[4]);

    /* 等待这两个线程结束 */
    printf("等待线程4结束！\r\n");
    pthread_join(tid[4], NULL);
    printf("线程4运行结束\r\n");
    printf("等待线程5结束！\r\n");
    pthread_join(tid[5], NULL);
    printf("线程5运行结束\r\n");
}

/* 延迟线程无法取消 */
void test4_no_cancel_point(void)
{
    /* 创建线程 */
    pthread_create(&tid[1], NULL, thread1_without_cannel_point, NULL);

    /* 等待线程之间竞争 */
    sleep(THREAD_WAIT);

    /* 仅取消线程7，该线程取消时可能来不及释放锁 */
    printf("即将发起线程1取消请求！\r\n");
    pthread_cancel(tid[1]);

    /* 等待这两个线程结束 */
    printf("等待线程1结束！\r\n");
    pthread_join(tid[1], NULL);
    printf("线程1运行结束\r\n");
}

int main(int argc, char *argv[])
{
    int test_id = 0;
    /* 初始化锁 mutex==1 */
    pthread_mutex_init(&mutex, NULL);  

    if(argc > 1)
    {   
        /* 将字符型argv[1]转换为整型 */
        test_id = atoi(argv[1]); 
    }

    switch (test_id)
    {
    case 1:
        printf("测试场景: %d -> 异步线程取消产生死锁 \r\n", test_id);
        /* 异步线程取消产生死锁 */
        test1_has_dead_mutex_lock();
        break;
    case 2:
        printf("测试场景: %d -> 异步线程取消不产生死锁 \r\n", test_id);
        /* 异步线程取消不产生死锁 */
        test2_no_dead_mutex_lock();
        break;
    case 3:
        printf("测试场景: %d -> 延迟线程取消不产生死锁 \r\n", test_id);
        /* 延迟线程取消不产生死锁 */
        test3_no_dead_mutex_lock();
        break;
    case 4:
        printf("测试场景: %d -> 延迟线程无法取消 \r\n", test_id);
        /* 延迟线程无法取消 */
        test4_no_cancel_point();
        break;
    default:
        break;
    }
    
    /* 销毁锁 */ 
    pthread_mutex_destroy(&mutex);  

    /* 主程序等待，不退出 */   
    while(1)
    {
        sleep(1);
    }
    
    return 0;
}


