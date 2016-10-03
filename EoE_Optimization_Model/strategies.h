#pragma once
#include "stdafx.h"
#include <vector>

typedef bool did_patient_have_reaction;
typedef bool still_have_allergy;
class Patient; 
class Strategy_Results;
enum possible_allergies;

enum Number_of_Strategies
{
	SIX_FOOD_ELIMINATION,
	INITIAL_ONLY_DAIRY_AND_WHEAT,
	NUMBER_OF_STRATEGIES
};

namespace Simulation_Processes
{
	did_patient_have_reaction endoscopize(Patient &p);
	void eliminate_specific_food(possible_allergies const &allergy, Patient &p);
	still_have_allergy eliminate_all_foods(Patient &p);
	void add_back_food(possible_allergies const &food_to_add, Patient &p); 
	void finish_strategy(Patient &p, Strategy_Results &sr, std::string const &strategy);

	namespace Strategy_One
	{
		void run_six_food_elimination(Patient &p, Strategy_Results &sr);
	}
	namespace Strategy_Two
	{
		void run_strategy(Patient &p, Strategy_Results &sr); 
	}

	void patient_initial_statistics(
		Patient const &p, Strategy_Results &sr);
}