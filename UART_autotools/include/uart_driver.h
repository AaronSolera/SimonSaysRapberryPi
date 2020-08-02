#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdio.h>
#include <sys/mman.h> //mmap
#include <err.h> //error handling
#include <fcntl.h> //file ops
#include <unistd.h> //usleep
#include <string.h>
#include <math.h>

// UART clock frequency
#define FUARTCLK 48000000

#define ALL 0
#define CLEAR 0x0
#define SET 0xFFFFFFFF
// Macros for line control register
#define WLEN 5

// Macros for control register
#define RXE 9
#define TXE 8
#define UARTEN 0

// Macros for interrupt mask set clear register
#define TXIM 5
#define RXIM 4

// Function prototypes
void uartInit(int baudrate);
void uartTransmitData(char data);	
char uartReceiveData();

#endif
