/*
 * pcc.c
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */
#include <pcc.h>

#define _INIT_TOPOLOGY_dpf              (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->initialize_pfv)
#define _START_TOPOLOGY_dpf             (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->start_pf)
#define _STOP_TOPOLOGY_dpf              (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->stop_pfv)
#define _DEINIT_TOPOLOGY_dpf            (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->deinitalize_pfv)
#define _ISR_HANDLER_TOPOLOGY_dpf       (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->isr_handler_pfv)
#define _DRIVER_EN_ds                   (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->driver_enable_u)
#define _PARAMS_dps                     (s_PCC_Topologies_aps[s_PCC_ActiveTopology_e]->ctrl_params_pv)

static PCC_TopologyHandleState_enum     s_PCC_TopologyState_e = PCC_UNINITIALIZED_e;
static PCC_Topologies_enum	            s_PCC_ActiveTopology_e = PCC_TOPO_SinglePWM_e;
const PCC_TopologyHandle_struct* const s_PCC_Topologies_aps[] =
{
    &PCC_Topology_SinglePWM_s,
    &PCC_Topology_SingleComplementaryPWM_s,
    &PCC_Topology_FullBridgePhaseShiftedPWM_s,
    &PCC_Topology_FullBridgeUnipolarPWM_s,
    &PCC_Topology_FullBridgeBipolarPWM_s,
    &PCC_Topology_SinglePhaseUnipolarSinePWM_s,
    &PCC_Topology_SinglePhaseBipolarSinePWM_s,
    &PCC_Topology_ThreePhaseSinePWM_s,
    &PCC_Topology_SingleImpulseControlledRectifier_s,
    &PCC_Topology_SinglePhaseThyristorVoltageConverter_s,
    &PCC_Topology_SinglePhaseTriacVoltageConverter_s,
    &PCC_Topology_DoubleImpulseControlledRectifier_s,
    &PCC_Topology_SixPulseThreePhaseControlledRectifier_s,
    &PCC_Topology_ThreePulseThreePhaseControlledRectifier_s
};

boolean PCC_InitializeActiveTopology_b(void)
{
    assert_param(_INIT_TOPOLOGY_dpf != NULL);
    if(s_PCC_TopologyState_e != PCC_UNINITIALIZED_e)
        return False_b;

    _INIT_TOPOLOGY_dpf();
    s_PCC_TopologyState_e = PCC_INITIALIZED_e;
    return True_b;
}

boolean PCC_DeinitializeActiveTopology_v(void)
{
    assert_param(_DEINIT_TOPOLOGY_dpf != NULL);
    if(s_PCC_TopologyState_e != PCC_INITIALIZED_e)
        return False_b;

    _DEINIT_TOPOLOGY_dpf();
    s_PCC_TopologyState_e = PCC_UNINITIALIZED_e;
    return True_b;
}

boolean PCC_StartActiveTopology_v(void)
{
    assert_param(_START_TOPOLOGY_dpf != NULL);
    if(s_PCC_TopologyState_e != PCC_INITIALIZED_e)
        return False_b;

    s_PCC_TopologyState_e = PCC_ACTIVE_e;
    return True_b;
}

boolean PCC_StopActiveTopology_v(void)
{
    assert_param(_STOP_TOPOLOGY_dpf != NULL);
    if(s_PCC_TopologyState_e != PCC_ACTIVE_e)
        return False_b;

    _STOP_TOPOLOGY_dpf();
    PCC_SetGateDriverPowerStates((PCC_driver_enable_union){.byte_val_u8 = (u8)0});
    s_PCC_TopologyState_e = PCC_INITIALIZED_e;
    return True_b;
}

void PCC_InterruptHandler_v(void)
{
    assert_param(_ISR_HANDLER_TOPOLOGY_dpf != NULL);
    _ISR_HANDLER_TOPOLOGY_dpf();
}

boolean PCC_SetTopology_b(PCC_Topologies_enum topology_e)
{
	boolean return_status_b = False_b;
	assert_param(topology_e < (sizeof(s_PCC_Topologies_aps) / sizeof(s_PCC_Topologies_aps[0])));
	if(s_PCC_TopologyState_e == PCC_UNINITIALIZED_e)
	{
		s_PCC_ActiveTopology_e = topology_e;
		return_status_b = True_b;
	}
	return return_status_b;
}

void PCC_SetGateDriverPowerStates(PCC_driver_enable_union enable_states_u)
{
	/* PC0 - PC3 */
	GPIOC->BSRR				|=	(1UL << (16UL * !enable_states_u.drivers_s.gd1_f1))			|
								(1UL << (1UL + 16UL * !enable_states_u.drivers_s.gd2_f1))	|
								(1UL << (2UL + 16UL * !enable_states_u.drivers_s.gd3_f1))	|
								(1UL << (3UL + 16UL * !enable_states_u.drivers_s.gd4_f1));
	/* PB2 */
	GPIOB->BSRR				|= 1UL << (2UL + 16UL * !enable_states_u.drivers_s.gd5_f1);

	/* PA5 */
	GPIOA->BSRR				|= 1UL << (5UL + 16UL * !enable_states_u.drivers_s.gd6_f1);
}

void PCC_CheckAndCorrectIncorrectParameters_v(void)
{
    register u32            iteration_iu32;
    PCC_Param_struct*       data_ps;
    for(iteration_iu32 = (u32)0; iteration_iu32 < 4U; iteration_iu32 += (u32)1) {
        data_ps = &_PARAMS_dps->data_as[iteration_iu32];

        if(data_ps->val_f32 > data_ps->max_f32)
            data_ps->val_f32 = data_ps->max_f32;
        else if(data_ps->val_f32 < data_ps->min_f32)
            data_ps->val_f32 = data_ps->min_f32;
    }
}

PCC_Params_struct* PCC_GetActiveTopologyParameters_ps(void)
{
    return (PCC_Params_struct*)_PARAMS_dps;
}

RAM_FUNC void PCC_Handler_v(void)
{
  static boolean s_switch_delay_active_b = False_b;
  static u32 s_switch_delay_active_timestamp_u32;

  if(s_PCC_TopologyState_e == PCC_ACTIVE_e)
  {
      if(s_switch_delay_active_b)
      {
          if(ATB_CheckIfPeriodHasElapsed_b(&s_switch_delay_active_timestamp_u32,  ATB__ms__TO__ticks__du32(500)))
          {
              _START_TOPOLOGY_dpf();
              s_switch_delay_active_b = False_b;
          }
      }
      else
      {
          s_switch_delay_active_b = True_b;
          s_switch_delay_active_timestamp_u32 = ATB_GetCurrentTicks_u32();
          PCC_SetGateDriverPowerStates(_DRIVER_EN_ds);
      }
  }
}
