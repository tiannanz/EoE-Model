#include "stdafx.h"
#include "strategies.h"
#include "simulation.h"
#include "results.h"
#include <ctime>
#include <fstream>

#define SIMULATION_POPULATION 100000
#define PRINT_RAW_DATA true

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

	std::ofstream patient_level_info_S1;
	patient_level_info_S1.open("../Results/patient_info_s1.csv");
	std::ofstream patient_level_info_S2;
	patient_level_info_S2.open("../Results/patient_info_s2.csv");

	if (PRINT_RAW_DATA)
	{
		patient_level_info_S1 <<
			"patient ID," << "number of endoscopies," << "milk or dairy," <<
			"wheat," << "egg," << "legumes and soy," << "seafood," <<
			"nuts,";

		patient_level_info_S2 <<
			"patient ID," << "number of endoscopies," << "milk or dairy," <<
			"wheat," << "egg," << "legumes and soy," << "seafood," <<
			"nuts,";
	}

	for (int num_patients = 0; num_patients < SIMULATION_POPULATION; num_patients++)
	{
		Patient p(probs);
		for (int i = 0; i < NUMBER_OF_STRATEGIES; i++)
		{
			Simulation_Processes::patient_initial_statistics(p, hold_results[i]);
		}
		Simulation_Processes::Strategy_One::run_six_food_elimination(
			p, hold_results[SIX_FOOD_ELIMINATION]);

		if (PRINT_RAW_DATA)
		{
			if (p.how_many_allergies != UNKNOWN_ALLERGY)
			{
				patient_level_info_S1 << "\n" <<
					num_patients << "," << p.number_of_endoscopies << ",";
				for (int ii = 0; ii < NUMBER_OF_ALLERGIES; ii++)
				{
					patient_level_info_S1 << p.patient_allergies[ii] << ",";
				}
			}
		}
		p.reset_patient();

		Simulation_Processes::Strategy_Two::run_strategy(
				p, hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT]);

		if (PRINT_RAW_DATA)
		{
			if (p.how_many_allergies != UNKNOWN_ALLERGY)
			{
				patient_level_info_S2 << "\n" <<
					num_patients << "," << p.number_of_endoscopies << ",";

				for (int ii = 0; ii < NUMBER_OF_ALLERGIES; ii++)
				{
					patient_level_info_S2 << p.patient_allergies[ii] << ",";
				}
			}
		}

	}

	patient_level_info_S1.close();
	patient_level_info_S2.close();

	hold_results[SIX_FOOD_ELIMINATION].print_results("six_food_elimination");
	hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT].print_results("strategy_two_eliminate_wheat_dairy_initial");

	clock_t end_time = clock();
	double total_time = double(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTotal simulation time was %f seconds\n", total_time);

    return 0;
}

