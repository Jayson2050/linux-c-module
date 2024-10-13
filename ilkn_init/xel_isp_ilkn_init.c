/*************************************************************************************
 * Copyright (H), 2019-2029, XEL Tech. Co, Ltd.
 *
 * File Name:        xel_isp_ilkn_init.c
 * Description:      Common initialization for Interlaken-LA of ISP.
 *                   User should implement some functions according to hints.
 * Others:
 * History:
 *  <Author>          <Date>             <Modification>
 *  shenhaojie     2023/09/01            first version
 ************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned int uint32;

#if 0
extern int xel_reg32_write(int unit, uint32 addr, uint32 data);
extern int xel_reg32_read(int unit, uint32 addr, uint32 *data);
#else
int xel_reg32_write(int unit, uint32 addr, uint32 data) {}
int xel_reg32_read(int unit, uint32 addr, uint32 *data) {}
#endif

/* lane num bits offset and mask */
#define TX_LANE_NUM_OFFSET                  6
#define RX_LANE_NUM_OFFSET                  11
#define TX_LANE_NUM_BITS_WIDTH              5
#define RX_LANE_NUM_BITS_WIDTH              5
#define TX_LANE_NUM_MASK                    0x1f
#define RX_LANE_NUM_MASK                    0x1f

#define XEL_ILKN_LANE_NUM_MAX                24

typedef enum
{
    SOC_TYPE_ISP = 0,
    SOC_TYPE_FPGA_NP,
} soc_type_e;

typedef enum
{
    SERDES_LOOPBACK_DISABLE = 0,
    SERDES_LOOPBACK_INSIDE,
    SERDES_LOOPBACK_OUTSIDE,
} serdes_loopback_type_e;

typedef enum
{
    SERDES_BANDWIDTH_1_25G = 0,
    SERDES_BANDWIDTH_3_125G,
    SERDES_BANDWIDTH_6_25G,
    SERDES_BANDWIDTH_10_3125G

} serdes_rate_e;

typedef struct 
{
    uint32_t isp_unit;   /* PCIe unit id of chip ISP */
    uint32_t fpga_unit;  /* PCIe unit id of FPGA/NP */
    uint32_t serdes_num; /* Serdes number to be used */
    uint32_t skipword_disable_en;
    serdes_rate_e serdes_rate; /* rate of serdes */
    serdes_loopback_type_e serdes_loopback;

} isp_ilkn_cfg_t;

/* 
 * Function: xel_soc_reg32_write
 *
 * Purpose:
 *    write register by 32 bits.
 * Parameters:
 *    type    - (IN) object of write can be ISP or NP/FPGA
 *    unit    - (IN) PCIe unit id
 *    addr    - (IN) register address
 *    data    - (IN) data to be write
 * Returns:
 *    0       - OK
 *    others  - Error
 */
static inline int xel_soc_reg32_write(soc_type_e type, uint32_t unit, uint32_t addr, uint32_t data)
{
    switch (type)
    {
        case SOC_TYPE_ISP:
            return xel_reg32_write(unit, addr, data);
        
        case SOC_TYPE_FPGA_NP:
            return xel_reg32_write(unit, addr, data); /* User should implement this function */
        
        default:
            return -1;
    }

    return 0;
}

/* 
 * Function: xel_soc_reg32_read
 *
 * Purpose:
 *    read data from register by 32 bits.
 * Parameters:
 *    type    - (IN) object of write can be ISP or NP/FPGA
 *    unit    - (IN) PCIe unit id
 *    addr    - (IN) register address
 *    data    - (OUT) data read from registers
 * Returns:
 *    0       - OK
 *    others  - Error
 */
static inline int xel_soc_reg32_read(soc_type_e type, uint32_t unit, uint32_t addr, uint32_t *data)
{
    switch (type)
    {
        case SOC_TYPE_ISP:
            return xel_reg32_read(unit, addr, data);
        
        case SOC_TYPE_FPGA_NP:
            return xel_reg32_read(unit, addr, data); /* User should implement this function */
        
        default:
            return -1;
    }

    return 0;
}

