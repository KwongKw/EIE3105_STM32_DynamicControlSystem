#ifndef UART2_H
#define UART2_H

// baud rate 115200, 1 stop bit, 8-bit data bits, no parity bit

bool usart2_printBusy(void); // return true when USART2 is busy (using interrupt, no polling)
bool usart2_printString(const char*); // output a string to USART2 (using interrupt, no polling)

#endif // UART2_H
