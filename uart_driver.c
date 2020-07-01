#include <stdio.h>
#include <sys/mman.h> //mmap
#include <err.h> //error handling
#include <fcntl.h> //file ops
#include <unistd.h> //usleep
#include <string.h>

#define SPS 7
#define WLEN 5
#define FEN 4
#define STP2 3
#define EPS 2
#define PEN 1
#define BRK 0

typedef volatile unsigned int * reg;
//Static base
static unsigned UART_BASE = 0x7E20100;
//Regs pointers
reg data_rg;
reg line_control_rg;
/*Function prototypes*/
void uartInit();
void uartSendChar(char c);
void uartSetControlRg(int field_name, int bits);
void setBit(int * rgt, int n);
void clearBit(int * rgt, int n);
void writeBits(int * rgt, int bits, int n);

int main(){
	int r = 7;
	writeBits(&r, 0x2, STP2);
	printf("%i\n", r);
	return 0;
}

//Initialize pointers: performs memory mapping, exits if mapping fails
void uartInit(){
	/*
	int fd = -1;
	//Loading /dev/mem into a file
	if ((fd = open("/dev/mem", O_RDWR, 0)) == -1) err(1, "Error opening /dev/mem");
	//Mapping GPIO base physical address
	data_rg = (unsigned int*) mmap(0, getpagesize(), PROT_WRITE | PROT_READ, MAP_SHARED, fd, GPIO_BASE);
	//Check for mapping errors
	if (data_rg == MAP_FAILED) errx(1, "Error during mapping UART");
	//Set regs pointers
	line_control_rg = data_rg + 0x2C;
	*/
}

// This funtion writes a char to be sent into the data register
void uartSendChar(char c){
	writeBits((int *) data_rg, c, 0);
}

/* 
*	This set the line control register configuration bits into the specified field name macro. 
*   For field names information, look at manufacturer data sheet
*/
void uartSetControlRg(int field_name, int bits){
	writeBits((int *) line_control_rg, bits, field_name);
}

// This sets to 1 a bit in a specific memory position
void setBit(int * rgt, int n){
	*rgt |= 1 << n;
}

// This sets to 0 a bit in a specific memory position
void clearBit(int * rgt, int n){
	*rgt &= ~(1 << n);
}

// This function writes a binary value expressed by a hexadecimal value into a specific memory position
void writeBits(int * rgt, int bits, int n){
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