#ifndef WHEEL_H
#define WHEEL_H

// get the counter values from the left and right wheels
// and store them into the variables left and right
// return true once when the values are obtained in every 2.5 ms; otherwise, it returns false
bool speed(int &left, int &right);  

// set the PWM of two wheels, the maximum value is 44,999 (100% PWM)
// left = the PWM of the left wheel
// right = the PWM of the right wheel
void motor(int left, int right);

void led_on(void); // turn on the on-board LED
void led_off(void); // turn off the on-board LED
void led_toggle(void); // toggle the on-board LED (change the state)

#endif // WHEEL_H
