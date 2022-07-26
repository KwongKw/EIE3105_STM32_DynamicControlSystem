#include <stdlib.h>
#include <math.h>

// PI controller gain value, left wheel
float k_p_l = 562.72, k_i_l = 562;
// PI controller gain value, right wheel
float k_p_r = 562.72, k_i_r = 562;
// PI controller gain value, speed difference
float k_p_o = 0.5043, k_i_o = 0.5037;
// Error value reapeatly used store outside the control function
int left_i = 0, right_i = 0, out_i = 0;
// Sum of the encoder value
int left_sum = 0, right_sum = 0, out_sum = 0;
// Store the PWM value
int left_pwm = 0, right_pwm = 0, diff = 0;
// Counter
int ctr0 = 1, ctr1 = 1;
// Target wheel encoder, subject to be the same
const int target_left = 28, target_right = 28;


void control(int &left, int &right)
{
	int left_e = 0, right_e = 0, out_e = 0;
	
	// PI controller for each wheel
	// Sum the encoder for each 20*2.5ms = 50ms
	// For every 50ms, execute the PI correction once on both wheel
	if (ctr0 < 20)
	{
		left_sum += left;
		right_sum += right;
		ctr0++;
	}
	else if (ctr0 == 20)
	{
		left_sum += left;
		right_sum += right;
		out_sum = left_sum - right_sum;
		
		// Proportional
		left_e = target_left + diff - left_sum;
		right_e = target_right - diff - right_sum;
		out_e = 0 - out_sum;
		
		// Integral
		left_i += left_e;
		right_i += right_e;
		out_i += out_e;
		
		if ((left_e == 0) || (abs(left_e) > 10))
			left_i = 0;
		
		if ((right_e == 0) || (abs(right_e) > 10))
			right_i = 0;
		
		if ((out_e == 0) || (abs(out_e) > 10))
			out_i = 0;
		
		// Output
		left_pwm = k_p_l * left_e + k_i_l * left_i;
		right_pwm = k_p_r * right_e + k_i_r * right_i;
		diff = k_p_o * out_e + k_i_o * out_i;
		
		ctr1++;
		ctr0 = 1;
		left_sum = 0;
		right_sum = 0;
		out_sum = 0;
	}
	
	// PI controller for rev. matching
	// Check difference
	
	
	
	
	// Default speed 15000
	//left = 15000;
	//right = 15000;
	
	// Unit-step speed
	//left = 44999;
	//right = 44999;
		
	// PID adjusted output
	left = left_pwm;
	right = right_pwm;
	
	
	
	// Reset all stored value when 4s finish
	if (ctr1 > 80)
	{
		left_i = 0;
		right_i = 0;
		out_i = 0;
		left_pwm = 0;
		right_pwm = 0;
		diff = 0;
		ctr1 = 0;
	}
}
