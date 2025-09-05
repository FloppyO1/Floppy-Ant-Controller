/*
 * fac_mapper.c
 *
 *	HOW MAPPER WORKS:
 *	The mapper reads from the settings which device is connected
 *	to which output of the active mix or special function, and sets
 *	the values ​​of the various outputs to the corresponding devices.
 *	Mix outputs use the prefix 100 to identify them, while special
 *	functions use the prefix 200. (e.g., m1 = 102, motor 1 takes
 *	the value of output 2 of the active mix.)
 *	The mapper takes the active outputs from the active mixes or
 *	functions and conditions the value of the given output
 *	(in standard format) to be understandable by the devices.
 *	If the special function has input channel = 0, it is disabled
 *	and should not be calculated.
 *
 *	The mapper calculates the outputs at the time of the update call,
 *	and for each peripheral (if active) it conditions the outputs
 *	and applies them to the peripheral.
 *
 *
 *  Created on: Sep 5, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_mapper.h"


