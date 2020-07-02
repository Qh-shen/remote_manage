/*
 * File      : uart_task.h
 * uart task interface
 * COPYRIGHT (C) 2020, zc
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-5-4      zc           the first version
 * 2020-5-20     zc           Code standardization 
 */

/**
 * @addtogroup IMX6ULL
 */
/*@{*/
#ifndef _INCLUDE_UART_TASK_H
#define _INCLUDE_UART_TASK_H

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "UsrTypeDef.h"
#include "ApplicationThread.h"
#include "UsrProtocol.hpp"

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define UART_BUFFER_SIZE     		1200

/**************************************************************************
* Global Type Definition
***************************************************************************/
template<class T>
class CUartProtocolInfo:public CProtocolInfo<T>
{
public:
	using CProtocolInfo<T>::CProtocolInfo;

	int DeviceRead(int nFd, uint8_t *pDataStart, uint16_t nDataSize, T ExtraInfo)
	{
		*ExtraInfo = read(nFd, pDataStart, nDataSize);
		return *ExtraInfo;
	}
	int DeviceWrite(int nFd, uint8_t *pDataStart, uint16_t nDataSize, T ExtraInfo)
	{
		*ExtraInfo = write(nFd, pDataStart, nDataSize);
		return *ExtraInfo;
	}
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void UartThreadInit(void);
#endif