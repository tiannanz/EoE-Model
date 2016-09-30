#include "stdafx.h"
#include "simulation.h"
#include "random_aa.h"
#include <vector>

Probabilities::Probabilities()
{
	//initialize public probailities
	p_eliminate_allergies_still_unknown = .15;
	p_one_allergy = .5; 
	p_two_allergies = .25; 
	p_three_or_more = 1 - p_one_allergy - p_two_allergies; 

	/*private probabilities for individual allergies*/
	p_trigger_eggs = .25; 
	p_trigger_legume_soy = .15; 
	p_trigger_milk_dairy = .45; 
	p_trigger_wheat = .65;
	p_trigger_seafood = .14; 
	p_trigger_nuts = .1; 

	//initialize sorted array

	probability_of_rxn = { 
		p_trigger_milk_dairy, p_trigger_eggs, 
		p_trigger_wheat, p_trigger_legume_soy, 
		p_trigger_seafood, p_trigger_nuts };
}

Patient::Patient(Probabilities const &probs)
{
	how_many_allergies = UNINITIALIZED; 
	number_of_endoscopies = 0;
	real_num_allergies = 0; 
	//initialize current foods as all true 
	current_foods_in_diet = {
		true, true, true,
		true, true, true
	};
	patient_allergies = {
		false, false, false,
		false, false, false
	};
	allergies_found_in_strategy = {
		false, false, false,
		false, false, false
	};
	calculate_number_of_allergies(probs);
	generate_patient_allergies(probs);
}

void Patient::calculate_number_of_allergies(Probabilities const &probs)
{
	double check_for_unknown = uniform_rng();
	if (check_for_unknown < probs.p_eliminate_allergies_still_unknown)
	{
		how_many_allergies = UNKNOWN_ALLERGY; 
		return; 
	}
	double figure_out_how_many_allergies = uniform_rng(); 
	double p1a = probs.p_one_allergy; /*prob-one-allergy = p1a*/
	double p2a = p1a + probs.p_two_allergies; 

	if (figure_out_how_many_allergies < p1a) how_many_allergies = ONE;
	else if (figure_out_how_many_allergies < p2a) how_many_allergies = TWO;
	else how_many_allergies = THREE_OR_MORE;
	//figure out how many we have if we're above 3
	//currently assuming equal probability of each scenario
	real_num_allergies = how_many_allergies;
	if (how_many_allergies == THREE_OR_MORE)
	{
		int z = uniform_int_rng(THREE_OR_MORE, NUMBER_OF_ALLERGIES);
		real_num_allergies = z;
	}
	/*debug aa*/
	if (how_many_allergies == UNINITIALIZED) printf("Didn't assign number of allergies");
}

void Patient::generate_patient_allergies(Probabilities const &probs)
{
	//exit if we have an unknown allergy, don't waste time and peace out
	if (real_num_allergies == UNKNOWN_ALLERGY) return;
	/*
	generate an initial set of allergies
	hold the indexes of the allergies that you do generate 
	in terms of sorted probability set
	*/
	int temp_num_allergies = 0; 
	std::vector<int> index_of_allergies = {};

	for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
	{
		double x = uniform_rng();
		if (x < probs.probability_of_rxn[i])
		{
			patient_allergies[i] = true;
			index_of_allergies.push_back(i); //store the index
			temp_num_allergies++; //add to temp number of allergies
		}
		else patient_allergies[i] = false; 
	}

	if (temp_num_allergies == real_num_allergies) return; 

	else if (temp_num_allergies > real_num_allergies)
	{
		//we want to remove some so first calculate how many we have 
		int extra_allergies = temp_num_allergies - how_many_allergies;
		for (int extra = 1; extra <= extra_allergies; extra++)
		{
			//generates the number of allergy we want to remove
			//we generate from 0 to the size of the 
			//(vector that holds "true" index values) - 1 
			int x = uniform_int_rng(0, index_of_allergies.size()-1);
			int index_in_array = index_of_allergies[x]; 
			patient_allergies[index_in_array] = false; 
			//important that we now erase!! the xth indexed element in the vector 
			index_of_allergies.erase(index_of_allergies.begin() + x); 
		}
	}

	else if (temp_num_allergies < real_num_allergies)
	{
		//figure out how many allergies you need
		int needed_allergies = real_num_allergies - temp_num_allergies;
		//loop over the number of needed allergies 
		for (int k = 1; k <= needed_allergies; k++)
		{
			//generate a uniform number between 1 and how many allergies
			//you can still get (total - (initial number+allergies added) ) 
			int x = uniform_int_rng(1, NUMBER_OF_ALLERGIES - 
				temp_num_allergies);
			//count until you can add back that "number" 
			int counter_to_x = 0; 
			for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
			{
				/*remember x between 1 and y, so adding 1 to counter first is not bug*/
				if (patient_allergies[i] == false) counter_to_x++; 
				if (counter_to_x == x)
				{
					patient_allergies[i] = true;
					temp_num_allergies++; 
					break;
				}	
			}
		}
	}
}


