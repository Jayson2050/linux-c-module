/*
 * @Descripttion: reference : https://blog.csdn.net/keep_contact/article/details/127712918?spm=1001.2014.3001.5506
 * @version:
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2023-11-07 15:30:28
 * @LastEditors: shenhaojie
 * @LastEditTime: 2023-11-07 17:35:05
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h> /* common type define */
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stddef.h>

/* 定义数组计算的长度，以字节为单位，如array[0]对应byte0, array[1]对应byte1 */
#define CALC_ARRAY_BYTE_LEN     100

/* 定义进制单位 */
#define DIGITAL_BASE            256

/* bign is big number */
typedef struct
{
    uint8_t value[CALC_ARRAY_BYTE_LEN]; 
    int len;
} bign_t;

/*
最后如果需要输出bign_t变量的值，遍历一遍就可以实现了
*/
void debug_print(bign_t data)
{
    int i;

    for (i = data.len - 1; i >= 0; i--)
    {
        printf("%02x ", data.value[i]);
    }
    printf("\n");
}

/*
一般输入大整数时，都是先用字符串读入，然后再把字符串另存为至bign_t结构体。
由于使用char数组进行读入时，整数高位会变成数组地位，整数的地位会变成数组的高位。
因此为了让整数在bign_t中是顺位存储，需要让字符串倒着赋值给d[]数组。
 */
/* 将字符串整数转换为bign_t */
bign_t change(char str[])
{
    int i;
    bign_t array;
    
    memset(&array, 0, sizeof(bign_t));
    
    array.len = strlen(str); /* bign_t的长度就是字符串的长度 */
    for (i = 0; i < array.len; i++)
    {
        array.value[i] = str[array.len - i - 1] - '0';
    }
    return array;
}

/*
如果要比较两个bign_t变量的大小，也很简单：先判断len大小，若不相等，长的为大，
若相等，则从高位至低位逐个比较，直到某一位不等，则可以判断两者大小
*/
int compare(bign_t a, bign_t b)
{
    int i;
    
    if (a.len > b.len)
    {
        return 1; /* a大 */
    }
    else if (a.len < b.len)
    {
        return -1; /* a小 */
    }
    else
    {
        for (i = a.len - 1; i >= 0; i--) /* 从高位到低位开始比较 */
        {
            if (a.value[i] > b.value[i])
            {
                return 1; /* 只要有一位a大，则a大 */
            }
            else if (a.value[i] < b.value[i])
            {
                return -1; /* 只要有一位a小，则a小 */
            }
        }
        return 0; /* 两位相等 */
    }
}

/*
1、高精度加法
加法实现方式与我们以前学到的加法一样。对于某一位的运算：我们将该位上的两个数字与进位相加，
得到的结果取个位数作为该结果，十位数作为新的进位。
这里有一点需要注意，这样写法的条件是两个对象都是非负整数。如果有一方是负的，
可以在转换到数组这一步时去掉符号，再使用高精度减法；如果两个都是负的，都去掉负号后采用高精度加法，最后负号加回去即可。
 */
bign_t add(bign_t a, bign_t b)
{
    int i;
    int temp;
    bign_t c;
    
    int carry = 0; /* carry是进位 */

    memset(&c, 0, sizeof(bign_t));

    for (i = 0; i < a.len || i < b.len; i++) /* 以较长的为界限 */
    {
        temp = a.value[i] + b.value[i] + carry; /* 两个对应数组位置与进位相加 */

        c.value[c.len++] = temp % DIGITAL_BASE; /* 个位数为该位的结果 */
        carry = temp / DIGITAL_BASE; /* 计算新的进位 */
    }
    if (carry != 0) /* 如果最后进位不为0，则直接赋给结果的最高位 */
    {
        c.value[c.len++] = carry;
    }

    return c;
}

