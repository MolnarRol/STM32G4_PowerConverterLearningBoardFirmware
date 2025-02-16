/*
 * MDI_public_interface.h
 *
 *  Created on: Feb 15, 2025
 *      Author: molnar
 */

#ifndef MDI_MODULEDATAINTERCONNECT_INTERFACE_MDI_PUBLIC_INTERFACE_H_
#define MDI_MODULEDATAINTERCONNECT_INTERFACE_MDI_PUBLIC_INTERFACE_H_

#define MDI_INTERCONNECT_STRUCT_FACTORY_d(TYPE) \
struct                                  \
{                                               \
    TYPE    val;                                \
    TYPE*   dst_p;                              \
} MDI_Interconnect_ ## TYPE ## _s;

#define MDI_WriteInteconnect_d(type, destination_pv, value)   if(destination_pv != NULL) *(type*)destination_pv = value;

extern volatile boolean* MDI_Signal_TopologyActive_b;


#endif /* MDI_MODULEDATAINTERCONNECT_INTERFACE_MDI_PUBLIC_INTERFACE_H_ */
