/*
 * File      : SystemConfig.cpp
 * This file is for SystemConfig
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

#include "../include/SystemConfig.h"
#include "../driver/Led.h"
#include "../driver/Beep.h"
#include <fstream>
#include <iostream>

/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static SSystemConfig SSysConifg = {
    //串口配置
    115200,
    8,
    1,
    std::string("n"),

    //TCP网络设置
    std::string("127.0.0.1"),
    8000,

    //UDP网络设置
    std::string("127.0.0.1"),
    8001,

    //硬件状态
    0,
    0,

    //设备名称
    std::string(TTY_DEVICE),
    std::string(LED_DEVICE),
    std::string(BEEP_DEVICE),
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Local Function Declaration
***************************************************************************/

/**************************************************************************
* Local Function
***************************************************************************/

/**************************************************************************
* Function
***************************************************************************/
/**
 * 系统硬件初始化
 * 
 * @param NULL
 *  
 * @return 硬件配置处理的结果
 */
int system_config_init(std::string &sConfigfile)
{
    Json::Value root;
    std::ifstream ifs;
    ifs.open(sConfigfile.c_str());

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << root << std::endl;

    //串口状态
    SSysConifg.m_baud = root["Uart"]["Baud"].asInt();
    SSysConifg.m_data_bits = root["Uart"]["DataBits"].asInt();
    SSysConifg.m_stop_bits = root["Uart"]["StopBits"].asInt();
    SSysConifg.m_parity = std::string(root["Uart"]["Parity"].asString());

    //sokcet TCP状态
    SSysConifg.m_tcp_ipaddr = std::string(root["SocketTcp"]["ipaddr"].asString());
    SSysConifg.m_tcp_net_port = root["SocketTcp"]["net_port"].asInt();

    //sokcet UDP状态
    SSysConifg.m_udp_ipaddr = std::string(root["SocketUdp"]["ipaddr"].asString());
    SSysConifg.m_udp_net_port = root["SocketUdp"]["net_port"].asInt();

    //硬件状态
    SSysConifg.m_led0_status = root["Led0"].asInt();
    SSysConifg.m_beep0_status = root["Beep0"].asInt();

    //设备名称
    SSysConifg.m_dev_led = std::string(root["Device"]["Led"].asString());
    SSysConifg.m_dev_beep = std::string(root["Device"]["Beep"].asString());
    SSysConifg.m_dev_serial = std::string(root["Device"]["Serial"].asString());

    return EXIT_SUCCESS;
}


/**
 * 获取系统状态配置信息
 * 
 * @param NULL
 *  
 * @return 返回设备状态信息
 */
SSystemConfig *GetSSytemConfigInfo(void)
{
    return &SSysConifg;
}