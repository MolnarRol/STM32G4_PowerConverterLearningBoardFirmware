/*
 * sys_public_interface.h
 *
 *  Created on: Nov 21, 2024
 *      Author: roland
 */

#ifndef SYS_SYSTEMINTERFACE_INTERFACE_SYS_PUBLIC_INTERFACE_H_
#define SYS_SYSTEMINTERFACE_INTERFACE_SYS_PUBLIC_INTERFACE_H_
#ifdef __cplusplus
extern "C"
{
#endif

void SYS_IWDG_Init_v(void);
void SYS_IWDG_FeedTheDog_v(void);

#ifdef __cplusplus
}
#endif
#endif /* SYS_SYSTEMINTERFACE_INTERFACE_SYS_PUBLIC_INTERFACE_H_ */
