#include "stdafx.h"
#include "strategies.h"
#include "simulation.h"
#include "results.h"
#include <iostream>
#include <array>

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
			std::cout << "Failure in " << strategy << std::endl;
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

