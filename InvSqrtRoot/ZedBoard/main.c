/*
 * main.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include <stdlib.h>

int getResult(float in, float* out);



/**
 *  printDecimalFXPVal - print fixed-point value in decimal format
 *  val - value to print out in radix-2 fixed-point
 *  scale - Fixed-point scaling factor
 *	nbr_of_decimal_digit - number precision. The number of digits after decimal point
 */

void printDecimalFXPVal(float number )
{
	xil_printf("%f", number );
}

float readFloat(){
    float ret;
    char8 c;
    char buffer[64];
    char number[9]={};
    uint8_t i;

    int flag = 0;
    for(i=0; i<8; i++)
    {
    	outbyte ( c = inbyte() );
    	if(c == '.')
    	        {
    	            if(flag == 1) break;
    	            flag = 1;
    	        number[i] = c;
    	        }
    	        else if ( c >= '0' && c <= '9')
    	        {
    	            number[i] = c;
    	        }
    	        else break;
    	        }
    print("\n\r");
    print("Input is: ");
    ret = (float) atof(number);
    snprintf(buffer, 64, "%f", ret);
    print(buffer);
    print("\n\r");

    return ret;
}

int main()
{
	float in = 0;
	float out;


    init_platform();

    while(1){
    	print("Enter input as floating point with 8 characters:");
    	in = readFloat();



    	print("\n\r");

    	getResult(in, &out);


    	print("The result is ");
    	printDecimalFXPVal(out);

    	print("\n\r");
    	print("\n\r");
    }

}
