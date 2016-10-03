#include "stdafx.h"
#include "strategies.h"
#include "simulation.h"
#include "results.h"
#include <ctime>
#include <fstream>
#include "global_parameters.h"
#include "metropolishastings.h"

int main()
{	
	clock_t start_time = clock();
	printf("Beginning EoE simulation.");

	Metropolis_Hastings run_psa;
	run_psa.run_algorithm();

	std::ofstream patient_level_info_S1;
	patient_level_info_S1.open("../Results/patient_info_s1.csv");
	std::ofstream patient_level_info_S2;
	patient_level_info_S2.open("../Results/patient_info_s2.csv");
	patient_level_info_S1 <<
		"patient ID," << "number of endoscopies," << "milk or dairy," <<
		"wheat," << "egg," << "legumes and soy," << "seafood," <<
		"nuts,";
	patient_level_info_S2 <<
		"patient ID," << "number of endoscopies," << "milk or dairy," <<
		"wheat," << "egg," << "legumes and soy," << "seafood," <<
		"nuts,";

	Probabilities probs;

	for (int i = 0; i <= Parameters::PSA_ITERATIONS; i++)
	{
		bool duplicate_param_set = run_psa.update_probability_object(probs, i);
		if (duplicate_param_set) continue; //will need to update here
		std::string attach_psa_param = "_psa_" + std::to_string(i)+".csv";
		Strategy_Results sr;
		std::vector<Strategy_Results> hold_results;
		for (int i = 0; i < NUMBER_OF_STRATEGIES; i++) hold_results.push_back(sr);
		for (int num_patients = 0; num_patients < Parameters::SIMULATION_POPULATION; num_patients++)
		{
			Patient p(probs);
			for (int i = 0; i < NUMBER_OF_STRATEGIES; i++)
			{
				Simulation_Processes::patient_initial_statistics(p, hold_results[i]);
			}
			Simulation_Processes::Strategy_One::run_six_food_elimination(
				p, hold_results[SIX_FOOD_ELIMINATION]);

			if (Parameters::PRINT_RAW_DATA && i == 0)
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

			if (Parameters::PRINT_RAW_DATA)
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
		hold_results[SIX_FOOD_ELIMINATION].print_results("six_food_elimination"+attach_psa_param);
		hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT].print_results("strategy_two_eliminate_wheat_dairy_initial"+attach_psa_param);
	}

	patient_level_info_S1.close();
	patient_level_info_S2.close();

	clock_t end_time = clock();
	double total_time = double(end_time - start_time) / CLOCKS_PER_SEC;
	printf("\nTotal simulation time was %f seconds\n", total_time);

    return 0;
}

