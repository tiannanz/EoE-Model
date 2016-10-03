#include "stdafx.h"
#include "metropolishastings.h"
#include "random_aa.h"

/*
Define the beta distribution as 
alpha = outcomes, and beta = n - outcomes. 
In order to implement into the model, because
boost does not have a native RNG for beta functions, 
we instead use that if x = gamma(a, c) and 
y = gamma(b, c), then x / (x + y ) = beta ( a, b ). 
*/


Metropolis_Hastings::Metropolis_Hastings()
{
	warmup_sets = 1000; 
	hold_parameter_sets = {};
}

density Metropolis_Hastings::generate_new_set(Probabilities &p)
{
	p.p_trigger_milk_dairy = beta_rng(10, 10);
	p.p_trigger_wheat = beta_rng(10, 10);
	p.p_trigger_eggs = beta_rng(10, 10);
	p.p_trigger_legume_soy = beta_rng(10, 10);
	p.p_trigger_seafood = beta_rng(10, 10);
	p.p_trigger_nuts = beta_rng(10, 10);
	return calculate_density(p);
}

density Metropolis_Hastings::calculate_density(Probabilities &p)
{
	double f_x = p.p_trigger_milk_dairy*p.p_trigger_wheat*p.p_trigger_eggs*
		p.p_trigger_legume_soy*p.p_trigger_seafood*p.p_trigger_nuts;
	return f_x; 
}

void Metropolis_Hastings::update_probability_array(Probabilities &p,
	const int &psa_iteration, const bool &is_it_a_duplicate)
{
	int array_size = NUMBER_OF_ALLERGIES;
	hold_parameter_sets[psa_iteration][MILK_OR_DAIRY] = p.p_trigger_milk_dairy;
	hold_parameter_sets[psa_iteration][WHEAT] = p.p_trigger_wheat;
	hold_parameter_sets[psa_iteration][EGG] = p.p_trigger_eggs;
	hold_parameter_sets[psa_iteration][LEGUMES_SLASH_SOY] = p.p_trigger_legume_soy;
	hold_parameter_sets[psa_iteration][SEAFOOD] = p.p_trigger_seafood;
	hold_parameter_sets[psa_iteration][NUTS] = p.p_trigger_nuts;
	hold_parameter_sets[psa_iteration][array_size] = is_it_a_duplicate;
}

check_duplicate Metropolis_Hastings::update_probability_object(Probabilities &p,
	const int &psa_iteration)
{
	int array_size = NUMBER_OF_ALLERGIES;
	p.p_trigger_milk_dairy = hold_parameter_sets[psa_iteration][MILK_OR_DAIRY];
	p.p_trigger_wheat = hold_parameter_sets[psa_iteration][WHEAT];
	p.p_trigger_eggs = hold_parameter_sets[psa_iteration][EGG];
	p.p_trigger_legume_soy = hold_parameter_sets[psa_iteration][LEGUMES_SLASH_SOY];
	p.p_trigger_seafood = hold_parameter_sets[psa_iteration][SEAFOOD];
	p.p_trigger_nuts = hold_parameter_sets[psa_iteration][NUTS];
	return hold_parameter_sets[psa_iteration][array_size];
}

store_parameter_sets_from_psa Metropolis_Hastings::run_algorithm()
{
	bool not_duplicate = false; 
	bool duplicate = true; 
	Probabilities p;
	update_probability_array(p, 0, not_duplicate);
	if (Parameters::PSA_ITERATIONS == 0) return; 
	density current_fallback_density = generate_new_set(p); /*initialization*/
	for (int i = 0; i < (Parameters::PSA_ITERATIONS + warmup_sets); i++)
	{
		int psa_iteration = i - warmup_sets; 
		bool start_storing = (psa_iteration > 0);
		density possible_density = generate_new_set(p);
		acceptance_ratio ar = possible_density / current_fallback_density;
		if (ar >= 1)
		{
			/*new comparison point*/
			current_fallback_density = possible_density;
			if (start_storing) update_probability_array(p, psa_iteration+1, not_duplicate);
		}
		else
		{
			/*accept with probability of the acceptance ratio*/
			double x = uniform_rng();
			if (x < ar) /*accept*/
			{
				current_fallback_density = possible_density; 
				if (start_storing)
				{
					update_probability_array(p, psa_iteration+1, not_duplicate);
				}
			}
			else
			{
				update_probability_array(p, psa_iteration+1, duplicate);
			}
		}
	}
}

