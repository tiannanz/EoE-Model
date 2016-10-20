#include "stdafx.h"
#include "strategies.h"
#include "simulation.h"
#include "results.h"
#include <array>
#include <fstream>
#include "metropolishastings.h"
#include "global_parameters.h"

did_patient_have_reaction Simulation_Processes::endoscopize(Patient &p)
{
	p.number_of_endoscopies++;
	for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
	{
		if ((p.patient_allergies[i] == true) &&
			(p.current_foods_in_diet[i] == true)) return true;
		
	}
	if (p.how_many_allergies == UNKNOWN_ALLERGY) return true; 
	return false;
}


still_have_allergy Simulation_Processes::eliminate_all_foods(Patient &p)
{
	for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
	{
		p.current_foods_in_diet[i] = false; 
	}
	return(endoscopize(p)); //now make sure allergy is gone
}

void Simulation_Processes::eliminate_specific_food(possible_allergies const &allergy, Patient &p)
{
	p.current_foods_in_diet[allergy] = false; 
}

void Simulation_Processes::add_back_food(possible_allergies const &food_to_add, Patient &p)
{
	bool reaction; 
	p.current_foods_in_diet[food_to_add] = true; 
	reaction = endoscopize(p);
	if (reaction)
	{
		eliminate_specific_food(food_to_add, p);
		p.allergies_found_in_strategy[food_to_add] = true; 
		p.number_of_endoscopies++; //they check again to make sure they are clear
	}
}

void Simulation_Processes::finish_strategy(Patient &p, Strategy_Results &sr, 
	std::string const &strategy)
{
	for (int allergy = 0; allergy < NUMBER_OF_ALLERGIES; allergy++)
	{
		if (p.patient_allergies[allergy] == true) sr.patients_with_allergy[allergy]++;
		if (p.patient_allergies[allergy] != p.allergies_found_in_strategy[allergy])
		{
			printf("WHERE IS THIS TOTALLY RANDOM ERROR HELP");
		}
	}
	sr.total_egd += p.number_of_endoscopies;
	sr.total_egd_without_unknown += p.number_of_endoscopies;
}

void Simulation_Processes::Strategy_One::run_six_food_elimination(
	Patient &p, Strategy_Results &sr)
{
	bool p_has_unknown_allergy = false; 
	p_has_unknown_allergy = eliminate_all_foods(p);
	if (p_has_unknown_allergy)
	{
		sr.total_egd += p.number_of_endoscopies;
		return;
	}
	add_back_food(NUTS, p); 
	add_back_food(SEAFOOD, p);
	add_back_food(LEGUMES_SLASH_SOY, p);
	add_back_food(EGG, p);
	add_back_food(WHEAT, p);
	add_back_food(MILK_OR_DAIRY, p); 
	finish_strategy(p, sr, "strategy six_food_elim");
}

void Simulation_Processes::Strategy_Two::run_strategy(
	Patient &p, Strategy_Results &sr)
{
	bool reaction = false; 
	eliminate_specific_food(WHEAT, p);
	eliminate_specific_food(MILK_OR_DAIRY, p);
	reaction = endoscopize(p); 
	if (reaction) Strategy_One::run_six_food_elimination(p, sr);
	else if (!reaction)
	{
		add_back_food(MILK_OR_DAIRY, p); 
		add_back_food(WHEAT, p); 
		finish_strategy(p, sr, "strategy elim initial two food");
	}
}

void Simulation_Processes::Strategy_Three::run_strategy(
	Patient &p, Strategy_Results &sr)
{
	return;
}

void Simulation_Processes::patient_initial_statistics(
	Patient const &p, Strategy_Results &sr)
{
	sr.total_patients++;
	if (p.real_num_allergies == UNKNOWN_ALLERGY)
	{
		sr.patients_with_unknown++;
		return;
	}
	sr.patients_we_simulate_whole_strategy++;
	sr.patients_with_multiple_allergies[p.real_num_allergies - 1]++;
}


void Simulation_Processes::print_patient_level_info_headers()
{
	for (int i = 1; i <= NUMBER_OF_STRATEGIES; i++)
	{
		std::ofstream file;
		std::string attach = std::to_string(i);

		file.open("../Results/patient_info_s" + attach + ".csv");
		file <<
			"patient ID," << "number of endoscopies," << "milk or dairy," <<
			"wheat," << "egg," << "legumes and soy," << "seafood," <<
			"nuts";
		file.close();
	}
}

