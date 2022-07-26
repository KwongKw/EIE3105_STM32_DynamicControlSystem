#include "wheel.h"
#include "uart2.h"
#include <stdio.h>

// string to USART2
char string[80];

// 400 Hz * 4 sec. = 1,600 (overall "NoInit" RAM size = 16,376 bytes)
// "NoInit" RAM = the RAM that will not be cleared after reset
// This value is also determined when the car will stop. Now it is set to stop after 4 seconds
#define SIZE 1600 

// values written in isr MIGHT be overwritten by main()

// an array to store the counter values
unsigned char data[SIZE] __attribute__((section( "NoInit"),zero_init));

// an array to store the PWM of the left wheel
unsigned short d_left[SIZE] __attribute__((section( "NoInit"),zero_init));

// an array to store the PWM of the right wheel
unsigned short d_right[SIZE] __attribute__((section( "NoInit"),zero_init));

volatile int w_index, r_index;

// ********************************
// write your control function to another file
// input: left, right (the counter values of two wheels)
// output: left, right (the PWM values of two wheels)
// ********************************

void control(int &left, int &right);

int main(void) 
{
	while (1) 
	{
		// variables to store counter values and PWM values
		int left, right;

		// get the counter values and return true in every 2.5 ms
		if (speed(left, right)) 
		{
			// pack left and right to a single byte
			// left = upper 4 bits, right = lower 4 bits
			// maximum values of left and right = 15
			if (w_index) data[--w_index] = (left << 4) + right;

			// a control function to move the car along a straight line
			control(left, right);

			// make sure left and right will not be over the max. value
			if (left > 44999) left = 44999;
			if (right > 44999) right = 44999;

			// the car will be stopped when w_index = 0
			left = w_index ? left : 0;
			right = w_index ? right : 0;

			// assign the PWM values to the motor
			motor(left, right);

			if (w_index) 
			{
				// store PWM values to d_left and d_right
				d_left[w_index] = left;
				d_right[w_index] = right;
			}

			// the on-board LED shows the status
			w_index ? led_on() : led_off();

		}

		// variables used in the following if-statement
		unsigned short lt, rt;

		// dump data to USART2
		if (r_index) 
		{
			// Do it when USART2 is not busy
			if (!usart2_printBusy()) 
			{
				// unpack PWM values to lt and rt
				lt = d_left[--r_index];
				rt = d_right[r_index];

				// unpack counter values to left1 and right1
				int left1 = data[r_index];
				int right1 = left1 & 15;
				left1 >>= 4;
				// left counter, right counter, left PWM, right PWM
				sprintf(string, "%d %d %d %d\r\n", left1, right1, lt, rt);

				// send the string to USART2
				usart2_printString(string);
			}
		}
	}
}

// The switch state is sampled every 2.5 ms in the background (i.e., TIM3 interrupt)
// The function below is called when a change is detected (i.e., call by TIM3 interrupt).

void button_changed(int new_button_state) 
{
	// do something when the on-board button is pressed
	static bool r;
	if (new_button_state) 
	{
		w_index = (r = !r) ? SIZE : 0;	// button up: toggle state
	}
	else 
		w_index = 0; // button down: stop the car
}

// This function is to print data to USART2 when the character 'd' is received
void usart2_receive(char c) 
{
	static char buffer[256], *ptr = buffer;
	buffer[0] = ' ';

	// ignore newline
	if (c == '\n') return;

	// ignore trim leading space and compress space
	if (c == ' ') 
		if (*ptr == ' ') 
			return;

	*++ptr = c;

	// ignore carriage return
	if (c != '\r') return;

	*ptr = 0; 
	ptr = &buffer[1];
	int v = 0;

	// read an integer
	while ((*ptr >= '0') && (*ptr <= '9')) v = v * 10 + *ptr++ - '0';
	switch (*ptr) {
		case 'd': r_index = v ? v : SIZE;	break;		// dump data
		default:;
	}
	ptr = buffer;
}
