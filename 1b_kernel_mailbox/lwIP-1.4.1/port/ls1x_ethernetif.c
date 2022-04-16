/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bsp.h"

#ifdef USE_LWIP

/*
 * Standard library include
 */
#include <stdio.h>
#include <string.h>

#include "lwip/tcpip.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "netif/etharp.h"

#include "ls1x_gmac.h"

//-----------------------------------------------------------------------------

#define netifINTERFACE_TASK_STACK_SIZE      (8*1024)

#define netifINTERFACE_TASK_PRIORITY        DEFAULT_THREAD_PRIO

#define IFNAME0 'e'
#define IFNAME1 '0'

/*
 * The time to block waiting for input.
 */
#if BSP_USE_OS
#define RW_LOCK(sem)    sys_sem_wait(sem);      /* 获取设备使用权 */
#define RW_UNLOCK(sem)  sys_sem_signal(sem);    /* 释放设备使用权 */
#else
#define RW_LOCK(sem)
#define RW_UNLOCK(sem)
#endif

//---------------------------------------------------------------------------------------

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct eth_addr *ethaddr;
    /*
     * Add whatever per-interface state that is needed here.
     */
#if BSP_USE_OS
    sys_sem_t tx_sem;               // tramsmit lock
    sys_sem_t rx_sem;               // receive lock
#endif

    void *dev_gmac;                 // GMAC_t device pointer
};

/**********************************************************
 * netcard 0
 */
#if defined(BSP_USE_GMAC0)

static char   ls1x_gmac0_mac_addr[6] = { 0x72, 0x74, 0x00, 0x83, 0xDE, 0x12 };
static struct ethernetif ls1x_gmac0_ethernetif =
{
    .ethaddr  = NULL,
    .dev_gmac = NULL,
};

static struct netif ls1x_gmac0_netif;

struct netif *p_gmac0_netif = &ls1x_gmac0_netif;

#endif

/**********************************************************
 * netcard 1
 */
#if defined(BSP_USE_GMAC1)

static char   ls1x_gmac1_mac_addr[6] = { 0x72, 0x74, 0x00, 0x83, 0xEE, 0x12 };
static struct ethernetif ls1x_gmac1_ethernetif =
{
    .ethaddr  = NULL,
    .dev_gmac = NULL,
};

static struct netif ls1x_gmac1_netif;

struct netif *p_gmac1_netif = &ls1x_gmac1_netif;

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/* Forward declarations. */
#if BSP_USE_OS
static
#endif
void ethernetif_input(void *pParams);

/* netif the already initialized lwip network interface structure
 * for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
    struct ethernetif *ptr_if = (struct ethernetif *)netif->state;
    void *pMAC = ptr_if->dev_gmac;
    char *p_mac_addr;

#if defined(BSP_USE_GMAC0)
    if (pMAC == devGMAC0)
        p_mac_addr = ls1x_gmac0_mac_addr;
#endif

#if defined(BSP_USE_GMAC1)
    if (pMAC == devGMAC1)
        p_mac_addr = ls1x_gmac1_mac_addr;
#endif

#if BSP_USE_OS
    sys_sem_new(&ptr_if->rx_sem, 1);
    sys_sem_new(&ptr_if->tx_sem, 1);
#endif

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
	netif->hwaddr[0] = p_mac_addr[0];
	netif->hwaddr[1] = p_mac_addr[1];
	netif->hwaddr[2] = p_mac_addr[2];
	netif->hwaddr[3] = p_mac_addr[3];
	netif->hwaddr[4] = p_mac_addr[4];
	netif->hwaddr[5] = p_mac_addr[5];

	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

    /**************************************************************************
     * Do whatever else is needed to initialize interface.
     */
	ls1x_gmac_init(pMAC, (void *)p_mac_addr);

    /**************************************************************************
     * Create the task that handles the GMAC.
     * return: TaskHandle_t
     */
#if BSP_USE_OS
  #if defined(BSP_USE_GMAC0)
    if (pMAC == devGMAC0)
    {
	    sys_thread_new("ETH0",
                        ethernetif_input,
                        netif,
                        netifINTERFACE_TASK_STACK_SIZE,
			            netifINTERFACE_TASK_PRIORITY);
    }
  #endif

  #if defined(BSP_USE_GMAC1)
    if (pMAC == devGMAC1)
    {
	    sys_thread_new("ETH1",
                        ethernetif_input,
                        netif,
                        netifINTERFACE_TASK_STACK_SIZE,
			            netifINTERFACE_TASK_PRIORITY);
    }
  #endif
