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
	std::array<Strategy_Results, 4> hold_results = {
		sr, sr, sr, sr
	};

	for (int num_patients = 0; num_patients < 1000000; num_patients++)
	{
		static int total_patients = 1;
		total_patients++;
		if (total_patients % 100000 == 0) printf("\nSimulated a total of %d patients...", total_patients);
		for (int i = 0; i < 4; i++) hold_results[i].total_patients++;
		Patient p(probs); 
		std::array<Patient, 4> hold_patients = {
				p, p, p, p };
		Simulation_Processes::Strategy_One::run_six_food_elimination(
			hold_patients[0], hold_results[0]);
		Simulation_Processes::Strategy_Two::run_strategy(
			hold_patients[1], hold_results[1]);
	}

	hold_results[0].print_results("six_food_elimination");
	hold_results[1].print_results("strategy_two_eliminate_wheat_dairy_initial");

	clock_t end_time = clock();
	double total_time = double(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTotal simulation time was %f seconds\n", total_time);

    return 0;
}

