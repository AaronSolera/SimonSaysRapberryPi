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
#define SPS 7
#define WLEN 5
#define FEN 4
#define STP2 3
#define EPS 2
#define PEN 1
#define BRK 0

// Macros for control register
#define RXE 9
#define TXE 8
#define UARTEN 0

// Macros for interrupt mask set clear register
#define TXIM 5
#define RXIM 4

typedef volatile unsigned int * reg;
// Static base
static unsigned UART_BASE = 0x3f201000;
static unsigned GPIO_BASE = 0x3f200000;

// Regs pointers
reg gpio_rg;
reg gpio_sel1;
reg data_rg;
reg line_control_rg;
reg control_rg;
reg flags_rg;
reg int_baudrate_rg;
reg frac_baudrate_rg;
reg interrupt_mask_set_clear_rg;
reg raw_interrupt_status_rg;
reg interrupt_clear_rg;
// Function prototypes
void uartInit(int baudrate);
char uartReadChar();
void uartSetBaudrate(int baudrate);
void uartSetLineControlReg(int field_name, int bits);
void uartSetControlReg(int field_name, int bits);
void uartSetInterruptMaskSetClearReg(int field_name, int bits);
void uartSetInterruptClearReg(int field_name, int bits);
void uartTransmitData(char data);	
char uartReceiveData();
void setBit(reg rgt, int n);
void clearBit(reg rgt, int n);
void writeBits(reg rgt, int bits, int n);
int  readBits(reg rgt, int n_read, int position);

int main(){
	printf("Holi soy el driver de UART (:D)\n");
	return 0;
}

// Initialize pointers: performs memory mapping, exits if mapping fails
void uartInit(int baudrate){
	int fd = -1;
	// Loading /dev/mem into a file
	if ((fd = open("/dev/mem", O_RDWR, 0)) == -1) err(1, "Error opening /dev/mem");
	// Mapping GPIO base physical address
	gpio_rg = (unsigned int*) mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	// Mapping UART base physical address
	data_rg = (unsigned int*) mmap(0, getpagesize(), PROT_WRITE | PROT_READ, MAP_SHARED, fd, UART_BASE);
	// Check for mapping errors
	if (gpiobase == MAP_FAILED) errx(1, "Error during mapping GPIO");
	if (data_rg == MAP_FAILED) errx(1, "Error during mapping UART");
	// Setting regs pointers
	gpio_sel1 = gpio_rg + 0x1;
	flags_rg = data_rg + 0x6; //0x18
	int_baudrate_rg = data_rg + 0x9; //0x24
	frac_baudrate_rg = data_rg + 0xA; //0x28
	line_control_rg = data_rg + 0xB; //0x2C
	control_rg = data_rg + 0xC; // 0x30
	interrupt_mask_set_clear_rg = data_rg + 0xE; //0x38
	raw_interrupt_status_rg = data_rg + 0xF; //0x3C
	interrupt_clear_rg = data_rg + 0x11; //0x44
	// Seting gpio pins for working with UART
	*gpio_sel1 = *gpio_sel1 | 0x24000;
	// Configuring UART control registers
	uartSetControlReg(UARTEN, 0x0); // Desabling UART
	uartSetBaudrate(baudrate); // Setting baudrate with the given value
	uartSetControlReg(RXE, 0x1); // Enabling receive
	uartSetControlReg(TXE, 0x1); // Enabling transmit
	uartSetLineControlReg(WLEN, 0x3); // Set word size to 8 bits
	uartSetInterruptClearReg(ALL, SET); // Clear all interrupts
	uartSetInterruptMaskSetClearReg(TXIM, 0x1); // Enable interrupt mask TXIM
	uartSetInterruptMaskSetClearReg(RXIM, 0x1); // Enable interrupt mask RXIM
	uartSetControlReg(UARTEN, 0x1); // Enabling UART
}

// This function transmit data via UART
void uartTransmitData(char c){
	writeBits(data_rg, c, 0);
}

// This function receive data via UART
char uartReceiveData(){
	while(*raw_interrupt_status_rg != 0x20); // While UART is transmiting data, wait
	*raw_interrupt_status_rg |= 0x20;
	return readBits(data_rg, 0x8, 0x0);
}

/*
*	Set functions for configuration registers. It writes bits into the specified field_name macro
*   For field_names information, look at manufacturer data sheet
*/
void uartSetLineControlReg(int field_name, int bits){
	writeBits(line_control_rg, bits, field_name);
}

void uartSetControlReg(int field_name, int bits){
	writeBits(control_rg, bits, field_name);
}

void uartSetInterruptMaskSetClearReg(int field_name, int bits){
	writeBits(interrupt_mask_set_clear_rg, bits, field_name);
}

void uartSetInterruptClearReg(int field_name, int bits){
	writeBits(interrupt_mask_set_clear_rg, bits, field_name);
}

// This function sets a baudrate. It writes it into the specific registers
void uartSetBaudrate(int baudrate){
	float divisor = FUARTCLK / (16 * baudrate);
	unsigned int int_part = (unsigned int) (FUARTCLK / (16 * baudrate));
	unsigned int frac_part = (unsigned int) ((FUARTCLK % (16 * baudrate)) * 64 /(16 * baudrate));
	writeBits(int_baudrate_rg, int_part, 0);
	writeBits(frac_baudrate_rg, frac_part, 0);
}

// This sets to 1 a bit in a specific memory position
void setBit(reg rgt, int n){
	*rgt |= 1 << n;
}

// This sets to 0 a bit in a specific memory position
void clearBit(reg rgt, int n){
	*rgt &= ~(1 << n);
}

// This function gets a binary value expressed by a decimal value from an specific memory position
int readBits(reg rgt, int n_read, int position){
	int data = *rgt, clear = ((int) pow(2,n_read)) - 1; 
	return (data >> position) & clear;
}

// This function writes a binary value expressed by a hexadecimal value into a specific memory position
void writeBits(reg rgt, int bits, int n){
	int current_bit;
	do{
		current_bit = bits & 0x1;
		if(current_bit){
			setBit(rgt, n);
		}else{
			clearBit(rgt, n);
		}
		n++;
	} while(bits = bits >> 1);
}