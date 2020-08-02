int main(int argc, char const *argv[])
{
	if (strcmp(argv[1], "uartInit") == 0)
	{
		uartInit(9600);
	} else if (strcmp(argv[1], "uartReceiveData") == 0){
		printf("%c", uartReceiveData());
	} else if (strcmp(argv[1], "uartTransmitData") == 0){
		uartTransmitData((char)*argv[2]);
	}
	return 0;
}
