/*
 * PCC_private_interface.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_
#define PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <app_types.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_tim.h>
#include <stm32g4xx_ll_dma.h>
#include <PCC_public_interface.h>
#include <stdio.h>
#include <stm32_assert.h>
#include <ATB_public_interface.h>

typedef enum
{
	PCC_UNINITIALIZED_e		= 0,
	PCC_INITIALIZATION_e,
	PCC_INITIALIZED_e,
	PCC_ACTIVE_e,
	PCC_UNINITIALIZATION_e
} PCC_TopologyHandleState_enum;

#define PCC_NUMBER_OF_TOPOLOGIES_d              2U

typedef enum
{
	PCC_TOPO_SimplePWM_e	                = 0,
	PCC_TOPO_FullBridgePhaseShiftedPWM_e    = 1
} PCC_Topologies_enum;

#define PCC_IS_VALID_TOPOLOGY(topology_e)	(topology_e >= 0) && (topology_e < PCC_NUMBER_OF_TOPOLOGIES_d)

typedef struct
{
	const PCC_Topologies_enum 			c_topology_e;
    void (*const initialize_pfv)		(void);
    void (*const initialized_pfv)       (void);
    void (*const start_pf)				(void);
    void (*const active_handler_pfv)	(void);
    void (*const stop_pfv)				(void);
    void (*const deinitalize_pfv)		(void);
    const PCC_driver_enable_union 		driver_enable_u;
} PCC_TopologyHandle_struct;

extern const PCC_TopologyHandle_struct PCC_Topology_SimplePWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_FullBridgePhaseShiftedPWM_s;

#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_ */
