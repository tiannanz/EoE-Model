#pragma once
#include "stdafx.h"
#include "simulation.h"
#include "global_parameters.h"

typedef double acceptance_ratio;
typedef void store_parameter_sets_from_psa;
typedef double density; 
typedef bool check_duplicate; 

class Metropolis_Hastings
{
public:
	Metropolis_Hastings(); //constructor just for warmup sets right now 
	store_parameter_sets_from_psa run_algorithm(); 
	void update_probability_array(Probabilities &p,
		const int &psa_iteration, const bool &is_it_a_duplicate);
	check_duplicate update_probability_object(Probabilities &p, const int &psa_iteration);
private:
	acceptance_ratio generate_new_set(Probabilities &p);
	density calculate_density(Probabilities &p);
	std::array<std::array<double, (NUMBER_OF_ALLERGIES+Parameters::DUPLICATE)>, 
		(Parameters::PSA_ITERATIONS+1)> hold_parameter_sets;
	int warmup_sets; //sets before we actually start storing parameters
};