/*
2、 高精度减法
通过对减法步骤的拆分可以得到一个简练的步骤：对某一位，比较被减位和减位，如果不够减，则令被减位的高位减1，
被减位加10再进行减法；如果够减，直接减。最后需要注意减法后高位可能有多余的0，要去除他们，但也要保证结果至少有一位数。
最后需要指出，使用sub函数前要比较两个数的大小，如果被减数小于减数，需要交换两个变量，然后输出负号，再用sub函数。
*/
bign_t sub(bign_t a, bign_t b) /* a - b */
{
    int i;
    bign_t c;

    memset(&c, 0, sizeof(bign_t));

    for (i = 0; i < a.len || i < b.len; i++) /* 以较长的为界限 */
    {
        if (a.value[i] < b.value[i]) /* 不够减 */
        {
            a.value[i + 1]--;  /* 向高位借位 */
            a.value[i] += DIGITAL_BASE; /* 向前位借位 DIGITAL_BASE */
        }
        c.value[c.len++] = a.value[i] - b.value[i]; /* 减法结果为当前位 */
    }
    while ((c.len - 1 >= 1) && (c.value[c.len - 1] == 0))
    {
        c.len--; /* 去除高位的0，同时至少保留一位最低位 */
    }
    
    return c;
}

/*
3、高精度与低精度的乘法
这里所谓的低精度就是基本数据类型存储的数据，如int。这里就是bign_t与int的乘法。
对某一位来说是这样的步骤：取bign_t的某位与int型整体相乘，再与进位相加，所得结果的个位数作为该结果，高位部分作为新的进位。
另外，如果a和b中存在负数，需要先记录下负号，然后取他们的绝对值带入函数。
*/
bign_t multi(bign_t a, int b)
{
    int i;
    int temp;
    bign_t c;
    int carry = 0; /* 进位 */

    memset(&c, 0, sizeof(bign_t));

    for (i = 0; i < a.len; i++)
    {
        temp = a.value[i] * b + carry;
        c.value[c.len++] = temp % DIGITAL_BASE; /* 个位作为该结果 */
        carry = temp / DIGITAL_BASE;        /* 高位部分作为新的进位 */
    }

    while (carry != 0) /* 和加法不一样，乘法的进位可能不止一位，因此用while */
    {
        c.value[c.len++] = carry % DIGITAL_BASE;
        carry /= DIGITAL_BASE;
    }
    return c;
}

/*
4、高精度与低精度的除法
归纳其中一位的步骤：上一步的余数乘以10加上该步的位，得到该步临时的被除数，将其与除数比较；
如果不够除，则该位的商为0；如果够除，则商即为对应的商，余数即为对应的余数。
最后一步要注意减法后高位可能有多余的0，要去除他们，但也要保证结果至少有一位数。
考虑到很多题目会要求得到余数，因此把余数写成引用的形式直接作为参数传入，或者也可以把余数设成全局变量。
*/
bign_t divide(bign_t a, int b, int r) /* r为余数 */
{
    int i;
    bign_t c;

    memset(&c, 0, sizeof(bign_t));

    c.len = a.len;  /* 被除数的每一位和商的每一位是一一对应的，因此先令长度相等 */
    for (i = a.len - 1; i >= 0; i--) /* 从高位开始 */
    {
        r = r * DIGITAL_BASE + a.value[i]; /* 和上一位遗留的余数组合 */
        if (r < b)
        {
            c.value[i] = 0; /* 不够除，该位为0 */
        }
        else /* 够除 */
        {
            c.value[i] = r / b; /* 商 */
            r = r % b; /* 获得新的余数 */
        }
    }
    while (c.len - 1 >= 1 && c.value[c.len - 1] == 0)
    {
        c.len--; /* 去除高位的0，同时至少保留一位最低位 */
    }
    return c;
}

void main(void)
{
    int i = 0;
    uint8_t data1[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    uint8_t data2[4] = {0xff, 0x33, 0x44, 0x55 };

    bign_t a, b, c;
    memset(&a, 0, sizeof(bign_t));
    memset(&b, 0, sizeof(bign_t));
    memset(&c, 0, sizeof(bign_t));

    a.len = 8;
    b.len = 8;
    c.len = 8;

    #if 1
    memcpy(a.value, data1, sizeof(data1));
    memcpy(b.value, data2, sizeof(data2));
    #else
    for (i = 0; i < 8; i++)
    {
        a.value[i] = data1[i];
        b.value[i] = data2[i];
    }
    #endif
    
    printf("====== add test ====== \r\n");
    c = add(a, b);
    debug_print(c);

    printf("====== sub test ====== \r\n");
    c = sub(a, b);
    debug_print(c);

    printf("\n");
}