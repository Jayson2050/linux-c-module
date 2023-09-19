/*
    该示例代码用于计算函数执行时间；
    如果需要计算某个程序运行时间，可以在终端通过time 命令实现。
    注意：编译的时候需要加上编译选项：-lrt
*/

#include <stdio.h>
#include <time.h>
#include <string.h>


/*
 * Function:
 *      _xel_timespec_check
 * Purpose:
 *      check timespec.
 * Parameters:
 *      ts        - (IN) value of timespec
 * Returns:
 *      0 is ok, -1 is error.
 * Notes:
 *
*/
static int 
_xel_timespec_check(struct timespec *ts)
{
    if((ts->tv_nsec < 0 ) || (ts->tv_nsec >= 1000000000))
    {
        return -1;
    }

    return 0;
}


/*
 * Function:
 *      xel_timespec_get
 * Purpose:
 *      get current timespec of OS.
 * Parameters:
 *      ts        - (OUT) value of timespec
 * Returns:
 *      
 * Notes:
 *
*/
int xel_timespec_get(struct timespec *ts)
{
    int rv;

    memset(ts, 0, sizeof(struct timespec));
    rv = clock_gettime(CLOCK_MONOTONIC, ts);
    if(rv)
    {
        printf("Get timespec error!\r\n");
    }
    
    return rv;
}


/*
 * Function:
 *      xel_runtime_get
 * Purpose:
 *      get time difference between two timespec.
 * Parameters:
 *      ts_start        - (IN) value of timespec start
 *      ts_start        - (IN) value of timespec end
 *      ts_diff         - (OUT) value of timespec difference
 * Returns:
 *      0 is ok, -1 is error.
 * Notes:
 */
int xel_runtime_get(struct timespec *ts_start,
                    struct timespec *ts_end,
                    struct timespec *ts_diff)
{
    int rv = -1;

    memset(ts_diff, 0, sizeof(struct timespec));

    rv = _xel_timespec_check(ts_start);
    if (rv)
    {
        fprintf(stderr, "invalid time #1: %lld.%.9ld.\n",
                        (long long) ts_start->tv_sec,ts_start->tv_nsec);
        return rv;
    }

    rv = _xel_timespec_check(ts_end);
    if (rv) 
    {
        fprintf(stderr, "invalid time #2: %lld.%.9ld.\n",
                        (long long) ts_end->tv_sec,ts_end->tv_nsec);
        return rv;
    }

    ts_diff->tv_sec  = ts_end->tv_sec  - ts_start->tv_sec;
    ts_diff->tv_nsec = ts_end->tv_nsec - ts_start->tv_nsec;
    
    if (ts_diff->tv_nsec >= 1000000000)
    {
        ts_diff->tv_sec++;
        ts_diff->tv_nsec -= 1000000000;
    }
    else if (ts_diff->tv_nsec < 0)
    {
        ts_diff->tv_sec--;
        ts_diff->tv_nsec += 1000000000;
    }

    return 0;
}


void main(void)
{
    int rv;
    int count = 10;
    long t_time_n = 0;  //nano secend
    long t_time_s = 0;  //secnd
    struct timespec ts_start, ts_end, ts;

    while (count--) 
    {
        rv = xel_timespec_get(&ts_start);
        usleep(2000); /* 2ms */
        rv = xel_timespec_get(&ts_end);

        rv = xel_runtime_get(&ts_start, &ts_end, &ts);
        
        t_time_n += ts.tv_nsec;
        t_time_s += ts.tv_sec;
    }
    
    printf("** Total time %ld s + %ld nsec\n",t_time_s, t_time_n);
}
