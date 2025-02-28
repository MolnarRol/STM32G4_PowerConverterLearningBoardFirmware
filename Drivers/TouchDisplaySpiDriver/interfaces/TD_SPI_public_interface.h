/*
 * TD_SPI_public_interface.h
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */

#ifndef TOUCHDISPLAYSPIDRIVER_INTERFACES_TD_SPI_PUBLIC_INTERFACE_H_
#define TOUCHDISPLAYSPIDRIVER_INTERFACES_TD_SPI_PUBLIC_INTERFACE_H_

void TD_SPI_Init_v(void);
void TD_SPI_TransferInBlockingMode_v(u8* data_pu8, u32 n_of_bytes_u32);

#endif /* TOUCHDISPLAYSPIDRIVER_INTERFACES_TD_SPI_PUBLIC_INTERFACE_H_ */
