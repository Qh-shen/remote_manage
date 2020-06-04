/*
 * File      : template.cpp
 * template file
 * COPYRIGHT (C) 2020, zc
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-5-4      zc           the first version
 */

/**
 * @addtogroup IMX6ULL
 */
/*@{*/

#include "../include/SocketUdpThread.h"
#include "../include/SystemConfig.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static CUdpProtocolInfo<UdpInfo *> *pUdpProtocolInfo;
static uint8_t 	nRxCacheBuffer[UDP_BUFFER_SIZE];
static uint8_t  nTxCacheBuffer[UDP_BUFFER_SIZE];

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Local Function Declaration
***************************************************************************/

/**************************************************************************
* Local Function
***************************************************************************/
static void *SocketUdpLoopThread(void *arg);

/**************************************************************************
* Function
***************************************************************************/
/**
 * UDP Socket通讯初始化
 * 
 * @param NULL
 *  
 * @return NULL
 */
void SocketUdpThreadInit(void)
{
    int nErr;
	pthread_t tid1;

    pUdpProtocolInfo = new CUdpProtocolInfo<UdpInfo *>(nRxCacheBuffer, nTxCacheBuffer, UDP_BUFFER_SIZE);
    nErr = pthread_create(&tid1, NULL, SocketUdpLoopThread, NULL);
	if(nErr != 0)
    {
		USR_DEBUG("udp task thread create nErr, %d\n", nErr);
	}
}

/**
 * Udp绑定和数据处理的接口实现
 * 
 * @param arg:通过pthread传递的参数
 *  
 * @return NULL
 */
static void *SocketUdpLoopThread(void *arg)
{
    int socket_fd, result, recv_len;   
    char recvbuf[1024] = {0};  
    struct sockaddr_in servaddr, clientaddr;  
    socklen_t client_sock_len;      
    struct SSystemConfig *pSystemConfigInfo;
	int is_bind_fail = 0;
    UdpInfo sUdpInfo;

    USR_DEBUG("Socket Udp Thread Start!\n");
	pSystemConfigInfo = GetSSytemConfigInfo();
    /*创建socket接口, SOCK_DGRAM表示无连接的udp接口*/
    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(socket_fd != -1)
    {
        /*绑定到指定端口*/
        memset(&servaddr, 0, sizeof(servaddr));    
        servaddr.sin_family = AF_INET;     
        servaddr.sin_addr.s_addr = inet_addr(pSystemConfigInfo->m_udp_ipaddr.c_str());  
        servaddr.sin_port = htons(pSystemConfigInfo->m_udp_net_port); 

        do 
        {
            result = bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
            if(result == -1)
            {
                if(is_bind_fail == 0)
                {
                    is_bind_fail = 1;
                    SOCKET_DEBUG("Udp Bind Failed!\r\n");
                }
                sleep(1);
            }
            else
            {
                break;
            }
        } while (1); //网络等待socket绑定完成后执行后续

        SOCKET_DEBUG("Udp Bind ok, ServerIp:%s, NetPort:%d\n", pSystemConfigInfo->m_udp_ipaddr.c_str(), 
                pSystemConfigInfo->m_udp_net_port);  

        for(;;)
        {	   
            int nFlag;
		    nFlag = pUdpProtocolInfo->CheckRxBuffer(socket_fd, &sUdpInfo);
		    if(nFlag == RT_OK)
            {
                pUdpProtocolInfo->ExecuteCommand(socket_fd);
                pUdpProtocolInfo->SendTxBuffer(socket_fd, &sUdpInfo);
                SOCKET_DEBUG("Udp Process Success\r\n");
                break;
		    }
            else
            {
                sleep(1);
            }
        }
	}
    else
    {
        SOCKET_DEBUG("Udp Socket Init Failed\n");
    }
    
    close(socket_fd);
}