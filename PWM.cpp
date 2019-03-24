/*
 * AD7705 test program for the Raspberry PI
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * Copyright (c) 2013-2018  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "gpio-sysfs.h"
#include "gpio-sysfs.cpp"
#include <cmath>
#include <iostream>
using namespace std;

unsigned int drdy_GPIO = 22;

//Function declaration of PWM
int *Motor_PWM(unsigned int frequency,double duty)
{
static int Timeinterval[2];
Timeinterval[0]=floor((1000000/frequency)*(duty));
Timeinterval[1]=floor((1000000/frequency)*(1-duty));
return Timeinterval;
}

int main()
{
//Setting GPIO 22 as output
gpio_export(drdy_GPIO);
gpio_set_dir(drdy_GPIO,1);
gpio_set_value(drdy_GPIO,1);

unsigned int freq;
freq=1;
double d;
d=0.75;
int *duration;

while(1)
{
duration=Motor_PWM(freq,d);
gpio_set_value(drdy_GPIO,1);
usleep(*duration);
gpio_set_value(drdy_GPIO,0);
usleep(*(duration+1));
}
return 0;
}