/*
 * Function:
 *      xel_ilkn_lane_num_set
 * Purpose:
 *      set lane number for ilkn.
 * Parameters:
 *      unit            - (IN) Unit(channel) number.
 *      rx_lane_num     - (IN) lane number for RX
 *      tx_lane_num     - (IN) lane number for TX
 *  Returns:
 *      XEL_E_XXX
 * Notes:
 *      
 */
int xel_ilkn_lane_num_set(int unit, uint8_t rx_lane_num, uint8_t tx_lane_num)
{
    int rv = 0;
    uint8_t rx_lane_num_cal, tx_lane_num_cal;
    uint32 data;

    if(rx_lane_num > 24)
    {
        rx_lane_num = 24;
    }
    
    if(tx_lane_num > 24)
    {
        tx_lane_num = 24;
    }

    rx_lane_num_cal = XEL_ILKN_LANE_NUM_MAX - rx_lane_num;
    tx_lane_num_cal = XEL_ILKN_LANE_NUM_MAX - tx_lane_num;

    xel_soc_reg32_read(SOC_TYPE_ISP, unit, 0x2004, &data);
    data &= ~(TX_LANE_NUM_MASK << TX_LANE_NUM_OFFSET);
    data &= ~(RX_LANE_NUM_MASK << RX_LANE_NUM_OFFSET);
    
    data |= ((tx_lane_num_cal & TX_LANE_NUM_MASK) << TX_LANE_NUM_OFFSET);
    data |= ((rx_lane_num_cal & RX_LANE_NUM_MASK) << RX_LANE_NUM_OFFSET);
    
    rv= xel_soc_reg32_write(SOC_TYPE_ISP, unit, 0x2004, data);
    
    return rv;
}

/* 
 * Function: xel_serdes_rate_set
 *
 * Purpose:
 *    set serdes rate.
 * Parameters:
 *    type    - (IN) object of write can be ISP or NP/FPGA
 *    unit    - (IN) PCIe unit id
 *    addr    - (IN) register address
 *    data    - (OUT) data read from registers
 * Returns:
 *    0       - OK
 *    others  - Error
 */
static inline int xel_serdes_rate_set(int unit, serdes_rate_e rate)
{
    switch (rate)
    {
        case SERDES_BANDWIDTH_1_25G:
            return xel_soc_reg32_write(SOC_TYPE_ISP, unit, 0x9808, 0x3);
        
        case SERDES_BANDWIDTH_3_125G:
            return xel_soc_reg32_write(SOC_TYPE_ISP, unit, 0x9808, 0x2);

        case SERDES_BANDWIDTH_6_25G:
            return xel_soc_reg32_write(SOC_TYPE_ISP, unit, 0x9808, 0x1);
        
        case SERDES_BANDWIDTH_10_3125G:
            return xel_soc_reg32_write(SOC_TYPE_ISP, unit, 0x9808, 0x0);
        
        default:
            return -1;
    }

    return 0;
}

/* 
 * Function: xel_isp_ilkn_init
 *
 * Purpose:
 *    Interlaken-LA initialization.
 *    Note: User should implement step: 1,7,11,13,15
 * Parameters:
 *    cfg    - (IN) configuration of Interlaken-LA initialization.
 * Returns:
 *    0       - OK
 *    others  - Error
 */
