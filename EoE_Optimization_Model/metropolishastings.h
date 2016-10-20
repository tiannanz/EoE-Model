#pragma once
#include "stdafx.h"
#include "simulation.h"
#include <vector>

typedef double acceptance_ratio;
typedef void store_parameter_sets_from_psa;
typedef double density; 
typedef bool check_duplicate; 

class Metropolis_Hastings
{
public:
	Metropolis_Hastings(); //constructor just for warmup sets right now 
	store_parameter_sets_from_psa run_algorithm(); 
	std::vector<Probabilities> hold_probabilities;
	std::vector<bool> duplicate;
private:
	void update_probability_vector(Probabilities const &p,
		const bool &is_it_a_duplicate);
	acceptance_ratio generate_new_set(Probabilities &p);
	density calculate_density(Probabilities &p);
	int warmup_sets; //sets before we actually start storing parameters
};