void Simulation_Processes::run_PSA_simulation()
{
	Metropolis_Hastings run_psa;
	run_psa.run_algorithm();

	for (int i = 0; i < Parameters::PSA_ITERATIONS; i++)
	{
		bool duplicate_param_set = run_psa.duplicate[i];
		if (duplicate_param_set) continue;

		std::string attach_psa_param = "_psa_" + std::to_string(i) + ".csv";
		Strategy_Results sr;

		std::vector<Strategy_Results> hold_results;
		for (int j = 0; j < NUMBER_OF_STRATEGIES; j++) hold_results.push_back(sr);
		for (int num_patients = 0; num_patients < Parameters::SIMULATION_POPULATION; num_patients++)
		{
			Patient p(run_psa.hold_probabilities[i]);
			for (int i = 0; i < NUMBER_OF_STRATEGIES; i++) Simulation_Processes::patient_initial_statistics(p, hold_results[i]);

			/*NOW WE SET THE STRATEGIES*/

			Simulation_Processes::Strategy_One::run_six_food_elimination(
				p, hold_results[SIX_FOOD_ELIMINATION]);
			p.reset_patient();
			Simulation_Processes::Strategy_Two::run_strategy(
				p, hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT]);
			p.reset_patient();
		}
		/*ACTUALLY PRINT THE RESULTS OF THE STRATEGY*/
		hold_results[SIX_FOOD_ELIMINATION].print_results("six_food_elimination_" + attach_psa_param + ".csv");
		hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT].print_results("strategy_two_eliminate_wheat_dairy_initial_" + attach_psa_param + ".csv");
	}
}

void Simulation_Processes::run_base_case_simulation()
{
	Strategy_Results sr;
	Probabilities probs;
	std::vector<Strategy_Results> hold_results;
	for (int j = 0; j < NUMBER_OF_STRATEGIES; j++) hold_results.push_back(sr);
	for (int num_patients = 0; num_patients < Parameters::SIMULATION_POPULATION; num_patients++)
	{
		Patient p(probs);
		/*record intial patient statistics*/
		for (int i = 0; i < NUMBER_OF_STRATEGIES; i++) Simulation_Processes::patient_initial_statistics(p, hold_results[i]);

		/*NOW WE SET THE STRATEGIES*/

		Simulation_Processes::Strategy_One::run_six_food_elimination(
			p, hold_results[SIX_FOOD_ELIMINATION]);

		/*in case you awnt to debug / print EVERYTHING and break your excel*/
		if (Parameters::PRINT_RAW_DATA)
		{
			std::ofstream file;
			file.open("../Results/patient_info_s1.csv", std::ios::app);
			if (p.how_many_allergies != UNKNOWN_ALLERGY)
			{
				file << "\n" <<
					num_patients << "," << p.number_of_endoscopies << ",";
				for (int ii = 0; ii < NUMBER_OF_ALLERGIES; ii++)
				{
					file << p.patient_allergies[ii] << ",";
				}
			}
			file.close();
		}

		p.reset_patient();

		Simulation_Processes::Strategy_Two::run_strategy(
			p, hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT]);

		if (Parameters::PRINT_RAW_DATA)
		{
			std::ofstream file;
			file.open("../Results/patient_info_s2.csv", std::ios::app);
			if (p.how_many_allergies != UNKNOWN_ALLERGY)
			{
				file << "\n" <<
					num_patients << "," << p.number_of_endoscopies << ",";
				for (int ii = 0; ii < NUMBER_OF_ALLERGIES; ii++)
				{
					file << p.patient_allergies[ii] << ",";
				}
			}
			file.close();
		}

		p.reset_patient();
	}
	/*ACTUALLY PRINT THE RESULTS OF THE STRATEGY*/
	hold_results[SIX_FOOD_ELIMINATION].print_results("six_food_elimination_base_case.csv");
	hold_results[INITIAL_ONLY_DAIRY_AND_WHEAT].print_results("strategy_two_eliminate_wheat_dairy_initial_base_case.csv");
}

