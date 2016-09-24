/*
 * uart.hpp
 *
 *  Created on: Sep 10, 2016
 *      Author: James
 */

#ifndef UART_UART_HPP_
#define UART_UART_HPP_

#include "Buffers.hpp"
#include "msp.h"

namespace Peripherials
{
class UART
{
private:
	EUSCI_A_Type& regs;
	Buffers::RollingBuffer txBuffer;
	Buffers::BaseBuffer rxBuffer;
	int rxIndex;

public:
	UART(EUSCI_A_Type& instance, int baud, Buffers::RollingBuffer backingTx, Buffers::BaseBuffer backingRx);
	void Send(char c);
	void Send(char* str);
	//void Send(char* arr, int length);

	Buffers::BaseBuffer GetBuffer();
	void ClearBuffer();
	int GetBufferLength();

	void OnInterrupt();
};

extern UART UART_A1, UART_A3;
}


#endif /* UART_UART_HPP_ */
