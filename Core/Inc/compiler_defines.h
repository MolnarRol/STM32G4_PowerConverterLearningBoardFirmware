/*
 * compiler_defines.h
 *
 *  Created on: Feb 10, 2025
 *      Author: molnar
 */

#ifndef INC_COMPILER_DEFINES_H_
#define INC_COMPILER_DEFINES_H_

#define INLINE			inline
#define FORCE_INLINE	inline __attribute__((always_inline))
#define RAM_FUNC		__attribute__((section(".RamFunc")))



#endif /* INC_COMPILER_DEFINES_H_ */
