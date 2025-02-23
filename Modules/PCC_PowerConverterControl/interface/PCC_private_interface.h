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
#include <general_config.h>
#include <UTIL_public_interface.h>

#include <stm32g4xx.h>
#include <stm32g4xx_ll_tim.h>
#include <stm32g4xx_ll_dma.h>
#include <PCC_public_interface.h>
#include <stdio.h>
#include <stm32_assert.h>
#include <ATB_public_interface.h>

#define ARM_MATH_CM4
#include <arm_math.h>

#define PI_d                        3.141592653589793f
#define TWO_PI_d                    6.283185307179586f

typedef enum
{
	PCC_UNINITIALIZED_e		= 0,
	PCC_INITIALIZATION_e,
	PCC_INITIALIZED_e,
	PCC_ACTIVE_e,
	PCC_UNINITIALIZATION_e
} PCC_TopologyHandleState_enum;

typedef enum
{
	PCC_TOPO_SinglePWM_e	                        = 0,
	PCC_TOPO_SingleComplementaryPWM_e               = 1,
	PCC_TOPO_FullBridgePhaseShiftedPWM_e            = 2,
	PCC_TOPO_FullBridgeUnipolarPWM_e                = 3,
	PCC_TOPO_FullBridgeBipolarPWM_e                 = 4,
	PCC_TOPO_SinglePhaseUnipolarSinePWM_e           = 5,
	PCC_TOPO_SinglePhaseBipolarSinePWM_e            = 6,
	PCC_TOPO_ThreePhaseSinePWM_e                    = 7,
	PCC_TOPO_SinglePhaseControlledRectifier_e       = 8,
	PCC_TOPO_SinglePhaseThyristorVoltageConverter_e = 9,
	PCC_TOPO_SinglePhaseTriacVoltageConverter_e     = 10,
	PCC_TOPO_DoubleImpulseControlledRectifier_e     = 11
} PCC_Topologies_enum;

#define PCC_NUMBER_OF_TOPOLOGIES_d          8U
#define PCC_IS_VALID_TOPOLOGY(topology_e)	(topology_e >= 0) && (topology_e < PCC_NUMBER_OF_TOPOLOGIES_d)

typedef struct
{
	const PCC_Topologies_enum 			    c_topology_e;
    void (*const initialize_pfv)		    (void);
    void (*const initialized_pfv)           (void);
    void (*const start_pf)				    (void);
    void (*const active_handler_pfv)	    (void);
    void (*const stop_pfv)				    (void);
    void (*const deinitalize_pfv)		    (void);
    void (*const isr_handler_pfv)           (void);
    const PCC_driver_enable_union 		    driver_enable_u;
} PCC_TopologyHandle_struct;

typedef struct
{
    f32 amplitude_f32;
    f32 modulation_freq__Hz__f32;
    f32 switching_freq__Hz__f32;
} PCC_SinePWM_Parameters_s;

typedef struct
{
    f32 frequency__Hz__f32;
    f32 duty__per_cent__f32;
    f32 deadtime__s__f32;
} PCC_FC_PWM_Params_s;

typedef struct
{
    f32 alpha__deg__f32;
    f32 pulse_len__deg__f32;
} PCC_LC_CommutationParams_s;

extern const PCC_TopologyHandle_struct PCC_Topology_SinglePWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_FullBridgePhaseShiftedPWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_FullBridgeUnipolarPWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_FullBridgeBipolarPWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SingleComplementaryPWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseUnipolarSinePWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseBipolarSinePWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_ThreePhaseSinePWM_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SingleImpulseControlledRectifier_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseThyristorVoltageConverter_s;
extern const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseTriacVoltageConverter_s;
extern const PCC_TopologyHandle_struct PCC_Topology_DoubleImpulseControlledRectifier_s;

void PCC_InterruptHandler_v(void);

#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_ */
