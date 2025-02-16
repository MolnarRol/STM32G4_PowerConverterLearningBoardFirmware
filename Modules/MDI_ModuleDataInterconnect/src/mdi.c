/*
 * mdi.c
 *
 *  Created on: Feb 15, 2025
 *      Author: molnar
 */
#include "mdi.h"

MDI_INTERCONNECT_STRUCT_FACTORY_d(u8);

volatile boolean* MDI_Signal_TopologyActive_b = NULL;

