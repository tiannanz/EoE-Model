========================================================================
    EoE_Optimization_Model Project Overview
========================================================================

Console application built in C++, VS2015
Using VS PCH (include stdafx.h in files)

========================================================================

Details to add a strategy::

1) add a namespace in strategies.h with your strategy
2) add strategy enumerator name in enum Number_of_Strategies (strategies.h)
3) write strategy in strategies.cpp using other strategies as a template of how
	to add and remove allergies / tests etc. 
4) add strategy to Simulation_Processess::run_PSA_simulation() and to 
Simulation_Processes::run_base_case_simulation(), all you need to do is :
	I) run strategy in the same place as other two strategies and reset patient after
	II) hold_results[strat name from step 2].print_results(filetag) where the other
		strategies do their thing as well

That's it! 