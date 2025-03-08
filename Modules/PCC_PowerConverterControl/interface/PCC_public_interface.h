/*
 * PCC_public_interface.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_
#define PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    PCC_TOPO_SinglePWM_e                                = 0,
    PCC_TOPO_SingleComplementaryPWM_e                   = 1,
    PCC_TOPO_FullBridgePhaseShiftedPWM_e                = 2,
    PCC_TOPO_FullBridgeUnipolarPWM_e                    = 3,
    PCC_TOPO_FullBridgeBipolarPWM_e                     = 4,
    PCC_TOPO_SinglePhaseUnipolarSinePWM_e               = 5,
    PCC_TOPO_SinglePhaseBipolarSinePWM_e                = 6,
    PCC_TOPO_ThreePhaseSinePWM_e                        = 7,
    PCC_TOPO_SinglePhaseControlledRectifier_e           = 8,
    PCC_TOPO_SinglePhaseThyristorVoltageConverter_e     = 9,
    PCC_TOPO_SinglePhaseTriacVoltageConverter_e         = 10,
    PCC_TOPO_DoubleImpulseControlledRectifier_e         = 11,
    PCC_TOPO_SixPulseThreePhaseControlledRectifier_e    = 12,
    PCC_TOPO_ThreePulseThreePhaseControlledRectifier_e  = 13
} PCC_Topologies_enum;

typedef enum
{
    PCC_ParamType_PWM_e,
    PCC_ParamType_ComplementaryPWM_e,
    PCC_ParamType_PhaseShiftedPWM_e,
    PCC_ParamType_SinePWM_e,
    PCC_ParamType_LineCommutated_e
} PCC_ParamType_enum;


typedef struct
{
    volatile f32    val_f32;
    const f32       min_f32;
    const f32       max_f32;
} PCC_Param_struct;

typedef struct {
    const PCC_ParamType_enum    type_e;
    union {
        struct {
            PCC_Param_struct    frequency__Hz__s;
            PCC_Param_struct    duty_cycle__per_cent__s;
        } PWM_struct;

        struct {
            PCC_Param_struct    frequency__Hz__s;
            PCC_Param_struct    duty_cycle__per_cent__s;
            PCC_Param_struct    dead_time__s__s;
        } ComplementaryPWM_struct;

        struct {
            PCC_Param_struct    frequency__Hz__s;
            PCC_Param_struct    phase_shift__deg__s;
            PCC_Param_struct    dead_time__s__s;
        } PhaseShiftedPWM_struct;

        struct {
            PCC_Param_struct    switching_frequency__Hz__s;
            PCC_Param_struct    modulation_frequency__Hz__s;
            PCC_Param_struct    amplitude__per_cent__s;
            PCC_Param_struct    dead_time__s__s;
        } SinePWM_struct;

        struct {
            PCC_Param_struct    firing_angle__deg__s;
            PCC_Param_struct    pulse_len__deg__s;
        } LineCommutation_struct;

        PCC_Param_struct        data_as[4U];
    };
} PCC_Params_struct;

boolean PCC_InitializeActiveTopology_b(void);
boolean PCC_DeinitializeActiveTopology_v(void);
boolean PCC_StartActiveTopology_v(void);
boolean PCC_StopActiveTopology_v(void);

void PCC_Handler_v(void);
void PCC_InterruptHandler_v(void);
PCC_Params_struct* PCC_GetActiveTopologyParameters_ps(void);

#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_ */
