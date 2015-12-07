##Tiva C Stabilizer

###Overview

The primary goal of this project is to create a stabilizing device which keeps a platform at a uniform level relative to the earth, despite of the angle of the base.
###Peripherals & Devices
####Servo motors (x2)
Each servo is mechanically connected to a jointed arm. The arms are aligned such that the rotation of each servo rotates the platform in one range of motion. 
####LSM303DLHC Accelerometer & Magnetometer (x1)
The accelerometer is placed on the platform to provide readings about the current platform angle.
####TI TM4C123GH6PM Microcontroller (x1)
All logic and processing is handled.

###Implementation

The Tiva-C sensorlib and driverlib libraries were used for interfacing with the servos and accelerometer.

The servo motors are each controlled with a simple modulated PWM output. Each servo has its own PWM signal. The signal’s duty cycle is varied betweena defined maximum and minimum bounds for its corresponding arm.

The accelerometer data is continuously read from the LSM303DLHC, converted to m/s^2 format and then used in the servo movement calculations. A simple proportional approximation is used in this case to determine how far to move each arm given by the following equation:
next_shift_angle_arm = (accelerometer_axis_a * constant)
	i.e. move the arm by a factor of the amount that the equivalent axis is off center.

###Results

The platform keeps fairly strait for all angles within range. The angle of the platform takes about a third of a second to stabilize. This latency causes objects being supported by the platform to slightly shift if their friction with the surface is too low.

###Issues & Future Improvements

(performance issues)
The final product does experience for minor jitter when attempting to find its desired platform plane angle. This is mainly due to the controller implementation. A solely proportional approach is used for calculating how much to move the servos. To improve this, A Proportional - Integral - Derivative (PID) controller could be implemented as it takes into account the current proportional error, the past error, and the future error of the mechanism. With some variable tweaking a PID controller can be very stable for a device like this.

Another important issue with the system is found when shifting the base in the perpendicular direction to earth's gravity. Because solely the accelerometer was used (see implementation issues below), the device is tricked into thinking the down direction is elsewhere when a orientation shift is made. In the future it would be useful to use the gyroscope, magnetometer, and gyroscope with sensor fusion in order to approximate the real world spatial angle of the platform.

(implementation issues)
One of the primary issues when integrating the sensorlib library was trying to get the LSM303DLHC to return non-zero values for the accelerometer and magnetometer readings. The core library examples did not work without modification and after many hours of debugging it was realized that the sensorlib library for the LSM303DLHC simply does not work. However, with some register modification, the similar implementation for the LSM303D chip works, but only for accelerometer readings.