int xel_isp_ilkn_init(isp_ilkn_cfg_t cfg)
{
    uint32_t i = 0, read_times = 0, rddata = 0;
    uint32_t isp_unit = cfg.isp_unit, fpga_unit = cfg.fpga_unit;

    /* Step-1. 用户实现 : 解除FPGA/NP侧 Serdes-TX复位 */

    /* Step-2. 配置ISP的寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc020, 0x94da);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc034, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc034, 0x1);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc008, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc008, 0x3);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x9808, 0x0);

    /* init serdes used, one serdes has 4 physical lanes */
    xel_serdes_rate_set(isp_unit, cfg.serdes_rate);
    for (i = 0; i <= cfg.serdes_num; i++)
    {
        switch (cfg.serdes_loopback)
        {
            case SERDES_LOOPBACK_OUTSIDE:
                xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8004 + (i * 0x400), 0x0f);
                break;

            case SERDES_LOOPBACK_INSIDE:
                xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8004 + (i * 0x400), 0xf0);
                break;

            case SERDES_LOOPBACK_DISABLE:
            default:
                xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8004 + (i * 0x400), 0x0);
                break;
        }
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8094 + (i * 0x400), 0xf0);
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8034 + (i * 0x400), 0x0);
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8000 + (i * 0x400), 0xfffe);
        usleep(25);
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8000 + (i * 0x400), 0x0);
        usleep(10);
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8028 + (i * 0x400), 0x1b3);

        read_times = 0;
        while (1)
        {
            xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x80d0 + (i * 0x400), &rddata);
            //printf("Reg 0x80d0, rd=0x%04x\n", rddata);
            read_times++;
            if( (rddata & 0xff) == 0x0 )
            {
                printf("Reg 0x%04x, rd=0x%04x, RdTimes=%d\n", (0x80d0 + (i * 0x400)), rddata, read_times);
                break;
            }
            usleep(2000);
        }
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8034 + (i * 0x400), 0xf);
    }

    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc008, 0x1f  );

    /* set lane number of ilkn */
    xel_ilkn_lane_num_set(isp_unit, cfg.serdes_num, cfg.serdes_num);

    /* Step-3. 配置ISP ILA的burst_max/burst_min，打开burst_check，使能Interlaken_la功能 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2008, 0xdf03);

    /* Step-4. 配置ISP的Interlaken_la的帧长, 2048 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x201c, 0x0800);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2024, 0x0800);

    /* Step-5. 配置ISP的Interlaken_la的skipword */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x200c, &rddata);
    switch (cfg.skipword_disable_en)
    {
        case 1: /* 不使能skipword */
            xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x200c, (rddata | 0x08));
            break;

        case 0: /* 缺省使能skipword */
        default:
            xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x200c, (rddata & (~0x08))); 
            break;
    }

    /* Step-6. 配置ISP的寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc00c, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc010, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc014, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc018, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc00c, 0xffff);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc010, 0xff);

    /* Step-7. 用户实现 : 解除FPGA/NP侧 ILKN-TX复位 */

    /* Step-8. 配置ISP的寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2000, 0xd7ff);

    /* Step-9. 配置ISP的寄存器 */
    for (i = 0; i <= cfg.serdes_num; i++ )
    {
        xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x8094 + (i * 0x4000), 0xff0);
    }

    /* Step-10. 配置ISP的寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc040, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc044, 0x0);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc040, 0xffff);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0xc044, 0xff);

    /* Step-11. 用户实现: 解除FPGA/NP侧 Serdes-RX复位 */

    /* Step-12. 配置ISP的寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2000, 0xafff);

    /* Step-13. 用户实现: 解除FPGA/NP侧 ILKN-TX复位 */

    /* Step-14. 配置ISP的寄存器 */
    #if 0
    xel_soc_reg32_write(isp_unit, 0xc008, 0x9e);
    xel_soc_reg32_write(isp_unit, 0xc008, 0xff);
    #endif

    sleep(1);

    /* Step-15. 用户实现：FPGA侧serdes和ilkn的 RX 解复位 */

    /* Step-16. 读ISP的align状态寄存器 */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2064, &rddata);
    printf("Reg 0x2064, rd=0x%04x ", rddata);
    if ((rddata & 0x8000) == 0x8000)
    {
        printf("Aligned\n");
    }
    else
    {
        printf("Not Aligned\n");
    }

    /* Step-17. 配置ISP的寄存器 */
    #if 1
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2514, 0x26);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2520, 0xa00a);
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x630c, 0x70ec0e1a);
    #endif

    /* Step-18. 读ISP的流控状态寄存器 */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2564, &rddata);
    printf("Reg 0x2564, rd=0x%04x ", rddata);
    if ((rddata != 0x07) && (rddata != 0x87))
    {
        printf("Unexpected\n");
    }
    else
    {
        printf("Expected\n");
    }

    /* Step-19. 读debug寄存器 */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2080, &rddata);
    printf("Reg 0x2080, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x208c, &rddata);
    printf("Reg 0x208c, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0xc00c, &rddata);
    printf("Reg 0xc00c, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0xc010, &rddata);
    printf("Reg 0xc010, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0xc040, &rddata);
    printf("Reg 0xc040, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0xc044, &rddata);
    printf("Reg 0xc044, rd=0x%04x\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x80c0, &rddata);
    printf("Reg 0x80c0, rd=0x%04x\n", rddata);

    usleep(10);

    /* 读ISP的中断状态寄存器 */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2080, &rddata);
    printf("Reg 0x2080, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x208c, &rddata);
    printf("Reg 0x208c, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2098, &rddata);
    printf("Reg 0x2098, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20a4, &rddata);
    printf("Reg 0x20a4, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20b0, &rddata);
    printf("Reg 0x20b0, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20bc, &rddata);
    printf("Reg 0x20bc, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20c8, &rddata);
    printf("Reg 0x20c8, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20d4, &rddata);
    printf("Reg 0x20d4, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20e0, &rddata);
    printf("Reg 0x20e0, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20ec, &rddata);
    printf("Reg 0x20ec, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20f8, &rddata);
    printf("Reg 0x20f8, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2104, &rddata);
    printf("Reg 0x2104, rd=0x%04x\r\n", rddata);

    /* 清除ISP的中断状态寄存器 */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2080, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x208c, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2098, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20a4, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20b0, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20bc, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20c8, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20d4, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20e0, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20ec, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x20f8, 0xffff );
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x2104, 0xffff );

    /* 读ISP的中断状态寄存器 */
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2080, &rddata);
    printf("Reg 0x2080, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x208c, &rddata);
    printf("Reg 0x208c, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2098, &rddata);
    printf("Reg 0x2098, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20a4, &rddata);
    printf("Reg 0x20a4, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20b0, &rddata);
    printf("Reg 0x20b0, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20bc, &rddata);
    printf("Reg 0x20bc, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20c8, &rddata);
    printf("Reg 0x20c8, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20d4, &rddata);
    printf("Reg 0x20d4, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20e0, &rddata);
    printf("Reg 0x20e0, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20ec, &rddata);
    printf("Reg 0x20ec, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x20f8, &rddata);
    printf("Reg 0x20f8, rd=0x%04x\r\n", rddata);
    xel_soc_reg32_read(SOC_TYPE_ISP, isp_unit, 0x2104, &rddata);
    printf("Reg 0x2104, rd=0x%04x\r\n", rddata);

    /* Step-20. 配置ISP芯片工作的字节序模式
        1、小端CPU配置为网络字节序模式(对应寄存器值为0x8)，大端CPU配置为非网络字节序模式(对应寄存器值为0x0)
    */
    xel_soc_reg32_write(SOC_TYPE_ISP, isp_unit, 0x6300, 0x8);

    return 0;
}


void main(void)
{
    isp_ilkn_cfg_t cfg;
    
    memset(&cfg, 0, sizeof(isp_ilkn_cfg_t));

    cfg.isp_unit = 0;
    cfg.serdes_num = 2;
    cfg.serdes_rate = SERDES_BANDWIDTH_10_3125G;
    xel_isp_ilkn_init(cfg);
}
