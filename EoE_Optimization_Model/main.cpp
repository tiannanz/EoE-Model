#include "stdafx.h"
#include "strategies.h"
#include <ctime>
#include "global_parameters.h"



int main()
{	
	clock_t start_time = clock();
	printf("Beginning EoE simulation.");

	if (Parameters::RUN_PSA) Simulation_Processes::run_PSA_simulation();
	else Simulation_Processes::run_base_case_simulation();
	
	clock_t end_time = clock();
	double total_time = double(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTotal simulation time was %f seconds\n", total_time);

    return 0;
}

