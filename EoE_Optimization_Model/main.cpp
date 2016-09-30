#include "stdafx.h"
#include "strategies.h"
#include "simulation.h"
#include "results.h"
#include <ctime>



int main()
{	
	clock_t start_time = clock();
	printf("Beginning EoE simulation.");

	/*will include a loop enclosing later for sensitivity analysis*/
	/*the loop will vary the probabilities*/
	Probabilities probs;

	Strategy_Results sr;
	std::vector<Strategy_Results> hold_results;
	for (int i = 0; i < NUMBER_OF_STRATEGIES; i++) hold_results.push_back(sr);

	for (int num_patients = 0; num_patients < 100000; num_patients++)
	{
		Patient p(probs);

		std::vector<Patient> hold_patients; 
		for (int i = 0; i < NUMBER_OF_STRATEGIES; i++)
		{
			hold_patients.push_back(p);
			Simulation_Processes::patient_initial_statistics(p, hold_results[i]);
		}

		Simulation_Processes::Strategy_One::run_six_food_elimination(
			hold_patients[SIX_FOOD_ELIMINATION], hold_results[SIX_FOOD_ELIMINATION]);
		Simulation_Processes::Strategy_Two::run_strategy(
			hold_patients[INITIAL_ONLY_DAIRY_AND_WHEAT], hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT]);
	}

	hold_results[0].print_results("six_food_elimination");
	hold_results[1].print_results("strategy_two_eliminate_wheat_dairy_initial");

	clock_t end_time = clock();
	double total_time = double(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTotal simulation time was %f seconds\n", total_time);

    return 0;
}

