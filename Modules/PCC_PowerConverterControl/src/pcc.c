/*
 * pcc.c
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */
#include <pcc.h>


PCC_TopologyHandleState_enum PCC_TopologyState_e = PCC_UNINITIALIZED_e;
PCC_Topologies_enum	PCC_ActiveTopology_e = PCC_TOPO_SinglePWM_e;

const PCC_TopologyHandle_struct* const PCC_Topologies_as[] =
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

boolean pcc_start_test_b = False_b;

RAM_FUNC void PCC_Handler_v(void)
{
	static boolean switch_delay_active_b = False_b;
	static u32 switch_delay_active_timestamp_u32;
	switch(PCC_TopologyState_e)
	{
		case PCC_UNINITIALIZED_e:
		{
			break;
		};

		case PCC_INITIALIZATION_e:
		{
		    PCC_Topologies_as[PCC_ActiveTopology_e]->initialize_pfv();
			PCC_TopologyState_e = PCC_INITIALIZED_e;
		};

		case PCC_INITIALIZED_e:
		{
		    PCC_Topologies_as[PCC_ActiveTopology_e]->active_handler_pfv();
		    if(pcc_start_test_b)
		    {
		        PCC_TopologyState_e = PCC_ACTIVE_e;
		        switch_delay_active_b = True_b;
		        PCC_SetGateDriverPowerStates(PCC_Topologies_as[PCC_ActiveTopology_e]->driver_enable_u);
		        switch_delay_active_timestamp_u32 = ATB_GetCurrentTicks_u32();
		    }
			break;
		}

		case PCC_ACTIVE_e:
		{
		    if(switch_delay_active_b)
		    {
		        if(ATB_CheckIfPeriodHasElapsed_b(&switch_delay_active_timestamp_u32, ATB__ms__TO__ticks__du32(500)))
		        {
		            switch_delay_active_b = False_b;
	                PCC_Topologies_as[PCC_ActiveTopology_e]->start_pf();
		        }
		    }
		    else
		    {
                PCC_Topologies_as[PCC_ActiveTopology_e]->active_handler_pfv();
                if(!pcc_start_test_b)
                {
                    PCC_Topologies_as[PCC_ActiveTopology_e]->stop_pfv();
                    PCC_TopologyState_e = PCC_INITIALIZED_e;
                    const PCC_driver_enable_union stop_drivers_s = {.byte_val_u8 = 0};
                    PCC_SetGateDriverPowerStates(stop_drivers_s);
                }
		    }
			break;
		}

		case PCC_UNINITIALIZATION_e:
		default:
		{
		    const PCC_driver_enable_union stop_drivers_s = {.byte_val_u8 = 0};
		    PCC_SetGateDriverPowerStates(stop_drivers_s);
		    PCC_Topologies_as[PCC_ActiveTopology_e]->deinitalize_pfv();
			PCC_TopologyState_e = PCC_UNINITIALIZED_e;
			break;
		}
	}
}

void PCC_InterruptHandler_v(void)
{
    assert_param(PCC_Topologies_as[PCC_ActiveTopology_e]->isr_handler_pfv != NULL);
    PCC_Topologies_as[PCC_ActiveTopology_e]->isr_handler_pfv();
}

boolean PCC_SetTopology_b(PCC_Topologies_enum topology_e)
{
	boolean return_status_b = False_b;
	if((PCC_TopologyState_e == PCC_UNINITIALIZED_e) && PCC_IS_VALID_TOPOLOGY(topology_e))
	{
		PCC_ActiveTopology_e = topology_e;
		PCC_TopologyState_e = PCC_INITIALIZATION_e;
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
