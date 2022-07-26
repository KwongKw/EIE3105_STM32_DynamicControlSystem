# EIE3105_STM32_DynamicControlSystem
Performing PID control on a two wheel robot car


## Equipment:

●	uVision (software) 

●	Robot Car (hardware)

●	STM32F103RBT6 (hardware) 

![image](https://user-images.githubusercontent.com/56528924/181018427-ff3ab6f5-72cf-4123-9401-07d7cbf9c87b.png)
![image](https://user-images.githubusercontent.com/56528924/181018453-8d2f6ed7-a94e-4e12-a2eb-6599629ff936.png)

## The product of the system.

![image](https://user-images.githubusercontent.com/56528924/181018587-b9dd3608-d441-437b-be58-86611049fed6.png)

// Uncompensated System

![image](https://user-images.githubusercontent.com/56528924/181018690-2cca9c5e-979d-4259-bc39-ced4ce5e8e54.png)

// Compensated System

![image](https://user-images.githubusercontent.com/56528924/181018738-f33a3a40-446b-4dff-b5c5-793a34cc2f69.png)

## Program Concept

### // Initialization

#### // PI controller gain value, left wheel

float k_p_l = 562.72, k_i_l = 562;

#### // PI controller gain value, right wheel

float k_p_r = 562.72, k_i_r = 562;

#### // PI controller gain value, speed difference

float k_p_o = 0.5043, k_i_o = 0.5037;


### // Output

left_pwm = k_p_l * left_e + k_i_l * left_i;

right_pwm = k_p_r * right_e + k_i_r * right_i;

diff = k_p_o * out_e + k_i_o * out_i;	


### // Proportional

left_e = target_left + diff - left_sum;

right_e = target_right - diff - right_sum;

out_e = 0 - out_sum;


### // Integral

left_i += left_e;

right_i += right_e;

out_i += out_e;


### // Derivative (NOT needed)