#endif
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    struct ethernetif *ptr_if = (struct ethernetif *)netif->state;
    void *pMAC = ptr_if->dev_gmac;
    struct pbuf *q;
    unsigned char *p_txbuf=NULL, *ptr;

    //-------------------------------------------------------------------------
    // 获取可用的发送缓冲区
    //-------------------------------------------------------------------------

    RW_LOCK(&ptr_if->tx_sem);

    if (ls1x_gmac_wait_tx_idle(pMAC, &p_txbuf) <= 0)
    {
        RW_UNLOCK(&ptr_if->tx_sem);         /* 释放设备使用权 */
        return ERR_BUF;                     /* 发送失败 */
    }

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE);          /* drop the padding word */
#endif

    ptr = p_txbuf;
    for(q = p; q != NULL; q = q->next)
    {
        /* Send the data from the pbuf to the interface, one pbuf at a time.
           The size of the data in each pbuf is kept in the ->len variable. */
        memcpy((void *)ptr, q->payload, q->len);
        ptr += q->len;
    }

    /**************************************************************************
     * signal that packet should be sent();
     */
    ls1x_gmac_write(pMAC, (void *)p_txbuf, (int)p->tot_len, NULL);

    RW_UNLOCK(&ptr_if->tx_sem);

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);           /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct ethernetif *ptr_if = (struct ethernetif *)netif->state;
    void *pMAC = ptr_if->dev_gmac;
    struct pbuf *p, *q;
    unsigned char *p_rxbuf=NULL, *ptr;
    short len;

    //-------------------------------------------------------------------------
    // 获取有数据的接收缓冲区
    //-------------------------------------------------------------------------

    RW_LOCK(&ptr_if->rx_sem);

	/****************************************************
     * 取接收缓冲区
     */
    if (ls1x_gmac_wait_rx_packet(pMAC, &p_rxbuf) <= 0)
    {
        RW_UNLOCK(&ptr_if->rx_sem);
        return NULL;
    }

    len = ls1x_gmac_read(pMAC, (void *)p_rxbuf, 1514, NULL);
    if (len <= 0)
    {
        RW_UNLOCK(&ptr_if->rx_sem);
	    return NULL;
	}

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE;                    /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    if (p != NULL)
    {
#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE);      /* drop the padding word */
#endif

        ptr = p_rxbuf;

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf.
         */
        for (q = p; q != NULL; q = q->next)
        {
            /* Read enough bytes to fill this pbuf in the chain. The
             * available data in the pbuf is given by the q->len
             * variable.
             * This does not necessarily have to be a memcpy, you can also preallocate
             * pbufs for a DMA-enabled MAC and after receiving truncate it to the
             * actually received size. In this case, ensure the tot_len member of the
             * pbuf is the sum of the chained pbuf len members.
             */
            memcpy(q->payload, ptr, q->len);
            ptr += q->len;
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE);       /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    }
    else
    {
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
    }
    
    RW_UNLOCK(&ptr_if->rx_sem);

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface.Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
#if BSP_USE_OS
static
#endif
void ethernetif_input(void *pParams)
{
	struct netif *netif;
	struct eth_hdr *ethhdr;
	struct pbuf *p = NULL;
	netif = (struct netif *)pParams;
	struct ethernetif *ptr_if = (struct ethernetif *)netif->state;
    void *pMAC = ptr_if->dev_gmac;
    // unsigned int recv;

#if BSP_USE_OS
    delay_ms(200);      /* 在其它任务后启动 */
    
    /**************************************************************************
     * 在这个位置启动 GMAC
     */
    if (ls1x_gmac_ioctl(pMAC, IOCTL_GMAC_IS_RUNNING, NULL) <= 0)
    {
        ls1x_gmac_ioctl(pMAC, IOCTL_GMAC_START, NULL);
    }
    
    /**************************************************************************
     * 进入接收任务
     */
	for (;;)
#endif
    {
        unsigned char *p_rxbuf;
        
        p = NULL;
        
		/**************************************************
         * 等待 GMAC_RX_EVENT
         */
        ls1x_gmac_wait_rx_packet(pMAC, &p_rxbuf);

        if (p_rxbuf != NULL)
        {
			p = low_level_input(netif);
        }
        
        if (p == NULL)
        {
#if BSP_USE_OS
            continue;
#else
            return;
#endif
        }

		/* points to packet payload, which starts with an Ethernet header */
		ethhdr = p->payload;
		switch (htons(ethhdr->type))
        {
		    /* IP or ARP packet? */
		    case ETHTYPE_IP:
		    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
            /* PPPoE packet? */
            case ETHTYPE_PPPOEDISC:
            case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
			    /*
                 * full packet send to tcpip_thread to process
                 */
			    if (netif->input(p, netif) != ERR_OK)
                {
				    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
				    pbuf_free(p);
				    p = NULL;
			    }
			    break;

            default:
			    pbuf_free(p);
			    p = NULL;
			    break;
		}
	}
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
   struct ethernetif *ptr_if = (struct ethernetif *)netif->state;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
     
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    ptr_if->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

//-----------------------------------------------------------------------------

/*
 * Parameter:
 *
 *   ip0: unsigned char ip0[4], GMAC0 IPv4 Address.
 *   ip1: unsigned char ip1[4], GMAC1 IPv4 Address.
 *
 */
static int is_lwip_initialized = 0;

void ls1x_initialize_lwip(unsigned char *ip0, unsigned char *ip1)
{
    int added_default = 0;

    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gateway;

    if (is_lwip_initialized)
        return;
        
#if BSP_USE_OS
    tcpip_init(NULL, NULL);
#endif

    // IP4_ADDR(&netmask, 255, 255 , 255, 0);
    // IP4_ADDR(&gateway, 192, 168,    1, 1);

#if defined(BSP_USE_GMAC0)

    if (ip0 != NULL)
    {
        IP4_ADDR(&ipaddr,  ip0[0], ip0[1], ip0[2], ip0[3]); /* user IP address */
        IP4_ADDR(&netmask, 255, 255, 255, 0);
        IP4_ADDR(&gateway, ip0[0], ip0[1], ip0[2], 1);
    }
    else
    {
        IP4_ADDR(&ipaddr,  192, 168,   1, 123);     /* default IP address */
        IP4_ADDR(&netmask, 255, 255, 255,   0);
        IP4_ADDR(&gateway, 192, 168,   1,   1);
    }
    
    ls1x_gmac0_ethernetif.dev_gmac = devGMAC0;      /* user data */

    /* add the network interface */
    netif_add(&ls1x_gmac0_netif,
              &ipaddr,
              &netmask,
              &gateway,
              &ls1x_gmac0_ethernetif,
              &ethernetif_init,
              &ethernet_input);

    /******************************************************
     * Registers the default network interface
     */
    netif_set_default(&ls1x_gmac0_netif);
    netif_set_up(&ls1x_gmac0_netif);

    added_default = 1;

#endif

#if defined(BSP_USE_GMAC1)

    if (ip1 != NULL)
    {
        IP4_ADDR(&ipaddr,  ip1[0], ip1[1], ip1[2], ip1[3]); /* user IP address */
        IP4_ADDR(&netmask, 255, 255, 255, 0);
        IP4_ADDR(&gateway, ip1[0], ip1[1], ip1[2],   1);
    }
    else
    {
        IP4_ADDR(&ipaddr,  192, 168,   1, 124);     /* default IP address */
        IP4_ADDR(&netmask, 255, 255, 255,   0);
        IP4_ADDR(&gateway, 192, 168,   1,   1);
    }
    
    ls1x_gmac1_ethernetif.dev_gmac = devGMAC1;      /* user data */

    /* add the network interface */
    netif_add(&ls1x_gmac1_netif,
              &ipaddr,
              &netmask,
              &gateway,
              &ls1x_gmac1_ethernetif,
              &ethernetif_init,
              &ethernet_input);

    if (added_default)
    {
        is_lwip_initialized = 1;
        return;
    }

    /******************************************************
     * Registers the default network interface
     */
    netif_set_default(&ls1x_gmac1_netif);
    netif_set_up(&ls1x_gmac1_netif);

#endif

    is_lwip_initialized = 1;
}

#endif // #ifdef USE_LWIP

/*
 * @@ END
 */
 
